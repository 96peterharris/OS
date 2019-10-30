//Enrique
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <iterator>
#include "State.hpp"
#include <functional>

class PCB
{
protected:
	std::string pid;

	int processAddress;
	int priority;
	int commandCounter;
	int registryA;
	int registryB;
	int registryC;
	int registryD;
	State state;

	static std::queue<PCB*> readyQueue; //Real Time processes


	std::vector<std::string> openedFilesList;

	int getProcessAddress();
	void setProcessAddress(int _processAddress);

	//static void sortMapByPriority();
	static void addToReadyQueue(PCB* pcb);

	static std::map<std::string, PCB*> processesMap;
	//memoryPointer 

public:
	PCB() {};
	PCB(std::string _pid, int _processAddress, int _priority, State _state);
	~PCB() {};


	static std::queue<PCB*>* getReadyProccesses();
	bool createProcess(std::string pid, int processAddress, int priority, State state);
	bool removeProcess(std::string pid);
	bool resumeProcess(std::string pid);
	bool haltProcess(std::string pid);

	std::string getPid();
	void setPid(std::string pid);
	int getRegistryA();
	void setRegistryA(int registryA);
	int getRegistryB();
	void setRegistryB(int registryB);
	int getRegistryC();
	void setRegistryC(int registryC);
	int getRegistryD();
	void setRegistryD(int registryD);
	int getCommandCounter();
	void setCommandCounter(int commandCounter);
	int getPriority();
	void setPriority(int priority);
	State getState();
	void setState(State state);


};

