//Pioter
#pragma once
#include "PCB.hpp"
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

class CPU_Scheduling : protected PCB
{
private:



	std::priority_queue<PCB*, std::vector<PCB*>, Compare> recivedQueue; //Queue of processes
	std::vector<PCB*> terminatedProcesses; //Vector of Terminated processes

	void increasePriority();
	void changeState(PCB* pcb);
	void addToTerminatedVec(PCB* pcb);
	std::priority_queue < PCB*, std::vector<PCB*>, Compare> getProcesses();
	void countingDown();

public:
	CPU_Scheduling();
	~CPU_Scheduling() {};

	void running();

};

