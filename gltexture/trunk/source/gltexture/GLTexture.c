/*
  Copyright (c) 2013 Alex Diener
  
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

#include "gltexture/GLTexture.h"
#include "glgraphics/GLGraphics.h"
#include <string.h>

#define SUPERCLASS StemObject

GLTexture * GLTexture_create(GLenum bitmapDataFormat,
                             GLenum bitmapDataType,
                             GLenum minFilter,
                             GLenum magFilter,
                             GLenum wrapS,
                             GLenum wrapT,
                             enum GLTextureAutoBlendMode autoBlendMode,
                             bool autoMipmap,
                             bool anisotropicFilter) {
	stemobject_create_implementation(GLTexture, init, bitmapDataFormat, bitmapDataType, minFilter, magFilter, wrapS, wrapT, autoBlendMode, autoMipmap, anisotropicFilter)
}

void GLTexture_init(GLTexture * self,
                    GLenum bitmapDataFormat,
                    GLenum bitmapDataType,
                    GLenum minFilter,
                    GLenum magFilter,
                    GLenum wrapS,
                    GLenum wrapT,
                    enum GLTextureAutoBlendMode autoBlendMode,
                    bool autoMipmap,
                    bool anisotropicFilter) {
	call_super(init, self);
	
	self->imageName = NULL;
	self->imageNameAllocated = false;
	self->textureName = 0;
	self->bitmapDataFormat = bitmapDataFormat;
	self->bitmapDataType = bitmapDataType;
	self->minFilter = minFilter;
	self->magFilter = magFilter;
	self->wrapS = wrapS;
	self->wrapT = wrapT;
	self->autoBlendMode = autoBlendMode;
	self->autoMipmap = autoMipmap;
	self->anisotropicFilter = anisotropicFilter;
	self->dispose = GLTexture_dispose;
	self->setImage = GLTexture_setImage;
	self->updateImage = GLTexture_updateImage;
	self->updateTexParams = GLTexture_updateTexParams;
	self->activate = GLTexture_activate;
	self->deactivate = GLTexture_deactivate;
}

void GLTexture_dispose(GLTexture * self) {
	if (self->imageNameAllocated) {
		free(self->imageName);
	}
	if (self->textureName != 0) {
		glDeleteTextures(1, &self->textureName);
	}
	call_super(dispose, self);
}

GLTexture * GLTexture_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	GLTexture * self;
	
	self = malloc(sizeof(GLTexture));
	if (!GLTexture_loadSerializedData(self, context)) {
		free(self);
		return NULL;
	}
	self->protected_ivar(allocated) = true;
	
	return self;
}

bool GLTexture_loadSerializedData(GLTexture * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	const char * imageName;
	GLenum bitmapDataFormat;
	GLenum bitmapDataType;
	GLenum minFilter;
	GLenum magFilter;
	GLenum wrapS;
	GLenum wrapT;
	enum GLTextureAutoBlendMode autoBlendMode;
	bool autoMipmap;
	bool anisotropicFilter;
	
	context->beginStructure(context, "gltexture");
	formatVersion     = context->readUInt16(context, "format_version");
	if (context->status == SERIALIZATION_ERROR_OK && formatVersion > GLTEXTURE_SERIALIZATION_FORMAT_VERSION) {
		return false;
	}
	imageName         = context->readString(context, "image_name");
	bitmapDataFormat  = context->readEnumeration(context, "bitmap_data_format", enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
	bitmapDataType    = context->readEnumeration(context, "bitmap_data_type", enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
	minFilter         = context->readEnumeration(context, "min_filter", enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
	magFilter         = context->readEnumeration(context, "mag_filter", enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
	wrapS             = context->readEnumeration(context, "wrap_s", enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	wrapT             = context->readEnumeration(context, "wrap_t", enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	autoBlendMode     = context->readEnumeration(context, "auto_blend_mode", enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
	autoMipmap        = context->readBoolean(context, "auto_mipmap");
	anisotropicFilter = context->readBoolean(context, "anisotropic_filter");
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	
	GLTexture_init(self, bitmapDataFormat, bitmapDataType, minFilter, magFilter, wrapS, wrapT, autoBlendMode, autoMipmap, anisotropicFilter);
	self->imageName = malloc(strlen(imageName) + 1);
	strcpy(self->imageName, imageName);
	self->imageNameAllocated = true;
	return true;
}

void GLTexture_serialize(GLTexture * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	
	context->beginStructure(context, "gltexture");
	context->writeUInt16(context, "format_version", GLTEXTURE_SERIALIZATION_FORMAT_VERSION);
	context->writeString(context, "image_name", self->imageName);
	context->writeEnumeration(context, "bitmap_data_format", self->bitmapDataFormat, enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
	context->writeEnumeration(context, "bitmap_data_type", self->bitmapDataType, enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
	context->writeEnumeration(context, "min_filter", self->minFilter, enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
	context->writeEnumeration(context, "mag_filter", self->magFilter, enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
	context->writeEnumeration(context, "wrap_s", self->wrapS, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->writeEnumeration(context, "wrap_t", self->wrapT, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->writeEnumeration(context, "auto_blend_mode", self->autoBlendMode, enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
	context->writeBoolean(context, "auto_mipmap", self->autoMipmap);
	context->writeBoolean(context, "anisotropic_filter", self->anisotropicFilter);
	context->endStructure(context);
}

void GLTexture_setImage(GLTexture * self, GLint mipmapLevel, GLsizei width, GLsizei height, unsigned int bytesPerRow, void * bitmapData) {
	if (self->textureName == 0) {
		glGenTextures(1, &self->textureName);
	}
	self->pixelWidth = width;
	self->pixelHeight = height;
	glBindTexture(GL_TEXTURE_2D, self->textureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, self->wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, self->wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, self->magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, self->minFilter);
	
#if defined(GL_EXT_texture_filter_anisotropic) && GL_EXT_texture_filter_anisotropic
	if (self->anisotropicFilter) {
		GLfloat maxAnisotropy;
		
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	}
#endif
	
	if (bytesPerRow % 4 == 0) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	} else if (bytesPerRow % 2 == 0) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	} else {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
	
#if !TARGET_OPENGL_ES
	if (self->autoMipmap) {
		switch (GLGraphics_getOpenGLAPIVersion()) {
			case GL_API_VERSION_DESKTOP_1:
			case GL_API_VERSION_DESKTOP_2:
			case GL_API_VERSION_DESKTOP_3:
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
				break;
			default:
				break;
		}
	}
#endif
	
	glTexImage2D(GL_TEXTURE_2D, mipmapLevel, self->bitmapDataFormat, width, height, 0, self->bitmapDataFormat, self->bitmapDataType, bitmapData);
	
#if TARGET_OPENGL_ES
	if (self->autoMipmap) {
		switch (GLGraphics_getOpenGLAPIVersion()) {
			case GL_API_VERSION_ES1:
				glGenerateMipmapOES(GL_TEXTURE_2D);
				break;
				
			case GL_API_VERSION_ES2:
				glGenerateMipmap(GL_TEXTURE_2D);
				break;
				
			default:
				break;
		}
	}
#endif

	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture_updateImage(GLTexture * self, GLint mipmapLevel, GLint x, GLint y, GLsizei width, GLsizei height, unsigned int bytesPerRow, void * bitmapData) {
	if (self->textureName != 0) {
		glBindTexture(GL_TEXTURE_2D, self->textureName);
		if (bytesPerRow % 4 == 0) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		} else if (bytesPerRow % 2 == 0) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
		} else {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
		glTexSubImage2D(GL_TEXTURE_2D, mipmapLevel, x, y, width, height, self->bitmapDataFormat, self->bitmapDataType, bitmapData);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GLTexture_updateTexParams(GLTexture * self) {
	if (self->textureName != 0) {
		glBindTexture(GL_TEXTURE_2D, self->textureName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, self->wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, self->wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, self->magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, self->minFilter);
		
#if defined(GL_EXT_texture_filter_anisotropic) && GL_EXT_texture_filter_anisotropic
		if (self->anisotropicFilter) {
			GLfloat maxAnisotropy;
			
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
		} else {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
		}
#endif
	}
}

void GLTexture_activate(GLTexture * self) {
	switch (self->autoBlendMode) {
		case AUTO_BLEND_MODE_NONE:
			break;
		case AUTO_BLEND_MODE_OPAQUE:
			glDisable(GL_BLEND);
			break;
		case AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED:
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			break;
	}
	if (GLGraphics_getOpenGLAPIVersion() != GL_API_VERSION_ES2) {
		glEnable(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, self->textureName);
}

void GLTexture_deactivate(GLTexture * self) {
	glBindTexture(GL_TEXTURE_2D, 0);
	if (GLGraphics_getOpenGLAPIVersion() != GL_API_VERSION_ES2) {
		glDisable(GL_TEXTURE_2D);
	}
	switch (self->autoBlendMode) {
		case AUTO_BLEND_MODE_NONE:
		case AUTO_BLEND_MODE_OPAQUE:
			break;
		case AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED:
		case AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED:
			glDisable(GL_BLEND);
			break;
	}
}
