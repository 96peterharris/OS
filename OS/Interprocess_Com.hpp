#pragma once
//VoiTee

#include <algorithm>
#include <exception>
#include <string>

class PCB;

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

struct RAMreadingError : public std::exception
{
	const char* what() const throw ()
	{
		return "Error while reading RAM";
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
	bool RAMtoMessage(std::string RAMbytes);
};



bool showMessages(PCB* pcb);
std::string prepareMessage(Message mess);

bool operator==(Message& m1, Message& m2);
