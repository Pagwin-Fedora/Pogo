#include <string>
#include <list>
#include <optional>
#include <functional>
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
	GET_GROUP_NAME
};

class MessageArg{
	std::optional<string> stringArg;
	std::optional<double> decimalArg;
	std::optional<obj_id> id;
};


class Message{
	private:
		MessageType type;
		std::list<MessageArg> args;
		Message(MessageType, std::list<MessageArg>);
	public:
		static Message parseMessage(string);
		MessageType getType();
};

class StateResponse{
	private:
	std::list<obj_id>
	public:
	string toString();
};



Message parseMessage(string);
std::function<void(StateResponse)> addressExchange(std::function<StateResponse(Message)>);
std::function<string(string)> contactExchange(std::function<void(string)>);
