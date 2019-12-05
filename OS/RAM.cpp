#include "ram.hpp"

//baza, odleglosc, v/i, ramAddr, firstBlock (vec blocks), size
//v/i - 0-virtual, 1-ram
//segment - 0-text, 1-data
//loadFromVirtual i buddy - 0-git, 1-brak miejsca
//std::string PROG;

void Ram::loadToRam(PCB* pcb, int segment, char sth, int logAddr) {
    if (isInRam(pcb, segment)){
        int pAddr = physAddr(pcb, segment, logAddr);
        ram[pAddr] = sth;
    }

    //!!! co jak nie ma w ramie?
    else {
        //loadFromVirtual(pcb, bytes, segment);
    }
}

//!!!
void Ram::loadFromVirtual(PCB* pcb, std::string bytes, int segment) {
    buddy(pcb, segment, 0, bytes);
}

void Ram::buddy(PCB* pcb, int segment, int divisionLvl, std::string bytes) {
    int fileSize = pcb->segTab[segment]->limit;
    int blockSize = std::pow(2, 9 - divisionLvl);  //rozmiar obecnego bloku
    int nextBlockSize; //rozmiar kolejnego (mniejszego) bloku
    if (fileSize > 7) nextBlockSize = blockSize/2;
    else nextBlockSize = 0;
    int numOfBlocks = std::pow(2, divisionLvl); //liczba blokow danego podzialu
    int jump = blockSize/8; //rozmiar bloku
    bool ok; //do sprawdzania, czy blok jest zajety
    int startAddr; //adres poczatkowy bloku
    int startAddrBlocks; //adres poczatkowy bloku w vec blocks

    if (blockSize >= fileSize && nextBlockSize < fileSize) { //znaleziono rozmiar bloku
        for (int i = 0; i < 64; i=i+jump) { //dla polowek
            for (int j = i; j<i+jump; j++) { //sprawdzenie, czy polowka jest pusta
                if (j==i){
                    startAddr = j*8; //ustawienie poczatkowego adresu bloku
                    startAddrBlocks = j;
                    }
                if(!blocks[j]) ok = true; //jeśli blok pusty to gituwa
                else ok = false;
                if (!ok) break; //jak nie to pal wroty
            }
            if (ok) break; //jeśli gituwa to gituwa
        }
        if (!ok){
            //nie ma miejsca - wywolac Karola
        }
        else {
            //insert:
            for (int i = 0; i < fileSize; i++) //ram
            {
                //clearBlocks(startAddrBlocks, numOfBlocks);
                ram[startAddr+i] = bytes[i];  //
            }
            for (int i = 0; i < jump; i++) //blocks
            {
                blocks[startAddrBlocks+i] = 1;
            }
            pcb->segTab[segment]->vi = 1; //do poprawy
        }
    }
    else{
        divisionLvl++;
        buddy(pcb, segment, divisionLvl, bytes);
    }
}

char Ram::readFromRam(PCB* pcb, int segment, int logAddr) {
    if (isInRam(pcb, segment)){
        int pAddr = physAddr(pcb, segment, logAddr);
        return ram[pAddr];
    }
    //!!! co jak nie ma w ramie?
    else {
        //loadFromVirtual(pcb, segment);
    }

}

void Ram::deleteFromRam(PCB* pcb) { //usuwanie calego procesu
//??? tylko zerowanie blokow czy czyszczenie ramu tez?
    //segment 0
    int numOfBlocks; //liczba blokow
    int num1 = pcb->segTab[0]->limit/8;
    int num2 = pcb->segTab[0]->limit%8;
    if (num2==0) numOfBlocks = num1;
    else numOfBlocks = num1+1;

    int firstBlock = pcb->segTab[0]->baseRAM/8; //liczone od 0

    for (int i = firstBlock; i < numOfBlocks+firstBlock; i++) { //dla blokow tego procesu
        blocks[i] = 0;
        for (int j = 0; j < 8; j++) { //dla elementow blokow
            ram[i*8+j] = 0;
        }
    }
    //segment 1
    num1 = pcb->segTab[1]->limit/8;
    num2 = pcb->segTab[1]->limit%8;
    if (num2==0) numOfBlocks = num1;
    else numOfBlocks = num1+1;

    firstBlock = pcb->segTab[1]->baseRAM/8; //liczone od 0

    //update
    std::string state;// = PCB->state.getStateName;  do przywrocenia potem
    if (state == "WAITING"){
        std::string bytes;
        for (int i = firstBlock; i < numOfBlocks+firstBlock; i++) { //dla blokow tego procesu
            for (int j = 0; j < 8; j++) { //dla elementow blokow
                bytes.append[i*8+j];
            }
        }
        updateVM(pcb, bytes);
    }

    //segment 1
    for (int i = firstBlock; i < numOfBlocks+firstBlock; i++) { //dla blokow tego procesu
        blocks[i] = 0;
        for (int j = 0; j < 8; j++) { //dla elementow blokow
            ram[i*8+j] = 0;
        }
    }
}

//??? kiedy wywolywac? przy usuwaniu procesu czy przed zapisaniem?
/*void Ram::clearBlocks(int firstBlock, int numOfBlocks) { 
    for (int i = firstBlock; i<numOfBlocks; i++) { //dla blokow
        for (int j = 0; j < 8; j++) { //dla elementow blokow
            ram[i*8+j] = 0;
        }
    }
}*/

int Ram::physAddr(PCB* pcb, int segment, int logAddr) {
    return logAddr+pcb->segTab[segment]->baseRAM;
}

bool Ram::isInRam(PCB* pcb, int segment) {
    return pcb->segTab[segment]->vi; //vi
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