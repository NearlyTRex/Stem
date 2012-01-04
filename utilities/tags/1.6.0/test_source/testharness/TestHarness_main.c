#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilities/IOUtilities.h"
#include "utilities/Ranrot.h"

int main(int argc, char ** argv) {
	// Functionality other than random number generation and temporary file creation fully tested by unittest target
	
	if (argc < 2) {
		fprintf(stderr, "Must specify -random, -tempfile, or -readstdin as argv[1]\n");
		return EXIT_FAILURE;
	}
	
	if (!strcmp(argv[1], "-random")) {
		int randSeed = 0;
		unsigned int iterations = 3, stirIterations = 0, iteration;
		float frandRange = 1.0f;
		
		if (argc < 3) {
			printf("Specify random seed as argv[2] if desired; proceeding with %d\n", randSeed);
		} else {
			if (!sscanf(argv[2], "%d", &randSeed)) {
				printf("Couldn't parse \"%s\" as %%d; proceeding with random seed %d\n", argv[2], randSeed);
			}
		}
		
		if (argc < 4) {
			printf("Specify number of iterations as argv[3] if desired; proceeding with %u\n", iterations);
		} else {
			if (!sscanf(argv[3], "%u", &iterations)) {
				printf("Couldn't parse \"%s\" as %%u; proceeding with %u iterations\n", argv[3], iterations);
			}
		}
		
		if (argc < 5) {
			printf("Specify frand/ufrand range as argv[4] if desired; proceeding with %f\n", frandRange);
		} else {
			if (!sscanf(argv[4], "%f", &frandRange)) {
				printf("Couldn't parse \"%s\" as %%f; proceeding with frand/ufrand range %f\n", argv[4], frandRange);
			}
		}
		
		if (argc < 6) {
			printf("Specify stir iterations as argv[5] if desired; proceeding with %u\n", stirIterations);
		} else {
			if (!sscanf(argv[5], "%u", &stirIterations)) {
				printf("Couldn't parse \"%s\" as %%u; proceeding with %u stir iterations\n", argv[5], stirIterations);
			}
		}
		
		printf("sdrand(%d);\n", randSeed);
		sdrand(randSeed);
		if (stirIterations > 0) {
			printf("stirrand(%u)\n", stirIterations);
			stirrand(stirIterations);
		}
		for (iteration = 0; iteration < iterations; iteration++) {
			printf("uirand(): %u\n", uirand());
		}
		printf("\nsdrand(%d);\n", randSeed);
		sdrand(randSeed);
		if (stirIterations > 0) {
			printf("stirrand(%u)\n", stirIterations);
			stirrand(stirIterations);
		}
		for (iteration = 0; iteration < iterations; iteration++) {
			printf("irand(): %d\n", irand());
		}
		printf("\nsdrand(%d);\n", randSeed);
		sdrand(randSeed);
		if (stirIterations > 0) {
			printf("stirrand(%u)\n", stirIterations);
			stirrand(stirIterations);
		}
		for (iteration = 0; iteration < iterations; iteration++) {
			printf("ufrand(%f): %f\n", frandRange, ufrand(frandRange));
		}
		printf("\nsdrand(%d);\n", randSeed);
		sdrand(randSeed);
		if (stirIterations > 0) {
			printf("stirrand(%u)\n", stirIterations);
			stirrand(stirIterations);
		}
		for (iteration = 0; iteration < iterations; iteration++) {
			printf("frand(%f): %f\n", frandRange, frand(frandRange));
		}
		fflush(stdout);
		
	} else if (!strcmp(argv[1], "-tempfile")) {
		char * template = "tmpXXXXXX";
		const char * path;
		int fd;
		
		if (argc < 3) {
			printf("Specify file name template as argv[2] if desired; proceeding with \"%s\"\n", template);
		} else {
			template = argv[2];
		}
		path = temporaryFilePath(template, &fd);
		printf("%s (fd = %d)\n", path, fd);
		
	} else if (!strcmp(argv[1], "-readstdin")) {
		char * data;
		size_t length;
		
		data = readStdinSimple(&length);
		printf("Read %u bytes from stdin:\n%s\n", (unsigned int) length, data);
		free(data);
		
	} else {
		fprintf(stderr, "Unexpected value \"%s\" in argv[1] (expected -random, -tempfile, or -readstdin)\n", argv[1]);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
