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

#include "uitoolkit/UIRadioButton.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS UIElement

UIRadioButton * UIRadioButton_create(UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin, const char * text, float width, enum UIElement_overflowMode overflowMode, bool checked, UIRadioGroup * radioGroup, UIRadioButtonActionCallback actionCallback, void * actionCallbackContext) {
	stemobject_create_implementation(UIRadioButton, init, appearance, position, relativeOrigin, text, width, overflowMode, checked, radioGroup, actionCallback, actionCallbackContext)
}

bool UIRadioButton_init(UIRadioButton * self, UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin, const char * text, float width, enum UIElement_overflowMode overflowMode, bool checked, UIRadioGroup * radioGroup, UIRadioButtonActionCallback actionCallback, void * actionCallbackContext) {
	call_super(init, self, UIELEMENT_RADIO_BUTTON, appearance, position, relativeOrigin);
	self->dispose = UIRadioButton_dispose;
	self->hitTest = UIRadioButton_hitTest;
	self->mouseDown = UIRadioButton_mouseDown;
	self->mouseUp = UIRadioButton_mouseUp;
	self->mouseDragged = UIRadioButton_mouseDragged;
	self->getBounds = UIRadioButton_getBounds;
	self->getVertices = UIRadioButton_getVertices;
	
	self->text = strdup(text);
	self->actionCallback = actionCallback;
	self->actionCallbackContext = actionCallbackContext;
	self->overflowMode = overflowMode;
	if (overflowMode == OVERFLOW_RESIZE) {
		self->width = self->appearance->metrics.radioButtonGraphicWidth + self->appearance->metrics.radioButtonTextPadding + ceilf(GLBitmapFont_measureString(self->appearance->font, text, GLBITMAPFONT_USE_STRLEN) * self->appearance->metrics.fontHeight);
	} else {
		self->width = width;
	}
	self->clickInProgress = false;
	self->down = false;
	self->checked = checked;
	self->radioGroup = radioGroup;
	if (self->checked) {
		UIRadioGroup_setCheckedButton(self->radioGroup, self);
	}
	return true;
}

void UIRadioButton_dispose(UIRadioButton * self) {
	free(self->text);
	call_super(dispose, self);
}

void UIRadioButton_setText(UIRadioButton * self, const char * text) {
	free(self->text);
	self->text = strdup(text);
	if (self->overflowMode == OVERFLOW_RESIZE) {
		self->width = self->appearance->metrics.radioButtonGraphicWidth + self->appearance->metrics.radioButtonTextPadding + ceilf(GLBitmapFont_measureString(self->appearance->font, text, GLBITMAPFONT_USE_STRLEN) * self->appearance->metrics.fontHeight);
	}
}

void UIRadioButton_action(UIRadioButton * self) {
	if (!self->checked) {
		self->checked = true;
		UIRadioGroup_setCheckedButton(self->radioGroup, self);
		if (self->actionCallback != NULL) {
			self->actionCallback(self, self->actionCallbackContext);
		}
	}
}

void UIRadioButton_uncheck(UIRadioButton * self) {
	self->checked = false;
}

UIElement * UIRadioButton_hitTest(UIRadioButton * self, float x, float y) {
	if (Rect4f_containsVector2f(self->getBounds(self), VECTOR2f(x, y))) {
		return (UIElement *) self;
	}
	return NULL;
}

bool UIRadioButton_mouseDown(UIRadioButton * self, unsigned int buttonNumber, float x, float y) {
	if (buttonNumber == 0) {
		self->clickInProgress = true;
		self->down = true;
		return true;
	}
	return false;
}

bool UIRadioButton_mouseUp(UIRadioButton * self, unsigned int buttonNumber, float x, float y) {
	if (self->clickInProgress && self->down) {
		self->clickInProgress = false;
		self->down = false;
		if (!self->checked) {
			self->checked = true;
			UIRadioGroup_setCheckedButton(self->radioGroup, self);
			if (self->actionCallback != NULL) {
				self->actionCallback(self, self->actionCallbackContext);
			}
		}
		return true;
	}
	self->clickInProgress = false;
	return false;
}

bool UIRadioButton_mouseDragged(UIRadioButton * self, unsigned int buttonMask, float x, float y) {
	if ((buttonMask & 0x1) && self->clickInProgress) {
		self->down = self->hitTest(self, x, y);
		return true;
	}
	return false;
}

Rect4f UIRadioButton_getBounds(UIRadioButton * self) {
	Rect4f result;
	float height = self->appearance->metrics.radioButtonGraphicHeight;
	
	if (self->appearance->metrics.fontHeight > height) {
		height = self->appearance->metrics.fontHeight;
	}
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

void UIRadioButton_getVertices(UIRadioButton * self, Vector2f offset, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	unsigned int vertexCount = 0, indexCount = 0;
	Rect4f bounds = self->getBounds(self);
	
	if (ioVertexCount != NULL) {
		vertexCount = *ioVertexCount;
	}
	if (ioIndexCount != NULL) {
		indexCount = *ioIndexCount;
	}
	if (outVertices != NULL) {
		struct TextureAtlas_entry atlasEntry = TextureAtlas_lookup(self->appearance->atlas, self->down ? self->checked ? UIAPPEARANCE_KEY_RADIO_DOWN_CHECKED : UIAPPEARANCE_KEY_RADIO_DOWN : self->checked ? UIAPPEARANCE_KEY_RADIO_UP_CHECKED : UIAPPEARANCE_KEY_RADIO_UP);
		struct vertex_p2f_t2f_c4f vertex;
		
		vertex.color[0] = vertex.color[1] = vertex.color[2] = vertex.color[3] = 1.0f;
		
		writeVertex(vertexCount + 0, offset.x + bounds.left, offset.y + roundf(bounds.bottom + (bounds.top - bounds.bottom - self->appearance->metrics.radioButtonGraphicHeight) * 0.5f), atlasEntry.left, atlasEntry.bottom);
		writeVertex(vertexCount + 1, offset.x + bounds.left + self->appearance->metrics.radioButtonGraphicWidth, offset.y + roundf(bounds.bottom + (bounds.top - bounds.bottom - self->appearance->metrics.radioButtonGraphicHeight) * 0.5f), atlasEntry.right, atlasEntry.bottom);
		writeVertex(vertexCount + 2, offset.x + bounds.left + self->appearance->metrics.radioButtonGraphicWidth, offset.y + roundf(bounds.bottom + (bounds.top - bounds.bottom - self->appearance->metrics.radioButtonGraphicHeight) * 0.5f) + self->appearance->metrics.radioButtonGraphicHeight, atlasEntry.right, atlasEntry.top);
		writeVertex(vertexCount + 3, offset.x + bounds.left, offset.y + roundf(bounds.bottom + (bounds.top - bounds.bottom - self->appearance->metrics.radioButtonGraphicHeight) * 0.5f) + self->appearance->metrics.radioButtonGraphicHeight, atlasEntry.left, atlasEntry.top);
	}
	if (outIndexes != NULL) {
		outIndexes[indexCount + 0] = vertexCount + 0;
		outIndexes[indexCount + 1] = vertexCount + 1;
		outIndexes[indexCount + 2] = vertexCount + 2;
		outIndexes[indexCount + 3] = vertexCount + 2;
		outIndexes[indexCount + 4] = vertexCount + 3;
		outIndexes[indexCount + 5] = vertexCount + 0;
	}
	if (ioVertexCount != NULL) {
		*ioVertexCount += 4;
	}
	if (ioIndexCount != NULL) {
		*ioIndexCount += 6;
	}
	
	GLBitmapFont_getStringVertices(self->appearance->font,
	                               self->text,
	                               GLBITMAPFONT_USE_STRLEN,
	                               self->appearance->metrics.fontHeight,
	                               VECTOR2f(roundf(offset.x + bounds.left + self->appearance->metrics.radioButtonGraphicWidth + self->appearance->metrics.radioButtonTextPadding), roundf(offset.y + bounds.bottom + (bounds.top - bounds.bottom) * 0.5f)),
	                               VECTOR2f(0.0f, 0.5f),
	                               true,
	                               self->overflowMode == OVERFLOW_TRUNCATE ? bounds : GLBITMAPFONT_NO_CLIP,
	                               self->appearance->metrics.radioButtonTextColor,
	                               outVertices,
	                               outIndexes,
	                               ioVertexCount,
	                               ioIndexCount);
}
