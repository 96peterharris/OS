//Enrique
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <iterator>
#include <functional>

#include "State.hpp"
#include "Register.hpp"
#include "Virtual_Mem.hpp"

class PCB
{
protected:
	std::string pid;

	int processAddress;
	short priority;
	const short priority_default;
	int commandCounter;

	State state;
	Register reg;

	std::vector<std::string> openedFilesList;

	static std::map<std::string, PCB*> processesMap;
	static std::vector<PCB*> readyQueue;
	
	//memoryPointer 
	static std::vector<SegmentPCB*> segTab;

public:
	PCB() : priority_default(1) {}
	PCB(std::string pid, int processAddress, short priority, State state);
	~PCB() {};

	//Changing state inner function
	//todo calling running
	void removeProcess() { if(state == RUNNING) this->state = TERMINATED; }
	void resumeProcess() { if(state == WAITING) this->state = READY; }
	void haltProcess() { if(state == RUNNING) this->state = WAITING; }
	void setRunning() { if (state == READY) this->state = RUNNING; }

	//PID
	std::string getPid() { return this->pid; }
	void setPid(std::string pid) { this->pid = pid; }
	//Process address
	int getProcessAddress() { return this->processAddress; }
	void setProcessAddress(int processAddress) { this->processAddress = processAddress; }
	//Register
	Register getRegister() { return this->reg; }
	Register* getRegisterPointer() { return &this->reg; }
	//CC
	int getCommandCounter() { return this->commandCounter; }
	void setCommandCounter(int commandCounter) { this->commandCounter = commandCounter; }
	//Priority data
	short getDefaultPriority() { return this->priority_default; }
	short getPriority() { return this->priority; }
	void setPriority(short priority) { this->priority = priority; }
	//State
	State getState() { return this->state; }
	void setState(State state) { this->state = state; }
	//Map getter
	static std::map<std::string, PCB*>* getProcessMapPointer() { return &processesMap; }
	//"Queue" getter
	static std::vector<PCB*>* getReadyQueuePointer();
	//if there is need to update queue
	static bool NEW_PROCESS;
	//Segment table getter
	static std::vector<SegmentPCB*>* getSegTab() { return &segTab; }
};

bool PCB::NEW_PROCESS = false;

bool createProcess(std::string pid, int processAddress, short priority);
bool removeProcess(std::string pid);
bool resumeProcess(std::string pid);
bool haltProcess(std::string pid);
bool update();