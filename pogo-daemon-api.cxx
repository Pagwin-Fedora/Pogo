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
	std::transform(split.begin(),split.end(),args.begin(),InternalValue::parseString);
	Message* msg;
	sswitch(split[0])
		scase("CREATE_ITEM",{
			Message tmp(MessageType::CREATE_ITEM,args);
			msg = &tmp;
		})
		scase("DEL_ITEM",{
			Message tmp(MessageType::DEL_ITEM,args);
			msg = &tmp;
		})
		scase("DEL_ITEM_REC",{
			Message tmp(MessageType::DEL_ITEM_REC,args);
			msg = &tmp;
		})
		scase("CREATE_GROUP",{
			Message tmp(MessageType::CREATE_GROUP,args);
			msg = &tmp;
		})
		scase("DEL_GROUP",{
			Message tmp(MessageType::DEL_GROUP,args);
			msg = &tmp;
		})
		scase("DEL_GROUP_REC",{
			Message tmp(MessageType::DEL_GROUP_REC,args);
			msg = &tmp;
		})
		scase("ADD_GROUP",{
			Message tmp(MessageType::ADD_GROUP,args);
			msg = &tmp;
		})
		scase("ADD_GROUP_REC",{
			Message tmp(MessageType::ADD_GROUP_REC,args);
			msg = &tmp;
		})
		scase("REM_GROUP",{
			Message tmp(MessageType::REM_GROUP,args);
			msg = &tmp;
		})
		scase("REM_GROUP_REC",{
			Message tmp(MessageType::REM_GROUP_REC,args);
			msg = &tmp;
		})
		scase("ADOPT",{
			Message tmp(MessageType::ADOPT,args);
			msg = &tmp;
		})
		scase("DISOWN",{
			Message tmp(MessageType::DISOWN,args);
			msg = &tmp;
		})
		scase("SET_MESSAGE",{
			Message tmp(MessageType::SET_MESSAGE,args);
			msg = &tmp;
		})
		scase("SET_PROGRESS",{
			Message tmp(MessageType::SET_PROGRESS,args);
			msg = &tmp;
		})
		scase("SET_METADATA",{
			Message tmp(MessageType::SET_METADATA,args);
			msg = &tmp;
		})
		scase("SET_ITEM_NAME",{
			Message tmp(MessageType::SET_ITEM_NAME,args);
			msg = &tmp;
		})
		scase("SET_GROUP_NAME",{
			Message tmp(MessageType::SET_GROUP_NAME,args);
			msg = &tmp;
		})
		scase("LIST_ITEM_IDS",{
			Message tmp(MessageType::LIST_ITEM_IDS,args);
			msg = &tmp;
		})
		scase("GET_GROUPS",{
			Message tmp(MessageType::GET_GROUPS,args);
			msg = &tmp;
		})
		scase("GET_PARENTS",{
			Message tmp(MessageType::GET_PARENTS,args);
			msg = &tmp;
		})
		scase("GET_SIBLINGS",{
			Message tmp(MessageType::GET_SIBLINGS,args);
			msg = &tmp;
		})
		scase("GET_CHILDREN",{
			Message tmp(MessageType::GET_CHILDREN,args);
			msg = &tmp;
		})
		scase("GET_CHILDREN_REC",{
			Message tmp(MessageType::GET_CHILDREN_REC,args);
			msg = &tmp;
		})
		scase("GET_PROGRESS",{
			Message tmp(MessageType::GET_PROGRESS,args);
			msg = &tmp;
		})
		scase("GET_MESSAGE",{
			Message tmp(MessageType::GET_MESSAGE,args);
			msg = &tmp;
		})
		scase("GET_GROUP_IDS",{
			Message tmp(MessageType::GET_GROUP_IDS,args);
			msg = &tmp;
		})
		scase("GET_GROUP_MEMBERS",{
			Message tmp(MessageType::GET_GROUP_MEMBERS,args);
			msg = &tmp;
		})
		scase("GET_GROUP_MEMBERS_REC",{
			Message tmp(MessageType::GET_GROUP_MEMBERS_REC,args);
			msg = &tmp;
		})
		scase("GET_METADATA",{
			Message tmp(MessageType::GET_METADATA,args);
			msg = &tmp;
		})
		scase("GET_ITEM_NAME",{
			Message tmp(MessageType::GET_ITEM_NAME,args);
			msg = &tmp;
		})
		scase("GET_GROUP_NAME",{
			Message tmp(MessageType::GET_GROUP_NAME,args);
			msg = &tmp;
		})
		sdefault {
			Message tmp (MessageType::NONE,args);
		}
		close_sswitch
	return *msg;			
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
