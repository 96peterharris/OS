#pragma once
//Seba

#include <regex>

#include "interpreter_functions\arythmetics.hpp"
#include "interpreter_functions\Interpreter_syntax.hpp"
#include "interpreter_functions\jumps.hpp"
#include "PCB.hpp"

std::vector<std::string> getArgs(PCB *pcb, int argNum, int &takenBytes){
    std::vector<std::string> vector;
    takenBytes += pcb->getCommandCounter();
    for (int i = 0; i < argNum; i++){
        char read;
        std::string str="";
        do
        {
            read = readFromRam(pcb, takenBytes);
            str.append(1,read);
            takenBytes++;
        } while (read != ' ');
        str.pop_back();        
        vector.push_back(str);
    }
    takenBytes -= pcb->getCommandCounter();
    return vector;
}

bool interprate(PCB *pcb){
    std::string command = "";
    bool ret;
    command.append(1, readFromRam(pcb, pcb->getCommandCounter()));
    command.append(1, readFromRam(pcb, pcb->getCommandCounter()+1));

    int takenBytes = 0;
    if (command.size() == 2){
        takenBytes = 3;
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
        args = getArgs(pcb, 2, takenBytes);
        ret = ADD(pcb, args[0], args[1]);
    }
    else if (command == "SB") //substarct
    {
        args = getArgs(pcb, 2,takenBytes);
        ret = SUB(pcb, args[0], args[1]);
    }
    else if (command == "MU") //multiply
    {
        args = getArgs(pcb, 2, takenBytes);
        ret = MUL(pcb, args[0], args[1]);
    }
    else if (command == "JP") //unconditional jump
    {
        args = getArgs(pcb, 1, takenBytes);
        return jump(pcb, args[0]);
    }
    else if (command == "JZ")
    {
        args = getArgs(pcb, 2, takenBytes);
        return jumpIf0(pcb, args[0], args[1]);
    }
    else if (command == "JN")
    {
        args = getArgs(pcb, 2, takenBytes);
        return jumpIfN0(pcb, args[0], args[1]);
    }
    else if (command == "WT")
    {
        pcb->setCommandCounter(pcb->getCommandCounter()+2);
        // ret = haltProcess(pcb->getPid());
    }
    else if (command == "MV")
    {
        args = getArgs(pcb, 2, takenBytes);
        //TODO: Code
    }
    

    pcb->setCommandCounter(pcb->getCommandCounter()+takenBytes);
    return ret;
}