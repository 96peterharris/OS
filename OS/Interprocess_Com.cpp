//Wojtek
#include "Interprocess_Com.hpp"
#include "Headers.h"
#include "RAM.hpp"
#include "PCB.hpp"

Message::Message() {}

Message::Message(std::string sender, std::string content) {
	this->pid_sender = sender;
	this->content = content;

	this->size = content.size();

	//this->size = loadToRam() ???czekam na kinie

}


bool PCB::sendMessage(std::string pid_receiver, std::string content) {
	//loadToRam
	PCB* receiver = getPCB(pid_receiver);
	Message temp = Message(pid_receiver, content);

	if (receiver != nullptr) {
		receiver->messages.push_back(temp);

		if (!System::RAM.loadToRam(receiver, prepareMessage(temp), 2)) return false;

	}
	else return false;

	//semaphore action
	if(!receiver->pSem.signal_sem()) return false;
	
	return true;
}
bool PCB::receiveMessage() {
	//ReceiveMessage

	//semaphore action
	if (pSem.wait_sem(this->pid))
		return true;

	Message received = messages.at(0);
	std::string RAM_string = System::RAM.readMessage(received.RAMadrress);
	Message RAM_received;
	//

	//setting parameters based on RAM
	(RAM_received).pid_sender.push_back(RAM_string.at(0) + RAM_string.at(1));
	for (int i = 3; i < RAM_string.size(); i++) {
		(RAM_received).pid_sender.push_back(RAM_string.at(i));
	}
	(RAM_received).size = (RAM_received).content.size();
	(RAM_received).RAMadrress = (received).RAMadrress;
	//

	if (!messages.empty()) {
		received = messages.at(0);
		(received).content = System::RAM.readMessage((received).RAMadrress);

		//comparing if RAM values and object values are similar
		if (!(received == RAM_received)) return false;

		std::cout << "\n\n\tODEBRANO WIADOMOSC! ";
		if (!(RAM_received).printMessage()) return false;

		messages.erase(messages.begin());
	}
	else {
		
		return false;
	
	}

	return true;
}


bool showMessages(PCB* pcb) {

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