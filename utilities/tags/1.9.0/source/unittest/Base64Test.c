#include "unittest/TestSuite.h"
#include "utilities/Base64.h"
#include "utilities/IOUtilities.h"
#include "utilities/printfFormats.h"
#include <stdbool.h>

static const char allBase64CharsInOrder[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char allBytesInOrder[256] =
	"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
	"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
	"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F"
	"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
	"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F"
	"\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
	"\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F"
	"\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
	"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
	"\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
	"\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
	"\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
	"\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF"
	"\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF"
	"\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF"
	"\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF";

static void testEncode() {
	char encodedString[512];
	size_t length;
	
	memset(encodedString, 0xFF, 512);
	length = encodeBase64("", 0, encodedString, 512);
	TestCase_assert(length == 0, "Expected 0 but got " SIZE_T_FORMAT, length);
	TestCase_assert(encodedString[0] == 0, "Expected \"\" but got \"%s\"", encodedString);
	
	length = encodeBase64("Hello", 5, NULL, 0);
	TestCase_assert(length == 8, "Expected 8 but got " SIZE_T_FORMAT, length);
	
	memset(encodedString, 0xFF, 512);
	length = encodeBase64("Hello", 5, encodedString, 512);
	TestCase_assert(length == 8, "Expected 8 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(encodedString, "SGVsbG8="), "Expected \"SGVsbG8=\" but got \"%s\"", encodedString);
	
	memset(encodedString, 0xFF, 512);
	length = encodeBase64("Hello!", 6, encodedString, 512);
	TestCase_assert(length == 8, "Expected 8 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(encodedString, "SGVsbG8h"), "Expected \"SGVsbG8h\" but got \"%s\"", encodedString);
	
	memset(encodedString, 0xFF, 512);
	length = encodeBase64("ello", 4, encodedString, 512);
	TestCase_assert(length == 8, "Expected 8 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(encodedString, "ZWxsbw=="), "Expected \"ZWxsbw==\" but got \"%s\"", encodedString);
	
	length = encodeBase64("llo", 3, NULL, 0);
	TestCase_assert(length == 4, "Expected 4 but got " SIZE_T_FORMAT, length);
	
	memset(encodedString, 0xFF, 512);
	length = encodeBase64("llo", 3, encodedString, 512);
	TestCase_assert(length == 4, "Expected 4 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(encodedString, "bGxv"), "Expected \"bGxv\" but got \"%s\"", encodedString);
	
	memset(encodedString, 0xFF, 512);
	length = encodeBase64("ello", 4, encodedString, 3);
	TestCase_assert(length == 8, "Expected 8 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(encodedString, "ZW"), "Expected \"ZW\" but got \"%s\"", encodedString);
	
	memset(encodedString, 0xFF, 512);
	length = encodeBase64(allBytesInOrder, 256, encodedString, 512);
	TestCase_assert(length == 344, "Expected 344 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(encodedString, "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w=="), "Expected \"AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w==\" but got \"%s\"", encodedString);
	
	memset(encodedString, 0xFF, 512);
	length = encodeBase64("\x00\x10\x83\x10Q\x87 \x92\x8B" "0\xD3\x8F" "A\x14\x93QU\x97" "a\x96\x9Bq\xD7\x9F\x82\x18\xA3\x92Y\xA7\xA2\x9A\xAB\xB2\xDB\xAF\xC3\x1C\xB3\xD3]\xB7\xE3\x9E\xBB\xF3\xDF\xBF", 48, encodedString, 512);
	TestCase_assert(length == 64, "Expected 64 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(encodedString, allBase64CharsInOrder), "Expected \"%s\" but got \"%s\"", allBase64CharsInOrder, encodedString);
}

static void testDecode() {
	char decodedString[512];
	char hexBuffer1[1024], hexBuffer2[1024];
	size_t length;
	
	memset(decodedString, 0xFF, 512);
	length = decodeBase64("", 0, decodedString, 512);
	TestCase_assert(length == 0, "Expected 0 but got " SIZE_T_FORMAT, length);
	TestCase_assert(decodedString[0] == 0, "Expected \"\" but got \"%s\"", decodedString);
	
	length = decodeBase64("SGVsbG8=", 8, NULL, 0);
	TestCase_assert(length == 5, "Expected 5 but got " SIZE_T_FORMAT, length);
	
	memset(decodedString, 0xFF, 512);
	length = decodeBase64("SGVsbG8=", 8, decodedString, 512);
	TestCase_assert(length == 5, "Expected 5 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(decodedString, "Hello"), "Expected \"Hello\" but got \"%s\"", decodedString);
	
	memset(decodedString, 0xFF, 512);
	length = decodeBase64("SGVsbG8h", 8, decodedString, 512);
	TestCase_assert(length == 6, "Expected 6 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(decodedString, "Hello!"), "Expected \"Hello!\" but got \"%s\"", decodedString);
	
	memset(decodedString, 0xFF, 512);
	length = decodeBase64("ZWxsbw==", 8, decodedString, 512);
	TestCase_assert(length == 4, "Expected 4 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(decodedString, "ello"), "Expected \"ello\" but got \"%s\"", decodedString);
	
	length = decodeBase64("bGxv", 4, NULL, 0);
	TestCase_assert(length == 3, "Expected 3 but got " SIZE_T_FORMAT, length);
	
	memset(decodedString, 0xFF, 512);
	length = decodeBase64("bGxv", 4, decodedString, 512);
	TestCase_assert(length == 3, "Expected 3 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(decodedString, "llo"), "Expected \"llo\" but got \"%s\"", decodedString);
	
	memset(decodedString, 0xFF, 512);
	length = decodeBase64("ZWxsbw==", 8, decodedString, 3);
	TestCase_assert(length == 4, "Expected 4 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(decodedString, "ell", 3), "Expected \"ell\" but got \"%.3s\"", decodedString);
	
	memset(decodedString, 0xFF, 512);
	length = decodeBase64("AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w==", 344, decodedString, 512);
	TestCase_assert(length == 256, "Expected 256 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!memcmp(decodedString, allBytesInOrder, 256), "Expected \"%s\" but got \"%s\"", printHexString(allBytesInOrder, 256, hexBuffer1, sizeof(hexBuffer1) - 1), printHexString(decodedString, 256, hexBuffer2, sizeof(hexBuffer2) - 1));
	
	memset(decodedString, 0xFF, 512);
	length = decodeBase64(allBase64CharsInOrder, 64, decodedString, 512);
	TestCase_assert(length == 48, "Expected 48 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!memcmp(decodedString, "\x00\x10\x83\x10Q\x87 \x92\x8B" "0\xD3\x8F" "A\x14\x93QU\x97" "a\x96\x9Bq\xD7\x9F\x82\x18\xA3\x92Y\xA7\xA2\x9A\xAB\xB2\xDB\xAF\xC3\x1C\xB3\xD3]\xB7\xE3\x9E\xBB\xF3\xDF\xBF", 48), "Expected \"%s\" but got \"%s\"", printHexString("\x00\x10\x83\x10Q\x87 \x92\x8B" "0\xD3\x8F" "A\x14\x93QU\x97" "a\x96\x9Bq\xD7\x9F\x82\x18\xA3\x92Y\xA7\xA2\x9A\xAB\xB2\xDB\xAF\xC3\x1C\xB3\xD3]\xB7\xE3\x9E\xBB\xF3\xDF\xBF", 48, hexBuffer1, sizeof(hexBuffer1) - 1), printHexString(decodedString, 256, hexBuffer2, sizeof(hexBuffer2) - 1));
}

TEST_SUITE(Base64Test,
           testEncode,
           testDecode)
