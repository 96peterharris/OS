//Karol
#include"Sync.h"

void Semaphore::signal_sem() {
	value++;
	PCB* pcb;
	if (queue.size() != 0) {
		pcb = queue.at(0);
		queue.erase(queue.begin());
		pcb->resumeProcess();
	}
}

void Semaphore::wait_sem(PCB* pcb) {
	if (value <= 0) {
		value--;
		queue.push_back(pcb);
		pcb->haltProcess();
	}
	else {
		value--;
	}
}