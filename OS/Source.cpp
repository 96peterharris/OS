#include "Headers.h"

int main() {
	PCB::createDummy();
	PCB::createProcess("p1","si",5);
	PCB::createProcess("p3", "si", 1);
//	PCB::createProcess("p4", "si", 6);
	PCB::printMap();
	PCB::update();
	PCB::printReadyQueue();

	/*for (int i = 0; i < 10; i++)
	{
		System::CPU.nexStep();
		std::cout << "==============================" << std::endl;
		System::CPU.displayRunning();
		//System::RAM.printProcess(PCB::getPCB("p1"));
		//System::RAM.printAllRam();
	}*/
	
	



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


	System::CPU.displayPCBqueue();

}
