#include "CPU_Scheduling.hpp"
#include "PCB.hpp"

int main() {
	PCB pcb;
	pcb.createProcess("pierwszy", 99, 9, READY);
	pcb.createProcess("drugi", 99, 8, READY);

	CPU_Scheduling cpuS;
	cpuS.running();

}
