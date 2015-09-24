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

#include "serialization/TestSerializationContext.h"
#include "utilities/IOUtilities.h"
#include "utilities/printfFormats.h"
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define SUPERCLASS SerializationContext

TestSerializationContext * TestSerializationContext_create(jmp_buf * sequenceBreakJmpEnv) {
	stemobject_create_implementation(TestSerializationContext, init, sequenceBreakJmpEnv)
}

bool TestSerializationContext_init(TestSerializationContext * self, jmp_buf * sequenceBreakJmpEnv) {
	call_super(init, self);
	
	self->sequenceBreakJmpEnv = sequenceBreakJmpEnv;
	self->error[0] = '\x00';
	self->callIndexToFail = UINT_MAX;
	self->failStatus = 0;
	self->expectedCalls = NULL;
	self->numExpectedCalls = 0;
	self->nextExpectedCallIndex = 0;
	
	self->dispose = TestSerializationContext_dispose;
	self->beginStructure = TestSerializationContext_beginStructure;
	self->beginDictionary = TestSerializationContext_beginDictionary;
	self->beginArray = TestSerializationContext_beginArray;
	self->endStructure = TestSerializationContext_endStructure;
	self->endDictionary = TestSerializationContext_endDictionary;
	self->endArray = TestSerializationContext_endArray;
	self->writeBoolean = TestSerializationContext_writeBoolean;
	self->writeInt8 = TestSerializationContext_writeInt8;
	self->writeUInt8 = TestSerializationContext_writeUInt8;
	self->writeInt16 = TestSerializationContext_writeInt16;
	self->writeUInt16 = TestSerializationContext_writeUInt16;
	self->writeInt32 = TestSerializationContext_writeInt32;
	self->writeUInt32 = TestSerializationContext_writeUInt32;
	self->writeInt64 = TestSerializationContext_writeInt64;
	self->writeUInt64 = TestSerializationContext_writeUInt64;
	self->writeFloat = TestSerializationContext_writeFloat;
	self->writeDouble = TestSerializationContext_writeDouble;
	self->writeFixed16_16 = TestSerializationContext_writeFixed16_16;
	self->writeEnumeration = TestSerializationContext_writeEnumeration;
	self->writeBitfield8 = TestSerializationContext_writeBitfield8;
	self->writeBitfield16 = TestSerializationContext_writeBitfield16;
	self->writeBitfield32 = TestSerializationContext_writeBitfield32;
	self->writeBitfield64 = TestSerializationContext_writeBitfield64;
	self->writeString = TestSerializationContext_writeString;
	self->writeBlob = TestSerializationContext_writeBlob;
	self->expectCall = TestSerializationContext_expectCall;
	self->failNthCall = TestSerializationContext_failNthCall;
	self->finish = TestSerializationContext_finish;
	self->rewind = TestSerializationContext_rewind;
	return true;
}

void TestSerializationContext_dispose(TestSerializationContext * self) {
	unsigned int expectedCallIndex;
	
	for (expectedCallIndex = 0; expectedCallIndex < self->numExpectedCalls; expectedCallIndex++) {
		free(self->expectedCalls[expectedCallIndex].additionalArgs);
	}
	free(self->expectedCalls);
	
	call_super(dispose, self);
}

static char * functionNameForPtr(TestSerializationContext * self, void * functionPtr) {
#define tryFunctionName(function) if (functionPtr == self->function) {return #function;}
	tryFunctionName(beginStructure)
	tryFunctionName(beginDictionary)
	tryFunctionName(beginArray)
	tryFunctionName(endStructure)
	tryFunctionName(endDictionary)
	tryFunctionName(endArray)
	tryFunctionName(writeBoolean)
	tryFunctionName(writeInt8)
	tryFunctionName(writeUInt8)
	tryFunctionName(writeInt16)
	tryFunctionName(writeUInt16)
	tryFunctionName(writeInt32)
	tryFunctionName(writeUInt32)
	tryFunctionName(writeInt64)
	tryFunctionName(writeUInt64)
	tryFunctionName(writeFloat)
	tryFunctionName(writeDouble)
	tryFunctionName(writeFixed16_16)
	tryFunctionName(writeEnumeration)
	tryFunctionName(writeBitfield8)
	tryFunctionName(writeBitfield16)
	tryFunctionName(writeBitfield32)
	tryFunctionName(writeBitfield64)
	tryFunctionName(writeString)
	tryFunctionName(writeBlob)
#undef tryFunctionName
	return "<invalid>";
}

#define BLOB_STRING_MAX 256

static void verifyCallIsInSequence(TestSerializationContext * self, void * functionPtr, ...) {
	va_list args;
	
	if (self->nextExpectedCallIndex >= self->numExpectedCalls) {
		snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Additional function %s called after expected end of calls", functionNameForPtr(self, functionPtr));
		longjmp(*self->sequenceBreakJmpEnv, 1);
	}
	
	if (self->expectedCalls[self->nextExpectedCallIndex].functionPtr != functionPtr) {
		snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Function %s called when %s was expected (index %u)", functionNameForPtr(self, functionPtr), functionNameForPtr(self, self->expectedCalls[self->nextExpectedCallIndex].functionPtr), self->nextExpectedCallIndex);
		longjmp(*self->sequenceBreakJmpEnv, 2);
	}
	
	va_start(args, functionPtr);
	if (functionPtr != self->endStructure && functionPtr != self->endDictionary && functionPtr != self->endArray) {
		char * key;
		
		key = va_arg(args, char *);
		if (((self->expectedCalls[self->nextExpectedCallIndex].key == NULL || key == NULL) && self->expectedCalls[self->nextExpectedCallIndex].key != key) || (self->expectedCalls[self->nextExpectedCallIndex].key != NULL && key != NULL && strcmp(self->expectedCalls[self->nextExpectedCallIndex].key, key))) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 2 to call %d (%s) was expected to be \"%s\", but was \"%s\" instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].key, key);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
	}
	if (functionPtr == self->writeInt8) {
		int8_t int8Value;
		
		int8Value = va_arg(args, int);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.int8Value != int8Value) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %d, but was %d instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.int8Value, int8Value);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeUInt8 || functionPtr == self->writeBitfield8) {
		uint8_t uint8Value;
		
		uint8Value = va_arg(args, int);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.uint8Value != uint8Value) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %u, but was %u instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.uint8Value, uint8Value);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeInt16) {
		int16_t int16Value;
		
		int16Value = va_arg(args, int);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.int16Value != int16Value) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %d, but was %d instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.int16Value, int16Value);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeUInt16 || functionPtr == self->writeBitfield16) {
		uint16_t uint16Value;
		
		uint16Value = va_arg(args, int);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.uint16Value != uint16Value) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %u, but was %u instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.uint16Value, uint16Value);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeInt32) {
		int32_t int32Value;
		
		int32Value = va_arg(args, int32_t);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.int32Value != int32Value) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %d, but was %d instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.int32Value, int32Value);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeUInt32 || functionPtr == self->writeBitfield32) {
		uint32_t uint32Value;
		
		uint32Value = va_arg(args, uint32_t);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.uint32Value != uint32Value) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %u, but was %u instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.uint32Value, uint32Value);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeInt64) {
		int64_t int64Value;
		
		int64Value = va_arg(args, int64_t);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.int64Value != int64Value) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be " INT64_FORMAT ", but was " INT64_FORMAT " instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.int64Value, int64Value);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeUInt64 || functionPtr == self->writeBitfield64) {
		uint64_t uint64Value;
		
		uint64Value = va_arg(args, uint64_t);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.uint64Value != uint64Value) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be " UINT64_FORMAT ", but was " UINT64_FORMAT " instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.uint64Value, uint64Value);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeFloat) {
		float floatValue;
		
		floatValue = va_arg(args, double);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.floatValue != floatValue) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %f, but was %f instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.floatValue, floatValue);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeDouble) {
		double doubleValue;
		
		doubleValue = va_arg(args, double);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.doubleValue != doubleValue) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %f, but was %f instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.doubleValue, doubleValue);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeFixed16_16) {
		fixed16_16 fixed16_16Value;
		
		fixed16_16Value = va_arg(args, fixed16_16);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.fixed16_16Value != fixed16_16Value) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be 0x%05X, but was 0x%05X instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.fixed16_16Value, fixed16_16Value);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeString) {
		char * stringValue;
		
		stringValue = va_arg(args, char *);
		if (strcmp(self->expectedCalls[self->nextExpectedCallIndex].value.stringValue, stringValue)) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be \"%s\", but was \"%s\" instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.stringValue, stringValue);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeBlob) {
		void * blobValue;
		size_t length;
		
		blobValue = va_arg(args, void *);
		length = va_arg(args, size_t);
		if (length != self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[0].length) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 4 to call %d (%s) was expected to be " SIZE_T_FORMAT ", but was " SIZE_T_FORMAT " instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[0].length, length);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		if (memcmp(self->expectedCalls[self->nextExpectedCallIndex].value.blobValue, blobValue, length)) {
			char blobString1[256], blobString2[256];
			
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %s, but was %s instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), printHexString(self->expectedCalls[self->nextExpectedCallIndex].value.blobValue, length, blobString1, sizeof(blobString1)), printHexString(blobValue, length, blobString2, sizeof(blobString2)));
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeBoolean) {
		bool boolValue;
		
		boolValue = va_arg(args, int);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.boolValue != boolValue) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %s, but was %s instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.boolValue ? "true" : "false", boolValue ? "true" : "false");
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
		
	} else if (functionPtr == self->writeEnumeration) {
		int enumValue;
		
		enumValue = va_arg(args, int);
		if (self->expectedCalls[self->nextExpectedCallIndex].value.enumValue != enumValue) {
			snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Arg 3 to call %d (%s) was expected to be %d, but was %d instead", self->nextExpectedCallIndex, functionNameForPtr(self, functionPtr), self->expectedCalls[self->nextExpectedCallIndex].value.enumValue, enumValue);
			longjmp(*self->sequenceBreakJmpEnv, 3);
		}
	}
	
	if (functionPtr == self->writeEnumeration) {
		unsigned int additionalArgIndex = 0;
		char * string;
		int enumValue;
		va_list * args2;
		
		args2 = va_arg(args, va_list *);
		do {
			string = va_arg(*args2, char *);
			if (string == NULL) {
				if (self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue != NULL) {
					snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Fewer additional args specified to %s (call index %d) than expected (got NULL instead of \"%s\")", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
			} else {
				enumValue = va_arg(*args2, int);
				
				if (self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue == NULL) {
					snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "More additional args specified to %s (call index %d) than expected (got \"%s\" instead of NULL)", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, string);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
				if (strcmp(self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue, string)) {
					snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Nonmatching additional arg specified to %s (call index %d) (got \"%s\" instead of \"%s\")", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, string, self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
				if (self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex + 1].enumValue != enumValue) {
					snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Nonmatching additional arg specified to %s (call index %d) (got %d instead of %d)", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, enumValue, self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex + 1].enumValue);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
			}
			additionalArgIndex += 2;
		} while (string != NULL);
	
	} else if (functionPtr == self->writeBitfield8 || functionPtr == self->writeBitfield16 || functionPtr == self->writeBitfield32 || functionPtr == self->writeBitfield64) {
		unsigned int additionalArgIndex = 0;
		char * string;
		va_list * args2;
		
		args2 = va_arg(args, va_list *);
		do {
			string = va_arg(*args2, char *);
			if (string == NULL) {
				if (self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue != NULL) {
					snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Fewer additional args specified to %s (call index %d) than expected (got NULL instead of \"%s\")", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
			} else {
				if (self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue == NULL) {
					snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "More additional args specified to %s (call index %d) than expected (got \"%s\" instead of NULL)", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, string);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
				if (strcmp(self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue, string)) {
					snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "Nonmatching additional arg specified to %s (call index %d) (got \"%s\" instead of \"%s\")", functionNameForPtr(self, functionPtr), self->nextExpectedCallIndex, string, self->expectedCalls[self->nextExpectedCallIndex].additionalArgs[additionalArgIndex].stringValue);
					longjmp(*self->sequenceBreakJmpEnv, 3);
				}
			}
			additionalArgIndex++;
		} while (string != NULL);
	}
	va_end(args);
	
	self->nextExpectedCallIndex++;
}

static void failIfRequested(TestSerializationContext * self) {
	if (self->nextExpectedCallIndex - 1 == self->callIndexToFail) {
		self->status = self->failStatus;
		if (self->jmpBuf != NULL) {
			longjmp(*self->jmpBuf, self->status);
		}
	}
}

void TestSerializationContext_beginStructure(TestSerializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->beginStructure, key);
	failIfRequested(self);
}

void TestSerializationContext_beginDictionary(TestSerializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->beginDictionary, key);
	failIfRequested(self);
}

void TestSerializationContext_beginArray(TestSerializationContext * self, const char * key) {
	verifyCallIsInSequence(self, self->beginArray, key);
	failIfRequested(self);
}

void TestSerializationContext_endStructure(TestSerializationContext * self) {
	verifyCallIsInSequence(self, self->endStructure);
	failIfRequested(self);
}

void TestSerializationContext_endDictionary(TestSerializationContext * self) {
	verifyCallIsInSequence(self, self->endDictionary);
	failIfRequested(self);
}

void TestSerializationContext_endArray(TestSerializationContext * self) {
	verifyCallIsInSequence(self, self->endArray);
	failIfRequested(self);
}

void TestSerializationContext_writeBoolean(TestSerializationContext * self, const char * key, bool value) {
	verifyCallIsInSequence(self, self->writeBoolean, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeInt8(TestSerializationContext * self, const char * key, int8_t value) {
	verifyCallIsInSequence(self, self->writeInt8, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeUInt8(TestSerializationContext * self, const char * key, uint8_t value) {
	verifyCallIsInSequence(self, self->writeUInt8, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeInt16(TestSerializationContext * self, const char * key, int16_t value) {
	verifyCallIsInSequence(self, self->writeInt16, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeUInt16(TestSerializationContext * self, const char * key, uint16_t value) {
	verifyCallIsInSequence(self, self->writeUInt16, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeInt32(TestSerializationContext * self, const char * key, int32_t value) {
	verifyCallIsInSequence(self, self->writeInt32, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeUInt32(TestSerializationContext * self, const char * key, uint32_t value) {
	verifyCallIsInSequence(self, self->writeUInt32, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeInt64(TestSerializationContext * self, const char * key, int64_t value) {
	verifyCallIsInSequence(self, self->writeInt64, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeUInt64(TestSerializationContext * self, const char * key, uint64_t value) {
	verifyCallIsInSequence(self, self->writeUInt64, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeFloat(TestSerializationContext * self, const char * key, float value) {
	verifyCallIsInSequence(self, self->writeFloat, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeDouble(TestSerializationContext * self, const char * key, double value) {
	verifyCallIsInSequence(self, self->writeDouble, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeFixed16_16(TestSerializationContext * self, const char * key, fixed16_16 value) {
	verifyCallIsInSequence(self, self->writeFixed16_16, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeEnumeration(TestSerializationContext * self, const char * key, int value, ...) {
	va_list args;
	
	va_start(args, value);
	verifyCallIsInSequence(self, self->writeEnumeration, key, value, &args);
	va_end(args);
	failIfRequested(self);
}

void TestSerializationContext_writeBitfield8(TestSerializationContext * self, const char * key, uint8_t value, ...) {
	va_list args;
	
	va_start(args, value);
	verifyCallIsInSequence(self, self->writeBitfield8, key, value, &args);
	va_end(args);
	failIfRequested(self);
}

void TestSerializationContext_writeBitfield16(TestSerializationContext * self, const char * key, uint16_t value, ...) {
	va_list args;
	
	va_start(args, value);
	verifyCallIsInSequence(self, self->writeBitfield16, key, value, &args);
	va_end(args);
	failIfRequested(self);
}

void TestSerializationContext_writeBitfield32(TestSerializationContext * self, const char * key, uint32_t value, ...) {
	va_list args;
	
	va_start(args, value);
	verifyCallIsInSequence(self, self->writeBitfield32, key, value, &args);
	va_end(args);
	failIfRequested(self);
}

void TestSerializationContext_writeBitfield64(TestSerializationContext * self, const char * key, uint64_t value, ...) {
	va_list args;
	
	va_start(args, value);
	verifyCallIsInSequence(self, self->writeBitfield64, key, value, &args);
	va_end(args);
	failIfRequested(self);
}

void TestSerializationContext_writeString(TestSerializationContext * self, const char * key, const char * value) {
	verifyCallIsInSequence(self, self->writeString, key, value);
	failIfRequested(self);
}

void TestSerializationContext_writeBlob(TestSerializationContext * self, const char * key, const void * value, size_t length) {
	verifyCallIsInSequence(self, self->writeBlob, key, value, length);
	failIfRequested(self);
}

void TestSerializationContext_expectCall(TestSerializationContext * self, void * functionPtr, ...) {
	va_list args;
	
	self->expectedCalls = realloc(self->expectedCalls, sizeof(struct TestSerializationContext_expectedCall) * (self->numExpectedCalls + 1));
	self->expectedCalls[self->numExpectedCalls].functionPtr = functionPtr;
	self->expectedCalls[self->numExpectedCalls].additionalArgs = NULL;
	
	va_start(args, functionPtr);
	if (functionPtr != self->endStructure && functionPtr != self->endDictionary && functionPtr != self->endArray) {
		self->expectedCalls[self->numExpectedCalls].key = va_arg(args, char *);
	}
	
	if (functionPtr == self->writeBoolean) {
		self->expectedCalls[self->numExpectedCalls].value.boolValue = va_arg(args, int);
		
	} else if (functionPtr == self->writeInt8) {
		self->expectedCalls[self->numExpectedCalls].value.int8Value = va_arg(args, int);
		
	} else if (functionPtr == self->writeUInt8 || functionPtr == self->writeBitfield8) {
		self->expectedCalls[self->numExpectedCalls].value.uint8Value = va_arg(args, int);
		
	} else if (functionPtr == self->writeInt16) {
		self->expectedCalls[self->numExpectedCalls].value.int16Value = va_arg(args, int);
		
	} else if (functionPtr == self->writeUInt16 || functionPtr == self->writeBitfield16) {
		self->expectedCalls[self->numExpectedCalls].value.uint16Value = va_arg(args, int);
		
	} else if (functionPtr == self->writeInt32) {
		self->expectedCalls[self->numExpectedCalls].value.int32Value = va_arg(args, int32_t);
		
	} else if (functionPtr == self->writeUInt32 || functionPtr == self->writeBitfield32) {
		self->expectedCalls[self->numExpectedCalls].value.uint32Value = va_arg(args, uint32_t);
		
	} else if (functionPtr == self->writeInt64) {
		self->expectedCalls[self->numExpectedCalls].value.int64Value = va_arg(args, int64_t);
		
	} else if (functionPtr == self->writeUInt64 || functionPtr == self->writeBitfield64) {
		self->expectedCalls[self->numExpectedCalls].value.uint64Value = va_arg(args, uint64_t);
		
	} else if (functionPtr == self->writeFloat) {
		self->expectedCalls[self->numExpectedCalls].value.floatValue = va_arg(args, double);
		
	} else if (functionPtr == self->writeDouble) {
		self->expectedCalls[self->numExpectedCalls].value.doubleValue = va_arg(args, double);
		
	} else if (functionPtr == self->writeFixed16_16) {
		self->expectedCalls[self->numExpectedCalls].value.fixed16_16Value = va_arg(args, fixed16_16);
		
	} else if (functionPtr == self->writeEnumeration) {
		self->expectedCalls[self->numExpectedCalls].value.enumValue = va_arg(args, int);
		
	} else if (functionPtr == self->writeString) {
		self->expectedCalls[self->numExpectedCalls].value.stringValue = va_arg(args, const char *);
		
	} else if (functionPtr == self->writeBlob) {
		self->expectedCalls[self->numExpectedCalls].value.blobValue = va_arg(args, const void *);
		self->expectedCalls[self->numExpectedCalls].additionalArgs = malloc(sizeof(union TestSerializationContext_additionalArg));
		self->expectedCalls[self->numExpectedCalls].additionalArgs[0].length = va_arg(args, size_t);
	}
	
	if (functionPtr == self->writeEnumeration) {
		unsigned int additionalArgIndex = 0;
		char * string;
		int enumValue;
		
		do {
			string = va_arg(args, char *);
			enumValue = va_arg(args, int);
			self->expectedCalls[self->numExpectedCalls].additionalArgs = realloc(self->expectedCalls[self->numExpectedCalls].additionalArgs, sizeof(union TestSerializationContext_additionalArg) * (additionalArgIndex + 2));
			self->expectedCalls[self->numExpectedCalls].additionalArgs[additionalArgIndex++].stringValue = string;
			self->expectedCalls[self->numExpectedCalls].additionalArgs[additionalArgIndex++].enumValue = enumValue;
		} while (string != NULL);
		
	} else if (functionPtr == self->writeBitfield8 || functionPtr == self->writeBitfield16 || functionPtr == self->writeBitfield32 || functionPtr == self->writeBitfield64) {
		unsigned int additionalArgIndex = 0;
		char * string;
		
		do {
			string = va_arg(args, char *);
			self->expectedCalls[self->numExpectedCalls].additionalArgs = realloc(self->expectedCalls[self->numExpectedCalls].additionalArgs, sizeof(union TestSerializationContext_additionalArg) * (additionalArgIndex + 1));
			self->expectedCalls[self->numExpectedCalls].additionalArgs[additionalArgIndex++].stringValue = string;
		} while (string != NULL);
	}
	va_end(args);
	
	self->numExpectedCalls++;
}

void TestSerializationContext_failNthCall(TestSerializationContext * self, unsigned int callIndex, int status) {
	self->callIndexToFail = callIndex;
	self->failStatus = status;
}

void TestSerializationContext_finish(TestSerializationContext * self) {
	if (self->nextExpectedCallIndex < self->numExpectedCalls) {
		snprintf_safe(self->error, SERIALIZATION_ERROR_MAX, "%d expected call%s still left in queue at end (next expected call is %s)", self->numExpectedCalls - self->nextExpectedCallIndex, self->numExpectedCalls - self->nextExpectedCallIndex == 1 ? "" : "s", functionNameForPtr(self, self->expectedCalls[self->nextExpectedCallIndex].functionPtr));
		longjmp(*self->sequenceBreakJmpEnv, 4);
	}
}

void TestSerializationContext_rewind(TestSerializationContext * self) {
	self->nextExpectedCallIndex = 0;
	self->error[0] = '\x00';
	self->status = SERIALIZATION_ERROR_OK;
}
