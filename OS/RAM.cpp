#include "RAM.hpp"
#include "Headers.h"

Ram::Ram() : ramSem(1){
    ram.fill(' ');
    blocks.fill(0);
    
}

Ram::~Ram(){}

/**
 * Saves data to RAM from Interpreter.
 * 
 * Checks if segment has been loaded to RAM. If not, loads it.
 * Interpret logical address to physical address and loads character to RAM.
 * 
 * @param pcb Pointer to PCB needed to call other functions.
 * @param segment Int specifing VM segment: 1 for data.
 * @param ch Character to load to RAM.
 * @param logAddr Logical address where the character is going to be saved.
 */
bool Ram::saveInRam(PCB* pcb, int segment, char ch, int logAddr) {
    if(!isInRam(pcb,segment)){
        std::string text = System::VM.getSegment(pcb, segment);
        loadToRam(pcb, text, segment);
    }
    int pAddr = physAddr(pcb, segment, logAddr);
    ram[pAddr] = ch;
    return 1;
}

/**
 * Calls buddy function.
 * 
 * Calls buddy function which Loads segment to RAM from Virtual Memory or message from Interprocess Communication.
 * 
 * @param pcb Pointer to PCB needed to call buddy function.
 * @param bytes String which contains data to be loaded.
 * @param segment Int specifing VM segment or that a message is going to be loaded: 0 for text segment, 1 for data segment, 2 for message.
 * @return buddy function's value, true for success or false for failure.
 */
bool Ram::loadToRam(PCB* pcb, std::string bytes, int segment) {
    return buddy(pcb, segment, bytes, 0);
}

/**
 * Loads segment to RAM from Virtual Memory or message from Interprocess Communication
 * 
 * Recursive function.
 * Starts with maximum length of RAM.
 * Finds optimum length by calling itself with inceremnting divisionLvl param.
 * Finds free space in RAM of the found length. If it doesn't exist, returns false.
 * Loads data to found free space and sets blocks as busy
 * Sets physical address in PCB, v/i bit or message.
 * 
 * @param pcb Pointer to PCB to find segment size, set physical address in RAM and v/i bit.
 * @param segment Int specifing VM segment or that a message is going to be loaded: 0 for text segment, 1 for data segment, 2 for message.
 * @param divisionLvl Int specifing the level of memory division. Maximum value is 6.
 * @param bytes String which contains data to be loaded.
 * @return true for success or false for failure.
 */
bool Ram::buddy(PCB* pcb, int segment, std::string bytes, int divisionLvl) {
	int fileSize = 0;
	if (segment == 2) fileSize = bytes.size();
	else fileSize = pcb->segTab[segment]->limit;
	if (fileSize > 512) return 0;
	int blockSize = std::pow(2, 9 - divisionLvl);
	int nextBlockSize;
	if (divisionLvl != maxDivision) {
		nextBlockSize = blockSize / 2;
	}
	else nextBlockSize = 0;
	int numOfBlocks = std::pow(2, divisionLvl);
	int jump = blockSize / 8;
	bool ok = false;
	int startAddr;
	int startAddrBlocks;

    if (blockSize >= fileSize && nextBlockSize < fileSize) {
        for (int i = 0; i < 64; i=i+jump) {
            for (int j = i; j<i+jump; j++) {
                if (j==i){
                    startAddr = j*8;
                    startAddrBlocks = j;
                    }
                if(!blocks[j]) ok = true;
                else ok = false;
                if (!ok) break;
            }
            if (ok) break;
        }
        if (!ok){
            if(ramSem.wait_sem(pcb->getPid()));

            return 1;
        }
        else {
            for (int i = 0; i < fileSize; i++)
            {
                ram[startAddr+i] = bytes[i];
            }
            for (int i = 0; i < jump; i++)
            {
                blocks[startAddrBlocks+i] = 1;
            }
            if(segment == 2) {
                (*pcb->messages.at(pcb->messages.size())).RAMadrress =  startAddr;
            }
            else {
                pcb->segTab[segment]->baseRAM = startAddr;
                pcb->segTab[segment]->vi = 1;
                return 1;
            }
        }
    }
    else{
        divisionLvl++;
        buddy(pcb, segment, bytes, divisionLvl);
    }
}

/**
 * Reads character from RAM.
 * 
 * Checks if segment has been loaded to RAM. If not, loads it.
 * Interpret logical address to physical address and reads character from RAM.
 * 
 * @param pcb Pointer to PCB needed to call other functions.
 * @param segment Int specifing VM segment: 0 for text, 1 for data.
 * @param logAddr Int meaning logical address where the character is.
 * @return Char found in requested place.
 */
char Ram::readFromRam(PCB* pcb, int segment, int logAddr) {
    if(!isInRam(pcb, segment)){
        std::string text = System::VM.getSegment(pcb, segment);
        loadToRam(pcb, text, segment);
    }
    int pAddr = physAddr(pcb, segment, logAddr);
    return ram[pAddr];
}

/**
 * Reads message from RAM.
 * 
 * Creates string for message.
 * Pushes back characters from got physical address until second ' ' is found.
 * Deletes message from RAM and sets blocks as free.
 * Returns message.
 * 
 * @param ramAddr Int meaning physical address in RAM where the message begins.
 * @return String containing message.
 */
std::string Ram::readMessage(int ramAddr) {
    int space = 0;
    std::string msg;
    int i = ramAddr;

    while (space != 2) {
        if (ram[i] == ' ') space++;
        msg.push_back(ram[i]);
        i++;
    }
    msg.pop_back();

    int size = msg.size();
    int numOfBlocks;
    int num1 = size/8;
    int num2 = size%8;
    if (num2==0) numOfBlocks = num1;
    else numOfBlocks = num1+1;

    int firstBlock = size/8;

    for (int i = firstBlock; i < numOfBlocks+firstBlock; i++) {
        blocks[i] = 0;
        for (int j = 0; j < 8; j++) {
            ram[i*8+j] = ' ';
        }
    }
    return msg;
}

/**
 * Deletes process from RAM.
 * 
 * Releases segment 0 by setting blocks as free and deleting data from RAM.
 * Does the same for segment 1 but before it, updates segment 1 if PCB state is WAITING.
 * 
 * @param pcb Pointer to PCB needed to get data's size, physical address.
 */
bool Ram::deleteFromRam(PCB* pcb) {
    if (!pcb->segTab[0]->vi && !pcb->segTab[1]->vi) return 0;
    //segment 0
    int numOfBlocks;
    int num1 = pcb->segTab[0]->limit/8;
    int num2 = pcb->segTab[0]->limit%8;
    if (num2==0) numOfBlocks = num1;
    else numOfBlocks = num1+1;

    int firstBlock = pcb->segTab[0]->baseRAM/8;

    for (int i = firstBlock; i < numOfBlocks+firstBlock; i++) {
        blocks[i] = 0;
        for (int j = 0; j < 8; j++) {
            ram[i*8+j] = ' ';
        }
    }
    //segment 1
    num1 = pcb->segTab[1]->limit/8;
    num2 = pcb->segTab[1]->limit%8;
    if (num2==0) numOfBlocks = num1;
    else numOfBlocks = num1+1;

    firstBlock = pcb->segTab[1]->baseRAM/8;

    //update
    if (pcb->getState() == WAITING) {
        std::string bytes;
        for (int i = firstBlock; i < numOfBlocks+firstBlock; i++) {
            for (int j = 0; j < 8; j++) {
                bytes.push_back(i*8+j);
            }
        }
		System::VM.loadToVM(pcb, bytes);
    }

    //segment 1
    for (int i = firstBlock; i < numOfBlocks+firstBlock; i++) {
        blocks[i] = 0;
        for (int j = 0; j < 8; j++) {
            ram[i*8+j] = ' ';
        }
    }
    if(ramSem.value_sem() <= 0){
        if(ramSem.signal_sem());
    }
    pcb->segTab[0]->vi = 1;
    pcb->segTab[1]->vi = 1;
    return 1;
}

/**
 * Converts logical address to physical address in RAM.
 * 
 * Adds physical address of the segment's beginning to logical address.
 * 
 * @param pcb Pointer to PCB needed to get data's physical address.
 * @param segment Int specifing VM segment: 0 for text, 1 for data.
 * @param logAddr Int specifing logical address.
 * @return Int specifing physical address.
 */
int Ram::physAddr(PCB* pcb, int segment, int logAddr) {
    return logAddr+pcb->segTab[segment]->baseRAM;
}

/**
 * Informs if RAM contains segment.
 * 
 * Returns the value of v/i bit from segment table.
 * 
 * @param pcb Pointer to PCB needed to get data's v/i bit.
 * @param segment Int specifing VM segment: 0 for text, 1 for data.
 * @return true if RAM contains segment or false for not.
 */
bool Ram::isInRam(PCB* pcb, int segment) {
    return pcb->segTab[segment]->vi;
}

void Ram::printAllRam() {
    std::cout << "RAM" << std::endl;
    for (int i = 0; i < 512; i++) {
        std::cout << i << "   " << ram[i] << std::endl;
    }
}

void Ram::printRam(int start, int stop) {
   std::cout << "RAM from " << start << " to " << stop << std::endl;
    for (int i = start; i < stop+1; i++) {
        std::cout << i << "   " << ram[i] << std::endl;
    } 
}

void Ram::printProcess(PCB* pcb) {
    std::cout << pcb->getPid() << std::endl;
    std::cout << "Segment text" << std::endl;
    for (int i = pcb->segTab[0]->baseRAM; i < pcb->segTab[0]->baseRAM + pcb->segTab[0]->limit; i++) {
        std::cout << i << "   " << ram[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Segment data" << std::endl;
    for (int i = pcb->segTab[1]->baseRAM; i < pcb->segTab[1]->baseRAM + pcb->segTab[1]->limit; i++) {
        std::cout << i << "   " << ram[i] << std::endl;
    }
}

void Ram::printSegment(PCB* pcb, int segment) {
    std::cout << pcb->getPid() << std::endl;
    if (segment == 0) std::cout << "Segment text" << std::endl;
    else if (segment == 1) std::cout << "Segment data" << std::endl;
    for (int i = pcb->segTab[segment]->baseRAM; i < pcb->segTab[segment]->baseRAM + pcb->segTab[segment]->limit; i++) {
        std::cout << i << "   " << ram[i] << std::endl;
    }
}

void Ram::printMessage(int ramAddr) {
    int space = 0;
    int i = ramAddr;

    while (space != 2) {
        if (ram[i] == ' ') space++;
        if(space==2) return;
        std::cout << i << "   " << ram[i] << std::endl;
        i++;
    }
}

void Ram::printSemaphore() {
    ramSem.print_value();
    ramSem.print_queue();
}