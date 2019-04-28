#pragma once

#include <list>
#include <string>

#include "typedef.hpp"

namespace ASM {

    class Instruction {
        public:
            RegSet use;
            RegSet def;

            RegSet LVin;
            RegSet LVout;
            bool isInWList = false;

            std::list<Instruction*> predecessors;
            std::list<Instruction*> successors;

            double usageCountWeight;  // used to calculate spilling node

            Instruction();

            Instruction(bool needInsert);  // initialize without insert into instructions

            void setUse(Reg *reg);

            void setUse(OpRand *opRand);

            void setDef(Reg *reg);

            void setDef(OpRand *opRand);

            virtual void assembly() = 0;

            virtual void generateControlFlow(Instruction *nextInstr);
    };

    class ListRegInstruction : public Instruction {
        public:
            std::list<Reg*> registers;

            void assembly();

            virtual std::string opName() = 0;
    };

}
