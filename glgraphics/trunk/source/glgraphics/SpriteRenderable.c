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

SpriteRenderable * SpriteRenderable_create(TextureAtlas * atlas, const char * atlasKey, Vector2f relativeOrigin, Vector2f offset, Vector2f size, Color4f color) {
	stemobject_create_implementation(SpriteRenderable, init, atlas, atlasKey, relativeOrigin, offset, size, color)
}

bool SpriteRenderable_init(SpriteRenderable * self, TextureAtlas * atlas, const char * atlasKey, Vector2f relativeOrigin, Vector2f offset, Vector2f size, Color4f color) {
	call_super(init, self, RENDERABLE_SPRITE);
	self->dispose = SpriteRenderable_dispose;
	self->getTextureBindID = SpriteRenderable_getTextureBindID;
	self->getVertices = SpriteRenderable_getVertices;
	
	self->atlas = atlas;
	self->atlasKey = atlasKey;
	self->relativeOrigin = relativeOrigin;
	self->offset = offset;
	self->size = size;
	self->color = color;
	
	return true;
}

void SpriteRenderable_dispose(SpriteRenderable * self) {
	call_super(dispose, self);
}

unsigned int SpriteRenderable_getTextureBindID(SpriteRenderable * self) {
	if (self->atlas == NULL) {
		return 0;
	}
	return self->atlas->textureID;
}

void SpriteRenderable_getVertices(SpriteRenderable * self, void * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	TextureAtlas_getVertices(self->atlas,
	                         self->atlasKey,
	                         self->offset,
	                         self->relativeOrigin,
	                         TextureAtlas_getEntryDimensions(self->atlas, self->atlasKey, self->size.x, self->size.y),
	                         self->color,
	                         outVertices,
	                         outIndexes,
	                         ioVertexCount,
	                         ioIndexCount);
}
