#include <cstdint>
#include <queue>
#include <string>
#include <functional>
#include "pogo-daemon-api-interns.h"

using std::string;

std::function<void(string)> returnContact;
std::function<StateUpdate(UpdateMessage)> updateAddress;
std::function<QueryResult(QueryMessage)> queryAddress;

const uint32_t Message::updateKind = 0;
const uint32_t Message::queryKind = 1;


string contact(string message){
	Message toForward = parseMessage(message);
	switch(toForward.kind){
		case Message::updateKind:
			return updateAddress(toForward.value.update).toString();
			break;
		case Message::queryKind:
			return queryAddress(toForward.value.query).toString();
			break;
		default:
			throw "bad kind specifier";
	}
}

void address(StateUpdate update){
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
