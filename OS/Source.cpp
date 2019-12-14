#include "Headers.h"

int main() {
	PCB::createDummy();
	PCB::createProcess("p1","si",5);
	PCB::printMap();
	PCB::update();
	PCB::printReadyQueue();

	//for (int i = 0; i < 50; i++)
	//{
	for (int i = 0; i < 25; i++) {
		System::CPU.nexStep();
		PCB::printMap();
		std::cout << std::endl;

		}
		std::cout << "==============================" << std::endl;
		System::CPU.displayRunning();
		//System::RAM.printProcess(PCB::getPCB("p1"));
		//System::RAM.printAllRam();
	//}
	
		System::FS.createFile("hi");
		System::FS.openFile("hi");
		System::FS.writeToFile("hi", "abcdefg");
		System::FS.displaydrivecontent();
		System::FS.displayoft();


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
