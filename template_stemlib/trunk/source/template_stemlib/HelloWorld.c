#include "HelloWorld.h"
#include <stdlib.h>

const char * helloWorld() {
	void * foo;
	
	foo = malloc(1); // This is here just to generate an example analyzer warning
	return "Hello, world!";
}
