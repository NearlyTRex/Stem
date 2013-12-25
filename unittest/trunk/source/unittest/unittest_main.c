/*
  Copyright (c) 2013 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener adiener@sacredsoftware.net
*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <setjmp.h>
#include <unistd.h>

#if defined(WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "unittest/TestSuite.h"
#include "unittest/TestList.h"

void (* g_unitTestFailureCallback)(const char * file, const char * function, int line, const char * format, ...) __attribute__((__noreturn__)) __attribute__((format(printf, 4, 5)));

static int failures = 0;
static jmp_buf jmpEnv;

static int lengthOfSharedPrefix(const char * string1, const char * string2) {
	int charIndex;
	
	for (charIndex = 0; string1[charIndex] != '\x00' && string2[charIndex] != '\x00'; charIndex++) {
		if (string1[charIndex] != string2[charIndex]) break;
	}
	return charIndex;
}

static void assertFailureCallback(const char * file, const char * function, int line, const char * format, ...) __attribute__((__noreturn__)) __attribute__((format(printf, 4, 5)));
static void assertFailureCallback(const char * file, const char * function, int line, const char * format, ...) {
	va_list args;
	static char * sourceRoot = NULL;
	
	if (sourceRoot == NULL) {
		int charIndex;
		int length;
		
		length = strlen(__FILE__);
		sourceRoot = malloc(length + 1);
		strcpy(sourceRoot, __FILE__);
		for (charIndex = 0; charIndex < length; charIndex++) {
			if (!strcmp(sourceRoot + charIndex, "unittest/unittest_main.c")) {
				sourceRoot[charIndex] = '\x00';
				break;
			}
		}
	}
	
	file += lengthOfSharedPrefix(sourceRoot, file);
	printf("  FAILURE in %s (%s:%d):\n    ", function, file, line);
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	putchar('\n');
	
	failures++;
	longjmp(jmpEnv, 1);
}

int main(int argc, char ** argv) {
	char suiteFunctionName[256];
	unsigned int suiteCount;
	unsigned int suiteIndex;
	unsigned int testCaseIndex;
	struct TestSuite * (* suiteFunction)();
	struct TestSuite * testSuite;
#if defined(WIN32)
	HMODULE moduleHandle;
	
	moduleHandle = GetModuleHandle(NULL);
#endif
	
	if (argc > 1) {
		chdir(argv[1]);
	}
	
	g_unitTestFailureCallback = assertFailureCallback;
	
	for (suiteCount = 0; UnitTest_suiteNameList[suiteCount] != NULL; suiteCount++);
	
	putchar('\n');
	
	for (suiteIndex = 0; suiteIndex < suiteCount; suiteIndex++) {
		snprintf(suiteFunctionName, 256, "%s_suite", UnitTest_suiteNameList[suiteIndex]);
#if defined(WIN32)
		suiteFunction = (struct TestSuite * (*)()) GetProcAddress(moduleHandle, suiteFunctionName);
#else
		suiteFunction = (struct TestSuite * (*)()) dlsym(RTLD_DEFAULT, suiteFunctionName);
#endif
		if (suiteFunction == NULL) {
			fprintf(stderr, "Couldn't load test suite %s (no symbol named %s found)\n", UnitTest_suiteNameList[suiteIndex], suiteFunctionName);
			abort();
		}
		
		testSuite = suiteFunction();
		printf("%s (%d/%d) running %d test%s...\n", testSuite->description, suiteIndex + 1, suiteCount, testSuite->numberOfTestCases, testSuite->numberOfTestCases == 1 ? "" : "s");
		for (testCaseIndex = 0; testCaseIndex < testSuite->numberOfTestCases; testCaseIndex++) {
			if (setjmp(jmpEnv) != 0) {
				continue;
			}
			testSuite->testCases[testCaseIndex]();
		}
	}
	printf("\nTests completed (%d failure%s)\n\n", failures, failures == 1 ? "" : "s");
	
	return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}
