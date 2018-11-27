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

#ifndef __SpriteRenderable_H__
#define __SpriteRenderable_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SpriteRenderable SpriteRenderable;

#include "gamemath/Matrix4x4f.h"
#include "gamemath/Vector2f.h"
#include "glgraphics/Renderable.h"
#include "glgraphics/TextureAtlas.h"

#define SpriteRenderable_structContents(self_type) \
	Renderable_structContents(self_type) \
	\
	TextureAtlas * atlas; \
	const char * atlasKey; \
	Vector2f drawOrigin; \
	Matrix4x4f transform; \
	unsigned int pixelWidth; /* Read only */ \
	unsigned int pixelHeight; /* Read only */ \
	GLuint vaoID; \
	GLuint vertexBufferID; \
	GLuint indexBufferID;

stemobject_struct_definition(SpriteRenderable)

SpriteRenderable * SpriteRenderable_create(TextureAtlas * atlas, const char * atlasKey, Vector2f drawOrigin, Matrix4x4f transform);
bool SpriteRenderable_init(SpriteRenderable * self, TextureAtlas * atlas, const char * atlasKey, Vector2f drawOrigin, Matrix4x4f transform);
void SpriteRenderable_dispose(SpriteRenderable * self);

void SpriteRenderable_updateAtlasKey(SpriteRenderable * self, const char * atlasKey, Vector2f drawOrigin);

#ifdef __cplusplus
}
#endif
#endif
