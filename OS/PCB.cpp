//Enrique
#include "PCB.hpp"


bool PCB::NEW_PROCESS = false;
std::map<std::string, PCB*> PCB::processesMap;
std::vector<PCB*> PCB::readyQueue;

//Constructor 
PCB::PCB(std::string pid, int processAddress, short priority, State state) : priority_default(priority), priority(priority) {
	if (state == NEW) {
		this->state = READY;
		this->pid = pid;
		this->processAddress = processAddress;
		this->reg = Register();
	}
}

PCB::~PCB() {

}

PCB* PCB::getPCB(std::string pid) {
	for (auto x : processesMap) {
		if (x.first == pid) {
			return x.second;
		}
	}
	return nullptr;
}


std::vector<PCB*>* PCB::getReadyQueuePointer() {
	//Update the queue -> sort protity and return
	update();
	return &readyQueue;
}

bool PCB::update() {
	readyQueue.clear();
	for (auto x : processesMap) {
		if (x.second->getState() == READY) {
			readyQueue.push_back(x.second);
		}
	}
	std::sort(readyQueue.begin(), readyQueue.end(), [](PCB* a, PCB* b) { return a->getPriority() > b->getPriority(); });
	return true;
}

//Check all of the find methods and try something else
bool PCB::createProcess(std::string pid, int processAddress, short priority) {
	if (getPCB(pid) != nullptr) {
		return false;
	}
	else {
		//This should to everything
		PCB* pcb = new PCB(pid, processAddress, priority, NEW);

		processesMap.insert(std::pair<std::string, PCB*>(pid, pcb));
		NEW_PROCESS = true;
		return true;
	}
}

bool PCB::removeProcess(std::string pid) {
	PCB* tPCB = getPCB(pid);
	if (getPCB(pid) != nullptr) {
		tPCB->setTerminated();
		processesMap.erase(pid);

		//call fbi remove etc

		return true;
	}
	else {
		return false;
	}
}

bool PCB::resumeProcess(std::string pid) {
	PCB* tPCB = getPCB(pid);
	if (tPCB == nullptr) {
		tPCB->setReady();
		return true;
	}
	else {
		return false;
	}
}

bool PCB::haltProcess(std::string pid) {
	PCB* tPCB = getPCB(pid);
	if (tPCB == nullptr) {
		tPCB->setWaiting();
		return true;
	}
	else {
		return false;
	}
}

void PCB::printPCB(std::string pid) {
	PCB* tPCB = getPCB(pid);
	std::cout << tPCB->pid << " "
		<< tPCB->getDefaultPriority() << " " << tPCB->getPriority() << " " << tPCB->getState() << " "
		<< (int)tPCB->getRegisterPointer()->getA() << " " << (int)tPCB->getRegisterPointer()->getB() << " " << (int)tPCB->getRegisterPointer()->getC() << " " << (int)tPCB->getRegisterPointer()->getD() << " "
		<< std::endl;
}

void PCB::printReadyQueue() {
	std::cout << "PID\t PRIORITY" << std::endl;
	for (auto x : readyQueue) {
		std::cout << x->pid << "\t " << x->priority << std::endl;
	}
}

void PCB::printMap() {
	for (auto x : processesMap) {
		printPCB(x.first);
	}
}

bool PCB::readFile(std::string name, std::string &text) {
	std::fstream file(name);
	if (file.is_open()) {
		std::string f = "";

		while (std::getline(file, f)) {
			text += f + " ";
		}
	}
	else {
		return false;
	}
	return true;
}