#include <algorithm>
#include <functional>
#include <string>
#include <numeric>
#include "pogo-daemon-api-interns.h"
#include "string-utils.h"

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
	auto split = nstd::str_split(msgTxt);
	std::list<InternalValue> args;
	sswitch(split[0])
		scase("CREATE_ITEM",{
			
		})
		scase("DEL_ITEM",{
			a
		})
	close_sswitch();
	return ;
}
string StateResponse::toString(){
	std::list<string> storage;
	std::transform(std::begin(this->pairs),std::end(this->pairs),std::begin(storage),[](StatePair pair){
				return std::to_string(pair.first) + " " + pair.second.toString();
			});
	return std::reduce(std::next(std::begin(storage)),std::end(storage),storage.front(),[](string first, string second){
				return first+"\n"+second;
			});
}
string InternalValue::toString(){
	if(this->stringVal.has_value()) return this->stringVal.value();
	if(this->decimalVal.has_value()) return std::to_string(this->decimalVal.value());
	if(this->id.has_value()) return std::to_string(this->id.value());
	return "";
}
