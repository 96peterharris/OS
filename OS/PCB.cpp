//Enrique
#include "PCB.hpp"
#include <vector>

//Constructor 
PCB::PCB(std::string pid, int processAddress, short priority, State state) : priority_default(priority), priority(priority) {
	//all the shitty init goes here;
}

PCB::~PCB() {
	//bruh leave ram empty etc
}

std::vector<PCB*>* PCB::getReadyQueuePointer() {
	std::vector<PCB> y;
	for (auto x : processesMap) {
		if (x.second->getState() == READY) {
			
		}
	}
}

//Check all of the find methods and try something else
bool createProcess(std::string pid, int processAddress, short priority) {
	std::map<std::string, PCB*>* tmp = PCB::getProcessMapPointer();
	std::map<std::string, PCB*>::iterator it = tmp->find(pid);

	if (pid == it->first) {
		return false;
	}
	else {
		PCB* pcb = new PCB(pid, processAddress, priority, NEW);
		tmp->insert(std::pair<std::string, PCB*>(pid, pcb));
		return true;
	}
}

bool removeProcess(std::string pid) {
	std::map<std::string, PCB*>* tmp = PCB::getProcessMapPointer();
	std::map<std::string, PCB*>::iterator it = tmp->find(pid);

	if (pid == it->first) {
		//it->second->setState(TERMINATED);
		tmp->erase(it);
		//call fbi remove etc
		return true;
	}
	else {
		return false;
	}
}
bool resumeProcess(std::string pid) {
	std::map<std::string, PCB*>* tmp = PCB::getProcessMapPointer();
	std::map<std::string, PCB*>::iterator it = tmp->find(pid);

	if (pid == it->first) {
		//it->second->state == State::READY;
		return true;
	}
	else {
		return false;
	}
}
bool haltProcess(std::string pid) {
	std::map<std::string, PCB*>* tmp = PCB::getProcessMapPointer();
	std::map<std::string, PCB*>::iterator it = tmp->find(pid);

	if (pid == it->first) {
		//it->second->state == State::WAITING;
		return true;
	}
	else {
		return false;
	}
}

bool update() {
	return false;
}