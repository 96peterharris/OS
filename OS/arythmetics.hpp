#pragma once

#include "Headers.h"

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
    unsigned char a, b;

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
        b = RAM.readFromRam(pcb, 1, std::stoi(m[1]));

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
            a = pcb->getRegisterPointer()->getA();
            if (a+b>255){
                pcb->getRegisterPointer()->setA(255);
            } else{
                pcb->getRegisterPointer()->setA(a+b);
            }
            
        }
        else if (m[1] == "B")
        {
            a = pcb->getRegisterPointer()->getB();
            if (a+b>255){
                pcb->getRegisterPointer()->setB(255);
            } else{
                pcb->getRegisterPointer()->setB(a+b);
            }
        }
        else if(m[1] == "C")
        {
            a = pcb->getRegisterPointer()->getC();
            if (a+b>255){
                pcb->getRegisterPointer()->setC(255);
            } else{
                pcb->getRegisterPointer()->setC(a+b);
            }
        }
        else if(m[1] == "D")
        {
            a = pcb->getRegisterPointer()->getD();
            if (a+b>255){
                pcb->getRegisterPointer()->setD(255);
            } else{
                pcb->getRegisterPointer()->setD(a+b);
            }
        }
    }
    else if(std::regex_match(dest, m, memory))
    {
        a = RAM.readFromRam(pcb, 1, std::stoi(m[1]));
        if (a+b>255){
            RAM.saveInRam(pcb, 1, 255, std::stoi(m[1]));
        } else{
            RAM.saveInRam(pcb, 1, a+b, std::stoi(m[1]));
        }
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
    std::regex regi("([ABCD])X");
    std::regex memory("\\[(\\d+)\\]");
    std::smatch m;
    unsigned char a, b;

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
        b = RAM.readFromRam(pcb, 1, std::stoi(m[1]));

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
            a = pcb->getRegisterPointer()->getA();
            if (a < b){
                pcb->getRegisterPointer()->setA(0);
            } else{
                pcb->getRegisterPointer()->setA(a-b);
            }
            
        }
        else if (m[1] == "B")
        {
            a = pcb->getRegisterPointer()->getB();
            if (a < b){
                pcb->getRegisterPointer()->setB(0);
            } else{
                pcb->getRegisterPointer()->setB(a-b);
            }
        }
        else if(m[1] == "C")
        {
            a = pcb->getRegisterPointer()->getC();
            if (a < b){
                pcb->getRegisterPointer()->setC(0);
            } else{
                pcb->getRegisterPointer()->setC(a-b);
            }
        }
        else if(m[1] == "D")
        {
            a = pcb->getRegisterPointer()->getD();
            if (a < b){
                pcb->getRegisterPointer()->setD(0);
            } else{
                pcb->getRegisterPointer()->setD(a-b);
            }
        }
    }
    else if(std::regex_match(dest, m, memory))
    {
        a = RAM.readFromRam(pcb, 1, std::stoi(m[1]));
        if (a < b){
            RAM.saveInRam(pcb, 1, 0, std::stoi(m[1]));
        } else{
            RAM.saveInRam(pcb, 1, a-b, std::stoi(m[1]));
        }
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
    unsigned char a, b;

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
        b = RAM.readFromRam(pcb, 1, std::stoi(m[1]));

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
            a = pcb->getRegisterPointer()->getA();
            if (a*b>255){
                pcb->getRegisterPointer()->setA(255);
            } else{
                pcb->getRegisterPointer()->setA(a*b);
            }
            
        }
        else if (m[1] == "B")
        {
            a = pcb->getRegisterPointer()->getB();
            if (a*b>255){
                pcb->getRegisterPointer()->setB(255);
            } else{
                pcb->getRegisterPointer()->setB(a*b);
            }
        }
        else if(m[1] == "C")
        {
            a = pcb->getRegisterPointer()->getC();
            if (a*b>255){
                pcb->getRegisterPointer()->setC(255);
            } else{
                pcb->getRegisterPointer()->setC(a*b);
            }
        }
        else if(m[1] == "D")
        {
            a = pcb->getRegisterPointer()->getD();
            if (a*b>255){
                pcb->getRegisterPointer()->setD(255);
            } else{
                pcb->getRegisterPointer()->setD(a*b);
            }
        }
    }
    else if(std::regex_match(dest, m, memory))
    {
        a = RAM.readFromRam(pcb, 1, std::stoi(m[1]));
        if (a*b>255){
            RAM.saveInRam(pcb, 1, 255, std::stoi(m[1]));
        } else{
            RAM.saveInRam(pcb, 1, a*b, std::stoi(m[1]));
        }
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
        b = RAM.readFromRam(pcb, 1, std::stoi(m[1]));
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
        RAM.saveInRam(pcb, 1, b, std::stoi(m[1]));
    }
    else
    {
        return 0;
    }

    return 1;
}