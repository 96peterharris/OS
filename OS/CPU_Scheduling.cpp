//Pioter
#include "CPU_Scheduling.hpp"
#include "Interpreter.hpp"


CPU_Scheduling::CPU_Scheduling() {}
void CPU_Scheduling::increasePriority() {


	for (int i = 1; i < recivedQueue->size(); i++){
		PCB* tempPCB = recivedQueue->at(i);

		if (tempPCB->getPriority() < 12) {
			int tmp = tempPCB->getPriority();
			tmp + 3;
			tempPCB->setPriority(tmp);
		}
		else {
			tempPCB->setPriority(15);
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
		running == recivedQueue->at(1);
		commandCounter = running->getCommandCounter();

	}
	else {
	
		recivedQueue = PCB::getReadyQueuePointer();
		running == recivedQueue->at(1);
		commandCounter = running->getCommandCounter();
	}
}
void CPU_Scheduling::nexStep() {

	if (commandCounter < 5 && PCB::NEW_PROCESS = false) {

		if (interprate() == false) {		//dlaczego false co zrobiæ
			
			haltProcess(running->getPid());	// halt process
			running->setCommandCounter(commandCounter);
			commandCounter = 0;
			increasePriority();
			cpu_sch();
		}

		commandCounter++;
	}
	else {
		running->setState(State::READY);
		running->setCommandCounter(commandCounter);
		commandCounter = 0;
		increasePriority();
		cpu_sch();
	}
}