//
// Created by 58413 on 2023/10/17.
//

#ifndef MEGUMIN_TEST_FP_1SOURCE_H
#define MEGUMIN_TEST_FP_1SOURCE_H

#include <megumin_utils.h>
#include <Program.h>
#include <MachineState.h>
#include <visitor/InstructionExecution.h>
#include <iostream>
#include <Program.h>
#include <numeric>

using namespace megumin;
using namespace arm;

TEST_CASE("Floating point 1-source data processing", "[fneg double]") {
    MachineState s{};
    s.fp.set64(1, bits{(double) -3.5});
    InstructionExecution execution{s};

    Program prog = megumin::aarch64_asm("fneg d2, d1").value();
    auto& inst = prog.get_instruction_const(0);
    execution.execute(inst);

    REQUIRE(s.fp.get(64, 2).as_f64() == 3.5);

    s.fp.set64(1, bits{(double) 1.5});
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == -1.5);

    s.fp.set64(1, bits{std::numeric_limits<double>::infinity()});
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == -std::numeric_limits<double>::infinity());

    s.fp.set64(1, bits{(double) 0});
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == (double)-0.0);
}

TEST_CASE("Floating point 1-source data processing", "[fneg float]") {
    MachineState s{};
    s.fp.set32(1, bits{(float) -3.5});
    InstructionExecution execution{s};

    Program prog = megumin::aarch64_asm("fneg s2, s1").value();
    auto& inst = prog.get_instruction_const(0);
    execution.execute(inst);

    REQUIRE(s.fp.get(32, 2).as_f32() == 3.5);

    s.fp.set32(1, bits{(float) 1.5});
    execution.execute(inst);
    REQUIRE(s.fp.get(32, 2).as_f32() == -1.5);

    s.fp.set32(1, bits{std::numeric_limits<float>::infinity()});
    execution.execute(inst);
    REQUIRE(s.fp.get(32, 2).as_f32() == -std::numeric_limits<float>::infinity());

    s.fp.set32(1, bits{(float) 0});
    execution.execute(inst);
    REQUIRE(s.fp.get(32, 2).as_f32() == (float)-0.0);
}

TEST_CASE("Floating point 1-source data processing", "[frintn]") {
    MachineState s{};
    s.fp.set64(1, bits{(double) -3.5});
    InstructionExecution execution{s};

    Program prog = megumin::aarch64_asm("frintn d2, d1").value();
    auto& inst = prog.get_instruction_const(0);
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == -4);

    s.fp.set64(1, bits{(double) 3.5});
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == 4);

    s.fp.set64(1, bits{(double) 0.0});
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == 0.0);

    s.fp.set64(1, bits{(double) 1.2});
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == 1);

    s.fp.set64(1, bits{(double) 1e100 + 0.5});
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == (double) 1e100);

    s.fp.set64(1, bits{(double) -(1e100 + 0.5)});
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == (double) (-1e100 + 1));
}

TEST_CASE("Floating point 1-source data processing", "[fsqrt]") {
    MachineState s{};
    s.fp.set64(1, bits{(double) -3.5});
    InstructionExecution execution{s};

    Program prog = megumin::aarch64_asm("fsqrt d2, d1").value();
    auto& inst = prog.get_instruction_const(0);
    execution.execute(inst);
    REQUIRE(std::isnan(s.fp.get(64, 2).as_f64()));
    REQUIRE(s.fp_exception == FPException::InvalidOp);
}

#endif //MEGUMIN_TEST_FP_1SOURCE_H
