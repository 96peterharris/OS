//Pioter
#include "CPU_Scheduling.hpp"
#include "Interpreter.hpp"


CPU_Scheduling::CPU_Scheduling() 
{
	this->commandCounter = 0;
	this->running = NULL;
}
void CPU_Scheduling::increasePriority() {


	for (int i = 1; i < recivedQueue->size(); i++)
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
void CPU_Scheduling::cpu_sch() {

	if (recivedQueue->size() > 1) {
		update();							//Updating process queue
		running = recivedQueue->at(1);
		commandCounter = 0;

	}
	else {
	
		recivedQueue = PCB::getReadyQueuePointer();
		
		if (recivedQueue->size() > 1)
		{
			running = recivedQueue->at(1);
		}
		else
		{
			running = recivedQueue->at(0);
		}
		
		commandCounter = 0;
	}
}
void CPU_Scheduling::nexStep() {

	if (commandCounter < 5 && PCB::NEW_PROCESS = false) 
	{

		if (interprate() == false)
		{	
			
			haltProcess(running->getPid());	
			running->setCommandCounter(+commandCounter);
			commandCounter = 0;
			increasePriority();
			cpu_sch();
		}

		commandCounter++;
	}
	else
	{
		running->setState(State::READY);
		running->setCommandCounter(+commandCounter);
		commandCounter = 0;
		increasePriority();
		cpu_sch();
	}
}