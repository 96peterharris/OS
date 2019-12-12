#pragma once
#include <iostream>
#include <array> //std::array
#include <vector>
#include <string>
#include <cmath> //std::pow

#include "Virtual_Mem.hpp"
#include "PCB.hpp"
#include "Sync_Mech.hpp"
#include "CPU_Scheduling.hpp"

class Virtual_Mem;

class Ram{
private:
    std::array<char,512> ram;
    std::array<bool, 64> blocks;
    const int maxDivision = 6;
    Semaphore ramSem;

public:
    Ram();
    ~Ram();

    bool saveInRam(PCB* pcb, int segment, char ch, int logAddr);
    bool loadToRam(PCB* pcb,  std::string bytes, int segment);
    char readFromRam(PCB* pcb, int segment, int logAddr);
    bool deleteFromRam(PCB* pcb);
    std::string readMessage(int ramAddr);

    void printAllRam();
    void printRam(int start, int stop);
    void printProcess(PCB* pcb);
    void printSegment(PCB* pcb, int segment);
    void printMessage(int ramAddr);
    void printSemaphore();

private:
    bool buddy(PCB* pcb, int segment, std::string bytes, int divisionLvl);
    int physAddr(PCB* pcb, int segment, int logAddr);
    bool isInRam(PCB* pcb, int segment);
}RAM;