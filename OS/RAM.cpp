#include "ram.hpp"

//baza, odleglosc, v/i, ramAddr, firstBlock (vec blocks), size
//v/i - 0-virtual, 1-ram
std::string PROG;

void Ram::loadToRam(PCB* pcb, int segment, char sth, int logAddr) {
    if (isInRam(pcb, segment)){
        int pAddr = physAddr(pcb, segment, logAddr);
        ram[pAddr] = sth;
    }
    else {
        loadFromVirtual(pcb, segment);
    }
}

void Ram::loadFromVirtual(PCB* pcb, int segment) {
    if(!isInRam(pcb, segment)) {
        buddy(pcb, segment, 0);
    }
    pcb->segment_table[segment][2] = 1; //jesli segment_table jest public
}

void Ram::buddy(PCB* pcb, int segment, int divisionLvl) {
    int fileSize = pcb->segment_table[segment][1];
    int blockSize = std::pow(2, 6 - divisionLvl);  //rozmiar obecnego bloku
    int nextBlockSize; //rozmiar kolejnego bloku
    if (fileSize > 7) nextBlockSize = blockSize/2;
    else nextBlockSize = 0;
    int numOfBlocks = std::pow(2, divisionLvl); //liczba blokow danego podzialu
    int jump = blockSize/8; //rozmiar bloku
    bool ok; //do sprawdzania, czy blok jest zajety
    int startAddr; //adres poczatkowy bloku
    int startAddrBlocks; //adres poczatkowy bloku w vec blocks

    if (blockSize >= fileSize && nextBlockSize < fileSize) { //znaleziono rozmiar bloku
        for (int i = 0; i < 64; i=i+jump) { //dla polowek
            for (int j = i; j<jump; j++) { //sprawdzenie, czy polowka jest pusta
                if (j==i) startAddr = j*8; //ustawienie poczatkowego adresu bloku
                if(!blocks[j]) ok = true; //jeśli blok pusty to gituwa
                if (!ok) break; //jak nie to pal wroty
            }
            if (ok) break; //jeśli gituwa to gituwa
        }
        if (!ok) throw "No space"; //jak nie to pal wroty razy 2 (potem zrobie to ladnie)
        else {
            //insert:
            for (int i = 0; i < fileSize; i++) //ram
            {
                clearBlocks(startAddrBlocks, numOfBlocks);
                ram[startAddr+i] = PROG[i];  //PROG?
            }
            for (int i = 0; i < jump; i++) //blocks
            {
                blocks[startAddrBlocks+i] = 1;
            }
        }
    }
    else{
        divisionLvl++;
        buddy(pcb, segment, divisionLvl);
    }
}

char Ram::readFromRam(PCB* pcb, int segment, int logAddr) {
    if (isInRam(pcb, segment)){
        int pAddr = physAddr(pcb, segment, logAddr);
        return ram[pAddr];
    } else {
        loadFromVirtual(pcb, segment);
    }

}

void Ram::deleteFromRam(PCB* pcb) { //usuwanie calego procesu
    int numOfBlocks = (pcb->segment_table[0][5])/8; //liczba blokow
    for (int i = pcb->segment_table[0][4]; i <numOfBlocks; i++) { //dla blokow tego procesu
        blocks[i] = 0;
    }
}

void Ram::clearBlocks(int firstBlock, int numOfBlocks) {
    for (int i = firstBlock; i<numOfBlocks; i++) { //dla blokow
        for (int j = 0; j < 8; j++) { //dla elementow blokow
            ram[i*8+j] = 0;
        }
    }
}

int Ram::physAddr(PCB* pcb, int segment, int logAddr) {
    return logAddr+=(pcb->segment_table[segment][4])*8;
}

bool Ram::isInRam(PCB* pcb, int segment) {
    return(pcb->segment_table[segment][2]);
}

/*
divisionLvl         size        numOfBlocks     jump
0                   512         1               64 
1                   256         2               32
2                   128         4               16
3                   64          8               8
4                   32          16              4
5                   16          32              2
6                   8           64              1

*/