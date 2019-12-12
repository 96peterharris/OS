#pragma once
#include <array>
#include "PCB.hpp"
#include "RAM.hpp"

class SegmentPCB {
public:
	int baseVM;
	int baseRAM;
	int limit;
	bool vi;
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
	int findFreeSpace(const int limit);
}VM;
