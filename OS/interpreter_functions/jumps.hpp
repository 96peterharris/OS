#pragma once

#include "..\PCB.hpp"

/**
 * Unconditional jump
 * @param pcb pointer to running PCB object
 * @param logAddr logic address to the next command's begining 
 * @return true XD
 * @see Interpreter JP command
*/
bool jump(PCB *pcb, std::string logAddr){
    pcb->setCommandCounter(std::stoi(logAddr));
    return 1;
}

/**
 * Jump if given register's value is equal to 0
 * @param pcb pointer to running PCB object
 * @param registerName name of the register that's value will be checked; Accepted: AX; BX; CX; DX
 * @param logAddr logic address to the next command's begining
 * @return true XD
 * @see Interpreter JZ command
*/
bool jumpIf0(PCB *pcb, std::string registerName, std::string logAddr){
    if(
        (registerName == "AX" && pcb->getRegisterPointer()->getA() == 0) ||
        (registerName == "BX" && pcb->getRegisterPointer()->getB() == 0) ||
        (registerName == "CX" && pcb->getRegisterPointer()->getC() == 0) ||
        (registerName == "DX" && pcb->getRegisterPointer()->getD() == 0)
    ){
        pcb->setCommandCounter(std::stoi(logAddr));
    }
    return 1;
}

/**
 * Jump if given register's value is equal to 0
 * @param pcb pointer to running PCB object
 * @param registerName name of the register that's value will be checked; Accepted: AX; BX; CX; DX
 * @param logAddr logic address to the next command's begining
 * @return true XD
 * @see Interpreter JN command
*/
bool jumpIfN0(PCB *pcb, std::string registerName, std::string logAddr){
    if(
        (registerName == "AX" && pcb->getRegisterPointer()->getA() != 0) ||
        (registerName == "BX" && pcb->getRegisterPointer()->getB() != 0) ||
        (registerName == "CX" && pcb->getRegisterPointer()->getC() != 0) ||
        (registerName == "DX" && pcb->getRegisterPointer()->getD() != 0)
    ){
        pcb->setCommandCounter(std::stoi(logAddr));
    }
    return 1;
}