#ifndef MEGUMIN_TEST_DP_REG_3SOURCE_H
#define MEGUMIN_TEST_DP_REG_3SOURCE_H

#include <megumin_utils.h>
#include <Program.h>
#include <MachineState.h>
#include <visitor/InstructionExecution.h>
#include <iostream>
#include <Program.h>
#include <numeric>

TEST_CASE("Dp reg 3-source data processing", "[umulh test 1]") {
    MachineState s{};
    s.set_gp(64, 1, bits::from_u64(0xfffffff0ffffffff), false);
    s.set_gp(64, 2, bits::from_u64(256), false);
    InstructionExecution execution{s};

    Program prog = megumin::aarch64_asm("umulh x0, x1, x2").value();
    auto& inst = prog.get_instruction_const(0);
    execution.execute(inst);

    REQUIRE(s.gp.get(64, 0).as_u64() == 0xff);
//    REQUIRE(s.fp.get(64, 2).as_f64() == 3.5);
//
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

#endif //MEGUMIN_TEST_DP_REG_3SOURCE_H
