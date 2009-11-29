#include <stdlib.h>
#include <stdio.h>
#include "shell/Shell.h"

int main() {
	double currentTime;
	
	currentTime = Shell_getCurrentTime();
	printf("%f\n", currentTime);
	return EXIT_SUCCESS;
}
