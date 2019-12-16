#include "Headers.h"

int main() {
	PCB::createDummy();
	PCB::createProcess("pr", "pr2", 4);
	//PCB::printMap();
	//PCB::update();
	std::cout << std::endl;
	for (int i = 0; i < 50; i++)
	{
		System::CPU.nextStep();
		System::CPU.displayRunning();
		std::cout << "Command: " << i << std::endl << "Procesess: " << std::endl;
		PCB::printMap();
		if (i > 6) {
			//System::FS.displaydrivecontent();
			//System::FS.displaycatalogue();
			//System::RAM.printAllRam();
			//System::VM.printPCBsegTab("p1");
			//System::VM.printPCBsegTab("f1");
			//System::VM.printPCBsegTab("DM");
		}
		//System::RAM.printProcess(PCB::getPCB("p1"));
		//System::RAM.printAllRam();
		std::cout << std::endl;
	}
	
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
