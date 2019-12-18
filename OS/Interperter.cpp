#pragma once
//Seba
#include "Interpreter.hpp"

#include "Headers.h"
/**
 * Reads arguments from RAM and increases takenBytes
 *
 * Reades chars from RAM to string increasing takenBytes param for each read byte. The end of each argument (string) is recognised by space character, which is poped from the string.
 *
 * @param pcb pointer to running PCB object
 * @param argNum number of separate arguments needed to read
 * @param takenBytes reference to a value of already read bytes from RAM
 * @return vector with each argument as a string in seperate cell in order of their appearance in RAM
*/
std::vector<std::string> getArgs(PCB *pcb, int argNum, int &takenBytes) {
	std::vector<std::string> vector;
	takenBytes += pcb->getCommandCounter();
	for (int i = 0; i < argNum; i++) {
		char read;
		std::string str = "";
		do
		{
			read = System::RAM.readFromRam(pcb, 0, takenBytes);
			str.append(1, read);
			takenBytes++;
		} while (read != ' ');
		str.pop_back();
		vector.push_back(str);
	}
	takenBytes -= pcb->getCommandCounter();
	return vector;
}

bool interprate(PCB *pcb) {
	std::string command = "";
	bool ret = false, IC = false;
	command.append(1, System::RAM.readFromRam(pcb, 0, pcb->getCommandCounter()));
	command.append(1, System::RAM.readFromRam(pcb, 0, pcb->getCommandCounter() + 1));

	int takenBytes = 0;
	if (command.size() == 2) {
		takenBytes = 3;
	}
	else {
		return 0;
	}

	std::vector<std::string> args;

	if (command == "AD") //add
	{
		args = getArgs(pcb, 2, takenBytes);
		ret = ADD(pcb, args[0], args[1]);
	}
	else if (command == "SB") //substarct
	{
		args = getArgs(pcb, 2, takenBytes);
		ret = SUB(pcb, args[0], args[1]);
	}
	else if (command == "MU") //multiply
	{
		args = getArgs(pcb, 2, takenBytes);
		ret = MUL(pcb, args[0], args[1]);
	}
	else if (command == "JP") //unconditional jump
	{
		args = getArgs(pcb, 1, takenBytes);
		ret = jump(pcb, IC, args[0]);
	}
	else if (command == "JZ") //jump if register == 0
	{
		args = getArgs(pcb, 2, takenBytes);
		ret = jumpIf0(pcb, IC, args[0], args[1]);
	}
	else if (command == "JN") //jump if register != 0
	{
		args = getArgs(pcb, 2, takenBytes);
		ret = jumpIfN0(pcb, IC, args[0], args[1]); 
	}
	else if (command == "MV") //copy value to dest
	{
		args = getArgs(pcb, 2, takenBytes);
		ret = MOV(pcb, args[0], args[1]);
		//System::RAM.printProcess(pcb);
		//std::cout << std::endl << pcb->commandCounter << std::endl;
	}

	// ------- NIE SEBOWE FUNKCJE ---------
	// PROCESY

	else if (command == "WT") //set process as waiting
	{
		args = getArgs(pcb, 1, takenBytes);
		pcb->setCommandCounter(pcb->getCommandCounter() + takenBytes); //FIXME: to nie ma prawa bytu bo nic tego procesu nie obudzi
		return PCB::haltProcess(args[0]);
	}
	else if (command == "CP")
	{
		args = getArgs(pcb, 3, takenBytes);
		ret = pcb->createProcess(args[0], args[1], args[2].at(0));
	}
	else if (command == "RP")
	{
		args = getArgs(pcb, 1, takenBytes);
		ret = pcb->removeProcess(args[0]);
	}
	else if (command == "HT") //koniec programu
	{
		pcb->setCommandCounter(pcb->getCommandCounter() + takenBytes);
		return 0;
	}

	// KOMUNIKACJA PROCESOW

	else if (command == "SM") //FIXME: WOJTEK! Lista argumentow
	{
		args = getArgs(pcb, 2, takenBytes);
		ret = pcb->sendMessage(args[0], args[1]);
	}
	else if (command == "RM")
	{
		ret = pcb->receiveMessage();
	}

	// FILE SYSTEM

	else if (command == "CF")
	{
		args = getArgs(pcb, 1, takenBytes);
		ret = System::FS.createFile(args[0]);
	}
	else if (command == "CL")
	{
		args = getArgs(pcb, 1, takenBytes);
		ret = System::FS.closeFile(args[0], pcb->getPid());
	}
	else if (command == "DF")
	{
		args = getArgs(pcb, 1, takenBytes);
		ret = System::FS.deleteFile(args[0],pcb->getPid());
	}
	else if (command == "OP")
	{
		args = getArgs(pcb, 1, takenBytes);
		ret = System::FS.openFile(args[0],pcb->getPid());
	}
	else if (command == "OW")
	{
		args = getArgs(pcb, 2, takenBytes);
		ret = System::FS.overwriteFile(args[0], args[1], pcb->getPid());
	}
	else if (command == "NF")
	{
		args = getArgs(pcb, 2, takenBytes);
		ret = System::FS.renameFile(args[0], args[1], pcb->getPid());
	}
	else if (command == "WF")
	{
		args = getArgs(pcb, 1, takenBytes);
		std::string tmp = "";
		ret = System::FS.writeToFile(args[0], tmp += pcb->getRegister().getD(), pcb->getPid());
	}
	else if (command == "RF")
	{
		args = getArgs(pcb, 1, takenBytes);
		std::string tmp = "";
		System::FS.readFile(args[0], pcb->getPid(), 1, tmp);
		ret = MOV(pcb, "DX", tmp);
	}
	// SEMAFORY

	else if (command == "SW")
	{
		args = getArgs(pcb, 1, takenBytes);
		for (int i = 0; i < System::FS.maincatalogue.size(); i++)
		{
			if (System::FS.maincatalogue[i].name == args[0]) {
				System::FS.maincatalogue[i].sem.wait_sem(pcb->getPid());
			}
		}
		ret = 1;
	}

	if (!IC)
		pcb->setCommandCounter(pcb->getCommandCounter() + takenBytes);

	std::cout << "Command: " << command;
	for (auto v : args) {
		std::cout << " " << v;
	}
	
	return ret;
}


/**
 * Unconditional jump
 * @param pcb pointer to running PCB object
 * @param logAddr logic address to the next command's begining
 * @return true XD
 * @see Interpreter JP command
*/
bool jump(PCB *pcb, bool &ICcheck, std::string logAddr) {
	//pcb->setCommandCounter(std::stoi(logAddr));
	if (logAddr.size() == 1) {
		pcb->setCommandCounter((int)logAddr.at(0));
		ICcheck = true;
		return 1;
	}
}

/**
 * Jump if given register's value is equal to 0
 * @param pcb pointer to running PCB object
 * @param registerName name of the register that's value will be checked; Accepted: AX; BX; CX; DX
 * @param logAddr logic address to the next command's begining
 * @return true XD
 * @see Interpreter JZ command
*/
bool jumpIf0(PCB *pcb, bool &ICcheck, std::string registerName, std::string logAddr) {
	if (registerName == "AX" ||
		registerName == "BX" ||
		registerName == "CX" ||
		registerName == "DX"
		) {
		if (
			(registerName == "AX" && pcb->getRegisterPointer()->getA() == 0) ||
			(registerName == "BX" && pcb->getRegisterPointer()->getB() == 0) ||
			(registerName == "CX" && pcb->getRegisterPointer()->getC() == 0) ||
			(registerName == "DX" && pcb->getRegisterPointer()->getD() == 0)
			) {
			pcb->setCommandCounter((int)logAddr.at(0));
			ICcheck = true;
		}
		return 1;
	}
	else {
		return 0;
	}
}

/**
 * Jump if given register's value is equal to 0
 * @param pcb pointer to running PCB object
 * @param registerName name of the register that's value will be checked; Accepted: AX; BX; CX; DX
 * @param logAddr logic address to the next command's begining
 * @return true XD
 * @see Interpreter JN command
*/
bool jumpIfN0(PCB *pcb, bool &ICcheck, std::string registerName, std::string logAddr) {
	if ((registerName == "AX" ||
		registerName == "BX" ||
		registerName == "CX" ||
		registerName == "DX"
		) && logAddr.size() == 1) {
		if (
			(registerName == "AX" && pcb->getRegisterPointer()->getA() != 0) ||
			(registerName == "BX" && pcb->getRegisterPointer()->getB() != 0) ||
			(registerName == "CX" && pcb->getRegisterPointer()->getC() != 0) ||
			(registerName == "DX" && pcb->getRegisterPointer()->getD() != 0)
			) {
			pcb->setCommandCounter((int)logAddr.at(0));
			ICcheck = true;
		}
		return 1;
	}
	else {
		return 0;
	}
}

/**
 * Adds value to a given register or RAM memory cell
 * Works as dest. += arg.
 * @param pcb pointer to running PCB object
 * @param dest Register name or RAM memory cell. Accepted: AX,BX,CX,DX,[logAddr_String]
 * @param arg Value to be added to dest. Accepted as param dest or a string consisting of a single char, which is read as an ASCII value
 * @return true if no problem was found, false otherwise
 * @see Interpreter AD command
*/
bool ADD(PCB *pcb, std::string dest, std::string arg) {
	std::regex regi("([ABCD])X");
	std::regex memory("\\[(\\d+)\\]");
	std::smatch m;
	unsigned char a, b;

	if (std::regex_match(arg, m, regi))
	{
		if (m[1] == "A")
		{
			b = pcb->getRegisterPointer()->getA();
		}
		else if (m[1] == "B")
		{
			b = pcb->getRegisterPointer()->getB();
		}
		else if (m[1] == "C")
		{
			b = pcb->getRegisterPointer()->getC();
		}
		else if (m[1] == "D")
		{
			b = pcb->getRegisterPointer()->getD();
		}
	}
	else if (std::regex_match(arg, m, memory))
	{
		// b = std::stoi(readRamNum(pcb, std::stoi(m[1])));
		b = System::RAM.readFromRam(pcb, 1, std::stoi(m[1]));

	}
	else if (arg.size() == 1)
	{
		// b = std::stoi(arg); //Test version
		b = arg.at(0);
	}
	else
	{
		return 0;
	}


	if (std::regex_match(dest, m, regi))
	{
		if (m[1] == "A")
		{
			a = pcb->getRegisterPointer()->getA();
			if (a + b > 255) {
				pcb->getRegisterPointer()->setA(255);
			}
			else {
				pcb->getRegisterPointer()->setA(a + b);
			}

		}
		else if (m[1] == "B")
		{
			a = pcb->getRegisterPointer()->getB();
			if (a + b > 255) {
				pcb->getRegisterPointer()->setB(255);
			}
			else {
				pcb->getRegisterPointer()->setB(a + b);
			}
		}
		else if (m[1] == "C")
		{
			a = pcb->getRegisterPointer()->getC();
			if (a + b > 255) {
				pcb->getRegisterPointer()->setC(255);
			}
			else {
				pcb->getRegisterPointer()->setC(a + b);
			}
		}
		else if (m[1] == "D")
		{
			a = pcb->getRegisterPointer()->getD();
			if (a + b > 255) {
				pcb->getRegisterPointer()->setD(255);
			}
			else {
				pcb->getRegisterPointer()->setD(a + b);
			}
		}
	}
	else if (std::regex_match(dest, m, memory))
	{
		a = System::RAM.readFromRam(pcb, 1, std::stoi(m[1]));
		if (a + b > 255) {
			System::RAM.saveInRam(pcb, 1, 255, std::stoi(m[1]));
		}
		else {
			System::RAM.saveInRam(pcb, 1, a + b, std::stoi(m[1]));
		}
	}
	else
	{
		return 0;
	}

	return 1;
}

/**
 * Subtracts value form a given register or RAM memory cell
 * Works as dest. -= arg.
 * @param pcb pointer to running PCB class
 * @param dest Register name or RAM memory cell. Accepted: AX,BX,CX,DX,[logAddr_String]
 * @param arg Value to be subtracted from dest. Accepted as param dest or a string consisting of a single char, which is read as an ASCII value
 * @return true if no problem was found, false otherwise
 * @see Interpreter SB command
*/
bool SUB(PCB *pcb, std::string dest, std::string arg) {
	std::regex regi("([ABCD])X");
	std::regex memory("\\[(\\d+)\\]");
	std::smatch m;
	unsigned char a, b;

	if (std::regex_match(arg, m, regi))
	{
		if (m[1] == "A")
		{
			b = pcb->getRegisterPointer()->getA();
		}
		else if (m[1] == "B")
		{
			b = pcb->getRegisterPointer()->getB();
		}
		else if (m[1] == "C")
		{
			b = pcb->getRegisterPointer()->getC();
		}
		else if (m[1] == "D")
		{
			b = pcb->getRegisterPointer()->getD();
		}
	}
	else if (std::regex_match(arg, m, memory))
	{
		// b = std::stoi(readRamNum(pcb, std::stoi(m[1])));
		b = System::RAM.readFromRam(pcb, 1, std::stoi(m[1]));

	}
	else if (arg.size() == 1)
	{
		// b = std::stoi(arg); //Test version
		b = arg.at(0);
	}
	else
	{
		return 0;
	}


	if (std::regex_match(dest, m, regi))
	{
		if (m[1] == "A")
		{
			a = pcb->getRegisterPointer()->getA();
			if (a < b) {
				pcb->getRegisterPointer()->setA(0);
			}
			else {
				pcb->getRegisterPointer()->setA(a - b);
			}

		}
		else if (m[1] == "B")
		{
			a = pcb->getRegisterPointer()->getB();
			if (a < b) {
				pcb->getRegisterPointer()->setB(0);
			}
			else {
				pcb->getRegisterPointer()->setB(a - b);
			}
		}
		else if (m[1] == "C")
		{
			a = pcb->getRegisterPointer()->getC();
			if (a < b) {
				pcb->getRegisterPointer()->setC(0);
			}
			else {
				pcb->getRegisterPointer()->setC(a - b);
			}
		}
		else if (m[1] == "D")
		{
			a = pcb->getRegisterPointer()->getD();
			if (a < b) {
				pcb->getRegisterPointer()->setD(0);
			}
			else {
				pcb->getRegisterPointer()->setD(a - b);
			}
		}
	}
	else if (std::regex_match(dest, m, memory))
	{
		a = System::RAM.readFromRam(pcb, 1, std::stoi(m[1]));
		if (a < b) {
			System::RAM.saveInRam(pcb, 1, 0, std::stoi(m[1]));
		}
		else {
			System::RAM.saveInRam(pcb, 1, a - b, std::stoi(m[1]));
		}
	}
	else
	{
		return 0;
	}

	return 1;
}

/**
 * Multiply a given register or RAM memory cell by a value
 * Works as dest. *= arg.
 * @param pcb pointer to running PCB object
 * @param dest Register name or RAM memory cell. Accepted: AX,BX,CX,DX,[logAddr_String]
 * @param arg Value to multiply. Accepted as param dest or a string consisting of a single char, which is read as an ASCII value
 * @return true if no problem was found, false otherwise
 * @see Interpreter MU command
*/
bool MUL(PCB *pcb, std::string dest, std::string arg) {
	std::regex regi("([ABCD])X");
	std::regex memory("\\[(\\d+)\\]");
	std::smatch m;
	unsigned char a, b;

	if (std::regex_match(arg, m, regi))
	{
		if (m[1] == "A")
		{
			b = pcb->getRegisterPointer()->getA();
		}
		else if (m[1] == "B")
		{
			b = pcb->getRegisterPointer()->getB();
		}
		else if (m[1] == "C")
		{
			b = pcb->getRegisterPointer()->getC();
		}
		else if (m[1] == "D")
		{
			b = pcb->getRegisterPointer()->getD();
		}
	}
	else if (std::regex_match(arg, m, memory))
	{
		// b = std::stoi(readRamNum(pcb, std::stoi(m[1])));
		b = System::RAM.readFromRam(pcb, 1, std::stoi(m[1]));

	}
	else if (arg.size() == 1)
	{
		// b = std::stoi(arg); //Test version
		b = arg.at(0);
	}
	else
	{
		return 0;
	}


	if (std::regex_match(dest, m, regi))
	{
		if (m[1] == "A")
		{
			a = pcb->getRegisterPointer()->getA();
			if (a*b > 255) {
				pcb->getRegisterPointer()->setA(255);
			}
			else {
				pcb->getRegisterPointer()->setA(a*b);
			}

		}
		else if (m[1] == "B")
		{
			a = pcb->getRegisterPointer()->getB();
			if (a*b > 255) {
				pcb->getRegisterPointer()->setB(255);
			}
			else {
				pcb->getRegisterPointer()->setB(a*b);
			}
		}
		else if (m[1] == "C")
		{
			a = pcb->getRegisterPointer()->getC();
			if (a*b > 255) {
				pcb->getRegisterPointer()->setC(255);
			}
			else {
				pcb->getRegisterPointer()->setC(a*b);
			}
		}
		else if (m[1] == "D")
		{
			a = pcb->getRegisterPointer()->getD();
			if (a*b > 255) {
				pcb->getRegisterPointer()->setD(255);
			}
			else {
				pcb->getRegisterPointer()->setD(a*b);
			}
		}
	}
	else if (std::regex_match(dest, m, memory))
	{
		a = System::RAM.readFromRam(pcb, 1, std::stoi(m[1]));
		if (a*b > 255) {
			System::RAM.saveInRam(pcb, 1, 255, std::stoi(m[1]));
		}
		else {
			System::RAM.saveInRam(pcb, 1, a*b, std::stoi(m[1]));
		}
	}
	else
	{
		return 0;
	}

	return 1;
}


/**
 * Multiply a given register or RAM memory cell by a value
 * Works as dest. = arg.
 * @param pcb pointer to running PCB object
 * @param dest Register name or RAM memory cell. Accepted: AX,BX,CX,DX,[logAddr_String]
 * @param arg Value to copy. Accepted as param dest or a string consisting of a single char, which is read as an ASCII value
 * @return true if no problem was found, false otherwise
 * @see Interpreter MV command
*/
bool MOV(PCB *pcb, std::string dest, std::string arg) {
	std::regex regi("([ABCD])X");
	std::regex memory("\\[(\\d+)\\]");
	std::smatch m;
	char b;

	if (std::regex_match(arg, m, regi))
	{
		if (m[1] == "A")
		{
			b = pcb->getRegisterPointer()->getA();
		}
		else if (m[1] == "B")
		{
			b = pcb->getRegisterPointer()->getB();
		}
		else if (m[1] == "C")
		{
			b = pcb->getRegisterPointer()->getC();
		}
		else if (m[1] == "D")
		{
			b = pcb->getRegisterPointer()->getD();
		}
	}
	else if (std::regex_match(arg, m, memory))
	{
		b = System::RAM.readFromRam(pcb, 1, std::stoi(m[1]));
	}
	else if (arg.size() == 1)
	{
		// b = std::stoi(arg); //Test version
		b = arg.at(0);
	}
	else
	{
		return 0;
	}

	if (std::regex_match(dest, m, regi))
	{
		if (m[1] == "A")
		{
			pcb->getRegisterPointer()->setA(b);
		}
		else if (m[1] == "B")
		{
			pcb->getRegisterPointer()->setB(b);
		}
		else if (m[1] == "C")
		{
			pcb->getRegisterPointer()->setC(b);
		}
		else if (m[1] == "D")
		{
			pcb->getRegisterPointer()->setD(b);
		}
	}
	else if (std::regex_match(dest, m, memory))
	{
		System::RAM.saveInRam(pcb, 1, b, std::stoi(m[1]));
	}
	else
	{
		return 0;
	}

	return 1;
}