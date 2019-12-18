#include"Sync_Mech.hpp"
#include "PCB.hpp"

/**
* Rises the semaphore value and if possible removes the longest waiting process from queue and changes its state to ready.
* @return true if no problem was found or false if the proces counldn't change the state but it should.
*/
bool Semaphore::signal_sem() {
	value++;
	std::string pcbid;
	if (queue.size() != 0) {
		pcbid = queue.at(0);
		queue.erase(queue.begin());
		if (PCB::resumeProcess(pcbid)) return true;
		else return false;
	}
	return true;
}

/**
* Lowers the semaphore value and if it is lower or equal to 0 it pushes the PID into the waiting queue chainging this proces state to waiting.
* @param pcbid takes the PID of the running proces.
* @return true if proces state was changed to waiting or false if not.
*/
bool Semaphore::wait_sem(std::string pcbid) {
	if (value <= 0) {
		value--;
		queue.push_back(pcbid);
		if (PCB::haltProcess(pcbid)) return true;
		else return false;
	}
	else {
		value--;
		return false;
	}
}

/**
* Makes sure that no proces gets stuck under the semaphore while a file is getting deleted.
* @return true if no problem was found or false if signal had any problem.
*/
bool Semaphore::delete_sem() {
	int loop = queue.size();
	for (int i = 0; i < loop; i++) {
		if (!signal_sem()) return false;
	}
	return true;
}

/**
* Returns the value of the semaphore.
* @return value of the semaphore.
*/
int Semaphore::value_sem() {
	return value;
}

/**
* Prints the value of the semaphore and a explanation about it.
* @return true if no problem was found.
*/
bool Semaphore::print_value() {
	std::cout << "Semaphore value is " << value << std::endl;
	return true;
}

/**
* Prints all PID of proceses waiting under the semaphore and a explanation about it.
* @return true if no problem was found.
*/
bool Semaphore::print_queue() {
	std::cout << "In the queue are:\n";
	for (int i = 0; i < queue.size(); i++) {
		std::cout << queue.at(i) << std::endl;
	}
	return true;
}