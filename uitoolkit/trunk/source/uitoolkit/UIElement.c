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

#include "uitoolkit/UIElement.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

UIElement * UIElement_create(UIElementType type, UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin) {
	stemobject_create_implementation(UIElement, init, type, appearance, position, relativeOrigin)
}

bool UIElement_init(UIElement * self, UIElementType type, UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin) {
	call_super(init, self);
	self->type = type;
	self->appearance = appearance;
	self->position = position;
	self->relativeOrigin = relativeOrigin;
	self->dispose             = UIElement_dispose;
	self->hitTest             = UIElement_hitTest;
	self->mouseDown           = UIElement_mouseDown;
	self->mouseUp             = UIElement_mouseUp;
	self->mouseMoved          = UIElement_mouseMoved;
	self->mouseDragged        = UIElement_mouseDragged;
	self->scrollWheel         = UIElement_scrollWheel;
	self->keyDown             = UIElement_keyDown;
	self->keyUp               = UIElement_keyUp;
	self->keyModifiersChanged = UIElement_keyModifiersChanged;
	self->setFocusedElement   = UIElement_setFocusedElement;
	self->getFocusedElement   = UIElement_getFocusedElement;
	self->acceptsFocus        = UIElement_acceptsFocus;
	self->getBounds           = UIElement_getBounds;
	self->getVertices         = UIElement_getVertices;
	return true;
}

void UIElement_dispose(UIElement * self) {
	call_super(dispose, self);
}

UIElement * UIElement_hitTest(UIElement * self, float x, float y) {
	return NULL;
}

bool UIElement_mouseDown(UIElement * self, unsigned int buttonNumber, float x, float y) {
	return false;
}

bool UIElement_mouseUp(UIElement * self, unsigned int buttonNumber, float x, float y) {
	return false;
}

bool UIElement_mouseMoved(UIElement * self, float x, float y) {
	return false;
}

bool UIElement_mouseDragged(UIElement * self, unsigned int buttonMask, float x, float y) {
	return false;
}

bool UIElement_scrollWheel(UIElement * self, int deltaX, int deltaY) {
	return false;
}

bool UIElement_keyDown(UIElement * self, unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat) {
	return false;
}

bool UIElement_keyUp(UIElement * self, unsigned int keyCode, unsigned int modifiers) {
	return false;
}

bool UIElement_keyModifiersChanged(UIElement * self, unsigned int modifiers) {
	return false;
}

bool UIElement_setFocusedElement(UIElement * self, UIElement * element) {
	return false;
}

UIElement * UIElement_getFocusedElement(UIElement * self) {
	return NULL;
}

bool UIElement_acceptsFocus(UIElement * self) {
	return false;
}

Rect4f UIElement_getBounds(UIElement * self) {
	return RECT4f_EMPTY;
}

void UIElement_getVertices(UIElement * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
}
