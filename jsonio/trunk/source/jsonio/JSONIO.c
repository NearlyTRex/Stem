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

#include "jsonio/JSONIO.h"
#include "utilities/UTFUtilities.h"
#include <stdio.h>
#include <string.h>

static void escapeJSONStringInternal(const char * string, size_t length, char * outString, size_t * outLength) {
	size_t charIndex, escapedCharIndex;
	
	escapedCharIndex = 0;
	for (charIndex = 0; charIndex < length; charIndex++) {
		if (string[charIndex] == '"') {
			if (outString != NULL) {
				outString[escapedCharIndex] = '\\';
				outString[escapedCharIndex + 1] = '"';
			}
			escapedCharIndex += 2;
			
		} else if (string[charIndex] == '\\') {
			if (outString != NULL) {
				outString[escapedCharIndex] = '\\';
				outString[escapedCharIndex + 1] = '\\';
			}
			escapedCharIndex += 2;
			
		} else if (string[charIndex] == '\b') {
			if (outString != NULL) {
				outString[escapedCharIndex] = '\\';
				outString[escapedCharIndex + 1] = 'b';
			}
			escapedCharIndex += 2;
			
		} else if (string[charIndex] == '\n') {
			if (outString != NULL) {
				outString[escapedCharIndex] = '\\';
				outString[escapedCharIndex + 1] = 'n';
			}
			escapedCharIndex += 2;
			
		} else if (string[charIndex] == '\f') {
			if (outString != NULL) {
				outString[escapedCharIndex] = '\\';
				outString[escapedCharIndex + 1] = 'f';
			}
			escapedCharIndex += 2;
			
		} else if (string[charIndex] == '\r') {
			if (outString != NULL) {
				outString[escapedCharIndex] = '\\';
				outString[escapedCharIndex + 1] = 'r';
			}
			escapedCharIndex += 2;
			
		} else if (string[charIndex] >= '\0' && string[charIndex] < ' ' && string[charIndex] != '\t') {
			if (outString != NULL) {
				char hex[5];
				
				outString[escapedCharIndex] = '\\';
				outString[escapedCharIndex + 1] = 'u';
				sprintf(hex, "%04X", (unsigned int) string[charIndex]);
				outString[escapedCharIndex + 2] = hex[0];
				outString[escapedCharIndex + 3] = hex[1];
				outString[escapedCharIndex + 4] = hex[2];
				outString[escapedCharIndex + 5] = hex[3];
			}
			escapedCharIndex += 6;
			
		} else {
			if (outString != NULL) {
				outString[escapedCharIndex] = string[charIndex];
			}
			escapedCharIndex++;
		}
	}
	if (outString != NULL) {
		outString[escapedCharIndex] = '\x00';
	}
	if (outLength != NULL) {
		*outLength = escapedCharIndex;
	}
}

char * escapeJSONString(const char * string, size_t length, size_t * outLength) {
	char * escapedString;
	size_t escapedLength;
	
	escapeJSONStringInternal(string, length, NULL, &escapedLength);
	escapedString = malloc(escapedLength + 1);
	escapeJSONStringInternal(string, length, escapedString, outLength);
	
	return escapedString;
}

static bool unescapeJSONStringInternal(const char * string, size_t length, char * outString, size_t * outLength) {
	size_t charIndex, charIndex2, unescapedCharIndex;
	bool escaped = false;
	uint16_t * utf16String;
	uint8_t * utf8String;
	unsigned int utf16Char;
	size_t utf16Length, utf8Length;
	char hex[5] = {0, 0, 0, 0, 0};
	
	unescapedCharIndex = 0;
	for (charIndex = 0; charIndex < length; charIndex++) {
		if (!escaped && string[charIndex] == '\\') {
			escaped = true;
			
		} else {
			if (escaped) {
				escaped = false;
				switch (string[charIndex]) {
					case 'b':
						if (outString != NULL) {
							outString[unescapedCharIndex] = '\b';
						}
						unescapedCharIndex++;
						break;
					case 'f':
						if (outString != NULL) {
							outString[unescapedCharIndex] = '\f';
						}
						unescapedCharIndex++;
						break;
					case 'n':
						if (outString != NULL) {
							outString[unescapedCharIndex] = '\n';
						}
						unescapedCharIndex++;
						break;
					case 'r':
						if (outString != NULL) {
							outString[unescapedCharIndex] = '\r';
						}
						unescapedCharIndex++;
						break;
					case 't':
						if (outString != NULL) {
							outString[unescapedCharIndex] = '\t';
						}
						unescapedCharIndex++;
						break;
						
					case 'u':
						charIndex2 = charIndex - 1;
						while (charIndex < length) {
							if (charIndex >= length - 4) {
								return false;
							}
							charIndex += 4;
							if (charIndex < length - 2 && string[charIndex + 1] == '\\' && string[charIndex + 2] == 'u') {
								charIndex += 2;
							} else {
								break;
							}
						}
						
						utf16String = malloc(sizeof(uint16_t) * (charIndex - charIndex2 + 1) / 6);
						utf16Length = 0;
						while (charIndex2 < charIndex) {
							charIndex2 += 2;
							hex[0] = string[charIndex2];
							hex[1] = string[charIndex2 + 1];
							hex[2] = string[charIndex2 + 2];
							hex[3] = string[charIndex2 + 3];
							if (!sscanf(hex, "%4x", &utf16Char)) {
								free(utf16String);
								return false;
							}
							charIndex2 += 4;
							utf16String[utf16Length++] = (uint16_t) utf16Char;
						}
						
						if (!utf16StringIsWellFormed(utf16String, utf16Length)) {
							free(utf16String);
							return false;
						}
						
						utf8String = utf16StringToUTF8String(utf16String, utf16Length);
						utf8Length = utf16StringUTF8Length(utf16String, utf16Length);
						free(utf16String);
						
						if (outString != NULL) {
							strncpy(outString + unescapedCharIndex, (char *) utf8String, utf8Length);
						}
						unescapedCharIndex += utf8Length;
						
						free(utf8String);
						break;
						
					default:
						if (outString != NULL) {
							outString[unescapedCharIndex] = string[charIndex];
						}
						unescapedCharIndex++;
						break;
				}
			} else {
				if (outString != NULL) {
					outString[unescapedCharIndex] = string[charIndex];
				}
				unescapedCharIndex++;
			}
		}
	}
	if (outString != NULL) {
		outString[unescapedCharIndex] = '\x00';
	}
	if (outLength != NULL) {
		*outLength = unescapedCharIndex;
	}
	
	return true;
}

char * unescapeJSONString(const char * string, size_t length, size_t * outLength) {
	char * unescapedString;
	size_t unescapedLength;
	
	if (!unescapeJSONStringInternal(string, length, NULL, &unescapedLength)) {
		return NULL;
	}
	unescapedString = malloc(unescapedLength + 1);
	unescapeJSONStringInternal(string, length, unescapedString, outLength);
	
	return unescapedString;
}

size_t JSONNode_subitemIndexForKey(struct JSONNode * objectNode, const char * key, size_t keyLength) {
	if (objectNode->type == JSON_TYPE_OBJECT) {
		size_t subitemIndex;
		
		for (subitemIndex = 0; subitemIndex < objectNode->value.count; subitemIndex++) {
			if (objectNode->subitems[subitemIndex].keyLength == keyLength && !memcmp(objectNode->subitems[subitemIndex].key, key, keyLength)) {
				return subitemIndex;
			}
		}
	}
	return JSON_SUBITEM_NOT_FOUND;
}

void JSONNode_dispose(struct JSONNode * node) {
	JSONNode_disposeContents(node);
	free(node);
}

void JSONNode_disposeContents(struct JSONNode * node) {
	free(node->key);
	
	if (node->type == JSON_TYPE_STRING) {
		free(node->value.string);
	}
	
	if (node->type == JSON_TYPE_ARRAY || node->type == JSON_TYPE_OBJECT) {
		size_t subitemIndex;
		
		for (subitemIndex = 0; subitemIndex < node->value.count; subitemIndex++) {
			JSONNode_disposeContents(&node->subitems[subitemIndex]);
		}
		free(node->subitems);
	}
}

struct JSONNode * JSONNode_copy(struct JSONNode * node) {
	struct JSONNode * outNode;
	
	outNode = malloc(sizeof(struct JSONNode));
	JSONNode_copyContents(node, outNode);
	return outNode;
}

void JSONNode_copyContents(struct JSONNode * node, struct JSONNode * outNode) {
	*outNode = *node;
	
	if (outNode->key != NULL) {
		outNode->key = malloc(node->keyLength + 1);
		memcpy(outNode->key, node->key, node->keyLength + 1);
	}
	
	if (outNode->type == JSON_TYPE_STRING) {
		outNode->value.string = malloc(node->stringLength + 1);
		memcpy(outNode->value.string, node->value.string, node->stringLength + 1);
		
	} else if (outNode->type == JSON_TYPE_ARRAY || outNode->type == JSON_TYPE_OBJECT) {
		size_t subitemIndex;
		
		outNode->subitems = malloc(sizeof(struct JSONNode) * node->value.count);
		for (subitemIndex = 0; subitemIndex < outNode->value.count; subitemIndex++) {
			JSONNode_copyContents(&node->subitems[subitemIndex], &outNode->subitems[subitemIndex]);
		}
	}
}
