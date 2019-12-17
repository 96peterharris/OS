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

inline const char* stateToString(State s)
{
	switch (s)
	{
		case NEW:   return "NEW";
		case RUNNING:   return "RUNNING";
		case WAITING: return "WAITING";
		case READY: return "READY";
		case TERMINATED: return "TERMINATED";
		default:      return "[UNKNOW STATE]";
	}
}
