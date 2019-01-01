/*
  Copyright (c) 2019 Alex Diener
  
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

#include "uitoolkit/UIToolkitDrawing.h"

#define writeVertex(index, positionX, positionY, textureX, textureY) \
	vertex.position[0] = positionX; \
	vertex.position[1] = positionY; \
	vertex.texCoords[0] = textureX; \
	vertex.texCoords[1] = textureY; \
	outVertices[index] = vertex

void UIToolkit_getFrameVerticesWithSlices(Rect4f drawBounds,
                                          Rect4f textureBounds,
                                          UIAppearance_sliceGrid slices,
                                          Color4f color,
                                          struct vertex_p2f_t2f_c4f * outVertices,
                                          GLuint * outIndexes,
                                          unsigned int * ioVertexCount,
                                          unsigned int * ioIndexCount) {
	unsigned int vertexCount = 0, indexCount = 0;
	
	if (ioVertexCount != NULL) {
		vertexCount = *ioVertexCount;
	}
	if (ioIndexCount != NULL) {
		indexCount = *ioIndexCount;
	}
	if (outVertices != NULL) {
		struct vertex_p2f_t2f_c4f vertex;
		float texCoordScaleX, texCoordScaleY;
		
		vertex.color[0] = color.red;
		vertex.color[1] = color.green;
		vertex.color[2] = color.blue;
		vertex.color[3] = color.alpha;
		texCoordScaleX = (textureBounds.right - textureBounds.left) / (slices.leftColumn + slices.centerColumn + slices.rightColumn);
		texCoordScaleY = (textureBounds.top - textureBounds.bottom) / (slices.bottomRow + slices.centerRow + slices.topRow);
		
		writeVertex(vertexCount + 0, drawBounds.left, drawBounds.bottom, textureBounds.left, textureBounds.bottom);
		writeVertex(vertexCount + 1, drawBounds.left + slices.leftColumn, drawBounds.bottom, textureBounds.left + slices.leftColumn * texCoordScaleX, textureBounds.bottom);
		writeVertex(vertexCount + 2, drawBounds.right - slices.rightColumn, drawBounds.bottom, textureBounds.right - slices.rightColumn * texCoordScaleX, textureBounds.bottom);
		writeVertex(vertexCount + 3, drawBounds.right, drawBounds.bottom, textureBounds.right, textureBounds.bottom);
		
		writeVertex(vertexCount + 4, drawBounds.left, drawBounds.bottom + slices.bottomRow, textureBounds.left, textureBounds.bottom + slices.bottomRow * texCoordScaleY);
		writeVertex(vertexCount + 5, drawBounds.left + slices.leftColumn, drawBounds.bottom + slices.bottomRow, textureBounds.left + slices.leftColumn * texCoordScaleX, textureBounds.bottom + slices.bottomRow * texCoordScaleY);
		writeVertex(vertexCount + 6, drawBounds.right - slices.rightColumn, drawBounds.bottom + slices.bottomRow, textureBounds.right - slices.rightColumn * texCoordScaleX, textureBounds.bottom + slices.bottomRow * texCoordScaleY);
		writeVertex(vertexCount + 7, drawBounds.right, drawBounds.bottom + slices.bottomRow, textureBounds.right, textureBounds.bottom + slices.bottomRow * texCoordScaleY);
		
		writeVertex(vertexCount + 8, drawBounds.left, drawBounds.top - slices.topRow, textureBounds.left, textureBounds.top - slices.topRow * texCoordScaleY);
		writeVertex(vertexCount + 9, drawBounds.left + slices.leftColumn, drawBounds.top - slices.topRow, textureBounds.left + slices.leftColumn * texCoordScaleX, textureBounds.top - slices.topRow * texCoordScaleY);
		writeVertex(vertexCount + 10, drawBounds.right - slices.rightColumn, drawBounds.top - slices.topRow, textureBounds.right - slices.rightColumn * texCoordScaleX, textureBounds.top - slices.topRow * texCoordScaleY);
		writeVertex(vertexCount + 11, drawBounds.right, drawBounds.top - slices.topRow, textureBounds.right, textureBounds.top - slices.topRow * texCoordScaleY);
		
		writeVertex(vertexCount + 12, drawBounds.left, drawBounds.top, textureBounds.left, textureBounds.top);
		writeVertex(vertexCount + 13, drawBounds.left + slices.leftColumn, drawBounds.top, textureBounds.left + slices.leftColumn * texCoordScaleX, textureBounds.top);
		writeVertex(vertexCount + 14, drawBounds.right - slices.rightColumn, drawBounds.top, textureBounds.right - slices.rightColumn * texCoordScaleX, textureBounds.top);
		writeVertex(vertexCount + 15, drawBounds.right, drawBounds.top, textureBounds.right, textureBounds.top);
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
}
