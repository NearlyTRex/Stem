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

#ifndef __UICheckbox_H__
#define __UICheckbox_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct UICheckbox UICheckbox;

#include "uitoolkit/UIElement.h"
#include <math.h>

typedef void (* UICheckboxActionCallback)(UICheckbox * sender, void * context, bool checked);

#define UICheckbox_structContents(self_type) \
	UIElement_structContents(self_type) \
	\
	char * text; \
	float width; \
	enum UIElement_overflowMode overflowMode; \
	UICheckboxActionCallback actionCallback; \
	void * actionCallbackContext; \
	\
	bool clickInProgress; \
	bool down; \
	bool checked;

stemobject_struct_definition(UICheckbox)

// text is copied
UICheckbox * UICheckbox_create(UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin, const char * text, float width, enum UIElement_overflowMode overflowMode, bool checked, UICheckboxActionCallback actionCallback, void * actionCallbackContext);
bool UICheckbox_init(UICheckbox * self, UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin, const char * text, float width, enum UIElement_overflowMode overflowMode, bool checked, UICheckboxActionCallback actionCallback, void * actionCallbackContext);
void UICheckbox_dispose(UICheckbox * self);

void UICheckbox_setText(UICheckbox * self, const char * text);
void UICheckbox_action(UICheckbox * self);

UIElement * UICheckbox_hitTest(UICheckbox * self, float x, float y);
bool UICheckbox_mouseDown(UICheckbox * self, unsigned int buttonNumber, float x, float y);
bool UICheckbox_mouseUp(UICheckbox * self, unsigned int buttonNumber, float x, float y);
bool UICheckbox_mouseDragged(UICheckbox * self, unsigned int buttonMask, float x, float y);

Rect4f UICheckbox_getBounds(UICheckbox * self);
void UICheckbox_getVertices(UICheckbox * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
