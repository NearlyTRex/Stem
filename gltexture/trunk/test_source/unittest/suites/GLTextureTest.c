#include "gltexture/GLTexture.h"
#include "unittest/framework/TestSuite.h"
#include "unittest/TestDeserializationContext.h"
#include "unittest/TestSerializationContext.h"

static void testInit() {
	GLTexture * texturePtr, texture;
	
	texturePtr = GLTexture_create(GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE, AUTO_BLEND_MODE_NONE, false);
	TestCase_assert(texturePtr != NULL, "GLTexture_create unexpectedly returned NULL");
	TestCase_assert(texturePtr->imageName == NULL, "Expected NULL but got %p", texturePtr->imageName);
	TestCase_assert(texturePtr->textureName == 0, "Expected 0 but got %u", texturePtr->textureName);
	TestCase_assert(texturePtr->bitmapDataFormat == GL_RGBA, "Expected 0x%04X but got 0x%04X", GL_RGBA, texturePtr->bitmapDataFormat);
	TestCase_assert(texturePtr->bitmapDataType == GL_UNSIGNED_BYTE, "Expected 0x%04X but got 0x%04X", GL_UNSIGNED_BYTE, texturePtr->bitmapDataType);
	TestCase_assert(texturePtr->minFilter == GL_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST, texturePtr->minFilter);
	TestCase_assert(texturePtr->magFilter == GL_LINEAR, "Expected 0x%04X but got 0x%04X", GL_LINEAR, texturePtr->magFilter);
	TestCase_assert(texturePtr->wrapS == GL_REPEAT, "Expected 0x%04X but got 0x%04X", GL_REPEAT, texturePtr->wrapS);
	TestCase_assert(texturePtr->wrapT == GL_CLAMP_TO_EDGE, "Expected 0x%04X but got 0x%04X", GL_CLAMP_TO_EDGE, texturePtr->wrapT);
	TestCase_assert(texturePtr->autoBlendMode == AUTO_BLEND_MODE_NONE, "Expected %d but got %d", AUTO_BLEND_MODE_NONE, texturePtr->autoBlendMode);
	TestCase_assert(!texturePtr->autoMipmap, "Expected false but got true");
	TestCase_assert(texturePtr->dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->setImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->updateImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->activate != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->deactivate != NULL, "Method unexpectedly NULL");
	texturePtr->dispose(texturePtr);
	
	texturePtr = GLTexture_create(GL_RGB, GL_UNSIGNED_SHORT_4_4_4_4, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_REPEAT, AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, true);
	TestCase_assert(texturePtr != NULL, "GLTexture_create unexpectedly returned NULL");
	TestCase_assert(texturePtr->imageName == NULL, "Expected NULL but got %p", texturePtr->imageName);
	TestCase_assert(texturePtr->textureName == 0, "Expected 0 but got %u", texturePtr->textureName);
	TestCase_assert(texturePtr->bitmapDataFormat == GL_RGB, "Expected 0x%04X but got 0x%04X", GL_RGB, texturePtr->bitmapDataFormat);
	TestCase_assert(texturePtr->bitmapDataType == GL_UNSIGNED_SHORT_4_4_4_4, "Expected 0x%04X but got 0x%04X", GL_UNSIGNED_SHORT_4_4_4_4, texturePtr->bitmapDataType);
	TestCase_assert(texturePtr->minFilter == GL_NEAREST_MIPMAP_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST_MIPMAP_NEAREST, texturePtr->minFilter);
	TestCase_assert(texturePtr->magFilter == GL_NEAREST, "Expected 0x%04X but got 0x%04X", GL_LINEAR_MIPMAP_LINEAR, texturePtr->magFilter);
	TestCase_assert(texturePtr->wrapS == GL_CLAMP_TO_EDGE, "Expected 0x%04X but got 0x%04X", GL_CLAMP_TO_EDGE, texturePtr->wrapS);
	TestCase_assert(texturePtr->wrapT == GL_REPEAT, "Expected 0x%04X but got 0x%04X", GL_REPEAT, texturePtr->wrapT);
	TestCase_assert(texturePtr->autoBlendMode == AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, "Expected %d but got %d", AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, texturePtr->autoBlendMode);
	TestCase_assert(texturePtr->autoMipmap, "Expected true but got false");
	TestCase_assert(texturePtr->dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->setImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->updateImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->activate != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->deactivate != NULL, "Method unexpectedly NULL");
	texturePtr->dispose(texturePtr);
	
	GLTexture_init(&texture, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE, AUTO_BLEND_MODE_NONE, false);
	TestCase_assert(texture.imageName == NULL, "Expected NULL but got %p", texture.imageName);
	TestCase_assert(texture.textureName == 0, "Expected 0 but got %u", texture.textureName);
	TestCase_assert(texture.bitmapDataFormat == GL_RGBA, "Expected 0x%04X but got 0x%04X", GL_RGBA, texture.bitmapDataFormat);
	TestCase_assert(texture.bitmapDataType == GL_UNSIGNED_BYTE, "Expected 0x%04X but got 0x%04X", GL_UNSIGNED_BYTE, texture.bitmapDataType);
	TestCase_assert(texture.minFilter == GL_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST, texture.minFilter);
	TestCase_assert(texture.magFilter == GL_LINEAR, "Expected 0x%04X but got 0x%04X", GL_LINEAR, texture.magFilter);
	TestCase_assert(texture.wrapS == GL_REPEAT, "Expected 0x%04X but got 0x%04X", GL_REPEAT, texture.wrapS);
	TestCase_assert(texture.wrapT == GL_CLAMP_TO_EDGE, "Expected 0x%04X but got 0x%04X", GL_CLAMP_TO_EDGE, texture.wrapT);
	TestCase_assert(texture.autoBlendMode == AUTO_BLEND_MODE_NONE, "Expected %d but got %d", AUTO_BLEND_MODE_NONE, texture.autoBlendMode);
	TestCase_assert(!texture.autoMipmap, "Expected false but got true");
	TestCase_assert(texture.dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.setImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.updateImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.activate != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.deactivate != NULL, "Method unexpectedly NULL");
	texture.dispose(&texture);
	
	GLTexture_init(&texture, GL_RGB, GL_UNSIGNED_SHORT_4_4_4_4, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_REPEAT, AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, true);
	TestCase_assert(texture.imageName == NULL, "Expected NULL but got %p", texture.imageName);
	TestCase_assert(texture.textureName == 0, "Expected 0 but got %u", texture.textureName);
	TestCase_assert(texture.bitmapDataFormat == GL_RGB, "Expected 0x%04X but got 0x%04X", GL_RGB, texture.bitmapDataFormat);
	TestCase_assert(texture.bitmapDataType == GL_UNSIGNED_SHORT_4_4_4_4, "Expected 0x%04X but got 0x%04X", GL_UNSIGNED_SHORT_4_4_4_4, texture.bitmapDataType);
	TestCase_assert(texture.minFilter == GL_NEAREST_MIPMAP_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST_MIPMAP_NEAREST, texture.minFilter);
	TestCase_assert(texture.magFilter == GL_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST, texture.magFilter);
	TestCase_assert(texture.wrapS == GL_CLAMP_TO_EDGE, "Expected 0x%04X but got 0x%04X", GL_CLAMP_TO_EDGE, texture.wrapS);
	TestCase_assert(texture.wrapT == GL_REPEAT, "Expected 0x%04X but got 0x%04X", GL_REPEAT, texture.wrapT);
	TestCase_assert(texture.autoBlendMode == AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, "Expected %d but got %d", AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, texture.autoBlendMode);
	TestCase_assert(texture.autoMipmap, "Expected true but got false");
	TestCase_assert(texture.dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.setImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.updateImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.activate != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.deactivate != NULL, "Method unexpectedly NULL");
	texture.dispose(&texture);
}

static void testDeserialization() {
	GLTexture * texturePtr, texture;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	bool success;
	unsigned int failIndex;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "gltexture");
	context->expectCall(context, context->readString, "imageName", "foo");
	context->expectCall(context, context->readEnumeration, "bitmapDataFormat", GL_RGBA, enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
	context->expectCall(context, context->readEnumeration, "bitmapDataType", GL_UNSIGNED_BYTE, enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
	context->expectCall(context, context->readEnumeration, "minFilter", GL_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
	context->expectCall(context, context->readEnumeration, "magFilter", GL_LINEAR, enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
	context->expectCall(context, context->readEnumeration, "wrapS", GL_REPEAT, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->readEnumeration, "wrapT", GL_CLAMP_TO_EDGE, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->readEnumeration, "autoBlendMode", AUTO_BLEND_MODE_NONE, enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
	context->expectCall(context, context->readBoolean, "autoMipmap", false);
	context->expectCall(context, context->endStructure);
	
	texturePtr = GLTexture_deserialize((DeserializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(texturePtr != NULL, "GLTexture_deserialize unexpectedly returned NULL");
	TestCase_assert(!strcmp(texturePtr->imageName, "foo"), "Expected \"foo\" but got \"%s\"", texturePtr->imageName);
	TestCase_assert(texturePtr->textureName == 0, "Expected 0 but got %u", texturePtr->textureName);
	TestCase_assert(texturePtr->bitmapDataFormat == GL_RGBA, "Expected 0x%04X but got 0x%04X", GL_RGBA, texturePtr->bitmapDataFormat);
	TestCase_assert(texturePtr->bitmapDataType == GL_UNSIGNED_BYTE, "Expected 0x%04X but got 0x%04X", GL_UNSIGNED_BYTE, texturePtr->bitmapDataType);
	TestCase_assert(texturePtr->minFilter == GL_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST, texturePtr->minFilter);
	TestCase_assert(texturePtr->magFilter == GL_LINEAR, "Expected 0x%04X but got 0x%04X", GL_LINEAR, texturePtr->magFilter);
	TestCase_assert(texturePtr->wrapS == GL_REPEAT, "Expected 0x%04X but got 0x%04X", GL_REPEAT, texturePtr->wrapS);
	TestCase_assert(texturePtr->wrapT == GL_CLAMP_TO_EDGE, "Expected 0x%04X but got 0x%04X", GL_CLAMP_TO_EDGE, texturePtr->wrapT);
	TestCase_assert(texturePtr->autoBlendMode == AUTO_BLEND_MODE_NONE, "Expected %d but got %d", AUTO_BLEND_MODE_NONE, texturePtr->autoBlendMode);
	TestCase_assert(!texturePtr->autoMipmap, "Expected false but got true");
	TestCase_assert(texturePtr->dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->setImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->updateImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->activate != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->deactivate != NULL, "Method unexpectedly NULL");
	texturePtr->dispose(texturePtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "gltexture");
	context->expectCall(context, context->readString, "imageName", "bar");
	context->expectCall(context, context->readEnumeration, "bitmapDataFormat", GL_RGB, enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
	context->expectCall(context, context->readEnumeration, "bitmapDataType", GL_UNSIGNED_SHORT_4_4_4_4, enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
	context->expectCall(context, context->readEnumeration, "minFilter", GL_NEAREST_MIPMAP_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
	context->expectCall(context, context->readEnumeration, "magFilter", GL_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
	context->expectCall(context, context->readEnumeration, "wrapS", GL_CLAMP_TO_EDGE, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->readEnumeration, "wrapT", GL_REPEAT, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->readEnumeration, "autoBlendMode", AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
	context->expectCall(context, context->readBoolean, "autoMipmap", true);
	context->expectCall(context, context->endStructure);
	
	texturePtr = GLTexture_deserialize((DeserializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(texturePtr != NULL, "GLTexture_deserialize unexpectedly returned NULL");
	TestCase_assert(!strcmp(texturePtr->imageName, "bar"), "Expected \"bar\" but got \"%s\"", texturePtr->imageName);
	TestCase_assert(texturePtr->textureName == 0, "Expected 0 but got %u", texturePtr->textureName);
	TestCase_assert(texturePtr->bitmapDataFormat == GL_RGB, "Expected 0x%04X but got 0x%04X", GL_RGB, texturePtr->bitmapDataFormat);
	TestCase_assert(texturePtr->bitmapDataType == GL_UNSIGNED_SHORT_4_4_4_4, "Expected 0x%04X but got 0x%04X", GL_UNSIGNED_SHORT_4_4_4_4, texturePtr->bitmapDataType);
	TestCase_assert(texturePtr->minFilter == GL_NEAREST_MIPMAP_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST_MIPMAP_NEAREST, texturePtr->minFilter);
	TestCase_assert(texturePtr->magFilter == GL_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST, texturePtr->magFilter);
	TestCase_assert(texturePtr->wrapS == GL_CLAMP_TO_EDGE, "Expected 0x%04X but got 0x%04X", GL_CLAMP_TO_EDGE, texturePtr->wrapS);
	TestCase_assert(texturePtr->wrapT == GL_REPEAT, "Expected 0x%04X but got 0x%04X", GL_REPEAT, texturePtr->wrapT);
	TestCase_assert(texturePtr->autoBlendMode == AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, "Expected %d but got %d", AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, texturePtr->autoBlendMode);
	TestCase_assert(texturePtr->autoMipmap, "Expected true but got false");
	TestCase_assert(texturePtr->dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->setImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->updateImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->activate != NULL, "Method unexpectedly NULL");
	TestCase_assert(texturePtr->deactivate != NULL, "Method unexpectedly NULL");
	texturePtr->dispose(texturePtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "gltexture");
	context->expectCall(context, context->readString, "imageName", "foo");
	context->expectCall(context, context->readEnumeration, "bitmapDataFormat", GL_RGBA, enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
	context->expectCall(context, context->readEnumeration, "bitmapDataType", GL_UNSIGNED_BYTE, enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
	context->expectCall(context, context->readEnumeration, "minFilter", GL_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
	context->expectCall(context, context->readEnumeration, "magFilter", GL_LINEAR, enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
	context->expectCall(context, context->readEnumeration, "wrapS", GL_REPEAT, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->readEnumeration, "wrapT", GL_CLAMP_TO_EDGE, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->readEnumeration, "autoBlendMode", AUTO_BLEND_MODE_NONE, enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
	context->expectCall(context, context->readBoolean, "autoMipmap", false);
	context->expectCall(context, context->endStructure);
	
	success = GLTexture_loadSerializedData(&texture, (DeserializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(!strcmp(texture.imageName, "foo"), "Expected \"foo\" but got \"%s\"", texture.imageName);
	TestCase_assert(texture.textureName == 0, "Expected 0 but got %u", texture.textureName);
	TestCase_assert(texture.bitmapDataFormat == GL_RGBA, "Expected 0x%04X but got 0x%04X", GL_RGBA, texture.bitmapDataFormat);
	TestCase_assert(texture.bitmapDataType == GL_UNSIGNED_BYTE, "Expected 0x%04X but got 0x%04X", GL_UNSIGNED_BYTE, texture.bitmapDataType);
	TestCase_assert(texture.minFilter == GL_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST, texture.minFilter);
	TestCase_assert(texture.magFilter == GL_LINEAR, "Expected 0x%04X but got 0x%04X", GL_LINEAR, texture.magFilter);
	TestCase_assert(texture.wrapS == GL_REPEAT, "Expected 0x%04X but got 0x%04X", GL_REPEAT, texture.wrapS);
	TestCase_assert(texture.wrapT == GL_CLAMP_TO_EDGE, "Expected 0x%04X but got 0x%04X", GL_CLAMP_TO_EDGE, texture.wrapT);
	TestCase_assert(texture.autoBlendMode == AUTO_BLEND_MODE_NONE, "Expected %d but got %d", AUTO_BLEND_MODE_NONE, texture.autoBlendMode);
	TestCase_assert(!texture.autoMipmap, "Expected false but got true");
	TestCase_assert(texture.dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.setImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.updateImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.activate != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.deactivate != NULL, "Method unexpectedly NULL");
	texture.dispose(&texture);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "gltexture");
	context->expectCall(context, context->readString, "imageName", "bar");
	context->expectCall(context, context->readEnumeration, "bitmapDataFormat", GL_RGB, enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
	context->expectCall(context, context->readEnumeration, "bitmapDataType", GL_UNSIGNED_SHORT_4_4_4_4, enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
	context->expectCall(context, context->readEnumeration, "minFilter", GL_NEAREST_MIPMAP_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
	context->expectCall(context, context->readEnumeration, "magFilter", GL_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
	context->expectCall(context, context->readEnumeration, "wrapS", GL_CLAMP_TO_EDGE, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->readEnumeration, "wrapT", GL_REPEAT, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->readEnumeration, "autoBlendMode", AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
	context->expectCall(context, context->readBoolean, "autoMipmap", true);
	context->expectCall(context, context->endStructure);
	
	success = GLTexture_loadSerializedData(&texture, (DeserializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(!strcmp(texture.imageName, "bar"), "Expected \"bar\" but got \"%s\"", texture.imageName);
	TestCase_assert(texture.textureName == 0, "Expected 0 but got %u", texture.textureName);
	TestCase_assert(texture.bitmapDataFormat == GL_RGB, "Expected 0x%04X but got 0x%04X", GL_RGB, texture.bitmapDataFormat);
	TestCase_assert(texture.bitmapDataType == GL_UNSIGNED_SHORT_4_4_4_4, "Expected 0x%04X but got 0x%04X", GL_UNSIGNED_SHORT_4_4_4_4, texture.bitmapDataType);
	TestCase_assert(texture.minFilter == GL_NEAREST_MIPMAP_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST_MIPMAP_NEAREST, texture.minFilter);
	TestCase_assert(texture.magFilter == GL_NEAREST, "Expected 0x%04X but got 0x%04X", GL_NEAREST, texture.magFilter);
	TestCase_assert(texture.wrapS == GL_CLAMP_TO_EDGE, "Expected 0x%04X but got 0x%04X", GL_CLAMP_TO_EDGE, texture.wrapS);
	TestCase_assert(texture.wrapT == GL_REPEAT, "Expected 0x%04X but got 0x%04X", GL_REPEAT, texture.wrapT);
	TestCase_assert(texture.autoBlendMode == AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, "Expected %d but got %d", AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, texture.autoBlendMode);
	TestCase_assert(texture.autoMipmap, "Expected true but got false");
	TestCase_assert(texture.dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.setImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.updateImage != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.activate != NULL, "Method unexpectedly NULL");
	TestCase_assert(texture.deactivate != NULL, "Method unexpectedly NULL");
	texture.dispose(&texture);
	
	for (failIndex = 0; failIndex < 11; failIndex++) {
		context = TestDeserializationContext_create(&jmpEnv);
		if (setjmp(jmpEnv) != 0) {
			TestCase_assert(false, "%s", context->error);
		}
		
		context->expectCall(context, context->beginStructure, "gltexture");
		context->expectCall(context, context->readString, "imageName", "foo");
		context->expectCall(context, context->readEnumeration, "bitmapDataFormat", GL_RGBA, enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
		context->expectCall(context, context->readEnumeration, "bitmapDataType", GL_UNSIGNED_BYTE, enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
		context->expectCall(context, context->readEnumeration, "minFilter", GL_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
		context->expectCall(context, context->readEnumeration, "magFilter", GL_LINEAR, enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
		context->expectCall(context, context->readEnumeration, "wrapS", GL_REPEAT, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
		context->expectCall(context, context->readEnumeration, "wrapT", GL_CLAMP_TO_EDGE, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
		context->expectCall(context, context->readEnumeration, "autoBlendMode", AUTO_BLEND_MODE_NONE, enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
		context->expectCall(context, context->readBoolean, "autoMipmap", false);
		context->expectCall(context, context->endStructure);
		context->failNthCall(context, failIndex, 1);
		
		texturePtr = GLTexture_deserialize((DeserializationContext *) context);
		
		context->finish(context);
		context->dispose(context);
		
		TestCase_assert(texturePtr == NULL, "GLTexture_deserialize didn't return NULL when deserialization call %d failed", failIndex);
		
		context = TestDeserializationContext_create(&jmpEnv);
		if (setjmp(jmpEnv) != 0) {
			TestCase_assert(false, "%s", context->error);
		}
		
		context->expectCall(context, context->beginStructure, "gltexture");
		context->expectCall(context, context->readString, "imageName", "foo");
		context->expectCall(context, context->readEnumeration, "bitmapDataFormat", GL_RGBA, enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
		context->expectCall(context, context->readEnumeration, "bitmapDataType", GL_UNSIGNED_BYTE, enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
		context->expectCall(context, context->readEnumeration, "minFilter", GL_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
		context->expectCall(context, context->readEnumeration, "magFilter", GL_LINEAR, enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
		context->expectCall(context, context->readEnumeration, "wrapS", GL_REPEAT, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
		context->expectCall(context, context->readEnumeration, "wrapT", GL_CLAMP_TO_EDGE, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
		context->expectCall(context, context->readEnumeration, "autoBlendMode", AUTO_BLEND_MODE_NONE, enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
		context->expectCall(context, context->readBoolean, "autoMipmap", false);
		context->expectCall(context, context->endStructure);
		context->failNthCall(context, failIndex, 1);
		
		success = GLTexture_loadSerializedData(&texture, (DeserializationContext *) context);
		
		context->finish(context);
		context->dispose(context);
		
		TestCase_assert(!success, "GLTexture_loadSerializedData didn't return false when deserialization call %d failed", failIndex);
	}
}

static void testSerialization() {
	GLTexture texture;
	TestSerializationContext * context;
	jmp_buf jmpEnv;
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	GLTexture_init(&texture, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE, AUTO_BLEND_MODE_NONE, false);
	texture.imageName = "foo";
	
	context->expectCall(context, context->beginStructure, "gltexture");
	context->expectCall(context, context->writeString, "imageName", "foo");
	context->expectCall(context, context->writeEnumeration, "bitmapDataFormat", GL_RGBA, enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
	context->expectCall(context, context->writeEnumeration, "bitmapDataType", GL_UNSIGNED_BYTE, enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
	context->expectCall(context, context->writeEnumeration, "minFilter", GL_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
	context->expectCall(context, context->writeEnumeration, "magFilter", GL_LINEAR, enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
	context->expectCall(context, context->writeEnumeration, "wrapS", GL_REPEAT, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->writeEnumeration, "wrapT", GL_CLAMP_TO_EDGE, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->writeEnumeration, "autoBlendMode", AUTO_BLEND_MODE_NONE, enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
	context->expectCall(context, context->writeBoolean, "autoMipmap", false);
	context->expectCall(context, context->endStructure);
	
	GLTexture_serialize(&texture, (SerializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	texture.dispose(&texture);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	GLTexture_init(&texture, GL_RGB, GL_UNSIGNED_SHORT_4_4_4_4, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_REPEAT, AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, true);
	texture.imageName = "bar";
	
	context->expectCall(context, context->beginStructure, "gltexture");
	context->expectCall(context, context->writeString, "imageName", "bar");
	context->expectCall(context, context->writeEnumeration, "bitmapDataFormat", GL_RGB, enumKV(GL_ALPHA), enumKV(GL_LUMINANCE), enumKV(GL_LUMINANCE_ALPHA), enumKV(GL_RGB), enumKV(GL_RGBA), NULL);
	context->expectCall(context, context->writeEnumeration, "bitmapDataType", GL_UNSIGNED_SHORT_4_4_4_4, enumKV(GL_UNSIGNED_BYTE), enumKV(GL_UNSIGNED_SHORT_5_6_5), enumKV(GL_UNSIGNED_SHORT_4_4_4_4), enumKV(GL_UNSIGNED_SHORT_5_5_5_1), NULL);
	context->expectCall(context, context->writeEnumeration, "minFilter", GL_NEAREST_MIPMAP_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), enumKV(GL_NEAREST_MIPMAP_NEAREST), enumKV(GL_LINEAR_MIPMAP_NEAREST), enumKV(GL_NEAREST_MIPMAP_LINEAR), enumKV(GL_LINEAR_MIPMAP_LINEAR), NULL);
	context->expectCall(context, context->writeEnumeration, "magFilter", GL_NEAREST, enumKV(GL_NEAREST), enumKV(GL_LINEAR), NULL);
	context->expectCall(context, context->writeEnumeration, "wrapS", GL_CLAMP_TO_EDGE, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->writeEnumeration, "wrapT", GL_REPEAT, enumKV(GL_CLAMP_TO_EDGE), enumKV(GL_REPEAT), NULL);
	context->expectCall(context, context->writeEnumeration, "autoBlendMode", AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED, enumKV(AUTO_BLEND_MODE_NONE), enumKV(AUTO_BLEND_MODE_OPAQUE), enumKV(AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED), enumKV(AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED), NULL);
	context->expectCall(context, context->writeBoolean, "autoMipmap", true);
	context->expectCall(context, context->endStructure);
	
	GLTexture_serialize(&texture, (SerializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	texture.dispose(&texture);
}

TEST_SUITE(GLTextureTest, testInit, testDeserialization, testSerialization)
