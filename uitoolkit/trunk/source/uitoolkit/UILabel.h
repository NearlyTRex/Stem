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

#ifndef __UILabel_H__
#define __UILabel_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct UILabel UILabel;

#include "glbitmapfont/TextFlow.h"
#include "uitoolkit/UIElement.h"

#define UILabel_structContents(self_type) \
	UIElement_structContents(self_type) \
	\
	TextFlow * textFlow; \
	Color4f textColor; \
	Vector2f size; \
	enum UIElement_overflowMode overflowModeX; \
	enum UIElement_overflowMode overflowModeY;

stemobject_struct_definition(UILabel)

// Initialization properties should generally be treated as immutable for this UILabel's lifetime. If text needs to be updated, use UILabel_setText().
// If wrapMode is not WORD_WRAP_NONE, size.x will determine the wrap width, so set it to a sane value even if you're using OVERFLOW_RESIZE for overflowModeX.
UILabel * UILabel_create(UIAppearance * appearance, Vector2f position, Vector2f size, Vector2f relativeOrigin, const char * text, Color4f textColor, enum TextFlow_wordWrapMode wrapMode, enum UIElement_overflowMode overflowModeX, enum UIElement_overflowMode overflowModeY);
bool UILabel_init(UILabel * self, UIAppearance * appearance, Vector2f position, Vector2f size, Vector2f relativeOrigin, const char * text, Color4f textColor, enum TextFlow_wordWrapMode wrapMode, enum UIElement_overflowMode overflowModeX, enum UIElement_overflowMode overflowModeY);
void UILabel_dispose(UILabel * self);

void UILabel_setText(UILabel * self, const char * text);
Rect4f UILabel_getBounds(UILabel * self);
void UILabel_getVertices(UILabel * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
