#include <string>
#include "pogo-daemon-api-interns.h"

using std::string;

std::function<void(string)> returnContact;
std::function<StateResponse(Message)> returnAddress;



string contact(string message){
	Message toForward = parseMessage(message);
	return returnAddress(toForward).toString();
}

void address(StateResponse update){
	returnContact(update.toString());
}

std::function<string(string)> contactExchange(std::function<void(string)> ret){
	returnContact = ret;
	return contact;
}
std::function<void(StateResponse)> addressExchange(std::function<StateResponse(Message)> addr){
	returnAddress = addr;
	return address;
}

Message Message::parseMessage(string msgTxt){
	std::list<MessageArg> args;
	Message t(MessageType::LIST_ITEM_IDS,args);
	return t;
}
