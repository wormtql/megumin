//
// Created by 58413 on 2023/10/19.
//

#ifndef MEGUMIN_TEST_FP_3SOURCE_H
#define MEGUMIN_TEST_FP_3SOURCE_H

TEST_CASE("Floating point 3-source data processing", "[fmadd]") {
    MachineState s{};
    s.fp.set64(2, bits{(double) 3.5});
    s.fp.set64(3, bits{(double) 0.5});
    s.fp.set64(4, bits{(double) 1});
    InstructionExecution execution{s};

    Program prog = megumin::aarch64_asm("fmadd d1, d2, d3, d4").value();
    auto& inst = prog.get_instruction_const(0);
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 1).as_f64() == 2.75);

    s.fp.set64(2, bits{(double) 1.5});
    s.fp.set64(3, bits{(double) -1.5});
    execution.execute(inst);
    REQUIRE(s.fp.get(64, 1).as_f64() == -1.25);

//    s.fp.set64(1, bits{std::numeric_limits<double>::infinity()});
//    execution.execute(inst);
//    REQUIRE(s.fp.get(64, 2).as_f64() == -std::numeric_limits<double>::infinity());
//
//    s.fp.set64(1, bits{(double) 0});
//    execution.execute(inst);
//    REQUIRE(s.fp.get(64, 2).as_f64() == (double)-0.0);
}

#endif //MEGUMIN_TEST_FP_3SOURCE_H
