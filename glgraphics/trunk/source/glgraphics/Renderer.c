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

#include "glgraphics/Renderer.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/MeshRenderable.h"
#include "glgraphics/VertexTypes.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Generated sources
#include "glgraphics/StaticVertexShader.h"
#include "glgraphics/AnimatedVertexShader.h"
#include "glgraphics/LitSurfaceFragmentShader.h"

#define SUPERCLASS StemObject
#define BONE_COUNT_MAX 128

enum {
	VERTEX_ATTRIB_POSITION,
	VERTEX_ATTRIB_TEXTURE_COORD,
	VERTEX_ATTRIB_NORMAL,
	VERTEX_ATTRIB_COLOR,
	VERTEX_ATTRIB_BONE_ID,
	VERTEX_ATTRIB_BONE_WEIGHT
};

Renderer * Renderer_create() {
	stemobject_create_implementation(Renderer, init)
}

bool Renderer_init(Renderer * self) {
	int renderLayerIndex;
	
	call_super(init, self);
	self->dispose = Renderer_dispose;
	// TODO: Maybe Renderables should each have their own VAO
	glGenVertexArrays(1, &self->vaoID);
	glBindVertexArray(self->vaoID);
	for (renderLayerIndex = 0; renderLayerIndex < RENDER_LAYER_COUNT; renderLayerIndex++) {
		self->layers[renderLayerIndex].renderableCount = 0;
		self->layers[renderLayerIndex].allocatedCount = 0;
		self->layers[renderLayerIndex].renderables = NULL;
	}
	self->projectionMatrix = MATRIX4x4f_IDENTITY;
	self->viewMatrix = MATRIX4x4f_IDENTITY;
	self->shaderStatic = GLSLShader_create(STATIC_StaticVertexShader, sizeof(STATIC_StaticVertexShader), STATIC_LitSurfaceFragmentShader, sizeof(STATIC_LitSurfaceFragmentShader),
		"inPosition", VERTEX_ATTRIB_POSITION,
		"inTexCoord", VERTEX_ATTRIB_TEXTURE_COORD,
		"inNormal", VERTEX_ATTRIB_NORMAL,
		"inColor", VERTEX_ATTRIB_COLOR,
	NULL);
	self->shaderAnimated = GLSLShader_create(STATIC_AnimatedVertexShader, sizeof(STATIC_AnimatedVertexShader), STATIC_LitSurfaceFragmentShader, sizeof(STATIC_LitSurfaceFragmentShader),
		"inPosition", VERTEX_ATTRIB_POSITION,
		"inTexCoord", VERTEX_ATTRIB_TEXTURE_COORD,
		"inNormal", VERTEX_ATTRIB_NORMAL,
		"inColor", VERTEX_ATTRIB_COLOR,
		"inBoneID", VERTEX_ATTRIB_BONE_ID,
		"inBoneWeight", VERTEX_ATTRIB_BONE_WEIGHT,
	NULL);
	return true;
}

void Renderer_dispose(Renderer * self) {
	int renderLayerIndex;
	
	for (renderLayerIndex = 0; renderLayerIndex < RENDER_LAYER_COUNT; renderLayerIndex++) {
		free(self->layers[renderLayerIndex].renderables);
	}
	GLSLShader_dispose(self->shaderStatic);
	GLSLShader_dispose(self->shaderAnimated);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &self->vaoID);
	call_super(dispose, self);
}

void Renderer_addRenderable(Renderer * self, RenderLayerID layer, Renderable * renderable) {
	if (layer < RENDER_LAYER_COUNT) {
		if (self->layers[layer].allocatedCount <= self->layers[layer].renderableCount) {
			if (self->layers[layer].allocatedCount == 0) {
				self->layers[layer].allocatedCount = 1;
			} else {
				self->layers[layer].allocatedCount *= 2;
			}
			self->layers[layer].renderables = realloc(self->layers[layer].renderables, sizeof(Renderable *) * self->layers[layer].allocatedCount);
		}
		self->layers[layer].renderables[self->layers[layer].renderableCount++] = renderable;
	}
}

void Renderer_removeRenderable(Renderer * self, RenderLayerID layer, Renderable * renderable) {
	if (layer < RENDER_LAYER_COUNT) {
		unsigned int renderableIndex;
		
		for (renderableIndex = 0; renderableIndex < self->layers[layer].renderableCount; renderableIndex++) {
			if (self->layers[layer].renderables[renderableIndex] == renderable) {
				unsigned int renderableIndex2;
				
				self->layers[layer].renderableCount--;
				for (renderableIndex2 = renderableIndex; renderableIndex2 < self->layers[layer].renderableCount; renderableIndex2++) {
					self->layers[layer].renderables[renderableIndex2] = self->layers[layer].renderables[renderableIndex2 + 1];
				}
			}
		}
	}
}

void Renderer_clearRenderables(Renderer * self, RenderLayerID layer) {
	if (layer < RENDER_LAYER_COUNT) {
		self->layers[layer].renderableCount = 0;
	}
}

void Renderer_clearAllRenderables(Renderer * self) {
	unsigned int layerIndex;
	
	for (layerIndex = 0; layerIndex < RENDER_LAYER_COUNT; layerIndex++) {
		Renderer_clearRenderables(self, layerIndex);
	}
}

void Renderer_setClearColor(Renderer * self, Color4f color) {
	glClearColor(color.red, color.green, color.blue, color.alpha);
}

void Renderer_clear(Renderer * self) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer_setProjectionMatrix(Renderer * self, Matrix4x4f matrix) {
	self->projectionMatrix = matrix;
}

void Renderer_setViewMatrix(Renderer * self, Matrix4x4f matrix) {
	self->viewMatrix = matrix;
}

void Renderer_setLights(Renderer * self, Vector3f light0Position, Color4f light0Color, Vector3f light1Position, Color4f light1Color, Color4f ambientColor) {
	self->light0Position = light0Position;
	self->light0Color = light0Color;
	self->light1Position = light1Position;
	self->light1Color = light1Color;
	self->ambientColor = ambientColor;
}

void Renderer_beginDrawing(Renderer * self) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
}

void Renderer_endDrawing(Renderer * self) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void Renderer_drawLayer(Renderer * self, RenderLayerID layer) {
	if (layer < RENDER_LAYER_COUNT) {
		unsigned int renderableIndex;
		
		for (renderableIndex = 0; renderableIndex < self->layers[layer].renderableCount; renderableIndex++) {
			Renderer_drawSingle(self, self->layers[layer].renderables[renderableIndex]);
		}
	}
}

void Renderer_drawSingle(Renderer * self, Renderable * renderable) {
	if (!renderable->visible) {
		return;
	}
	
	switch (renderable->type) {
		case RENDERABLE_MESH: {
			MeshRenderable * mesh = (MeshRenderable *) renderable;
			GLSLShader * shader;
			
			// TODO: Can this all be done by some intermediary between renderer and material? Should it? (Material only stores a description of visual properties, not shaders or logic)
			shader = mesh->hasAnimationData ? self->shaderAnimated : self->shaderStatic;
			glUseProgram(shader->programID);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "projectionTransform"), 1, GL_FALSE, self->projectionMatrix.m);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "viewTransform"), 1, GL_FALSE, self->viewMatrix.m);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "modelTransform"), 1, GL_FALSE, mesh->transform.m);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light0Position"), self->light0Position.x, self->light0Position.y, self->light0Position.z);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light0Color"), self->light0Color.red, self->light0Color.green, self->light0Color.blue);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light1Position"), self->light1Position.x, self->light1Position.y, self->light1Position.z);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light1Color"), self->light1Color.red, self->light1Color.green, self->light1Color.blue);
			glUniform3f(GLSLShader_getUniformLocation(shader, "ambientColor"), self->ambientColor.red, self->ambientColor.green, self->ambientColor.blue);
			glUniform1f(GLSLShader_getUniformLocation(shader, "specularIntensity"), 0.875f);
			glUniform1f(GLSLShader_getUniformLocation(shader, "shininess"), 32.0f);
			glUniform1i(GLSLShader_getUniformLocation(shader, "colorTexture"), 0);
			glBindTexture(GL_TEXTURE_2D, mesh->material->colorTextureID);
			if (mesh->hasAnimationData) {
				glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "boneTransforms"), mesh->animationState->armature->boneCount, GL_FALSE, (GLfloat *) mesh->animationState->computedBoneTransforms);
			}
			
			// TODO: Can this all be done by the renderable? Should it? Probably in a VAO
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBufferID);
			glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
			glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE_COORD);
			glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
			glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
			
			if (mesh->hasAnimationData) {
				glEnableVertexAttribArray(VERTEX_ATTRIB_BONE_ID);
				glEnableVertexAttribArray(VERTEX_ATTRIB_BONE_WEIGHT);
				glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, position));
				glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, texCoords));
				glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, normal));
				glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, color));
				glVertexAttribPointer(VERTEX_ATTRIB_BONE_ID, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, boneIndexes));
				glVertexAttribPointer(VERTEX_ATTRIB_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, boneWeights));
			} else {
				glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, position));
				glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, texCoords));
				glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, normal));
				glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, color));
			}
			
			glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
			
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
			glDisableVertexAttribArray(VERTEX_ATTRIB_POSITION);
			glDisableVertexAttribArray(VERTEX_ATTRIB_TEXTURE_COORD);
			glDisableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
			glDisableVertexAttribArray(VERTEX_ATTRIB_COLOR);
			if (mesh->hasAnimationData) {
				glDisableVertexAttribArray(VERTEX_ATTRIB_BONE_ID);
				glDisableVertexAttribArray(VERTEX_ATTRIB_BONE_WEIGHT);
			}
			
			break;
		}
		default:
			fprintf(stderr, "Warning: Unknown renderable type %d\n", renderable->type);
			break;
	}
}
