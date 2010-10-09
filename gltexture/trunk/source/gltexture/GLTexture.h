/*
  Copyright (c) 2010 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __GL_TEXTURE_H__
#define __GL_TEXTURE_H__

#include "stemobject/StemObject.h"
#include "glgraphics/GLIncludes.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"
#include <stdlib.h>

typedef struct GLTexture GLTexture;

enum GLTextureAutoBlendMode {
	AUTO_BLEND_MODE_NONE,
	AUTO_BLEND_MODE_OPAQUE,
	AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED,
	AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED
};

#define GLTexture_structContents \
	StemObject_structContents \
	\
	bool imageNameAllocated; \
	char * imageName; \
	GLuint textureName; \
	GLsizei pixelWidth; \
	GLsizei pixelHeight; \
	GLenum bitmapDataFormat; \
	GLenum bitmapDataType; \
	GLenum minFilter; \
	GLenum magFilter; \
	GLenum wrapS; \
	GLenum wrapT; \
	enum GLTextureAutoBlendMode autoBlendMode; \
	bool autoMipmap; \
	bool anisotropicFilter; \
	\
	void (* setImage)(compat_type(GLTexture *) self, GLint mipmapLevel, GLsizei width, GLsizei height, unsigned int bytesPerRow, void * bitmapData); \
	void (* updateImage)(compat_type(GLTexture *) self, GLint mipmapLevel, GLint x, GLint y, GLsizei width, GLsizei height, unsigned int bytesPerRow, void * bitmapData); \
	void (* updateTexParams)(compat_type(GLTexture *) self); \
	void (* activate)(compat_type(GLTexture *) self); \
	void (* deactivate)(compat_type(GLTexture *) self);

struct GLTexture {
	GLTexture_structContents
};

GLTexture * GLTexture_create(GLenum bitmapDataFormat,
                             GLenum bitmapDataType,
                             GLenum minFilter,
                             GLenum magFilter,
                             GLenum wrapS,
                             GLenum wrapT,
                             enum GLTextureAutoBlendMode autoBlendMode,
                             bool autoMipmap,
                             bool anisotropicFilter);
void GLTexture_init(compat_type(GLTexture *) self,
                    GLenum bitmapDataFormat,
                    GLenum bitmapDataType,
                    GLenum minFilter,
                    GLenum magFilter,
                    GLenum wrapS,
                    GLenum wrapT,
                    enum GLTextureAutoBlendMode autoBlendMode,
                    bool autoMipmap,
                    bool anisotropicFilter);
void GLTexture_dispose(compat_type(GLTexture *) selfPtr);

GLTexture * GLTexture_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool GLTexture_loadSerializedData(compat_type(GLTexture *) selfPtr, compat_type(DeserializationContext *) deserializationContext);
void GLTexture_serialize(compat_type(GLTexture *) selfPtr, compat_type(SerializationContext *) serializationContext);

void GLTexture_setImage(compat_type(GLTexture *) selfPtr, GLint mipmapLevel, GLsizei width, GLsizei height, unsigned int bytesPerRow, void * bitmapData);
void GLTexture_updateImage(compat_type(GLTexture *) selfPtr, GLint mipmapLevel, GLint x, GLint y, GLsizei width, GLsizei height, unsigned int bytesPerRow, void * bitmapData);
/* Applicable to minFilter, magFilter, wrapS, wrapT, and anisotropicFilter */
void GLTexture_updateTexParams(compat_type(GLTexture *) selfPtr);

void GLTexture_activate(compat_type(GLTexture *) selfPtr);
void GLTexture_deactivate(compat_type(GLTexture *) selfPtr);

#endif
