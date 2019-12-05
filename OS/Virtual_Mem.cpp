#include "Virtual_Mem.hpp"

Virtual_Mem::Virtual_Mem()
{
}


Virtual_Mem::~Virtual_Mem()
{
}

int Virtual_Mem::findFreeSpace(int limit)
{
	int size = pfSegTab.size();
	if (size == 0) return 0;
	for (int i = 0; i < size; i++) {
		if (i == 0 && pfSegTab.at(i).base - 0 >= limit) {
			return 0;
		}
		//segment's base - physical adres of previous segment's end >= limit
		else if (pfSegTab.at(i).base - (pfSegTab.at(i - 1).base + pfSegTab.at(i - 1).limit) >= limit) {
			return pfSegTab.at(i - 1).base + pfSegTab.at(i - 1).limit;
		}
		else return -1;
	}
}

void Virtual_Mem::loadProg(PCB *pcb)
{
	auto segTab = pcb->getSegTab();
	int size = segTab->size();
	for (int i = 0; i < size; i++) {
		if (segTab->at(i)->baseVM == -1) {
			int freeSpace = findFreeSpace(PROG.length());
			for (int k = 0; k < PROG.length(); k++) {
				pagefile.at(freeSpace + k) = PROG.at(k);
			}

			segTab->at(i)->baseVM = freeSpace;
			segTab->at(i)->size = PROG.length();

			loadToRam(PROG, PROG.length());
			segTab->at(i)->vi = true;
		}
		else {
			std::string progCode;
			for (int k = segTab->at(i)->base; k < segTab->at(i)->size; k++) {
				progCode += pagefile.at(k);
			}
		}
	}
	
}

void Virtual_Mem::DeleteProg(PCB *_PCB)
{
}
