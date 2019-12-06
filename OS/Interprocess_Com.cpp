//Wojtek

#include "Interprocess_Com.hpp"
#include "PCB.hpp"

Message::Message() {}

Message::Message(std::string sender, std::string content) {
	this->pid_sender = sender;
	this->content = content;

	this->size = content.size();

	//this->size = loadToRam() ???czekam na kinie

}


bool PCB::sendMessage(std::string pid_receiver, std::string content) {

	return true;
}
bool PCB::receiveMessage(std::string pid_sender) {

	return true;
}