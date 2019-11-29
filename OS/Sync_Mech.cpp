//Karol
#include"Sync_Mech.hpp"

void signal_sem() {
	value++;
	PCB* pcb;
	if (queue.size() != 0) {
		pcb = queue.at(0);
		queue.erase(front());
		pcb->resumeProcess();
	}
}

void wait_sem(PCB* pcb) {
	if (queue <= 0) {
		value--;
		queue.push_back(pcb);
		pcb->haltProcess();
	}
	else {
		value--;
	}
}