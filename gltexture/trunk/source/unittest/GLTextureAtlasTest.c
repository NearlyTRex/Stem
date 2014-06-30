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
	TestCase_assert(atlas.getKeys == GLTextureAtlas_getKeys, "Expected %p but got %p", GLTextureAtlas_getKeys, atlas.getKeys);
	TestCase_assert(atlas.hasKey == GLTextureAtlas_hasKey, "Expected %p but got %p", GLTextureAtlas_hasKey, atlas.hasKey);
	TestCase_assert(atlas.setEntry == GLTextureAtlas_setEntry, "Expected %p but got %p", GLTextureAtlas_setEntry, atlas.setEntry);
	TestCase_assert(atlas.removeEntry == GLTextureAtlas_removeEntry, "Expected %p but got %p", GLTextureAtlas_removeEntry, atlas.removeEntry);
	TestCase_assert(atlas.lookup == GLTextureAtlas_lookup, "Expected %p but got %p", GLTextureAtlas_lookup, atlas.lookup);
	TestCase_assert(atlas.getVertices == GLTextureAtlas_getVertices, "Expected %p but got %p", GLTextureAtlas_getVertices, atlas.lookup);
	TestCase_assert(atlas.getIndexes == GLTextureAtlas_getIndexes, "Expected %p but got %p", GLTextureAtlas_getIndexes, atlas.lookup);
	TestCase_assert(atlas.textureName == NULL, "Expected NULL but got %p", atlas.textureName);
	atlas.dispose(&atlas);
	
	memset(&atlas, 0xFF, sizeof(GLTextureAtlas));
	success = GLTextureAtlas_init(&atlas);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(atlas.dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlas.dispose);
	TestCase_assert(atlas.getKeys == GLTextureAtlas_getKeys, "Expected %p but got %p", GLTextureAtlas_getKeys, atlas.getKeys);
	TestCase_assert(atlas.hasKey == GLTextureAtlas_hasKey, "Expected %p but got %p", GLTextureAtlas_hasKey, atlas.hasKey);
	TestCase_assert(atlas.setEntry == GLTextureAtlas_setEntry, "Expected %p but got %p", GLTextureAtlas_setEntry, atlas.setEntry);
	TestCase_assert(atlas.removeEntry == GLTextureAtlas_removeEntry, "Expected %p but got %p", GLTextureAtlas_removeEntry, atlas.removeEntry);
	TestCase_assert(atlas.lookup == GLTextureAtlas_lookup, "Expected %p but got %p", GLTextureAtlas_lookup, atlas.lookup);
	TestCase_assert(atlas.getVertices == GLTextureAtlas_getVertices, "Expected %p but got %p", GLTextureAtlas_getVertices, atlas.lookup);
	TestCase_assert(atlas.getIndexes == GLTextureAtlas_getIndexes, "Expected %p but got %p", GLTextureAtlas_getIndexes, atlas.lookup);
	TestCase_assert(atlas.textureName == NULL, "Expected NULL but got %p", atlas.textureName);
	atlas.dispose(&atlas);
	
	atlasPtr = GLTextureAtlas_create();
	TestCase_assert(atlasPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(atlasPtr->dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlasPtr->dispose);
	TestCase_assert(atlasPtr->getKeys == GLTextureAtlas_getKeys, "Expected %p but got %p", GLTextureAtlas_getKeys, atlasPtr->getKeys);
	TestCase_assert(atlasPtr->hasKey == GLTextureAtlas_hasKey, "Expected %p but got %p", GLTextureAtlas_hasKey, atlasPtr->hasKey);
	TestCase_assert(atlasPtr->setEntry == GLTextureAtlas_setEntry, "Expected %p but got %p", GLTextureAtlas_setEntry, atlasPtr->setEntry);
	TestCase_assert(atlasPtr->removeEntry == GLTextureAtlas_removeEntry, "Expected %p but got %p", GLTextureAtlas_removeEntry, atlasPtr->removeEntry);
	TestCase_assert(atlasPtr->lookup == GLTextureAtlas_lookup, "Expected %p but got %p", GLTextureAtlas_lookup, atlasPtr->lookup);
	TestCase_assert(atlasPtr->getVertices == GLTextureAtlas_getVertices, "Expected %p but got %p", GLTextureAtlas_getVertices, atlasPtr->lookup);
	TestCase_assert(atlasPtr->getIndexes == GLTextureAtlas_getIndexes, "Expected %p but got %p", GLTextureAtlas_getIndexes, atlasPtr->lookup);
	TestCase_assert(atlasPtr->textureName == NULL, "Expected NULL but got %p", atlasPtr->textureName);
	atlasPtr->dispose(atlasPtr);
}

static void testSetRemoveEntry() {
	GLTextureAtlas atlas;
	const char ** keys;
	size_t keyCount;
	struct GLTextureAtlas_entry entry;
	
	GLTextureAtlas_init(&atlas);
	keyCount = -1;
	keys = atlas.getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 0, "Expected 0 but got " SIZE_T_FORMAT, keyCount);
	free(keys);
	TestCase_assert(!atlas.hasKey(&atlas, "a"), "Expected false but got true");
	entry = atlas.lookup(&atlas, "a");
	TestCase_assert(entry.left == 0.0f, "Expected 0.0 but got %f", entry.left);
	TestCase_assert(entry.right == 0.0f, "Expected 0.0 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.0f, "Expected 0.0 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.0f, "Expected 0.0 but got %f", entry.top);
	
	atlas.setEntry(&atlas, "a", (struct GLTextureAtlas_entry) {0.125f, 0.25f, 0.375f, 0.5f});
	keyCount = -1;
	keys = atlas.getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 1, "Expected 1 but got " SIZE_T_FORMAT, keyCount);
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(keys[0], "a"), "Expected \"a\" but got \"%s\"", keys[0]);
	free(keys);
	TestCase_assert(atlas.hasKey(&atlas, "a"), "Expected true but got false");
	entry = atlas.lookup(&atlas, "a");
	TestCase_assert(entry.left == 0.125f, "Expected 0.125 but got %f", entry.left);
	TestCase_assert(entry.right == 0.25f, "Expected 0.25 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.375f, "Expected 0.375 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.5f, "Expected 0.5 but got %f", entry.top);
	
	entry = atlas.lookup(&atlas, "b");
	TestCase_assert(entry.left == 0.0f, "Expected 0.0 but got %f", entry.left);
	TestCase_assert(entry.right == 0.0f, "Expected 0.0 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.0f, "Expected 0.0 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.0f, "Expected 0.0 but got %f", entry.top);
	
	atlas.setEntry(&atlas, "b", (struct GLTextureAtlas_entry) {0.5f, 0.625f, 0.75f, 0.875f});
	keyCount = -1;
	keys = atlas.getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 2, "Expected 2 but got " SIZE_T_FORMAT, keyCount);
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(keys[0], "a") || !strcmp(keys[0], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[0]);
	TestCase_assert(!strcmp(keys[1], "a") || !strcmp(keys[1], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[1]);
	free(keys);
	TestCase_assert(atlas.hasKey(&atlas, "b"), "Expected true but got false");
	entry = atlas.lookup(&atlas, "b");
	TestCase_assert(entry.left == 0.5f, "Expected 0.5 but got %f", entry.left);
	TestCase_assert(entry.right == 0.625f, "Expected 0.625 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.75f, "Expected 0.75 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.875f, "Expected 0.875 but got %f", entry.top);
	
	atlas.removeEntry(&atlas, "a");
	keyCount = -1;
	keys = atlas.getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 1, "Expected 1 but got " SIZE_T_FORMAT, keyCount);
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(keys[0], "b"), "Expected \"b\" but got \"%s\"", keys[0]);
	free(keys);
	TestCase_assert(!atlas.hasKey(&atlas, "a"), "Expected false but got true");
	entry = atlas.lookup(&atlas, "a");
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
	TestCase_assert(atlasPtr->getKeys == GLTextureAtlas_getKeys, "Expected %p but got %p", GLTextureAtlas_getKeys, atlasPtr->getKeys);
	TestCase_assert(atlasPtr->hasKey == GLTextureAtlas_hasKey, "Expected %p but got %p", GLTextureAtlas_hasKey, atlasPtr->hasKey);
	TestCase_assert(atlasPtr->setEntry == GLTextureAtlas_setEntry, "Expected %p but got %p", GLTextureAtlas_setEntry, atlasPtr->setEntry);
	TestCase_assert(atlasPtr->removeEntry == GLTextureAtlas_removeEntry, "Expected %p but got %p", GLTextureAtlas_removeEntry, atlasPtr->removeEntry);
	TestCase_assert(atlasPtr->lookup == GLTextureAtlas_lookup, "Expected %p but got %p", GLTextureAtlas_lookup, atlasPtr->lookup);
	TestCase_assert(atlasPtr->textureName != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(atlasPtr->textureName, "foo"), "Expected \"foo\" but got \"%s\"", atlasPtr->textureName);
	keyCount = -1;
	keys = atlasPtr->getKeys(atlasPtr, &keyCount);
	TestCase_assert(keyCount == 0, "Expected 0 but got " SIZE_T_FORMAT, keyCount);
	free(keys);
	atlasPtr->dispose(atlasPtr);
	
	context->rewind(context);
	success = GLTextureAtlas_loadSerializedData(&atlas, (DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(atlas.dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlas.dispose);
	TestCase_assert(atlas.getKeys == GLTextureAtlas_getKeys, "Expected %p but got %p", GLTextureAtlas_getKeys, atlas.getKeys);
	TestCase_assert(atlas.hasKey == GLTextureAtlas_hasKey, "Expected %p but got %p", GLTextureAtlas_hasKey, atlas.hasKey);
	TestCase_assert(atlas.setEntry == GLTextureAtlas_setEntry, "Expected %p but got %p", GLTextureAtlas_setEntry, atlas.setEntry);
	TestCase_assert(atlas.removeEntry == GLTextureAtlas_removeEntry, "Expected %p but got %p", GLTextureAtlas_removeEntry, atlas.removeEntry);
	TestCase_assert(atlas.lookup == GLTextureAtlas_lookup, "Expected %p but got %p", GLTextureAtlas_lookup, atlas.lookup);
	TestCase_assert(atlas.textureName != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(atlas.textureName, "foo"), "Expected \"foo\" but got \"%s\"", atlas.textureName);
	keyCount = -1;
	keys = atlas.getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 0, "Expected 0 but got " SIZE_T_FORMAT, keyCount);
	free(keys);
	atlas.dispose(&atlas);
	
	
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
	TestCase_assert(atlasPtr->getKeys == GLTextureAtlas_getKeys, "Expected %p but got %p", GLTextureAtlas_getKeys, atlasPtr->getKeys);
	TestCase_assert(atlasPtr->hasKey == GLTextureAtlas_hasKey, "Expected %p but got %p", GLTextureAtlas_hasKey, atlasPtr->hasKey);
	TestCase_assert(atlasPtr->setEntry == GLTextureAtlas_setEntry, "Expected %p but got %p", GLTextureAtlas_setEntry, atlasPtr->setEntry);
	TestCase_assert(atlasPtr->removeEntry == GLTextureAtlas_removeEntry, "Expected %p but got %p", GLTextureAtlas_removeEntry, atlasPtr->removeEntry);
	TestCase_assert(atlasPtr->lookup == GLTextureAtlas_lookup, "Expected %p but got %p", GLTextureAtlas_lookup, atlasPtr->lookup);
	TestCase_assert(atlasPtr->textureName != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(atlasPtr->textureName, "bar"), "Expected \"bar\" but got \"%s\"", atlasPtr->textureName);
	keyCount = -1;
	keys = atlasPtr->getKeys(atlasPtr, &keyCount);
	TestCase_assert(keyCount == 2, "Expected 2 but got " SIZE_T_FORMAT, keyCount);
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(keys[0], "a") || !strcmp(keys[0], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[0]);
	TestCase_assert(!strcmp(keys[1], "a") || !strcmp(keys[1], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[1]);
	free(keys);
	TestCase_assert(atlasPtr->hasKey(atlasPtr, "a"), "Expected true but got false");
	TestCase_assert(atlasPtr->hasKey(atlasPtr, "b"), "Expected true but got false");
	entry = atlasPtr->lookup(atlasPtr, "a");
	TestCase_assert(entry.left == 0.0f, "Expected 0.0 but got %f", entry.left);
	TestCase_assert(entry.right == 0.25f, "Expected 0.25 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.5f, "Expected 0.5 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.75f, "Expected 0.75 but got %f", entry.top);
	entry = atlasPtr->lookup(atlasPtr, "b");
	TestCase_assert(entry.left == 0.125f, "Expected 0.125 but got %f", entry.left);
	TestCase_assert(entry.right == 0.375f, "Expected 0.375 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.625f, "Expected 0.625 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.875f, "Expected 0.875 but got %f", entry.top);
	atlasPtr->dispose(atlasPtr);
	
	context->rewind(context);
	success = GLTextureAtlas_loadSerializedData(&atlas, (DeserializationContext *) context);
	context->finish(context);
	
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(atlas.dispose == GLTextureAtlas_dispose, "Expected %p but got %p", GLTextureAtlas_dispose, atlas.dispose);
	TestCase_assert(atlas.getKeys == GLTextureAtlas_getKeys, "Expected %p but got %p", GLTextureAtlas_getKeys, atlas.getKeys);
	TestCase_assert(atlas.hasKey == GLTextureAtlas_hasKey, "Expected %p but got %p", GLTextureAtlas_hasKey, atlas.hasKey);
	TestCase_assert(atlas.setEntry == GLTextureAtlas_setEntry, "Expected %p but got %p", GLTextureAtlas_setEntry, atlas.setEntry);
	TestCase_assert(atlas.removeEntry == GLTextureAtlas_removeEntry, "Expected %p but got %p", GLTextureAtlas_removeEntry, atlas.removeEntry);
	TestCase_assert(atlas.lookup == GLTextureAtlas_lookup, "Expected %p but got %p", GLTextureAtlas_lookup, atlas.lookup);
	TestCase_assert(atlas.textureName != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(atlas.textureName, "bar"), "Expected \"bar\" but got \"%s\"", atlas.textureName);
	keyCount = -1;
	keys = atlas.getKeys(&atlas, &keyCount);
	TestCase_assert(keyCount == 2, "Expected 2 but got " SIZE_T_FORMAT, keyCount);
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(keys[0], "a") || !strcmp(keys[0], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[0]);
	TestCase_assert(!strcmp(keys[1], "a") || !strcmp(keys[1], "b"), "Expected \"a\" or \"b\" but got \"%s\"", keys[1]);
	free(keys);
	TestCase_assert(atlas.hasKey(&atlas, "a"), "Expected true but got false");
	TestCase_assert(atlas.hasKey(&atlas, "b"), "Expected true but got false");
	entry = atlas.lookup(&atlas, "a");
	TestCase_assert(entry.left == 0.0f, "Expected 0.0 but got %f", entry.left);
	TestCase_assert(entry.right == 0.25f, "Expected 0.25 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.5f, "Expected 0.5 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.75f, "Expected 0.75 but got %f", entry.top);
	entry = atlas.lookup(&atlas, "b");
	TestCase_assert(entry.left == 0.125f, "Expected 0.125 but got %f", entry.left);
	TestCase_assert(entry.right == 0.375f, "Expected 0.375 but got %f", entry.right);
	TestCase_assert(entry.bottom == 0.625f, "Expected 0.625 but got %f", entry.bottom);
	TestCase_assert(entry.top == 0.875f, "Expected 0.875 but got %f", entry.top);
	atlas.dispose(&atlas);
	
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
	atlas.dispose(&atlas);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	GLTextureAtlas_init(&atlas);
	atlas.textureName = "bar";
	atlas.setEntry(&atlas, "a", (struct GLTextureAtlas_entry) {0.0f, 0.25f, 0.5f, 0.75f});
	atlas.setEntry(&atlas, "b", (struct GLTextureAtlas_entry) {0.125f, 0.375f, 0.625f, 0.875f});
	
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
	atlas.dispose(&atlas);
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

static void testGetVertices() {
	GLTextureAtlas atlas;
	unsigned int vertexCount;
	struct vertex_p2f_t2f vertices[4];
	
	GLTextureAtlas_init(&atlas);
	atlas.textureName = "bar";
	atlas.setEntry(&atlas, "a", (struct GLTextureAtlas_entry) {0.0f, 0.25f, 0.5f, 0.75f});
	atlas.setEntry(&atlas, "b", (struct GLTextureAtlas_entry) {0.125f, 0.375f, 0.625f, 0.875f});
	
	vertexCount = atlas.getVertices(&atlas, "a", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, NULL);
	TestCase_assert(vertexCount == 4, "Expected 4 but get %u", vertexCount);
	
	memset(vertices, 0xFF, sizeof(vertices));
	vertexCount = atlas.getVertices(&atlas, "a", 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, vertices);
	TestCase_assert(vertexCount == 4, "Expected 4 but get %u", vertexCount);
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
	
	memset(vertices, 0xFF, sizeof(vertices));
	vertexCount = atlas.getVertices(&atlas, "b", 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, vertices);
	TestCase_assert(vertexCount == 4, "Expected 4 but get %u", vertexCount);
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
	
	memset(vertices, 0xFF, sizeof(vertices));
	vertexCount = atlas.getVertices(&atlas, "a", 6.0f, -3.0f, 0.5f, 1.0f, 2.0f, 1.5f, vertices);
	TestCase_assert(vertexCount == 4, "Expected 4 but get %u", vertexCount);
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
}

static void testGetIndexes() {
	GLTextureAtlas atlas;
	unsigned int indexCount;
	GLubyte indexesByte[6];
	GLushort indexesShort[6];
	GLuint indexesUint[6];
	
	GLTextureAtlas_init(&atlas);
	indexCount = atlas.getIndexes(&atlas, NULL, GL_UNSIGNED_BYTE, 0);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	
	memset(indexesByte, 0xFF, sizeof(indexesByte));
	indexCount = atlas.getIndexes(&atlas, indexesByte, GL_UNSIGNED_BYTE, 0);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	TestCase_assert(indexesByte[0] == 0, "Expected 0 but got %u", indexesByte[0]);
	TestCase_assert(indexesByte[1] == 1, "Expected 1 but got %u", indexesByte[1]);
	TestCase_assert(indexesByte[2] == 2, "Expected 2 but got %u", indexesByte[2]);
	TestCase_assert(indexesByte[3] == 2, "Expected 2 but got %u", indexesByte[3]);
	TestCase_assert(indexesByte[4] == 3, "Expected 3 but got %u", indexesByte[4]);
	TestCase_assert(indexesByte[5] == 0, "Expected 0 but got %u", indexesByte[5]);
	
	memset(indexesByte, 0xFF, sizeof(indexesByte));
	indexCount = atlas.getIndexes(&atlas, indexesByte, GL_UNSIGNED_BYTE, 4);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	TestCase_assert(indexesByte[0] == 4, "Expected 4 but got %u", indexesByte[0]);
	TestCase_assert(indexesByte[1] == 5, "Expected 5 but got %u", indexesByte[1]);
	TestCase_assert(indexesByte[2] == 6, "Expected 6 but got %u", indexesByte[2]);
	TestCase_assert(indexesByte[3] == 6, "Expected 6 but got %u", indexesByte[3]);
	TestCase_assert(indexesByte[4] == 7, "Expected 7 but got %u", indexesByte[4]);
	TestCase_assert(indexesByte[5] == 4, "Expected 4 but got %u", indexesByte[5]);
	
	memset(indexesShort, 0xFF, sizeof(indexesShort));
	indexCount = atlas.getIndexes(&atlas, indexesShort, GL_UNSIGNED_SHORT, 0);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	TestCase_assert(indexesShort[0] == 0, "Expected 0 but got %u", indexesShort[0]);
	TestCase_assert(indexesShort[1] == 1, "Expected 1 but got %u", indexesShort[1]);
	TestCase_assert(indexesShort[2] == 2, "Expected 2 but got %u", indexesShort[2]);
	TestCase_assert(indexesShort[3] == 2, "Expected 2 but got %u", indexesShort[3]);
	TestCase_assert(indexesShort[4] == 3, "Expected 3 but got %u", indexesShort[4]);
	TestCase_assert(indexesShort[5] == 0, "Expected 0 but got %u", indexesShort[5]);
	
	memset(indexesShort, 0xFF, sizeof(indexesShort));
	indexCount = atlas.getIndexes(&atlas, indexesShort, GL_UNSIGNED_SHORT, 5);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	TestCase_assert(indexesShort[0] == 5, "Expected 5 but got %u", indexesShort[0]);
	TestCase_assert(indexesShort[1] == 6, "Expected 6 but got %u", indexesShort[1]);
	TestCase_assert(indexesShort[2] == 7, "Expected 7 but got %u", indexesShort[2]);
	TestCase_assert(indexesShort[3] == 7, "Expected 7 but got %u", indexesShort[3]);
	TestCase_assert(indexesShort[4] == 8, "Expected 8 but got %u", indexesShort[4]);
	TestCase_assert(indexesShort[5] == 5, "Expected 5 but got %u", indexesShort[5]);
	
	memset(indexesUint, 0xFF, sizeof(indexesUint));
	indexCount = atlas.getIndexes(&atlas, indexesUint, GL_UNSIGNED_INT, 0);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	TestCase_assert(indexesUint[0] == 0, "Expected 0 but got %u", indexesUint[0]);
	TestCase_assert(indexesUint[1] == 1, "Expected 1 but got %u", indexesUint[1]);
	TestCase_assert(indexesUint[2] == 2, "Expected 2 but got %u", indexesUint[2]);
	TestCase_assert(indexesUint[3] == 2, "Expected 2 but got %u", indexesUint[3]);
	TestCase_assert(indexesUint[4] == 3, "Expected 3 but got %u", indexesUint[4]);
	TestCase_assert(indexesUint[5] == 0, "Expected 0 but got %u", indexesUint[5]);
	
	memset(indexesUint, 0xFF, sizeof(indexesUint));
	indexCount = atlas.getIndexes(&atlas, indexesUint, GL_UNSIGNED_INT, 6);
	TestCase_assert(indexCount == 6, "Expected 6 but got %u", indexCount);
	TestCase_assert(indexesUint[0] == 6, "Expected 6 but got %u", indexesUint[0]);
	TestCase_assert(indexesUint[1] == 7, "Expected 7 but got %u", indexesUint[1]);
	TestCase_assert(indexesUint[2] == 8, "Expected 8 but got %u", indexesUint[2]);
	TestCase_assert(indexesUint[3] == 8, "Expected 8 but got %u", indexesUint[3]);
	TestCase_assert(indexesUint[4] == 9, "Expected 9 but got %u", indexesUint[4]);
	TestCase_assert(indexesUint[5] == 6, "Expected 6 but got %u", indexesUint[5]);
}

TEST_SUITE(GLTextureAtlasTest,
           testInit,
           testSetRemoveEntry,
           testDeserialization,
           testSerialization,
           testGetVertices,
           testGetIndexes,
           testFormatVersionVerification)
