#pragma once
#include <vector>
#include "PCB.hpp"
//Karol
class Semaphore {
public:
	unsigned int value;
	std::vector<PCB*> queue;

	Semaphore(unsigned int v) {
		this->value = v;
	}
	~Semaphore() {};
};

void signal_sem(Semaphore* sem) {
	sem->value++;
	//sem->queue.wyci¹gniêcie PCB z kolejki procesów i wrzucenie go do kolejki procesów aktywnych;
	sem->queue.pull_front();
}

void wait_sem(Semaphore* sem,PCB *itself) {
	//sem->queue.wywalenie PCB z kolejki procesów aktywnych i wrzucenie go tutaj (itself);
	int place;
	for (int i = 0; i < queue.size(); i++) {
		if (t == queue.at(i)) {
			place = i;
		}
	}
	std::cout << "Place is " << place << std::endl;
	value--;
}