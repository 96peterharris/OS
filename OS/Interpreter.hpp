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

std::vector<std::string> getArgs(PCB *pcb, int argNum, int &takenBytes);
bool interprate(PCB *pcb);