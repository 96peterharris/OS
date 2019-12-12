#pragma once
//Seba


//#include "interpreter_functions\Interpreter_syntax.hpp"

#include "Headers.h"

/**
 * Reads arguments from RAM and increases takenBytes
 * 
 * Reades chars from RAM to string increasing takenBytes param for each read byte. The end of each argument (string) is recognised by space character, which is poped from the string.
 * 
 * @param pcb pointer to running PCB object
 * @param argNum number of separate arguments needed to read
 * @param takenBytes reference to a value of already read bytes from RAM
 * @return vector with each argument as a string in seperate cell in order of their appearance in RAM
*/
std::vector<std::string> getArgs(PCB *pcb, int argNum, int &takenBytes){
    std::vector<std::string> vector;
    takenBytes += pcb->getCommandCounter();
    for (int i = 0; i < argNum; i++){
        char read;
        std::string str="";
        do
        {
            read = readFromRam(pcb, 0, takenBytes);
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
    command.append(1, readFromRam(pcb, 0, pcb->getCommandCounter()));
    command.append(1, readFromRam(pcb, 0, pcb->getCommandCounter()+1));

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
    else if (command == "JZ") //jump if register == 0
    {
        args = getArgs(pcb, 2, takenBytes);
        return jumpIf0(pcb, args[0], args[1]);
    }
    else if (command == "JN") //jump if register != 0
    {
        args = getArgs(pcb, 2, takenBytes);
        return jumpIfN0(pcb, args[0], args[1]);
    }
    else if (command == "MV") //copy value to dest
    {
        args = getArgs(pcb, 2, takenBytes);
        ret = MOV(pcb, args[0], args[1]);
    }

    // ------- NIE SEBOWE FUNKCJE ---------

    else if (command == "WT") //set process as waiting
    {
        pcb->setCommandCounter(pcb->getCommandCounter()+2);
        ret = haltProcess(pcb->getPid());
    }
    else if (command == "SM") //FIXME: skad brac argumenty jak PID?
    {
        args = getArgs(pcb, 3, takenBytes);
        ret = sendMessage(args[0], args[1], args[2]);
    }
    else if (command == "RM") //FIXME: skad brac argumenty jak PID?
    {
        args = getArgs(pcb, 3, takenBytes);
        ret = receiveMessage(args[0], args[1], args[2]);
    }
    

    pcb->setCommandCounter(pcb->getCommandCounter()+takenBytes);
    return ret;
}