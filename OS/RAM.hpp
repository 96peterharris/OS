#pragma once
#include <array> //std::array
#include <cmath> //std::pow
#include <memory> //std::shared_ptr

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

    void loadToRam(std::string file, int size); //argumenty nwm
    //char readFromRam(*PCB, int logAddr); //pobiera z ramu
    int* physAddr(int logAddr); //zwraca wsk do adresu fizycznego
    //void deleteFromRam(*PCB); //usuwa z ramu

    //void getFromVirtual(/*coś z tablicy segmentów*/); //pobranie segmentu z pamięci wirtualnej

    private:
    void buddy(std::string file, int fileSize, int divisionLvl);
};

/*
std::vector<> segVector;
*/