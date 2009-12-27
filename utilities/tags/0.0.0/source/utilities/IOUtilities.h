#ifndef __IO_UTILITIES_H__
#define __IO_UTILITIES_H__

#include <stdbool.h>
#include <stdlib.h>

struct memreadContext {
	const void * data;
	size_t length;
	size_t position;
};

struct memreadContext memreadContextInit(const void * data, size_t length);
bool memread(struct memreadContext * context, size_t nbytes, void * outData);

struct memwriteContext {
	void * data;
	size_t length;
	size_t allocatedSize;
	size_t position;
	bool realloc;
};

// If reallocIfNeeded is true, the memwriteContext struct takes ownership of the data pointer. You
// should no longer directly access the data pointer you passed to the function; instead, access the
// data field of the returned memwriteContext struct. However, if reallocIfNeeded is set to false,
// you retain ownership of the pointer, and you can continue accessing it as normal.
struct memwriteContext memwriteContextInit(void * data, size_t length, size_t allocatedSize, bool reallocIfNeeded);
bool memwrite(struct memwriteContext * context, size_t nbytes, const void * inData);

void * readFileSimple(const char * filePath, size_t * outFileLength);
bool writeFileSimple(const char * filePath, void * contents, size_t length);

const char * resourcePath(const char * fileName);

#endif
