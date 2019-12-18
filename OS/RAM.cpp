#include "RAM.hpp"
#include "Headers.h"

Ram::Ram() {
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
 * @return true for success or false for failure.
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
            clearRam();

            //return 0;
			buddy(pcb, segment, bytes, divisionLvl);
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
				(pcb->messages.at(pcb->messages.size() - 1)).RAMadrress = startAddr;
				return true;
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
	//printAllRam();
    int space = 0;
    std::string msg = "";
    int i = ramAddr;

    while (space != 2) {
        if (ram[i] == ' ') space++;
        msg.push_back(ram[i]);
        i++;
    }
    msg.pop_back();

    size_t size = msg.size();
    /*
	int numOfBlocks;
    int num1 = size/8;
    int num2 = size%8;
    if (num2==0) numOfBlocks = num1;
    else numOfBlocks = num1+1;
	*/
	int blockSize;
	int numOfBlocks;
	for (int i = 3; i < 9; i++) {
		if (std::pow(2, i) >= size) {
			blockSize = std::pow(2, i);
			numOfBlocks = blockSize / 8;
			break;
		}
	}

    int firstBlock = ramAddr/8;

    for (int i = firstBlock; i < numOfBlocks+firstBlock; i++) {
        blocks[i] = 0;
        for (int j = 0; j < 8; j++) {
            ram[i*8+j] = ' ';
        }
    }
	//printAllRam();
    return msg;
}

/**
 * Deletes process from RAM.
 * 
 * Releases segment 0 by setting blocks as free and deleting data from RAM.
 * Does the same for segment 1 but before it, updates segment 1 if PCB state is WAITING.
 * 
 * @param pcb Pointer to PCB needed to get data's size, physical address.
 * @return true for success or false for failure.
 */
bool Ram::deleteFromRam(PCB* pcb) {
	int blockSize;
	int numOfBlocks;
	int firstBlock;
	if (isInRam(pcb, 0)) {
		//segment 0
		
		for (int i = 3; i < 9; i++) {
			if (std::pow(2, i) >= pcb->segTab[0]->limit) {
				blockSize = std::pow(2, i);
				numOfBlocks = blockSize / 8;
				break;
			}
		}

		firstBlock = pcb->segTab[0]->baseRAM / 8;

		for (int i = firstBlock; i < numOfBlocks + firstBlock; i++) {
			blocks[i] = 0;
			for (int j = 0; j < 8; j++) {
				ram[i * 8 + j] = ' ';
			}
		}
	}
		//segment 1
	if (pcb->segTab.size() == 2) {
		if (isInRam(pcb, 1)) {
			for (int i = 3; i < 9; i++) {
				if (std::pow(2, i) >= pcb->segTab[1]->limit) {
					blockSize = std::pow(2, i);
					numOfBlocks = blockSize / 8;
					break;
				}
			}

			firstBlock = pcb->segTab[1]->baseRAM / 8;


			//update
			if (pcb->getState() == WAITING) {
				std::string bytes;
				for (int i = firstBlock; i < numOfBlocks + firstBlock; i++) {
					for (int j = 0; j < 8; j++) {
						bytes.push_back(i * 8 + j);
					}
				}
				System::VM.loadToVM(pcb, bytes);
			}

			//segment 1
			for (int i = firstBlock; i < numOfBlocks + firstBlock; i++) {
				blocks[i] = 0;
				for (int j = 0; j < 8; j++) {
					ram[i * 8 + j] = ' ';
				}
			}
			pcb->segTab[1]->vi = 0;
		}
	}
	if (isInRam(pcb, 0))
	pcb->segTab[0]->vi = 0;
	return 1;
}

/**
* Clears RAM.
*
* Deletes both segments of every process from RAM.
*
* @return true for success or false for failure.
*/
bool Ram::clearRam() {
    std::map<std::string, PCB*>* map = PCB::getProcessMapPointer();
    for (auto x : *map) {
        deleteFromRam(x.second);
    }  
    return 1;
}

/**
* Deletes message from RAM.
*
* Deletes message saved in RAM.
*
* @param ramAddr Int meaning physical address in RAM where the message begins.
* @return true for success or false for failure.
*/
bool Ram::deleteMessage(int ramAddr) {
    int space = 0;
    int size = 0;
    int i = ramAddr;

    while (space != 2) {
        if (ram[i] == ' ') space++;
        size++;
        i++;
    }

	int blockSize;
	int numOfBlocks;
	for (int i = 3; i < 9; i++) {
		if (std::pow(2, i) >= size) {
			blockSize = std::pow(2, i);
			numOfBlocks = blockSize / 8;
			break;
		}
	}

	int firstBlock = ramAddr / 8;

    for (int i = firstBlock; i < numOfBlocks+firstBlock; i++) {
        blocks[i] = 0;
        for (int j = 0; j < 8; j++) {
            ram[i*8+j] = ' ';
        }
    }
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

/**
 * Prints whole RAM.
 * 
 * Prints all content of RAM. 8 characters in every line.
 */
void Ram::printAllRam() {
	std::cout << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "|||||||||||||||| RAM ||||||||||||||||" << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << std::endl;
	for (int i = -1; i < 8; i++) {
		if (i == -1) std::cout << "       ";
		else std::cout << i << "   ";
	}
	std::cout << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	for (int i = 0; i < 64; i++) {
		for (int j = -1; j < 8; j++) {
			if (j == -1) {
				if (i*8 < 10)
				std::cout << i*8 << "   |  ";
				else if (i*8 < 100)
					std::cout << i*8 << "  |  ";
				else
					std::cout << i * 8 << " |  ";
			}
			else std::cout << ram[i*8 + j] << "   ";
		}
		std::cout << std::endl;
	}
	std::cout << "-------------------------------------" << std::endl;
	std::cout << std::endl;
}

/**
 * Prints precised section of RAM.
 * 
 * Prints content of RAM from specified first element to last element. 8 characters in every line.
 * 
 * @param start Int specifing the first displayed element.
 * @param stop Int specifing the last displayed element.
 */
void Ram::printRam(int start, int stop) {

	int st = start / 8;
	int sp = stop / 8;
	if (stop % 8 != 0) sp++;

	std::cout << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "||||||||| RAM FROM " <<start << " TO " << stop << " |||||||||" << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << std::endl;
	for (int i = -1; i < 8; i++) {
		if (i == -1) std::cout << "       ";
		else std::cout << i << "   ";
	}
	std::cout << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	for (int i = st; i < sp; i++) {
		for (int j = -1; j < 8; j++) {
			if (j == -1) {
				if (i * 8 < 10)
					std::cout << i * 8 << "   |  ";
				else if (i * 8 < 100)
					std::cout << i * 8 << "  |  ";
				else
					std::cout << i * 8 << " |  ";
			}
			else std::cout << ram[i * 8 + j] << "   ";
		}
		std::cout << std::endl;
	}
	std::cout << "-------------------------------------" << std::endl;
	std::cout << std::endl;

}

/**
 * Prints content of process in RAM.
 * 
 * Prints segments of process saved in RAM. 8 characters in every line.
 * 
 * @param pid String specifing the pid of process to display.
 */
void Ram::printProcess(std::string pid) {
	PCB* pcb = PCB::getPCB(pid);
	std::cout << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "||||||||| PROCESS " << pid << " IN RAM |||||||||" << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << std::endl;

	int start;
	int stop;

	if (isInRam(pcb, 0)) {
		start = pcb->segTab[0]->baseRAM / 8;
		stop = (pcb->segTab[0]->baseRAM + pcb->segTab[0]->limit) / 8;
		if ((pcb->segTab[0]->baseRAM + pcb->segTab[0]->limit) % 8 != 0) stop++;
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "|||||||||||| SEGMENT TEXT |||||||||||" << std::endl;
		std::cout << "-------------------------------------" << std::endl;
		for (int i = -1; i < 8; i++) {
			if (i == -1) std::cout << "       ";
			else std::cout << i << "   ";
		}
		std::cout << std::endl;
		std::cout << "-------------------------------------" << std::endl;
		for (int i = start; i < stop; i++) {
			for (int j = -1; j < 8; j++) {
				if (j == -1) {
					if (i * 8 < 10)
						std::cout << i * 8 << "   |  ";
					else if (i * 8 < 100)
						std::cout << i * 8 << "  |  ";
					else
						std::cout << i * 8 << " |  ";
				}
				else std::cout << ram[i * 8 + j] << "   ";
			}
			std::cout << std::endl;
		}

	}
	if (pcb->segTab.size() == 2) {
		if (isInRam(pcb, 1)) {
			start = pcb->segTab[1]->baseRAM / 8;
			stop = (pcb->segTab[1]->baseRAM + pcb->segTab[1]->limit) / 8;
			if ((pcb->segTab[1]->baseRAM + pcb->segTab[1]->limit) % 8 != 0) stop++;
			std::cout << "-------------------------------------" << std::endl;
			std::cout << "|||||||||||| SEGMENT DATA ||||||||||||" << std::endl;
			std::cout << "-------------------------------------" << std::endl;
			for (int i = -1; i < 8; i++) {
				if (i == -1) std::cout << "       ";
				else std::cout << i << "   ";
			}
			std::cout << std::endl;
			std::cout << "-------------------------------------" << std::endl;
			for (int i = start; i < stop; i++) {
				for (int j = -1; j < 8; j++) {
					if (j == -1) {
						if (i * 8 < 10)
							std::cout << i * 8 << "   |  ";
						else if (i * 8 < 100)
							std::cout << i * 8 << "  |  ";
						else
							std::cout << i * 8 << " |  ";
					}
					else std::cout << ram[i * 8 + j] << "   ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	}
}

/**
 * Prints content of segment in RAM.
 * 
 * Prints specified segment of process saved in RAM. 8 characters in every line.
 * 
 * @param pid String specifing the pid of process to display.
 * @param segment Int specifing VM segment to display: 0 for text, 1 for data.
 */
void Ram::printSegment(std::string pid, int segment) {
	PCB* pcb = PCB::getPCB(pid);

	std::cout << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "|||| PROCESS " << pid << " SEGMENT " << segment << " IN RAM ||||" << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << std::endl;
	std::cout << "-------------------------------------" << std::endl;

		if (isInRam(pcb, segment)) {
			int start = pcb->segTab[segment]->baseRAM / 8;
			int stop = (pcb->segTab[segment]->baseRAM + pcb->segTab[segment]->limit) / 8;
			if ((pcb->segTab[segment]->baseRAM + pcb->segTab[segment]->limit) % 8 != 0) stop++;
			for (int i = -1; i < 8; i++) {
				if (i == -1) std::cout << "       ";
				else std::cout << i << "   ";
			}
			std::cout << std::endl;
			std::cout << "-------------------------------------" << std::endl;
			for (int i = start; i < stop; i++) {
				for (int j = -1; j < 8; j++) {
					if (j == -1) {
						if (i * 8 < 10)
							std::cout << i * 8 << "   |  ";
						else if (i * 8 < 100)
							std::cout << i * 8 << "  |  ";
						else
							std::cout << i * 8 << " |  ";
					}
					else std::cout << ram[i * 8 + j] << "   ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
}

/**
 * Prints message saved in RAM.
 * 
 * Prints message which starts at specified physical address.
 * 
 * @param ramAddr Int meaning physical address in RAM where the message begins.
 */
void Ram::printMessage(int ramAddr) {
    int space = 0;
    int i = ramAddr;

    while (space != 2) {
        if (ram[i] == ' ') space++;
        if(space==2) return;
        //std::cout << i << "   " << ram[i] << std::endl;
		std::cout << ram[i];
        i++;
    }
	std::cout << std::endl;
	std::cout << std::endl;
}