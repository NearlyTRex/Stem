#include <string.h>
#include "gltexture/GLTextureAtlas.h"
#include "serialization/TestDeserializationContext.h"
#include "serialization/TestSerializationContext.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"

static void testInit() {
	GLTextureAtlas atlas, * atlasPtr;
	bool success;
	
	memset(&atlas, 0x00, sizeof(GLTextureAtlas));
	success = GLTextureAtlas_init(&atlas);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(atlas.dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlas.dispose);
	TestCase_assert(atlas.textureName == NULL, "Expected NULL but got %p", atlas.textureName);
	TestCase_assert(atlas.texture == NULL, "Expected NULL but got %p", atlas.texture);
	GLTextureAtlas_dispose(&atlas);
	
	memset(&atlas, 0xFF, sizeof(GLTextureAtlas));
	success = GLTextureAtlas_init(&atlas);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(atlas.dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlas.dispose);
	TestCase_assert(atlas.textureName == NULL, "Expected NULL but got %p", atlas.textureName);
	TestCase_assert(atlas.texture == NULL, "Expected NULL but got %p", atlas.texture);
	GLTextureAtlas_dispose(&atlas);
	
	atlasPtr = GLTextureAtlas_create();
	TestCase_assert(atlasPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(atlasPtr->dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlasPtr->dispose);
	TestCase_assert(atlasPtr->textureName == NULL, "Expected NULL but got %p", atlasPtr->textureName);
	TestCase_assert(atlasPtr->texture == NULL, "Expected NULL but got %p", atlasPtr->texture);
	GLTextureAtlas_dispose(atlasPtr);
}

static void testSetRemoveEntry() {
	GLTextureAtlas atlas;
	const char ** keys;
	size_t keyCount;
	struct GLTextureAtlas_entry entry;
	
	GLTextureAtlas_init(&atlas);
	keyCount = -1;
	keys = GLTextureAtlas_getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 0, "Expected 0 but got " SIZE_T_FORMAT, keyCount);
	free(keys);
	TestCase_assert(!GLTextureAtlas_hasKey(&atlas, "a"), "Expected false but got true");
	entry = GLTextureAtlas_lookup(&atlas, "a");
	TestCase_assert(entry.left == 0.0f, "Expected 0.0 but got %f", entry.left);
	TestCase_assert(entry.right == 0.0f, "Expected 0.0 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.0f, "Expected 0.0 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.0f, "Expected 0.0 but got %f", entry.top);
	
	GLTextureAtlas_setEntry(&atlas, "a", (struct GLTextureAtlas_entry) {0.125f, 0.25f, 0.375f, 0.5f});
	keyCount = -1;
	keys = GLTextureAtlas_getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 1, "Expected 1 but got " SIZE_T_FORMAT, keyCount);
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(keys[0], "a"), "Expected \"a\" but got \"%s\"", keys[0]);
	free(keys);
	TestCase_assert(GLTextureAtlas_hasKey(&atlas, "a"), "Expected true but got false");
	entry = GLTextureAtlas_lookup(&atlas, "a");
	TestCase_assert(entry.left == 0.125f, "Expected 0.125 but got %f", entry.left);
	TestCase_assert(entry.right == 0.25f, "Expected 0.25 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.375f, "Expected 0.375 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.5f, "Expected 0.5 but got %f", entry.top);
	
	entry = GLTextureAtlas_lookup(&atlas, "b");
	TestCase_assert(entry.left == 0.0f, "Expected 0.0 but got %f", entry.left);
	TestCase_assert(entry.right == 0.0f, "Expected 0.0 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.0f, "Expected 0.0 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.0f, "Expected 0.0 but got %f", entry.top);
	
	GLTextureAtlas_setEntry(&atlas, "b", (struct GLTextureAtlas_entry) {0.5f, 0.625f, 0.75f, 0.875f});
	keyCount = -1;
	keys = GLTextureAtlas_getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 2, "Expected 2 but got " SIZE_T_FORMAT, keyCount);
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(keys[0], "a") || !strcmp(keys[0], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[0]);
	TestCase_assert(!strcmp(keys[1], "a") || !strcmp(keys[1], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[1]);
	free(keys);
	TestCase_assert(GLTextureAtlas_hasKey(&atlas, "b"), "Expected true but got false");
	entry = GLTextureAtlas_lookup(&atlas, "b");
	TestCase_assert(entry.left == 0.5f, "Expected 0.5 but got %f", entry.left);
	TestCase_assert(entry.right == 0.625f, "Expected 0.625 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.75f, "Expected 0.75 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.875f, "Expected 0.875 but got %f", entry.top);
	
	GLTextureAtlas_removeEntry(&atlas, "a");
	keyCount = -1;
	keys = GLTextureAtlas_getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 1, "Expected 1 but got " SIZE_T_FORMAT, keyCount);
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(keys[0], "b"), "Expected \"b\" but got \"%s\"", keys[0]);
	free(keys);
	TestCase_assert(!GLTextureAtlas_hasKey(&atlas, "a"), "Expected false but got true");
	entry = GLTextureAtlas_lookup(&atlas, "a");
	TestCase_assert(entry.left == 0.0f, "Expected 0.0 but got %f", entry.left);
	TestCase_assert(entry.right == 0.0f, "Expected 0.0 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.0f, "Expected 0.0 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.0f, "Expected 0.0 but got %f", entry.top);
}

static void testDeserialization() {
	GLTextureAtlas * atlasPtr, atlas;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	bool success;
	unsigned int failIndex;
	const char ** keys;
	size_t keyCount;
	struct GLTextureAtlas_entry entry;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "gltextureatlas");
	context->expectCall(context, context->readUInt16, "format_version", GLTEXTUREATLAS_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "texture_name", "foo");
	context->expectCall(context, context->beginDictionary, "entries", 0);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	atlasPtr = GLTextureAtlas_deserialize((DeserializationContext *) context);
	context->finish(context);
	
	TestCase_assert(atlasPtr != NULL, "GLTextureAtlas_deserialize unexpectedly returned NULL");
	if (atlasPtr == NULL) { return; } // Silence clang warning
	TestCase_assert(atlasPtr->dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlasPtr->dispose);
	TestCase_assert(atlasPtr->textureName != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(atlasPtr->textureName, "foo"), "Expected \"foo\" but got \"%s\"", atlasPtr->textureName);
	TestCase_assert(atlasPtr->texture == NULL, "Expected NULL but got %p", atlasPtr->texture);
	keyCount = -1;
	keys = GLTextureAtlas_getKeys(atlasPtr, &keyCount);
	TestCase_assert(keyCount == 0, "Expected 0 but got " SIZE_T_FORMAT, keyCount);
	free(keys);
	GLTextureAtlas_dispose(atlasPtr);
	
	context->rewind(context);
	success = GLTextureAtlas_loadSerializedData(&atlas, (DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(atlas.dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlas.dispose);
	TestCase_assert(atlas.textureName != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(atlas.textureName, "foo"), "Expected \"foo\" but got \"%s\"", atlas.textureName);
	TestCase_assert(atlas.texture == NULL, "Expected NULL but got %p", atlas.texture);
	keyCount = -1;
	keys = GLTextureAtlas_getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 0, "Expected 0 but got " SIZE_T_FORMAT, keyCount);
	free(keys);
	GLTextureAtlas_dispose(&atlas);
	
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "gltextureatlas");
	context->expectCall(context, context->readUInt16, "format_version", GLTEXTUREATLAS_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "texture_name", "bar");
	context->expectCall(context, context->beginDictionary, "entries", 2);
	context->expectCall(context, context->readNextDictionaryKey, "a");
	context->expectCall(context, context->beginStructure, "a");
	context->expectCall(context, context->readFloat, "left", 0.0f);
	context->expectCall(context, context->readFloat, "right", 0.25f);
	context->expectCall(context, context->readFloat, "bottom", 0.5f);
	context->expectCall(context, context->readFloat, "top", 0.75f);
	context->expectCall(context, context->endStructure);
	context->expectCall(context, context->readNextDictionaryKey, "b");
	context->expectCall(context, context->beginStructure, "b");
	context->expectCall(context, context->readFloat, "left", 0.125f);
	context->expectCall(context, context->readFloat, "right", 0.375f);
	context->expectCall(context, context->readFloat, "bottom", 0.625f);
	context->expectCall(context, context->readFloat, "top", 0.875f);
	context->expectCall(context, context->endStructure);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	atlasPtr = GLTextureAtlas_deserialize((DeserializationContext *) context);
	context->finish(context);
	
	TestCase_assert(atlasPtr != NULL, "GLTextureAtlas_deserialize unexpectedly returned NULL");
	if (atlasPtr == NULL) { return; } // Silence clang warning
	TestCase_assert(atlasPtr->dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlasPtr->dispose);
	TestCase_assert(atlasPtr->textureName != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(atlasPtr->textureName, "bar"), "Expected \"bar\" but got \"%s\"", atlasPtr->textureName);
	TestCase_assert(atlasPtr->texture == NULL, "Expected NULL but got %p", atlasPtr->texture);
	keyCount = -1;
	keys = GLTextureAtlas_getKeys(atlasPtr, &keyCount);
	TestCase_assert(keyCount == 2, "Expected 2 but got " SIZE_T_FORMAT, keyCount);
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(keys[0], "a") || !strcmp(keys[0], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[0]);
	TestCase_assert(!strcmp(keys[1], "a") || !strcmp(keys[1], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[1]);
	free(keys);
	TestCase_assert(GLTextureAtlas_hasKey(atlasPtr, "a"), "Expected true but got false");
	TestCase_assert(GLTextureAtlas_hasKey(atlasPtr, "b"), "Expected true but got false");
	entry = GLTextureAtlas_lookup(atlasPtr, "a");
	TestCase_assert(entry.left == 0.0f, "Expected 0.0 but got %f", entry.left);
	TestCase_assert(entry.right == 0.25f, "Expected 0.25 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.5f, "Expected 0.5 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.75f, "Expected 0.75 but got %f", entry.top);
	entry = GLTextureAtlas_lookup(atlasPtr, "b");
	TestCase_assert(entry.left == 0.125f, "Expected 0.125 but got %f", entry.left);
	TestCase_assert(entry.right == 0.375f, "Expected 0.375 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.625f, "Expected 0.625 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.875f, "Expected 0.875 but got %f", entry.top);
	GLTextureAtlas_dispose(atlasPtr);
	
	context->rewind(context);
	success = GLTextureAtlas_loadSerializedData(&atlas, (DeserializationContext *) context);
	context->finish(context);
	
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(atlas.dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlas.dispose);
	TestCase_assert(atlas.textureName != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(atlas.textureName, "bar"), "Expected \"bar\" but got \"%s\"", atlas.textureName);
	TestCase_assert(atlas.texture == NULL, "Expected NULL but got %p", atlas.texture);
	keyCount = -1;
	keys = GLTextureAtlas_getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 2, "Expected 2 but got " SIZE_T_FORMAT, keyCount);
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(keys[0], "a") || !strcmp(keys[0], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[0]);
	TestCase_assert(!strcmp(keys[1], "a") || !strcmp(keys[1], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[1]);
	free(keys);
	TestCase_assert(GLTextureAtlas_hasKey(&atlas, "a"), "Expected true but got false");
	TestCase_assert(GLTextureAtlas_hasKey(&atlas, "b"), "Expected true but got false");
	entry = GLTextureAtlas_lookup(&atlas, "a");
	TestCase_assert(entry.left == 0.0f, "Expected 0.0 but got %f", entry.left);
	TestCase_assert(entry.right == 0.25f, "Expected 0.25 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.5f, "Expected 0.5 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.75f, "Expected 0.75 but got %f", entry.top);
	entry = GLTextureAtlas_lookup(&atlas, "b");
	TestCase_assert(entry.left == 0.125f, "Expected 0.125 but got %f", entry.left);
	TestCase_assert(entry.right == 0.375f, "Expected 0.375 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.625f, "Expected 0.625 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.875f, "Expected 0.875 but got %f", entry.top);
	GLTextureAtlas_dispose(&atlas);
	
	for (failIndex = 0; failIndex < 20; failIndex++) {
		context->rewind(context);
		context->failNthCall(context, failIndex, 1);
		atlasPtr = GLTextureAtlas_deserialize((DeserializationContext *) context);
		TestCase_assert(atlasPtr == NULL, "GLTextureAtlas_deserialize didn't return NULL when deserialization call %d failed", failIndex);
		
		context->rewind(context);
		context->failNthCall(context, failIndex, 1);
		success = GLTextureAtlas_loadSerializedData(&atlas, (DeserializationContext *) context);
		TestCase_assert(!success, "GLTextureAtlas_loadSerializedData didn't return false when deserialization call %d failed", failIndex);
	}
	context->dispose(context);
}

static void testSerialization() {
	GLTextureAtlas atlas;
	TestSerializationContext * context;
	jmp_buf jmpEnv;
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	GLTextureAtlas_init(&atlas);
	atlas.textureName = "foo";
	
	context->expectCall(context, context->beginStructure, "gltextureatlas");
	context->expectCall(context, context->writeUInt16, "format_version", GLTEXTUREATLAS_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->writeString, "texture_name", "foo");
	context->expectCall(context, context->beginDictionary, "entries");
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	GLTextureAtlas_serialize(&atlas, (SerializationContext *) context);
	context->finish(context);
	context->dispose(context);
	GLTextureAtlas_dispose(&atlas);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	GLTextureAtlas_init(&atlas);
	atlas.textureName = "bar";
	GLTextureAtlas_setEntry(&atlas, "a", (struct GLTextureAtlas_entry) {0.0f, 0.25f, 0.5f, 0.75f});
	GLTextureAtlas_setEntry(&atlas, "b", (struct GLTextureAtlas_entry) {0.125f, 0.375f, 0.625f, 0.875f});
	
	context->expectCall(context, context->beginStructure, "gltextureatlas");
	context->expectCall(context, context->writeUInt16, "format_version", GLTEXTUREATLAS_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->writeString, "texture_name", "bar");
	context->expectCall(context, context->beginDictionary, "entries");
	context->expectCall(context, context->beginStructure, "a");
	context->expectCall(context, context->writeFloat, "left", 0.0f);
	context->expectCall(context, context->writeFloat, "right", 0.25f);
	context->expectCall(context, context->writeFloat, "bottom", 0.5f);
	context->expectCall(context, context->writeFloat, "top", 0.75f);
	context->expectCall(context, context->endStructure);
	context->expectCall(context, context->beginStructure, "b");
	context->expectCall(context, context->writeFloat, "left", 0.125f);
	context->expectCall(context, context->writeFloat, "right", 0.375f);
	context->expectCall(context, context->writeFloat, "bottom", 0.625f);
	context->expectCall(context, context->writeFloat, "top", 0.875f);
	context->expectCall(context, context->endStructure);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	GLTextureAtlas_serialize(&atlas, (SerializationContext *) context);
	context->finish(context);
	context->dispose(context);
	GLTextureAtlas_dispose(&atlas);
}

static void testFormatVersionVerification() {
	GLTextureAtlas * atlas;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "gltextureatlas");
	context->expectCall(context, context->readUInt16, "format_version", GLTEXTUREATLAS_SERIALIZATION_FORMAT_VERSION + 1);
	
	atlas = GLTextureAtlas_deserialize((DeserializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(atlas == NULL, "GLTextureAtlas_deserialize didn't return NULL when format_version was too high");
}

static void testGetEntryDimensions() {
	GLTextureAtlas atlas;
	GLTexture texture;
	Vector2f result;
	
	GLTexture_init(&texture, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, AUTO_BLEND_MODE_NONE, false, false);
	texture.pixelWidth = 64;
	texture.pixelHeight = 128;
	GLTextureAtlas_init(&atlas);
	GLTextureAtlas_setTexture(&atlas, &texture, true);
	GLTextureAtlas_setEntry(&atlas, "a", (struct GLTextureAtlas_entry) {0.0f, 0.5f, 0.25f, 0.75f});
	GLTextureAtlas_setEntry(&atlas, "b", (struct GLTextureAtlas_entry) {0.125f, 0.375f, 0.25f, 0.375f});
	
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", 1.0f, 2.0f);
	TestCase_assert(result.x == 1.0f, "Expected 1.0f but got %f\n", result.x);
	TestCase_assert(result.y == 2.0f, "Expected 2.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", 3.0f, 1.0f);
	TestCase_assert(result.x == 3.0f, "Expected 3.0f but got %f\n", result.x);
	TestCase_assert(result.y == 1.0f, "Expected 1.0f but got %f\n", result.y);
	
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", GLTEXTUREATLAS_SIZE_AUTO, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 32.0f, "Expected 32.0f but got %f\n", result.x);
	TestCase_assert(result.y == 64.0f, "Expected 64.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "b", GLTEXTUREATLAS_SIZE_AUTO, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 16.0f, "Expected 16.0f but got %f\n", result.x);
	TestCase_assert(result.y == 16.0f, "Expected 16.0f but got %f\n", result.y);
	
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", 1.0f, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 1.0f, "Expected 1.0f but got %f\n", result.x);
	TestCase_assert(result.y == 2.0f, "Expected 2.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", 2.0f, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 2.0f, "Expected 2.0f but got %f\n", result.x);
	TestCase_assert(result.y == 4.0f, "Expected 4.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "b", 1.0f, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 1.0f, "Expected 1.0f but got %f\n", result.x);
	TestCase_assert(result.y == 1.0f, "Expected 1.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "b", 0.5f, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 0.5f, "Expected 0.5f but got %f\n", result.x);
	TestCase_assert(result.y == 0.5f, "Expected 0.5f but got %f\n", result.y);
	
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", GLTEXTUREATLAS_SIZE_AUTO, 1.0f);
	TestCase_assert(result.x == 0.5f, "Expected 0.5f but got %f\n", result.x);
	TestCase_assert(result.y == 1.0f, "Expected 1.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", GLTEXTUREATLAS_SIZE_AUTO, 2.0f);
	TestCase_assert(result.x == 1.0f, "Expected 1.0f but got %f\n", result.x);
	TestCase_assert(result.y == 2.0f, "Expected 2.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "b", GLTEXTUREATLAS_SIZE_AUTO, 1.0f);
	TestCase_assert(result.x == 1.0f, "Expected 1.0f but got %f\n", result.x);
	TestCase_assert(result.y == 1.0f, "Expected 1.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "b", GLTEXTUREATLAS_SIZE_AUTO, 0.5f);
	TestCase_assert(result.x == 0.5f, "Expected 0.5f but got %f\n", result.x);
	TestCase_assert(result.y == 0.5f, "Expected 0.5f but got %f\n", result.y);
	
	texture.pixelWidth = 32;
	texture.pixelHeight = 16;
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", GLTEXTUREATLAS_SIZE_AUTO, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 16.0f, "Expected 16.0f but got %f\n", result.x);
	TestCase_assert(result.y == 8.0f, "Expected 8.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "b", GLTEXTUREATLAS_SIZE_AUTO, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 8.0f, "Expected 8.0f but got %f\n", result.x);
	TestCase_assert(result.y == 2.0f, "Expected 2.0f but got %f\n", result.y);
	
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", 1.0f, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 1.0f, "Expected 1.0f but got %f\n", result.x);
	TestCase_assert(result.y == 0.5f, "Expected 0.5f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", 2.0f, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 2.0f, "Expected 2.0f but got %f\n", result.x);
	TestCase_assert(result.y == 1.0f, "Expected 1.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "b", 1.0f, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 1.0f, "Expected 1.0f but got %f\n", result.x);
	TestCase_assert(result.y == 0.25f, "Expected 0.25f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "b", 0.5f, GLTEXTUREATLAS_SIZE_AUTO);
	TestCase_assert(result.x == 0.5f, "Expected 0.5f but got %f\n", result.x);
	TestCase_assert(result.y == 0.125f, "Expected 0.125f but got %f\n", result.y);
	
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", GLTEXTUREATLAS_SIZE_AUTO, 1.0f);
	TestCase_assert(result.x == 2.0f, "Expected 2.0f but got %f\n", result.x);
	TestCase_assert(result.y == 1.0f, "Expected 1.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "a", GLTEXTUREATLAS_SIZE_AUTO, 2.0f);
	TestCase_assert(result.x == 4.0f, "Expected 4.0f but got %f\n", result.x);
	TestCase_assert(result.y == 2.0f, "Expected 2.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "b", GLTEXTUREATLAS_SIZE_AUTO, 1.0f);
	TestCase_assert(result.x == 4.0f, "Expected 4.0f but got %f\n", result.x);
	TestCase_assert(result.y == 1.0f, "Expected 1.0f but got %f\n", result.y);
	result = GLTextureAtlas_getEntryDimensions(&atlas, "b", GLTEXTUREATLAS_SIZE_AUTO, 0.5f);
	TestCase_assert(result.x == 2.0f, "Expected 2.0f but got %f\n", result.x);
	TestCase_assert(result.y == 0.5f, "Expected 0.5f but got %f\n", result.y);
	
	GLTextureAtlas_dispose(&atlas);
}

void GLTextureAtlas_getVertices(GLTextureAtlas * self,
                                const char * key,
                                Vector2f offset,
                                Vector2f relativeOrigin,
                                Vector2f size,
                                GLenum indexType,
                                unsigned int baseIndex,
                                struct vertex_p2f_t2f * outVertices,
                                void * outIndexes,
                                unsigned int * ioVertexCount,
                                unsigned int * ioIndexCount);

static void testGetVertices() {
	GLTextureAtlas atlas;
	unsigned int vertexCount, indexCount;
	struct vertex_p2f_t2f vertices[4];
	GLubyte indexesByte[6];
	GLushort indexesShort[6];
	GLuint indexesInt[6];
	
	GLTextureAtlas_init(&atlas);
	GLTextureAtlas_setEntry(&atlas, "a", (struct GLTextureAtlas_entry) {0.0f, 0.25f, 0.5f, 0.75f});
	GLTextureAtlas_setEntry(&atlas, "b", (struct GLTextureAtlas_entry) {0.125f, 0.375f, 0.625f, 0.875f});
	
	vertexCount = indexCount = 0;
	GLTextureAtlas_getVertices(&atlas, "a", VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), GL_UNSIGNED_INT, 0, NULL, NULL, &vertexCount, &indexCount);
	TestCase_assert(vertexCount == 4, "Expected 4 but got %u", vertexCount);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	
	memset(vertices, 0xFF, sizeof(vertices));
	memset(indexesByte, 0xFF, sizeof(indexesByte));
	vertexCount = 1;
	indexCount = 2;
	GLTextureAtlas_getVertices(&atlas, "a", VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 1.0f), GL_UNSIGNED_BYTE, 0, vertices, indexesByte, &vertexCount, &indexCount);
	TestCase_assert(vertexCount == 5, "Expected 5 but got %u", vertexCount);
	TestCase_assert(vertices[0].position[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].position[0]);
	TestCase_assert(vertices[0].position[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].position[1]);
	TestCase_assert(vertices[0].texCoords[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].texCoords[0]);
	TestCase_assert(vertices[0].texCoords[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[0].texCoords[1]);
	TestCase_assert(vertices[1].position[0] == 1.0f, "Expected 1.0 but got %f\n", vertices[1].position[0]);
	TestCase_assert(vertices[1].position[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[1].position[1]);
	TestCase_assert(vertices[1].texCoords[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[1].texCoords[0]);
	TestCase_assert(vertices[1].texCoords[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[1].texCoords[1]);
	TestCase_assert(vertices[2].position[0] == 1.0f, "Expected 1.0 but got %f\n", vertices[2].position[0]);
	TestCase_assert(vertices[2].position[1] == 1.0f, "Expected 1.0 but got %f\n", vertices[2].position[1]);
	TestCase_assert(vertices[2].texCoords[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[2].texCoords[0]);
	TestCase_assert(vertices[2].texCoords[1] == 0.75f, "Expected 0.75 but got %f\n", vertices[2].texCoords[1]);
	TestCase_assert(vertices[3].position[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].position[0]);
	TestCase_assert(vertices[3].position[1] == 1.0f, "Expected 1.0 but got %f\n", vertices[3].position[1]);
	TestCase_assert(vertices[3].texCoords[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].texCoords[0]);
	TestCase_assert(vertices[3].texCoords[1] == 0.75f, "Expected 0.75 but got %f\n", vertices[3].texCoords[1]);
	TestCase_assert(indexCount == 8, "Expected 8 but got %u", indexCount);
	TestCase_assert(indexesByte[0] == 0, "Expected 0 but got %u", indexesByte[0]);
	TestCase_assert(indexesByte[1] == 1, "Expected 1 but got %u", indexesByte[1]);
	TestCase_assert(indexesByte[2] == 2, "Expected 2 but got %u", indexesByte[2]);
	TestCase_assert(indexesByte[3] == 2, "Expected 2 but got %u", indexesByte[3]);
	TestCase_assert(indexesByte[4] == 3, "Expected 3 but got %u", indexesByte[4]);
	TestCase_assert(indexesByte[5] == 0, "Expected 0 but got %u", indexesByte[5]);
	
	memset(vertices, 0xFF, sizeof(vertices));
	memset(indexesShort, 0xFF, sizeof(indexesShort));
	vertexCount = indexCount = 0;
	GLTextureAtlas_getVertices(&atlas, "b", VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 1.0f), GL_UNSIGNED_SHORT, 1, vertices, indexesShort, &vertexCount, &indexCount);
	TestCase_assert(vertexCount == 4, "Expected 4 but got %u", vertexCount);
	TestCase_assert(vertices[0].position[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].position[0]);
	TestCase_assert(vertices[0].position[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].position[1]);
	TestCase_assert(vertices[0].texCoords[0] == 0.125f, "Expected 0.125 but got %f\n", vertices[0].texCoords[0]);
	TestCase_assert(vertices[0].texCoords[1] == 0.625f, "Expected 0.625 but got %f\n", vertices[0].texCoords[1]);
	TestCase_assert(vertices[1].position[0] == 1.0f, "Expected 1.0 but got %f\n", vertices[1].position[0]);
	TestCase_assert(vertices[1].position[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[1].position[1]);
	TestCase_assert(vertices[1].texCoords[0] == 0.375f, "Expected 0.375 but got %f\n", vertices[1].texCoords[0]);
	TestCase_assert(vertices[1].texCoords[1] == 0.625f, "Expected 0.625 but got %f\n", vertices[1].texCoords[1]);
	TestCase_assert(vertices[2].position[0] == 1.0f, "Expected 1.0 but got %f\n", vertices[2].position[0]);
	TestCase_assert(vertices[2].position[1] == 1.0f, "Expected 1.0 but got %f\n", vertices[2].position[1]);
	TestCase_assert(vertices[2].texCoords[0] == 0.375f, "Expected 0.375 but got %f\n", vertices[2].texCoords[0]);
	TestCase_assert(vertices[2].texCoords[1] == 0.875f, "Expected 0.875 but got %f\n", vertices[2].texCoords[1]);
	TestCase_assert(vertices[3].position[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].position[0]);
	TestCase_assert(vertices[3].position[1] == 1.0f, "Expected 1.0 but got %f\n", vertices[3].position[1]);
	TestCase_assert(vertices[3].texCoords[0] == 0.125f, "Expected 0.125 but got %f\n", vertices[3].texCoords[0]);
	TestCase_assert(vertices[3].texCoords[1] == 0.875f, "Expected 0.875 but got %f\n", vertices[3].texCoords[1]);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	TestCase_assert(indexesShort[0] == 1, "Expected 1 but got %u", indexesShort[0]);
	TestCase_assert(indexesShort[1] == 2, "Expected 2 but got %u", indexesShort[1]);
	TestCase_assert(indexesShort[2] == 3, "Expected 3 but got %u", indexesShort[2]);
	TestCase_assert(indexesShort[3] == 3, "Expected 3 but got %u", indexesShort[3]);
	TestCase_assert(indexesShort[4] == 4, "Expected 4 but got %u", indexesShort[4]);
	TestCase_assert(indexesShort[5] == 1, "Expected 1 but got %u", indexesShort[5]);
	
	memset(vertices, 0xFF, sizeof(vertices));
	memset(indexesInt, 0xFF, sizeof(indexesInt));
	vertexCount = indexCount = 0;
	GLTextureAtlas_getVertices(&atlas, "a", VECTOR2f(6.0f, -3.0f), VECTOR2f(0.5f, 1.0f), VECTOR2f(2.0f, 1.5f), GL_UNSIGNED_INT, 2, vertices, indexesInt, &vertexCount, &indexCount);
	TestCase_assert(vertexCount == 4, "Expected 4 but got %u", vertexCount);
	TestCase_assert(vertices[0].position[0] == 5.0f, "Expected 5.0 but got %f\n", vertices[0].position[0]);
	TestCase_assert(vertices[0].position[1] == -4.5f, "Expected -4.5 but got %f\n", vertices[0].position[1]);
	TestCase_assert(vertices[0].texCoords[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].texCoords[0]);
	TestCase_assert(vertices[0].texCoords[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[0].texCoords[1]);
	TestCase_assert(vertices[1].position[0] == 7.0f, "Expected 7.0 but got %f\n", vertices[1].position[0]);
	TestCase_assert(vertices[1].position[1] == -4.5f, "Expected -4.5 but got %f\n", vertices[1].position[1]);
	TestCase_assert(vertices[1].texCoords[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[1].texCoords[0]);
	TestCase_assert(vertices[1].texCoords[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[1].texCoords[1]);
	TestCase_assert(vertices[2].position[0] == 7.0f, "Expected 7.0 but got %f\n", vertices[2].position[0]);
	TestCase_assert(vertices[2].position[1] == -3.0f, "Expected -3.0 but got %f\n", vertices[2].position[1]);
	TestCase_assert(vertices[2].texCoords[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[2].texCoords[0]);
	TestCase_assert(vertices[2].texCoords[1] == 0.75f, "Expected 0.75 but got %f\n", vertices[2].texCoords[1]);
	TestCase_assert(vertices[3].position[0] == 5.0f, "Expected 5.0 but got %f\n", vertices[3].position[0]);
	TestCase_assert(vertices[3].position[1] == -3.0f, "Expected -3.0 but got %f\n", vertices[3].position[1]);
	TestCase_assert(vertices[3].texCoords[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].texCoords[0]);
	TestCase_assert(vertices[3].texCoords[1] == 0.75f, "Expected 0.75 but got %f\n", vertices[3].texCoords[1]);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	TestCase_assert(indexesInt[0] == 2, "Expected 2 but got %u", indexesInt[0]);
	TestCase_assert(indexesInt[1] == 3, "Expected 3 but got %u", indexesInt[1]);
	TestCase_assert(indexesInt[2] == 4, "Expected 4 but got %u", indexesInt[2]);
	TestCase_assert(indexesInt[3] == 4, "Expected 4 but got %u", indexesInt[3]);
	TestCase_assert(indexesInt[4] == 5, "Expected 5 but got %u", indexesInt[4]);
	TestCase_assert(indexesInt[5] == 2, "Expected 2 but got %u", indexesInt[5]);
}

static void testGetVerticesWithColor() {
	GLTextureAtlas atlas;
	unsigned int vertexCount, indexCount;
	struct vertex_p2f_t2f_c4f vertices[4];
	GLubyte indexesByte[6];
	GLushort indexesShort[6];
	GLuint indexesInt[6];
	
	GLTextureAtlas_init(&atlas);
	GLTextureAtlas_setEntry(&atlas, "a", (struct GLTextureAtlas_entry) {0.0f, 0.25f, 0.5f, 0.75f});
	GLTextureAtlas_setEntry(&atlas, "b", (struct GLTextureAtlas_entry) {0.125f, 0.375f, 0.625f, 0.875f});
	
	vertexCount = indexCount = 0;
	GLTextureAtlas_getVerticesWithColor(&atlas, "a", VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), 0.0f, 0.0f, 0.0f, 0.0f, GL_UNSIGNED_INT, 0, NULL, NULL, &vertexCount, &indexCount);
	TestCase_assert(vertexCount == 4, "Expected 4 but got %u", vertexCount);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	
	memset(vertices, 0xFF, sizeof(vertices));
	memset(indexesByte, 0xFF, sizeof(indexesByte));
	vertexCount = 1;
	indexCount = 2;
	GLTextureAtlas_getVerticesWithColor(&atlas, "a", VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 1.0f), 0.0f, 0.0f, 0.0f, 0.0f, GL_UNSIGNED_BYTE, 3, vertices, indexesByte, &vertexCount, &indexCount);
	TestCase_assert(vertexCount == 5, "Expected 5 but got %u", vertexCount);
	TestCase_assert(vertices[0].position[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].position[0]);
	TestCase_assert(vertices[0].position[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].position[1]);
	TestCase_assert(vertices[0].texCoords[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].texCoords[0]);
	TestCase_assert(vertices[0].texCoords[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[0].texCoords[1]);
	TestCase_assert(vertices[0].color[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].color[0]);
	TestCase_assert(vertices[0].color[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].color[1]);
	TestCase_assert(vertices[0].color[2] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].color[2]);
	TestCase_assert(vertices[0].color[3] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].color[3]);
	TestCase_assert(vertices[1].position[0] == 1.0f, "Expected 1.0 but got %f\n", vertices[1].position[0]);
	TestCase_assert(vertices[1].position[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[1].position[1]);
	TestCase_assert(vertices[1].texCoords[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[1].texCoords[0]);
	TestCase_assert(vertices[1].texCoords[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[1].texCoords[1]);
	TestCase_assert(vertices[1].color[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[1].color[0]);
	TestCase_assert(vertices[1].color[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[1].color[1]);
	TestCase_assert(vertices[1].color[2] == 0.0f, "Expected 0.0 but got %f\n", vertices[1].color[2]);
	TestCase_assert(vertices[0].color[3] == 0.0f, "Expected 0.0 but got %f\n", vertices[1].color[3]);
	TestCase_assert(vertices[2].position[0] == 1.0f, "Expected 1.0 but got %f\n", vertices[2].position[0]);
	TestCase_assert(vertices[2].position[1] == 1.0f, "Expected 1.0 but got %f\n", vertices[2].position[1]);
	TestCase_assert(vertices[2].texCoords[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[2].texCoords[0]);
	TestCase_assert(vertices[2].texCoords[1] == 0.75f, "Expected 0.75 but got %f\n", vertices[2].texCoords[1]);
	TestCase_assert(vertices[2].color[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[2].color[0]);
	TestCase_assert(vertices[2].color[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[2].color[1]);
	TestCase_assert(vertices[2].color[2] == 0.0f, "Expected 0.0 but got %f\n", vertices[2].color[2]);
	TestCase_assert(vertices[2].color[3] == 0.0f, "Expected 0.0 but got %f\n", vertices[2].color[3]);
	TestCase_assert(vertices[3].position[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].position[0]);
	TestCase_assert(vertices[3].position[1] == 1.0f, "Expected 1.0 but got %f\n", vertices[3].position[1]);
	TestCase_assert(vertices[3].texCoords[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].texCoords[0]);
	TestCase_assert(vertices[3].texCoords[1] == 0.75f, "Expected 0.75 but got %f\n", vertices[3].texCoords[1]);
	TestCase_assert(vertices[3].color[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].color[0]);
	TestCase_assert(vertices[3].color[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].color[1]);
	TestCase_assert(vertices[3].color[2] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].color[2]);
	TestCase_assert(vertices[3].color[3] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].color[3]);
	TestCase_assert(indexCount == 8, "Expected 8 but got %u", indexCount);
	TestCase_assert(indexesByte[0] == 3, "Expected 3 but got %u", indexesByte[0]);
	TestCase_assert(indexesByte[1] == 4, "Expected 4 but got %u", indexesByte[1]);
	TestCase_assert(indexesByte[2] == 5, "Expected 5 but got %u", indexesByte[2]);
	TestCase_assert(indexesByte[3] == 5, "Expected 5 but got %u", indexesByte[3]);
	TestCase_assert(indexesByte[4] == 6, "Expected 6 but got %u", indexesByte[4]);
	TestCase_assert(indexesByte[5] == 3, "Expected 3 but got %u", indexesByte[5]);
	
	memset(vertices, 0xFF, sizeof(vertices));
	memset(indexesShort, 0xFF, sizeof(indexesShort));
	vertexCount = indexCount = 0;
	GLTextureAtlas_getVerticesWithColor(&atlas, "b", VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 1.0f), 0.25f, 0.5f, 0.75f, 1.0f, GL_UNSIGNED_SHORT, 4, vertices, indexesShort, &vertexCount, &indexCount);
	TestCase_assert(vertexCount == 4, "Expected 4 but got %u", vertexCount);
	TestCase_assert(vertices[0].position[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].position[0]);
	TestCase_assert(vertices[0].position[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].position[1]);
	TestCase_assert(vertices[0].texCoords[0] == 0.125f, "Expected 0.125 but got %f\n", vertices[0].texCoords[0]);
	TestCase_assert(vertices[0].texCoords[1] == 0.625f, "Expected 0.625 but got %f\n", vertices[0].texCoords[1]);
	TestCase_assert(vertices[0].color[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[0].color[0]);
	TestCase_assert(vertices[0].color[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[0].color[1]);
	TestCase_assert(vertices[0].color[2] == 0.75f, "Expected 0.75 but got %f\n", vertices[0].color[2]);
	TestCase_assert(vertices[0].color[3] == 1.0f, "Expected 1.0 but got %f\n", vertices[0].color[3]);
	TestCase_assert(vertices[1].position[0] == 1.0f, "Expected 1.0 but got %f\n", vertices[1].position[0]);
	TestCase_assert(vertices[1].position[1] == 0.0f, "Expected 0.0 but got %f\n", vertices[1].position[1]);
	TestCase_assert(vertices[1].texCoords[0] == 0.375f, "Expected 0.375 but got %f\n", vertices[1].texCoords[0]);
	TestCase_assert(vertices[1].texCoords[1] == 0.625f, "Expected 0.625 but got %f\n", vertices[1].texCoords[1]);
	TestCase_assert(vertices[1].color[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[1].color[0]);
	TestCase_assert(vertices[1].color[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[1].color[1]);
	TestCase_assert(vertices[1].color[2] == 0.75f, "Expected 0.75 but got %f\n", vertices[1].color[2]);
	TestCase_assert(vertices[1].color[3] == 1.0f, "Expected 1.0 but got %f\n", vertices[1].color[3]);
	TestCase_assert(vertices[2].position[0] == 1.0f, "Expected 1.0 but got %f\n", vertices[2].position[0]);
	TestCase_assert(vertices[2].position[1] == 1.0f, "Expected 1.0 but got %f\n", vertices[2].position[1]);
	TestCase_assert(vertices[2].texCoords[0] == 0.375f, "Expected 0.375 but got %f\n", vertices[2].texCoords[0]);
	TestCase_assert(vertices[2].texCoords[1] == 0.875f, "Expected 0.875 but got %f\n", vertices[2].texCoords[1]);
	TestCase_assert(vertices[2].color[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[2].color[0]);
	TestCase_assert(vertices[2].color[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[2].color[1]);
	TestCase_assert(vertices[2].color[2] == 0.75f, "Expected 0.75 but got %f\n", vertices[2].color[2]);
	TestCase_assert(vertices[2].color[3] == 1.0f, "Expected 1.0 but got %f\n", vertices[2].color[3]);
	TestCase_assert(vertices[3].position[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].position[0]);
	TestCase_assert(vertices[3].position[1] == 1.0f, "Expected 1.0 but got %f\n", vertices[3].position[1]);
	TestCase_assert(vertices[3].texCoords[0] == 0.125f, "Expected 0.125 but got %f\n", vertices[3].texCoords[0]);
	TestCase_assert(vertices[3].texCoords[1] == 0.875f, "Expected 0.875 but got %f\n", vertices[3].texCoords[1]);
	TestCase_assert(vertices[3].color[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[3].color[0]);
	TestCase_assert(vertices[3].color[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[3].color[1]);
	TestCase_assert(vertices[3].color[2] == 0.75f, "Expected 0.75 but got %f\n", vertices[3].color[2]);
	TestCase_assert(vertices[3].color[3] == 1.0f, "Expected 1.0 but got %f\n", vertices[3].color[3]);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	TestCase_assert(indexesShort[0] == 4, "Expected 4 but got %u", indexesShort[0]);
	TestCase_assert(indexesShort[1] == 5, "Expected 5 but got %u", indexesShort[1]);
	TestCase_assert(indexesShort[2] == 6, "Expected 6 but got %u", indexesShort[2]);
	TestCase_assert(indexesShort[3] == 6, "Expected 6 but got %u", indexesShort[3]);
	TestCase_assert(indexesShort[4] == 7, "Expected 7 but got %u", indexesShort[4]);
	TestCase_assert(indexesShort[5] == 4, "Expected 4 but got %u", indexesShort[5]);
	
	memset(vertices, 0xFF, sizeof(vertices));
	memset(indexesInt, 0xFF, sizeof(indexesInt));
	vertexCount = indexCount = 0;
	GLTextureAtlas_getVerticesWithColor(&atlas, "a", VECTOR2f(6.0f, -3.0f), VECTOR2f(0.5f, 1.0f), VECTOR2f(2.0f, 1.5f), 0.25f, 0.5f, 0.75f, 1.0f, GL_UNSIGNED_INT, 5, vertices, indexesInt, &vertexCount, &indexCount);
	TestCase_assert(vertexCount == 4, "Expected 4 but got %u", vertexCount);
	TestCase_assert(vertices[0].position[0] == 5.0f, "Expected 5.0 but got %f\n", vertices[0].position[0]);
	TestCase_assert(vertices[0].position[1] == -4.5f, "Expected -4.5 but got %f\n", vertices[0].position[1]);
	TestCase_assert(vertices[0].texCoords[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[0].texCoords[0]);
	TestCase_assert(vertices[0].texCoords[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[0].texCoords[1]);
	TestCase_assert(vertices[0].color[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[0].color[0]);
	TestCase_assert(vertices[0].color[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[0].color[1]);
	TestCase_assert(vertices[0].color[2] == 0.75f, "Expected 0.75 but got %f\n", vertices[0].color[2]);
	TestCase_assert(vertices[0].color[3] == 1.0f, "Expected 1.0 but got %f\n", vertices[0].color[3]);
	TestCase_assert(vertices[1].position[0] == 7.0f, "Expected 7.0 but got %f\n", vertices[1].position[0]);
	TestCase_assert(vertices[1].position[1] == -4.5f, "Expected -4.5 but got %f\n", vertices[1].position[1]);
	TestCase_assert(vertices[1].texCoords[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[1].texCoords[0]);
	TestCase_assert(vertices[1].texCoords[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[1].texCoords[1]);
	TestCase_assert(vertices[1].color[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[1].color[0]);
	TestCase_assert(vertices[1].color[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[1].color[1]);
	TestCase_assert(vertices[1].color[2] == 0.75f, "Expected 0.75 but got %f\n", vertices[1].color[2]);
	TestCase_assert(vertices[1].color[3] == 1.0f, "Expected 1.0 but got %f\n", vertices[1].color[3]);
	TestCase_assert(vertices[2].position[0] == 7.0f, "Expected 7.0 but got %f\n", vertices[2].position[0]);
	TestCase_assert(vertices[2].position[1] == -3.0f, "Expected -3.0 but got %f\n", vertices[2].position[1]);
	TestCase_assert(vertices[2].texCoords[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[2].texCoords[0]);
	TestCase_assert(vertices[2].texCoords[1] == 0.75f, "Expected 0.75 but got %f\n", vertices[2].texCoords[1]);
	TestCase_assert(vertices[2].color[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[2].color[0]);
	TestCase_assert(vertices[2].color[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[2].color[1]);
	TestCase_assert(vertices[2].color[2] == 0.75f, "Expected 0.75 but got %f\n", vertices[2].color[2]);
	TestCase_assert(vertices[2].color[3] == 1.0f, "Expected 1.0 but got %f\n", vertices[2].color[3]);
	TestCase_assert(vertices[3].position[0] == 5.0f, "Expected 5.0 but got %f\n", vertices[3].position[0]);
	TestCase_assert(vertices[3].position[1] == -3.0f, "Expected -3.0 but got %f\n", vertices[3].position[1]);
	TestCase_assert(vertices[3].texCoords[0] == 0.0f, "Expected 0.0 but got %f\n", vertices[3].texCoords[0]);
	TestCase_assert(vertices[3].texCoords[1] == 0.75f, "Expected 0.75 but got %f\n", vertices[3].texCoords[1]);
	TestCase_assert(vertices[3].color[0] == 0.25f, "Expected 0.25 but got %f\n", vertices[3].color[0]);
	TestCase_assert(vertices[3].color[1] == 0.5f, "Expected 0.5 but got %f\n", vertices[3].color[1]);
	TestCase_assert(vertices[3].color[2] == 0.75f, "Expected 0.75 but got %f\n", vertices[3].color[2]);
	TestCase_assert(vertices[3].color[3] == 1.0f, "Expected 1.0 but got %f\n", vertices[3].color[3]);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	TestCase_assert(indexesInt[0] == 5, "Expected 5 but got %u", indexesInt[0]);
	TestCase_assert(indexesInt[1] == 6, "Expected 6 but got %u", indexesInt[1]);
	TestCase_assert(indexesInt[2] == 7, "Expected 7 but got %u", indexesInt[2]);
	TestCase_assert(indexesInt[3] == 7, "Expected 7 but got %u", indexesInt[3]);
	TestCase_assert(indexesInt[4] == 8, "Expected 8 but got %u", indexesInt[4]);
	TestCase_assert(indexesInt[5] == 5, "Expected 5 but got %u", indexesInt[5]);
}

TEST_SUITE(GLTextureAtlasTest,
           testInit,
           testSetRemoveEntry,
           testDeserialization,
           testSerialization,
           testGetEntryDimensions,
           testGetVertices,
           testGetVerticesWithColor,
           testFormatVersionVerification)
