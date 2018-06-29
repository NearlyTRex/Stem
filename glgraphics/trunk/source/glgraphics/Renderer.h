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

#ifndef __Renderer_H__
#define __Renderer_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Renderer Renderer;

enum RendererDrawMode {
	RENDERER_3D_OPAQUE,
	RENDERER_3D_TRANSLUCENT,
	RENDERER_2D_OPAQUE,
	RENDERER_2D_TRANSLUCENT
};

enum {
	VERTEX_ATTRIB_POSITION,
	VERTEX_ATTRIB_TEXTURE_COORD,
	VERTEX_ATTRIB_NORMAL,
	VERTEX_ATTRIB_TANGENT,
	VERTEX_ATTRIB_COLOR,
	VERTEX_ATTRIB_BONE_ID,
	VERTEX_ATTRIB_BONE_WEIGHT
};

#include "stemobject/StemObject.h"
#include "gamemath/Matrix4x4f.h"
#include "glgraphics/GLSLShader.h"
#include "glgraphics/Material.h"
#include "glgraphics/Renderable.h"
#include "glgraphics/VertexTypes.h"

struct RenderLayer {
	unsigned int renderableCount;
	unsigned int allocatedCount;
	Renderable ** renderables;
};

#define Renderer_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	struct RenderLayer * layers; \
	unsigned int layerCount; \
	Vector3f light0Position; \
	Color4f light0Color; \
	Vector3f light1Position; \
	Color4f light1Color; \
	Color4f ambientColor; \
	Matrix4x4f projectionMatrix; \
	Matrix4x4f viewMatrix; \
	GLSLShader * shaderStatic; \
	GLSLShader * shaderAnimated; \
	Material * nullMaterial;

stemobject_struct_definition(Renderer)

Renderer * Renderer_create(unsigned int layerCount);
bool Renderer_init(Renderer * self, unsigned int layerCount);
void Renderer_dispose(Renderer * self);

void Renderer_addRenderable(Renderer * self, unsigned int layerIndex, Renderable * renderable);
void Renderer_removeRenderable(Renderer * self, unsigned int layerIndex, Renderable * renderable);
void Renderer_clearRenderables(Renderer * self, unsigned int layerIndex);
void Renderer_clearAllRenderables(Renderer * self);
void Renderer_setClearColor(Renderer * self, Color4f color);
void Renderer_clear(Renderer * self);
void Renderer_setProjectionMatrix(Renderer * self, Matrix4x4f matrix);
void Renderer_setViewMatrix(Renderer * self, Matrix4x4f matrix);
void Renderer_setLights(Renderer * self, Vector3f light0Position, Color4f light0Color, Vector3f light1Position, Color4f light1Color, Color4f ambientColor);
void Renderer_setDrawMode(Renderer * self, enum RendererDrawMode mode);
void Renderer_drawLayer(Renderer * self, unsigned int layerIndex);
void Renderer_drawSingle(Renderer * self, Renderable * renderable);

#ifdef __cplusplus
}
#endif
#endif
