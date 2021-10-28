#include <memory>
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

typedef struct {
	obj_id id;
	InternalValue value;
} StatePair;
class Message{
	private:
		MessageType type;
		std::list<InternalValue> args;
	public:
		Message(MessageType, std::list<InternalValue>);
		MessageType getType();
};

typedef struct{
	StatePair* pairs;
	size_t num_of_pairs;
} StateResponse;
std::string stringFromStateResponse(StateResponse);

void initMiddleware(void* frontend_state, void* backend_state);

//implemented in pogo-daemon-api.cxx and need to be exported
extern "C" void StateUpdate(StateResponse);
extern "C" char* StringForward(char*);
//needs to be exported because I don't wanna reimplement Message in whatever else needs to do stuff with Message but this may be in vain
extern "C" Message* parseMessage(string);
//implemented elsewhere and need to be imported
extern "C"{
	//pointers is for the state object the frontend will need in future func calls
	void* initFrontend(size_t, unsigned char*);
	StateResponse MessageReceive(void* backend_state, Message*);
	void StringBack(void* frontend_state,char*);
}
