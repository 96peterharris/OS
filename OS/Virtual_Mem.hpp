#pragma once
#include <array>
#include <vector>
#include <regex>
#include <locale>
#include "PCB.hpp"

class SegmentPCB {
public:
	int baseVM;
	int baseRAM;
	int limit;
	bool vi;

	int getBaseVM() { return baseVM; }
	int getBaseRAM() { return baseRAM; }
	int getLimit() { return limit; }
	bool getVI() { return vi; }
};

class SegmentVM {
public:
	int base;
	int limit;

	bool operator<(const SegmentVM& segVM) const;
};

class Virtual_Mem
{
private:
	std::array<char, 4096> pagefile;
	std::vector<SegmentVM> pfSegTab; //Pagefile Segment Table
	//std::vector<SegmentPCB*>* segTab; - stored in PCB
public:
	Virtual_Mem();
	~Virtual_Mem();
	bool createProg(PCB *pcb, std::string data);
	bool loadProg(PCB *pcb);
	bool deleteProg(PCB *pcb);
	std::string getSegment(PCB *pcb, const int segment);
	bool loadToVM(PCB *pcb, const std::string data);

	void printPCBSegments(PCB *pcb);
	void printVMSegTab();
	void printVM(PCB *pcb);
	void printPCBSegTab();

private:
	int findFreeSpace(int limit);
}VM;
