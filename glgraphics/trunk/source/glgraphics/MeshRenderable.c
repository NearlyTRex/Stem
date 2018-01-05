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

#include "glgraphics/MeshRenderable.h"
#include <stdlib.h>

#define SUPERCLASS Renderable

MeshRenderable * MeshRenderable_create(VertexBuffer * vertexBuffer, Material * material, AnimationState * animationState, Matrix4x4f transform) {
	stemobject_create_implementation(MeshRenderable, init, vertexBuffer, material, animationState, transform)
}

bool MeshRenderable_init(MeshRenderable * self, VertexBuffer * vertexBuffer, Material * material, AnimationState * animationState, Matrix4x4f transform) {
	call_super(init, self, RENDERABLE_MESH);
	self->dispose = MeshRenderable_dispose;
	
	self->vertexBuffer = vertexBuffer;
	self->transform = transform;
	self->material = material;
	self->animationState = animationState;
	
	return true;
}

void MeshRenderable_dispose(MeshRenderable * self) {
	// Decrease reference count for vertexBuffer, material, and animationState?
	call_super(dispose, self);
}
