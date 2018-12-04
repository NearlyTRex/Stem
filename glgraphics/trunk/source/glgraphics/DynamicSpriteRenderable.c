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
#include "glgraphics/DynamicSpriteRenderable.h"
#include <stddef.h>
#include <stdlib.h>

#define SUPERCLASS Renderable

DynamicSpriteRenderable * DynamicSpriteRenderable_create(TextureAtlas * atlas, DynamicSpriteRenderable_getVerticesCallback callback, void * context, Matrix4x4f transform) {
	stemobject_create_implementation(DynamicSpriteRenderable, init, atlas, callback, context, transform)
}

bool DynamicSpriteRenderable_init(DynamicSpriteRenderable * self, TextureAtlas * atlas, DynamicSpriteRenderable_getVerticesCallback callback, void * context, Matrix4x4f transform) {
	call_super(init, self, RENDERABLE_SPRITE);
	self->dispose = DynamicSpriteRenderable_dispose;
	self->getTextureBindID = DynamicSpriteRenderable_getTextureBindID;
	self->getVertices = DynamicSpriteRenderable_getVertices;
	
	self->atlas = atlas;
	self->callback = callback;
	self->context = context;
	self->transform = transform;
	
	return true;
}

void DynamicSpriteRenderable_dispose(DynamicSpriteRenderable * self) {
	call_super(dispose, self);
}

unsigned int DynamicSpriteRenderable_getTextureBindID(DynamicSpriteRenderable * self) {
	if (self->atlas == NULL) {
		return 0;
	}
	return self->atlas->textureID;
}

void DynamicSpriteRenderable_getVertices(DynamicSpriteRenderable * self, void * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	self->callback(self, outVertices, outIndexes, ioVertexCount, ioIndexCount, self->context);
}
