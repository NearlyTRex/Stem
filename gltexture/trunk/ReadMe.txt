GLTexture provides support for uploading and binding OpenGL textures. Image loading and GL rendering are not provided; you must use other stem libraries or your own code.

A GLTexture object can be created directly, or loaded from a serialized representation using the serialization interface. After initial creation, you must upload an image with GLTexture_setImage(). You can then use GLTexture_activate to bind the texture, and GLTexture_deactivate to unbind it.

For performance and/or fine-grained control over texturing, you may not want to use activate() and deactivate(). They're provided only for convenience, and have some properties that may be undesirable in large applications; for example, if autoBlendMode is set to anything other than AUTO_BLEND_MODE_NONE, GL_BLEND will be enabled and/or disabled every time a texture is activated or deactivated. Accessing the textureName directly to bind it yourself is perfectly normal usage of the GLTexture API.

GLTexture serialization schema:
structure "gltexture" {
	uint16 "format_version" // GLTEXTURE_SERIALIZATION_FORMAT_VERSION
	string "image_name"
	enumeration "bitmap_data_format", GL_ALPHA, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA
	enumeration "bitmap_data_type", GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_5_5_5_1
	enumeration "min_filter", GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
	enumeration "mag_filter", GL_NEAREST, GL_LINEAR
	enumeration "wrap_s", GL_CLAMP_TO_EDGE, GL_REPEAT
	enumeration "wrap_t", GL_CLAMP_TO_EDGE, GL_REPEAT
	enumeration "auto_blend_mode", AUTO_BLEND_MODE_NONE, AUTO_BLEND_MODE_OPAQUE, AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED
	boolean "auto_mipmap"
	boolean "anisotropic_filter"
}

GLTextureAtlas serialization schema:
structure "gltextureatlas" {
	uint16 "format_version" // GLTEXTUREATLAS_SERIALIZATION_FORMAT_VERSION
	string "texture_name"
	dictionary "entries" {
		structure "entry_name" {
			float "left"
			float "right"
			float "bottom"
			float "top"
		}
	}
}
