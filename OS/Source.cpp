#include "Headers.h"

int main() {
	PCB::createDummy();
	System::CPU.cpu_sch();

	PCB::createProcess("p1", "si", 2);
	PCB::createProcess("p2", "si", 1);
	PCB::createProcess("p4", "si", 3);
	//PCB::createProcess("p4", "si", 6);
	//PCB::printMap();
	//PCB::update();
	std::cout << std::endl;
	
	/*for (int i = 0; i < 50; i++)
	{
		System::CPU.nextStep();
		std::cout << "==============================" << std::endl;
		System::CPU.displayRunning();
		System::CPU.displayPCBqueue();
	}*/
	//System::CPU.cpu_sch();
	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.displayPCBqueue();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();


	PCB* ptr = PCB::getPCB("p4");
	ptr->setWaiting();
	
	System::CPU.displayRunning();
	System::CPU.displayPCBqueue();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	PCB::createProcess("p5", "si", 10);



	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();
	
	System::CPU.displayPCBqueue();



}
