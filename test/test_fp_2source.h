//
// Created by 58413 on 2023/10/19.
//

#ifndef MEGUMIN_TEST_FP_2SOURCE_H
#define MEGUMIN_TEST_FP_2SOURCE_H

TEST_CASE("Floating point 2-source data processing", "[fadd]") {
    MachineState s{};
    s.fp.set64(1, bits{(double) -3.5});
    InstructionExecution execution{s};

    Program prog = megumin::aarch64_asm("fadd d2, d1, d1; fadd d2, d1, d3").value();
    auto& inst = prog.get_instruction_const(0);
    auto& inst2 = prog.get_instruction_const(1);
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == -7.0);

    s.fp.set64(1, bits{(double) 1.5});
    s.fp.set64(3, bits{(double) -1.5});
    execution.execute(inst2);
    REQUIRE(s.fp.get(64, 2).as_f64() == 0);

//    s.fp.set64(1, bits{std::numeric_limits<double>::infinity()});
//    execution.execute(inst);
//    REQUIRE(s.fp.get(64, 2).as_f64() == -std::numeric_limits<double>::infinity());
//
//    s.fp.set64(1, bits{(double) 0});
//    execution.execute(inst);
//    REQUIRE(s.fp.get(64, 2).as_f64() == (double)-0.0);
}

TEST_CASE("Floating point 2-source data processing", "[fmul]") {
    MachineState s{};
    s.fp.set64(1, bits{(double) -3.5});
    InstructionExecution execution{s};

    Program prog = megumin::aarch64_asm("fmul d2, d1, d1; fmul d2, d1, d3").value();
    auto& inst = prog.get_instruction_const(0);
    auto& inst2 = prog.get_instruction_const(1);
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 2).as_f64() == 3.5 * 3.5);

    s.fp.set64(1, bits{(double) 1.5});
    s.fp.set64(3, bits{(double) -2.5});
    execution.execute(inst2);
    REQUIRE(s.fp.get(64, 2).as_f64() == -1.5 * 2.5);

//    s.fp.set64(1, bits{std::numeric_limits<double>::infinity()});
//    execution.execute(inst);
//    REQUIRE(s.fp.get(64, 2).as_f64() == -std::numeric_limits<double>::infinity());
//
//    s.fp.set64(1, bits{(double) 0});
//    execution.execute(inst);
//    REQUIRE(s.fp.get(64, 2).as_f64() == (double)-0.0);
}

#endif //MEGUMIN_TEST_FP_2SOURCE_H
