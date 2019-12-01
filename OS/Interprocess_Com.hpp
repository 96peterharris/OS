#pragma once
//Wojtek

//Mateusz jak cos to wszystkie couty mozesz zmienic na te swoje SFMLowe okienka nara

#include <algorithm>
#include <exception>

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

	Message() {}
	Message(std::string sender, std::string content) {
		this->pid_sender = sender;
		this->content = content;
	}

};

bool operator==(Message& m1, Message& m2) {
	if (m1.pid_sender == m2.pid_sender && m1.content == m2.content) return true;

	return false;
}

class Message_Queue {
public:
	std::string pid_receiver;
	std::vector <Message> messages;

	Message_Queue() {

	}

	Message_Queue(std::string pid) {
		pid_receiver = pid;
	}

};

class Queue_Container {
public:
	std::vector<Message_Queue> message_queues;
}queue_container;


bool sendMessage(std::string pid_sender, std::string pid_receiver, std::string content) {
	bool isGood = true;
	Message_Queue receiver;

	try {
		receiver = find_Receiver(pid_receiver);
	}
	catch (receiverNotFound & e) {

		std::cout << "\n Nie odnaleziono odbiorcy. \n ";
		isGood = false;

		//creating message queue for the receiver
		receiver = Message_Queue(pid_receiver);
		queue_container.message_queues.push_back(receiver);
	}


	//tu bedzie dodawanie do wektora
	//if(!queue_container.message_queues.messages.push_back(Message(pid_sender, content)) isGood = false;



	return true;
}

Message_Queue find_Receiver(std::string rec) {



}

bool receiveMessage(std::string pid_receiver, std::string pid_sender, std::string content) {
	bool isGood = true;
	Message m;

	try {
		m = find_rec_message(pid_receiver, pid_sender, content);
	}
	catch (receiverNotFound & e) {

		std::cout << "\n Nie odnaleziono odbiorcy. \n ";
		isGood = false;
	}
	catch (messageNotFound & e) {

		std::cout << "\n Nie odnaleziono wiadomosci. \n ";
		isGood = false;
	}


	std::cout << "\n\nPomyslnie odebrano komunikat!\n Nadawca: " << m.pid_sender << "\n Wiadomosc: " << m.content;


	try {
		delete_Message(pid_receiver, m)
	}
	catch (std::exception & e) {
		std::cout << "\nBlad nieznanego pochodzenia podczas usuwania pliku\n";
		isGood = false;
	}

	return true;
}


Message find_rec_message(std::string rec, std::string send, std::string mess) {
	Message_Queue que;
	Message found;


	bool find_success = false;
	for (auto r : queue_container.message_queues) {
		if (r.pid_receiver == rec) {
			que = r;
			find_success = true;
		}
	}
	if (!find_success) throw receiverNotFound();


	find_success = false;
	for (auto m : que.messages) {
		if (m.content == mess && m.pid_sender == send) {
			found = m;
			find_success = true;
		}
	}
	if (!find_success) throw messageNotFound();

	return found;


}

bool delete_Message(std::string pid_receiver, Message m) {

	bool deleted = false;

	for (auto rec : queue_container.message_queues) {
		if (rec.pid_receiver == pid_receiver) {
			int it = 0;

			for (auto mess : rec.messages) {

				//rec.messages
				if (mess == m) {
					rec.messages.erase(rec.messages.begin + it);
					deleted = true;
					break;
				}
				if (deleted) break;
				it++;
			}
			if (deleted) break;
		}
	}

	if (deleted) return true;
	else return false;

}