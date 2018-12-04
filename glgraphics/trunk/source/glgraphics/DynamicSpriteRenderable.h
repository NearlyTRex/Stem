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

#ifndef __DynamicSpriteRenderable_H__
#define __DynamicSpriteRenderable_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DynamicSpriteRenderable DynamicSpriteRenderable;

#include "gamemath/Matrix4x4f.h"
#include "gamemath/Vector2f.h"
#include "glgraphics/Renderable.h"
#include "glgraphics/TextureAtlas.h"

typedef void (* DynamicSpriteRenderable_getVerticesCallback)(DynamicSpriteRenderable * self, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount, void * context);

#define DynamicSpriteRenderable_structContents(self_type) \
	Renderable_structContents(self_type) \
	\
	TextureAtlas * atlas; \
	Matrix4x4f transform; \
	DynamicSpriteRenderable_getVerticesCallback callback; \
	void * context;

stemobject_struct_definition(DynamicSpriteRenderable)

DynamicSpriteRenderable * DynamicSpriteRenderable_create(TextureAtlas * atlas, DynamicSpriteRenderable_getVerticesCallback, void * context, Matrix4x4f transform);
bool DynamicSpriteRenderable_init(DynamicSpriteRenderable * self, TextureAtlas * atlas, DynamicSpriteRenderable_getVerticesCallback, void * context, Matrix4x4f transform);
void DynamicSpriteRenderable_dispose(DynamicSpriteRenderable * self);
unsigned int DynamicSpriteRenderable_getTextureBindID(DynamicSpriteRenderable * self);
void DynamicSpriteRenderable_getVertices(DynamicSpriteRenderable * self, void * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
