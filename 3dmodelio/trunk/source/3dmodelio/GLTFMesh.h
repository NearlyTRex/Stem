/*
  Copyright (c) 2017 Alex Diener
  
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

#ifndef __GLTFMesh_H__
#define __GLTFMesh_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct GLTFMesh GLTFMesh;

#include "stemobject/StemObject.h"
#include "3dmodel/GLTFContainer.h"

#define GLTFMesh_structContents(self_type) \
	GLTFContainer * container; \
	char * name; \
	GLTFAccessor * positions; \
	GLTFAccessor * normals; \
	GLTFAccessor * tangents; \
	GLTFAccessor * texCoords0; \
	GLTFAccessor * texCoords1; \
	GLTFAccessor * colors0; \
	GLTFAccessor * joints0; \
	GLTFAccessor * weights0; \
	GLTFAccessor * indexes; \
	GLenum mode; \
	GLTFMaterial * material; \
	StemObject_structContents(self_type)

stemobject_struct_definition(GLTFMesh)

GLTFMesh * GLTFMesh_create(GLTFContainer * container, char * name, GLTFAccessor * positions, GLTFAccessor * normals, GLTFAccessor * tangents, GLTFAccessor * texCoords0, GLTFAccessor * texCoords1, GLTFAccessor * colors0, GLTFAccessor * joints0, GLTFAccessor * weights0, GLTFAccessor * indexes, GLenum mode, GLTFMaterial * material);
bool GLTFMesh_init(GLTFMesh * self);
void GLTFMesh_dispose(GLTFMesh * self);

#ifdef __cplusplus
}
#endif
#endif
