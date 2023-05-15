#include <iostream>
#include <string>
#include <cassert>

//#include <llvm/MC/MCAssembler.h
#include <llvm/IR/CFG.h>
#include <llvm/MC/MCCodeEmitter.h>
#include <llvm/MC/MCAsmBackend.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCInstrDesc.h>
#include <llvm/MC/MCTargetOptionsCommandFlags.h>
#include <llvm/MC/MCTargetOptions.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCStreamer.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/MC/MCParser/MCTargetAsmParser.h>
#include <llvm/MC/MCMachObjectWriter.h>
#include <llvm/MC/MCObjectFileInfo.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/AArch64TargetParser.h>
//#include <llvm/Target/Mach

using namespace std;
using namespace llvm;

int main() {
    MCCodeEmitter* code_emitter;

    InitializeAllTargetInfos();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllTargets();
    InitializeAllAsmPrinters();

    string error;
    StringRef triple_name = "aarch64";

    Triple the_triple("aarch64");
    const Target* the_target = TargetRegistry::lookupTarget("aarch64", the_triple, error);
    if (error.length() > 0) {
        cerr << error << endl;
    }

    MCInstrInfo* mc_instr_info = the_target->createMCInstrInfo();
    assert(mc_instr_info && "unable to create instr info");

        MCRegisterInfo* mc_register_info = the_target->createMCRegInfo(triple_name);
    assert(mc_register_info && "unable to create register info");

//    cout << "before init options\n";
    MCTargetOptions mc_target_options = mc::InitMCTargetOptionsFromFlags();
//    MCTargetOptions mc_target_options;
//    cout << "after init options\n";
//    mc_target_options.
    MCAsmInfo* mc_asm_info = the_target->createMCAsmInfo(*mc_register_info, triple_name, mc_target_options);
    assert(mc_asm_info && "unable to create asm info");


    MCSubtargetInfo* mc_subtarget_info = the_target->createMCSubtargetInfo(triple_name, "generic", "");
    assert(mc_subtarget_info && "unable to create sub target info");

    MCAsmBackend* mc_asm_backend = the_target->createMCAsmBackend(*mc_subtarget_info, *mc_register_info, mc_target_options);
    assert(mc_asm_backend && "unable to create asm backend");

    SourceMgr source_manager;
    MCContext mc_context{the_triple, mc_asm_info, mc_register_info, mc_subtarget_info, &source_manager, &mc_target_options};
    MCObjectFileInfo mc_object_file_info;
    mc_object_file_info.initMCObjectFileInfo(mc_context, false);

    MCCodeEmitter* mc_code_emitter = the_target->createMCCodeEmitter(*mc_instr_info, mc_context);
    assert(mc_code_emitter && "unable to create code emitter");

//    SmallString<1024> Msg;
//    raw_svector_ostream OS(Msg,);
//    string Msg;
//    raw_string_ostream OS{Msg};


    unique_ptr<MCObjectWriter> mc_object_writer = mc_asm_backend->createObjectWriter(outs());
    assert(mc_object_writer && "unable to create mc object writer");

    MCStreamer* mc_streamer = the_target->createMCObjectStreamer(
            the_triple,
            mc_context,
            unique_ptr<MCAsmBackend>(mc_asm_backend),
            move(mc_object_writer),
            unique_ptr<MCCodeEmitter>(mc_code_emitter),
            *mc_subtarget_info,
            mc_target_options.MCRelaxAll,
            mc_target_options.MCIncrementalLinkerCompatible,
            false
            );
    assert(mc_streamer && "unable to create mc_streamer");

    ErrorOr<unique_ptr<MemoryBuffer>> memory_buffer = MemoryBuffer::getMemBuffer("fmov d1, d2\0\0\0");
    if (memory_buffer.getError()) {
        cerr << memory_buffer.getError() << endl;
    }
    cout << "create memory buffer\n";
//    source_manager
    unsigned int i = source_manager.AddNewSourceBuffer(move(memory_buffer.get()), SMLoc());
    cout << "get mem buffer: " << source_manager.getMemoryBuffer(i) << endl;

    MCAsmParser* mc_asm_parser = createMCAsmParser(source_manager, mc_context, *mc_streamer, *mc_asm_info);
    assert(mc_asm_parser && "unable to create mc_asm_parser");
    cout << "mc_asm_parser: " << mc_asm_parser << endl;

    MCTargetAsmParser* mc_target_asm_parser = the_target->createMCAsmParser(*mc_subtarget_info, *mc_asm_parser, *mc_instr_info, mc_target_options);
    assert(mc_target_asm_parser && "unable tp create mc_target_asm_parser");
    cout << "mc_target_asm_parser: " << mc_target_asm_parser << endl;

    mc_asm_parser->setTargetParser(*mc_target_asm_parser);
//    mc_asm_parser->

    bool run_result = mc_asm_parser->Run(true);
//    int size = Msg.size();
//    cout << size << endl;
}