#pragma once

#include "..\PCB.hpp"

bool jump(PCB *pcb, std::string logAddr){
    pcb->setCommandCounter(std::stoi(logAddr));
    return 1;
}

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