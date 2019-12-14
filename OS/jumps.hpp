#pragma once

#include "PCB.hpp"

bool jump(PCB *pcb, bool &ICcheck, std::string logAddr);
bool jumpIf0(PCB *pcb, bool &ICcheck, std::string registerName, std::string logAddr);
bool jumpIfN0(PCB *pcb, bool &ICcheck, std::string registerName, std::string logAddr);