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

#ifndef __Renderable_H__
#define __Renderable_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Renderable Renderable;

typedef enum {
	RENDERABLE_MESH,
	RENDERABLE_SPRITE
} RenderableType;

#include "stemobject/StemObject.h"
#include "glgraphics/GLIncludes.h"

#define Renderable_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	RenderableType type; \
	/* Set by owner. Renderer ignores for drawing any Renderable that's not visible. True by default. */ \
	bool visible; \
	/* Set by owner. Groups of objects with the same drawOrder will be drawn in an unspecified order determined by what Renderer */ \
	/* decides is optimal. This can be overridden by assigning a specific value to this field, which will be respected even at */ \
	/* the potential cost of performance. Default is 0. */ \
	int drawOrder; \
	\
	/* Overridden by subclass. Returns a stable, arbitrary, unique* identifier for the set of texture binds that will be used */ \
	/* to draw this object. Renderer will attempt to batch draw calls for objects that have the same drawOrder and texture binds. */ \
	/* Default implementation returns 0. */ \
	/* *Note: This identifier is only required to be unique within the vertex type group to which it belongs. */ \
	unsigned int (* getTextureBindID)(self_type * self); \
	/* Overridden by subclass. Fills the buffers specified by outVertices and outIndexes with the appropriate data for drawing */ \
	/* this object. outVertices is assumed to be in the appropriate format for this type of renderable (see RendererVertexType enum), */ \
	/* and both it and outIndexes are assumed to have enough room in it to hold the data being written. */ \
	/* ioVertexCount and ioIndexCount will be used as indexes into outVertices and outIndexes, and updated with the additional */ \
	/* data appended to them. If outVertices and/or outIndexes is NULL, ioVertexCount and ioIndexCount will still be updated, */ \
	/* allowing this function to be used to measure the space that needs to be allocated for them. */ \
	/* The default implementation of this function does nothing. */ \
	void (* getVertices)(self_type * self, void * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

stemobject_struct_definition(Renderable)

bool Renderable_init(Renderable * self, RenderableType type);
void Renderable_dispose(Renderable * self);
unsigned int Renderable_getTextureBindID(Renderable * self);
void Renderable_getVertices(Renderable * self, void * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
