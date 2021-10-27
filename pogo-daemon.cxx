#include "pogo-daemon-api.h"
#include <iostream>
int main(){
	void* frontend_state = initFrontend(1, (unsigned char*)"a");
	void* backend_state = nullptr; // = initBackend(1, (unsigned char*)"a");
	initMiddleware(frontend_state, backend_state);
}
