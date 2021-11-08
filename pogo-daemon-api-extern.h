#include <stdlib.h>
void StateUpdate(char*,size_t);
char* StringForward(char*,size_t);

void* initFrontend(size_t, unsigned char*);
char* MessageReceive(void* backend_state, char*, size_t,size_t*);
void* initBackend(size_t, char*);
void StringBack(void* frontend_state,char*,size_t);
