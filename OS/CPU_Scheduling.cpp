#include "CPU_Scheduling.hpp"

#include "Headers.h"

CPU_Scheduling::CPU_Scheduling()
{
	this->commandCounter = 0;
	this->running = nullptr;
}
void CPU_Scheduling::increasePriority()
{
	for (int i = 0; i < recivedQueue->size() - 1; i++)
	{
		if (recivedQueue->at(i)->getPriority() < 12 && recivedQueue->at(i)->getPid() != "DM")
		{
			int tmp = recivedQueue->at(i)->getPriority();
			tmp += 3;
			recivedQueue->at(i)->setPriority(tmp);
		}
		else if (recivedQueue->at(i)->getPriority() > 12 && recivedQueue->at(i)->getPid() != "DM")
		{
			recivedQueue->at(i)->setPriority(15);
		}
	}
}
void CPU_Scheduling::getProcesses() {

	this->recivedQueue = PCB::getReadyQueuePointer();
}
void CPU_Scheduling::cpu_sch()
{
	if (recivedQueue == nullptr)
	{
		getProcesses();						//Updating process queue
		running = recivedQueue->at(0);
		running->setRunning(); //Attention!!!
		commandCounter = 0;
		System::VM.loadProg(running); //robbert musi utworzyæ
	}
	else
	{
		PCB::update();							//Updating process queue
		running = recivedQueue->at(0);
		running->setRunning(); //Attention!!!
		commandCounter = 0;
		System::VM.loadProg(running); //robbert musi utworzyæ
	}
		
	
	/*else if()
	{
		getProcesses();
		running = recivedQueue->at(0);
		commandCounter = 0;
		running->setRunning();
		System::VM.loadProg(running); //robbert musi utworzyæ

	}*/
}
void CPU_Scheduling::nexStep()
{
	/*if (recivedQueue == nullptr)
	{
		//cpu_sch();
		getProcesses();

	}*/
	
	if ((commandCounter < 5) && (PCB::NEW_PROCESS == false))
	{
		if (interprate(running) == true)
		{
			std::cout << "\n interpreter true";
			commandCounter++;
		}
		else
		{
			cpu_sch();
			running->setTerminated(); //eryk zmieni³ funkcje
			//haltProcess(running->getPid());
			int tmp = running->getCommandCounter();
			tmp += commandCounter;
			running->setCommandCounter(tmp); //attention!!!!
			commandCounter = 0;
			increasePriority();
		}
	}
	else if ((commandCounter < 5) && (PCB::NEW_PROCESS == true))
	{
		cpu_sch();
		if (interprate(running) == true)
		{
			std::cout << "\n interpreter true";
			commandCounter++;
			PCB::NEW_PROCESS = false;
		}

	}
	else
	{
		running->setReady();
		int tmp = running->getCommandCounter();
		tmp += commandCounter;
		running->setCommandCounter(tmp);
		cpu_sch();
		commandCounter = 0;
		PCB::NEW_PROCESS = false;
		increasePriority();
	}
}
std::string CPU_Scheduling::getRunningPID() {
	return running->getPid();
}
void CPU_Scheduling::displayPCBqueue()
{
	std::cout << "PID\t PRIORITY \tSTATE" << std::endl;

	for (int i = 0; i < recivedQueue->size(); i++)
	{
		std::cout << recivedQueue->at(i)->getPid() << "\t " << recivedQueue->at(i)->getPriority() << "\t" << recivedQueue->at(i)->getState() << std::endl;
	}
}
void CPU_Scheduling::displayRunning()
{
	PCB::printPCB(running->getPid());
}