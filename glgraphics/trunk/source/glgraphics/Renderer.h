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

#ifndef __Renderer_H__
#define __Renderer_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Renderer Renderer;

typedef unsigned int RenderLayerID;
#define RENDER_LAYER_3D_OPAQUE 0
#define RENDER_LAYER_3D_TRANSLUCENT 1
#define RENDER_LAYER_2D 2
#define RENDER_LAYER_COUNT 3

enum {
	VERTEX_ATTRIB_POSITION,
	VERTEX_ATTRIB_TEXTURE_COORD,
	VERTEX_ATTRIB_NORMAL,
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
	struct RenderLayer layers[RENDER_LAYER_COUNT]; \
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

Renderer * Renderer_create();
bool Renderer_init(Renderer * self);
void Renderer_dispose(Renderer * self);

void Renderer_addRenderable(Renderer * self, RenderLayerID layer, Renderable * renderable);
void Renderer_removeRenderable(Renderer * self, RenderLayerID layer, Renderable * renderable);
void Renderer_clearRenderables(Renderer * self, RenderLayerID layer);
void Renderer_clearAllRenderables(Renderer * self);
void Renderer_setClearColor(Renderer * self, Color4f color);
void Renderer_clear(Renderer * self);
void Renderer_setProjectionMatrix(Renderer * self, Matrix4x4f matrix);
void Renderer_setViewMatrix(Renderer * self, Matrix4x4f matrix);
void Renderer_setLights(Renderer * self, Vector3f light0Position, Color4f light0Color, Vector3f light1Position, Color4f light1Color, Color4f ambientColor);
void Renderer_beginDrawing(Renderer * self);
void Renderer_drawLayer(Renderer * self, RenderLayerID layer);
void Renderer_drawSingle(Renderer * self, Renderable * renderable);
void Renderer_endDrawing(Renderer * self);

#ifdef __cplusplus
}
#endif
#endif
