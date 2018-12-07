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

#ifndef __UIElement_H__
#define __UIElement_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct UIElement UIElement;

#include "gamemath/Rect4f.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "stemobject/StemObject.h"

#define UIElement_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	UIElement * parent; \
	\
	bool (* hitTest)(self_type * self, float x, float y); \
	bool (* mouseDown)(self_type * self, unsigned int buttonNumber, float x, float y); \
	bool (* mouseUp)(self_type * self, unsigned int buttonNumber, float x, float y); \
	bool (* mouseMoved)(self_type * self, float x, float y); \
	bool (* mouseDragged)(self_type * self, unsigned int buttonMask, float x, float y); \
	bool (* keyDown)(self_type * self, unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat); \
	bool (* keyUp)(self_type * self, unsigned int keyCode, unsigned int modifiers); \
	bool (* keyModifiersChanged)(self_type * self, unsigned int modifiers); \
	bool (* scrollWheel)(self_type * self, int deltaX, int deltaY); \
	bool (* actionDown)(self_type * self); \
	bool (* actionUp)(self_type * self); \
	\
	Rect4f (* getBounds)(self_type * self); \
	void (* getVertices)(self_type * self, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

stemobject_struct_definition(UIElement)

UIElement * UIElement_create();
bool UIElement_init(UIElement * self);
void UIElement_dispose(UIElement * self);

bool UIElement_hitTest(UIElement * self, float x, float y);
bool UIElement_mouseDown(UIElement * self, unsigned int buttonNumber, float x, float y);
bool UIElement_mouseUp(UIElement * self, unsigned int buttonNumber, float x, float y);
bool UIElement_mouseMoved(UIElement * self, float x, float y);
bool UIElement_mouseDragged(UIElement * self, unsigned int buttonMask, float x, float y);
bool UIElement_keyDown(UIElement * self, unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat);
bool UIElement_keyUp(UIElement * self, unsigned int keyCode, unsigned int modifiers);
bool UIElement_keyModifiersChanged(UIElement * self, unsigned int modifiers);
bool UIElement_scrollWheel(UIElement * self, int deltaX, int deltaY);
bool UIElement_actionDown(UIElement * self);
bool UIElement_actionUp(UIElement * self);

Rect4f UIElement_getBounds(UIElement * self);
void UIElement_getVertices(UIElement * self, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
