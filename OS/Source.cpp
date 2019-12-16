#include "Headers.h"

int main() {
	PCB::createDummy();
	PCB::createProcess("pr", "si", 2);
	//PCB::printMap();
	//PCB::update();
	std::cout << std::endl;
	/*for (int i = 0; i < 50; i++)
	{
		System::CPU.nextStep();
		System::CPU.displayRunning();
		std::cout << "Command: " << i << std::endl << "Procesess: " << std::endl;
		PCB::printMap();
		if (i > 4) {
			System::FS.displaydrivecontent();
			System::FS.displaycatalogue();
		}
		//System::RAM.printProcess(PCB::getPCB("p1"));
		//System::RAM.printAllRam();
		std::cout << std::endl;
	}*/
	
		/*System::FS.createFile("hi");
		System::FS.openFile("hi");
		System::FS.writeToFile("hi", "abcdefg");
		System::FS.displaydrivecontent();
		System::FS.displayoft();*/

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

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();
	System::CPU.displayPCBqueue();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	PCB::createProcess("p3", "si", 4);

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nextStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();
	System::CPU.displayPCBqueue();
	
	PCB* pointer2 = PCB::getPCB("pr");
	PCB* pointer = PCB::getPCB("p3");
	pointer->setWaiting();
	System::CPU.displayPCBqueue();

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


	System::CPU.displayPCBqueue(); 

}
