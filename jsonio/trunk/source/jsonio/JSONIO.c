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

size_t escapeJSONString(const char * unescapedString, char * outEscapedString, size_t bufferSize) {
	return 0;
}

size_t unescapeJSONString(const char * escapedString, char * outUnescapedString, size_t bufferSize) {
	return 0;
}

struct JSONNode * JSONNode_subitemForKey(struct JSONNode * objectNode, const char * key) {
	return NULL;
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
		unsigned int subitemIndex;
		
		for (subitemIndex = 0; subitemIndex < node->value.count; subitemIndex++) {
			JSONNode_disposeContents(&node->subitems[subitemIndex]);
		}
		free(node->subitems);
	}
}
