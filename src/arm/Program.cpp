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

    void Program::add_basic_block(const std::vector<arm::Instruction> &bb, const std::vector<int> &out_edges) {
        instructions.push_back(bb);
        out_connections.push_back(out_edges);
    }

    void Program::add_nop_basic_block() {
        instructions.emplace_back();
        out_connections.emplace_back();
    }

    void Program::calc_in_connections_from_out_connections() {
        assert(out_connections.size() == instructions.size());
        in_connections.resize(instructions.size());

        for (int i = 0; i < instructions.size(); i++) {
            in_connections[i].clear();
        }

        for (int i = 0; i < instructions.size(); i++) {
            for (int to: out_connections[i]) {
                in_connections[to].push_back(i);
            }
        }
    }

    int Program::get_size() const {
        return calculate_size();
//        return instruction_size;
    }

    int Program::get_basic_block_size() const {
        return instructions.size();
    }

    int Program::get_instruction_size(int basic_block_id) const {
        return static_cast<int>(instructions[basic_block_id].size());
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

            if (state.pc >= instructions[state.current_basic_block].size()) {
                // if pc exceeds the current BB size, goto next BB
                state.current_basic_block++;
                state.pc = 0;
            }
        }
    }

    Program::Program(int basic_block_size) {
        for (int i = 0; i < basic_block_size; i++) {
            instructions.emplace_back();
            out_connections.emplace_back();
            in_connections.emplace_back();
            def_ins.emplace_back();
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

    void Program::set_instruction(Program::ProgramPosition position, const Instruction &instruction) {
        set_instruction(position.basic_block_id, position.index, instruction);
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

    void Program::swap_instructions(ProgramPosition p1, ProgramPosition p2) {
        swap_instructions(p1.basic_block_id, p1.index, p2.basic_block_id, p2.index);
    }

    const Instruction& Program::get_instruction_const(int basic_block_id, int index) const {
        megumin::megumin_assert(basic_block_id < instructions.size());
        megumin::megumin_assert(index < instructions[basic_block_id].size());
        return instructions[basic_block_id][index];
    }

    const Instruction& Program::get_instruction_const(ProgramPosition position) const {
        return get_instruction_const(position.basic_block_id, position.index);
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

    void Program::print_with_def_ins(std::ostream &os) const {
        InstructionPrinter printer{os};

        auto bb_count = instructions.size();
        for (int i = 0; i < bb_count; i++) {
            os << "BB" << i << ":\n";

            int instruction_size = static_cast<int>(instructions[i].size());
            def_ins[i][0].print_one_line(os);
            os << std::endl;
            for (int j = 0; j < instruction_size; j++) {
                printer.visit_instruction(instructions[i][j]);
                os << std::endl;
                def_ins[i][j + 1].print_one_line(os);
                os << std::endl;
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

    bool Program::is_all_integral_instructions_except_branch() const {
        for (const auto & i : instructions) {
            for (const auto& instruction: i) {
                auto ty = instruction.get_type();
                if (ty == InstructionType::DataProcessingSIMD || ty == InstructionType::SVE
                    || ty == InstructionType::LoadAndStore || ty == InstructionType::Reserved
                    || ty == InstructionType::Unallocated) {
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

    bool Program::is_all_fp_instructions_except_branch() const {
        for (const auto & i : instructions) {
            for (const auto& instruction: i) {
                auto ty = instruction.get_type();
                if (ty == InstructionType::Unallocated || ty == InstructionType::Reserved
                    || ty == InstructionType::LoadAndStore || ty == InstructionType::SVE
                    || ty == InstructionType::DataProcessingImm || ty == InstructionType::DataProcessingReg) {
                    return false;
                }
            }
        }

        return true;
    }

    int Program::get_non_nop_size() const {
        int result = 0;
        for (const auto& bb: instructions) {
            for (const auto& inst: bb) {
                if (!inst.is_nop()) {
                    result++;
                }
            }
        }
        return result;
    }

    const RegSet& Program::get_def_in(Program::ProgramPosition position) const {
        return get_def_in(position.basic_block_id, position.index);
    }

    std::optional<int> Program::has_nop_in_basic_block(int basic_block_id) const {
        for (const auto& instruction: instructions[basic_block_id]) {
            if (instruction.is_nop()) {
                return true;
            }
        }
        return false;
    }

    std::optional<int> Program::random_nop_position_in_basic_block(int basic_block_id, int random_number) const {
        std::vector<int> positions;
        for (int i = 0; i < instructions[basic_block_id].size(); i++) {
            const auto& instruction = instructions[basic_block_id][i];
            if (instruction.is_nop()) {
                positions.push_back(i);
            }
        }
        if (positions.empty()) {
            return {};
        } else {
            int index = random_number % positions.size();
            return positions[index];
        }
    }

    void Program::delete_instruction(int basic_block_id, int index) {
        megumin::megumin_assert(basic_block_id < instructions.size());
        megumin::megumin_assert(index < instructions[basic_block_id].size());
        instructions[basic_block_id].erase(instructions[basic_block_id].begin() + index);
    }

    void Program::delete_instruction(ProgramPosition position) {
        delete_instruction(position.basic_block_id, position.index);
    }

    void Program::insert_instruction(int basic_block_id, int index, const arm::Instruction &instruction) {
        megumin::megumin_assert(basic_block_id < instructions.size());
        megumin::megumin_assert(index <= instructions[basic_block_id].size());

        auto begin = instructions[basic_block_id].begin();
        instructions[basic_block_id].insert(begin + index, instruction);
    }

    void Program::insert_instruction(ProgramPosition position, const arm::Instruction &instruction) {
        insert_instruction(position.basic_block_id, position.index, instruction);
    }

    Program Program::clone_program_all_nop() const {
        Program result{*this};
        int basic_block_size = instructions.size();
        for (int i = 0; i < basic_block_size; i++) {
            int instruction_size = instructions[i].size();
            for (int j = 0; j < instruction_size; j++) {
                result.set_instruction(i, j, arm::Instruction::nop());
            }
        }
        result.calculate_def_ins();
        return result;
    }
}
