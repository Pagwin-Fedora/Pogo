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
std::function<void(StateResponse)> addressExchange(std::function<StateResponse(Message)>);
std::function<string(string)> contactExchange(std::function<void(string)>);

namespace Pogo{
	void init_frontend(string);
}
