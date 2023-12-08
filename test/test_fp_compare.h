//
// Created by 58413 on 2023/12/8.
//

#ifndef MEGUMIN_TEST_FP_COMPARE_H
#define MEGUMIN_TEST_FP_COMPARE_H

TEST_CASE("Floating point compare", "[fcmp test 1]") {
    MachineState s{};
    s.fp.set64(1, bits{(double) -3.5});
    s.fp.set64(2, bits{(double) -3.5});
    InstructionExecution execution{s};

    Program prog = megumin::aarch64_asm("fcmp d1, d2").value();
    auto& inst = prog.get_instruction_const(0);
    execution.execute(inst);
    REQUIRE(s.p_state.n == false);
    REQUIRE(s.p_state.z == true);
    REQUIRE(s.p_state.c == true);
    REQUIRE(s.p_state.v == false);

//    s.fp.set64(1, bits{(double) 1.5});
//    execution.execute(inst);
//    REQUIRE(s.fp.get(64, 2).as_f64() == -1.5);
//
//    s.fp.set64(1, bits{std::numeric_limits<double>::infinity()});
//    execution.execute(inst);
//    REQUIRE(s.fp.get(64, 2).as_f64() == -std::numeric_limits<double>::infinity());
//
//    s.fp.set64(1, bits{(double) 0});
//    execution.execute(inst);
//    REQUIRE(s.fp.get(64, 2).as_f64() == (double)-0.0);
}

#endif //MEGUMIN_TEST_FP_COMPARE_H
