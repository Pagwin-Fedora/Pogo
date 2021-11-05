#include <memory>
#include <string>
#include <list>
#include <optional>
#include <functional>
#include <utility>
using std::string;


void initMiddleware(void* frontend_state, void* backend_state);
char* BackToFront(char*,size_t,size_t*);
char* FrontToBack(char*,size_t,size_t*);
//implemented in pogo-daemon-api.cxx and need to be exported
extern "C" void StateUpdate(char*,size_t);
extern "C" char* StringForward(char*,size_t);
//needs to be exported because I don't wanna reimplement Message in whatever else needs to do stuff with Message but this may be in vain
//implemented elsewhere and need to be imported
extern "C"{
	//pointers is for the state object
	void* initFrontend(size_t, unsigned char*);
	char* MessageReceive(void* backend_state, char*, size_t,size_t*);
	void* initBackend(size_t, unsigned char*);
	void StringBack(void* frontend_state,char*,size_t);
}
