#pragma once
//Seba

#include <regex>

#include "interpreter_functions\arythmetics.hpp"
#include "interpreter_functions\Interpreter_syntax.hpp"
#include "interpreter_functions\jumps.hpp"
#include "PCB.hpp"

std::vector<std::string> getArgs(PCB *pcb, int argNum){
    std::vector<std::string> vector;
    for (int i = 0; i < argNum; i++){
        char read;
        std::string str="";
        int j = pcb->getCommandCounter();
        do
        {
            read = readFromRam(pcb, j+3);
            str.append(1,read);
            j++;
        } while (read != ' ');        
        vector.push_back(str);
    }
    return vector;
}

bool interprate(PCB *pcb){
    std::string command = "";
    command.append(1, readFromRam(pcb, pcb->getCommandCounter()));
    command.append(1, readFromRam(pcb, pcb->getCommandCounter()+1));

    int takenBytes = 0;
    if (command.size() == 2){
        takenBytes = 2;
    }else{
        return 0;
    }
    
    std::vector<std::string> args;

    if(command == "HT") //koniec programu
    {
        pcb->setCommandCounter(pcb->getCommandCounter()+takenBytes);
        return 0;
    }
    else if (command == "AD") //add
    {
        args = getArgs(pcb, 2);
        return ADD(pcb, args[0], args[1]);
    }
    else if (command == "SB") //substarct
    {
        args = getArgs(pcb, 2);
        return SUB(pcb, args[0], args[1]);
    }
    else if (command == "MU") //multiply
    {
        args = getArgs(pcb, 2);
        return MUL(pcb, args[0], args[1]);
    }
    else if (command == "JP") //unconditional jump
    {
        args = getArgs(pcb, 1);
        return jump(pcb, args[0]);
    }
    else if (command == "JZ")
    {
        args = getArgs(pcb, 2);
        return jumpIf0(pcb, args[0], args[1]);
    }
    else if (command == "JN")
    {
        args = getArgs(pcb, 2);
        return jumpIfN0(pcb, args[0], args[1]);
    }
    
    
    
    
}