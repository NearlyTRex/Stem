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

#ifndef __JSON_PARSER_H__
#define __JSON_PARSER_H__

#include "jsonio/JSONIO.h"

enum JSONParseErrorCode {
	JSONParseError_rootNodeNotFound,
	JSONParseError_objectKeyNotFound,
	JSONParseError_malformedString,
	JSONParseError_keyNotFollowedByColon,
	JSONParseError_unexpectedEOF,
	JSONParseError_unexpectedToken,
	JSONParseError_fileNotFound
};

struct JSONParseError {
	size_t charIndex;
	enum JSONParseErrorCode code;
	const char * description;
};

/* Attempts to load the file at filePath as JSON, returning a pointer to the top-level node if successful. It is
   the caller's resposibility to free the returned node with JSONNode_dispose. If parsing fails, NULL is returned,
   and information about the parse error will be stored in outError if outError is not NULL. */
struct JSONNode * JSONParser_loadFile(const char * filePath, struct JSONParseError * outError);

/* Attempts to load string as JSON, returning a pointer to the top-level node if successful. It is the caller's
   resposibility to free the returned node with JSONNode_dispose. If parsing fails, NULL is returned, and
   information about the parse error will be stored in outError if outError is not NULL. */
struct JSONNode * JSONParser_loadString(const char * string, size_t length, struct JSONParseError * outError);

#endif
