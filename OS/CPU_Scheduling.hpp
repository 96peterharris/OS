//Pioter
#pragma once
#include "PCB.hpp"
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

class CPU_Scheduling
{
private:

	std::vector<PCB*>* recivedQueue; //Vector of Terminated processes
	std::vector<std::string> terminatedPrcID;
	int commandCounter;
	PCB* running;

	void increasePriority();
	void addToTerminatedVec(PCB* pcb);
	void getProcesses();

public:
	CPU_Scheduling();
	~CPU_Scheduling() {};

	void cpu_sch();
	void nexStep();

};

