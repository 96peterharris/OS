#pragma once
#include "Headers.h"

class Ram{
    private:
    std::array<char,512> ram;
    std::array<bool, 64> blocks;
    const int maxDivision = 6;

    public:
    Ram();
    ~Ram();

    Semaphore ramSem;

    void saveInRam(PCB* pcb, int segment, char ch, int logAddr);
    bool loadToRam(PCB* pcb,  std::string bytes, int segment);
    char readFromRam(PCB* pcb, int segment, int logAddr);
    void deleteFromRam(PCB* pcb);
    std::string readMessage(int ramAddr);
    void printRam();
    void printProcess(PCB* pcb);
    void printSegment(PCB* pcb, int segment);

    private:
    bool buddy(PCB* pcb, std::string bytes, int segment, int divisionLvl);
    int physAddr(PCB* pcb, int segment, int logAddr);
    bool isInRam(PCB* pcb, int segment);
} RAM;