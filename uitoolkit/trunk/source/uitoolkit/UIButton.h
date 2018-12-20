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

#include "uitoolkit/UIElement.h"
#include <math.h>

typedef void (* UIButtonActionCallback)(UIButton * sender, void * context);

#define UIButton_structContents(self_type) \
	UIElement_structContents(self_type) \
	\
	char * text; \
	float width; \
	enum UIElement_overflowMode overflowMode; \
	UIButtonActionCallback actionCallback; \
	void * actionCallbackContext; \
	\
	bool clickInProgress; \
	bool down;

stemobject_struct_definition(UIButton)

// text is copied
UIButton * UIButton_create(UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin, const char * text, float width, enum UIElement_overflowMode overflowMode, UIButtonActionCallback actionCallback, void * actionCallbackContext);
bool UIButton_init(UIButton * self, UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin, const char * text, float width, enum UIElement_overflowMode overflowMode, UIButtonActionCallback actionCallback, void * actionCallbackContext);
void UIButton_dispose(UIButton * self);

void UIButton_setText(UIButton * self, const char * text);
void UIButton_action(UIButton * self);

UIElement * UIButton_hitTest(UIButton * self, float x, float y);
bool UIButton_mouseDown(UIButton * self, unsigned int buttonNumber, float x, float y);
bool UIButton_mouseUp(UIButton * self, unsigned int buttonNumber, float x, float y);
bool UIButton_mouseDragged(UIButton * self, unsigned int buttonMask, float x, float y);

Rect4f UIButton_getBounds(UIButton * self);
void UIButton_getVertices(UIButton * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
