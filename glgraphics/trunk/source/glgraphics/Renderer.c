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
#include "glgraphics/SpriteRenderable.h"
#include "glgraphics/VertexTypes.h"
#include <stdio.h>
#include <stdlib.h>

// Generated sources
#include "glgraphics/StaticMeshVertexShader.h"
#include "glgraphics/AnimatedMeshVertexShader.h"
#include "glgraphics/LitSurfaceFragmentShader.h"
#include "glgraphics/SpriteVertexShader.h"
#include "glgraphics/SpriteFragmentShader.h"

#define SUPERCLASS StemObject
#define BONE_COUNT_MAX 128

Renderer * Renderer_create(unsigned int layerCount) {
	stemobject_create_implementation(Renderer, init, layerCount)
}

bool Renderer_init(Renderer * self, unsigned int layerCount) {
	unsigned int layerIndex;
	unsigned char nullMaterialColor[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char nullMaterialNormal[4] = {0x7F, 0x7F, 0xFF, 0xFF};
	
	call_super(init, self);
	self->dispose = Renderer_dispose;
	self->layers = malloc(sizeof(struct RenderLayer) * layerCount);
	for (layerIndex = 0; layerIndex < layerCount; layerIndex++) {
		self->layers[layerIndex].renderableCount = 0;
		self->layers[layerIndex].allocatedCount = 0;
		self->layers[layerIndex].renderables = NULL;
	}
	self->layerCount = layerCount;
	self->projectionMatrix = MATRIX4x4f_IDENTITY;
	self->viewMatrix = MATRIX4x4f_IDENTITY;
	self->shaderStaticMesh = GLSLShader_create(STATIC_StaticMeshVertexShader, sizeof(STATIC_StaticMeshVertexShader), STATIC_LitSurfaceFragmentShader, sizeof(STATIC_LitSurfaceFragmentShader),
		"inPosition", VERTEX_ATTRIB_POSITION,
		"inTexCoord", VERTEX_ATTRIB_TEXTURE_COORD,
		"inNormal", VERTEX_ATTRIB_NORMAL,
		"inTangent", VERTEX_ATTRIB_TANGENT,
		"inColor", VERTEX_ATTRIB_COLOR,
	NULL);
	self->shaderAnimatedMesh = GLSLShader_create(STATIC_AnimatedMeshVertexShader, sizeof(STATIC_AnimatedMeshVertexShader), STATIC_LitSurfaceFragmentShader, sizeof(STATIC_LitSurfaceFragmentShader),
		"inPosition", VERTEX_ATTRIB_POSITION,
		"inTexCoord", VERTEX_ATTRIB_TEXTURE_COORD,
		"inNormal", VERTEX_ATTRIB_NORMAL,
		"inTangent", VERTEX_ATTRIB_TANGENT,
		"inColor", VERTEX_ATTRIB_COLOR,
		"inBoneID", VERTEX_ATTRIB_BONE_ID,
		"inBoneWeight", VERTEX_ATTRIB_BONE_WEIGHT,
	NULL);
	self->shaderSprite = GLSLShader_create(STATIC_SpriteVertexShader, sizeof(STATIC_SpriteVertexShader), STATIC_SpriteFragmentShader, sizeof(STATIC_SpriteFragmentShader),
		"inPosition", VERTEX_ATTRIB_POSITION,
		"inTexCoord", VERTEX_ATTRIB_TEXTURE_COORD,
		"inColor", VERTEX_ATTRIB_COLOR,
	NULL);
	self->nullMaterial = Material_create(COLOR4f(1.0f, 1.0f, 1.0f, 1.0f), 0.875f, 32.0f, 0.0f);
	Material_setTexture(self->nullMaterial, MaterialTextureType_color, true, 1, 1, nullMaterialColor);
	Material_setTexture(self->nullMaterial, MaterialTextureType_normal, true, 1, 1, nullMaterialNormal);
	return true;
}

void Renderer_dispose(Renderer * self) {
	unsigned int layerIndex;
	
	for (layerIndex = 0; layerIndex < self->layerCount; layerIndex++) {
		free(self->layers[layerIndex].renderables);
	}
	free(self->layers);
	GLSLShader_dispose(self->shaderStaticMesh);
	GLSLShader_dispose(self->shaderAnimatedMesh);
	GLSLShader_dispose(self->shaderSprite);
	glBindVertexArray(0);
	Material_dispose(self->nullMaterial);
	call_super(dispose, self);
}

void Renderer_addRenderable(Renderer * self, unsigned int layerIndex, Renderable * renderable) {
	if (layerIndex < self->layerCount) {
		if (self->layers[layerIndex].allocatedCount <= self->layers[layerIndex].renderableCount) {
			if (self->layers[layerIndex].allocatedCount == 0) {
				self->layers[layerIndex].allocatedCount = 1;
			} else {
				self->layers[layerIndex].allocatedCount *= 2;
			}
			self->layers[layerIndex].renderables = realloc(self->layers[layerIndex].renderables, sizeof(Renderable *) * self->layers[layerIndex].allocatedCount);
		}
		self->layers[layerIndex].renderables[self->layers[layerIndex].renderableCount++] = renderable;
	} else {
		fprintf(stderr, "Warning: Renderer_addRenderable called with a layerIndex of %u, while Renderer %p has been initialized with only %u layers\n", layerIndex, self, self->layerCount);
	}
}

void Renderer_removeRenderable(Renderer * self, unsigned int layerIndex, Renderable * renderable) {
	if (layerIndex < self->layerCount) {
		unsigned int renderableIndex;
		
		for (renderableIndex = 0; renderableIndex < self->layers[layerIndex].renderableCount; renderableIndex++) {
			if (self->layers[layerIndex].renderables[renderableIndex] == renderable) {
				unsigned int renderableIndex2;
				
				self->layers[layerIndex].renderableCount--;
				for (renderableIndex2 = renderableIndex; renderableIndex2 < self->layers[layerIndex].renderableCount; renderableIndex2++) {
					self->layers[layerIndex].renderables[renderableIndex2] = self->layers[layerIndex].renderables[renderableIndex2 + 1];
				}
			}
		}
	} else {
		fprintf(stderr, "Warning: Renderer_removeRenderable called with a layerIndex of %u, while Renderer %p has been initialized with only %u layers\n", layerIndex, self, self->layerCount);
	}
}

void Renderer_clearRenderables(Renderer * self, unsigned int layerIndex) {
	if (layerIndex < self->layerCount) {
		self->layers[layerIndex].renderableCount = 0;
	} else {
		fprintf(stderr, "Warning: Renderer_clearRenderables called with a layerIndex of %u, while Renderer %p has been initialized with only %u layers\n", layerIndex, self, self->layerCount);
	}
}

void Renderer_clearAllRenderables(Renderer * self) {
	unsigned int layerIndex;
	
	for (layerIndex = 0; layerIndex < self->layerCount; layerIndex++) {
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

void Renderer_setDrawMode(Renderer * self, enum RendererDrawMode mode) {
	switch (mode) {
		case RENDERER_3D_OPAQUE:
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glDepthFunc(GL_LEQUAL);
			break;
			
		case RENDERER_3D_TRANSLUCENT:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glDepthFunc(GL_LEQUAL);
			break;
			
		case RENDERER_2D_OPAQUE:
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			break;
			
		case RENDERER_2D_TRANSLUCENT:
			glEnable(GL_BLEND);
			// TODO: Options for premultiplication
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			break;
	}
}

void Renderer_drawLayer(Renderer * self, unsigned int layerIndex) {
	if (layerIndex < self->layerCount) {
		unsigned int renderableIndex;
		
		for (renderableIndex = 0; renderableIndex < self->layers[layerIndex].renderableCount; renderableIndex++) {
			Renderer_drawSingle(self, self->layers[layerIndex].renderables[renderableIndex]);
		}
	} else {
		fprintf(stderr, "Warning: Renderer_drawLayer called with a layerIndex of %u, while Renderer %p has been initialized with only %u layers\n", layerIndex, self, self->layerCount);
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
			
			shader = mesh->animationState == NULL ? self->shaderStaticMesh : self->shaderAnimatedMesh;
			glUseProgram(shader->programID);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "projectionTransform"), 1, GL_FALSE, self->projectionMatrix.m);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "viewTransform"), 1, GL_FALSE, self->viewMatrix.m);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "modelTransform"), 1, GL_FALSE, mesh->transform.m);
			// TODO: Compute normal matrix so vertex shader doesn't have to; see https://learnopengl.com/#!Lighting/Basic-Lighting
			glUniform3f(GLSLShader_getUniformLocation(shader, "light0Position"), self->light0Position.x, self->light0Position.y, self->light0Position.z);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light0Color"), self->light0Color.red, self->light0Color.green, self->light0Color.blue);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light1Position"), self->light1Position.x, self->light1Position.y, self->light1Position.z);
			glUniform3f(GLSLShader_getUniformLocation(shader, "light1Color"), self->light1Color.red, self->light1Color.green, self->light1Color.blue);
			glUniform3f(GLSLShader_getUniformLocation(shader, "ambientColor"), self->ambientColor.red, self->ambientColor.green, self->ambientColor.blue);
			cameraPosition = Matrix4x4f_multiplyVector3f(Matrix4x4f_inverted(self->viewMatrix), VECTOR3f_ZERO);
			glUniform3f(GLSLShader_getUniformLocation(shader, "cameraPosition"), cameraPosition.x, cameraPosition.y, -cameraPosition.z);
			glUniform1i(GLSLShader_getUniformLocation(shader, "colorTexture"), 0);
			glUniform1i(GLSLShader_getUniformLocation(shader, "normalTexture"), 1);
			if (mesh->animationState != NULL) {
				glUniformMatrix4fv(GLSLShader_getUniformLocation(shader, "boneTransforms"), mesh->animationState->armature->boneCount, GL_FALSE, (GLfloat *) mesh->animationState->computedBoneTransforms);
			}
			
			if (mesh->material == NULL) {
				glUniform1f(GLSLShader_getUniformLocation(shader, "specularity"), 0.875f);
				glUniform1f(GLSLShader_getUniformLocation(shader, "shininess"), 32.0f);
				glUniform1f(GLSLShader_getUniformLocation(shader, "emissiveness"), 0.0f);
				glUniform4f(GLSLShader_getUniformLocation(shader, "materialColor"), 1.0f, 1.0f, 1.0f, 1.0f);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, self->nullMaterial->colorTextureID);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, self->nullMaterial->normalTextureID);
			} else {
				glUniform1f(GLSLShader_getUniformLocation(shader, "specularity"), mesh->material->specularity);
				glUniform1f(GLSLShader_getUniformLocation(shader, "shininess"), mesh->material->shininess);
				glUniform1f(GLSLShader_getUniformLocation(shader, "emissiveness"), mesh->material->emissiveness);
				glUniform4f(GLSLShader_getUniformLocation(shader, "materialColor"), mesh->material->color.red, mesh->material->color.green, mesh->material->color.blue, mesh->material->color.alpha);
				glActiveTexture(GL_TEXTURE0);
				if (mesh->material->colorTextureID == 0) {
					glBindTexture(GL_TEXTURE_2D, self->nullMaterial->colorTextureID);
				} else {
					glBindTexture(GL_TEXTURE_2D, mesh->material->colorTextureID);
				}
				glActiveTexture(GL_TEXTURE1);
				if (mesh->material->normalTextureID == 0) {
					glBindTexture(GL_TEXTURE_2D, self->nullMaterial->normalTextureID);
				} else {
					glBindTexture(GL_TEXTURE_2D, mesh->material->normalTextureID);
				}
			}
			
			glBindVertexArray(mesh->vertexBuffer->vaoID);
			glDrawElements(mesh->primitiveType, mesh->vertexBuffer->indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
			
			break;
		}
		case RENDERABLE_SPRITE: {
			// TODO: Batch by atlas
			SpriteRenderable * sprite = (SpriteRenderable *) renderable;
			
			glUseProgram(self->shaderSprite->programID);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(self->shaderSprite, "projectionTransform"), 1, GL_FALSE, self->projectionMatrix.m);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(self->shaderSprite, "viewTransform"), 1, GL_FALSE, self->viewMatrix.m);
			glUniformMatrix4fv(GLSLShader_getUniformLocation(self->shaderSprite, "modelTransform"), 1, GL_FALSE, sprite->transform.m);
			glUniform1i(GLSLShader_getUniformLocation(self->shaderSprite, "colorTexture"), 0);
			glUniform4f(GLSLShader_getUniformLocation(self->shaderSprite, "foreColor"), 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4f(GLSLShader_getUniformLocation(self->shaderSprite, "backColor"), 0.0f, 0.0f, 0.0f, 1.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, sprite->atlas->textureID);
			
			glBindVertexArray(sprite->vaoID);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
