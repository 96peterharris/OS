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

	int findFreeSpace(int limit);
	void loadProg(PCB *pcb, std::string data);
	void DeleteProg(PCB *pcb);
	std::string getSegment(PCB *pcb, const int segment);
	void loadToVM(PCB *pcb, const std::string data);
};

