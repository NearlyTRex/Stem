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

#ifndef __GLTFContainer_H__
#define __GLTFContainer_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct GLTFContainer GLTFContainer;

#include "stemobject/StemObject.h"
#include "3dmodel/GLTFAccessor.h"
#include "3dmodel/GLTFBuffer.h"
#include "3dmodel/GLTFBufferView.h"
#include "3dmodel/GLTFImage.h"
#include "3dmodel/GLTFMaterial.h"
#include "3dmodel/GLTFMesh.h"
#include "3dmodel/GLTFNode.h"
#include "3dmodel/GLTFSampler.h"
#include "3dmodel/GLTFScene.h"
#include "3dmodel/GLTFTexture.h"

#define GLTFContainer_structContents(self_type) \
	unsigned int versionMajor; \
	unsigned int versionMinor; \
	unsigned int minVersionMajor; \
	unsigned int minVersionMinor; \
	unsigned int sceneCount; \
	GLTFScene * scenes; \
	unsigned int nodeCount; \
	GLTFNode * nodes; \
	unsigned int meshCount; \
	GLTFMesh * meshes; \
	unsigned int accessorCount; \
	GLTFAccessor * accessors; \
	unsigned int bufferCount; \
	GLTFBuffer * buffers; \
	unsigned int bufferViewCount; \
	GLTFBufferView * bufferViews; \
	unsigned int materialCount; \
	GLTFMaterial * materials; \
	unsigned int textureCount; \
	GLTFTexture * textures; \
	unsigned int imageCount; \
	GLTFImage * images; \
	unsigned int samplerCount; \
	GLTFSampler * samplers; \
	StemObject_structContents(self_type)

stemobject_struct_definition(GLTFContainer)

GLTFContainer * GLTFContainer_create(unsigned int versionMajor, unsigned int versionMinor);
bool GLTFContainer_init(GLTFContainer * self, unsigned int versionMajor, unsigned int versionMinor);
void GLTFContainer_dispose(GLTFContainer * self);

#ifdef __cplusplus
}
#endif
#endif
