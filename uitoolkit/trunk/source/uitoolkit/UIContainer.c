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

#include "uitoolkit/UIContainer.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS UIElement

UIContainer * UIContainer_create(Vector2f position, UIAppearance * appearance) {
	stemobject_create_implementation(UIContainer, init, position, appearance)
}

bool UIContainer_init(UIContainer * self, Vector2f position, UIAppearance * appearance) {
	unsigned int buttonIndex;
	
	call_super(init, self, UIELEMENT_CONTAINER, position, VECTOR2f_ZERO, appearance);
	self->dispose             = UIContainer_dispose;
	self->hitTest             = UIContainer_hitTest;
	self->mouseDown           = UIContainer_mouseDown;
	self->mouseUp             = UIContainer_mouseUp;
	self->mouseMoved          = UIContainer_mouseMoved;
	self->mouseDragged        = UIContainer_mouseDragged;
	self->scrollWheel         = UIContainer_scrollWheel;
	self->keyDown             = UIContainer_keyDown;
	self->keyUp               = UIContainer_keyUp;
	self->keyModifiersChanged = UIContainer_keyModifiersChanged;
	self->setFocusedElement   = UIContainer_setFocusedElement;
	self->getFocusedElement   = UIContainer_getFocusedElement;
	self->acceptsFocus        = UIContainer_acceptsFocus;
	self->getBounds           = UIContainer_getBounds;
	self->getVertices         = UIContainer_getVertices;
	self->elementCount = 0;
	self->private_ivar(elementAllocatedCount) = 8;
	self->elements = malloc(sizeof(*self->elements) * self->private_ivar(elementAllocatedCount));
	self->focusedElementIndex = UICONTAINER_FOCUS_NONE;
	for (buttonIndex = 0; buttonIndex < UICONTAINER_MOUSE_BUTTON_NUMBER_RESPONSE_COUNT; buttonIndex++) {
		self->lastMouseDownTargets[buttonIndex] = NULL;
	}
	self->lastKeyDownTarget = NULL;
	return true;
}

void UIContainer_dispose(UIContainer * self) {
	unsigned int elementIndex;
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex].owned) {
			self->elements[elementIndex].element->dispose(self->elements[elementIndex].element);
		}
	}
	free(self->elements);
	call_super(dispose, self);
}

void UIContainer_addElement(UIContainer * self, UIElement * element, bool takeOwnership) {
	if (self->private_ivar(elementAllocatedCount) <= self->elementCount) {
		self->private_ivar(elementAllocatedCount) *= 2;
		self->elements = realloc(self->elements, sizeof(*self->elements) * self->private_ivar(elementAllocatedCount));
	}
	self->elements[self->elementCount].element = element;
	self->elements[self->elementCount].owned = takeOwnership;
	++self->elementCount;
}

void UIContainer_removeElement(UIContainer * self, UIElement * element) {
	// TODO: Potentially dangerous if this happens during iteration! Add safeguards just in case
	unsigned int elementIndex;
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex].element == element) {
			if (self->elements[elementIndex].owned) {
				self->elements[elementIndex].element->dispose(self->elements[elementIndex].element);
			}
			--self->elementCount;
			for (; elementIndex < self->elementCount; elementIndex++) {
				self->elements[elementIndex] = self->elements[elementIndex + 1];
			}
			break;
		}
	}
}

void UIContainer_removeAllElements(UIContainer * self) {
	unsigned int elementIndex;
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex].owned) {
			self->elements[elementIndex].element->dispose(self->elements[elementIndex].element);
		}
	}
	self->elementCount = 0;
}

UIElement * UIContainer_hitTest(UIContainer * self, float x, float y) {
	unsigned int elementIndex;
	UIElement * result;
	
	x -= self->position.x;
	y -= self->position.y;
	for (elementIndex = self->elementCount - 1; elementIndex < self->elementCount; elementIndex--) {
		result = self->elements[elementIndex].element->hitTest(self->elements[elementIndex].element, x, y);
		if (result != NULL) {
			return result;
		}
	}
	return NULL;
}

bool UIContainer_mouseDown(UIContainer * self, unsigned int buttonNumber, float x, float y) {
	unsigned int elementIndex;
	
	if (buttonNumber >= UICONTAINER_MOUSE_BUTTON_NUMBER_RESPONSE_COUNT) {
		return false;
	}
	x -= self->position.x;
	y -= self->position.y;
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex].element->hitTest(self->elements[elementIndex].element, x, y) && self->elements[elementIndex].element->mouseDown(self->elements[elementIndex].element, buttonNumber, x, y)) {
			self->lastMouseDownTargets[buttonNumber] = self->elements[elementIndex].element;
			return true;
		}
	}
	return false;
}

bool UIContainer_mouseUp(UIContainer * self, unsigned int buttonNumber, float x, float y) {
	if (buttonNumber >= UICONTAINER_MOUSE_BUTTON_NUMBER_RESPONSE_COUNT || self->lastMouseDownTargets[buttonNumber] == NULL) {
		return false;
	}
	x -= self->position.x;
	y -= self->position.y;
	return self->lastMouseDownTargets[buttonNumber]->mouseUp(self->lastMouseDownTargets[buttonNumber], buttonNumber, x, y);
}

bool UIContainer_mouseMoved(UIContainer * self, float x, float y) {
	unsigned int elementIndex;
	bool handled = false;
	
	x -= self->position.x;
	y -= self->position.y;
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex].element->mouseMoved(self->elements[elementIndex].element, x, y)) {
			handled = true;
		}
	}
	return handled;
}

bool UIContainer_mouseDragged(UIContainer * self, unsigned int buttonMask, float x, float y) {
	unsigned int buttonIndex;
	bool handled = false;
	
	x -= self->position.x;
	y -= self->position.y;
	for (buttonIndex = 0; buttonIndex < UICONTAINER_MOUSE_BUTTON_NUMBER_RESPONSE_COUNT; buttonIndex++) {
		if ((buttonMask & 1 << buttonIndex) && self->lastMouseDownTargets[buttonIndex] != NULL) {
			if (self->lastMouseDownTargets[buttonIndex]->mouseDragged(self->lastMouseDownTargets[buttonIndex], buttonMask, x, y)) {
				handled = true;
			}
		}
	}
	return handled;
}

bool UIContainer_scrollWheel(UIContainer * self, int deltaX, int deltaY) {
	// TODO: ???
	return false;
}

bool UIContainer_keyDown(UIContainer * self, unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat) {
	if (self->focusedElementIndex != UICONTAINER_FOCUS_NONE) {
		self->lastKeyDownTarget = self->elements[self->focusedElementIndex].element;
		return self->elements[self->focusedElementIndex].element->keyDown(self->elements[self->focusedElementIndex].element, charCode, keyCode, modifiers, isRepeat);
	}
	return false;
}

bool UIContainer_keyUp(UIContainer * self, unsigned int keyCode, unsigned int modifiers) {
	bool handled = false;
	if (self->lastKeyDownTarget != NULL) {
		handled = self->lastKeyDownTarget->keyUp(self->lastKeyDownTarget, keyCode, modifiers);
		self->lastKeyDownTarget = NULL;
	}
	return handled;
}

bool UIContainer_keyModifiersChanged(UIContainer * self, unsigned int modifiers) {
	unsigned int elementIndex;
	bool handled = false;
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex].element->keyModifiersChanged(self->elements[elementIndex].element, modifiers)) {
			handled = true;
		}
	}
	return handled;
}

bool UIContainer_setFocusedElement(UIContainer * self, UIElement * element) {
	unsigned int elementIndex;
	
	if (element == NULL) {
		self->focusedElementIndex = UICONTAINER_FOCUS_NONE;
		return true;
	}
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex].element->setFocusedElement(self->elements[elementIndex].element, element)) {
			self->focusedElementIndex = elementIndex;
			return true;
		} 
	}
	return false;
}

UIElement * UIContainer_getFocusedElement(UIContainer * self) {
	if (self->focusedElementIndex == UICONTAINER_FOCUS_NONE || !self->elements[self->focusedElementIndex].element->acceptsFocus(self->elements[self->focusedElementIndex].element)) {
		return NULL;
	}
	return self->elements[self->focusedElementIndex].element->getFocusedElement(self->elements[self->focusedElementIndex].element);
}

bool UIContainer_acceptsFocus(UIContainer * self) {
	unsigned int elementIndex;
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex].element->acceptsFocus(self->elements[elementIndex].element)) {
			return true;
		}
	}
	return false;
}

Rect4f UIContainer_getBounds(UIContainer * self) {
	Rect4f result = RECT4f_EMPTY;
	unsigned int elementIndex;
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		result = Rect4f_union(result, self->elements[elementIndex].element->getBounds(self->elements[elementIndex].element));
	}
	return result;
}

void UIContainer_getVertices(UIContainer * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	unsigned int elementIndex;
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		self->elements[elementIndex].element->getVertices(self->elements[elementIndex].element, VECTOR2f(offset.x + self->position.x, offset.y + self->position.y), outVertices, outIndexes, ioVertexCount, ioIndexCount);
	}
}
