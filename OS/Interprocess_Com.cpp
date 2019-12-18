//VoiTee
#include "Interprocess_Com.hpp"
#include "Headers.h"
#include "RAM.hpp"
#include "PCB.hpp"

Message::Message() {}

Message::Message(std::string sender, std::string content) {
	this->pid_sender = sender;
	this->content = content;
	this->size = content.size();
}


bool PCB::sendMessage(std::string pid_receiver, std::string content) {

	PCB* receiver = getPCB(pid_receiver);
	Message temp = Message(this->pid, content);

	if (receiver != nullptr) {
		
		receiver->messages.push_back(temp);

		if (!System::RAM.loadToRam(receiver, prepareMessage(temp), 2)) return false;

	}
	else return false;

	//semaphore action
	if(!receiver->pSem.signal_sem()) return false;
	else receiver->receiveMessage();
	
	return true;
}



bool PCB::receiveMessage() {

	if (messages.empty()) {
		//semaphore action
		pSem.wait_sem(this->pid);
		return true;
	}

	//trying receivin from PCB
	Message & received = messages.at(0);

	//getting from RAM
	Message RAM_received;
	RAM_received.RAMtoMessage(System::RAM.readMessage(received.RAMadrress));
	RAM_received.RAMadrress = received.RAMadrress;		
	//received.content = System::RAM.readMessage(received.RAMadrress);

		//comparing if RAM values and object values are similar
		if (!(received == RAM_received)) return false;

		std::cout << "\n\n\tODEBRANO WIADOMOSC! ";
		if (!(RAM_received).printMessage()) return false;

		messages.erase(messages.begin());
	


	return true;
}


bool PCB::showMessages(PCB* pcb) {

	if (pcb == nullptr) return false;
	for (auto mess : pcb->messages) {
		if (!mess.printMessage()) return false;
	}

	return true;
}

bool Message::printMessage() {

	std::cout <<
		"\n NADAWCA: " << pid_sender <<
		"\n TRESC: " << content <<
		"\n ADRES POCZATKU: " << RAMadrress;

	return true;
}

bool Message::RAMtoMessage(std::string RAMbytes) {

	Message RAM_received;
	//

	//setting parameters based on RAM
	
	this->pid_sender.push_back(RAMbytes.at(0));
	this->pid_sender.push_back(RAMbytes.at(1));
	for (int i = 3; i < RAMbytes.size(); i++) {
		this->content.push_back(RAMbytes.at(i));
	}
	this->size = this->content.size();
	//(RAM_received).RAMadrress = (received).RAMadrress;
	//
	

	return true;
}

bool PCB::deleteMessageRAM() {

	for (auto mess: this->messages) {
		if (!System::RAM.deleteMessage(mess.RAMadrress)) return false;
	}
	return true;
}

std::string prepareMessage(Message mess) {
	std::string chain;

	//pid
	chain.push_back(mess.pid_sender.at(0));
	chain.push_back(mess.pid_sender.at(1));
	chain.push_back(' ');
	//content
	for (int i = 0; i < mess.size; i++) {
		chain.push_back(mess.content.at(i));
	}

	return chain;
}

bool operator==(Message& m1, Message& m2) {
	if (m1.pid_sender == m2.pid_sender && m1.content == m2.content && m1.RAMadrress == m2.RAMadrress) return true;
	return false;
}