#pragma once

#include "PCB.hpp"

bool jump(PCB *pcb, std::string logAddr);
bool jumpIf0(PCB *pcb, std::string registerName, std::string logAddr);
bool jumpIfN0(PCB *pcb, std::string registerName, std::string logAddr);