#include "Headers.h"

int main() {
	PCB::createDummy();
<<<<<<< HEAD
	PCB::createProcess("pr", "pr2", 2);
=======
	PCB::createProcess("p1","si",12);
	PCB::createProcess("p3", "si", 11);
 	//PCB::createProcess("p4", "si", 6);
>>>>>>> poop
	PCB::printMap();
	PCB::update();
	std::cout << std::endl;
	for (int i = 0; i < 50; i++)
	{
		System::CPU.nextStep();
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

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();


	System::CPU.displayPCBqueue(); */

}
