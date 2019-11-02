#pragma once
#include <exception>
#include <string>

enum State
{
	NEW,
	RUNNING,
	WAITING,
	READY,
	TERMINATED

};

std::string getStateName(State e) {
	switch (e) {

		case NEW: 
			return "NEW";   
			break;
		case RUNNING:
			return "RUNNING";
			break;
		case WAITING: 
			return "WAITING";  
			break;
		case READY: 
			return "READY"; 
			break;
		case TERMINATED: 
			return "TERMINATED"; 
			break;
	}

};
