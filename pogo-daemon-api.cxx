#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <numeric>
#include <optional>
#include <iostream>
#include <thread>
#include <chrono>
#include "pogo-daemon-api.h"
#include "string-utils.h"

using std::string;


void* frontend_state;
void* backend_state;


char* StringForward(char* str,size_t size){
	std::cout << str << std::endl;
	size_t responseSize;
	char* response = MessageReceive(backend_state, str, size,&responseSize);
	response = BackToFront(response,responseSize,&responseSize);
	nullEnd(response,responseSize);
	return response;
}

void initMiddleware(void *f_state, void *b_state){
	frontend_state = f_state;
	backend_state = b_state;
	while(true) std::this_thread::sleep_for(std::chrono::minutes(1));
}
//both of these do nothing because I don't need them to do anything right now(I just realized I should probably put them as another library that gets linked in rather than in the source files themselves but at this point fuck it I'll do it when I refactor this into more git repos)
char* BackToFront(char* resp,size_t size,size_t *newSize){
	*newSize = size;
	return resp;
}

char* FrontToBack(char* resp,size_t size,size_t *newSize){
	*newSize = size;
	return resp;
}

void StateUpdate(char* update, size_t updateLen){
	//new size_t var declared to avoid potential bugs where the new size is updated but the old size is still needed
	size_t newUpdateLen;
	update = BackToFront(update, updateLen, &newUpdateLen);
}
