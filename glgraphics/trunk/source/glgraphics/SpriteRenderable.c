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

#include "glgraphics/Renderer.h"
#include "glgraphics/SpriteRenderable.h"
#include <stddef.h>
#include <stdlib.h>

#define SUPERCLASS Renderable

SpriteRenderable * SpriteRenderable_create(TextureAtlas * atlas, const char * atlasKey, Vector2f drawOrigin, Matrix4x4f transform) {
	stemobject_create_implementation(SpriteRenderable, init, atlas, atlasKey, drawOrigin, transform)
}

static void bufferVertexData(SpriteRenderable * self) {
	struct vertex_p2f_t2f_c4f vertices[4];
	GLuint indexes[6] = {0, 1, 2, 2, 3, 0};
	Vector2f dimensions;
	
	glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indexBufferID);
	dimensions = TextureAtlas_getEntryDimensions(self->atlas, self->atlasKey, TEXTUREATLAS_SIZE_AUTO, TEXTUREATLAS_SIZE_AUTO);
	self->pixelWidth = dimensions.x;
	self->pixelHeight = dimensions.y;
	TextureAtlas_getVerticesWithColor(self->atlas,
	                                  self->atlasKey,
	                                  VECTOR2f_ZERO,
	                                  self->drawOrigin,
	                                  dimensions,
	                                  COLOR4f(1.0f, 1.0f, 1.0f, 1.0f), 
	                                  GL_UNSIGNED_INT,
	                                  vertices,
	                                  NULL,
	                                  NULL,
	                                  NULL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
}

bool SpriteRenderable_init(SpriteRenderable * self, TextureAtlas * atlas, const char * atlasKey, Vector2f drawOrigin, Matrix4x4f transform) {
	call_super(init, self, RENDERABLE_SPRITE);
	self->dispose = SpriteRenderable_dispose;
	
	self->atlas = atlas;
	self->atlasKey = atlasKey;
	self->drawOrigin = drawOrigin;
	self->transform = transform;
	
	// TODO: This shouldn't have its own VAO or VBO. Renderer should store one VAO for all sprites and regenerate the VBO every draw?
	glGenVertexArrays(1, &self->vaoID);
	glBindVertexArray(self->vaoID);
	glGenBuffers(1, &self->vertexBufferID);
	glGenBuffers(1, &self->indexBufferID);
	bufferVertexData(self);
	glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE_COORD);
	glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, position));
	glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, texCoords));
	glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, color));
	glBindVertexArray(0);
	
	return true;
}

void SpriteRenderable_dispose(SpriteRenderable * self) {
	glDeleteBuffers(1, &self->vertexBufferID);
	glDeleteBuffers(1, &self->indexBufferID);
	glDeleteVertexArrays(1, &self->vaoID);
	call_super(dispose, self);
}

void SpriteRenderable_updateAtlasKey(SpriteRenderable * self, const char * atlasKey, Vector2f drawOrigin) {
	self->atlasKey = atlasKey;
	self->drawOrigin = drawOrigin;
	glBindVertexArray(self->vaoID);
	bufferVertexData(self);
	glBindVertexArray(0);
}
