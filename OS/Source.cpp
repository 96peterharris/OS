#include "Headers.h"

int main() {
	PCB::createDummy();
	System::CPU.cpu_sch();

	PCB::createProcess("pr", "pr2", 8);
	//PCB::createProcess("p4", "si", 6);
	//PCB::printMap();
	//PCB::update();
	std::cout << std::endl;
	
	for (int i = 0; i < 50; i++)
	{
		System::CPU.displayRunning();
		System::CPU.displayPCBqueue();
		System::CPU.nextStep();
		std::cout << "\n" << "_____________________________________" << std::endl;
		if (i > 9) {
			// System::VM.printPCBsegTab("pr");
			//System::RAM.printAllRam();
		}
	}
}
