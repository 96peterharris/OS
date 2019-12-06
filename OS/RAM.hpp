#pragma once
#include <array> //std::array
#include <vector> //std::vector
#include <cmath> //std::pow
#include <memory> //std::shared_ptr
//#include "Virtual_Mem.hpp"
//#include "PCB.hpp"
//#include "Sync_Mech.hpp"

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

    Semaphore ramSem(1); //public czy private? czy Robert potrzebuje?

    void saveInRam(PCB* pcb, int segment, char sth, int logAddr); //zapisuje od interpretera
    void loadToRam(PCB* pcb,  std::string bytes, int segment); //pobranie segmentu z pamięci wirtualnej
    char readFromRam(PCB* pcb, int segment, int logAddr); //przekazuje do interpretera
    void deleteFromRam(PCB* pcb); //usuwanie procesu z ramu
    std::string readMessage(PCB* pcb, int size, int ramAddr); //przekazywanie tresci komunikatu

    private:
    void buddy(PCB* pcb, int segment, int divisionLvl, std::string bytes);
    int physAddr(PCB* pcb, int segment, int logAddr);
    bool isInRam(PCB* pcb, int segment);
};

/*
DO OMOWIENIA/POPRAWIENIA:
- semafory, jak nie ma miejsca - done
- jesli segmentu nie ma w ramie - std::string getSegment(PCB*, int segment)
- updateVM
- sprawdzenie state WAITING - done
- nazwa funkcji loadToRam, loadFromVirtual - done
loadToRam -> saveInRam
loadFromVirtual -> loadToRam
*/