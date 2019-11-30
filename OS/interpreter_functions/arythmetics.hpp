#pragma once

#include <regex>

// #include "Interpreter_syntax.hpp"
#include "..\PCB.hpp"
#include "..\RAM.hpp"

/**
 * Adds value to a given register or RAM memory cell
 * Works as dest. += arg.
 * @param pcb pointer to running PCB object
 * @param dest Register name or RAM memory cell. Accepted: AX,BX,CX,DX,[logAddr_String]
 * @param arg Value to be added to dest. Accepted as param dest or a string consisting of a single char, which is read as an ASCII value
 * @return true if no problem was found, false otherwise
 * @see Interpreter AD command
*/
bool ADD(PCB *pcb, std::string dest, std::string arg){
    std::regex regi("([ABCD])X");
    std::regex memory("\\[(\\d+)\\]");
    std::smatch m;
    char b;

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
        // b = std::stoi(readRamNum(pcb, std::stoi(m[1])));
        b = readFromRam(pcb, std::stoi(m[1]));

    }
    else if (arg.size() == 1)
    {
        // b = std::stoi(arg); //Test version
        b = arg.at(0);
    }
    else
    {
        return 0;
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

/**
 * Subtracts value form a given register or RAM memory cell
 * Works as dest. -= arg.
 * @param pcb pointer to running PCB class
 * @param dest Register name or RAM memory cell. Accepted: AX,BX,CX,DX,[logAddr_String]
 * @param arg Value to be subtracted from dest. Accepted as param dest or a string consisting of a single char, which is read as an ASCII value
 * @return true if no problem was found, false otherwise
 * @see Interpreter SB command
*/
bool SUB(PCB *pcb, std::string dest, std::string arg){
    //FIXME: Czy dzialac na signed czy unsigned?

    std::regex regi("([ABCD])X");
    std::regex memory("\\[(\\d+)\\]");
    std::smatch m;
    char b;

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
        // b = std::stoi(readRamNum(pcb, std::stoi(m[1])));
        b = readFromRam(pcb, std::stoi(m[1]));

    }
    else if (arg.size() == 1)
    {
        // b = std::stoi(arg); //Test version
        b = arg.at(0);
    }
    else
    {
        return 0;
    }
    

    if(std::regex_match(dest, m, regi))
    {
        if (m[1] == "A")
        {
            pcb->getRegisterPointer()->setA(pcb->getRegisterPointer()->getA()-b);
        }
        else if (m[1] == "B")
        {
            pcb->getRegisterPointer()->setB(pcb->getRegisterPointer()->getB()-b);
        }
        else if(m[1] == "C")
        {
            pcb->getRegisterPointer()->setC(pcb->getRegisterPointer()->getC()-b);
        }
        else if(m[1] == "D")
        {
            pcb->getRegisterPointer()->setD(pcb->getRegisterPointer()->getD()-b);
        }
    }
    else if(std::regex_match(dest, m, memory))
    {
        loadToRam(pcb, readFromRam(pcb, std::stoi(m[1]))-b, std::stoi(m[1]));
    }
    else
    {
        return 0;
    }

    return 1;
}

/**
 * Multiply a given register or RAM memory cell by a value
 * Works as dest. *= arg.
 * @param pcb pointer to running PCB object
 * @param dest Register name or RAM memory cell. Accepted: AX,BX,CX,DX,[logAddr_String]
 * @param arg Value to multiply. Accepted as param dest or a string consisting of a single char, which is read as an ASCII value
 * @return true if no problem was found, false otherwise
 * @see Interpreter MU command
*/
bool MUL(PCB *pcb, std::string dest, std::string arg){
    std::regex regi("([ABCD])X");
    std::regex memory("\\[(\\d+)\\]");
    std::smatch m;
    char b;

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
        b = readFromRam(pcb, std::stoi(m[1]));
    }
    else if (arg.size() == 1)
    {
        // b = std::stoi(arg); //Test version
        b = arg.at(0);
    }
    else
    {
        return 0;
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
}

/**
 * Multiply a given register or RAM memory cell by a value
 * Works as dest. = arg.
 * @param pcb pointer to running PCB object
 * @param dest Register name or RAM memory cell. Accepted: AX,BX,CX,DX,[logAddr_String]
 * @param arg Value to copy. Accepted as param dest or a string consisting of a single char, which is read as an ASCII value
 * @return true if no problem was found, false otherwise
 * @see Interpreter MV command
*/
bool MOV(PCB *pcb, std::string dest, std::string arg){
    std::regex regi("([ABCD])X");
    std::regex memory("\\[(\\d+)\\]");
    std::smatch m;
    char b;

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
        b = readFromRam(pcb, std::stoi(m[1]));
    }
    else if (arg.size() == 1)
    {
        // b = std::stoi(arg); //Test version
        b = arg.at(0);
    }
    else
    {
        return 0;
    }

    if(std::regex_match(dest, m, regi))
    {
        if (m[1] == "A")
        {
            pcb->getRegisterPointer()->setA(b);
        }
        else if (m[1] == "B")
        {
            pcb->getRegisterPointer()->setB(b);
        }
        else if(m[1] == "C")
        {
            pcb->getRegisterPointer()->setC(b);
        }
        else if(m[1] == "D")
        {
            pcb->getRegisterPointer()->setD(b);
        }
    }
    else if(std::regex_match(dest, m, memory))
    {
        loadToRam(pcb, b, std::stoi(m[1]));
    }
    else
    {
        return 0;
    }

    return 1;
}