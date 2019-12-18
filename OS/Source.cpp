#include "Headers.h"

int main() {
	PCB::createDummy();
	System::CPU.cpu_sch();
	
	PCB::createProcess("pr", "pr2", 8);
	
	for (int i = 0; i < 100; i++)
	{
		std::cout << "\n" << "_____________________________________\t" << "i = " << i << "\t_____________________________________" << std::endl;
		
		System::CPU.nextStep();
		
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
