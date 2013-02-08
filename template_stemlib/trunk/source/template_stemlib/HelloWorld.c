#include "template_stemlib/HelloWorld.h"
#include <stdlib.h>

void bar(void * ptr) {
}

const char * helloWorld() {
	void * foo;
	
	foo = malloc(1); // This is here just to generate an example analyzer warning
	bar(foo); // Suppress gcc warning for unused foo
	return "Hello, world!";
}
