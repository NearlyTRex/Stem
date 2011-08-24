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

#include "utilities/IOUtilities.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
		
		if (context->allocatedSize == 0) {
			context->allocatedSize = context->position + nbytes;
		} else {
			while (context->position + nbytes > context->allocatedSize) {
				context->allocatedSize *= 2;
			}
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
	size_t bytesRead;
	
	file = fopen(filePath, "rb");
	if (file == NULL) {
		if (outFileLength != NULL) {
			*outFileLength = 0;
		}
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	fseek(file, 0, SEEK_SET);
	fileContents = malloc(fileLength);
	bytesRead = fread(fileContents, 1, fileLength, file);
	fclose(file);
	
	if (bytesRead < fileLength) {
		free(fileContents);
		if (outFileLength != NULL) {
			*outFileLength = 0;
		}
		return NULL;
	}
	
	if (outFileLength != NULL) {
		*outFileLength = fileLength;
	}
	return fileContents;
}

bool writeFileSimple(const char * filePath, const void * contents, size_t length) {
	FILE * file;
	
	file = fopen(filePath, "wb");
	if (file == NULL) {
		return false;
	}
	fwrite(contents, 1, length, file);
	fclose(file);
	
	return true;
}

void * readStdinSimple(size_t * outLength) {
	int byte;
	size_t allocatedSize = 1;
	char * data;
	size_t length = 0;
	
	data = malloc(allocatedSize);
	for (;;) {
		byte = getchar();
		if (feof(stdin)) {
			break;
		}
		if (length >= allocatedSize) {
			allocatedSize *= 2;
			data = realloc(data, allocatedSize);
		}
		data[length++] = byte;
	}
	
	if (outLength != NULL) {
		*outLength = length;
	}
	return data;
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

const char * temporaryFilePath(const char * fileNameTemplate, int * outFD) {
	char * fileName;
	char * tempDirEnvironmentVariables[] = {
		"TMPDIR",
		"TEMPDIR",
		"TEMP",
		"TMP"
	};
	unsigned int variableIndex;
	char * tempDir = NULL;
	int fd;
	
	for (variableIndex = 0; variableIndex < sizeof(tempDirEnvironmentVariables) / sizeof(char *); variableIndex++) {
		tempDir = getenv(tempDirEnvironmentVariables[variableIndex]);
		if (tempDir != NULL) {
			break;
		}
	}
	
#ifndef WIN32
	if (tempDir == NULL) {
		tempDir = "/tmp";
	}
#endif
	
	if (tempDir == NULL) {
#ifdef DEBUG
		fprintf(stderr, "Warning: Couldn't find any of the expected temporary directory environment variables (%s", tempDirEnvironmentVariables[0]);
		for (variableIndex = 1; variableIndex < sizeof(tempDirEnvironmentVariables) / sizeof(char *); variableIndex++) {
			fprintf(stderr, ", %s", tempDirEnvironmentVariables[variableIndex]);
		}
		fprintf(stderr, "); unable to create a temporary file\n");
#endif
		return NULL;
	}
	
	fileName = malloc(strlen(tempDir) + strlen(fileNameTemplate) + 2);
	sprintf(fileName, "%s/%s", tempDir, fileNameTemplate);
	fd = mkstemp(fileName);
	
	if (fd == -1) {
#ifdef DEBUG
		fprintf(stderr, "Warning: Couldn't create temporary file \"%s\"; errno = %d\n", fileName, errno);
#endif
		return NULL;
	}
	
	if (outFD != NULL) {
		*outFD = fd;
	}
	return AutoFreePool_add(fileName);
}
