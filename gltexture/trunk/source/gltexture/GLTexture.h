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

/* For performance and/or fine-grained control over texturing, you may not want to use active() and deactivate().
   They're provided only for convenience, and have some properties that may be undesireable in large applications;
   for example, if autoBlendMode is set to anything other than AUTO_BLEND_MODE_NONE, GL_BLEND will be enabled and/or
   disabled every time a texture is activated or deactivated. Accessing the textureName directly to bind it yourself
   is perfectly normal usage of the GLTexture API. */

#define GLTexture_structContents \
	StemObject_structContents \
	\
	bool imageNameAllocated; \
	char * imageName; \
	GLuint textureName; \
	GLenum bitmapDataFormat; \
	GLenum bitmapDataType; \
	GLenum minFilter; \
	GLenum magFilter; \
	GLenum wrapS; \
	GLenum wrapT; \
	enum GLTextureAutoBlendMode autoBlendMode; \
	bool autoMipmap; \
	\
	void (* setImage)(void * self, GLint mipmapLevel, GLsizei width, GLsizei height, void * bitmapData); \
	void (* updateImage)(void * self, GLint mipmapLevel, GLint x, GLint y, GLsizei width, GLsizei height, void * bitmapData); \
	void (* activate)(void * self); \
	void (* deactivate)(void * self);

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
                             bool autoMipmap);
void GLTexture_init(GLTexture * self,
                    GLenum bitmapDataFormat,
                    GLenum bitmapDataType,
                    GLenum minFilter,
                    GLenum magFilter,
                    GLenum wrapS,
                    GLenum wrapT,
                    enum GLTextureAutoBlendMode autoBlendMode,
                    bool autoMipmap);
void GLTexture_dispose(void * selfPtr);

GLTexture * GLTexture_deserialize(DeserializationContext * context);
bool GLTexture_loadSerializedData(GLTexture * self, DeserializationContext * context);
void GLTexture_serialize(GLTexture * self, SerializationContext * context);

void GLTexture_setImage(void * self, GLint mipmapLevel, GLsizei width, GLsizei height, void * bitmapData);
void GLTexture_updateImage(void * self, GLint mipmapLevel, GLint x, GLint y, GLsizei width, GLsizei height, void * bitmapData);

void GLTexture_activate(void * selfPtr);
void GLTexture_deactivate(void * selfPtr);

#endif
