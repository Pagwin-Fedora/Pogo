#include <string>

using std::string;

class UpdateMessage{

};

class QueryMessage{

};

class StateUpdate{
	public:
	string toString();
};

class QueryResult{
	public:
	string toString();
};

union _Message{
	UpdateMessage update;
	QueryMessage query;
};

class Message{
	public:
	Message(){
		
	}
	std::uint32_t kind;
	_Message value;
	const static std::uint32_t updateKind;
	const static std::uint32_t queryKind;
};


Message parseMessage(string);

