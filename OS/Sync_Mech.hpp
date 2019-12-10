#pragma once
#include <vector>
#include "PCB.hpp"
//Karol
class Semaphore {
protected:
	int value;
	std::vector<PCB*> queue;

public:
	Semaphore(unsigned int v) : value(v) {}
	~Semaphore() {}

	void signal_sem();

	void wait_sem(PCB* pcb);
};