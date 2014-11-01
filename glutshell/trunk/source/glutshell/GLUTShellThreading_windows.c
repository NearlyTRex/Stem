/*
  Copyright (c) 2014 Alex Diener
  
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
  
  Alex Diener alex@ludobloom.com
*/

#include "shell/ShellThreads.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

struct threadFuncInvocation {
	int (* threadFunction)(void * context);
	void * context;
};

static DWORD WINAPI callThreadFunc(LPVOID context) {
	struct threadFuncInvocation * invocation = context;
	int (* threadFunction)(void * context);
	void * threadContext;
	
	threadFunction  = invocation->threadFunction;
	threadContext = invocation->context;
	free(invocation);
	return (DWORD) threadFunction(threadContext);
}

ShellThread Shell_createThread(int (* threadFunction)(void * context), void * context) {
	HANDLE thread;
	struct threadFuncInvocation * invocation;
	
	invocation = malloc(sizeof(struct threadFuncInvocation));
	invocation->threadFunction = threadFunction;
	invocation->context = context;
	thread = CreateThread(NULL, 0, callThreadFunc, invocation, 0, NULL);
	return thread;
}

void Shell_detachThread(ShellThread thread) {
	CloseHandle(thread);
}

int Shell_joinThread(ShellThread thread) {
	DWORD status;
	DWORD returnValue = -1;
	
	status = WaitForSingleObject(thread, INFINITE);
	if (status != WAIT_FAILED) {
		GetExitCodeThread(thread, &returnValue);
		return returnValue;
	}
	return status;
}

void Shell_exitThread(int statusCode) {
	ExitThread(statusCode);
}

ShellThread Shell_getCurrentThread() {
	HANDLE duplicate = NULL;
	
	if (DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &duplicate, 0, true, DUPLICATE_SAME_ACCESS)) {
		CloseHandle(&duplicate);
	}
	return duplicate;
}

ShellMutex Shell_createMutex() {
	return CreateMutex(NULL, false, NULL);
}

void Shell_disposeMutex(ShellMutex mutex) {
	CloseHandle(mutex);
}

void Shell_lockMutex(ShellMutex mutex) {
	WaitForSingleObject(mutex, INFINITE);
}

bool Shell_tryLockMutex(ShellMutex mutex) {
	return WaitForSingleObject(mutex, 0) != WAIT_FAILED;
}

void Shell_unlockMutex(ShellMutex mutex) {
	ReleaseMutex(mutex);
}

#define SEMAPHORE_MAX 32767

ShellSemaphore Shell_createSemaphore(unsigned int value) {
	return CreateSemaphore(NULL, value > SEMAPHORE_MAX ? SEMAPHORE_MAX : value, SEMAPHORE_MAX, NULL);
}

void Shell_disposeSemaphore(ShellSemaphore semaphore) {
	CloseHandle(semaphore);
}

void Shell_postSemaphore(ShellSemaphore semaphore) {
	ReleaseSemaphore(semaphore, 1, NULL);
}

void Shell_waitSemaphore(ShellSemaphore semaphore) {
	WaitForSingleObject(semaphore, INFINITE);
}

bool Shell_tryWaitSemaphore(ShellSemaphore semaphore) {
	return WaitForSingleObject(semaphore, 0) != WAIT_TIMEOUT;
}
