#include <stdio.h>
#include <string.h>

#include "utilities/UTFUtilities.h"

#include "unittest/framework/TestSuite.h"

static void testConversions() {
	uint8_t stringUTF8[] = {0x61, 0xC2, 0xA3, 0xE4, 0xB8, 0x89, 0xF0, 0x9D, 0x84, 0x90, 0x7A, 0x00};
	uint16_t stringUTF16[] = {0x0061, 0x00A3, 0x4E09, 0xD834, 0xDD10, 0x007A, 0x0000};
	uint32_t stringUTF32[] = {0x00000061, 0x000000A3, 0x00004E09, 0x0001D110, 0x0000007A, 0x00000000};
	uint8_t * resultUTF8;
	uint16_t * resultUTF16;
	uint32_t * resultUTF32;
	size_t length;
	
	resultUTF8 = utf32StringToUTF8String(stringUTF32, utf32StringLength(stringUTF32));
	length = utf8StringLength(resultUTF8);
	TestCase_assert(length == sizeof(stringUTF8) / sizeof(uint8_t) - 1, "Expected length to be %lu, but got %lu", (unsigned long) (sizeof(stringUTF8) / sizeof(uint8_t) - 1), (unsigned long) length);
	TestCase_assert(!memcmp(stringUTF8, resultUTF8, sizeof(stringUTF8)), "Expected {0x61, 0xC2, 0xA3, 0xE4, 0xB8, 0x89, 0xF0, 0x9D, 0x84, 0x90, 0x7A, 0x00}, but got {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}", resultUTF8[0], resultUTF8[1], resultUTF8[2], resultUTF8[3], resultUTF8[4], resultUTF8[5], resultUTF8[6], resultUTF8[7], resultUTF8[8], resultUTF8[9], resultUTF8[10], resultUTF8[11]);
	free(resultUTF8);
	
	resultUTF16 = utf32StringToUTF16String(stringUTF32, utf32StringLength(stringUTF32));
	length = utf16StringLength(resultUTF16);
	TestCase_assert(length == sizeof(stringUTF16) / sizeof(uint16_t) - 1, "Expected length to be %lu, but got %lu", (unsigned long) (sizeof(stringUTF16) / sizeof(uint16_t) - 1), (unsigned long) length);
	TestCase_assert(!memcmp(stringUTF16, resultUTF16, sizeof(stringUTF16)), "Expected {0x0061, 0x00A3, 0x4E09, 0xD834, 0xDD10, 0x007A, 0x0000}, but got {0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X}", resultUTF16[0], resultUTF16[1], resultUTF16[2], resultUTF16[3], resultUTF16[4], resultUTF16[5], resultUTF16[6]);
	free(resultUTF16);
	
	resultUTF8 = utf16StringToUTF8String(stringUTF16, utf16StringLength(stringUTF16));
	length = utf8StringLength(resultUTF8);
	TestCase_assert(length == sizeof(stringUTF8) / sizeof(uint8_t) - 1, "Expected length to be %lu, but got %lu", (unsigned long) (sizeof(stringUTF8) / sizeof(uint8_t) - 1), (unsigned long) length);
	TestCase_assert(!memcmp(stringUTF8, resultUTF8, sizeof(stringUTF8)), "Expected {0x61, 0xC2, 0xA3, 0xE4, 0xB8, 0x89, 0xF0, 0x9D, 0x84, 0x90, 0x7A, 0x00}, but got {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}", resultUTF8[0], resultUTF8[1], resultUTF8[2], resultUTF8[3], resultUTF8[4], resultUTF8[5], resultUTF8[6], resultUTF8[7], resultUTF8[8], resultUTF8[9], resultUTF8[10], resultUTF8[11]);
	free(resultUTF8);
	
	resultUTF32 = utf16StringToUTF32String(stringUTF16, utf16StringLength(stringUTF16));
	length = utf32StringLength(resultUTF32);
	TestCase_assert(length == sizeof(stringUTF32) / sizeof(uint32_t) - 1, "Expected length to be %lu, but got %lu", (unsigned long) (sizeof(stringUTF32) / sizeof(uint32_t) - 1), (unsigned long) length);
	TestCase_assert(!memcmp(stringUTF32, resultUTF32, sizeof(stringUTF32)), "Expected {0x00000061, 0x000000A0, 0x00004E09, 0x0001D110, 0x0000007A, 0x00000000}, but got {0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X}", resultUTF32[0], resultUTF32[1], resultUTF32[2], resultUTF32[3], resultUTF32[4], resultUTF32[5]);
	free(resultUTF32);
	
	resultUTF16 = utf8StringToUTF16String(stringUTF8, utf8StringLength(stringUTF8));
	length = utf16StringLength(resultUTF16);
	TestCase_assert(length == sizeof(stringUTF16) / sizeof(uint16_t) - 1, "Expected length to be %lu, but got %lu", (unsigned long) (sizeof(stringUTF16) / sizeof(uint16_t) - 1), (unsigned long) length);
	TestCase_assert(!memcmp(stringUTF16, resultUTF16, sizeof(stringUTF16)), "Expected {0x0061, 0x00A3, 0x4E09, 0xD834, 0xDD10, 0x007A, 0x0000}, but got {0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X, 0x%04X}", resultUTF16[0], resultUTF16[1], resultUTF16[2], resultUTF16[3], resultUTF16[4], resultUTF16[5], resultUTF16[6]);
	free(resultUTF16);
	
	resultUTF32 = utf8StringToUTF32String(stringUTF8, utf8StringLength(stringUTF8));
	length = utf32StringLength(resultUTF32);
	TestCase_assert(length == sizeof(stringUTF32) / sizeof(uint32_t) - 1, "Expected length to be %lu, but got %lu", (unsigned long) (sizeof(stringUTF32) / sizeof(uint32_t) - 1), (unsigned long) length);
	TestCase_assert(!memcmp(stringUTF32, resultUTF32, sizeof(stringUTF32)), "Expected {0x00000061, 0x000000A0, 0x00004E09, 0x0001D110, 0x0000007A, 0x00000000}, but got {0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X}", resultUTF32[0], resultUTF32[1], resultUTF32[2], resultUTF32[3], resultUTF32[4], resultUTF32[5]);
	free(resultUTF32);
}

static void testLength() {
	uint8_t stringUTF8[] = {0x61, 0xC2, 0xA3, 0xE4, 0xB8, 0x89, 0xF0, 0x9D, 0x84, 0x90, 0x7A, 0x00};
	uint16_t stringUTF16[] = {0x0061, 0x00A3, 0x4E09, 0xD834, 0xDD10, 0x007A, 0x0000};
	uint32_t stringUTF32[] = {0x00000061, 0x000000A3, 0x00004E09, 0x0001D110, 0x0000007A, 0x00000000};
	size_t length;
	
	length = utf8StringLength(stringUTF8);
	TestCase_assert(length == sizeof(stringUTF8) / sizeof(uint8_t) - 1, "Expected length == %lu, but got %lu\n", (unsigned long) (sizeof(stringUTF8) / sizeof(uint8_t) - 1), (unsigned long) length);
	length = utf16StringLength(stringUTF16);
	TestCase_assert(length == sizeof(stringUTF16) / sizeof(uint16_t) - 1, "Expected length == %lu, but got %lu\n", (unsigned long) (sizeof(stringUTF16) / sizeof(uint16_t) - 1), (unsigned long) length);
	length = utf32StringLength(stringUTF32);
	TestCase_assert(length == sizeof(stringUTF32) / sizeof(uint32_t) - 1, "Expected length == %lu, but got %lu\n", (unsigned long) (sizeof(stringUTF32) / sizeof(uint32_t) - 1), (unsigned long) length);
	
	length = utf8StringUTF16Length(stringUTF8, sizeof(stringUTF8) / sizeof(uint8_t) - 1);
	TestCase_assert(length == sizeof(stringUTF16) / sizeof(uint16_t) - 1, "Expected length == %lu, but got %lu\n", (unsigned long) (sizeof(stringUTF16) / sizeof(uint16_t) - 1), (unsigned long) length);
	length = utf8StringUTF32Length(stringUTF8, sizeof(stringUTF8) / sizeof(uint8_t) - 1);
	TestCase_assert(length == sizeof(stringUTF32) / sizeof(uint32_t) - 1, "Expected length == %lu, but got %lu\n", (unsigned long) (sizeof(stringUTF32) / sizeof(uint32_t) - 1), (unsigned long) length);
	
	length = utf16StringUTF8Length(stringUTF16, sizeof(stringUTF16) / sizeof(uint16_t) - 1);
	TestCase_assert(length == sizeof(stringUTF8) / sizeof(uint8_t) - 1, "Expected length == %lu, but got %lu\n", (unsigned long) (sizeof(stringUTF8) / sizeof(uint8_t) - 1), (unsigned long) length);
	length = utf16StringUTF32Length(stringUTF16, sizeof(stringUTF16) / sizeof(uint16_t) - 1);
	TestCase_assert(length == sizeof(stringUTF32) / sizeof(uint32_t) - 1, "Expected length == %lu, but got %lu\n", (unsigned long) (sizeof(stringUTF32) / sizeof(uint32_t) - 1), (unsigned long) length);
	
	length = utf32StringUTF8Length(stringUTF32, sizeof(stringUTF32) / sizeof(uint32_t) - 1);
	TestCase_assert(length == sizeof(stringUTF8) / sizeof(uint8_t) - 1, "Expected length == %lu, but got %lu\n", (unsigned long) (sizeof(stringUTF8) / sizeof(uint8_t) - 1), (unsigned long) length);
	length = utf32StringUTF16Length(stringUTF32, sizeof(stringUTF32) / sizeof(uint32_t) - 1);
	TestCase_assert(length == sizeof(stringUTF16) / sizeof(uint16_t) - 1, "Expected length == %lu, but got %lu\n", (unsigned long) (sizeof(stringUTF16) / sizeof(uint16_t) - 1), (unsigned long) length);
}

static void testMalformedness() {
	uint8_t wellFormedStringUTF8[] = {0x61, 0xC2, 0xA3, 0xE4, 0xB8, 0x89, 0xF0, 0x9D, 0x84, 0x90, 0x7A, 0x00};
	uint16_t wellFormedStringUTF16[] = {0x0061, 0x00A3, 0x4E09, 0xD834, 0xDD10, 0x007A, 0x0000};
	uint32_t wellFormedStringUTF32[] = {0x00000061, 0x000000A3, 0x00004E09, 0x0001D110, 0x0000007A, 0x00000000};
	uint8_t malformedStringsUTF8[][5] = {
		{0x80, 0x00, 0x00, 0x00, 0x00},
		{0xC0, 0x00, 0x00, 0x00, 0x00},
		{0xC0, 0xC0, 0x00, 0x00, 0x00},
		{0xC0, 0x20, 0x00, 0x00, 0x00},
		{0xE0, 0xC0, 0x80, 0x00, 0x00},
		{0xE0, 0x80, 0xC0, 0x00, 0x00},
		{0xF0, 0x80, 0x80, 0xC0, 0x00},
		{0xF8, 0x80, 0x80, 0x80, 0x00}
	};
	uint16_t malformedStringsUTF16[][3] = {
		{0xD800, 0x0000, 0x0000},
		{0xDC00, 0xDC00, 0x0000},
		{0xD800, 0x0020, 0x0000},
		{0xD800, 0xD800, 0x0000}
	};
	uint32_t malformedStringsUTF32[][2] = {
		{0x00110000, 0x00000000},
		{0x0000D800, 0x00000000},
		{0x0000DFFF, 0x00000000},
		{0xFFFFFFFF, 0x00000000}
	};
	unsigned int malformedStringIndex;
	
	TestCase_assert(utf8StringIsWellFormed(wellFormedStringUTF8, sizeof(wellFormedStringUTF8) / sizeof(uint8_t) - 1), "Got malformed for well-formed string");
	TestCase_assert(utf16StringIsWellFormed(wellFormedStringUTF16, sizeof(wellFormedStringUTF16) / sizeof(uint16_t) - 1), "Got malformed for well-formed string");
	TestCase_assert(utf32StringIsWellFormed(wellFormedStringUTF32, sizeof(wellFormedStringUTF32) / sizeof(uint32_t) - 1), "Got malformed for well-formed string");
	
	for (malformedStringIndex = 0; malformedStringIndex < sizeof(malformedStringsUTF8) / (sizeof(uint8_t) * 5); malformedStringIndex++) {
		TestCase_assert(!utf8StringIsWellFormed(malformedStringsUTF8[malformedStringIndex], utf8StringLength(malformedStringsUTF8[malformedStringIndex])), "Got well-formed for malformed string %d", malformedStringIndex);
	}
	for (malformedStringIndex = 0; malformedStringIndex < sizeof(malformedStringsUTF16) / (sizeof(uint16_t) * 3); malformedStringIndex++) {
		TestCase_assert(!utf16StringIsWellFormed(malformedStringsUTF16[malformedStringIndex], utf16StringLength(malformedStringsUTF16[malformedStringIndex])), "Got well-formed for malformed string %d", malformedStringIndex);
	}
	for (malformedStringIndex = 0; malformedStringIndex < sizeof(malformedStringsUTF32) / (sizeof(uint32_t) * 2); malformedStringIndex++) {
		TestCase_assert(!utf32StringIsWellFormed(malformedStringsUTF32[malformedStringIndex], utf32StringLength(malformedStringsUTF32[malformedStringIndex])), "Got well-formed for malformed string %d", malformedStringIndex);
	}
}

TEST_SUITE(UTFUtilitiesTest, testConversions, testLength, testMalformedness)
