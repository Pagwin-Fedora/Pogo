#include <queue>
#include <string>
#include <functional>

using std::string;

class StateUpdate{

};

class DaemonInterpreter{
	private:
	std::queue<string> messages;
	public:
	DaemonInterpreter(){
	}
};

std::function<void(string)> contactExchange(std::function<void(StateUpdate)>){
	
}
