//Wojtek
#include "Interprocess_Com.hpp"
#include "Headers.h"
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
	Message* temp = new Message(pid_receiver, content);

	if (receiver != nullptr) {
		receiver->messages.push_back(temp);

		if (!System::RAM.loadToRam(receiver, content, 2)) return false;

	}
	else return false;



	return true;
}
bool PCB::receiveMessage() {
	//ReceiveMessage
	Message* received;
	std::string RAM_string = System::RAM.readMessage((*received).RAMadrress);
	Message* RAM_received;
	//
	
	//setting parameters based on RAM
	(*RAM_received).pid_sender.push_back(RAM_string.at(0) + RAM_string.at(1));
	for (int i = 3; i < RAM_string.size(); i++) {
		(*RAM_received).pid_sender.push_back(RAM_string.at(i));
	}
	(*RAM_received).size = (*RAM_received).content.size();
	(*RAM_received).RAMadrress = (*received).RAMadrress;
	//

	if (!messages.empty()) {
		received = messages.at(0);
		(*received).content = System::RAM.readMessage((*received).RAMadrress);

		//comparing if RAM values and object values are similar
		if (!(received == RAM_received)) return false;

		std::cout << "\n\n\tODEBRANO WIADOMOSC! ";
		if (!(*RAM_received).printMessage()) return false;

		messages.erase(messages.begin());
		// U¯yj tu delete na tym co wiesz
	}
	else return false;

	return true;
}


bool showMessages(PCB* pcb) {

	for (auto mess : pcb->messages) {
		if(!mess->printMessage()) return false;
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