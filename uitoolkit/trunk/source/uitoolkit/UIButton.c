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
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS UIElement

UIButton * UIButton_create(UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin, const char * text, float width, enum UIElement_overflowMode overflowMode, UIButtonActionCallback actionCallback, void * actionCallbackContext) {
	stemobject_create_implementation(UIButton, init, appearance, position, relativeOrigin, text, width, overflowMode, actionCallback, actionCallbackContext)
}

bool UIButton_init(UIButton * self, UIAppearance * appearance, Vector2f position, Vector2f relativeOrigin, const char * text, float width, enum UIElement_overflowMode overflowMode, UIButtonActionCallback actionCallback, void * actionCallbackContext) {
	call_super(init, self, UIELEMENT_BUTTON, appearance, position, relativeOrigin);
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
	unsigned int vertexCount = 0, indexCount = 0;
	Rect4f bounds = self->getBounds(self);
	float stringWidth;
	
	if (ioVertexCount != NULL) {
		vertexCount = *ioVertexCount;
	}
	if (ioIndexCount != NULL) {
		indexCount = *ioIndexCount;
	}
	if (outVertices != NULL) {
		struct TextureAtlas_entry atlasEntry = TextureAtlas_lookup(self->appearance->atlas, self->down ? UIAPPEARANCE_KEY_BUTTON_DOWN : UIAPPEARANCE_KEY_BUTTON_UP);
		struct vertex_p2f_t2f_c4f vertex;
		struct UIAppearance_sliceGrid slices = self->appearance->metrics.buttonSlices;
		float texCoordScaleX, texCoordScaleY;
		
		vertex.color[0] = vertex.color[1] = vertex.color[2] = vertex.color[3] = 1.0f;
		texCoordScaleX = (atlasEntry.right - atlasEntry.left) / (slices.leftColumn + slices.centerColumn + slices.rightColumn);
		texCoordScaleY = (atlasEntry.top - atlasEntry.bottom) / (slices.bottomRow + slices.centerRow + slices.topRow);
		
		writeVertex(vertexCount + 0, offset.x + bounds.left, offset.y + bounds.bottom, atlasEntry.left, atlasEntry.bottom);
		writeVertex(vertexCount + 1, offset.x + bounds.left + slices.leftColumn, offset.y + bounds.bottom, atlasEntry.left + slices.leftColumn * texCoordScaleX, atlasEntry.bottom);
		writeVertex(vertexCount + 2, offset.x + bounds.right - slices.rightColumn, offset.y + bounds.bottom, atlasEntry.right - slices.rightColumn * texCoordScaleX, atlasEntry.bottom);
		writeVertex(vertexCount + 3, offset.x + bounds.right, offset.y + bounds.bottom, atlasEntry.right, atlasEntry.bottom);
		
		writeVertex(vertexCount + 4, offset.x + bounds.left, offset.y + bounds.bottom + slices.bottomRow, atlasEntry.left, atlasEntry.bottom + slices.bottomRow * texCoordScaleY);
		writeVertex(vertexCount + 5, offset.x + bounds.left + slices.leftColumn, offset.y + bounds.bottom + slices.bottomRow, atlasEntry.left + slices.leftColumn * texCoordScaleX, atlasEntry.bottom + slices.bottomRow * texCoordScaleY);
		writeVertex(vertexCount + 6, offset.x + bounds.right - slices.rightColumn, offset.y + bounds.bottom + slices.bottomRow, atlasEntry.right - slices.rightColumn * texCoordScaleX, atlasEntry.bottom + slices.bottomRow * texCoordScaleY);
		writeVertex(vertexCount + 7, offset.x + bounds.right, offset.y + bounds.bottom + slices.bottomRow, atlasEntry.right, atlasEntry.bottom + slices.bottomRow * texCoordScaleY);
		
		writeVertex(vertexCount + 8, offset.x + bounds.left, offset.y + bounds.top - slices.topRow, atlasEntry.left, atlasEntry.top - slices.topRow * texCoordScaleY);
		writeVertex(vertexCount + 9, offset.x + bounds.left + slices.leftColumn, offset.y + bounds.top - slices.topRow, atlasEntry.left + slices.leftColumn * texCoordScaleX, atlasEntry.top - slices.topRow * texCoordScaleY);
		writeVertex(vertexCount + 10, offset.x + bounds.right - slices.rightColumn, offset.y + bounds.top - slices.topRow, atlasEntry.right - slices.rightColumn * texCoordScaleX, atlasEntry.top - slices.topRow * texCoordScaleY);
		writeVertex(vertexCount + 11, offset.x + bounds.right, offset.y + bounds.top - slices.topRow, atlasEntry.right, atlasEntry.top - slices.topRow * texCoordScaleY);
		
		writeVertex(vertexCount + 12, offset.x + bounds.left, offset.y + bounds.top, atlasEntry.left, atlasEntry.top);
		writeVertex(vertexCount + 13, offset.x + bounds.left + slices.leftColumn, offset.y + bounds.top, atlasEntry.left + slices.leftColumn * texCoordScaleX, atlasEntry.top);
		writeVertex(vertexCount + 14, offset.x + bounds.right - slices.rightColumn, offset.y + bounds.top, atlasEntry.right - slices.rightColumn * texCoordScaleX, atlasEntry.top);
		writeVertex(vertexCount + 15, offset.x + bounds.right, offset.y + bounds.top, atlasEntry.right, atlasEntry.top);
	}
	if (outIndexes != NULL) {
		unsigned int rowIndex, columnIndex;
		
		for (rowIndex = 0; rowIndex < 3; rowIndex++) {
			for (columnIndex = 0; columnIndex < 9; columnIndex++) {
				outIndexes[indexCount + (rowIndex * 3 + columnIndex) * 6 + 0] = vertexCount + rowIndex * 4 + columnIndex;
				outIndexes[indexCount + (rowIndex * 3 + columnIndex) * 6 + 1] = vertexCount + rowIndex * 4 + columnIndex + 1;
				outIndexes[indexCount + (rowIndex * 3 + columnIndex) * 6 + 2] = vertexCount + (rowIndex + 1) * 4 + columnIndex + 1;
				outIndexes[indexCount + (rowIndex * 3 + columnIndex) * 6 + 3] = vertexCount + (rowIndex + 1) * 4 + columnIndex + 1;
				outIndexes[indexCount + (rowIndex * 3 + columnIndex) * 6 + 4] = vertexCount + (rowIndex + 1) * 4 + columnIndex;
				outIndexes[indexCount + (rowIndex * 3 + columnIndex) * 6 + 5] = vertexCount + rowIndex * 4 + columnIndex;
			}
		}
	}
	if (ioVertexCount != NULL) {
		*ioVertexCount += 4 * 4;
	}
	if (ioIndexCount != NULL) {
		*ioIndexCount += 6 * 3 * 3;
	}
	
	stringWidth = GLBitmapFont_measureString(self->appearance->font, self->text, GLBITMAPFONT_USE_STRLEN) * self->appearance->metrics.fontHeight;
	GLBitmapFont_getStringVertices(self->appearance->font,
	                               self->text,
	                               GLBITMAPFONT_USE_STRLEN,
	                               self->appearance->metrics.fontHeight,
	                               VECTOR2f(roundf(offset.x + bounds.left + (bounds.right - bounds.left - stringWidth) * 0.5f), roundf(offset.y + bounds.bottom + (bounds.top - bounds.bottom - self->appearance->metrics.fontHeight) * 0.5f)),
	                               VECTOR2f(0.0f, 0.0f),
	                               true,
	                               self->overflowMode == OVERFLOW_TRUNCATE ? bounds : GLBITMAPFONT_NO_CLIP,
	                               self->appearance->metrics.buttonTextColor,
	                               outVertices,
	                               outIndexes,
	                               ioVertexCount,
	                               ioIndexCount);
}
