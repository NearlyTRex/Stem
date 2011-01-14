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

#include "preferences/Preferences_private.h"
#include "jsonio/JSONEmitter.h"
#include "jsonio/JSONParser.h"
#include <stdio.h>
#include <string.h>

void Preferences_loadPrivate(compat_type(Preferences *) selfPtr) {
	Preferences * self = selfPtr;
	char filePath[PATH_MAX];
	struct JSONNode * jsonData;
	struct JSONParseError error;
	
	Preferences_getFilePathPrivate(self, filePath);
	jsonData = JSONParser_loadFile(filePath, &error);
	if (jsonData == NULL && error.code != JSONParseError_fileNotFound) {
		fprintf(stderr, "Error: JSONParser_loadFile: %s (%d, %u)\n", error.description, error.code, (unsigned int) error.charIndex);
		
	} else if (jsonData != NULL) {
		size_t valueIndex;
		size_t nodeIndex;
		
		for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
			nodeIndex = JSONNode_subitemIndexForKey(jsonData, self->values[valueIndex].name, strlen(self->values[valueIndex].name));
			
			if (nodeIndex != JSON_SUBITEM_NOT_FOUND) {
				switch (self->values[valueIndex].type) {
					case PREFERENCES_TYPE_INTEGER:
						if (jsonData->subitems[nodeIndex].type == JSON_TYPE_NUMBER) {
							Preferences_setIntegerPrivate(self, self->values[valueIndex].name, jsonData->subitems[nodeIndex].value.number);
						}
						break;
						
					case PREFERENCES_TYPE_FLOAT:
						if (jsonData->subitems[nodeIndex].type == JSON_TYPE_NUMBER) {
							Preferences_setFloatPrivate(self, self->values[valueIndex].name, jsonData->subitems[nodeIndex].value.number);
						}
						break;
						
					case PREFERENCES_TYPE_BOOLEAN:
						if (jsonData->subitems[nodeIndex].type == JSON_TYPE_BOOLEAN) {
							Preferences_setBooleanPrivate(self, self->values[valueIndex].name, jsonData->subitems[nodeIndex].value.boolean);
						}
						break;
						
					case PREFERENCES_TYPE_STRING:
						if (jsonData->subitems[nodeIndex].type == JSON_TYPE_STRING) {
							Preferences_setStringPrivate(self, self->values[valueIndex].name, jsonData->subitems[nodeIndex].value.string);
						}
						break;
						
					case PREFERENCES_TYPE_DATA:
						if (jsonData->subitems[nodeIndex].type == JSON_TYPE_STRING) {
							Preferences_setDataPrivate(self, self->values[valueIndex].name, jsonData->subitems[nodeIndex].value.string, jsonData->subitems[nodeIndex].stringLength);
						}
						break;
				}
			}
		}
	}
}

void Preferences_savePrivate(compat_type(Preferences *) selfPtr) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	struct JSONNode rootNode;
	char filePath[PATH_MAX];
	bool success;
	struct JSONEmissionError error;
	
	rootNode.type = JSON_TYPE_OBJECT;
	rootNode.key = NULL;
	rootNode.value.count = self->valueCount;
	rootNode.subitems = malloc(sizeof(struct JSONNode) * rootNode.value.count);
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		rootNode.subitems[valueIndex].keyLength = strlen(self->values[valueIndex].name);
		rootNode.subitems[valueIndex].key = malloc(rootNode.subitems[valueIndex].keyLength + 1);
		strcpy(rootNode.subitems[valueIndex].key, self->values[valueIndex].name);
		
		switch (self->values[valueIndex].type) {
			case PREFERENCES_TYPE_INTEGER:
				rootNode.subitems[valueIndex].type = JSON_TYPE_NUMBER;
				rootNode.subitems[valueIndex].value.number = self->values[valueIndex].value.integer;
				break;
				
			case PREFERENCES_TYPE_FLOAT:
				rootNode.subitems[valueIndex].type = JSON_TYPE_NUMBER;
				rootNode.subitems[valueIndex].value.number = self->values[valueIndex].value.number;
				break;
				
			case PREFERENCES_TYPE_BOOLEAN:
				rootNode.subitems[valueIndex].type = JSON_TYPE_BOOLEAN;
				rootNode.subitems[valueIndex].value.number = self->values[valueIndex].value.boolean;
				break;
				
			case PREFERENCES_TYPE_STRING:
				rootNode.subitems[valueIndex].type = JSON_TYPE_STRING;
				rootNode.subitems[valueIndex].stringLength = strlen(self->values[valueIndex].value.string);
				rootNode.subitems[valueIndex].value.string = malloc(rootNode.subitems[valueIndex].stringLength + 1);
				strcpy(rootNode.subitems[valueIndex].value.string, self->values[valueIndex].value.string);
				break;
				
			case PREFERENCES_TYPE_DATA:
				rootNode.subitems[valueIndex].type = JSON_TYPE_STRING;
				rootNode.subitems[valueIndex].stringLength = self->values[valueIndex].value.data.length;
				rootNode.subitems[valueIndex].value.string = malloc(rootNode.subitems[valueIndex].stringLength);
				memcpy(rootNode.subitems[valueIndex].value.string, self->values[valueIndex].value.data.bytes, self->values[valueIndex].value.data.length);
				break;
		}
	}
	
	Preferences_getFilePathPrivate(self, filePath);
	success = JSONEmitter_writeFile(&rootNode, JSONEmitterFormat_multiLine, filePath, &error);
	JSONNode_disposeContents(&rootNode);
	
	if (!success) {
		fprintf(stderr, "Warning: Saving preferences failed: %s (%d)\n", error.description, error.code);
	}
}
