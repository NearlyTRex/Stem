#include "utilities/IOUtilities.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "shell/Shell.h"
#include "utilities/AutoFreePool.h"

struct memreadContext memreadContextInit(const void * data, size_t length) {
	struct memreadContext context;
	
	context.data = data;
	context.length = length;
	context.position = 0;
	
	return context;
}

bool memread(struct memreadContext * context, size_t nbytes, void * outData) {
	if (context->position + nbytes > context->length) {
		return false;
	}
	
	if (outData != NULL) {
		memcpy(outData, context->data + context->position, nbytes);
	}
	
	context->position += nbytes;
	
	return true;
}

struct memwriteContext memwriteContextInit(void * data, size_t length, size_t allocatedSize, bool reallocIfNeeded) {
	struct memwriteContext context;
	
	context.data = data;
	context.length = length;
	context.allocatedSize = allocatedSize;
	context.position = 0;
	context.realloc = reallocIfNeeded;
	
	return context;
}

bool memwrite(struct memwriteContext * context, size_t nbytes, const void * inData) {
	if (context->position + nbytes > context->allocatedSize) {
		if (!context->realloc) {
			return false;
		}
		
		while (context->position + nbytes > context->allocatedSize) {
			context->allocatedSize *= 2;
		}
		context->data = realloc(context->data, context->allocatedSize);
	}
	
	if (inData == NULL) {
		memset(context->data + context->position, 0, nbytes);
	} else {
		memcpy(context->data + context->position, inData, nbytes);
	}
	
	context->position += nbytes;
	if (context->length < context->position) {
		context->length = context->position;
	}
	
	return true;
}

void * readFileSimple(const char * filePath, size_t * outFileLength) {
	FILE * file;
	size_t fileLength;
	void * fileContents;
	
	file = fopen(filePath, "rb");
	if (file == NULL) {
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	fseek(file, 0, SEEK_SET);
	fileContents = malloc(fileLength);
	fread(fileContents, 1, fileLength, file);
	fclose(file);
	
	if (outFileLength != NULL) *outFileLength = fileLength;
	return fileContents;
}

bool writeFileSimple(const char * filePath, void * contents, size_t length) {
	FILE * file;
	
	file = fopen(filePath, "wb");
	if (file == NULL) {
		return false;
	}
	fwrite(contents, 1, length, file);
	fclose(file);
	
	return true;
}

const char * resourcePath(const char * filePath) {
	static char * path;
	
	path = malloc(PATH_MAX);
	snprintf(path, PATH_MAX, "%s/%s", Shell_getResourcePath(), filePath);
	AutoFreePool_add(path);
	
	return path;
}
