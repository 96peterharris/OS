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
		if (recivedQueue->at(i)->getPriority() < 12 && recivedQueue->at(i)->getPid() != "DM")
		{
			int tmp = recivedQueue->at(i)->getPriority();
			tmp += 3;
			recivedQueue->at(i)->setPriority(tmp);
		}
		else if (recivedQueue->at(i)->getPriority() > 12 && recivedQueue->at(i)->getPid() != "DM")
		{
			recivedQueue->at(i)->setPriority(15);
		}
	}
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
	if (recivedQueue == nullptr)//This condition is using on start
	{
		getProcesses();//Getting process queue
		running = recivedQueue->at(0);
		running->setRunning(); 
		commandCounter = 0;
		System::VM.loadProg(running); 
	}
	else//This condition is using when recivedQueue is not empty
	{
		//Updating process queue
		if (PCB::update() == false)
		{
			running = recivedQueue->at(0);
			running->setRunning();
			commandCounter = 0;
			System::VM.loadProg(running);
		}
		else
		{
			if (running->getState == TERMINATED)
			{
				running = recivedQueue->at(0);
				commandCounter = 0;
			}
			else 
			{
				running->setRunning();
				commandCounter = 0;
			}
		}
		
	}
}
/**
 * This function is responsible for execute next step of program.
 *
 * Controls the correct execution of the next assembly program command and proper CPU allocation.
 *
*/
void CPU_Scheduling::nexStep()
{	
	if ((commandCounter < 5) && (PCB::NEW_PROCESS == false))//This condition is using in situation when commandCounter is lover than five and when wasn't createed any new processes 
	{
		if (interprate(running) == true)
		{
			std::cout << "\n interpreter true";
			commandCounter++;
		}
		else//When interprate return false
		{
			cpu_sch();
			running->setTerminated(); 
			commandCounter = 0;
			increasePriority();
		}
	}
	else if ((commandCounter < 5) && (PCB::NEW_PROCESS == true))//This condidition is using on the start 
	{
		cpu_sch();
		if (interprate(running) == true)
		{
			std::cout << "\n interpreter true";
			commandCounter++;
			PCB::NEW_PROCESS = false;
		}
	}
	else
	{
		running->setReady();
		cpu_sch();
		commandCounter = 0;
		PCB::NEW_PROCESS = false;
		increasePriority();
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
	std::cout << "\nPID\t PRIORITY \tSTATE" << std::endl;

	for (int i = 0; i < recivedQueue->size(); i++)
	{
		std::cout << recivedQueue->at(i)->getPid() << "\t " << recivedQueue->at(i)->getPriority() << "\t\t" << recivedQueue->at(i)->getState() << std::endl;
	}

	std::cout << std::endl;
}
/**
 * This function is display a infromation about current running process.
 *
 * It is shows a PID, default priority, dynamic priority, state and content of registers of current running process.
 *
*/
void CPU_Scheduling::displayRunning()
{
	std::cout << std::left << std::setw(6) << "|PID|" << std::right << std::setw(6) << " |Default Pri.|" << std::right << std::setw(10) << " |Dynamic Pri.|";
	std::cout << std::right << std::setw(6) << " |State|" << std::right << std::setw(6) << " |Reg A|" << std::right << std::setw(6) << " |Reg B|" << std::right << std::setw(6) << " |Reg C|";
	std::cout << std::right << std::setw(6) << " |Reg D|\n";
	std::cout << " " << std::left << std::setw(5) << running->pid << " "
		<< std::right << std::setw(7) << running->getDefaultPriority() << " " << std::right << std::setw(14) << running->getPriority() << " " << std::right << std::setw(11) << running->getState() << " "
		<< std::right << std::setw(7) << (int)running->getRegisterPointer()->getA() << " " << std::right << std::setw(7) << (int)running->getRegisterPointer()->getB() << " "
		<< std::right << std::setw(7) << (int)running->getRegisterPointer()->getC() << " " 	<< std::right << std::setw(7) << (int)running->getRegisterPointer()->getD() << " "
		<< std::endl;
}