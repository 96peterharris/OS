#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "PCB.hpp"
//Karol
class Semaphore {
protected:
	int value;
	std::vector<std::string> queue;

	void step_sem();

public:
	Semaphore(int v) : value(v) {}
	~Semaphore() {}

	bool signal_sem();

	bool wait_sem(std::string pcbid);
};