/*
  Copyright (c) 2010 Alex Diener
  
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
bool writeFileSimple(const char * filePath, const void * contents, size_t length);
void * readStdinSimple(size_t * outLength);

// fileNameTemplate is a mkstemp template. Return value is added to AutoFreePool, so it needn't be freed directly by the caller.
const char * temporaryFilePath(const char * fileNameTemplate, int * outFD);

#endif
