//Enrique
#pragma once
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <iterator>
#include <functional>
#include <fstream>

#include "State.hpp"
#include "Register.hpp"

class SegmentPCB;
class Message;

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
	std::vector<SegmentPCB*> segTab;
public:

	PCB() : priority_default(1) {}
	PCB(std::string pid, int processAddress, short priority, State state);
	~PCB();

	//Changing state inner function
	//todo calling running
	void setTerminated() { if (state == RUNNING) this->state = TERMINATED; }
	void setReady() { if (state == WAITING || state == RUNNING) this->state = READY; }
	void setWaiting() { if (state == RUNNING || state == READY) this->state = WAITING; }
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
	void setState(State &state) { this->state = state; }
	//Map getter
	static std::map<std::string, PCB*>* getProcessMapPointer() { return &processesMap; }
	//"Queue" getter
	static std::vector<PCB*>* getReadyQueuePointer();
	//if there is need to update queue
	static bool NEW_PROCESS;

	//Segment table getter
	std::vector<SegmentPCB*>* getSegTab() { return &segTab; }	

	//re did into pcb::function() as static ones
	static bool createProcess(std::string pid, int processAddress, short priority);
	//Terminates and deletes
	static bool removeProcess(std::string pid);
	//Changes state to READY
	static bool resumeProcess(std::string pid);
	//Changes state to WAITING
	static bool haltProcess(std::string pid);
	//Returns PCB
	static PCB* getPCB(std::string pid);
	//Update a Ready Process Queue
	static bool update();

	//File read and removing the spaces
	static bool readFile(std::string name, std::string &text);

	//Value prints
	static void printPCB(std::string pid);
	static void printMap();
	static void printReadyQueue();

	//To IPC work

	std::vector<Message*> messages;

	bool sendMessage(std::string pid_receiver, std::string content);
	bool receiveMessage();

};