/*
  Copyright (c) 2013 Alex Diener
  
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

#ifndef __JSON_IO_H__
#define __JSON_IO_H__

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum JSONType {
	JSON_TYPE_OBJECT = 0,
	JSON_TYPE_ARRAY = 1,
	JSON_TYPE_STRING = 2,
	JSON_TYPE_NUMBER = 3,
	JSON_TYPE_BOOLEAN = 4,
	JSON_TYPE_NULL = 5
};

#ifndef SIZE_T_MAX
#define SIZE_T_MAX ((size_t) SIZE_MAX)
#endif
#define JSON_SUBITEM_NOT_FOUND SIZE_T_MAX

struct JSONNode {
	// Defined for all nodes
	enum JSONType type;
	
	// Defined for all nodes that are subitems of an object node. NULL for all nodes that are not.
	char * key;
	
	// Defined if key is defined. All strings are null-terminated, but can contain \u0000, so the actual length may be longer than the length measured by strlen.
	size_t keyLength;
	
	// Defined for object and array nodes. The number of subitems is given by value.count. NULL for all other node types.
	struct JSONNode * subitems;
	
	// Defined for string nodes. All strings are null-terminated, but can contain \u0000, so the actual length may be longer than the length measured by strlen.
	size_t stringLength;
	
	union {
		// Defined for object and array nodes
		size_t count;
		
		// Defined for string nodes
		char * string;
		
		// Defined for number nodes
		double number;
		
		// Defined for boolean nodes
		bool boolean;
	} value;
};

/* Caller is responsible for freeing return value. */
char * escapeJSONString(const char * string, size_t length, size_t * outLength);

/* Returns NULL if string contains invalid escapes. Caller is responsible for freeing return value. */
char * unescapeJSONString(const char * string, size_t length, size_t * outLength);

// Returns JSON_SUBITEM_NOT_FOUND if no subitem with the specified key exists in objectNode, or if objectNode is not of JSON_TYPE_OBJECT
size_t JSONNode_subitemIndexForKey(struct JSONNode * objectNode, const char * key, size_t keyLength);

/* Frees node, its contents, and all subitems. */
void JSONNode_dispose(struct JSONNode * node);

/* Frees node's contents without freeing node itself */
void JSONNode_disposeContents(struct JSONNode * node);

/* Performs a deep copy of node. */
struct JSONNode * JSONNode_copy(struct JSONNode * node);

/* Performs a deep copy of node's contents, writing them to outNode. */
void JSONNode_copyContents(struct JSONNode * node, struct JSONNode * outNode);

#endif
