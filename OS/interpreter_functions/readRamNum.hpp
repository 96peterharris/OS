#pragma once
#include "Interpreter_syntax.hpp"

std::string readRamNum(PCB *pcb, int addr){
    char read;
    std::string str="";
    do
    {
        read = readFromRam(pcb, addr);
        str.append(1,read);
        addr++;
    } while (read != ' ');
    return str;
}