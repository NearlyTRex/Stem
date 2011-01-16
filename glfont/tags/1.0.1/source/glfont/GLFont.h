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

#ifndef __GL_FONT_H__
#define __GL_FONT_H__

#include "stemobject/StemObject.h"
#include <stdlib.h>

typedef struct GLFont GLFont;

#define GLFont_structContents \
	StemObject_structContents \
	\
	/* Returns the width of string (of length UTF-8 bytes) in ems */ \
	float (* measureString)(void * self, const char * string, size_t length); \
	\
	/* Returns the closest character index to emWidth, as measured from the beginning of string (of length UTF-8 bytes). */ \
	/* If outLeadingEdge is non-NULL, it will be set to true if emWidth falls on the leading half of the */ \
	/* character whose index is returned, or false if it does not. */ \
	size_t (* indexAtWidth)(void * self, const char * string, size_t length, float emWidth, bool * outLeadingEdge); \
	\
	/* Draws string (of length UTF-8 bytes) at offsetX, offsetY, offsetZ, using the current projection-modelview transform. */ \
	/* Text is scaled according such that characters' heights are equal to emHeight. */ \
	void (* drawString)(void * self, const char * string, size_t length, float emHeight, float offsetX, float offsetY, float offsetZ);

struct GLFont {
	GLFont_structContents
};

void GLFont_init(compat_type(GLFont *) selfPtr);
void GLFont_dispose(compat_type(GLFont *) selfPtr);

#endif
