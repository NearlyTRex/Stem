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

typedef enum UIElementType {
	UIELEMENT_BUTTON,
	UIELEMENT_LABEL,
	UIELEMENT_CHECKBOX,
	UIELEMENT_RADIO_BUTTON,
	UIELEMENT_CONTAINER
} UIElementType;

enum UIElement_overflowMode {
	OVERFLOW_TRUNCATE, // Clip text where it's larger than the element's bounds
	OVERFLOW_SPILL, // Allow text to spill outside the element's bounds if it's larger, without actually changing this object's bounds
	OVERFLOW_RESIZE // Change this element's bounds to fit the measured size of the text
};

#include "gamemath/Rect4f.h"
#include "gamemath/Vector2f.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "stemobject/StemObject.h"
#include "uitoolkit/UIAppearance.h"

#define UIElement_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	UIElementType type; \
	Vector2f position; \
	Vector2f relativeOrigin; \
	UIAppearance * appearance; \
	\
	UIElement * (* hitTest)(self_type * self, float x, float y); \
	bool (* mouseDown)(self_type * self, unsigned int buttonNumber, float x, float y); \
	bool (* mouseUp)(self_type * self, unsigned int buttonNumber, float x, float y); \
	bool (* mouseMoved)(self_type * self, float x, float y); \
	bool (* mouseDragged)(self_type * self, unsigned int buttonMask, float x, float y); \
	bool (* scrollWheel)(self_type * self, int deltaX, int deltaY); \
	bool (* keyDown)(self_type * self, unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat); \
	bool (* keyUp)(self_type * self, unsigned int keyCode, unsigned int modifiers); \
	bool (* keyModifiersChanged)(self_type * self, unsigned int modifiers); \
	bool (* setFocusedElement)(self_type * self, UIElement * element); \
	UIElement * (* getFocusedElement)(self_type * self); \
	bool (* acceptsFocus)(self_type * self); \
	Rect4f (* getBounds)(self_type * self); \
	void (* getVertices)(self_type * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

stemobject_struct_definition(UIElement)

UIElement * UIElement_create(UIElementType type, UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin);
bool UIElement_init(UIElement * self, UIElementType type, UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin);
void UIElement_dispose(UIElement * self);

// Caller conventions: Before sending mouseDown to an element, caller should call hitTest and receive a positive return value.
// When hitTest returns true and mouseDown returns true, caller should remember which element returned true from mouseDown,
// and send mouseDragged and mouseUp events only to that element until mouseUp. Be cautious of different button numbers;
// remember that a new mouseDown on a different button number can start before a previous one has finished with a mouseUp.
// Send mouseMoved events to all elements.
// Receiver conventions: Return self from hitTest if the location is within your bounds, or if you represent multiple elements,
// the topmost one that contains the specified point. "Topmost" means the element lastest in the call order for getVertices, so
// if getVertices iterates forward, hitTest should iterate backward. Return true from mouseDown if you want to receive the
// followup mouseDragged and mouseUp events from that click, or if it changed your state in some way that necessitates a redraw.
// Return true from other mouse events only if a redraw is needed.
UIElement * UIElement_hitTest(UIElement * self, float x, float y);
bool UIElement_mouseDown(UIElement * self, unsigned int buttonNumber, float x, float y);
bool UIElement_mouseUp(UIElement * self, unsigned int buttonNumber, float x, float y);
bool UIElement_mouseMoved(UIElement * self, float x, float y);
bool UIElement_mouseDragged(UIElement * self, unsigned int buttonMask, float x, float y);

// Caller conventions: Send this event to any elements you expect to scroll, depending on desired scroll behavior.
// Receiver conventions: Assume you are the intended target of this event, even if hitTest would have failed with the current
// mouse position. Return true if it changed your state in a way that necessitates a redraw.
bool UIElement_scrollWheel(UIElement * self, int deltaX, int deltaY);

// Caller conventions: Caller should keep track of which element is in focus, and send keyDown events only to that element.
// All keyUp events should be sent only to the element that most recently received a keyDown and returned true from it.
// However, keyModifiersChanged events should go to *all* elements, regardless of focus and key up/down state.
// Receiver conventions: Return true from key events if the key event has changed your state in some way that requires a
// redraw, or if you want to receive a followup keyUp event from a keyDown.
bool UIElement_keyDown(UIElement * self, unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat);
bool UIElement_keyUp(UIElement * self, unsigned int keyCode, unsigned int modifiers);
bool UIElement_keyModifiersChanged(UIElement * self, unsigned int modifiers);

// Return true if the specified element was found and focus has been set to it. Clears focus if called with NULL.
// Non-container elements should return true if element == self and they currently can accept focus.
bool UIElement_setFocusedElement(UIElement * self, UIElement * element);
// Return the deepest element in the focus hierarchy, or NULL if no element is focused. Non-container elements that return
// true from acceptsFocus should return self from getFocusedElement.
UIElement * UIElement_getFocusedElement(UIElement * self);
// Return true if this element can accept focus; as in, whether a call to keyDown would be processed in some meaningful way.
bool UIElement_acceptsFocus(UIElement * self);

Rect4f UIElement_getBounds(UIElement * self);
void UIElement_getVertices(UIElement * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
