#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cctype>
#include <cstdlib>
#include <functional>
#include <string>
#include <numeric>
#include <optional>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "pogo-daemon-api.h"
#include "string-utils.h"

using std::string;

std::function<void(string)> returnContact;
std::function<StateResponse(Message)> returnAddress;


string contact(string message){
	std::cout << "contact called"<< std::endl;
	std::cout << message << " received!" << std::endl;
	Message toForward = Message::parseMessage(message);
	return returnAddress(toForward).toString();
}

void address(StateResponse update){
	returnContact(update.toString());
}

extern "C" contact_ret contactExchange(void(*return_contact)(string)){
	returnContact = return_contact;
	return contact;
}
extern "C" addr_ret addressExchange(StateResponse(*ret_addr)(Message)){
	returnAddress = ret_addr;
	return &address;
}

Message Message::parseMessage(string msgTxt){
	std::vector<string> splits;
	boost::split(splits,msgTxt,boost::is_any_of(" "));
	std::list<InternalValue> args;
	std::transform(splits.begin(),splits.end(),args.begin(),InternalValue::parseString);
	Message* msg;
	sswitch(splits[0])
		scase("CREATE_ITEM",{
			msg = new Message(MessageType::CREATE_ITEM,args);
		})
		scase("DEL_ITEM",{
			msg = new Message(MessageType::DEL_ITEM,args);
		})
		scase("DEL_ITEM_REC",{
			msg = new Message(MessageType::DEL_ITEM_REC,args);
		})
		scase("CREATE_GROUP",{
			msg = new Message(MessageType::CREATE_GROUP,args);
		})
		scase("DEL_GROUP",{
			msg = new Message(MessageType::DEL_GROUP,args);
		})
		scase("DEL_GROUP_REC",{
			msg = new Message(MessageType::DEL_GROUP_REC,args);
		})
		scase("ADD_GROUP",{
			msg = new Message(MessageType::ADD_GROUP,args);
		})
		scase("ADD_GROUP_REC",{
			msg = new Message(MessageType::ADD_GROUP_REC,args);
		})
		scase("REM_GROUP",{
			msg = new Message(MessageType::REM_GROUP,args);
		})
		scase("REM_GROUP_REC",{
			msg = new Message(MessageType::REM_GROUP_REC,args);
		})
		scase("ADOPT",{
			msg = new Message(MessageType::ADOPT,args);
		})
		scase("DISOWN",{
			msg = new Message(MessageType::DISOWN,args);
		})
		scase("SET_MESSAGE",{
			msg = new Message(MessageType::SET_MESSAGE,args);
		})
		scase("SET_PROGRESS",{
			msg = new Message(MessageType::SET_PROGRESS,args);
		})
		scase("SET_METADATA",{
			msg = new Message(MessageType::SET_METADATA,args);
		})
		scase("SET_ITEM_NAME",{
			msg = new Message(MessageType::SET_ITEM_NAME,args);
		})
		scase("SET_GROUP_NAME",{
			msg = new Message(MessageType::SET_GROUP_NAME,args);
		})
		scase("LIST_ITEM_IDS",{
			msg = new Message(MessageType::LIST_ITEM_IDS,args);
		})
		scase("GET_GROUPS",{
			msg = new Message(MessageType::GET_GROUPS,args);
		})
		scase("GET_PARENTS",{
			msg = new Message(MessageType::GET_PARENTS,args);
		})
		scase("GET_SIBLINGS",{
			msg = new Message(MessageType::GET_SIBLINGS,args);
		})
		scase("GET_CHILDREN",{
			msg = new Message(MessageType::GET_CHILDREN,args);
		})
		scase("GET_CHILDREN_REC",{
			msg = new Message(MessageType::GET_CHILDREN_REC,args);
		})
		scase("GET_PROGRESS",{
			msg = new Message(MessageType::GET_PROGRESS,args);
		})
		scase("GET_MESSAGE",{
			msg = new Message(MessageType::GET_MESSAGE,args);
		})
		scase("GET_GROUP_IDS",{
			msg = new Message(MessageType::GET_GROUP_IDS,args);
		})
		scase("GET_GROUP_MEMBERS",{
			msg = new Message(MessageType::GET_GROUP_MEMBERS,args);
		})
		scase("GET_GROUP_MEMBERS_REC",{
			msg = new Message(MessageType::GET_GROUP_MEMBERS_REC,args);
		})
		scase("GET_METADATA",{
			msg = new Message(MessageType::GET_METADATA,args);
		})
		scase("GET_ITEM_NAME",{
			msg = new Message(MessageType::GET_ITEM_NAME,args);
		})
		scase("GET_GROUP_NAME",{
			msg = new Message(MessageType::GET_GROUP_NAME,args);
		})
		sdefault {
			msg = new Message(MessageType::NONE,args);
		}
		close_sswitch
		Message deref = *msg;
		delete msg;
	return deref;			
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
enum parseVals{character,decimalPoint,digit};
InternalValue InternalValue::parseString(string str){
	std::list<parseVals> parsed;
	std::transform(str.begin(),str.end(),parsed.begin(),[](char c){
		if(std::isdigit(c)){
			return parseVals::digit;
		}
		else if(c == '.'){
			return parseVals::decimalPoint;
		}
		else{
			return parseVals::character;
		}
	});
	
	if(std::all_of(parsed.begin(),parsed.end(),[](auto val){return val == parseVals::digit;})){
		return (obj_id)std::stoi(str);
	}
	else if(std::none_of(parsed.begin(),parsed.end(),[](auto val){return val == parseVals::character;})){
		return std::stod(str);
	}
	else{
		return str;
	}
}

InternalValue::InternalValue(string str) : stringVal(str){}
InternalValue::InternalValue(double dec) : decimalVal(dec){}
InternalValue::InternalValue(obj_id id) : id(id){}
Message::Message(MessageType type, std::list<InternalValue> vals): type(type),args(vals){}
