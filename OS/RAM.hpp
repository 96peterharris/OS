#pragma once
#include <array> //std::array
#include <vector> //std::vector
#include <cmath> //std::pow
#include <memory> //std::shared_ptr


//do usuniecia potem
class PCB {
    public:
    std::vector<std::array<int, 6>>segment_table; //baza, odleglosc, v/i, ramAddr, block
};

class statements {
    public:
    std::string senderID;
    std::string content;
};

class Ram{
    std::array<char,512> ram; //RAM
    std::array<bool, 64> blocks; //0 - block is empty, 1 - block with data
    const int maxDivision = 6;

    public:
    Ram() {
        ram.fill(0);
        blocks.fill(0);
    }

    void loadToRam(PCB* pcb, int segment, char sth, int logAddr); //zapisuje od interpretera
    void loadFromVirtual(PCB* pcb, int segment); //pobranie segmentu z pamięci wirtualnej
    char readFromRam(PCB* pcb, int segment, int logAddr); //przekazuje do interpretera
    void deleteFromRam(PCB* pcb); //usuwa z ramu
    //do zrobienia:
    /*
    void sendMessage(statements st); //zapisanie wiadomosci   //argumenty
    statements readMessage(something); //oczytanie wiadomosci   //argumenty
    */

    private:
    void buddy(PCB* pcb, int segment, int divisionLvl);
    int physAddr(PCB* pcb, int segment, int logAddr);
    void clearBlocks(int firstBlock, int numOfBlocks);
    bool isInRam(PCB* pcb, int segment);
};

/*
DO OMOWIENIA:
- komunikaty
- wirtualna
-exceptions?

*/