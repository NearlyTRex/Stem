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

#ifndef __Material_H__
#define __Material_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Material Material;

#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "stemobject/StemObject.h"

#define Material_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Color4f color; \
	float specularity; \
	float shininess; \
	float emissiveness; \
	GLuint colorTextureID; \
	GLuint normalTextureID;

stemobject_struct_definition(Material)

// specularity (0-1) determines blend portion of specular highlight (higher is more visible)
// shininess (0-inf) determines specular highlight spread (higher is smaller)
// emissiveness (0-1) determines ambient emission of color (higher is more visible)
Material * Material_create(Color4f color, float specularity, float shininess, float emissiveness);
bool Material_init(Material * self, Color4f color, float specularity, float shininess, float emissiveness);
void Material_dispose(Material * self);

// TODO: How to handle translucency and premultiplication
// Implicit: RGBA_8888, GL_UNSIGNED_BYTE, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR minification, anisotropy on, mipmaps generated
// If magnifyNearest is true GL_TEXTURE_MAX_FILTER is set to GL_NEAREST. Otherwise, GL_TEXTURE_MAG_FILTER is set to GL_LINEAR.
void Material_setColorTexture(Material * self, bool magnifyNearest, unsigned int width, unsigned int height, void * bitmapData);
void Material_setNormalTexture(Material * self, bool magnifyNearest, unsigned int width, unsigned int height, void * bitmapData);

#ifdef __cplusplus
}
#endif
#endif
