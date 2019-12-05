#pragma once
#include <array>
#include <vector>
#include "PCB.hpp"

class SegmentPCB {
public:
	int baseVM;
	int blockNo;
	int size;
	bool vi;
};

class Segment {
public:
	int base;
	int limit;
	bool vi;
};

class Virtual_Mem
{
private:
	std::array<char, 2048> pagefile;
	std::vector<Segment> pfSegTab; //Pagefile Segment Table
	//std::vector<SegmentPCB> segTab; - stored in PCB
public:
	Virtual_Mem();
	~Virtual_Mem();

	int findFreeSpace(int limit);
	void loadProg(PCB *pcb);
	void DeleteProg(PCB *pcb);
};

