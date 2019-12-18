//Enrique
#include "PCB.hpp"
#include "Sync_Mech.hpp"
#include "Interprocess_Com.hpp"
#include "Headers.h"

bool PCB::NEW_PROCESS = false;
std::map<std::string, PCB*> PCB::processesMap;
std::vector<PCB*> PCB::readyQueue;

//Constructor 
PCB::PCB(std::string pid, short priority, State state) : priority_default(priority), priority(priority), pSem(0) {
	this->state = NEW;
	this->commandCounter = 0;
	if (state == NEW) {
		this->state = READY;
		this->pid = pid;
		this->reg = Register();
	}
}

PCB::~PCB() {
	deleteMessageRAM();
}

PCB* PCB::getPCB(std::string pid) {
	if (pid.size() != 2) {
		std::cout << " PCB:004 - PCB pid illegal length" << std::endl;
		return nullptr;
	}

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
bool PCB::createProcess(std::string pid, std::string file, short priority) {
	if (pid.length() != 2) {
		std::cout << " PCB:005 - PCB pid illegal lenght on create" << std::endl;
		return false;
	}

	if (priority <= 0 || priority >= 16) {
		std::cout << " PCB:006 - PCB priority illegal on create" << std::endl;
		return false;
	}


	if (getPCB(pid) != nullptr) {
		std::cout << " PCB:003 - PCB already existing" << std::endl;
		return false;
	}
	else {
		//This should to everything
		PCB* pcb = new PCB(pid, priority, NEW);

		std::string text;
		if (!PCB::readFile(file, text)) {
			std::cout << " PCB:001 - Cant read file " << std::endl;
			return false;
		}

		if (!System::VM.createProg(pcb, text)) {
			std::cout << " PCB:002 - Cant load program to memory " << std::endl;
			return false;
		}

		processesMap.insert(std::pair<std::string, PCB*>(pid, pcb));
		System::CPU.cpu_sch();
		return true;
	}
}

bool PCB::removeProcess(std::string pid) {
	PCB* tPCB = getPCB(pid);
	if (tPCB != nullptr) {
		//tPCB->setTerminated();
		System::VM.deleteProg(tPCB);

		processesMap.erase(pid);


		delete tPCB;
		//call fbi remove etc
		System::CPU.cpu_sch();
		return true;
	}
	else {
		return false;
	}
}

bool PCB::createDummy() {
	PCB* pcb = new PCB("DM", 0, NEW);

	if (!System::VM.createProg(pcb, ".text JP 0")) {
		std::cout << " PCB:002 - Cant load program to memory " << std::endl;
		return false;
	}

	processesMap.insert(std::pair<std::string, PCB*>("DM", pcb));
	System::CPU.cpu_sch();
	return true;
}

bool PCB::resumeProcess(std::string pid) {
	PCB* tPCB = getPCB(pid);
	if (tPCB != nullptr) {
		tPCB->setReady();
		return true;
	}
	else {
		return false;
	}
	//FIXME: czy tu nie powinno byc PCB::update?
}

bool PCB::haltProcess(std::string pid) {
	PCB* tPCB = getPCB(pid);
	if (tPCB != nullptr) {
		tPCB->setWaiting();
		return true;
	}
	else {
		return false;
	}
}

void PCB::printPCB(std::string pid) {
	PCB* tPCB = getPCB(pid);
	if (tPCB == nullptr) {
		std::cout << " PCB:007 - print error PCB was nullptr" << std::endl;
		return;
	}

	std::cout << " " << std::left << std::setw(5) << tPCB->pid << " "
		<< std::right << std::setw(7) << tPCB->getDefaultPriority() << " " << std::right << std::setw(14) << tPCB->getPriority() << " " << std::right << std::setw(14) << stateToString(tPCB->getState()) << " "
		<< std::right << std::setw(4) << (int)tPCB->getRegisterPointer()->getA() << " " << std::right << std::setw(7) << (int)tPCB->getRegisterPointer()->getB() << " "
		<< std::right << std::setw(7) << (int)tPCB->getRegisterPointer()->getC() << " " << std::right << std::setw(7) << (int)tPCB->getRegisterPointer()->getD() << " "
		<< std::setw(11) << (int)tPCB->getCommandCounter() << std::setw(11) << tPCB->pSem.value_sem();

}

void PCB::printReadyQueue() {
	std::cout << "\n================================\n";
	std::cout << "||||||| CONTENT OF QUEUE |||||||";
	std::cout << "\n================================\n";
	std::cout << "|PID|\t |PRIORITY| \t|STATE|" << std::endl;

	for (auto x : readyQueue)
	{
		std::cout << " " << std::left << std::setw(6) << x->getPid() << std::right << std::setw(8) << x->getPriority() << "\t\t"
			<< std::right << std::setw(4) << stateToString(x->getState()) << std::endl;
	}
	std::cout << "================================\n";
}

void PCB::printMap() {
	std::cout << "\n=====================================================================================================\n";
	std::cout << "|||||||||||||||||||||||||||||||||||||||||||| PROCESS ||||||||||||||||||||||||||||||||||||||||||||||||";
	std::cout << "\n=====================================================================================================\n";
	std::cout << std::left << std::setw(6) << "|PID|" << std::right << std::setw(6) << " |Default Pri.|" << std::right << std::setw(10) << " |Dynamic Pri.|";
	std::cout << std::right << std::setw(6) << " |State|" << std::right << std::setw(6) << " |Reg A|" << std::right << std::setw(6) << " |Reg B|" << std::right << std::setw(6) << " |Reg C|";
	std::cout << std::right << std::setw(6) << " |Reg D|" << std::right << std::setw(6) << " |Com. Counter|" << std::right << std::setw(6) << " |Semafor|\n";;
	for (auto x : processesMap) {
		printPCB(x.first);
		std::cout << std::endl;
	}
	std::cout << "\n=====================================================================================================\n";
}

void PCB::printSemQueue(std::string pid) {
	PCB* tPCB = PCB::getPCB(pid);
	tPCB->pSem.print_queue();
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
void PCB::setTerminated() { if (state == RUNNING) { this->state = TERMINATED; removeProcess(this->pid); } }
void PCB::setReady() { if (state == WAITING || state == RUNNING) this->state = READY; PCB::NEW_PROCESS = true; }
void PCB::setWaiting() { if (state == RUNNING || state == READY) { this->state = WAITING; PCB::NEW_PROCESS = true; } }
void PCB::setRunning() { if (state == READY) this->state = RUNNING; }
