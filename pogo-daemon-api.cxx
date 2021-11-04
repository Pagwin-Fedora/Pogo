#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cctype>
#include <cstdlib>
#include <cstring>
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


char* StringForward(char* str,size_t size){
	std::cout << str << std::endl;
	StateResponse rawResponse = MessageReceive(backend_state, str, size);
	string strResponse = stringFromStateResponse(rawResponse);
	//MEMORY LEAK HERE OH GOD
	char* ret = (char*) malloc(strResponse.length()+1);
	std::strcpy(strResponse.data(),ret);
	return ret;
}

void initMiddleware(void *f_state, void *b_state){
	frontend_state = f_state;
	backend_state = b_state;
	while(true) std::this_thread::sleep_for(std::chrono::minutes(1));
}
