#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utilities/Atom.h"
#include "utilities/IOUtilities.h"
#include "utilities/Ranrot.h"

#if defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#elif defined(__linux)
#include <time.h>
#elif defined(WIN32)
#include <windows.h>
#endif

static void printUsage() {
	fprintf(stderr, "Must specify -random, -tempfile, -readstdin, or -atomprofile as argv[1]\n");
}

static double getCurrentTime() {
#if defined(__APPLE__)
	static mach_timebase_info_data_t timebaseInfo;
	
	if (timebaseInfo.denom == 0) {
		mach_timebase_info(&timebaseInfo);
	}
	return mach_absolute_time() * (double) timebaseInfo.numer / timebaseInfo.denom * 0.000000001;
	
#elif defined(WIN32)
	static LARGE_INTEGER frequency;
	LARGE_INTEGER currentTime;
	
	if (frequency.QuadPart == 0) {
		QueryPerformanceFrequency(&frequency);
	}
	QueryPerformanceCounter(&currentTime);
	
	return (double) currentTime.QuadPart / frequency.QuadPart;
	
#elif defined(__linux)
	struct timespec currentTime;
	
	clock_gettime(CLOCK_MONOTONIC, &currentTime);
	return currentTime.tv_sec + currentTime.tv_nsec * 0.000000001;
#endif
}

int main(int argc, char ** argv) {
	if (argc < 2) {
		printUsage();
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
		
	} else if (!strcmp(argv[1], "-atomprofile")) {
		unsigned int iteration, iterationCount = 1000000;
		unsigned int stringIndex, stringCount = 500;
		double currentTime;
		char ** strings;
		unsigned int charIndex, charCount;
		
		if (argc < 3 || !sscanf(argv[2], "%u", &iterationCount)) {
			printf("Specify iteration count as argv[2] if desired; proceeding with %u\n", iterationCount);
		}
		if (argc < 4 || !sscanf(argv[3], "%u", &stringCount)) {
			printf("Specify string count as argv[3] if desired; proceeding with %u\n", stringCount);
		}
		strings = malloc(sizeof(char *) * stringCount);
		for (stringIndex = 0; stringIndex < stringCount; stringIndex++) {
			charCount = stringIndex / 127 + 1;
			strings[stringIndex] = malloc(charCount + 1);
			for (charIndex = 0; charIndex < charCount; charIndex++) {
				strings[stringIndex][charIndex] = stringIndex / (int) pow(127, charIndex) % 127 + 1;
			}
			strings[stringIndex][charIndex] = 0;
		}
		stringIndex = 0;
		currentTime = getCurrentTime();
		for (iteration = 0; iteration < iterationCount; iteration++) {
			Atom_fromString(strings[stringIndex]);
			stringIndex = (stringIndex + 1) % stringCount;
		}
		printf("Time taken: %f seconds\n", getCurrentTime() - currentTime);
		
	} else {
		printUsage();
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
