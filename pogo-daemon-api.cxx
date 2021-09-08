#include <cstdint>
#include <queue>
#include <string>
#include <functional>
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
std::function<void(StateUpdate)> addressExchange(std::function<StateUpdate(UpdateMessage)> up,std::function<QueryResult(QueryMessage)> que){
	updateAddress = up;
	queryAddress = que;
	return address;
}

Message parseMessage(string msgTxt){
	Message t;
	return t;
}
