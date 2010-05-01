#include "gltexture/GLTexture.h"

GLTexture * GLTexture_create(GLenum bitmapDataFormat,
                             GLenum bitmapDataType,
                             GLenum minFilter,
                             GLenum magFilter,
                             GLenum wrapS,
                             GLenum wrapT,
                             enum GLTextureAutoBlendMode autoBlendMode,
                             bool autoMipmap) {
	GLTexture * self;
	
	self = malloc(sizeof(GLTexture));
	GLTexture_init(self, bitmapDataFormat, bitmapDataType, minFilter, magFilter, wrapS, wrapT, autoBlendMode, autoMipmap);
	self->allocated = true;
	
	return self;
}

void GLTexture_init(GLTexture * self,
                    GLenum bitmapDataFormat,
                    GLenum bitmapDataType,
                    GLenum minFilter,
                    GLenum magFilter,
                    GLenum wrapS,
                    GLenum wrapT,
                    enum GLTextureAutoBlendMode autoBlendMode,
                    bool autoMipmap) {
	StemObject_init((StemObject *) self);
	
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
	self->dispose = GLTexture_dispose;
	self->setImage = GLTexture_setImage;
	self->updateImage = GLTexture_updateImage;
	self->activate = GLTexture_activate;
	self->deactivate = GLTexture_deactivate;
}

void GLTexture_dispose(void * selfPtr) {
	GLTexture * self = selfPtr;
	
	if (self->imageNameAllocated) {
		free(self->imageName);
	}
	if (self->textureName != 0) {
		glDeleteTextures(1, &self->textureName);
	}
	StemObject_dispose((StemObject *) self);
}

GLTexture * GLTexture_deserialize(DeserializationContext * context) {
	GLTexture * self;
	
	self = malloc(sizeof(GLTexture));
	if (!GLTexture_loadSerializedData(self, context)) {
		free(self);
		return NULL;
	}
	self->allocated = true;
	
	return self;
}

bool GLTexture_loadSerializedData(GLTexture * self, DeserializationContext * context) {
	char * imageName;
	GLenum bitmapDataFormat;
	GLenum bitmapDataType;
	GLenum minFilter;
	GLenum magFilter;
	GLenum wrapS;
	GLenum wrapT;
	enum GLTextureAutoBlendMode autoBlendMode;
	bool autoMipmap;
	
	context->beginStructure(context, "gltexture");
	imageName        = context->readString(context, "imageName");
	bitmapDataFormat = context->readEnumeration(context, "bitmapDataFormat", enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
	bitmapDataType   = context->readEnumeration(context, "bitmapDataType", enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
	minFilter        = context->readEnumeration(context, "minFilter", enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
	magFilter        = context->readEnumeration(context, "magFilter", enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
	wrapS            = context->readEnumeration(context, "wrapS", enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	wrapT            = context->readEnumeration(context, "wrapT", enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	autoBlendMode    = context->readEnumeration(context, "autoBlendMode", enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
	autoMipmap       = context->readBoolean(context, "autoMipmap");
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	
	GLTexture_init(self, bitmapDataFormat, bitmapDataType, minFilter, magFilter, wrapS, wrapT, autoBlendMode, autoMipmap);
	self->imageName = imageName;
	return true;
}

void GLTexture_serialize(GLTexture * self, SerializationContext * context) {
	context->beginStructure(context, "gltexture");
	context->writeString(context, "imageName", self->imageName);
	context->writeEnumeration(context, "bitmapDataFormat", self->bitmapDataFormat, enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
	context->writeEnumeration(context, "bitmapDataType", self->bitmapDataType, enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
	context->writeEnumeration(context, "minFilter", self->minFilter, enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
	context->writeEnumeration(context, "magFilter", self->magFilter, enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
	context->writeEnumeration(context, "wrapS", self->wrapS, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->writeEnumeration(context, "wrapT", self->wrapT, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->writeEnumeration(context, "autoBlendMode", self->autoBlendMode, enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
	context->writeBoolean(context, "autoMipmap", self->autoMipmap);
	context->endStructure(context);
}

void GLTexture_setImage(void * self, GLint mipmapLevel, GLsizei width, GLsizei height, void * bitmapData) {
}

void GLTexture_updateImage(void * self, GLint mipmapLevel, GLint x, GLint y, GLsizei width, GLsizei height, void * bitmapData) {
}

void GLTexture_activate(void * selfPtr) {
}

void GLTexture_deactivate(void * selfPtr) {
}
