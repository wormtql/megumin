//
// Created by wormtql on 2023/6/13.
//

#include <iostream>
#include <cctype>
#include <fstream>
#include <set>
#include "BBExtractor.h"
#include "megumin_utils.h"

using namespace std;

namespace arm {
    vector<BasicBlock> arm::BBExtractor::extract_basic_blocks() {
        ifstream f{filename};
        if (!f.is_open()) {
            return {};
        }
        string line;

        vector<BasicBlock> result;
        BasicBlock bb;
        int i = 0;
        bb.set_start(i);
        while (std::getline(f, line)) {
            string line2 = megumin::trim(line);

            if (line2.starts_with(".") || line2.starts_with("//")) {
                i++;
                continue;
            }

            if (is_load_store(line2) || is_branch(line2) || is_other_bb_break(line2)) {
                if (bb.size() > 4) {
                    result.push_back(bb);
                    bb.set_start(i + 1);
                    bb.clear_instructions();
                    cout << endl;

                    if (result.size() == max_bb) {
                        break;
                    }
                }
            } else {
                bb.add_instruction(Instruction::nop());
                cout << line2 << endl;
            }

            i++;
        }

        return std::move(result);
    }

    bool BBExtractor::is_load_store(const string &s) {
        static set<string> opcodes = {
                "str",
                "stp",
                "casp",
                "caspa",
                "caspal",
                "caspl",
                "st1",
                "st2",
                "st3",
                "st4",
                "ld1",
                "ld2",
                "ld3",
                "ld4",
                "ld1r",
                "ld3r",
                "ld2r",
                "ld4r",
                "stg",
                "stzgm",
                "ldg",
                "stzg",
                "stgm",
                "st2g",
                "stz2g",
                "ldgm",
                "stxp",
                "stlxp",
                "ldxp",
                "ldaxp",
                "stxrb",
                "stlxrb",
                "ldxrb",
                "ldaxrb",
                "stxrh",
                "stlxrh",
                "ldxrh",
                "ldaxrh",
                "stllrb",
                "stlrb",
                "ldlarb",
                "ldarb",
                "stllrh",
                "stlrh",
                "ldlarh",
                "ldarh",
                "stllr",
                "stlr",
                "ldlar",
                "ldar",
                "stllr",
                "stlr",
                "ldlar",
                "ldar",
                "stlurb",
                "ldapurb",
                "ldapursb",
                "stlurh",
                "ldapurh",
                "ldapursh",
                "stlur",
                "ldapur",
                "ldapursw",
                "ldr",
                "ldrsw",
                "prfm",
                "stnp",
                "ldnp",
                "stp",
                "ldp",
                "stgp",
                "ldpsw",
                "sturb",
                "ldurb",
                "ldursb",
                "stur",
                "ldur",
                "sturh",
                "ldurh",
                "ldursh",
                "ldursw",
                "prfum",
                "strb",
                "ldrb",
                "ldrsb",
                "strh",
                "ldrh",
                "ldrsh",
                "ldrsw",
                "sttrb",
                "ldtrb",
                "ldtrsb",
                "sttrh",
                "ldtrh",
                "ldtrsh",
                "sttr",
                "ldtr",
                "ldtrsw",
                "ldraa",
                "ldrab"
        };

        string opcode = extract_opcode(s);
        if (opcodes.contains(opcode)) {
            return true;
        } else {
            return false;
        }
    }

    bool BBExtractor::is_branch(const string &s) {
        static set<string> opcodes = {
                "b",
                "bc",
                "svc",
                "hvc",
                "smc",
                "brk",
                "hlt",
                "dcps1",
                "dcps2",
                "dcps3",
                "wfet",
                "wfit",
                "hint",
//                "nop",
                "yield",
                "wfe",
                "wfi",
                "sev",
                "sevl",
                "dgh",
                "esb",
                "psb",
                "csdb",
                "bti",
                "clrex",
                "dsb",
                "dmb",
                "isb",
                "sb",
                "dsb",
                "msr",
                "cfinv",
                "xaflag",
                "axflag",
                "sys",
                "sysl",
                "msr",
                "mrs",
                "br",
                "blr",
                "ret",
                "eret",
                "drps",
                "bl",
                "cbz",
                "cbnz",
                "tbz",
                "tbnz"
        };

        string opcode = extract_opcode(s);

        return opcodes.contains(opcode);
    }

    string BBExtractor::extract_opcode(const string &s) {
        string result;
        for (int i = 0; i < s.length(); i++) {
            if (isalnum(s[i])) {
                result.push_back(s[i]);
            } else {
                break;
            }
        }
        return std::move(result);
    }

    bool BBExtractor::is_other_bb_break(const string &s) {
        static set<string> opcodes = {
                "adrp"
        };

        string opcode = extract_opcode(s);
        return opcodes.contains(opcode);
    }
}

