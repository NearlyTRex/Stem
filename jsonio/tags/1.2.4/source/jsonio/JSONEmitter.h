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

#ifndef __JSON_EMITTER_H__
#define __JSON_EMITTER_H__

#include "jsonio/JSONIO.h"

enum JSONEmitterFormat {
	// Omits all optional whitespace to output the fewest possible bytes
	JSONEmitterFormat_compact,
	
	// Includes spaces for readability, but outputs the entire document on one line
	JSONEmitterFormat_singleLine,
	
	// Includes spaces, line breaks, and indentation for readability
	JSONEmitterFormat_multiLine
};

enum JSONEmissionErrorCode {
	JSONEmissionError_invalidNodeType,
	JSONEmissionError_nullKeyString,
	JSONEmissionError_nullValueString,
	JSONEmissionError_nullSubitems,
	JSONEmissionError_writeFileFailed
};

struct JSONEmissionError {
	struct JSONNode * node;
	enum JSONEmissionErrorCode code;
	const char * description;
};

bool JSONEmitter_writeFile(struct JSONNode * rootNode, enum JSONEmitterFormat format, const char * filePath, struct JSONEmissionError * outError);
char * JSONEmitter_writeString(struct JSONNode * rootNode, enum JSONEmitterFormat format, size_t * outLength, struct JSONEmissionError * outError);

#endif
