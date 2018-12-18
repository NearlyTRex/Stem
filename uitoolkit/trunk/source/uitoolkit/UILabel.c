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

#include "uitoolkit/UILabel.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS UIElement

UILabel * UILabel_create(UIAppearance * appearance, Vector2f position, Vector2f size, Vector2f relativeOrigin, const char * text, Color4f textColor, enum TextFlow_wordWrapMode wrapMode, enum UILabel_overflowMode overflowModeX, enum UILabel_overflowMode overflowModeY) {
	stemobject_create_implementation(UILabel, init, appearance, position, size, relativeOrigin, text, textColor, wrapMode, overflowModeX, overflowModeY)
}

bool UILabel_init(UILabel * self, UIAppearance * appearance, Vector2f position, Vector2f size, Vector2f relativeOrigin, const char * text, Color4f textColor, enum TextFlow_wordWrapMode wrapMode, enum UILabel_overflowMode overflowModeX, enum UILabel_overflowMode overflowModeY) {
	call_super(init, self, UIELEMENT_BUTTON, appearance, position, relativeOrigin);
	self->dispose = UILabel_dispose;
	self->getBounds = UILabel_getBounds;
	self->getVertices = UILabel_getVertices;
	
	self->textFlow = TextFlow_create(appearance->font, text, wrapMode, size.x);
	self->textColor = textColor;
	self->overflowModeX = overflowModeX;
	self->overflowModeY = overflowModeY;
	
	if (overflowModeX == UILABEL_OVERFLOW_RESIZE) {
		self->size.x = ceilf(TextFlow_measureString(self->textFlow).x * self->appearance->metrics.fontHeight);
	} else {
		self->size.x = size.x;
	}
	if (overflowModeY == UILABEL_OVERFLOW_RESIZE) {
		self->size.y = ceilf(TextFlow_getLineCount(self->textFlow) * self->appearance->metrics.fontHeight);
	} else {
		self->size.y = size.y;
	}
	return true;
}

void UILabel_dispose(UILabel * self) {
	TextFlow_dispose(self->textFlow);
	call_super(dispose, self);
}

void UILabel_setText(UILabel * self, const char * text) {
	TextFlow_setString(self->textFlow, text);
	if (self->overflowModeX == UILABEL_OVERFLOW_RESIZE) {
		self->size.x = ceilf(TextFlow_measureString(self->textFlow).x * self->appearance->metrics.fontHeight);
	}
	if (self->overflowModeY == UILABEL_OVERFLOW_RESIZE) {
		self->size.y = ceilf(TextFlow_getLineCount(self->textFlow) * self->appearance->metrics.fontHeight);
	}
}

Rect4f UILabel_getBounds(UILabel * self) {
	Rect4f result;
	
	result.left = self->position.x - self->size.x * self->relativeOrigin.x;
	result.bottom = self->position.y - self->size.y * self->relativeOrigin.y;
	result.right = result.left + self->size.x;
	result.top = result.bottom + self->size.y;
	return result;
}

void UILabel_getVertices(UILabel * self, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	Rect4f clipBounds = GLBITMAPFONT_NO_CLIP;
	
	if (self->overflowModeX == UILABEL_OVERFLOW_TRUNCATE || self->overflowModeY == UILABEL_OVERFLOW_TRUNCATE) {
		Rect4f bounds = UILabel_getBounds(self);
		
		if (self->overflowModeX == UILABEL_OVERFLOW_TRUNCATE) {
			clipBounds.left = bounds.left;
			clipBounds.right = bounds.right;
		}
		if (self->overflowModeY == UILABEL_OVERFLOW_TRUNCATE) {
			clipBounds.bottom = bounds.bottom;
			clipBounds.top = bounds.top;
		}
	}
	TextFlow_getVertices(self->textFlow,
	                     self->appearance->metrics.fontHeight,
	                     self->position,
	                     self->relativeOrigin,
	                     true,
	                     clipBounds,
	                     self->textColor,
	                     outVertices,
	                     outIndexes,
	                     ioVertexCount,
	                     ioIndexCount);
}
