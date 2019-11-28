#include "ram.hpp"

/*void Ram::loadToRam(std::string file, int size) {
    if (size > 512) throw "Too large file"; //albo cos
    buddy(file, size, 0);
}*/

//baza, odleglosc, v/i, ramAddr, firstBlock (vec blocks), size

void Ram::loadToRam(PCB* pcb, int segment, char sth, int logAddr) {
    //dodac sprawdzenie, czy jest w ramie
    if (pcb->segment_table[segment][2] == 0){ //? jesli jest w ramie
        int pAddr = physAddr(pcb, segment, logAddr);
        ram[pAddr] = sth;
    }
    else {
        //dodaj z wirtualnej chyba
    }
}

void Ram::buddy(std::string file, int fileSize, int divisionLvl) //file - wsk do memory, fileSize - rozmiar pliku, blockLevel - poziom bloku (6=512, 0=8)
{
    int blockSize = std::pow(2, 6 - divisionLvl);  //rozmiar obecnego bloku
    int nextBlockSize; //rozmiar kolejnego bloku
    if (fileSize > 7) nextBlockSize = blockSize/2;
    else nextBlockSize = 0;
    int numOfBlocks = std::pow(2, divisionLvl); //liczba bloków danego podziału
    int jump = blockSize/8; //rozmiar bloku
    bool ok; //do sprawdzania, czy blok jest zajęty
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
                ram[startAddr+i] = file[i];
            }
            for (int i = 0; i < jump; i++) //blocks
            {
                blocks[startAddrBlocks+i] = 1;
            }
        }
    }
    else{
        divisionLvl++;
        buddy(file, fileSize, divisionLvl);
    }
}

char Ram::readFromRam(PCB* pcb, int segment, int logAddr) {
    int pAddr = physAddr(pcb, segment, logAddr);
    return ram[pAddr];
}

void Ram::deleteFromRam(PCB* pcb) { //usuwanie całego procesu
    int numOfBlocks = (pcb->segment_table[0][5])/8; //liczba blokow
    for (int i = pcb->segment_table[0][4]; i <numOfBlocks; i++) { //dla blokow tego procesu
        blocks[i] = 0;
    }
}

void Ram::clearBlocks(int firstBlock, int numOfBlocks) {
    for (int i = firstBlock; i<numOfBlocks; i++) { //dla bloków
        for (int j = 0; j < 8; j++) { //dla elementow blokow
            ram[i*8+j] = 0;
        }
    }
}

int physAddr(PCB* pcb, int segment, int logAddr) {
    return logAddr+=(pcb->segment_table[segment][4])*8;
}

//stara wersja - usunelabym gdybym nie byla pizda lel
/*void Ram::buddy(*beginAddr, int size, int divisionLvl) //file - wsk do memory, fileSize - rozmiar pliku, blockLevel - poziom bloku (6=512, 0=8)
{
    int nextBlockSize = std::pow(2, blockLvl - 1); //rozmiar kolejnego (mniejszego) bloku
    int blockSize = nextBlockSize * 2; // rozmiar obecnego bloku

    if (blockLvl == 6 && fileSize > blockSize) { //jeśli plik > ram
        throw "Too large file";
    }

// if plik mniejszy niż pamięć, ale nie ma odstatecznie dużo wolnego miejsca

    if (nextBlockSize < fileSize && fileSize <= blockSize) { //znaleziony rozmiar bloku
    int i = 0; //adres
    int n = 0; //nr bloku w std::array blocks
       do { //poblokach danego rozmiaru
           if (!blocks[n]);  //gituwa
           else {
                i++;
                n+=blockLvl*2;
           }
       } while (i<254);
       throw "No space in RAM"; //jeśli ram jest zapełniony
    }
    else {
        insert(file, fileSize, blockLvl-1);
    }
}*/

//nwm co to, stare jakies i niepotrzebne chyba ale nie wywale bo jeszcze sie okaze, ze jednak potrzebne
/*int* Ram::getProgAddr(int prog) {
    for(int i=0; i<16; i++) {
        if(*programs[i] == prog) return programs[i];
    }
    throw "Program wasn't found in RAM";
}*/

//notatki
/*
- jak raz zapiszemy w danym miejscu to nie przenosimy, niech siedzi i zagraca
- algorytm usuwania
- niech dodaje najoptymalniej - tam gdzie jest już podzielone (teraz jest first)
- smartpojntery

*/
//-----------------------------------------
//struktura
/*
całkowity rozmiar = 512

divisionLvl         size        numOfBlocks     jump
0                   512         1               64 
1                   256         2               32
2                   128         4               16
3                   64          8               8
4                   32          16              4
5                   16          32              2
6                   8           64              1

*/