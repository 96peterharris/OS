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

class PCB//
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



	class Compare
	{
	public:
		bool operator() (PCB* p1, PCB* p2)
		{
			if (p1->priority > p2->priority) {
				return true;
			}
			else {
				false;
			}
		}
	};






	static std::priority_queue < PCB*, std::vector<PCB*>, Compare> readyQueue; //Real Time processes


	std::vector<std::string> openedFilesList;

	int getProcessAddress();
	void setProcessAddress(int _processAddress);

	//static void sortMapByPriority();
	static void addToReadyQueue(PCB* pcb);

	//typedef std::function<bool(std::pair<std::string, PCB*>, std::pair<std::string, PCB*>)> Comparator;

	/*lass MapComparer {

		Comparator compFunctor = [](std::pair<std::string, PCB*> elem1, std::pair<std::string, PCB*> elem2)
		{
			return elem1.second->priority < elem2.second->priority;
		};
	};*/

	static std::map<std::string, PCB*> processesMap;
	//memoryPointer 

public:
	PCB() {};
	PCB(std::string _pid, int _processAddress, int _priority, State _state);
	~PCB() {};


	static std::priority_queue < PCB*, std::vector<PCB*>, Compare> getReadyProccesses();
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

