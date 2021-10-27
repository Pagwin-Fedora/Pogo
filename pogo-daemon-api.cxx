#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cctype>
#include <cstdlib>
#include <functional>
#include <memory>
#include <string>
#include <numeric>
#include <optional>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <thread>
#include <chrono>
#include "pogo-daemon-api.h"
#include "string-utils.h"

using std::string;


void* frontend_state;
void* backend_state;
string contact(char* message){
	std::cout << "contact called"<< std::endl;
	std::cout << message << " received!" << std::endl;
	Message* toForward = parseMessage(message);
	string pass_message = stringFromStateResponse(MessageReceive(toForward));
	delete toForward;
	return pass_message;
}

void address(StateResponse update){
	StringBack(frontend_state,stringFromStateResponse(update).data());
}

Message* parseMessage(string msgTxt){
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
	return msg;			
}
std::string stringFromStateResponse(StateResponse serializing){
	std::list<string> storage;
	for(size_t i = 0;i<serializing.num_of_pairs;i++){
		storage.push_back(std::to_string(serializing.pairs[i].id) + " " + serializing.pairs[i].value.toString());
	}
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
char* StringForward(char* str){
	std::cout << str << std::endl;
	//CHANGE THIS ASAP
	return (char*)malloc(1);
}

void initMiddleware(void *f_state, void *b_state){
	frontend_state = f_state;
	backend_state = b_state;
	while(true) std::this_thread::sleep_for(std::chrono::minutes(1));
}
