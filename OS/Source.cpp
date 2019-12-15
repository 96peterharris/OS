#include "Headers.h"

int main() {
	PCB::createDummy();
	PCB::createProcess("p2","pr2",5);
	PCB::printMap();
	PCB::update();
	PCB::printReadyQueue();
	System::VM.printVM(PCB::getPCB("p2"));
	std::cout << std::endl;
	System::RAM.printProcess("p2");
	for (int i = 0; i < 50; i++)
	{
		System::CPU.nexStep();
		std::cout << "==============================" << std::endl << "Running: ";
		System::CPU.displayRunning();
		std::cout << "Procesess: " << std::endl;
		PCB::printMap();
		//System::RAM.printProcess(PCB::getPCB("p1"));
		//System::RAM.printAllRam();

	}
	
		/*System::FS.createFile("hi");
		System::FS.openFile("hi");
		System::FS.writeToFile("hi", "abcdefg");
		System::FS.displaydrivecontent();
		System::FS.displayoft();*/


	/*System::CPU.nexStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nexStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();

	System::CPU.nexStep();
	std::cout << "==============================" << std::endl;
	System::CPU.displayRunning();*/

}
