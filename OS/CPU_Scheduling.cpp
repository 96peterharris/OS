//Pioter
#include "CPU_Scheduling.hpp"
#include "Interpreter.hpp"


CPU_Scheduling::CPU_Scheduling() 
{
	this->commandCounter = 0;
	this->running = NULL;
}
void CPU_Scheduling::increasePriority()
{
	for (int i = 0; i < recivedQueue->size()-1; i++)
	{
		if (recivedQueue->at(i)->getPriority() < 12)
		{
			int tmp = recivedQueue->at(i)->getPriority();
			tmp + 3;
			recivedQueue->at(i)->setPriority(tmp);
		}
		else 
		{
			recivedQueue->at(i)->setPriority(15);
		}
	}
}
void CPU_Scheduling::addToTerminatedVec(PCB* pcb) {
	this->addToTerminatedVec(pcb);
}
void CPU_Scheduling::getProcesses() {

	this->recivedQueue = PCB::getReadyQueuePointer();
}
void CPU_Scheduling::cpu_sch() 
{
	if (recivedQueue->size() > 1) 
	{
		PCB::update();							//Updating process queue
		running = recivedQueue->at(0);
		running->setRunning(); //Attention!!!
		commandCounter = 0;
		VM.loadProg(running); //robbert musi utworzyæ
	}
	else 
	{
		recivedQueue = PCB::getReadyQueuePointer();
		
		/*if (recivedQueue->size() > 1)
		{
			running = recivedQueue->at(0);
		}
		else
		{
			running = recivedQueue->at(0);
		}*/

		running = recivedQueue->at(0);
		running->setTerminated();
		commandCounter = 0;
		VM.loadProg(running); //robbert musi utworzyæ
	}
}
void CPU_Scheduling::nexStep()
{
	if ((commandCounter < 5) && (PCB::NEW_PROCESS = false)) 
	{
		if (interprate(running) == true)
		{	
			commandCounter++;
		}
		else
		{
			running->setTerminated(); //eryk zmieni³ funkcje
			//haltProcess(running->getPid());
			running->setCommandCounter(+commandCounter); //attention!!!!
			commandCounter = 0;
			increasePriority();
			cpu_sch();
		}
	}
	else
	{
		running->setTerminated();
		running->setCommandCounter(+commandCounter);
		commandCounter = 0;
		increasePriority();
		cpu_sch();
	}
}
std::string CPU_Scheduling::getRunningPID() {
	return running->getPid();
}
void CPU_Scheduling::displayPCBqueue()
{
	std::cout << "PID\t PRIORITY \tSTATE" << std::endl;

	for(int i = 0; i < recivedQueue->size(); i++)
	{
		std::cout << recivedQueue->at(i)->getPid() << "\t " << recivedQueue->at(i)->getPriority() << "\t" << recivedQueue->at(i)->getState() << std::endl;
	}
}
void CPU_Scheduling::displayRunning()
{
	PCB::printPCB(running->getPid());
}