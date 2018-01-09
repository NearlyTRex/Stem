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

#include "glgraphics/Renderer.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/MeshRenderable.h"
#include "glgraphics/VertexTypes.h"
#include <stdio.h>
#include <stdlib.h>

// Generated sources
#include "glgraphics/StaticVertexShader.h"
#include "glgraphics/AnimatedVertexShader.h"
#include "glgraphics/LitSurfaceFragmentShader.h"

#define SUPERCLASS StemObject
#define BONE_COUNT_MAX 128

Renderer * Renderer_create() {
	stemobject_create_implementation(Renderer, init)
}

bool Renderer_init(Renderer * self) {
	int renderLayerIndex;
	unsigned char nullMaterialColor[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char nullMaterialNormal[4] = {0x00, 0x00, 0xFF, 0xFF};
	
	call_super(init, self);
	self->dispose = Renderer_dispose;
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
	self->nullMaterial = Material_create(COLOR4f(1.0f, 1.0f, 1.0f, 1.0f), 0.875f, 32.0f, 0.0f);
	Material_setColorTexture(self->nullMaterial, true, 1, 1, nullMaterialColor);
	Material_setNormalTexture(self->nullMaterial, true, 1, 1, nullMaterialNormal);
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
	Material_dispose(self->nullMaterial);
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
			Vector3f cameraPosition;
			
			shader = mesh->animationState == NULL ? self->shaderStatic : self->shaderAnimated;
			glUseProgram(shader->programID);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "projectionTransform"), 1, GL_FALSE, self->projectionMatrix.m);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "viewTransform"), 1, GL_FALSE, self->viewMatrix.m);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "modelTransform"), 1, GL_FALSE, mesh->transform.m);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light0Position"), self->light0Position.x, self->light0Position.y, self->light0Position.z);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light0Color"), self->light0Color.red, self->light0Color.green, self->light0Color.blue);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light1Position"), self->light1Position.x, self->light1Position.y, self->light1Position.z);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light1Color"), self->light1Color.red, self->light1Color.green, self->light1Color.blue);
			glUniform3f(GLSLShader_getUniformLocation(shader, "ambientColor"), self->ambientColor.red, self->ambientColor.green, self->ambientColor.blue);
			cameraPosition = Matrix4x4f_multiplyVector3f(self->viewMatrix, VECTOR3f_ZERO);
			glUniform3f(GLSLShader_getUniformLocation(shader, "cameraPosition"), cameraPosition.x, cameraPosition.y, cameraPosition.z);
			glUniform1i(GLSLShader_getUniformLocation(shader, "colorTexture"), 0);
			if (mesh->animationState != NULL) {
				glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "boneTransforms"), mesh->animationState->armature->boneCount, GL_FALSE, (GLfloat *) mesh->animationState->computedBoneTransforms);
			}
			
			// TODO: Respect material's color, emissiveness, and normal map
			if (mesh->material == NULL) {
				glUniform1f(GLSLShader_getUniformLocation(shader, "specularity"), 0.875f);
				glUniform1f(GLSLShader_getUniformLocation(shader, "shininess"), 32.0f);
				glBindTexture(GL_TEXTURE_2D, self->nullMaterial->colorTextureID);
			} else {
				glUniform1f(GLSLShader_getUniformLocation(shader, "specularity"), mesh->material->specularity);
				glUniform1f(GLSLShader_getUniformLocation(shader, "shininess"), mesh->material->shininess);
				glBindTexture(GL_TEXTURE_2D, mesh->material->colorTextureID);
			}
			
			glBindVertexArray(mesh->vertexBuffer->vaoID);
			glDrawElements(GL_TRIANGLES, mesh->vertexBuffer->indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
			
			break;
		}
		default:
			fprintf(stderr, "Warning: Unknown renderable type %d\n", renderable->type);
			break;
	}
}
