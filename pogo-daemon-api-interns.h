#include <string>
#include <list>
#include <optional>
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
	GET_METADATA
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
	public:
	string toString();
};



Message parseMessage(string);

