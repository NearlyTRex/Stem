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

#ifndef __TestSuite_H__
#define __TestSuite_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef struct TestSuite TestSuite;

extern void (* g_unitTestFailureCallback)(const char * file, const char * function, int line, const char * format, ...) __attribute__((__noreturn__)) __attribute__((format(printf, 4, 5)));

struct TestSuite {
	char * description;
	unsigned int numberOfTestCases;
	void (** testCases)();
};

#define TestCase_assert(condition, ...) \
	if (!(condition)) { \
		(*g_unitTestFailureCallback)(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
	}

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#define TEST_SUITE(name, ...) \
DLLEXPORT TestSuite * name##_suite() { \
	return testSuite(#name, __VA_ARGS__, NULL); \
}

static inline TestSuite * testSuite(const char * description, ...) __attribute__((sentinel));
static inline TestSuite * testSuite(const char * description, ...) {
	TestSuite * suite;
	va_list args;
	unsigned int testCaseIndex;
	
	suite = malloc(sizeof(TestSuite));
	
	va_start(args, description);
	for (suite->numberOfTestCases = 0; va_arg(args, void (*)()) != NULL; suite->numberOfTestCases++);
	va_end(args);
	
	suite->description = malloc(strlen(description) + 1);
	strcpy(suite->description, description);
	suite->testCases = malloc(sizeof(void (*)()) * suite->numberOfTestCases);
	va_start(args, description);
	for (testCaseIndex = 0; testCaseIndex < suite->numberOfTestCases; testCaseIndex++) {
		suite->testCases[testCaseIndex] = va_arg(args, void (*)());
	}
	va_end(args);
	
	return suite;
}

#ifdef __cplusplus
}
#endif
#endif
