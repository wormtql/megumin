//
// Created by 58413 on 2023/4/18.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <queue>

#include "megumin_utils.h"
#include "Program.h"
#include "RegSet.h"
#include "visitor/InstructionPrinter.h"

using megumin::megumin_assert;

namespace arm {
    std::ostream& operator<<(std::ostream& os, const arm::Program& prog) {
        prog.print(os);

        return os;
    }
}

namespace arm {
    const int MAX_BB = 100;

    int Program::get_size() const {
        return calculate_size();
//        return instruction_size;
    }

    int Program::calculate_size() const {
        int size = 0;
        for (const auto & instruction : instructions) {
            size += instruction.size();
        }
        return size;
    }

    void Program::execute(MachineState &state) const {
        state.current_basic_block = 0;
        state.pc = 0;

        while (state.current_basic_block < instructions.size() && state.pc < instructions[state.current_basic_block].size()) {
            const auto& instruction = instructions[state.current_basic_block][state.pc];
            instruction.execute(state);
        }
    }

    Program::Program(int basic_block_size) {
        for (int i = 0; i < basic_block_size; i++) {
            instructions.emplace_back();
        }
    }

    void Program::add_instruction(int basic_block_id, const Instruction &instruction) {
        instructions[basic_block_id].push_back(instruction);
    }

    void Program::set_instruction(int basic_block_id, int index, const Instruction &instruction) {
        megumin::megumin_assert(basic_block_id < instructions.size());
        megumin::megumin_assert(index < instructions[basic_block_id].size());
        instructions[basic_block_id][index] = instruction;
    }

    void Program::set_instruction_nop(int basic_block_id, int index) {
        megumin::megumin_assert(basic_block_id < instructions.size());
        megumin::megumin_assert(index < instructions[basic_block_id].size());
        instructions[basic_block_id][index].set_as_nop();
    }

    void Program::swap_instructions(int basic_block_id1, int i1, int basic_block_id2, int i2) {
        megumin::megumin_assert(basic_block_id1 < instructions.size() << basic_block_id2 < instructions.size());
        megumin::megumin_assert(i1 < instructions[basic_block_id1].size());
        megumin::megumin_assert(i2 < instructions[basic_block_id2].size());

        Instruction t = instructions[basic_block_id1][i1];
        instructions[basic_block_id1][i1] = instructions[basic_block_id2][i2];
        instructions[basic_block_id2][i2] = t;
    }

    const Instruction& Program::get_instruction_const(int basic_block_id, int index) const {
        megumin::megumin_assert(basic_block_id < instructions.size());
        megumin::megumin_assert(index < instructions[basic_block_id].size());
        return instructions[basic_block_id][index];
    }

    void Program::print(std::ostream& os) const {
        InstructionPrinter printer{os};

        auto bb_count = instructions.size();
        for (int i = 0; i < bb_count; i++) {
            os << i << ":\n";
            for (const auto& j : instructions[i]) {
                printer.visit_instruction(j);
                os << "\n";
            }
        }
    }

    void Program::calc_def_ins_for_basic_block(int basic_block_id) {
        for (int i = 0; i < instructions[basic_block_id].size(); i++) {
            def_ins[basic_block_id][i + 1] = def_ins[basic_block_id][i];
            const Instruction& instruction = instructions[basic_block_id][i];
            auto def_reg = instruction.get_def_register();
            if (def_reg.has_value()) {
                def_ins[basic_block_id][i + 1].add_reg(def_reg.value());
            }
        }
    }

    void Program::calculate_def_ins() {
        def_ins.resize(instructions.size());
        for (int i = 0; i < instructions.size(); i++) {
            def_ins[i].resize(instructions[i].size() + 1);
        }
        def_ins[0][0] = entry_def_ins;

        calc_def_ins_for_basic_block(0);

        // 拓扑顺序
        std::queue<int> q;
        int in_edges[MAX_BB];
        for (int i = 0; i < instructions.size(); i++) {
            in_edges[i] = in_connections[i].size();
        }
        for (int to : out_connections[0]) {
            in_edges[to]--;
            if (in_edges[to] == 0) {
                q.push(to);
            }
        }

        while (!q.empty()) {
            int p = q.front();
            q.pop();
            RegSet bb_entry_def_in{};
            for (int from: in_connections[p]) {
                auto from_size = instructions[from].size();
                const RegSet& from_def_ins = def_ins[from][from_size];
                bb_entry_def_in = bb_entry_def_in.merge(from_def_ins);
            }
            def_ins[p][0] = bb_entry_def_in;
            calc_def_ins_for_basic_block(p);

            for (int to: out_connections[p]) {
                in_edges[to]--;
                if (in_edges[to] == 0) {
                    q.push(to);
                }
            }
        }
    }

    const RegSet& Program::get_def_in(int basic_block_id, int index) const {
        return def_ins[basic_block_id][index];
    }

    void Program::set_entry_def_ins(const RegSet& def_ins0) {
        entry_def_ins = def_ins0;
    }

    RegSet Program::get_minimum_def_ins() const {
        RegSet result{};

        std::queue<int> q;
        size_t in_edges[MAX_BB];
        for (int i = 0; i < instructions.size(); i++) {
            in_edges[i] = in_connections[i].size();
            if (in_edges[i] == 0) {
                q.push(i);
            }
        }

        std::vector<RegSet> gen_regs{instructions.size()};

        while (!q.empty()) {
            int p = q.front();
            q.pop();

            RegSet gen{};
            gen.set_full();
            for (int from: in_connections[p]) {
                gen = gen.intersect(gen_regs[from]);
            }

            for (const auto& instruction : instructions[p]) {
                Reg read_regs[3];
                int read_reg_size = instruction.get_read_registers(read_regs);
                for (int j = 0; j < read_reg_size; j++) {
                    if (!gen.have_reg(read_regs[j])) {
                        result.add_reg(read_regs[j]);
                    }
                }

                auto def_reg = instruction.get_def_register();
                if (def_reg.has_value()) {
                    gen.add_reg(def_reg.value());
                }
            }

            gen_regs[p] = gen;

            for (int to: out_connections[p]) {
                in_edges[to]--;
                if (in_edges[to] == 0) {
                    q.push(to);
                }
            }
        }

        return result;
    }

    void Program::execute(MachineStateS &state) const {
        // todo
//        for (const auto& : instructions) {
//            instruction.execute(state);
//            // todo control flow
//        }
    }

    bool Program::is_all_integral_instructions() const {
        for (const auto & i : instructions) {
            for (const auto& instruction: i) {
                auto ty = instruction.get_type();
                if (ty != InstructionType::DataProcessingImm && ty != InstructionType::DataProcessingReg) {
                    return false;
                }
            }
        }

        return true;
    }

    bool Program::is_all_fp_instructions() const {
        for (const auto & i : instructions) {
            for (const auto& instruction: i) {
                auto ty = instruction.get_type();
                if (ty != InstructionType::DataProcessingSIMD) {
                    return false;
                }
            }
        }

        return true;
    }
}
