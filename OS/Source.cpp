#include "Headers.h"

int main() {
	PCB::createDummy();
	System::CPU.cpu_sch();

	PCB::createProcess("pr", "pr2", 2);
	//PCB::createProcess("p4", "si", 6);
	//PCB::printMap();
	//PCB::update();
	std::cout << std::endl;
	
	for (int i = 0; i < 50; i++)
	{
		System::CPU.nextStep();
		std::cout << "==============================" << std::endl;
		System::CPU.displayRunning();
		System::CPU.displayPCBqueue();
	}
}
