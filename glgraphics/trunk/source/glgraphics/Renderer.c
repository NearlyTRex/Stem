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

#define VERTEX_ATTRIB_BONE_IDS 1
#define VERTEX_ATTRIB_BONE_WEIGHTS 2

Renderer * Renderer_create() {
	stemobject_create_implementation(Renderer, init)
}

bool Renderer_init(Renderer * self) {
	int renderLayerIndex;
	
	call_super(init, self);
	self->dispose = Renderer_dispose;
	for (renderLayerIndex = 0; renderLayerIndex < RENDER_LAYER_COUNT; renderLayerIndex++) {
		self->layers[renderLayerIndex].renderableCount = 0;
		self->layers[renderLayerIndex].allocatedCount = 0;
		self->layers[renderLayerIndex].renderables = NULL;
	}
	self->projectionMatrix = MATRIX4x4f_IDENTITY;
	self->viewMatrix = MATRIX4x4f_IDENTITY;
	self->shaderStatic = GLSLShader_create(STATIC_StaticVertexShader, sizeof(STATIC_StaticVertexShader), STATIC_LitSurfaceFragmentShader, sizeof(STATIC_LitSurfaceFragmentShader), NULL);
	self->shaderAnimated = GLSLShader_create(STATIC_AnimatedVertexShader, sizeof(STATIC_AnimatedVertexShader), STATIC_LitSurfaceFragmentShader, sizeof(STATIC_LitSurfaceFragmentShader), "boneIDs", VERTEX_ATTRIB_BONE_IDS, "boneWeights", VERTEX_ATTRIB_BONE_WEIGHTS, NULL);
	return true;
}

void Renderer_dispose(Renderer * self) {
	int renderLayerIndex;
	
	for (renderLayerIndex = 0; renderLayerIndex < RENDER_LAYER_COUNT; renderLayerIndex++) {
		free(self->layers[renderLayerIndex].renderables);
	}
	GLSLShader_dispose(self->shaderStatic);
	GLSLShader_dispose(self->shaderAnimated);
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
	switch (renderable->type) {
		case RENDERABLE_MESH: {
			MeshRenderable * mesh = (MeshRenderable *) renderable;
			GLSLShader * shader;
			
			// TODO: Can this all be done by some intermediary between renderer and material? Should it? (Material only stores a description of visual properties, not shaders or logic)
			shader = mesh->hasAnimationData ? self->shaderAnimated : self->shaderStatic;
			GLSLShader_activate(shader);
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
			if (mesh->hasAnimationData) {
				// Pass animation uniforms to shader, if any (might not be any uniforms; possibly attributes only)
			}
			
			// TODO: Can this all be done by the renderable? Should it?
			glBindBufferARB(GL_ARRAY_BUFFER, mesh->vertexBufferID);
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBufferID);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			
			if (mesh->hasAnimationData) {
				glVertexPointer(3, GL_FLOAT, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, position));
				glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, texCoords));
				glNormalPointer(GL_FLOAT, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, normal));
				glColorPointer(4, GL_FLOAT, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, color));
				glVertexAttribPointer(VERTEX_ATTRIB_BONE_IDS, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, boneIndexes));
				glVertexAttribPointer(VERTEX_ATTRIB_BONE_WEIGHTS, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, boneWeights));
				glEnableVertexAttribArray(VERTEX_ATTRIB_BONE_IDS);
				glEnableVertexAttribArray(VERTEX_ATTRIB_BONE_WEIGHTS);
			} else {
				glVertexPointer(3, GL_FLOAT, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, position));
				glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, texCoords));
				glNormalPointer(GL_FLOAT, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, normal));
				glColorPointer(4, GL_FLOAT, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, color));
			}
			
			glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
			
			glBindBufferARB(GL_ARRAY_BUFFER, 0);
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
			GLSLShader_deactivate(shader);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			if (mesh->hasAnimationData) {
				glDisableVertexAttribArray(VERTEX_ATTRIB_BONE_IDS);
				glDisableVertexAttribArray(VERTEX_ATTRIB_BONE_WEIGHTS);
			}
			
			break;
		}
		default:
			fprintf(stderr, "Warning: Unknown renderable type %d\n", renderable->type);
			break;
	}
}
