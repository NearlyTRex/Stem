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

#ifndef __UIButton_H__
#define __UIButton_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct UIButton UIButton;

#include "gamemath/Vector2f.h"
#include "uitoolkit/UIAppearance.h"
#include "uitoolkit/UIElement.h"
#include <math.h>

#define UIBUTTON_SIZE_TO_FIT_LABEL INFINITY

#define UIButton_structContents(self_type) \
	UIElement_structContents(self_type) \
	\
	char * label; \
	Vector2f position; \
	Vector2f relativeOrigin; \
	float width; \
	UIAppearance * appearance; \
	\
	bool clickInProgress; \
	bool down;

stemobject_struct_definition(UIButton)

// label is copied
UIButton * UIButton_create(const char * label, Vector2f position, Vector2f relativeOrigin, float width, UIAppearance * appearance);
bool UIButton_init(UIButton * self, const char * label, Vector2f position, Vector2f relativeOrigin, float width, UIAppearance * appearance);
void UIButton_dispose(UIButton * self);

bool UIButton_hitTest(UIButton * self, float x, float y);
bool UIButton_mouseDown(UIButton * self, unsigned int buttonNumber, float x, float y);
bool UIButton_mouseUp(UIButton * self, unsigned int buttonNumber, float x, float y);
bool UIButton_mouseDragged(UIButton * self, unsigned int buttonMask, float x, float y);
bool UIButton_actionDown(UIButton * self);

Rect4f UIButton_getBounds(UIButton * self);
void UIButton_getVertices(UIButton * self, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
