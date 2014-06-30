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

#include "jsonio/JSONEmitter.h"
#include "utilities/AutoFreePool.h"
#include "utilities/IOUtilities.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

bool JSONEmitter_writeFile(struct JSONNode * rootNode, enum JSONEmitterFormat format, const char * filePath, struct JSONEmissionError * outError) {
	char * string;
	size_t length;
	bool success;
	
	string = JSONEmitter_writeString(rootNode, format, &length, outError);
	if (string == NULL) {
		return false;
	}
	
	success = writeFileSimple(filePath, string, length);
	if (!success && outError != NULL) {
		char description[64];
		char * allocatedDescription;
		
		outError->node = rootNode;
		outError->code = JSONEmissionError_writeFileFailed;
		sprintf(description, "writeFileSimple failed with errno %d", errno);
		allocatedDescription = malloc(strlen(description) + 1);
		strcpy(allocatedDescription, description);
		AutoFreePool_add(allocatedDescription);
		outError->description = allocatedDescription;
	}
	free(string);
	return success;
}

struct nodeStackItem {
	struct JSONNode * node;
	size_t index;
};

char * JSONEmitter_writeString(struct JSONNode * rootNode, enum JSONEmitterFormat format, size_t * outLength, struct JSONEmissionError * outError) {
	struct memwriteContext context;
	size_t subitemIndex;
	struct JSONNode * containerNode;
	struct nodeStackItem * nodeStack;
	size_t nodeStackAllocatedSize, nodeStackCurrentDepth;
	size_t indentIndex;
	
	context = memwriteContextInit(malloc(1), 0, 1, true);
	
	if (rootNode->type == JSON_TYPE_ARRAY) {
		memwrite(&context, 1, "[");
		
	} else if (rootNode->type == JSON_TYPE_OBJECT) {
		memwrite(&context, 1, "{");
		
	} else {
		free(context.data);
		if (outError != NULL) {
			outError->node = rootNode;
			outError->code = JSONEmissionError_invalidNodeType;
			outError->description = "Invalid root node type";
		}
		return NULL;
	}
	
	nodeStackAllocatedSize = 1;
	nodeStack = malloc(sizeof(struct nodeStackItem) * nodeStackAllocatedSize);
	nodeStackCurrentDepth = 0;
	containerNode = rootNode;
	subitemIndex = 0;
	
	if (format == JSONEmitterFormat_multiLine) {
		memwrite(&context, 1, "\n");
	}
	
	for (;;) {
		if (subitemIndex >= containerNode->value.count) {
			if (format == JSONEmitterFormat_multiLine) {
				if (containerNode->value.count > 0) {
					memwrite(&context, 1, "\n");
				}
				for (indentIndex = 0; indentIndex < nodeStackCurrentDepth; indentIndex++) {
					memwrite(&context, 1, "\t");
				}
			}
			
			if (containerNode->type == JSON_TYPE_ARRAY) {
				memwrite(&context, 1, "]");
				
			} else if (containerNode->type == JSON_TYPE_OBJECT) {
				memwrite(&context, 1, "}");
			}
			
			if (nodeStackCurrentDepth == 0) {
				break;
			}
			
			nodeStackCurrentDepth--;
			containerNode = nodeStack[nodeStackCurrentDepth].node;
			subitemIndex = nodeStack[nodeStackCurrentDepth].index + 1;
			
			continue;
		}
		
		if (subitemIndex > 0) {
			memwrite(&context, 1, ",");
			if (format == JSONEmitterFormat_singleLine) {
				memwrite(&context, 1, " ");
			} else if (format == JSONEmitterFormat_multiLine) {
				memwrite(&context, 1, "\n");
			}
		}
		
		if (format == JSONEmitterFormat_multiLine) {
			for (indentIndex = 0; indentIndex <= nodeStackCurrentDepth; indentIndex++) {
				memwrite(&context, 1, "\t");
			}
		}
		
		if (containerNode->type == JSON_TYPE_OBJECT) {
			char * escapedString;
			size_t length;
			
			if (containerNode->subitems[subitemIndex].key == NULL) {
				free(context.data);
				if (outError != NULL) {
					outError->node = &containerNode->subitems[subitemIndex];
					outError->code = JSONEmissionError_nullKeyString;
					outError->description = "Null key string";
				}
				free(nodeStack);
				return NULL;
			}
			
			memwrite(&context, 1, "\"");
			escapedString = escapeJSONString(containerNode->subitems[subitemIndex].key, containerNode->subitems[subitemIndex].keyLength, &length);
			memwrite(&context, length, escapedString);
			free(escapedString);
			memwrite(&context, 2, "\":");
			if (format != JSONEmitterFormat_compact) {
				memwrite(&context, 1, " ");
			}
		}
		
		if (containerNode->subitems[subitemIndex].type == JSON_TYPE_NUMBER) {
			char formattedNumber[32];
			
			snprintf(formattedNumber, 32, "%.29g", containerNode->subitems[subitemIndex].value.number);
			memwrite(&context, strlen(formattedNumber), formattedNumber);
			
		} else if (containerNode->subitems[subitemIndex].type == JSON_TYPE_STRING) {
			char * escapedString;
			size_t length;
			
			if (containerNode->subitems[subitemIndex].value.string == NULL) {
				free(context.data);
				if (outError != NULL) {
					outError->node = &containerNode->subitems[subitemIndex];
					outError->code = JSONEmissionError_nullValueString;
					outError->description = "Null value string";
				}
				free(nodeStack);
				return NULL;
			}
			
			memwrite(&context, 1, "\"");
			escapedString = escapeJSONString(containerNode->subitems[subitemIndex].value.string, containerNode->subitems[subitemIndex].stringLength, &length);
			memwrite(&context, length, escapedString);
			free(escapedString);
			memwrite(&context, 1, "\"");
			
		} else if (containerNode->subitems[subitemIndex].type == JSON_TYPE_BOOLEAN) {
			if (containerNode->subitems[subitemIndex].value.boolean) {
				memwrite(&context, 4, "true");
			} else {
				memwrite(&context, 5, "false");
			}
			
		} else if (containerNode->subitems[subitemIndex].type == JSON_TYPE_NULL) {
			memwrite(&context, 4, "null");
			
		} else if (containerNode->subitems[subitemIndex].type == JSON_TYPE_ARRAY) {
			if (containerNode->subitems[subitemIndex].value.count > 0 && containerNode->subitems[subitemIndex].subitems == NULL) {
				free(context.data);
				if (outError != NULL) {
					outError->node = &containerNode->subitems[subitemIndex];
					outError->code = JSONEmissionError_nullSubitems;
					outError->description = "Null subitems with nonzero count";
				}
				free(nodeStack);
				return NULL;
			}
			
			if (nodeStackAllocatedSize <= nodeStackCurrentDepth) {
				nodeStackAllocatedSize *= 2;
				nodeStack = realloc(nodeStack, sizeof(struct nodeStackItem) * nodeStackAllocatedSize);
			}
			nodeStack[nodeStackCurrentDepth].node = containerNode;
			nodeStack[nodeStackCurrentDepth].index = subitemIndex;
			nodeStackCurrentDepth++;
			
			memwrite(&context, 1, "[");
			if (format == JSONEmitterFormat_multiLine) {
				memwrite(&context, 1, "\n");
			}
			containerNode = &containerNode->subitems[subitemIndex];
			subitemIndex = 0;
			continue;
			
		} else if (containerNode->subitems[subitemIndex].type == JSON_TYPE_OBJECT) {
			if (containerNode->subitems[subitemIndex].value.count > 0 && containerNode->subitems[subitemIndex].subitems == NULL) {
				free(context.data);
				if (outError != NULL) {
					outError->node = &containerNode->subitems[subitemIndex];
					outError->code = JSONEmissionError_nullSubitems;
					outError->description = "Null subitems with nonzero count";
				}
				free(nodeStack);
				return NULL;
			}
			
			if (nodeStackAllocatedSize <= nodeStackCurrentDepth) {
				nodeStackAllocatedSize *= 2;
				nodeStack = realloc(nodeStack, sizeof(struct nodeStackItem) * nodeStackAllocatedSize);
			}
			nodeStack[nodeStackCurrentDepth].node = containerNode;
			nodeStack[nodeStackCurrentDepth].index = subitemIndex;
			nodeStackCurrentDepth++;
			
			memwrite(&context, 1, "{");
			if (format == JSONEmitterFormat_multiLine) {
				memwrite(&context, 1, "\n");
			}
			containerNode = &containerNode->subitems[subitemIndex];
			subitemIndex = 0;
			continue;
			
		} else {
			free(context.data);
			if (outError != NULL) {
				outError->node = &containerNode->subitems[subitemIndex];
				outError->code = JSONEmissionError_invalidNodeType;
				outError->description = "Invalid node type";
			}
			free(nodeStack);
			return NULL;
		}
		
		subitemIndex++;
	}
	free(nodeStack);
	
	memwrite(&context, 1, NULL);
	if (outLength != NULL) {
		*outLength = context.length - 1;
	}
	return context.data;
}
