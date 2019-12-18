#include "Headers.h"

int main() {
	PCB::createDummy();
	System::CPU.cpu_sch();
	
	PCB::createProcess("pr", "pr2", 8);
	
	for (int i = 0; i < 100; i++)
	{
		if (i == 0) {
			//PCB::createProcess("s1", "si", 3);
			//PCB::createProcess("s2", "si", 6);
		}

		if (i == 30) {
			//PCB::removeProcess("s1");
			//PCB::removeProcess("s2");
		}

		std::cout << "\n" << "_____________________________________\t" << "i = " << i << "\t_____________________________________" << std::endl;
		
		System::CPU.nextStep();
		
		if (i > 14 && i < 18) {
			//System::RAM.printAllRam();
		}

		PCB::printMap();
		
		std::cout << "\n\n";
		/*
		
		System::VM.printVM();
		System::VM.printVMsegTab();

		System::RAM.printAllRam();
		
		System::FS.displaycatalogue();
		System::FS.displaydrivecontent();
		System::FS.displayoft();
		System::FS.displayvector();
		*/


	}
}
