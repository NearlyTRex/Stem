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

UIContainer * UIContainer_create(UIAppearance * appearance, UIElement ** elements, unsigned int elementCount) {
	stemobject_create_implementation(UIContainer, init, appearance, elements, elementCount)
}

bool UIContainer_init(UIContainer * self, UIAppearance * appearance, UIElement ** elements, unsigned int elementCount) {
	unsigned int elementIndex, buttonIndex;
	
	call_super(init, self, UIELEMENT_CONTAINER, appearance, VECTOR2f_ZERO, VECTOR2f_ZERO);
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
	self->menuAction          = UIContainer_menuAction;
	self->menuCancel          = UIContainer_menuCancel;
	self->menuLeft            = UIContainer_menuLeft;
	self->menuRight           = UIContainer_menuRight;
	self->menuUp              = UIContainer_menuUp;
	self->menuDown            = UIContainer_menuDown;
	self->menuNext            = UIContainer_menuNext;
	self->menuPrevious        = UIContainer_menuPrevious;
	self->setFocusedElement   = UIContainer_setFocusedElement;
	self->getFocusedElement   = UIContainer_getFocusedElement;
	self->acceptsFocus        = UIContainer_acceptsFocus;
	self->getBounds           = UIContainer_getBounds;
	self->getVertices         = UIContainer_getVertices;
	self->elementCount = elementCount;
	self->elements = malloc(sizeof(*elements) * elementCount);
	memcpy(self->elements, elements, sizeof(*elements) * elementCount);
	self->focusedElementIndex = UICONTAINER_FOCUS_NONE;
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex]->acceptsFocus(self->elements[elementIndex])) {
			self->focusedElementIndex = elementIndex;
			break;
		}
	}
	for (buttonIndex = 0; buttonIndex < UICONTAINER_MOUSE_BUTTON_NUMBER_RESPONSE_COUNT; buttonIndex++) {
		self->lastMouseDownTargets[buttonIndex] = NULL;
	}
	self->lastKeyDownTarget = NULL;
	return true;
}

void UIContainer_dispose(UIContainer * self) {
	free(self->elements);
	call_super(dispose, self);
}

UIElement * UIContainer_hitTest(UIContainer * self, float x, float y) {
	unsigned int elementIndex;
	UIElement * result;
	
	for (elementIndex = self->elementCount - 1; elementIndex < self->elementCount; elementIndex--) {
		result = self->elements[elementIndex]->hitTest(self->elements[elementIndex], x, y);
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
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex]->hitTest(self->elements[elementIndex], x, y) && self->elements[elementIndex]->mouseDown(self->elements[elementIndex], buttonNumber, x, y)) {
			self->lastMouseDownTargets[buttonNumber] = self->elements[elementIndex];
			return true;
		}
	}
	return false;
}

bool UIContainer_mouseUp(UIContainer * self, unsigned int buttonNumber, float x, float y) {
	if (buttonNumber >= UICONTAINER_MOUSE_BUTTON_NUMBER_RESPONSE_COUNT || self->lastMouseDownTargets[buttonNumber] == NULL) {
		return false;
	}
	return self->lastMouseDownTargets[buttonNumber]->mouseUp(self->lastMouseDownTargets[buttonNumber], buttonNumber, x, y);
}

bool UIContainer_mouseMoved(UIContainer * self, float x, float y) {
	unsigned int elementIndex;
	bool handled = false;
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex]->mouseMoved(self->elements[elementIndex], x, y)) {
			handled = true;
		}
	}
	return handled;
}

bool UIContainer_mouseDragged(UIContainer * self, unsigned int buttonMask, float x, float y) {
	unsigned int buttonIndex;
	bool handled = false;
	
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
		self->lastKeyDownTarget = self->elements[self->focusedElementIndex];
		return self->elements[self->focusedElementIndex]->keyDown(self->elements[self->focusedElementIndex], charCode, keyCode, modifiers, isRepeat);
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
		if (self->elements[elementIndex]->keyModifiersChanged(self->elements[elementIndex], modifiers)) {
			handled = true;
		}
	}
	return handled;
}

bool UIContainer_menuAction(UIContainer * self) {
	if (self->focusedElementIndex != UICONTAINER_FOCUS_NONE) {
		return self->elements[self->focusedElementIndex]->menuAction(self->elements[self->focusedElementIndex]);
	}
	return false;
}

bool UIContainer_menuCancel(UIContainer * self) {
	if (self->focusedElementIndex != UICONTAINER_FOCUS_NONE) {
		return self->elements[self->focusedElementIndex]->menuCancel(self->elements[self->focusedElementIndex]);
	}
	return false;
}

bool UIContainer_menuLeft(UIContainer * self) {
	// If currently focused element can process menuLeft, do that first
	// Otherwise, search leftward (with what metrics?) for a focusable element, and change focus to it
	// Wrapping parameters? Tiebreaking? This is going to be messy
	return false;
}

bool UIContainer_menuRight(UIContainer * self) {
	return false;
}

bool UIContainer_menuUp(UIContainer * self) {
	return false;
}

bool UIContainer_menuDown(UIContainer * self) {
	return false;
}

bool UIContainer_menuNext(UIContainer * self) {
	unsigned int focusOffset, elementIndex, offsetElementIndex;
	
	if (self->focusedElementIndex == UICONTAINER_FOCUS_NONE) {
		focusOffset = 0;
	} else {
		focusOffset = (self->focusedElementIndex + 1) % self->elementCount;
	}
	for (elementIndex = 0; elementIndex < self->elementCount - 1; elementIndex++) {
		offsetElementIndex = (elementIndex + focusOffset) % self->elementCount;
		if (self->elements[offsetElementIndex]->acceptsFocus(self->elements[offsetElementIndex])) {
			// TODO: This seems incorrect; may need to propagate focus inward, because if the target is a container with nothing focused at the moment, the chain will stop there
			self->focusedElementIndex = offsetElementIndex;
			return true;
		}
	}
	return false;
}

bool UIContainer_menuPrevious(UIContainer * self) {
	unsigned int focusOffset, elementIndex, offsetElementIndex;
	
	if (self->focusedElementIndex == UICONTAINER_FOCUS_NONE) {
		focusOffset = 0;
	} else {
		focusOffset = (self->focusedElementIndex + 1) % self->elementCount;
	}
	for (elementIndex = 1; elementIndex < self->elementCount; elementIndex++) {
		offsetElementIndex = (self->elementCount - elementIndex + focusOffset) % self->elementCount;
		if (self->elements[offsetElementIndex]->acceptsFocus(self->elements[offsetElementIndex])) {
			self->focusedElementIndex = offsetElementIndex;
			return true;
		}
	}
	return false;
}

bool UIContainer_setFocusedElement(UIContainer * self, UIElement * element) {
	unsigned int elementIndex;
	
	if (element == NULL) {
		self->focusedElementIndex = UICONTAINER_FOCUS_NONE;
		return true;
	}
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex]->setFocusedElement(self->elements[elementIndex], element)) {
			self->focusedElementIndex = elementIndex;
			return true;
		} 
	}
	return false;
}

UIElement * UIContainer_getFocusedElement(UIContainer * self) {
	if (self->focusedElementIndex == UICONTAINER_FOCUS_NONE || !self->elements[self->focusedElementIndex]->acceptsFocus(self->elements[self->focusedElementIndex])) {
		return NULL;
	}
	return self->elements[self->focusedElementIndex]->getFocusedElement(self->elements[self->focusedElementIndex]);
}

bool UIContainer_acceptsFocus(UIContainer * self) {
	unsigned int elementIndex;
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		if (self->elements[elementIndex]->acceptsFocus(self->elements[elementIndex])) {
			return true;
		}
	}
	return false;
}

Rect4f UIContainer_getBounds(UIContainer * self) {
	Rect4f result = RECT4f_EMPTY;
	unsigned int elementIndex;
	
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		result = Rect4f_union(result, self->elements[elementIndex]->getBounds(self->elements[elementIndex]));
	}
	return result;
}

void UIContainer_getVertices(UIContainer * self, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	unsigned int elementIndex;
	
	// TODO: This container's position needs to somehow offset returned positions of children. How will that happen?
	for (elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
		self->elements[elementIndex]->getVertices(self->elements[elementIndex], outVertices, outIndexes, ioVertexCount, ioIndexCount);
	}
}
