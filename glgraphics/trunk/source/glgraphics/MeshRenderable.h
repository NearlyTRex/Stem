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

#ifndef __MeshRenderable_H__
#define __MeshRenderable_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MeshRenderable MeshRenderable;

#include "gamemath/Matrix4x4f.h"
#include "gamemath/Box6f.h"
#include "glgraphics/AnimationState.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/Material.h"
#include "glgraphics/Renderable.h"
#include "glgraphics/VertexTypes.h"

#define MeshRenderable_structContents(self_type) \
	Renderable_structContents(self_type) \
	\
	Material * material; \
	bool hasAnimationData; \
	GLuint vertexBufferID; \
	GLuint indexBufferID; \
	unsigned int indexCount; \
	Box6f bounds; \
	Matrix4x4f transform; \
	AnimationState * animationState;

stemobject_struct_definition(MeshRenderable)

// Accessor for dictionary of shader uniforms?
MeshRenderable * MeshRenderable_createStatic(struct vertex_p3f_t2f_n3f_c4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount, Material * material);
MeshRenderable * MeshRenderable_createAnimated(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount, Material * material, AnimationState * animationState);
bool MeshRenderable_initStatic(MeshRenderable * self, struct vertex_p3f_t2f_n3f_c4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount, Material * material);
bool MeshRenderable_initAnimated(MeshRenderable * self, struct vertex_p3f_t2f_n3f_c4f_b4u_w4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount, Material * material, AnimationState * animationState);
void MeshRenderable_dispose(MeshRenderable * self);

#ifdef __cplusplus
}
#endif
#endif
