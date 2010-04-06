#include <stdlib.h>
#include <stdio.h>
#include "utilities/Random.h"

int main(int argc, char ** argv) {
	int randSeed = 0;
	unsigned int iterations = 3, discardedIterations = 0, iteration;
	float frandRange = 1.0f;
	
	// Non-random functionality fully tested by unittest target
	
	if (argc < 2) {
		printf("Specify random seed as argv[1] if desired; proceeding with %d\n", randSeed);
	} else {
		if (!sscanf(argv[1], "%d", &randSeed)) {
			printf("Couldn't parse \"%s\" as %%d; proceeding with random seed %d\n", argv[1], randSeed);
		}
	}
	
	if (argc < 3) {
		printf("Specify number of iterations as argv[2] if desired; proceeding with %u\n", iterations);
	} else {
		if (!sscanf(argv[2], "%u", &iterations)) {
			printf("Couldn't parse \"%s\" as %%u; proceeding with %u iterations\n", argv[2], iterations);
		}
	}
	
	if (argc < 4) {
		printf("Specify frand/ufrand range as argv[3] if desired; proceeding with %f\n", frandRange);
	} else {
		if (!sscanf(argv[3], "%f", &frandRange)) {
			printf("Couldn't parse \"%s\" as %%f; proceeding with frand/ufrand range %f\n", argv[3], frandRange);
		}
	}
	
	if (argc < 5) {
		printf("Specify discarded iterations as argv[4] if desired; proceeding with %u\n", discardedIterations);
	} else {
		if (!sscanf(argv[4], "%u", &discardedIterations)) {
			printf("Couldn't parse \"%s\" as %%u; proceeding with %u discarded iterations\n", argv[4], discardedIterations);
		}
	}
	
	printf("sdrand(%d);\n", randSeed);
	sdrand(randSeed);
	if (discardedIterations > 0) { printf("uirand() %u time%s\n", discardedIterations, discardedIterations == 1 ? "" : "s"); }
	for (iteration = 0; iteration < discardedIterations; iteration++) { uirand(); }
	for (iteration = 0; iteration < iterations; iteration++) {
		printf("uirand(): %u\n", uirand());
	}
	printf("\nsdrand(%d);\n", randSeed);
	sdrand(randSeed);
	if (discardedIterations > 0) { printf("uirand() %u time%s\n", discardedIterations, discardedIterations == 1 ? "" : "s"); }
	for (iteration = 0; iteration < discardedIterations; iteration++) { uirand(); }
	for (iteration = 0; iteration < iterations; iteration++) {
		printf("irand(): %d\n", irand());
	}
	printf("\nsdrand(%d);\n", randSeed);
	sdrand(randSeed);
	if (discardedIterations > 0) { printf("uirand() %u time%s\n", discardedIterations, discardedIterations == 1 ? "" : "s"); }
	for (iteration = 0; iteration < discardedIterations; iteration++) { uirand(); }
	for (iteration = 0; iteration < iterations; iteration++) {
		printf("ufrand(%f): %f\n", frandRange, ufrand(frandRange));
	}
	printf("\nsdrand(%d);\n", randSeed);
	sdrand(randSeed);
	if (discardedIterations > 0) { printf("uirand() %u time%s\n", discardedIterations, discardedIterations == 1 ? "" : "s"); }
	for (iteration = 0; iteration < discardedIterations; iteration++) { uirand(); }
	for (iteration = 0; iteration < iterations; iteration++) {
		printf("frand(%f): %f\n", frandRange, frand(frandRange));
	}
	fflush(stdout);
	
	return EXIT_SUCCESS;
}
