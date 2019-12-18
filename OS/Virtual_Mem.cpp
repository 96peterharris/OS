
#include "Virtual_Mem.hpp"
#include "RAM.hpp"
#include "Headers.h"

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
	size_t size = pfSegTab.size();
	if (size == 0) return 0;
	for (int i = 0; i < size; i++) { //check whole segment table
		if (i == 0 && pfSegTab.at(i).base >= limit) { //if 1st segment and it's base starts further than new limit, then new segments fits at(0)
			return 0;
		}
		else if (i > 0 && pfSegTab.at(i).base - (pfSegTab.at(i - 1).base + pfSegTab.at(i - 1).limit) >= limit) { //if (base - prev. seg. base + prev. seg. limit >= curr. limit) then enough space has been found
			return pfSegTab.at(i - 1).base + pfSegTab.at(i - 1).limit;
		}
		else if (i == size - 1) {
			return pfSegTab.at(i).base + pfSegTab.at(i).limit;
		}
	}
	return -1; //not enough memory in pagefile (VM)
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

bool Virtual_Mem::createProg(PCB *pcb, std::string data)
{
	std::vector<SegmentPCB*>* segTab = pcb->PCB::getSegTab();
	std::string ramString;
	int segTabSize = 0;
	size_t sLength;
	size_t textBegin = data.find(".text"); //.text,.data = 5 signs
	size_t dataBegin = data.find(".data");
	if (textBegin != std::string::npos) {// != -1  means that segment exists
		segTabSize++;
	}
	if (dataBegin != std::string::npos) {
		segTabSize++;
	}

	int k = 0;
	for (int i = 0; i < segTabSize; i++) { //for every segment in program
		size_t freeSpace = size_t(findFreeSpace(data.length()));
		if (freeSpace == -1) {
			return false;
		}
		int maxPos = 0, index = 0;
		k += 6;
		std::string snumber;
		int number = 0;
		int correction = 6; //6 is length of".text " or ".data "
		if (i == 0 && segTabSize == 2) {
			sLength = dataBegin - 1;
		}
		else {
			sLength = data.length();
		}
		int pos = 0;
		while (k < sLength) {
			if (i == 0) {
				if (data.at(k - 1) == ' ' && isdigit(data.at(k))) {
					int numLength = -1;//20-22
					for (int i = 0; i < 3; i++) {
						numLength++; //count how many digits were found to correct index in pagefile later
						snumber += data.at(k + i);
						if (data.length() == (k + i + 1) || data.at(k + i + 1) == ' ') { //if next byte is not digit, it means we found whole number, convert it to int
							number = std::stoi(snumber);
							snumber.clear(); //clear string as preparation for next number
							pagefile.at(freeSpace + pos) = number; //fill pagefile byte with converted number
							ramString += number;
							k += numLength;
							correction += numLength;
							pos++;
							break;
						}
					}
				}
				else { //if byte is not a number, just copy it to pagefile (using correction counted earlier)
					pagefile.at(freeSpace + pos) = data.at(k);
					ramString += data.at(k);
					pos++;
				}
			}
			else {
				if (data.at(k - 1) == '[' && isdigit(data.at(k))) {
					int numLength = -1;//20-22
					for (int i = 0; i < 3; i++) {
						numLength++; //count how many digits were found to correct index in pagefile later
						snumber += data.at(k + i);
						if (data.length() == (k + i + 1) || data.at(k + i + 1) == ']') { //if next byte is not digit, it means we found whole number, convert it to int
							index = std::stoi(snumber);
							snumber.clear(); //clear string as preparation for next number
							k += numLength;
							correction += numLength;
							//pos++;
							if (index > maxPos) maxPos = index;
							break;
						}
					}
				}
				else if (data.at(k - 1) == ' ' && isdigit(data.at(k))) {
					int numLength = -1;//20-22
					for (int i = 0; i < 3; i++) {
						numLength++; //count how many digits were found to correct index in pagefile later
						snumber += data.at(k + i);
						if (data.length() == (k + i + 1) || data.at(k + i + 1) == ' ') { //if next byte is not digit, it means we found whole number, convert it to int
							number = std::stoi(snumber);
							snumber.clear(); //clear string as preparation for next number
							pagefile.at(freeSpace + index) = number; //fill pagefile byte with converted number
							for (int j = 0; j <= index; j++) {
								if (ramString.size() == j) {
									if (j == index)
										ramString += number;
									else ramString += '\0';
								}
								else if (j == index) {
									ramString.at(index) = number;
								}
							}
							k += numLength;
							correction += numLength;
							pos++;
							break;
						}
					}
				}
				else {
					pagefile.at(freeSpace + pos) = 0;
					ramString += '\0';
					if (pos > maxPos) maxPos = pos;
				}
			}
			k++;
		}
		if (i == 1 && maxPos < ramString.size()) ramString.pop_back();
		SegmentVM segment;
		segment.base = freeSpace;
		if (i == 0) { //text segment
			segment.limit = pos;
		}
		else { //data segment
			segment.limit = maxPos + 1;
		}
		pfSegTab.push_back(segment);

		SegmentPCB *segmentPCB = new SegmentPCB();
		segmentPCB->baseVM = segment.base;
		segmentPCB->limit = segment.limit;
		segmentPCB->vi = 0;
		segTab->push_back(segmentPCB);

		if (System::RAM.loadToRam(pcb, ramString, i) == false) {
			return false;
		}
		ramString.clear();
	}
	std::sort(pfSegTab.begin(), pfSegTab.end());
	return true;
}

bool Virtual_Mem::loadProg(PCB * pcb)
{
	std::vector<SegmentPCB*>* segTab = new std::vector<SegmentPCB*>;
	segTab = pcb->getSegTab();
	for (int i = 0; i < segTab->size(); i++) {
		if (segTab->at(i)->vi == 0) {
			std::string data;
			for (int k = segTab->at(i)->baseVM; k < segTab->at(i)->baseVM + segTab->at(i)->limit; k++) {
				data += pagefile.at(k);
			}
			if (System::RAM.loadToRam(pcb, data, i) == false) {
				return false;
			}
		}
	}
	std::sort(pfSegTab.begin(), pfSegTab.end());
	return true;
}

/**
 * Deletes program from VM and RAM.
 *
 * Deletes program from RAM. Deletes row from segment table instead of deleting data for optimalization.
 *
 * @param pcb Pointer to Process Control Block, necessary to get segment table.
 * @param data Program code in string structure.
*/
bool Virtual_Mem::deleteProg(PCB *pcb)
{
	System::RAM.deleteFromRam(pcb);
	auto segTab = pcb->getSegTab();
	size_t size = segTab->size();
	for (int i = 0; i < size; i++) { //for every segment (.text, .data)
		SegmentVM segment = pfSegTab.at(i);
		for (int k = 0; k < pfSegTab.size(); k++) {
			if (segment.base == segTab->at(i)->baseVM && segment.limit == segTab->at(i)->limit) {
				pfSegTab.erase(pfSegTab.begin() + k);
				segTab->erase(segTab->begin() + i);
				i--;
				size--;
				std::cout << ":c";
				break;
			}
		}
	}
	std::sort(pfSegTab.begin(), pfSegTab.end());
	return true;
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
	std::vector<SegmentPCB*>* segTab = pcb->PCB::getSegTab();
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
bool Virtual_Mem::loadToVM(PCB * pcb, const std::string data)
{
	std::vector<SegmentPCB*>* segTab = pcb->PCB::getSegTab();
	int base = segTab->at(1)->baseVM;
	int limit = segTab->at(1)->limit;
	for (int i = 0; i < limit; i++) {
		pagefile.at(base + i) = data.at(i);
	}
	return true;
}

void Virtual_Mem::printPCBsegTab(std::string pid)
{
	PCB* pcb = PCB::getPCB(pid);
	if (pcb == nullptr) {
		std::cout << "Process doesn't exist.\n";
	}
	else {
		std::vector<SegmentPCB*>* segTab = pcb->getSegTab();
		std::cout << "Process " << pid << " segment table:\n";
		std::cout << " ----------------------------------\n";
		std::cout << " | BaseVM | BaseRAM | Limit | v/i |\n";
		for (int i = 0; i < segTab->size(); i++) {
			std::cout << " | ";
			if (segTab->at(i)->baseVM > 999)	  std::cout << "  ";
			else if (segTab->at(i)->baseVM > 99) std::cout << "   ";
			else if (segTab->at(i)->baseVM > 9)  std::cout << "    ";
			else								  std::cout << "     ";
			std::cout << segTab->at(i)->baseVM << " | ";

			if (segTab->at(i)->baseRAM > 99)	  std::cout << "    ";
			else if (segTab->at(i)->baseRAM > 9) std::cout << "     ";
			else								  std::cout << "      ";
			std::cout << segTab->at(i)->baseRAM << " | ";

			if (segTab->at(i)->limit > 999) 	 std::cout << " ";
			else if (segTab->at(i)->limit > 99) std::cout << "  ";
			else if (segTab->at(i)->limit > 9)  std::cout << "   ";
			else if (segTab->at(i)->limit > 0)  std::cout << "    ";
			std::cout << segTab->at(i)->limit << " |  ";
			std::cout << segTab->at(i)->vi << "  |\n";
		}
		std::cout << " ----------------------------------\n";
	}
}

void Virtual_Mem::printVMsegTab()
{
	std::cout << "Pagefile segment table:\n";
	std::cout << " ----------------\n";
	std::cout << " | Base | Limit |\n";
	for (int i = 0; i < pfSegTab.size(); i++) {
		std::cout << " | ";
		if (pfSegTab.at(i).base > 999)     std::cout << "";
		else if (pfSegTab.at(i).base > 99) std::cout << " ";
		else if (pfSegTab.at(i).base > 9)  std::cout << "  ";
		else							    std::cout << "   ";
		std::cout << pfSegTab.at(i).base << " | ";

		if (pfSegTab.at(i).limit > 999)     std::cout << " ";
		else if (pfSegTab.at(i).limit > 99) std::cout << "  ";
		else if (pfSegTab.at(i).limit > 9)  std::cout << "   ";
		else								 std::cout << "    ";
		std::cout << pfSegTab.at(i).limit << " |\n";
	}
	std::cout << " ----------------\n";
}

void Virtual_Mem::printPCBsegments(std::string pid)
{
	PCB* pcb = PCB::getPCB(pid);
	if (pcb == nullptr) {
		std::cout << "Process doesn't exist.\n";
	}
	else {
		auto segTab = pcb->getSegTab();
		std::cout << "Process " << pid << " segments content:\n";
		for (int k = 0; k < segTab->size(); k++) {
			if (k == 0) {
				std::cout << "Segment .text:\n";
				for (int i = 0; i < 60; i++) {
					if (i == 0) std::cout << " ";
					else std::cout << "-";
				}
				std::cout << "\n| 0  ";
				for (int i = 1; i < 20; i++) {
					std::cout << i << " ";
					if (i < 9) std::cout << " ";
				}
				std::cout << "|\n ";
				for (int i = 0; i < 59; i++) {
					std::cout << "-";
				}
				std::cout << " ";
				int last = segTab->at(0)->limit;
				for (int i = 0; i < last; i++) {
					if (i == 0) std::cout << "\n| " << pagefile.at(segTab->at(0)->baseVM + i) << "  ";
					else if (i % 20 == 0) std::cout << "|\n| " << pagefile.at(segTab->at(0)->baseVM + i) << "  ";
					else if (i % 20 == 19) std::cout << pagefile.at(segTab->at(0)->baseVM + i) << " ";
					else std::cout << pagefile.at(segTab->at(0)->baseVM + i) << "  ";
				}
				std::cout << ">END<";
				std::cout << " \n\n";
			}
			else {
				std::cout << "Segment .data:\n";
				for (int i = 0; i < 31; i++) {
					if (i == 0) std::cout << " ";
					else std::cout << "-";
				}
				std::cout << "\n| 0  ";
				for (int i = 1; i < 10; i++) {
					std::cout << i << " ";
					if (i < 9) std::cout << " ";
				}
				std::cout << "|\n ";
				for (int i = 0; i < 30; i++) {
					std::cout << "-";
				}
				std::cout << " ";
				int last = segTab->at(1)->limit;
				for (int i = 0; i < last; i++) {
					if (i == 0) std::cout << "\n| " << pagefile.at(segTab->at(1)->baseVM + i) << "  ";
					else if (i % 10 == 0) std::cout << "|\n| " << pagefile.at(segTab->at(1)->baseVM + i) << "  ";
					else if (i % 10 == 9) std::cout << pagefile.at(segTab->at(1)->baseVM + i) << " ";
					else std::cout << pagefile.at(segTab->at(1)->baseVM + i) << "  ";
				}
				std::cout << ">END<";
				std::cout << " \n\n";
			}
		}
	}
}


void Virtual_Mem::printVM()
{
	int last = pfSegTab.at(pfSegTab.size() - 1).base + pfSegTab.at(pfSegTab.size() - 1).limit - 1;

	std::cout << "Pagefile bytes:\n";
	for (int i = 0; i < 90; i++) {
		if (i == 0) std::cout << " ";
		else std::cout << "-";
	}
	std::cout << "\n| 0  ";
	for (int i = 1; i < 30; i++) {
		std::cout << i << " ";
		if (i < 9) std::cout << " ";
	}
	std::cout << "|\n ";
	for (int i = 0; i < 89; i++) {
		std::cout << "-";
	}
	std::cout << " ";
	for (int i = 0; i < last; i++) {
		if (i == 0) std::cout << "\n| " << pagefile.at(i) << "  ";
		else if (i % 30 == 0) std::cout << "|\n| " << pagefile.at(i) << "  ";
		else if (i % 30 == 29) std::cout << pagefile.at(i) << " ";
		else std::cout << pagefile.at(i) << "  ";
	}
	std::cout << " \n\n";
}

bool SegmentVM::operator<(const SegmentVM & segVM) const
{
	return (base < segVM.base);
}
