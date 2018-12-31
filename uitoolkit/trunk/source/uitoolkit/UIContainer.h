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

#ifndef __UIContainer_H__
#define __UIContainer_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct UIContainer UIContainer;

#include "uitoolkit/UIElement.h"

// Generic hierarchical container; does no special layout. Subclasses might.

#define UICONTAINER_MOUSE_BUTTON_NUMBER_RESPONSE_COUNT 3
#define UICONTAINER_FOCUS_NONE ((unsigned int) -1)

struct UIContainer_element {
	UIElement * element;
	bool owned;
};

#define UIContainer_structContents(self_type) \
	UIElement_structContents(self_type) \
	\
	unsigned int elementCount; \
	unsigned int private_ivar(elementAllocatedCount); \
	struct UIContainer_element * elements; \
	unsigned int focusedElementIndex; \
	UIElement * lastMouseDownTargets[UICONTAINER_MOUSE_BUTTON_NUMBER_RESPONSE_COUNT]; \
	UIElement * lastKeyDownTarget;

stemobject_struct_definition(UIContainer)

UIContainer * UIContainer_create(Vector2f position, UIAppearance * appearance);
bool UIContainer_init(UIContainer * self, Vector2f position, UIAppearance * appearance);
void UIContainer_dispose(UIContainer * self);

void UIContainer_addElement(UIContainer * self, UIElement * element, bool takeOwnership);
void UIContainer_removeElement(UIContainer * self, UIElement * element);
void UIContainer_removeAllElements(UIContainer * self);

UIElement * UIContainer_hitTest(UIContainer * self, float x, float y);
bool UIContainer_mouseDown(UIContainer * self, unsigned int buttonNumber, float x, float y);
bool UIContainer_mouseUp(UIContainer * self, unsigned int buttonNumber, float x, float y);
bool UIContainer_mouseMoved(UIContainer * self, float x, float y);
bool UIContainer_mouseDragged(UIContainer * self, unsigned int buttonMask, float x, float y);
bool UIContainer_scrollWheel(UIContainer * self, int deltaX, int deltaY);
bool UIContainer_keyDown(UIContainer * self, unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat);
bool UIContainer_keyUp(UIContainer * self, unsigned int keyCode, unsigned int modifiers);
bool UIContainer_keyModifiersChanged(UIContainer * self, unsigned int modifiers);
bool UIContainer_menuAction(UIContainer * self);
bool UIContainer_menuCancel(UIContainer * self);
bool UIContainer_menuLeft(UIContainer * self);
bool UIContainer_menuRight(UIContainer * self);
bool UIContainer_menuUp(UIContainer * self);
bool UIContainer_menuDown(UIContainer * self);
bool UIContainer_menuNext(UIContainer * self);
bool UIContainer_menuPrevious(UIContainer * self);

bool UIContainer_setFocusedElement(UIContainer * self, UIElement * element);
UIElement * UIContainer_getFocusedElement(UIContainer * self);
bool UIContainer_acceptsFocus(UIContainer * self);

Rect4f UIContainer_getBounds(UIContainer * self);
void UIContainer_getVertices(UIContainer * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
