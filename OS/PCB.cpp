//Enrique
#include "PCB.hpp"


PCB::PCB(std::string _pid, int _processAddress, int _priority, State _state) : pid(_pid), processAddress(_processAddress), priority(_priority), state(_state) {

	this->addToReadyQueue(new PCB("DUMMY0", 11, 0, NEW));
}
void PCB::addToReadyQueue(PCB * pcb) {
	readyQueue.push(pcb);
}
std::string PCB::getPid() {
	return this->pid;
}
void PCB::setPid(std::string pid) {
	this->pid = pid;
}
int PCB::getCommandCounter() {
	return this->commandCounter;
}
void PCB::setCommandCounter(int commandCounter) {
	this->commandCounter = commandCounter;
}
int PCB::getProcessAddress() {
	return this->processAddress;
}
void PCB::setProcessAddress(int processAddress) {
	this->processAddress = processAddress;
}
int PCB::getPriority() {
	return this->priority;
}
void PCB::setPriority(int priority) {
	this->priority = priority;
}
int PCB::getRegistryA() {
	return this->registryA;
}
void PCB::setRegistryA(int registryA) {
	this->registryA = registryA;
}
int PCB::getRegistryB() {
	return this->registryB;
}
void PCB::setRegistryB(int registryB) {
	this->registryB = registryB;
}
int PCB::getRegistryC() {
	return this->registryC;
}
void PCB::setRegistryC(int registryC) {
	this->registryC = registryC;
}
int PCB::getRegistryD() {
	return this->registryD;
}
void PCB::setRegistryD(int registryD) {
	this->registryD = registryD;
}
State PCB::getState() {
	return this->state;
}
void PCB::setState(State state) {
	this->state = state;
}
bool PCB::createProcess(std::string pid, int processAddress, int priority, State state) {
	std::map<std::string, PCB*>::iterator it = this->processesMap.find(pid);

	if (pid == it->first) {
		return false;
	}
	else {
		PCB* pcb = new PCB(pid, processAddress, priority, state);
		this->processesMap.insert(std::pair<std::string, PCB*>(pid, pcb));
		return true;
	}
}
bool PCB::removeProcess(std::string pid) {
	std::map<std::string, PCB*>::iterator it = this->processesMap.find(pid);

	if (pid == it->first) {
		it->second->setState(TERMINATED);
		this->processesMap.erase(it);
		return true;
	}
	else {
		return false;
	}
}
bool PCB::resumeProcess(std::string pid) {
	std::map<std::string, PCB*>::iterator it = this->processesMap.find(pid);

	if (pid == it->first) {
		it->second->state == State::READY;
		return true;
	}
	else {
		return false;
	}
}
bool PCB::haltProcess(std::string pid) {
	std::map<std::string, PCB*>::iterator it = this->processesMap.find(pid);

	if (pid == it->first) {
		it->second->state == State::WAITING;
		return true;
	}
	else {
		return false;
	}
}
std::queue<PCB*>* PCB::getReadyProccesses() {

	
	return &readyQueue;
}
