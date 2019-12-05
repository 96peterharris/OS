#pragma once
#include <array> //std::array
#include <vector> //std::vector
#include <cmath> //std::pow
#include <memory> //std::shared_ptr
//#include "Virtual_Mem.hpp"
//#include "PCB.hpp"

//do usuniecia potem
void updateVM(PCB*pcb, std::string bytes);

//do usuniecia potem
class SegmentPCB {
    public:
    int baseVM;
    int baseRAM;
    int limit;
    bool vi;    
};

//do usuniecia potem
class PCB {
    public:
    std::string state;
    std::vector<SegmentPCB*> segTab;
};

class Ram{
    private:
    std::array<char,512> ram; //RAM
    std::array<bool, 64> blocks; //0 - block is empty, 1 - block with data
    const int maxDivision = 6;

    public:
    Ram() {
        ram.fill(0);
        blocks.fill(0);
    }

    void loadToRam(PCB* pcb, int segment, char sth, int logAddr); //zapisuje od interpretera
    void loadFromVirtual(PCB* pcb,  std::string bytes, int segment); //pobranie segmentu z pamięci wirtualnej
    char readFromRam(PCB* pcb, int segment, int logAddr); //przekazuje do interpretera
    void deleteFromRam(PCB* pcb); //usuwa z ramu
    //do zrobienia:
    /*
    void sendMessage(statements st); //zapisanie wiadomosci   //argumenty
    statements readMessage(something); //oczytanie wiadomosci   //argumenty
    */

    private:
    void buddy(PCB* pcb, int segment, int divisionLvl, std::string bytes);
    int physAddr(PCB* pcb, int segment, int logAddr);
    //void clearBlocks(int firstBlock, int numOfBlocks);
    bool isInRam(PCB* pcb, int segment);
};

/*
DO OMOWIENIA:
- komunikaty
- semafory, jak nie ma miejsca

*/