#pragma once

#include <regex>

#include "Interpreter_syntax.hpp"
#include "readRamNum.hpp"
#include "..\PCB.hpp"

bool ADD(PCB *pcb, std::string dest, std::string arg){
    
    char b;

    std::regex regi("([ABCD])X");
    // std::regex num("\\d+");
    std::regex memory("\\[(\\d+)\\]");
    std::smatch m;

    if (std::regex_match(arg, m, regi))
    {
        if (m[1] == "A")
        {
            b = pcb->getRegisterPointer()->getA();
        }
        else if (m[1] == "B")
        {
            b = pcb->getRegisterPointer()->getB();
        }
        else if(m[1] == "C")
        {
            b = pcb->getRegisterPointer()->getC();
        }
        else if(m[1] == "D")
        {
            b = pcb->getRegisterPointer()->getD();
        }
    }
    else if (std::regex_match(arg, m, memory))
    {
        b = std::stoi(readRamNum(pcb, std::stoi(m[1])));
    }
    else
    {
        b = std::stoi(arg);//FIXME
    }

    if(std::regex_match(dest, m, regi))
    {
        if (m[1] == "A")
        {
            pcb->getRegisterPointer()->setA(pcb->getRegisterPointer()->getA()+b);
        }
        else if (m[1] == "B")
        {
            pcb->getRegisterPointer()->setB(pcb->getRegisterPointer()->getB()+b);
        }
        else if(m[1] == "C")
        {
            pcb->getRegisterPointer()->setC(pcb->getRegisterPointer()->getC()+b);
        }
        else if(m[1] == "D")
        {
            pcb->getRegisterPointer()->setD(pcb->getRegisterPointer()->getD()+b);
        }
    }
    else if(std::regex_match(dest, m, memory))
    {
        loadToRam(pcb, readFromRam(pcb, std::stoi(m[1]))+b, std::stoi(m[1]));
    }
    else
    {
        return 0;
    }
    return 1;
}

bool SUB(PCB *pcb, std::string dest, std::string arg){
    //FIXME: Czy dzialac na signed czy unsigned?
    return 0;
}

bool MUL(PCB *pcb, std::string dest, std::string arg){
        
    char b;

    std::regex regi("([ABCD])X");
    // std::regex num("\\d+");
    std::regex memory("\\[(\\d+)\\]");
    std::smatch m;

    if (std::regex_match(arg, m, regi))
    {
        if (m[1] == "A")
        {
            b = pcb->getRegisterPointer()->getA();
        }
        else if (m[1] == "B")
        {
            b = pcb->getRegisterPointer()->getB();
        }
        else if(m[1] == "C")
        {
            b = pcb->getRegisterPointer()->getC();
        }
        else if(m[1] == "D")
        {
            b = pcb->getRegisterPointer()->getD();
        }
    }
    else if (std::regex_match(arg, m, memory))
    {
        b = std::stoi(readRamNum(pcb, std::stoi(m[1])));
    }
    else
    {
        b = std::stoi(arg);//FIXME
    }

    if(std::regex_match(dest, m, regi))
    {
        if (m[1] == "A")
        {
            pcb->getRegisterPointer()->setA(pcb->getRegisterPointer()->getA()*b);
        }
        else if (m[1] == "B")
        {
            pcb->getRegisterPointer()->setB(pcb->getRegisterPointer()->getB()*b);
        }
        else if(m[1] == "C")
        {
            pcb->getRegisterPointer()->setC(pcb->getRegisterPointer()->getC()*b);
        }
        else if(m[1] == "D")
        {
            pcb->getRegisterPointer()->setD(pcb->getRegisterPointer()->getD()*b);
        }
    }
    else if(std::regex_match(dest, m, memory))
    {
        loadToRam(pcb, readFromRam(pcb, std::stoi(m[1]))*b, std::stoi(m[1]));
    }
    else
    {
        return 0;
    }
    return 1;
    
    
    
    
    /* 
    bool ret;
    std::string dest0 = dest;
    //FIXME: arg == 0
    for (int i = 0; i < std::stoi(arg); i++)
    {
        ret = ADD(pcb, dest, dest0);
    }
    return ret; */
}