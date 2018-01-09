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

#include "glgraphics/Material.h"
#include <assert.h>
#include <stdlib.h>

#define SUPERCLASS StemObject

Material * Material_create(Color4f color, float specularity, float shininess, float emissiveness) {
	stemobject_create_implementation(Material, init, color, specularity, shininess, emissiveness)
}

bool Material_init(Material * self, Color4f color, float specularity, float shininess, float emissiveness) {
	call_super(init, self);
	self->dispose = Material_dispose;
	self->color = color;
	self->specularity = specularity;
	self->shininess = shininess;
	self->emissiveness = emissiveness;
	self->colorTextureID = 0;
	self->normalTextureID = 0;
	return true;
}

void Material_dispose(Material * self) {
	glDeleteTextures(1, &self->colorTextureID);
	glDeleteTextures(1, &self->normalTextureID);
	call_super(dispose, self);
}

void createTexture(GLuint * textureID, bool magnifyNearest, unsigned int width, unsigned int height, void * bitmapData) {
	GLfloat maxAnisotropy;
	
	assert(*textureID == 0);
	glGenTextures(1, textureID);
	
	glBindTexture(GL_TEXTURE_2D, *textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (magnifyNearest) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmapData);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Material_setColorTexture(Material * self, bool magnifyNearest, unsigned int width, unsigned int height, void * bitmapData) {
	createTexture(&self->colorTextureID, magnifyNearest, width, height, bitmapData);
}

void Material_setNormalTexture(Material * self, bool magnifyNearest, unsigned int width, unsigned int height, void * bitmapData) {
	createTexture(&self->normalTextureID, magnifyNearest, width, height, bitmapData);
}
