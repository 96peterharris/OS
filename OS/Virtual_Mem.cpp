#include "Virtual_Mem.hpp"

Virtual_Mem::Virtual_Mem()
{
}


Virtual_Mem::~Virtual_Mem()
{
}

/**
 * Finds space in pagefile that could contain whole new segment.
 *
 * Checks pagefile segment table from the beginning if there is enough space to fit new segment. Returns segment base address.
 *
 * @param limit Size of the segment that is needed to fit in pagefile.
 * @return Int which is equal to pfSegTab index where new segment should be fit in.
*/
int Virtual_Mem::findFreeSpace(int limit)
{
	int size = pfSegTab.size();
	if (size == 0) return 0;
	for (int i = 0; i < size; i++) { //check whole segment table
		if (i == 0 && pfSegTab.at(i).base >= limit) { //if 1st segment and it's base starts further than new limit, then new segments fits at(0)
			return 0;
		}
		else if (pfSegTab.at(i).base - (pfSegTab.at(i - 1).base + pfSegTab.at(i - 1).limit) >= limit) { //if (base - prev. seg. base + prev. seg. limit >= curr. limit) then enough space has been found
			return pfSegTab.at(i - 1).base + pfSegTab.at(i - 1).limit;
		}
		else return -1; //not enough memory in pagefile (VM)
	}
}

/**
 * Loads program to Virtual Memory and RAM.
 *
 * Load program's code to both VM and RAM or just to RAM depending on if program was just created or not. 
 * If so, segment table is filled in pcb, segment table in VM is updated. 
 * Otherwise data is loaded from VM to RAM.
 *
 * @param pcb Pointer to Process Control Block, necessary to get segment table.
 * @param data Program code in string structure.
*/
void Virtual_Mem::loadProg(PCB *pcb, std::string data)
{
	auto segTab = pcb->getSegTab();
	int size = 0;
	int textBegin = data.find(".text") + 5; //.text,.data = 5 signs
	int dataBegin = data.find(".data") + 5;
	if (dataBegin != -1) size++; // != -1  means that segment exists
	if (textBegin != -1) size++;

	std::regex letter("[a-zA-z]");
	std::regex number("[0-9]");
	std::smatch result;

	for (int i = 0; i < size; i++) { //for every segment in program
		int freeSpace = findFreeSpace(data.length());
		std::string snumber;
		int number = 0;
		int correction = 0;
		for (int k = 0; k < data.length(); k++) { //filling per byte to pagefile in found free space of memory
			if (isdigit(data.at(k))) { //if bytes create a number, convert them to 1-sign digit
				snumber += data.at(k);
				correction++; //count how many digits were found to correct index in pagefile later
				if (!isdigit(data.at(k + 1))) { //if next byte is not digit, it means we found whole number, convert it to int
					number = std::stoi(snumber);
					snumber.clear(); //clear string as preparation for next number
					pagefile.at(freeSpace + k - correction) = number; //fill pagefile byte with converted number
				}
			}
			else { //if byte is not a number, just copy it to pagefile (using correction counted earlier)
				pagefile.at(freeSpace + k - correction) = data.at(k);
			}
		}

		if (segTab->size() == 0) { //segment table is empty, that means process is new and it is need to fill and update segment tables
			//creating segments which are going to be added to segment tables
			SegmentVM segment;
			segment.base = freeSpace;
			segment.limit = size;
			pfSegTab.push_back(segment);

			SegmentPCB* segmentPCB;
			segmentPCB->baseVM = freeSpace;
			segmentPCB->limit = size;
			segmentPCB->vi = 0;
			segTab->push_back(segmentPCB);
		}
		loadToRAM(&pcb, data, i);
	}
	std::sort(pfSegTab.begin(), pfSegTab.end());
}

/**
 * Deletes program from VM and RAM.
 *
 * Deletes program from RAM. Deletes row from segment table instead of deleting data for optimalization.
 *
 * @param pcb Pointer to Process Control Block, necessary to get segment table.
 * @param data Program code in string structure.
*/
void Virtual_Mem::DeleteProg(PCB *pcb)
{	
	deleteFromRam(&pcb);
	auto segTab = pcb->getSegTab();
	for (int i = 0; i < segTab->size(); i++) { //for every segment (.text, .data)
		for (auto k = pfSegTab.begin(); k != pfSegTab.end(); k++) { //find that segment i segment table and delete it
			if (k->base == segTab->at(i)->baseVM && k->limit == segTab->at(i)->limit) {
				pfSegTab.erase(k);
			}
		}
	}
	std::sort(pfSegTab.begin(), pfSegTab.end());
}

/**
 * Gets data from segment.
 *
 * Gets data from precised segment, returns it in string.
 *
 * @param pcb Pointer to Process Control Block, necessary to get segment table.
 * @param segment Number of segment which from data is being pulled.
 * @return String variable containing code from segment.
*/
std::string Virtual_Mem::getSegment(PCB *pcb, const int segment)
{
	auto segTab = pcb->getSegTab();
	int base = segTab->at(segment)->baseVM;
	int limit = segTab->at(segment)->limit;
	std::string data;
	for (int i = base; i < base + limit; i++) {
		data += pagefile.at(i);
	}
	return std::string(data);
}

/**
 * Loads code to Virtual Memory.
 *
 * Loads code to pagefile. Function used by RAM module to update .data segments.
 *
 * @param pcb Pointer to Process Control Block, necessary to get segment table.
 * @param data Updated program code in string structure.
*/
void Virtual_Mem::loadToVM(PCB * pcb, const std::string data)
{
	auto segTab = pcb->getSegTab();
	int base = segTab->at(1)->baseVM;
	int limit = segTab->at(1)->limit;
	for (int i = 0; i < limit; i++) {
		pagefile.at(base + i) = data.at(i);
	}
}

bool SegmentVM::operator<(const SegmentVM & segVM) const
{
	return (base < segVM.base);
}
