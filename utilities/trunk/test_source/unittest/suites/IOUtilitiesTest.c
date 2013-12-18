#include "unittest/framework/TestSuite.h"
#include "utilities/IOUtilities.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define assertBytesMatch(expectedBytes, actualBytes, length) { \
	char * _failString; \
	unsigned int _index; \
	\
	_failString = malloc(strlen("Expected but got") + 3 * (length) * 2 + 1); \
	_failString[0] = 0; \
	strcat(_failString, "Expected"); \
	for (_index = 0; _index < (length); _index++) { \
		sprintf(_failString + strlen(_failString), " %02X", ((unsigned char *) (expectedBytes))[_index]); \
	} \
	strcat(_failString, " but got"); \
	for (_index = 0; _index < (length); _index++) { \
		sprintf(_failString + strlen(_failString), " %02X", ((unsigned char *) (actualBytes))[_index]); \
	} \
	\
	TestCase_assert(!memcmp((expectedBytes), (actualBytes), (length)), "%s", _failString); \
	\
	free(_failString); \
}

#ifdef WIN32
#include <fcntl.h>
#define _S_IREAD 256
#define _S_IWRITE 128
static int mkstemp(char * template) {
	int result = -1;
	mktemp(template); 
	result = open(template, O_RDWR | O_BINARY | O_CREAT | O_EXCL | _O_SHORT_LIVED, _S_IREAD | _S_IWRITE); 
	return result;
}
#endif

static void testMemreadContextInit() {
	struct memreadContext context;
	char * data1 = "abcd", * data2 = "efg";
	
	context = memreadContextInit(data1, 4);
	TestCase_assert(context.data == data1, "Expected %p but got %p", data1, context.data);
	TestCase_assert(context.length == 4, "Expected 4 but got %d", (int) context.length);
	TestCase_assert(context.position == 0, "Expected 0 but got %d", (int) context.position);
	
	context = memreadContextInit(data2, 3);
	TestCase_assert(context.data == data2, "Expected %p but got %p", data2, context.data);
	TestCase_assert(context.length == 3, "Expected 3 but got %d", (int) context.length);
	TestCase_assert(context.position == 0, "Expected 0 but got %d", (int) context.position);
}

static void testMemread() {
	char target[4];
	char expectedTarget1[4] = {'a', 'b', 'c', 'd'}, expectedTarget2[4] = {'1', '2', '3', '\x00'};
	struct memreadContext context;
	bool result;
	
	context = memreadContextInit("abcd1234", 8);
	memset(target, 0, 4);
	result = memread(&context, 4, target);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(context.position == 4, "Expected 4 but got %d", (int) context.position);
	assertBytesMatch(expectedTarget1, target, 4);
	
	memset(target, 0, 4);
	result = memread(&context, 3, target);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(context.position == 7, "Expected 7 but got %d", (int) context.position);
	assertBytesMatch(expectedTarget2, target, 4);
	
	result = memread(&context, 2, NULL);
	TestCase_assert(!result, "Expected false but got true");
	TestCase_assert(context.position == 7, "Expected 7 but got %d", (int) context.position);
	
	result = memread(&context, 1, NULL);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(context.position == 8, "Expected 8 but got %d", (int) context.position);
}

static void testMemwriteContextInit() {
	struct memwriteContext context;
	char * data1 = "abcd", * data2 = "efg";
	
	context = memwriteContextInit(data1, 4, 5, false);
	TestCase_assert(context.data == data1, "Expected %p but got %p", data1, context.data);
	TestCase_assert(context.length == 4, "Expected 4 but got %d", (int) context.length);
	TestCase_assert(context.allocatedSize == 5, "Expected 5 but got %d", (int) context.allocatedSize);
	TestCase_assert(context.position == 0, "Expected 0 but got %d", (int) context.position);
	TestCase_assert(!context.realloc, "Expected false but got true");
	
	context = memwriteContextInit(data2, 3, 4, true);
	TestCase_assert(context.data == data2, "Expected %p but got %p", data2, context.data);
	TestCase_assert(context.length == 3, "Expected 3 but got %d", (int) context.length);
	TestCase_assert(context.allocatedSize == 4, "Expected 4 but got %d", (int) context.allocatedSize);
	TestCase_assert(context.position == 0, "Expected 0 but got %d", (int) context.position);
	TestCase_assert(context.realloc, "Expected true but got false");
}

static void testMemwrite() {
	struct memwriteContext context;
	char target[4];
	bool result;
	
	memset(target, 0, 4);
	context = memwriteContextInit(target, 3, 4, false);
	result = memwrite(&context, 2, "fg");
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(context.length == 3, "Expected 3 but got %d", (int) context.length);
	TestCase_assert(context.position == 2, "Expected 2 but got %d", (int) context.position);
	assertBytesMatch("fg\x00\x00", target, 4);
	
	result = memwrite(&context, 3, "sfd");
	TestCase_assert(!result, "Expected false but got true");
	TestCase_assert(context.length == 3, "Expected 3 but got %d", (int) context.length);
	TestCase_assert(context.position == 2, "Expected 2 but got %d", (int) context.position);
	assertBytesMatch("fg\x00\x00", target, 4);
	
	result = memwrite(&context, 1, "s");
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(context.length == 3, "Expected 3 but got %d", (int) context.length);
	TestCase_assert(context.position == 3, "Expected 3 but got %d", (int) context.position);
	assertBytesMatch("fgs\x00", target, 4);
	
	target[3] = 0xFF;
	result = memwrite(&context, 1, NULL);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(context.length == 4, "Expected 4 but got %d", (int) context.length);
	TestCase_assert(context.position == 4, "Expected 4 but got %d", (int) context.position);
	assertBytesMatch("fgs\x00", target, 4);
	
	context = memwriteContextInit(malloc(1), 0, 1, true);
	result = memwrite(&context, 2, "aa");
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(context.length == 2, "Expected 2 but got %d", (int) context.length);
	TestCase_assert(context.allocatedSize >= 2, "Expected >= 2 but got %d", (int) context.allocatedSize);
	TestCase_assert(context.position == 2, "Expected 2 but got %d", (int) context.position);
	assertBytesMatch("aa", context.data, 2);
	
	result = memwrite(&context, 5, NULL);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(context.length == 7, "Expected 7 but got %d", (int) context.length);
	TestCase_assert(context.allocatedSize >= 7, "Expected >= 7 but got %d", (int) context.allocatedSize);
	TestCase_assert(context.position == 7, "Expected 7 but got %d", (int) context.position);
	assertBytesMatch("aa\x00\x00\x00\x00\x00", context.data, 7);
	
	context.position = 6;
	result = memwrite(&context, 3, "baz");
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(context.length == 9, "Expected 9 but got %d", (int) context.length);
	TestCase_assert(context.allocatedSize >= 9, "Expected >= 9 but got %d", (int) context.allocatedSize);
	TestCase_assert(context.position == 9, "Expected 9 but got %d", (int) context.position);
	assertBytesMatch("aa\x00\x00\x00\x00""baz", context.data, 9);
	
	free(context.data);
	
	context = memwriteContextInit(NULL, 0, 0, true);
	result = memwrite(&context, 2, "bb");
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(context.length == 2, "Expected 2 but got %d", (int) context.length);
	TestCase_assert(context.allocatedSize == 2, "Expected 2 but got %d", (int) context.allocatedSize);
	TestCase_assert(context.position == 2, "Expected 2 but got %d", (int) context.position);
	assertBytesMatch("bb", context.data, 2);
	
	free(context.data);
}

static void testReadFileSimple() {
	char fileName[10];
	int fileHandle;
	size_t length;
	void * result;
	FILE * file;
	
	strcpy(fileName, "tmpXXXXXX");
	fileHandle = mkstemp(fileName);
	close(fileHandle);
	unlink(fileName);
	result = readFileSimple(fileName, NULL);
	TestCase_assert(result == NULL, "Expected NULL but got %p", result);
	
	strcpy(fileName, "tmpXXXXXX");
	fileHandle = mkstemp(fileName);
	file = fdopen(fileHandle, "wb");
	fprintf(file, "foo bar\nbaz");
	fclose(file);
	result = readFileSimple(fileName, &length);
	unlink(fileName);
	TestCase_assert(result != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 11, "Expected 11 but got %d", (int) length);
	assertBytesMatch("foo bar\nbaz", result, 11);
	
	strcpy(fileName, "tmpXXXXXX");
	fileHandle = mkstemp(fileName);
	file = fdopen(fileHandle, "wb");
	fprintf(file, "Hello, world!");
	fclose(file);
	result = readFileSimple(fileName, &length);
	unlink(fileName);
	TestCase_assert(result != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 13, "Expected 13 but got %d", (int) length);
	assertBytesMatch("Hello, world!", result, 13);
}

static void testWriteFileSimple() {
	char fileName[10];
	int fileHandle;
	bool result;
	FILE * file;
	size_t fileLength;
	char * fileContents;
	
	strcpy(fileName, "tmpXXXXXX");
	fileHandle = mkstemp(fileName);
	result = writeFileSimple(fileName, "foo bar\nbaz", 11);
	file = fdopen(fileHandle, "rb");
	TestCase_assert(file != NULL, "writeFileSimple failed to create target file");
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	fseek(file, 0, SEEK_SET);
	fileContents = malloc(fileLength);
	fread(fileContents, 1, fileLength, file);
	fclose(file);
	unlink(fileName);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(fileLength == 11, "Expected 11 but got %d", (int) fileLength);
	assertBytesMatch("foo bar\nbaz", fileContents, 11);
	free(fileContents);
	
	strcpy(fileName, "tmpXXXXXX");
	fileHandle = mkstemp(fileName);
	result = writeFileSimple(fileName, "Hello, world!", 13);
	file = fdopen(fileHandle, "rb");
	TestCase_assert(file != NULL, "writeFileSimple failed to create target file");
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	fseek(file, 0, SEEK_SET);
	fileContents = malloc(fileLength);
	fread(fileContents, 1, fileLength, file);
	fclose(file);
	unlink(fileName);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(fileLength == 13, "Expected 13 but got %d", (int) fileLength);
	assertBytesMatch("Hello, world!", fileContents, 13);
	free(fileContents);
}

static void testTemporaryFilePath() {
	int fd;
	const char * template, * path;
	const char * uniqueString;
	bool success;
	FILE * file;
	size_t fileLength;
	char * fileContents;
	
	template = "tmpXXXXXX";
	uniqueString = "Unique string 1";
	fd = -1;
	path = temporaryFilePath(template, &fd);
	TestCase_assert(path != NULL, "temporaryFilePath unexpectedly returned NULL");
	TestCase_assert(strlen(path) >= strlen(template), "temporaryFilePath returned a shorter path (\"%s\") than the provided template", path);
	TestCase_assert(!strncmp(path + strlen(path) - strlen(template), template, 3), "Template prefix not preserved in path (\"%s\")", path);
	TestCase_assert(fd != -1, "temporaryFilePath didn't return a file descriptor");
	success = writeFileSimple(path, uniqueString, strlen(uniqueString));
	TestCase_assert(success, "Failed to write to returned path \"%s\"", path);
	file = fdopen(fd, "rb");
	TestCase_assert(file != NULL, "temporaryFilePath didn't return a file descriptor that could be opened with fdopen");
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	TestCase_assert(fileLength == strlen(uniqueString), "File length (%d) didn't match length of string written to path (%d)", (int) fileLength, (int) strlen(uniqueString));
	fseek(file, 0, SEEK_SET);
	fileContents = malloc(fileLength);
	fread(fileContents, 1, fileLength, file);
	TestCase_assert(!memcmp(fileContents, uniqueString, fileLength), "File contents \"%.*s\" didn't match expected string \"%s\"", (int) fileLength, fileContents, uniqueString);
	fclose(file);
	unlink(path);
	
	template = "helloXXXXXX";
	uniqueString = "Unique string 2";
	fd = -1;
	path = temporaryFilePath(template, &fd);
	TestCase_assert(path != NULL, "temporaryFilePath unexpectedly returned NULL");
	TestCase_assert(strlen(path) >= strlen(template), "temporaryFilePath returned a shorter path (\"%s\") than the provided template", path);
	TestCase_assert(!strncmp(path + strlen(path) - strlen(template), template, 5), "Template prefix not preserved in path (\"%s\")", path);
	TestCase_assert(fd != -1, "temporaryFilePath didn't return a file descriptor");
	success = writeFileSimple(path, uniqueString, strlen(uniqueString));
	TestCase_assert(success, "Failed to write to returned path \"%s\"", path);
	file = fdopen(fd, "rb");
	TestCase_assert(file != NULL, "temporaryFilePath didn't return a file descriptor that could be opened with fdopen");
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	TestCase_assert(fileLength == strlen(uniqueString), "File length (%d) didn't match length of string written to path (%d)", (int) fileLength, (int) strlen(uniqueString));
	fseek(file, 0, SEEK_SET);
	fileContents = malloc(fileLength);
	fread(fileContents, 1, fileLength, file);
	TestCase_assert(!memcmp(fileContents, uniqueString, fileLength), "File contents \"%.*s\" didn't match expected string \"%s\"", (int) fileLength, fileContents, uniqueString);
	fclose(file);
	unlink(path);
}

static void testEndianSwapping() {
	uint16_t value16;
	uint32_t value32;
	uint64_t value64;
	
	value16 = swapLittleEndian16(*(uint16_t *) "\x01\x00");
	TestCase_assert(value16 == 0x1, "Expected 0x1 but got 0x%X", value16);
	value16 = swapLittleEndian16(*(uint16_t *) "\x02\x03");
	TestCase_assert(value16 == 0x302, "Expected 0x302 but got 0x%X", value16);
	value16 = swapBigEndian16(*(uint16_t *) "\x01\x00");
	TestCase_assert(value16 == 0x100, "Expected 0x100 but got 0x%X", value16);
	value16 = swapBigEndian16(*(uint16_t *) "\x02\x03");
	TestCase_assert(value16 == 0x203, "Expected 0x203 but got 0x%X", value16);
	
	value32 = swapLittleEndian32(*(uint32_t *) "\x01\x00\x00\x00");
	TestCase_assert(value32 == 0x1, "Expected 0x1 but got 0x%X", value32);
	value32 = swapLittleEndian32(*(uint32_t *) "\x02\x03\x04\x05");
	TestCase_assert(value32 == 0x5040302, "Expected 0x5040302 but got 0x%X", value32);
	value32 = swapBigEndian32(*(uint32_t *) "\x01\x00\x00\x00");
	TestCase_assert(value32 == 0x1000000, "Expected 0x1000000 but got 0x%X", value32);
	value32 = swapBigEndian32(*(uint32_t *) "\x02\x03\x04\x05");
	TestCase_assert(value32 == 0x2030405, "Expected 0x2030405 but got 0x%X", value32);
	
	value64 = swapLittleEndian64(*(uint64_t *) "\x01\x00\x00\x00\x00\x00\x00\x00");
	TestCase_assert(value64 == 0x1, "Expected 0x1 but got 0x%llX", value64);
	value64 = swapLittleEndian64(*(uint64_t *) "\x02\x03\x04\x05\x06\x07\x08\x09");
	TestCase_assert(value64 == 0x908070605040302ull, "Expected 0x908070605040302 but got 0x%llX", value64);
	value64 = swapBigEndian64(*(uint64_t *) "\x01\x00\x00\x00\x00\x00\x00\x00");
	TestCase_assert(value64 == 0x100000000000000ull, "Expected 0x100000000000000 but got 0x%llX", value64);
	value64 = swapBigEndian64(*(uint64_t *) "\x02\x03\x04\x05\x06\x07\x08\x09");
	TestCase_assert(value64 == 0x203040506070809ull, "Expected 0x203040506070809 but got 0x%llX", value64);
}

TEST_SUITE(IOUtilitiesTest,
           testMemreadContextInit,
           testMemread,
           testMemwriteContextInit,
           testMemwrite,
           testReadFileSimple,
           testWriteFileSimple,
           testTemporaryFilePath,
           testEndianSwapping)
