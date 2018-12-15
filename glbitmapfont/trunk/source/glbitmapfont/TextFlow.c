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

#include "glbitmapfont/TextFlow.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SUPERCLASS StemObject

TextFlow * TextFlow_create(GLBitmapFont * font, const char * string, enum TextFlow_wordWrapMode wrapMode, float wrapWidth) {
	stemobject_create_implementation(TextFlow, init, font, string, wrapMode, wrapWidth)
}

bool TextFlow_init(TextFlow * self, GLBitmapFont * font, const char * string, enum TextFlow_wordWrapMode wrapMode, float wrapWidth) {
	call_super(init, self);
	self->dispose = TextFlow_dispose;
	self->font = font;
	self->string = strdup(string);
	self->wrapMode = wrapMode;
	self->wrapWidth = wrapWidth;
	self->private_ivar(wrapInfo).wrapPointAllocatedCount = 8;
	self->private_ivar(wrapInfo).wrapPoints = malloc(sizeof(*self->private_ivar(wrapInfo).wrapPoints) * self->private_ivar(wrapInfo).wrapPointAllocatedCount);
	self->private_ivar(wrapInfo).wrapPointCount = 0;
	self->private_ivar(wrapInfo).string = NULL;
	return true;
}

void TextFlow_dispose(TextFlow * self) {
	free(self->private_ivar(wrapInfo).wrapPoints);
	call_super(dispose, self);
}

static inline bool isWrappableWhitespace(char character) {
	return character == ' ' || character == '\t';
}

static void addWrapPoint(TextFlow * self, size_t wrapPoint) {
	if (self->private_ivar(wrapInfo).wrapPointAllocatedCount <= self->private_ivar(wrapInfo).wrapPointCount) {
		self->private_ivar(wrapInfo).wrapPointAllocatedCount *= 2;
		self->private_ivar(wrapInfo).wrapPoints = realloc(self->private_ivar(wrapInfo).wrapPoints, sizeof(*self->private_ivar(wrapInfo).wrapPoints) * self->private_ivar(wrapInfo).wrapPointAllocatedCount);
	}
	self->private_ivar(wrapInfo).wrapPoints[self->private_ivar(wrapInfo).wrapPointCount++] = wrapPoint;
}

static void updateWrapInfo(TextFlow * self) {
	if (self->font != self->private_ivar(wrapInfo).font ||
	    self->string != self->private_ivar(wrapInfo).string ||
	    self->wrapMode != self->private_ivar(wrapInfo).wrapMode ||
	    self->wrapWidth != self->private_ivar(wrapInfo).wrapWidth) {
		float measuredWidth;
		size_t lineEndCharIndex, edgeCharIndex, length = strlen(self->string), wrapIndex, lastWrapIndex = 0, lastLastWrapIndex = 1;
		
		self->private_ivar(wrapInfo).font = self->font;
		self->private_ivar(wrapInfo).string = self->string;
		self->private_ivar(wrapInfo).wrapMode = self->wrapMode;
		self->private_ivar(wrapInfo).wrapWidth = self->wrapWidth;
		
		self->private_ivar(wrapInfo).wrapPointCount = 0;
		
		for (; lastWrapIndex < length;) {
			if (lastLastWrapIndex == lastWrapIndex) {
				printf("Infinite loop detected!\n");
				break;
			}
			lastLastWrapIndex = lastWrapIndex;
			for (lineEndCharIndex = lastWrapIndex; lineEndCharIndex < length; lineEndCharIndex++) {
				if (self->string[lineEndCharIndex] == '\n') {
					break;
				}
			}
			if (lineEndCharIndex != lastWrapIndex && self->wrapMode != WORD_WRAP_NONE) {
				measuredWidth = GLBitmapFont_measureString(self->font, self->string + lastWrapIndex, lineEndCharIndex - lastWrapIndex);
				if (measuredWidth >= self->wrapWidth) {
					// Line needs to wrap
					edgeCharIndex = GLBitmapFont_indexAtWidth(self->font, self->string + lastWrapIndex, lineEndCharIndex - lastWrapIndex, self->wrapWidth, NULL) + lastWrapIndex;
					wrapIndex = edgeCharIndex;
					while (wrapIndex < lineEndCharIndex && isWrappableWhitespace(self->string[wrapIndex])) {
						wrapIndex++;
					}
					if (wrapIndex == lineEndCharIndex) {
						// Portion of line that overflows is whitespace only
						if (lineEndCharIndex == length) {
							break;
						}
						addWrapPoint(self, wrapIndex);
						lastWrapIndex = wrapIndex + 1;
						continue;
						
					} else {
						// Line contains non-whitespace overflow
						while (wrapIndex > lastWrapIndex && !isWrappableWhitespace(self->string[wrapIndex])) {
							wrapIndex--;
						}
						if (wrapIndex == lastWrapIndex && !isWrappableWhitespace(self->string[wrapIndex])) {
							// Line contains no wrappable whitespace prior to overflow
							if (self->wrapMode == WORD_WRAP_AGGRESSIVE) {
								// Split the overflowing word without whitespace
								if (edgeCharIndex == lastWrapIndex) {
									// This single character doesn't fit; let it overflow and move forward
									edgeCharIndex++;
								}
								if (self->string[edgeCharIndex] != '\n') {
									addWrapPoint(self, edgeCharIndex);
									lastWrapIndex = edgeCharIndex;
									continue;
								}
								
							} else {
								// Allow overflow and wrap at the next available whitespace
								wrapIndex = edgeCharIndex;
								while (wrapIndex < lineEndCharIndex && !isWrappableWhitespace(self->string[wrapIndex])) {
									wrapIndex++;
								}
								while (wrapIndex < lineEndCharIndex && isWrappableWhitespace(self->string[wrapIndex])) {
									wrapIndex++;
								}
								// If this line ends with a newline, just let that wrap by itself instead of inserting an extra break
								if (self->string[wrapIndex] != '\n') {
									addWrapPoint(self, wrapIndex);
									lastWrapIndex = wrapIndex;
									continue;
								}
							}
							
						} else {
							// Found wrappable whitespace prior to overflow
							addWrapPoint(self, wrapIndex + 1);
							lastWrapIndex = wrapIndex + 1;
							continue;
						}
					}
				}
			}
			if (lineEndCharIndex >= length) {
				// Reached the natural end of the string with no unhandled newlines or overflows
				break;
			}
			// If a continue or a break didn't happen prior to this, there's a newline at lineEndCharIndex, and no overflow prior to it
			addWrapPoint(self, lineEndCharIndex);
			lastWrapIndex = lineEndCharIndex + 1;
		}
	}
}

void TextFlow_getVertices(TextFlow * self,
                          float emSize,
                          Vector2f offset,
                          Vector2f relativeOrigin,
                          bool pixelSnapping,
                          float clipWidth,
                          float clipHeight,
                          Color4f color,
                          struct vertex_p2f_t2f_c4f * outVertices,
                          GLuint * outIndexes,
                          unsigned int * ioVertexCount,
                          unsigned int * ioIndexCount) {
	size_t wrapPointIndex, charIndex = 0, length = strlen(self->string);
	
	updateWrapInfo(self);
	for (wrapPointIndex = 0; wrapPointIndex < self->private_ivar(wrapInfo).wrapPointCount; wrapPointIndex++) {
		GLBitmapFont_getStringVertices(self->font, self->string + charIndex, self->private_ivar(wrapInfo).wrapPoints[wrapPointIndex] - charIndex, emSize, VECTOR2f(offset.x, offset.y - wrapPointIndex * emSize + self->private_ivar(wrapInfo).wrapPointCount * emSize * relativeOrigin.y), relativeOrigin, pixelSnapping, clipWidth, clipHeight, color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		charIndex = self->private_ivar(wrapInfo).wrapPoints[wrapPointIndex];
	}
	GLBitmapFont_getStringVertices(self->font, self->string + charIndex, length - charIndex, emSize, VECTOR2f(offset.x, offset.y - self->private_ivar(wrapInfo).wrapPointCount * emSize + self->private_ivar(wrapInfo).wrapPointCount * emSize * relativeOrigin.y), relativeOrigin, pixelSnapping, clipWidth, clipHeight, color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
}
