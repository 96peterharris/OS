
#include "CPU_Scheduling.hpp"

#include "Headers.h"

CPU_Scheduling::CPU_Scheduling()
{
	this->commandCounter = 0;
	this->running = nullptr;
}
/**
 * Increase priority of processes in recivedQueue.
 *
 * Checks in recivedQueue pointer from the beginning if there is any process different from Dummy process and if is it increase priority of this processes.
 *
*/
void CPU_Scheduling::increasePriority()
{
	for (int i = 0; i < recivedQueue->size() - 1; i++)
	{
		if (recivedQueue->size() < 1)
		{
			return;
		}
		else if (recivedQueue->at(i)->getPriority() < 12 && recivedQueue->at(i)->getPid() != "DM")
		{
			int tmp = recivedQueue->at(i)->getPriority();
			tmp += 3;
			recivedQueue->at(i)->setPriority(tmp);
			//std::sort(recivedQueue->begin(), recivedQueue->end(), [](PCB* a, PCB* b) { return a->getPriority() > b->getPriority(); });
		}
		else if (recivedQueue->at(i)->getPriority() >= 12 && recivedQueue->at(i)->getPid() != "DM")
		{
			recivedQueue->at(i)->setPriority(15);
			//std::sort(recivedQueue->begin(), recivedQueue->end(), [](PCB* a, PCB* b) { return a->getPriority() > b->getPriority(); });
		}
	}
	//std::sort(recivedQueue->begin(), recivedQueue->end(), [](PCB* a, PCB* b) { return a->getPriority() > b->getPriority(); });
}
/**
 * Getting readyQueuePointer and store it in recivedQueue.
 *
 * It use a function getReadyQueuePonter() from PCB Class to getting pointer to queue of ready processes.
 *
*/
void CPU_Scheduling::getProcesses()
{

	this->recivedQueue = PCB::getReadyQueuePointer();
}
/**
 * Managing current running process.
 *
 * At the start this function is getting pointer to queue of ready processes and in the other hand this function is updating content of queue.
 * Then this function is choosing proper process and setting this in running state and loading program to VM.
 *
*/
void CPU_Scheduling::cpu_sch()
{
	if (running != nullptr) {
		if (running->state == RUNNING) {
			running->setReady();
			PCB::NEW_PROCESS = false;
		}
		else if (running->state == WAITING) {
			//keep
		}
	}
	PCB::update();

	if (recivedQueue == nullptr) {
		getProcesses();
	}
	running = recivedQueue->at(0);
	running->setRunning();
	commandCounter = 0;
	System::VM.loadProg(running);
	PCB::update();
}
/**
 * This function is responsible for execute next step of program.
 *
 * Controls the correct execution of the next assembly program command and proper CPU allocation.
 *
*/
void CPU_Scheduling::nextStep()
{
	if (interprate(running) == true) 
	{
		if (PCB::NEW_PROCESS == true) 
		{
			PCB::NEW_PROCESS = false;
			cpu_sch();
		}
		else
		{
			if (commandCounter < 4) 
			{
				commandCounter++;
			}
			else 
			{
				increasePriority();
				cpu_sch();
			}
		}
	}
	else {
		commandCounter = 0;
		running->setTerminated();
		cpu_sch();
	}
}
/**
 * Finding PID of running process.
 *
 * This function is return a PID of current running process using getPid() function from PCB class.
 *
 * @return std::string which is a PID of process.
*/
std::string CPU_Scheduling::getRunningPID() {
	return running->getPid();
}
/**
 * This function is display a content of ready and running queue.
 *
 * It is shows a PID, priority and state of all proceses in queue.
 *
*/
void CPU_Scheduling::displayPCBqueue()
{
	std::cout << "\n================================\n";
	std::cout << "||||||| CONTENT OF QUEUE |||||||";
	std::cout << "\n================================\n";
	std::cout << "|PID|\t |PRIORITY| \t|STATE|" << std::endl;

	for (int i = 0; i < recivedQueue->size(); i++)
	{
		std::cout << " " << std::left << std::setw(6) << recivedQueue->at(i)->getPid() << std::right << std::setw(8) << recivedQueue->at(i)->getPriority() << "\t\t"
			<< std::right << std::setw(4) << stateToString(recivedQueue->at(i)->getState()) << std::endl;
	}
	std::cout << "================================\n";
}
/**
 * This function is display a infromation about current running process.
 *
 * It is shows a PID, default priority, dynamic priority, state and content of registers of current running process.
 *
*/
void CPU_Scheduling::displayRunning()
{
	std::cout << "\n===========================================================================================\n";
	std::cout << "||||||||||||||||||||||||||||||||||||| RUNNIG PROCESS ||||||||||||||||||||||||||||||||||||||";
	std::cout << "\n===========================================================================================\n";
	std::cout << std::left << std::setw(6) << "|PID|" << std::right << std::setw(6) << " |Default Pri.|" << std::right << std::setw(10) << " |Dynamic Pri.|";
	std::cout << std::right << std::setw(6) << " |State|" << std::right << std::setw(6) << " |Reg A|" << std::right << std::setw(6) << " |Reg B|" << std::right << std::setw(6) << " |Reg C|";
	std::cout << std::right << std::setw(6) << " |Reg D|" << std::right << std::setw(6) << " |Com. Counter|\n";
	std::cout << " " << std::left << std::setw(5) << running->pid << " "
		<< std::right << std::setw(7) << running->getDefaultPriority() << " " << std::right << std::setw(14) << running->getPriority() << " " << std::right << std::setw(14) << stateToString(running->getState()) << " "
		<< std::right << std::setw(4) << (int)running->getRegisterPointer()->getA() << " " << std::right << std::setw(7) << (int)running->getRegisterPointer()->getB() << " "
		<< std::right << std::setw(7) << (int)running->getRegisterPointer()->getC() << " " << std::right << std::setw(7) << (int)running->getRegisterPointer()->getD() << " "
		<< std::setw(11) << (int)running->getCommandCounter();
	std::cout << "\n===========================================================================================\n";
}


