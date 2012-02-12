#include "unittest/TestDeserializationContext.h"
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define SUPERCLASS DeserializationContext

TestDeserializationContext * TestDeserializationContext_create(jmp_buf * sequenceBreakJmpEnv) {
	stemobject_create_implementation(TestDeserializationContext, init, sequenceBreakJmpEnv)
}

void TestDeserializationContext_init(TestDeserializationContext * self, jmp_buf * sequenceBreakJmpEnv) {
	call_super(init, self);
	
	self->sequenceBreakJmpEnv = sequenceBreakJmpEnv;
	self->error[0] = '\x00';
	self->callIndexToFail = UINT_MAX;
	self->failStatus = 0;
	self->expectedCalls = NULL;
	self->numExpectedCalls = 0;
	self->nextExpectedCallIndex = 0;
	
	self->dispose = TestDeserializationContext_dispose;
	self->beginStructure = TestDeserializationContext_beginStructure;
	self->beginDictionary = TestDeserializationContext_beginDictionary;
	self->beginArray = TestDeserializationContext_beginArray;
	self->endStructure = TestDeserializationContext_endStructure;
	self->endDictionary = TestDeserializationContext_endDictionary;
	self->endArray = TestDeserializationContext_endArray;
	self->readInt8 = TestDeserializationContext_readInt8;
	self->readUInt8 = TestDeserializationContext_readUInt8;
	self->readInt16 = TestDeserializationContext_readInt16;
	self->readUInt16 = TestDeserializationContext_readUInt16;
	self->readInt32 = TestDeserializationContext_readInt32;
	self->readUInt32 = TestDeserializationContext_readUInt32;
	self->readInt64 = TestDeserializationContext_readInt64;
	self->readUInt64 = TestDeserializationContext_readUInt64;
	self->readFloat = TestDeserializationContext_readFloat;
	self->readDouble = TestDeserializationContext_readDouble;
	self->readString = TestDeserializationContext_readString;
	self->readBoolean = TestDeserializationContext_readBoolean;
	self->readEnumeration = TestDeserializationContext_readEnumeration;
	self->readBitfield8 = TestDeserializationContext_readBitfield8;
	self->readBitfield16 = TestDeserializationContext_readBitfield16;
	self->readBitfield32 = TestDeserializationContext_readBitfield32;
	self->readBitfield64 = TestDeserializationContext_readBitfield64;
	self->readNextDictionaryKey = TestDeserializationContext_readNextDictionaryKey;
	self->hasDictionaryKey = TestDeserializationContext_hasDictionaryKey;
	self->expectCall = TestDeserializationContext_expectCall;
	self->failNthCall = TestDeserializationContext_failNthCall;
	self->finish = TestDeserializationContext_finish;
}

void TestDeserializationContext_dispose(TestDeserializationContext * self) {
	unsigned int expectedCallIndex;
	
	for (expectedCallIndex = 0; expectedCallIndex < self->numExpectedCalls; expectedCallIndex++) {
		free(self->expectedCalls[expectedCallIndex].additionalArgs);
	}
	free(self->expectedCalls);
	
	call_super(dispose, self);
}

static char * functionNameForPtr(TestDeserializationContext * self, void * functionPtr) {
#define tryFunctionName(function) if (functionPtr == self->function) {return #function;}
	tryFunctionName(beginStructure)
	tryFunctionName(beginDictionary)
	tryFunctionName(beginArray)
	tryFunctionName(endStructure)
	tryFunctionName(endDictionary)
	tryFunctionName(endArray)
	tryFunctionName(readInt8)
	tryFunctionName(readUInt8)
	tryFunctionName(readInt16)
	tryFunctionName(readUInt16)
	tryFunctionName(readInt32)
	tryFunctionName(readUInt32)
	tryFunctionName(readInt64)
	tryFunctionName(readUInt64)
	tryFunctionName(readFloat)
	tryFunctionName(readDouble)
	tryFunctionName(readString)
	tryFunctionName(readBoolean)
	tryFunctionName(readEnumeration)
	tryFunctionName(readBitfield8)
	tryFunctionName(readBitfield16)
	tryFunctionName(readBitfield32)
	tryFunctionName(readBitfield64)
	tryFunctionName(readNextDictionaryKey)
	tryFunctionName(hasDictionaryKey)
#undef tryFunctionName
	return "<invalid>";
}

static void verifyCallIsInSequence(TestDeserializationContext * self, void * functionPtr, ...) {
	va_list args;
	
	if (self->nextExpectedCallIndex >= self->numExpectedCalls) {
		snprintf(self->error, DESERIALIZATION_ERROR_MAX, "Additional function %s called after expected end of calls", functionNameForPtr(self, functionPtr));
		longjmp(*self->sequenceBreakJmpEnv, 1);
	}
	
	if (self->expectedCalls[self->nextExpectedCallIndex].functionPtr != functionPtr) {
		snprintf(self->error, DESERIALIZATION_ERROR_MAX, "Function %s called when %s was expected (index %u)", functionNameForPtr(self, functionPtr), functionNameForPtr(self, self->expectedCalls[self->nextExpectedCallIndex].functionPtr), self->nextExpectedCallIndex);
		longjmp(*self->sequenceBreakJmpEnv, 2);
	}
	
	va_start(args, functionPtr);
	if (functionPtr != self->endStructure && functionPtr != self->endDictionary && functionPtr != self->endArray && functionPtr != self->readNextDictionaryKey) {
		char * key;
		
		key = va_arg(args, char *);
		if (((self->expectedCalls[self->nextExpectedCallIndex].key == NULL || key == NULL) && self->expectedCalls[self->nextExpectedCallIndex].key != key) || (self->expectedCalls[self->nextExpectedCallIndex].key != NULL && key != NULL && strcmp(self->expectedCalls[self->nextExpectedCallIndex].key, key))) {
			snprintf(self->error, DESERIALIZATION_ERROR_MAX, "Arg 2 to call %d (%s) was expected to be \"%s\", but was \"%s\" instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].key, key);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
	}
	
	if (functionPtr == self->readEnumeration) {
		unsigned int additionalArgIndex = 0;
		char * string;
		int enumValue;
		va_list * args2;
		
		args2 = va_arg(args, va_list *);
		do {
			string = va_arg(*args2, char *);
			if (string == NULL) {
				if (self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue != NULL) {
					snprintf(self->error, DESERIALIZATION_ERROR_MAX, "Fewer additional args specified to %s (call index %d) than expected (got NULL instead of \"%s\")", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
			} else {
				enumValue = va_arg(*args2, int);
				
				if (self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue == NULL) {
					snprintf(self->error, DESERIALIZATION_ERROR_MAX, "More additional args specified to %s (call index %d) than expected (got \"%s\" instead of NULL)", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, string);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
				if (strcmp(self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue, string)) {
					snprintf(self->error, DESERIALIZATION_ERROR_MAX, "Nonmatching additional arg specified to %s (call index %d) (got \"%s\" instead of \"%s\")", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, string, self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
				if (self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex + 1].enumValue != enumValue) {
					snprintf(self->error, DESERIALIZATION_ERROR_MAX, "Nonmatching additional arg specified to %s (call index %d) (got %d instead of %d)", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, enumValue, self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex + 1].enumValue);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
			}
			additionalArgIndex += 2;
		} while (string != NULL);
	
	} else if (functionPtr == self->readBitfield8 || functionPtr == self->readBitfield16 || functionPtr == self->readBitfield32 || functionPtr == self->readBitfield64) {
		unsigned int additionalArgIndex = 0;
		char * string;
		va_list * args2;
		
		args2 = va_arg(args, va_list *);
		do {
			string = va_arg(*args2, char *);
			if (string == NULL) {
				if (self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue != NULL) {
					snprintf(self->error, DESERIALIZATION_ERROR_MAX, "Fewer additional args specified to %s (call index %d) than expected (got NULL instead of \"%s\")", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
			} else {
				if (self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue == NULL) {
					snprintf(self->error, DESERIALIZATION_ERROR_MAX, "More additional args specified to %s (call index %d) than expected (got \"%s\" instead of NULL)", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, string);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
				if (strcmp(self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue, string)) {
					snprintf(self->error, DESERIALIZATION_ERROR_MAX, "Nonmatching additional arg specified to %s (call index %d) (got \"%s\" instead of \"%s\")", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, string, self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
			}
			additionalArgIndex++;
		} while (string != NULL);
	}
	va_end(args);
	
	self->nextExpectedCallIndex++;
}

static void failIfRequested(TestDeserializationContext * self) {
	if (self->nextExpectedCallIndex - 1 == self->callIndexToFail) {
		self->status = self->failStatus;
		if (self->jmpBuf != NULL) {
			longjmp(*self->jmpBuf, self->status);
		}
	}
}

void TestDeserializationContext_beginStructure(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->beginStructure, key);
	failIfRequested(self);
}

size_t TestDeserializationContext_beginDictionary(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->beginDictionary, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.sizeValue;
}

size_t TestDeserializationContext_beginArray(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->beginArray, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.sizeValue;
}

void TestDeserializationContext_endStructure(TestDeserializationContext * self) {
	verifyCallIsInSequence(self, self->endStructure);
	failIfRequested(self);
}

void TestDeserializationContext_endDictionary(TestDeserializationContext * self) {
	verifyCallIsInSequence(self, self->endDictionary);
	failIfRequested(self);
}

void TestDeserializationContext_endArray(TestDeserializationContext * self) {
	verifyCallIsInSequence(self, self->endArray);
	failIfRequested(self);
}

int8_t TestDeserializationContext_readInt8(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readInt8, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.int8Value;
}

uint8_t TestDeserializationContext_readUInt8(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readUInt8, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.uint8Value;
}

int16_t TestDeserializationContext_readInt16(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readInt16, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.int16Value;
}

uint16_t TestDeserializationContext_readUInt16(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readUInt16, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.uint16Value;
}

int32_t TestDeserializationContext_readInt32(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readInt32, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.int32Value;
}

uint32_t TestDeserializationContext_readUInt32(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readUInt32, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.uint32Value;
}

int64_t TestDeserializationContext_readInt64(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readInt64, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.int64Value;
}

uint64_t TestDeserializationContext_readUInt64(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readUInt64, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.uint64Value;
}

float TestDeserializationContext_readFloat(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readFloat, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.floatValue;
}

double TestDeserializationContext_readDouble(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readDouble, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.doubleValue;
}

const char * TestDeserializationContext_readString(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readString, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.stringValue;
}

bool TestDeserializationContext_readBoolean(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->readBoolean, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.boolValue;
}

int TestDeserializationContext_readEnumeration(TestDeserializationContext * self, const char * key, ...) {
	va_list args;
	
	va_start(args, key);
	verifyCallIsInSequence(self, self->readEnumeration, key, &args);
	va_end(args);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.enumValue;
}

uint8_t TestDeserializationContext_readBitfield8(TestDeserializationContext * self, const char * key, ...) {
	va_list args;
	
	va_start(args, key);
	verifyCallIsInSequence(self, self->readBitfield8, key, &args);
	va_end(args);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.uint8Value;
}

uint16_t TestDeserializationContext_readBitfield16(TestDeserializationContext * self, const char * key, ...) {
	va_list args;
	
	va_start(args, key);
	verifyCallIsInSequence(self, self->readBitfield16, key, &args);
	va_end(args);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.uint16Value;
}

uint32_t TestDeserializationContext_readBitfield32(TestDeserializationContext * self, const char * key, ...) {
	va_list args;
	
	va_start(args, key);
	verifyCallIsInSequence(self, self->readBitfield32, key, &args);
	va_end(args);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.uint32Value;
}

uint64_t TestDeserializationContext_readBitfield64(TestDeserializationContext * self, const char * key, ...) {
	va_list args;
	
	va_start(args, key);
	verifyCallIsInSequence(self, self->readBitfield64, key, &args);
	va_end(args);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.uint64Value;
}

const char * TestDeserializationContext_readNextDictionaryKey(TestDeserializationContext * self) {
	verifyCallIsInSequence(self, self->readNextDictionaryKey);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.stringValue;
}

bool TestDeserializationContext_hasDictionaryKey(TestDeserializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->hasDictionaryKey, key);
	failIfRequested(self);
	return self->expectedCalls[self->nextExpectedCallIndex - 1].returnValue.boolValue;
}

void TestDeserializationContext_expectCall(TestDeserializationContext * self, void * functionPtr, ...) {
	va_list args;
	
	self->expectedCalls = realloc(self->expectedCalls, sizeof(struct TestDeserializationContext_expectedCall) * (self->numExpectedCalls + 1));
	self->expectedCalls[self->numExpectedCalls].functionPtr = functionPtr;
	self->expectedCalls[self->numExpectedCalls].additionalArgs = NULL;
	
	va_start(args, functionPtr);
	if (functionPtr != self->endStructure && functionPtr != self->endDictionary && functionPtr != self->endArray && functionPtr != self->readNextDictionaryKey) {
		self->expectedCalls[self->numExpectedCalls].key = va_arg(args, char *);
	}
	
	if (functionPtr == self->beginDictionary || functionPtr == self->beginArray) {
		self->expectedCalls[self->numExpectedCalls].returnValue.sizeValue = va_arg(args, size_t);
		
	} else if (functionPtr == self->readInt8) {
		self->expectedCalls[self->numExpectedCalls].returnValue.int8Value = va_arg(args, int);
		
	} else if (functionPtr == self->readUInt8 || functionPtr == self->readBitfield8) {
		self->expectedCalls[self->numExpectedCalls].returnValue.uint8Value = va_arg(args, int);
		
	} else if (functionPtr == self->readInt16) {
		self->expectedCalls[self->numExpectedCalls].returnValue.int16Value = va_arg(args, int);
		
	} else if (functionPtr == self->readUInt16 || functionPtr == self->readBitfield16) {
		self->expectedCalls[self->numExpectedCalls].returnValue.uint16Value = va_arg(args, int);
		
	} else if (functionPtr == self->readInt32) {
		self->expectedCalls[self->numExpectedCalls].returnValue.int32Value = va_arg(args, int32_t);
		
	} else if (functionPtr == self->readUInt32 || functionPtr == self->readBitfield32) {
		self->expectedCalls[self->numExpectedCalls].returnValue.uint32Value = va_arg(args, uint32_t);
		
	} else if (functionPtr == self->readInt64) {
		self->expectedCalls[self->numExpectedCalls].returnValue.int64Value = va_arg(args, int64_t);
		
	} else if (functionPtr == self->readUInt64 || functionPtr == self->readBitfield64) {
		self->expectedCalls[self->numExpectedCalls].returnValue.uint64Value = va_arg(args, uint64_t);
		
	} else if (functionPtr == self->readFloat) {
		self->expectedCalls[self->numExpectedCalls].returnValue.floatValue = va_arg(args, double);
		
	} else if (functionPtr == self->readDouble) {
		self->expectedCalls[self->numExpectedCalls].returnValue.doubleValue = va_arg(args, double);
		
	} else if (functionPtr == self->readString) {
		self->expectedCalls[self->numExpectedCalls].returnValue.stringValue = va_arg(args, char *);
		
	} else if (functionPtr == self->readBoolean) {
		self->expectedCalls[self->numExpectedCalls].returnValue.boolValue = va_arg(args, int);
		
	} else if (functionPtr == self->readEnumeration) {
		self->expectedCalls[self->numExpectedCalls].returnValue.enumValue = va_arg(args, int);
		
	} else if (functionPtr == self->readNextDictionaryKey) {
		self->expectedCalls[self->numExpectedCalls].returnValue.stringValue = va_arg(args, char *);
		
	} else if (functionPtr == self->hasDictionaryKey) {
		self->expectedCalls[self->numExpectedCalls].returnValue.boolValue = va_arg(args, int);
	}
	
	if (functionPtr == self->readEnumeration) {
		unsigned int additionalArgIndex = 0;
		char * string;
		int enumValue;
		
		do {
			string = va_arg(args, char *);
			enumValue = va_arg(args, int);
			self->expectedCalls[self->numExpectedCalls].additionalArgs = realloc(self->expectedCalls[self->numExpectedCalls].additionalArgs, sizeof(union TestDeserializationContext_additionalArg) * (additionalArgIndex + 2));
			self->expectedCalls[self->numExpectedCalls].additionalArgs[additionalArgIndex++].stringValue = string;
			self->expectedCalls[self->numExpectedCalls].additionalArgs[additionalArgIndex++].enumValue = enumValue;
		} while (string != NULL);
		
	} else if (functionPtr == self->readBitfield8 || functionPtr == self->readBitfield16 || functionPtr == self->readBitfield32 || functionPtr == self->readBitfield64) {
		unsigned int additionalArgIndex = 0;
		char * string;
		
		do {
			string = va_arg(args, char *);
			self->expectedCalls[self->numExpectedCalls].additionalArgs = realloc(self->expectedCalls[self->numExpectedCalls].additionalArgs, sizeof(union TestDeserializationContext_additionalArg) * (additionalArgIndex + 1));
			self->expectedCalls[self->numExpectedCalls].additionalArgs[additionalArgIndex++].stringValue = string;
		} while (string != NULL);
	}
	va_end(args);
	
	self->numExpectedCalls++;
}

void TestDeserializationContext_failNthCall(TestDeserializationContext * self, unsigned int callIndex, int status) {
	self->callIndexToFail = callIndex;
	self->failStatus = status;
}

void TestDeserializationContext_finish(TestDeserializationContext * self) {
	if (self->nextExpectedCallIndex < self->numExpectedCalls) {
		snprintf(self->error, DESERIALIZATION_ERROR_MAX, "%d expected call%s still left in queue at end (next expected call is %s)", self->numExpectedCalls - self->nextExpectedCallIndex, self->numExpectedCalls - self->nextExpectedCallIndex == 1 ? "" : "s", functionNameForPtr(self, self->expectedCalls[self->nextExpectedCallIndex].functionPtr));
		longjmp(*self->sequenceBreakJmpEnv, 4);
	}
}
