/*
  Copyright (c) 2018 Alex Diener
  
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

#ifndef __TextFlow_H__
#define __TextFlow_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct TextFlow TextFlow;

#include "glbitmapfont/GLBitmapFont.h"
#include "stemobject/StemObject.h"

enum TextFlow_wordWrapMode {
	WORD_WRAP_NONE, // No wrapping (though newline characters start a new line)
	WORD_WRAP_NORMAL, // Wrap on whitespace characters, but don't split words
	WORD_WRAP_AGGRESSIVE // Wrap on whitespace characters, and split words if necessary
};

struct TextFlow_wrapInfo {
	size_t * wrapPoints;
	size_t wrapPointCount;
	size_t wrapPointAllocatedCount;
	GLBitmapFont * font;
	const char * string;
	enum TextFlow_wordWrapMode wrapMode;
	float wrapWidth;
};

#define TextFlow_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	GLBitmapFont * font; \
	const char * string; \
	enum TextFlow_wordWrapMode wrapMode; \
	float wrapWidth; \
	struct TextFlow_wrapInfo private_ivar(wrapInfo);

stemobject_struct_definition(TextFlow)

// string is copied; wrapWidth is expressed in ems
TextFlow * TextFlow_create(GLBitmapFont * font, const char * string, enum TextFlow_wordWrapMode wrapMode, float wrapWidth);
bool TextFlow_init(TextFlow * self, GLBitmapFont * font, const char * string, enum TextFlow_wordWrapMode wrapMode, float wrapWidth);
void TextFlow_dispose(TextFlow * self);

// Returns number of lines used by the wrapped string, including both word wrapping and hard line breaks.
unsigned int TextFlow_getLineCount(TextFlow * self);

// Returns the width and height occupied by the wrapped string, measured in ems. The width may be greater than wrapWidth,
// even with WORD_WRAP_AGGRESSIVE, if wrapWidth is small enough that a single wide charater doesn't fit. If only height
// is needed, it can be retrieved more quickly by calling TextFlow_getLineCount().
Vector2f TextFlow_measureString(TextFlow * self);

// Returns the closest character index to position, as measured from the beginning of string (of length UTF-8 bytes).
// If outLeadingEdge is non-NULL, it will be set to true if position falls on the leading half of the
// character whose index is returned, or false if it does not.
// position is expressed in coordinates relative to this TextFlow's origin, as specified by relativeOrigin.
size_t TextFlow_indexAtPosition(TextFlow * self, float emSize, Vector2f position, Vector2f relativeOrigin, bool * outLeadingEdge);

// See GLBitmapFont.h for parameter documentation
void TextFlow_getVertices(TextFlow * self,
                          float emSize,
                          Vector2f offset,
                          Vector2f relativeOrigin,
                          bool pixelSnapping,
                          Rect4f clipBounds,
                          Color4f color,
                          struct vertex_p2f_t2f_c4f * outVertices,
                          GLuint * outIndexes,
                          unsigned int * ioVertexCount,
                          unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
