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

#include "uitoolkit/UIButton.h"
#include "uitoolkit/UIToolkitDrawing.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS UIElement

UIButton * UIButton_create(const char * text, Vector2f position, Vector2f relativeOrigin, float width, enum UIElement_overflowMode overflowMode, UIButtonActionCallback actionCallback, void * actionCallbackContext, UIAppearance * appearance) {
	stemobject_create_implementation(UIButton, init, text, position, relativeOrigin, width, overflowMode, actionCallback, actionCallbackContext, appearance)
}

bool UIButton_init(UIButton * self, const char * text, Vector2f position, Vector2f relativeOrigin, float width, enum UIElement_overflowMode overflowMode, UIButtonActionCallback actionCallback, void * actionCallbackContext, UIAppearance * appearance) {
	call_super(init, self, UIELEMENT_BUTTON, position, relativeOrigin, appearance);
	self->dispose = UIButton_dispose;
	self->hitTest = UIButton_hitTest;
	self->mouseDown = UIButton_mouseDown;
	self->mouseUp = UIButton_mouseUp;
	self->mouseDragged = UIButton_mouseDragged;
	self->getBounds = UIButton_getBounds;
	self->getVertices = UIButton_getVertices;
	
	self->text = strdup(text);
	self->actionCallback = actionCallback;
	self->actionCallbackContext = actionCallbackContext;
	self->overflowMode = overflowMode;
	if (overflowMode == OVERFLOW_RESIZE) {
		self->width = ceilf(GLBitmapFont_measureString(self->appearance->font, text, GLBITMAPFONT_USE_STRLEN) * self->appearance->metrics.fontHeight + self->appearance->metrics.buttonTextPadding * 2);
	} else {
		self->width = width;
	}
	self->clickInProgress = false;
	self->down = false;
	return true;
}

void UIButton_dispose(UIButton * self) {
	free(self->text);
	call_super(dispose, self);
}

void UIButton_setText(UIButton * self, const char * text) {
	free(self->text);
	self->text = strdup(text);
	if (self->overflowMode == OVERFLOW_RESIZE) {
		self->width = ceilf(GLBitmapFont_measureString(self->appearance->font, text, GLBITMAPFONT_USE_STRLEN) * self->appearance->metrics.fontHeight + self->appearance->metrics.buttonTextPadding * 2);
	}
}

void UIButton_action(UIButton * self) {
	if (self->actionCallback != NULL) {
		self->actionCallback(self, self->actionCallbackContext);
	}
}

UIElement * UIButton_hitTest(UIButton * self, float x, float y) {
	if (Rect4f_containsVector2f(self->getBounds(self), VECTOR2f(x, y))) {
		return (UIElement *) self;
	}
	return NULL;
}

bool UIButton_mouseDown(UIButton * self, unsigned int buttonNumber, float x, float y) {
	if (buttonNumber == 0) {
		self->clickInProgress = true;
		self->down = true;
		return true;
	}
	return false;
}

bool UIButton_mouseUp(UIButton * self, unsigned int buttonNumber, float x, float y) {
	if (self->clickInProgress && self->down) {
		self->clickInProgress = false;
		self->down = false;
		if (self->actionCallback != NULL) {
			self->actionCallback(self, self->actionCallbackContext);
		}
		return true;
	}
	self->clickInProgress = false;
	return false;
}

bool UIButton_mouseDragged(UIButton * self, unsigned int buttonMask, float x, float y) {
	if ((buttonMask & 0x1) && self->clickInProgress) {
		self->down = self->hitTest(self, x, y);
		return true;
	}
	return false;
}

Rect4f UIButton_getBounds(UIButton * self) {
	Rect4f result;
	float height = self->appearance->metrics.fontHeight + self->appearance->metrics.buttonTextPadding * 2;
	
	result.left = self->position.x - self->width * self->relativeOrigin.x;
	result.bottom = self->position.y - height * self->relativeOrigin.y;
	result.right = result.left + self->width;
	result.top = result.bottom + height;
	return result;
}

#define writeVertex(index, positionX, positionY, textureX, textureY) \
	vertex.position[0] = positionX; \
	vertex.position[1] = positionY; \
	vertex.texCoords[0] = textureX; \
	vertex.texCoords[1] = textureY; \
	outVertices[index] = vertex

void UIButton_getVertices(UIButton * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	Rect4f bounds = Rect4f_offset(self->getBounds(self), offset);
	float stringWidth;
	
	UIToolkit_getFrameVerticesWithSlices(bounds,
	                                     TextureAtlas_lookup(self->appearance->atlas, self->down ? UIAPPEARANCE_KEY_BUTTON_DOWN : UIAPPEARANCE_KEY_BUTTON_UP),
	                                     self->appearance->metrics.buttonSlices,
	                                     COLOR4f(1.0f, 1.0f, 1.0f, 1.0f),
	                                     outVertices,
	                                     outIndexes,
	                                     ioVertexCount,
	                                     ioIndexCount);
	
	stringWidth = GLBitmapFont_measureString(self->appearance->font, self->text, GLBITMAPFONT_USE_STRLEN) * self->appearance->metrics.fontHeight;
	GLBitmapFont_getStringVertices(self->appearance->font,
	                               self->text,
	                               GLBITMAPFONT_USE_STRLEN,
	                               self->appearance->metrics.fontHeight,
	                               VECTOR2f(roundf(bounds.left + (bounds.right - bounds.left - stringWidth) * 0.5f), roundf(bounds.bottom + (bounds.top - bounds.bottom - self->appearance->metrics.fontHeight) * 0.5f)),
	                               VECTOR2f(0.0f, 0.0f),
	                               true,
	                               self->overflowMode == OVERFLOW_TRUNCATE ? bounds : GLBITMAPFONT_NO_CLIP,
	                               self->appearance->metrics.buttonTextColor,
	                               outVertices,
	                               outIndexes,
	                               ioVertexCount,
	                               ioIndexCount);
}
