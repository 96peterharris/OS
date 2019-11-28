#pragma once
#include <array> //std::array
#include <vector> //std::vector
#include <cmath> //std::pow
#include <memory> //std::shared_ptr

class PCB {
    public:
    std::vector<std::array<int, 6>>segment_table; //baza, odleglosc, v/i, ramAddr, block, size
};

class Ram{
    std::array<char,512> ram; //RAM
    std::array<bool, 64> blocks; //0 - block is empty, 1 - block with data
    const int maxDivision = 6;
    int numOfLvls; // = 6;

    public:
    Ram() {
        ram.fill(0);
        blocks.fill(0);
    }

    void loadToRam(PCB* pcb, int segment, char sth, int logAddr); //zapisuje z wirtualnej/interpreter
    char readFromRam(PCB* pcb, int segment, int logAddr); //pobiera z ramu  //int pcb = *PCB
    void deleteFromRam(PCB* pcb); //usuwa z ramu
    void sendMessage(std::string); //zapisanie wiadomosci
    std::string readMessage(); //oczytae wiadomosci   //argumenty

    //void getFromVirtual(); //pobranie segmentu z pamięci wirtualnej

    private:
    void buddy(std::string file, int fileSize, int divisionLvl);
    int physAddr(PCB*, int segment, int logAddr); //zwraca wsk do adresu fizycznego  //int pcb = *PCB
    void clearBlocks(int firstBlock, int numOfBlocks);
};

//poszczególne rozkazy, itp. rozdzielone będą spacjami

/*
std::vector<> segVector;
*/

/*
1
2
3
4
...
49 ' '
50 3
51 4
52 ' '
53 8
...

*/