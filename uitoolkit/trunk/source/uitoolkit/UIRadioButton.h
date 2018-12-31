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

#ifndef __UIRadioButton_H__
#define __UIRadioButton_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct UIRadioButton UIRadioButton;

#include "uitoolkit/UIElement.h"
#include "uitoolkit/UIRadioGroup.h"
#include <math.h>

typedef void (* UIRadioButtonActionCallback)(UIRadioButton * sender, void * context);

#define UIRadioButton_structContents(self_type) \
	UIElement_structContents(self_type) \
	\
	char * text; \
	float width; \
	enum UIElement_overflowMode overflowMode; \
	UIRadioButtonActionCallback actionCallback; \
	void * actionCallbackContext; \
	\
	bool clickInProgress; \
	bool down; \
	bool checked; \
	UIRadioGroup * radioGroup;

stemobject_struct_definition(UIRadioButton)

// text is copied
UIRadioButton * UIRadioButton_create(const char * text, Vector2f position, Vector2f relativeOrigin, float width, enum UIElement_overflowMode overflowMode, bool checked, UIRadioGroup * radioGroup, UIRadioButtonActionCallback actionCallback, void * actionCallbackContext, UIAppearance * appearance);
bool UIRadioButton_init(UIRadioButton * self, const char * text, Vector2f position, Vector2f relativeOrigin, float width, enum UIElement_overflowMode overflowMode, bool checked, UIRadioGroup * radioGroup, UIRadioButtonActionCallback actionCallback, void * actionCallbackContext, UIAppearance * appearance);
void UIRadioButton_dispose(UIRadioButton * self);

void UIRadioButton_setText(UIRadioButton * self, const char * text);
void UIRadioButton_action(UIRadioButton * self);
void UIRadioButton_uncheck(UIRadioButton * self);

UIElement * UIRadioButton_hitTest(UIRadioButton * self, float x, float y);
bool UIRadioButton_mouseDown(UIRadioButton * self, unsigned int buttonNumber, float x, float y);
bool UIRadioButton_mouseUp(UIRadioButton * self, unsigned int buttonNumber, float x, float y);
bool UIRadioButton_mouseDragged(UIRadioButton * self, unsigned int buttonMask, float x, float y);

Rect4f UIRadioButton_getBounds(UIRadioButton * self);
void UIRadioButton_getVertices(UIRadioButton * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
