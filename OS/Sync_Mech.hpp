#pragma once
//Required libraries.
#include <vector>
#include <string>
#include <iostream>

class Semaphore {
protected:
	//The data hold by the semaphore.
	int value;
	std::vector<std::string> queue;

public:
	//Constructor and deconstructor.
	Semaphore(int v) : value(v) {}
	~Semaphore() {}

	//Operations to do on the semaphore.
	bool signal_sem();
	bool wait_sem(std::string pcbid);
	bool delete_sem();

	//Value getter.
	int value_sem();

	//Funktions for displaying semaphore data.
	bool print_value();
	bool print_queue();
};