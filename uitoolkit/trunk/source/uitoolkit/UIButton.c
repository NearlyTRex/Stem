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

UIButton * UIButton_create(const char * label, Vector2f position, Vector2f relativeOrigin, float width, UIAppearance * appearance) {
	stemobject_create_implementation(UIButton, init, label, position, relativeOrigin, width, appearance)
}

bool UIButton_init(UIButton * self, const char * label, Vector2f position, Vector2f relativeOrigin, float width, UIAppearance * appearance) {
	call_super(init, self);
	self->dispose = UIButton_dispose;
	self->hitTest = UIButton_hitTest;
	self->mouseDown = UIButton_mouseDown;
	self->mouseUp = UIButton_mouseUp;
	self->mouseDragged = UIButton_mouseDragged;
	self->actionDown = UIButton_actionDown;
	self->getBounds = UIButton_getBounds;
	self->getVertices = UIButton_getVertices;
	
	self->label = strdup(label);
	self->position = position;
	self->relativeOrigin = relativeOrigin;
	self->appearance = appearance;
	if (width == UIBUTTON_SIZE_TO_FIT_LABEL) {
		self->width = GLBitmapFont_measureString(self->appearance->font, label, GLBITMAPFONT_USE_STRLEN) * self->appearance->metrics.buttonLabelHeight + self->appearance->metrics.buttonLabelPadding * 2;
	} else {
		self->width = width;
	}
	self->clickInProgress = false;
	self->down = false;
	return true;
}

void UIButton_dispose(UIButton * self) {
	free(self->label);
	call_super(dispose, self);
}

bool UIButton_hitTest(UIButton * self, float x, float y) {
	return Rect4f_containsVector2f(self->getBounds(self), VECTOR2f(x, y));
}

bool UIButton_mouseDown(UIButton * self, unsigned int buttonNumber, float x, float y) {
	if (buttonNumber == 0 && self->hitTest(self, x, y)) {
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
		// Do button action
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

bool UIButton_actionDown(UIButton * self) {
	// Do button action
	return false;
}

Rect4f UIButton_getBounds(UIButton * self) {
	Rect4f result;
	float height = self->appearance->metrics.buttonLabelHeight + self->appearance->metrics.buttonLabelPadding * 2;
	
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

void UIButton_getVertices(UIButton * self, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	unsigned int vertexCount = 0, indexCount = 0;
	Rect4f bounds = self->getBounds(self);
	
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
		
		writeVertex(vertexCount + 0, bounds.left, bounds.bottom, atlasEntry.left, atlasEntry.bottom);
		writeVertex(vertexCount + 1, bounds.left + slices.leftColumn, bounds.bottom, atlasEntry.left + slices.leftColumn * texCoordScaleX, atlasEntry.bottom);
		writeVertex(vertexCount + 2, bounds.right - slices.rightColumn, bounds.bottom, atlasEntry.right - slices.rightColumn * texCoordScaleX, atlasEntry.bottom);
		writeVertex(vertexCount + 3, bounds.right, bounds.bottom, atlasEntry.right, atlasEntry.bottom);
		
		writeVertex(vertexCount + 4, bounds.left, bounds.bottom + slices.bottomRow, atlasEntry.left, atlasEntry.bottom + slices.bottomRow * texCoordScaleY);
		writeVertex(vertexCount + 5, bounds.left + slices.leftColumn, bounds.bottom + slices.bottomRow, atlasEntry.left + slices.leftColumn * texCoordScaleX, atlasEntry.bottom + slices.bottomRow * texCoordScaleY);
		writeVertex(vertexCount + 6, bounds.right - slices.rightColumn, bounds.bottom + slices.bottomRow, atlasEntry.right - slices.rightColumn * texCoordScaleX, atlasEntry.bottom + slices.bottomRow * texCoordScaleY);
		writeVertex(vertexCount + 7, bounds.right, bounds.bottom + slices.bottomRow, atlasEntry.right, atlasEntry.bottom + slices.bottomRow * texCoordScaleY);
		
		writeVertex(vertexCount + 8, bounds.left, bounds.top - slices.topRow, atlasEntry.left, atlasEntry.top - slices.topRow * texCoordScaleY);
		writeVertex(vertexCount + 9, bounds.left + slices.leftColumn, bounds.top - slices.topRow, atlasEntry.left + slices.leftColumn * texCoordScaleX, atlasEntry.top - slices.topRow * texCoordScaleY);
		writeVertex(vertexCount + 10, bounds.right - slices.rightColumn, bounds.top - slices.topRow, atlasEntry.right - slices.rightColumn * texCoordScaleX, atlasEntry.top - slices.topRow * texCoordScaleY);
		writeVertex(vertexCount + 11, bounds.right, bounds.top - slices.topRow, atlasEntry.right, atlasEntry.top - slices.topRow * texCoordScaleY);
		
		writeVertex(vertexCount + 12, bounds.left, bounds.top, atlasEntry.left, atlasEntry.top);
		writeVertex(vertexCount + 13, bounds.left + slices.leftColumn, bounds.top, atlasEntry.left + slices.leftColumn * texCoordScaleX, atlasEntry.top);
		writeVertex(vertexCount + 14, bounds.right - slices.rightColumn, bounds.top, atlasEntry.right - slices.rightColumn * texCoordScaleX, atlasEntry.top);
		writeVertex(vertexCount + 15, bounds.right, bounds.top, atlasEntry.right, atlasEntry.top);
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
	
	GLBitmapFont_getStringVerticesWithColor(self->appearance->font,
	                                        self->label,
	                                        GLBITMAPFONT_USE_STRLEN,
	                                        self->appearance->metrics.buttonLabelHeight,
	                                        VECTOR2f(bounds.left + (bounds.right - bounds.left) * 0.5f, bounds.bottom + (bounds.top - bounds.bottom) * 0.5f),
	                                        VECTOR2f(0.5f, 0.5f),
	                                        self->appearance->metrics.buttonLabelColor,
	                                        GL_UNSIGNED_INT,
	                                        outVertices,
	                                        outIndexes,
	                                        ioVertexCount,
	                                        ioIndexCount);
}
