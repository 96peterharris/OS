//Karol
#include"Sync_Mech.hpp"

bool Semaphore::signal_sem() {
	value++;
	std::string pcbid;
	if (queue.size() != 0) {
		pcbid = queue.at(0);
		queue.erase(queue.begin());
		if (resumeProcess(pcbid)) return true;
		else return false;
	}
}

bool Semaphore::wait_sem(std::string pcbid) {
	if (value <= 0) {
		value--;
		queue.push_back(pcbid);
		if (haltProcess(pcbid)) return true;
		else return false;
	}
	else {
		value--;
		return true;
	}
}

void Semaphore::step_sem() {
	std::string input = "";
	std::cin >> input;
	if (input == "show_data") {
		std::cout << "Semaphore value is " << value << std::endl << "In the queue are:\n";
		for (int i = 0; i < queue.size(); i++) {
			std::cout << queue.at(i) << std::endl;
		}
	}
}