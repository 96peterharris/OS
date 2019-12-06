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

	//Vector of recived processes
	std::vector<PCB*>* recivedQueue; 
	//Vector of Terminated processes
	std::vector<std::string> terminatedPrcID;
	//Command counter (0-5) inside this class
	int commandCounter;
	//Current running process
	PCB* running;

	//This method is use for increase priority
	void increasePriority();
	//This method add terminated processes to the vector with terminated processes
	void addToTerminatedVec(PCB* pcb);
	//This method is use for download vector with PCB's in READY state
	void getProcesses();

public:
	//Coinstructor
	CPU_Scheduling();
	~CPU_Scheduling() {};

	//This method is responsible for cpu scheduling and it use another methods in this class
	void cpu_sch();
	//This method is responsible for make nex step (for example increase commandCounter) and it use another methods in this class
	void nexStep();

};

