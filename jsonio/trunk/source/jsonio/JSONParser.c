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

#include "jsonio/JSONParser.h"
#include "utilities/IOUtilities.h"
#include "utilities/UTFUtilities.h"
#include <stdio.h>
#include <string.h>

struct JSONNode * JSONParser_loadFile(const char * filePath) {
	char * fileContents;
	size_t fileLength = 0;
	struct JSONNode * node;
	
	fileContents = readFileSimple(filePath, &fileLength);
	node = JSONParser_loadString(fileContents, fileLength);
	free(fileContents);
	return node;
}

enum NumberToken {
	NUMBER_TOKEN_INVALID,
	NUMBER_TOKEN_PLUS,
	NUMBER_TOKEN_MINUS,
	NUMBER_TOKEN_DIGITS,
	NUMBER_TOKEN_DECIMAL,
	NUMBER_TOKEN_EXPONENT
};

static enum NumberToken nextNumberToken(const char * string, size_t length, size_t * ioCharIndex, size_t * outTokenLength) {
	size_t charIndex;
	
	charIndex = *ioCharIndex;
	
	if (string[charIndex] == '-') {
		*outTokenLength = 1;
		*ioCharIndex = charIndex + 1;
		return NUMBER_TOKEN_MINUS;
		
	} else if (string[charIndex] == '+') {
		*outTokenLength = 1;
		*ioCharIndex = charIndex + 1;
		return NUMBER_TOKEN_PLUS;
		
	} else if (string[charIndex] == '.') {
		*outTokenLength = 1;
		*ioCharIndex = charIndex + 1;
		return NUMBER_TOKEN_DECIMAL;
		
	} else if (string[charIndex] == 'e' || string[charIndex] == 'E') {
		*outTokenLength = 1;
		*ioCharIndex = charIndex + 1;
		return NUMBER_TOKEN_EXPONENT;
		
	} else if (string[charIndex] >= '0' && string[charIndex] <= '9') {
		size_t charIndex2;
		
		for (charIndex2 = charIndex + 1; charIndex2 < length; charIndex2++) {
			if (string[charIndex2] < '0' || string[charIndex2] > '9') {
				break;
			}
		}
		*outTokenLength = charIndex2 - charIndex;
		*ioCharIndex = charIndex2;
		return NUMBER_TOKEN_DIGITS;
	}
	
	*outTokenLength = 0;
	return NUMBER_TOKEN_INVALID;
}

enum JSONToken {
	JSON_TOKEN_INVALID,
	JSON_TOKEN_BEGIN_ARRAY,
	JSON_TOKEN_BEGIN_OBJECT,
	JSON_TOKEN_END_ARRAY,
	JSON_TOKEN_END_OBJECT,
	JSON_TOKEN_COLON,
	JSON_TOKEN_COMMA,
	JSON_TOKEN_NULL,
	JSON_TOKEN_TRUE,
	JSON_TOKEN_FALSE,
	JSON_TOKEN_NUMBER,
	JSON_TOKEN_STRING
};

static enum JSONToken nextJSONToken(const char * string, size_t length, size_t * ioCharIndex, size_t * outTokenLength) {
	size_t charIndex;
	
	charIndex = *ioCharIndex;
	while (charIndex < length && (string[charIndex] == ' ' || string[charIndex] == '\t' || string[charIndex] == '\r' || string[charIndex] == '\n')) {
		charIndex++;
	}
	*ioCharIndex = charIndex;
	
	if (charIndex >= length) {
		*outTokenLength = 0;
		return JSON_TOKEN_INVALID;
	}
	
	if (string[charIndex] == '[') {
		*outTokenLength = 1;
		*ioCharIndex = charIndex + 1;
		return JSON_TOKEN_BEGIN_ARRAY;
		
	} else if (string[charIndex] == '{') {
		*outTokenLength = 1;
		*ioCharIndex = charIndex + 1;
		return JSON_TOKEN_BEGIN_OBJECT;
		
	} else if (string[charIndex] == ']') {
		*outTokenLength = 1;
		*ioCharIndex = charIndex + 1;
		return JSON_TOKEN_END_ARRAY;
		
	} else if (string[charIndex] == '}') {
		*outTokenLength = 1;
		*ioCharIndex = charIndex + 1;
		return JSON_TOKEN_END_OBJECT;
		
	} else if (string[charIndex] == ':') {
		*outTokenLength = 1;
		*ioCharIndex = charIndex + 1;
		return JSON_TOKEN_COLON;
		
	} else if (string[charIndex] == ',') {
		*outTokenLength = 1;
		*ioCharIndex = charIndex + 1;
		return JSON_TOKEN_COMMA;
		
	} else if (string[charIndex] == 'n') {
		if (charIndex + 4 <= length && !strncmp(string + charIndex, "null", 4)) {
			*outTokenLength = 4;
			*ioCharIndex = charIndex + 4;
			return JSON_TOKEN_NULL;
			
		} else {
			*outTokenLength = 0;
			return JSON_TOKEN_INVALID;
		}
		
	} else if (string[charIndex] == 't') {
		if (charIndex + 4 <= length && !strncmp(string + charIndex, "true", 4)) {
			*outTokenLength = 4;
			*ioCharIndex = charIndex + 4;
			return JSON_TOKEN_TRUE;
			
		} else {
			*outTokenLength = 0;
			return JSON_TOKEN_INVALID;
		}
		
	} else if (string[charIndex] == 'f') {
		if (charIndex + 5 <= length && !strncmp(string + charIndex, "false", 5)) {
			*outTokenLength = 5;
			*ioCharIndex = charIndex + 5;
			return JSON_TOKEN_FALSE;
			
		} else {
			*outTokenLength = 0;
			return JSON_TOKEN_INVALID;
		}
		
	} else if (string[charIndex] == '"') {
		bool escaped = false;
		size_t charIndex2;
		
		for (charIndex2 = charIndex + 1; charIndex2 < length; charIndex2++) {
			if (string[charIndex2] < ' ' && string[charIndex2] != '\t') {
				break;
			}
			
			if (escaped) {
				escaped = false;
				
			} else {
				if (string[charIndex2] == '\\') {
					escaped = true;
					
				} else if (string[charIndex2] == '"') {
					*outTokenLength = charIndex2 - charIndex + 1;
					*ioCharIndex = charIndex2 + 1;
					return JSON_TOKEN_STRING;
				}
			}
		}
		
	} else if (string[charIndex] == '-' || (string[charIndex] >= '0' && string[charIndex] <= '9')) {
		enum NumberToken token;
		size_t charIndex2, tokenLength;
		
		charIndex2 = charIndex;
		
		token = nextNumberToken(string, length, &charIndex2, &tokenLength);
		if (token == NUMBER_TOKEN_MINUS) {
			token = nextNumberToken(string, length, &charIndex2, &tokenLength);
		}
		
		if (token == NUMBER_TOKEN_DIGITS && (tokenLength == 1 || string[charIndex2 - tokenLength] != '0')) {
			token = nextNumberToken(string, length, &charIndex2, &tokenLength);
			if (token == NUMBER_TOKEN_INVALID) {
				*outTokenLength = charIndex2 - charIndex;
				*ioCharIndex = charIndex2;
				return JSON_TOKEN_NUMBER;
			}
			
			if (token == NUMBER_TOKEN_DECIMAL) {
				token = nextNumberToken(string, length, &charIndex2, &tokenLength);
				if (token == NUMBER_TOKEN_DIGITS) {
					token = nextNumberToken(string, length, &charIndex2, &tokenLength);
					if (token == NUMBER_TOKEN_INVALID) {
						*outTokenLength = charIndex2 - charIndex;
						*ioCharIndex = charIndex2;
						return JSON_TOKEN_NUMBER;
					}
					
					if (token == NUMBER_TOKEN_EXPONENT) {
						token = nextNumberToken(string, length, &charIndex2, &tokenLength);
						if (token == NUMBER_TOKEN_PLUS || token == NUMBER_TOKEN_MINUS) {
							token = nextNumberToken(string, length, &charIndex2, &tokenLength);
						}
						
						if (token == NUMBER_TOKEN_DIGITS) {
							token = nextNumberToken(string, length, &charIndex2, &tokenLength);
							if (token == NUMBER_TOKEN_INVALID) {
								*outTokenLength = charIndex2 - charIndex;
								*ioCharIndex = charIndex2;
								return JSON_TOKEN_NUMBER;
							}
						}
					}
				}
				
			} else if (token == NUMBER_TOKEN_EXPONENT) {
				token = nextNumberToken(string, length, &charIndex2, &tokenLength);
				if (token == NUMBER_TOKEN_PLUS || token == NUMBER_TOKEN_MINUS) {
					token = nextNumberToken(string, length, &charIndex2, &tokenLength);
				}
				
				if (token == NUMBER_TOKEN_DIGITS) {
					token = nextNumberToken(string, length, &charIndex2, &tokenLength);
					if (token == NUMBER_TOKEN_INVALID) {
						*outTokenLength = charIndex2 - charIndex;
						*ioCharIndex = charIndex2;
						return JSON_TOKEN_NUMBER;
					}
				}
			}
		}
	}
	
	*outTokenLength = 0;
	return JSON_TOKEN_INVALID;
}

static bool unescapeStringInternal(const char * string, size_t length, char * outString, size_t * outLength) {
	size_t charIndex, charIndex2, unescapedCharIndex;
	bool escaped = false;
	uint16_t * utf16String;
	uint8_t * utf8String;
	unsigned int utf16Char;
	size_t utf16Length, utf8Length;
	
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
							if (!sscanf(string + charIndex2, "%4x", &utf16Char)) {
								free(utf16String);
								return false;
							}
							charIndex2 += 4;
							utf16String[utf16Length++] = utf16Char;
						}
						
						if (!utf16StringIsWellFormed(utf16String, utf16Length)) {
							free(utf16String);
							return false;
						}
						
						utf8String = utf16StringToUTF8String(utf16String, utf16Length);
						free(utf16String);
						
						utf8Length = utf8StringLength(utf8String);
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

static char * unescapeString(const char * string, size_t length, size_t * outLength) {
	char * unescapedString;
	size_t unescapedLength;
	
	if (!unescapeStringInternal(string, length, NULL, &unescapedLength)) {
		return NULL;
	}
	unescapedString = malloc(unescapedLength + 1);
	unescapeStringInternal(string, length, unescapedString, outLength);
	
	return unescapedString;
}

struct JSONNode * JSONParser_loadString(const char * string, size_t length) {
	struct JSONNode * rootNode, * containerNode, node;
	struct JSONNode nodePrototype = {JSON_TYPE_NULL, NULL, 0, NULL, 0, {.string = NULL}};
	size_t charIndex = 0, tokenLength;
	enum JSONToken token;
	struct JSONNode ** nodeStack;
	size_t nodeStackAllocatedSize, nodeStackCurrentDepth;
	
	rootNode = malloc(sizeof(struct JSONNode));
	*rootNode = nodePrototype;
	
	token = nextJSONToken(string, length, &charIndex, &tokenLength);
	if (token == JSON_TOKEN_BEGIN_ARRAY) {
		rootNode->type = JSON_TYPE_ARRAY;
		
	} else if (token == JSON_TOKEN_BEGIN_OBJECT) {
		rootNode->type = JSON_TYPE_OBJECT;
		
	} else {
		JSONNode_dispose(rootNode);
		return NULL;
	}
	
	nodeStackAllocatedSize = 1;
	nodeStack = malloc(sizeof(struct JSONNode *) * nodeStackAllocatedSize);
	nodeStackCurrentDepth = 0;
	containerNode = rootNode;
	
	for (;;) {
		token = nextJSONToken(string, length, &charIndex, &tokenLength);
		if ((token == JSON_TOKEN_END_ARRAY && containerNode->type == JSON_TYPE_ARRAY) ||
		    (token == JSON_TOKEN_END_OBJECT && containerNode->type == JSON_TYPE_OBJECT)) {
			if (nodeStackCurrentDepth == 0) {
				break;
			}
			
			nodeStackCurrentDepth--;
			containerNode = nodeStack[nodeStackCurrentDepth];
			
			continue;
		}
		
		if (token == JSON_TOKEN_COMMA && containerNode->value.count > 0) {
			token = nextJSONToken(string, length, &charIndex, &tokenLength);
		}
		
		node = nodePrototype;
		
		if (containerNode->type == JSON_TYPE_OBJECT) {
			if (token != JSON_TOKEN_STRING) {
				JSONNode_dispose(rootNode);
				free(nodeStack);
				return NULL;
			}
			
			node.key = unescapeString(string + charIndex - tokenLength + 1, tokenLength - 2, &node.keyLength);
			if (node.key == NULL) {
				JSONNode_dispose(rootNode);
				free(nodeStack);
				return NULL;
			}
			
			token = nextJSONToken(string, length, &charIndex, &tokenLength);
			if (token != JSON_TOKEN_COLON) {
				JSONNode_disposeContents(&node);
				JSONNode_dispose(rootNode);
				free(nodeStack);
				return NULL;
			}
			token = nextJSONToken(string, length, &charIndex, &tokenLength);
		}
		
		if (token == JSON_TOKEN_NULL) {
			node.type = JSON_TYPE_NULL;
			containerNode->value.count++;
			containerNode->subitems = realloc(containerNode->subitems, sizeof(struct JSONNode) * containerNode->value.count);
			containerNode->subitems[containerNode->value.count - 1] = node;
			
		} else if (token == JSON_TOKEN_FALSE) {
			node.type = JSON_TYPE_BOOLEAN;
			node.value.boolean = false;
			containerNode->value.count++;
			containerNode->subitems = realloc(containerNode->subitems, sizeof(struct JSONNode) * containerNode->value.count);
			containerNode->subitems[containerNode->value.count - 1] = node;
			
		} else if (token == JSON_TOKEN_TRUE) {
			node.type = JSON_TYPE_BOOLEAN;
			node.value.boolean = true;
			containerNode->value.count++;
			containerNode->subitems = realloc(containerNode->subitems, sizeof(struct JSONNode) * containerNode->value.count);
			containerNode->subitems[containerNode->value.count - 1] = node;
			
		} else if (token == JSON_TOKEN_NUMBER) {
			char * endPtr;
			
			if (charIndex >= length) {
				// strtod is potentially dangerous, so make sure it isn't called if we're at the end of a potentially-not-null-terminated string (which would be invalid json anyway)
				JSONNode_disposeContents(&node);
				JSONNode_dispose(rootNode);
				free(nodeStack);
				return NULL;
			}
			node.type = JSON_TYPE_NUMBER;
			node.value.number = strtod(string + charIndex - tokenLength, &endPtr);
			if (endPtr != string + charIndex) {
				JSONNode_disposeContents(&node);
				JSONNode_dispose(rootNode);
				free(nodeStack);
				return NULL;
			}
			containerNode->value.count++;
			containerNode->subitems = realloc(containerNode->subitems, sizeof(struct JSONNode) * containerNode->value.count);
			containerNode->subitems[containerNode->value.count - 1] = node;
			
		} else if (token == JSON_TOKEN_STRING) {
			node.type = JSON_TYPE_STRING;
			node.value.string = unescapeString(string + charIndex - tokenLength + 1, tokenLength - 2, &node.stringLength);
			if (node.value.string == NULL) {
				JSONNode_disposeContents(&node);
				JSONNode_dispose(rootNode);
				free(nodeStack);
				return NULL;
			}
			containerNode->value.count++;
			containerNode->subitems = realloc(containerNode->subitems, sizeof(struct JSONNode) * containerNode->value.count);
			containerNode->subitems[containerNode->value.count - 1] = node;
			
		} else if (token == JSON_TOKEN_BEGIN_ARRAY || token == JSON_TOKEN_BEGIN_OBJECT) {
			node.type = token == JSON_TOKEN_BEGIN_ARRAY ? JSON_TYPE_ARRAY : JSON_TYPE_OBJECT;
			if (nodeStackAllocatedSize <= nodeStackCurrentDepth) {
				nodeStackAllocatedSize *= 2;
				nodeStack = realloc(nodeStack, sizeof(struct JSONNode *) * nodeStackAllocatedSize);
			}
			nodeStack[nodeStackCurrentDepth++] = containerNode;
			containerNode->value.count++;
			containerNode->subitems = realloc(containerNode->subitems, sizeof(struct JSONNode) * containerNode->value.count);
			containerNode->subitems[containerNode->value.count - 1] = node;
			
			containerNode = &containerNode->subitems[containerNode->value.count - 1];
			
		} else {
			JSONNode_disposeContents(&node);
			JSONNode_dispose(rootNode);
			free(nodeStack);
			return NULL;
		}
	}
	free(nodeStack);
	
	while (charIndex < length && (string[charIndex] == ' ' || string[charIndex] == '\t' || string[charIndex] == '\r' || string[charIndex] == '\n')) {
		charIndex++;
	}
	if (charIndex != length) {
		JSONNode_dispose(rootNode);
		return NULL;
	}
	
	return rootNode;
}
