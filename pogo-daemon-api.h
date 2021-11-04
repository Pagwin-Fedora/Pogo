#include <memory>
#include <string>
#include <list>
#include <optional>
#include <functional>
#include <utility>
using std::string;

typedef uint64_t obj_id;
typedef unsigned int TransactionAttribute;
const TransactionAttribute
	ITEM = 0,//with no arg creates an item with an item id as an arg deletes the item
	ITEMS = 1,// lists out all items when given no args and when given a list of item ids(existing or not) will delete the items that fail to be a part of the inner join of the ids provided and the existing ids
	NAME = 2,// gives the name of an item when given it's id and changes the name of the item if given an id and a new name
	DESCRIPTION = 3,// gives the description of an item when given it's id and changes the description of the item if given an id and a new description
	PARENTS = 4,// gives the ids of the parent items when given an item id and changes the items parents when given an id to change the parents of list of new parent item ids
	CHILDREN = 5,// same as parents but for children
	PROGRESS = 6,// gives the fraction describing the current progress when given an item id and sets the fraction when given an item id a numerator and a denominator
	METADATA = 7;// gives the metadata of an item when given it's id and changes the metadata of the item if given an id and new metadata

typedef struct InternalValue {
	TransactionAttribute type;
} InternalValue;
typedef struct {
	obj_id id;
	//AttrType type;
	InternalValue value;
} StatePair;

typedef struct Message{
	Messagetype type;
	InternalValue* vals;
	size_t valsLen;
} Message;
Message constructMessage(Messagetype,InternalValue*,size_t);

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
	void* initBackend(size_t, unsigned char*);
	void StringBack(void* frontend_state,char*);
}
