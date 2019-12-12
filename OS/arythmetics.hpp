#pragma once

#include <regex>

#include "PCB.hpp"
#include "RAM.hpp"

class Ram;

bool ADD(PCB *pcb, std::string dest, std::string arg);
bool SUB(PCB *pcb, std::string dest, std::string arg);
bool MUL(PCB *pcb, std::string dest, std::string arg);
bool MOV(PCB *pcb, std::string dest, std::string arg);