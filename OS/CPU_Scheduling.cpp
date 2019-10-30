//Pioter
#include "CPU_Scheduling.hpp"



CPU_Scheduling::CPU_Scheduling() {}
void CPU_Scheduling::increasePriority() {

	std::queue <PCB*>* tempQueue = recivedQueue;
	PCB* tmp = this->recivedQueue->front();
	readyQueue.pop();

	while (!readyQueue.front()) {
		readyQueue.pop();
	}

	while (!tempQueue->empty()) {
		PCB* tempPCB = tempQueue->front();

		if (tempPCB->getPriority() < 12) {
			int tmp = tempPCB->getPriority();
			tmp + 3;
			tempPCB->setPriority(tmp);
		}
		else {
			tempPCB->setPriority(15);
		}

		readyQueue.push(tempPCB);
		tempQueue->pop();
	}
}
void CPU_Scheduling::changeState(PCB* pcb) {
	pcb->setState(State::TERMINATED);
}
void CPU_Scheduling::addToTerminatedVec(PCB* pcb) {
	this->addToTerminatedVec(pcb);
}
std::queue<PCB*>* CPU_Scheduling::getProcesses() {

	return this->recivedQueue = getReadyProccesses();
}
void CPU_Scheduling::countingDown() {
	for (int i = 1; i < 6; i++) {
		std::cout << " Commands counter: " << i;
		std::cout << "\x1B[2K\r";
	}
}

void CPU_Scheduling::running() {


	while (true) {

		countingDown();//Imitacja dummy taka tymczasowa

		while (!readyQueue.empty()) {
			for (int i = 0; i < 5; i++) {

				if (commandCounter == readyQueue.front()->getCommandCounter()) {
					this->commandCounter = 0;
					break;
				}

				std::cout << readyQueue.front()->getPid() << " Priority: " << readyQueue.front()->getPriority();

				increasePriority();
				commandCounter++;
			}
			std::cout << std::endl;
		}
	}


}