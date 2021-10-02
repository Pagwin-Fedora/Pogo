#include "pogo-daemon-api.h"
#include <iostream>
int main(){
	std::cout << "this may go badly" << std::endl;
	init_frontend(1, (unsigned char*)"a");
}
