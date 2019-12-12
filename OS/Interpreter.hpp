#pragma once
//Seba
#include "arythmetics.hpp"
#include "jumps.hpp"
#include "PCB.hpp"
#include "RAM.hpp"
#include "Interprocess_Com.hpp"
#include "Files_And_Directory_Management.hpp"

class Ram;
class PCB;

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
            read = RAM.readFromRam(pcb, 0, takenBytes);
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
    command.append(1, RAM.readFromRam(pcb, 0, pcb->getCommandCounter()));
    command.append(1, RAM.readFromRam(pcb, 0, pcb->getCommandCounter()+1));

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
        ret = PCB::haltProcess(pcb->getPid());
    }

    // KOMUNIKACJA PROCESOW

    else if (command == "SM") //FIXME: WOJTEK! Lista argumentow
    {
        args = getArgs(pcb, 2, takenBytes);
        ret = pcb->sendMessage(args[0], args[1]);
    }
    else if (command == "RM") 
    {
        ret = pcb->receiveMessage();
    }

    // FILE SYSTEM

    else if (command == "CF")
    {
        args = getArgs(pcb, 1, takenBytes);
        ret = fs.createFile(args[0]);
    }
    else if (command == "CL")
    {
        args = getArgs(pcb, 2, takenBytes);
        ret = fs.closeFile(args[0], args[1]);
    }
    else if (command == "DF")
    {
        args = getArgs(pcb, 2, takenBytes);
        ret = fs.deleteFile(args[0], args[1]);
    }
    else if (command == "OP")
    {
        args = getArgs(pcb, 2, takenBytes);
        ret = fs.openFile(args[0], args[1]);
    }
    else if (command == "OW")
    {
        args = getArgs(pcb, 3, takenBytes);
        ret = fs.overwriteFile(args[0], args[1], args[2]);
    }
    else if (command == "NF")
    {
        args = getArgs(pcb, 3, takenBytes);
        ret = fs.renameFile(args[0], args[1], args[2]);
    }
    else if (command == "WF")
    {
        args = getArgs(pcb, 3, takenBytes);
        ret = fs.writeToFile(args[0], args[1], args[2]);
    }
    
    
    
    
    

    pcb->setCommandCounter(pcb->getCommandCounter()+takenBytes);
    return ret;
}