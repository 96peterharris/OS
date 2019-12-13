#include "Headers.h"

int main() {
	PCB::createDummy();
	PCB::createProcess("p1","si",5);
	PCB::printMap();
	PCB::update();
	PCB::printReadyQueue();
	System::CPU.nexStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();
	System::CPU.nexStep();
	std::cout << "=============================="<< std::endl;
	System::CPU.displayRunning();
	System::CPU.nexStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();
	System::CPU.nexStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();
	System::CPU.nexStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();
	System::CPU.nexStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();
	System::CPU.nexStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

}
