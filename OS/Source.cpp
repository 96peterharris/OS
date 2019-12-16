#include "Headers.h"

int main() {
	PCB::createDummy();
	PCB::createProcess("pr", "pr2", 2);
	PCB::printMap();
	PCB::update();
	std::cout << std::endl;
	for (int i = 0; i < 50; i++)
	{
		System::CPU.nexStep();
		std::cout << "==============================" << std::endl << "Running: ";
		System::CPU.displayRunning();
		std::cout << "Command: " << i << std::endl << "Procesess: " << std::endl;
		PCB::printMap();
		if (i > 4) {
			System::FS.displaydrivecontent();
		}
		//System::RAM.printProcess(PCB::getPCB("p1"));
		//System::RAM.printAllRam();

	}
	
		/*System::FS.createFile("hi");
		System::FS.openFile("hi");
		System::FS.writeToFile("hi", "abcdefg");
		System::FS.displaydrivecontent();
		System::FS.displayoft();*/

/* 	System::CPU.nexStep();
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


	System::CPU.displayPCBqueue(); */

}
