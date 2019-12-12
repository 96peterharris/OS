#pragma once
//Wojtek

//Mateusz jak cos to wszystkie couty mozesz zmienic na te swoje SFMLowe okienka nara

#include <algorithm>
#include <exception>

#include "RAM.hpp"
#include "PCB.hpp"



struct messageNotFound : public std::exception
{
	const char* what() const throw ()
	{
		return "Message not Found Exception";
	}
};

struct receiverNotFound : public std::exception
{
	const char* what() const throw ()
	{
		return "Receiver not Found Exception";
	}
};

class Message {
public:
	std::string pid_sender;
	std::string content;

	int RAMadrress;
	int size;

	Message();
	Message(std::string sender, std::string content);

	//ReadMessage
	//LoadToRam

	bool printMessage();

};

bool sendMessage(std::string pid_receiver, std::string content);
bool receiveMessage();

bool operator==(Message& m1, Message& m2) {
	if (m1.pid_sender == m2.pid_sender && m1.content == m2.content && m1.RAMadrress == m2.RAMadrress) return true;
	return false;
}
