#include <string>
#include <list>
#include <optional>
#include <functional>
#include <utility>
using std::string;

typedef uint64_t obj_id;
enum MessageType{
	CREATE_ITEM,
	DEL_ITEM,
	DEL_ITEM_REC,
	CREATE_GROUP,
	DEL_GROUP,
	DEL_GROUP_REC,
	ADD_GROUP,
	ADD_GROUP_REC,
	REM_GROUP,
	REM_GROUP_REC,
	ADOPT,
	DISOWN,
	SET_MESSAGE,
	SET_PROGRESS,
	SET_METADATA,
	SET_ITEM_NAME,
	SET_GROUP_NAME,
	LIST_ITEM_IDS,
	GET_GROUPS,
	GET_PARENTS,
	GET_SIBLINGS,
	GET_CHILDREN,
	GET_CHILDREN_REC,
	GET_PROGRESS,
	GET_MESSAGE,
	GET_GROUP_IDS,
	GET_GROUP_MEMBERS,
	GET_GROUP_MEMBERS_REC,
	GET_METADATA,
	GET_ITEM_NAME,
	GET_GROUP_NAME,
	NONE
};

class InternalValue{
	private:
	InternalValue(string);
	InternalValue(double);
	InternalValue(obj_id);
	public:
	const std::optional<string> stringVal;
	const std::optional<double> decimalVal;
	const std::optional<obj_id> id;
	string toString();
	static InternalValue parseString(string);
	InternalValue operator=(InternalValue other){
		if(other.id) return other.id.value();
		else if(other.decimalVal) return other.decimalVal.value(); 
		else return other.stringVal.value();
	}
};

typedef std::pair<obj_id, InternalValue> StatePair;

class Message{
	private:
		MessageType type;
		std::list<InternalValue> args;
		Message(MessageType, std::list<InternalValue>);
	public:
		static Message parseMessage(string);
		MessageType getType();
};

class StateResponse{
	private:
	std::list<StatePair> pairs;
	public:
	string toString();
};

Message parseMessage(string);

//ew typedefs
typedef void (*addr_ret)(StateResponse);
typedef string (*contact_ret)(string);

extern "C" addr_ret addressExchange(StateResponse(*ret_addr)(Message));
extern "C" contact_ret contactExchange(void(*return_contact)(string));
extern "C"{
	void init_frontend(size_t, unsigned char*);
}
