#include "glbitmapfont/GLBitmapFont.h"
#include "serialization/TestDeserializationContext.h"
#include "serialization/TestSerializationContext.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"
#include <math.h>

static void testInit() {
	GLBitmapFont font, * fontPtr;
	const char * atlasKey = "a";
	struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS];
	struct GLBitmapFont_kernEntry testKernEntry = {'a', 1.0f};
	struct GLBitmapFont_charEntry testCharEntry1 = {1.0f, 0.0f, 0.5f, NULL, 0, NULL};
	struct GLBitmapFont_charEntry testCharEntry2 = {7.0f, 6.0f, 6.5f, atlasKey, 1, &testKernEntry};
	unsigned int charIndex;
	
	characters[0] = testCharEntry1;
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		characters[charIndex] = testCharEntry2;
	}
	
	GLBitmapFont_init(&font, characters);
	TestCase_assert(font.dispose == GLBitmapFont_dispose, "Expected %p but got %p", GLBitmapFont_dispose, font.dispose);
	TestCase_assert(font.characters[0].advance == 1.0f, "Expected 1.0 but got %f", font.characters[0].advance);
	TestCase_assert(font.characters[0].glyphOffset == 0.0f, "Expected 0.0 but got %f", font.characters[0].glyphOffset);
	TestCase_assert(font.characters[0].glyphWidth == 0.5f, "Expected 0.5 but got %f", font.characters[0].glyphWidth);
	TestCase_assert(font.characters[0].atlasKey == NULL, "Expected NULL but got %p", font.characters[0].atlasKey);
	TestCase_assert(font.characters[0].kernCharCount == 0, "Expected 0 but got " SIZE_T_FORMAT, font.characters[0].kernCharCount);
	TestCase_assert(font.characters[0].kernChars == NULL, "Expected NULL but got %p", font.characters[0].kernChars);
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		TestCase_assert(font.characters[charIndex].advance == 7.0f, "Expected 7.0 but got %f", font.characters[charIndex].advance);
		TestCase_assert(font.characters[charIndex].glyphOffset == 6.0f, "Expected 6.0 but got %f", font.characters[charIndex].glyphOffset);
		TestCase_assert(font.characters[charIndex].glyphWidth == 6.5f, "Expected 6.5 but got %f", font.characters[charIndex].glyphWidth);
		TestCase_assert(font.characters[charIndex].atlasKey == atlasKey, "Expected \"a\" but got \"%s\"", font.characters[charIndex].atlasKey);
		TestCase_assert(font.characters[charIndex].kernCharCount == 1, "Expected 1 but got " SIZE_T_FORMAT, font.characters[charIndex].kernCharCount);
		TestCase_assert(font.characters[charIndex].kernChars != NULL, "Expected non-NULL but got NULL");
		TestCase_assert(font.characters[charIndex].kernChars != testCharEntry2.kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(font.characters[charIndex].kernChars != font.characters[charIndex - 1].kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(font.characters[charIndex].kernChars[0].previous == 'a', "Expected a but got %c", font.characters[charIndex].kernChars[0].previous);
		TestCase_assert(font.characters[charIndex].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", font.characters[charIndex].kernChars[0].offset);
	}
	font.dispose(&font);
	
	fontPtr = GLBitmapFont_create(characters);
	TestCase_assert(fontPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fontPtr->dispose == GLBitmapFont_dispose, "Expected %p but got %p", GLBitmapFont_dispose, fontPtr->dispose);
	TestCase_assert(fontPtr->characters[0].advance == 1.0f, "Expected 1.0 but got %f", fontPtr->characters[0].advance);
	TestCase_assert(fontPtr->characters[0].glyphOffset == 0.0f, "Expected 0.0 but got %f", fontPtr->characters[0].glyphOffset);
	TestCase_assert(fontPtr->characters[0].glyphWidth == 0.5f, "Expected 0.5 but got %f", fontPtr->characters[0].glyphWidth);
	TestCase_assert(fontPtr->characters[0].atlasKey == NULL, "Expected NULL but got %p", fontPtr->characters[0].atlasKey);
	TestCase_assert(fontPtr->characters[0].kernCharCount == 0, "Expected 0 but got " SIZE_T_FORMAT, fontPtr->characters[0].kernCharCount);
	TestCase_assert(fontPtr->characters[0].kernChars == NULL, "Expected NULL but got %p", fontPtr->characters[0].kernChars);
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		TestCase_assert(fontPtr->characters[charIndex].advance == 7.0f, "Expected 7.0 but got %f", fontPtr->characters[charIndex].advance);
		TestCase_assert(fontPtr->characters[charIndex].glyphOffset == 6.0f, "Expected 6.0 but got %f", fontPtr->characters[charIndex].glyphOffset);
		TestCase_assert(fontPtr->characters[charIndex].glyphWidth == 6.5f, "Expected 6.5 but got %f", fontPtr->characters[charIndex].glyphWidth);
		TestCase_assert(fontPtr->characters[charIndex].atlasKey == atlasKey, "Expected \"a\" but got \"%s\"", fontPtr->characters[charIndex].atlasKey);
		TestCase_assert(fontPtr->characters[charIndex].kernCharCount == 1, "Expected 1 but got " SIZE_T_FORMAT, fontPtr->characters[charIndex].kernCharCount);
		TestCase_assert(fontPtr->characters[charIndex].kernChars != NULL, "Expected non-NULL but got NULL");
		TestCase_assert(fontPtr->characters[charIndex].kernChars != testCharEntry2.kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(fontPtr->characters[charIndex].kernChars != fontPtr->characters[charIndex - 1].kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(fontPtr->characters[charIndex].kernChars[0].previous == 'a', "Expected a but got %c", fontPtr->characters[charIndex].kernChars[0].previous);
		TestCase_assert(fontPtr->characters[charIndex].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", fontPtr->characters[charIndex].kernChars[0].offset);
	}
	fontPtr->dispose(fontPtr);
	
	characters[0] = testCharEntry2;
	characters[GLBITMAPFONT_NUM_CHARS - 1] = testCharEntry1;
	
	GLBitmapFont_init(&font, characters);
	TestCase_assert(font.dispose == GLBitmapFont_dispose, "Expected %p but got %p", GLBitmapFont_dispose, font.dispose);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].advance == 1.0f, "Expected 1.0 but got %f", font.characters[GLBITMAPFONT_NUM_CHARS - 1].advance);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].glyphOffset == 0.0f, "Expected 0.0 but got %f", font.characters[GLBITMAPFONT_NUM_CHARS - 1].glyphOffset);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].glyphWidth == 0.5f, "Expected 0.5 but got %f", font.characters[GLBITMAPFONT_NUM_CHARS - 1].glyphWidth);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].atlasKey == NULL, "Expected NULL but got %p", font.characters[GLBITMAPFONT_NUM_CHARS - 1].atlasKey);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernCharCount == 0, "Expected 0 but got " SIZE_T_FORMAT, font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernCharCount);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars == NULL, "Expected NULL but got %p", font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars);
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS - 1; charIndex++) {
		TestCase_assert(font.characters[charIndex].advance == 7.0f, "Expected 7.0 but got %f", font.characters[charIndex].advance);
		TestCase_assert(font.characters[charIndex].glyphOffset == 6.0f, "Expected 6.0 but got %f", font.characters[charIndex].glyphOffset);
		TestCase_assert(font.characters[charIndex].glyphWidth == 6.5f, "Expected 6.5 but got %f", font.characters[charIndex].glyphWidth);
		TestCase_assert(font.characters[charIndex].atlasKey == atlasKey, "Expected \"a\" but got \"%s\"", font.characters[charIndex].atlasKey);
		TestCase_assert(font.characters[charIndex].kernCharCount == 1, "Expected 1 but got " SIZE_T_FORMAT, font.characters[charIndex].kernCharCount);
		TestCase_assert(font.characters[charIndex].kernChars != NULL, "Expected non-NULL but got NULL");
		TestCase_assert(font.characters[charIndex].kernChars != testCharEntry2.kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(font.characters[charIndex].kernChars != font.characters[charIndex + 1].kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(font.characters[charIndex].kernChars[0].previous == 'a', "Expected a but got %c", font.characters[charIndex].kernChars[0].previous);
		TestCase_assert(font.characters[charIndex].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", font.characters[charIndex].kernChars[0].offset);
	}
	font.dispose(&font);
	
	fontPtr = GLBitmapFont_create(characters);
	TestCase_assert(fontPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fontPtr->dispose == GLBitmapFont_dispose, "Expected %p but got %p", GLBitmapFont_dispose, fontPtr->dispose);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].advance == 1.0f, "Expected 1.0 but got %f", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].advance);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].glyphOffset == 0.0f, "Expected 0.0 but got %f", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].glyphOffset);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].glyphWidth == 0.5f, "Expected 0.5 but got %f", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].glyphWidth);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].atlasKey == NULL, "Expected NULL but got %p", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].atlasKey);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernCharCount == 0, "Expected 0 but got " SIZE_T_FORMAT, fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernCharCount);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars == NULL, "Expected NULL but got %p", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars);
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS - 1; charIndex++) {
		TestCase_assert(fontPtr->characters[charIndex].advance == 7.0f, "Expected 7.0 but got %f", fontPtr->characters[charIndex].advance);
		TestCase_assert(fontPtr->characters[charIndex].glyphOffset == 6.0f, "Expected 6.0 but got %f", fontPtr->characters[charIndex].glyphOffset);
		TestCase_assert(fontPtr->characters[charIndex].glyphWidth == 6.5f, "Expected 6.5 but got %f", fontPtr->characters[charIndex].glyphWidth);
		TestCase_assert(fontPtr->characters[charIndex].atlasKey == atlasKey, "Expected \"a\" but got \"%s\"", fontPtr->characters[charIndex].atlasKey);
		TestCase_assert(fontPtr->characters[charIndex].kernCharCount == 1, "Expected 1 but got " SIZE_T_FORMAT, fontPtr->characters[charIndex].kernCharCount);
		TestCase_assert(fontPtr->characters[charIndex].kernChars != NULL, "Expected non-NULL but got NULL");
		TestCase_assert(fontPtr->characters[charIndex].kernChars != testCharEntry2.kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(fontPtr->characters[charIndex].kernChars != fontPtr->characters[charIndex + 1].kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(fontPtr->characters[charIndex].kernChars[0].previous == 'a', "Expected a but got %c", fontPtr->characters[charIndex].kernChars[0].previous);
		TestCase_assert(fontPtr->characters[charIndex].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", fontPtr->characters[charIndex].kernChars[0].offset);
	}
	fontPtr->dispose(fontPtr);
}

static void testDeserialization() {
	GLBitmapFont font, * fontPtr;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	unsigned int charIndex;
	char charStrings[GLBITMAPFONT_NUM_CHARS][2];
	char * kernKey1 = "a", * kernKey2 = "b";
	bool success;
	unsigned int failIndex;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charStrings[0][0] = ' ';
	charStrings[0][1] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charStrings[0]);
	context->expectCall(context, context->beginStructure, charStrings[0], (size_t) 4);
	context->expectCall(context, context->readFloat, "advance", 2.0f);
	context->expectCall(context, context->readFloat, "glyph_offset", 1.0f);
	context->expectCall(context, context->readFloat, "glyph_width", 1.5f);
	context->expectCall(context, context->readString, "atlas_key", "a");
	context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 2);
	context->expectCall(context, context->readNextDictionaryKey, kernKey1);
	context->expectCall(context, context->readFloat, kernKey1, -1.0f);
	context->expectCall(context, context->readNextDictionaryKey, kernKey2);
	context->expectCall(context, context->readFloat, kernKey2, -2.0f);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		charStrings[charIndex][0] = ' ' + charIndex;
		charStrings[charIndex][1] = 0;
		context->expectCall(context, context->readNextDictionaryKey, charStrings[charIndex]);
		context->expectCall(context, context->beginStructure, charStrings[charIndex], (size_t) 4);
		context->expectCall(context, context->readFloat, "advance", 4.0f);
		context->expectCall(context, context->readFloat, "glyph_offset", 3.0f);
		context->expectCall(context, context->readFloat, "glyph_width", 3.5f);
		context->expectCall(context, context->readString, "atlas_key", "b");
		context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 0);
		context->expectCall(context, context->endDictionary);
		context->expectCall(context, context->endStructure);
	}
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	memset(&font, 0x00, sizeof(font));
	success = GLBitmapFont_loadSerializedData(&font, (DeserializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(!strcmp(font.atlasName, "foo"), "Expected \"foo\" but got \"%s\"", font.atlasName);
	TestCase_assert(font.atlas == NULL, "Expected NULL but got %p", font.atlas);
	TestCase_assert(font.characters[0].advance == 2.0f, "Expected 2.0 but got %f", font.characters[0].advance);
	TestCase_assert(font.characters[0].glyphOffset == 1.0f, "Expected 1.0 but got %f", font.characters[0].glyphOffset);
	TestCase_assert(font.characters[0].glyphWidth == 1.5f, "Expected 1.5 but got %f", font.characters[0].glyphWidth);
	TestCase_assert(!strcmp(font.characters[0].atlasKey, "a"), "Expected \"a\" but got \"%s\"", font.characters[0].atlasKey);
	TestCase_assert(font.characters[0].kernCharCount == 2, "Expected 2 but got " SIZE_T_FORMAT, font.characters[0].kernCharCount);
	TestCase_assert(font.characters[0].kernChars != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(font.characters[0].kernChars[0].previous == 'a', "Expected 'a' but got '%c'", font.characters[0].kernChars[0].previous);
	TestCase_assert(font.characters[0].kernChars[0].offset == -1.0f, "Expected -1.0 but got %f", font.characters[0].kernChars[0].offset);
	TestCase_assert(font.characters[0].kernChars[1].previous == 'b', "Expected 'b' but got '%c'", font.characters[0].kernChars[1].previous);
	TestCase_assert(font.characters[0].kernChars[1].offset == -2.0f, "Expected -2.0 but got %f", font.characters[0].kernChars[1].offset);
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		TestCase_assert(font.characters[charIndex].advance == 4.0f, "Expected 4.0 but got %f (char %u)", font.characters[charIndex].advance, charIndex);
		TestCase_assert(font.characters[charIndex].glyphOffset == 3.0f, "Expected 3.0 but got %f (char %u)", font.characters[charIndex].glyphOffset, charIndex);
		TestCase_assert(font.characters[charIndex].glyphWidth == 3.5f, "Expected 3.5 but got %f (char %u)", font.characters[charIndex].glyphWidth, charIndex);
		TestCase_assert(!strcmp(font.characters[charIndex].atlasKey, "b"), "Expected \"b\" but got \"%s\" (char %u)", font.characters[charIndex].atlasKey, charIndex);
		TestCase_assert(font.characters[charIndex].kernCharCount == 0, "Expected 0 but got " SIZE_T_FORMAT " (char %u)", font.characters[charIndex].kernCharCount, charIndex);
		TestCase_assert(font.characters[charIndex].kernChars == NULL, "Expected NULL but got %p (char %u)", font.characters[charIndex].kernChars, charIndex);
	}
	font.dispose(&font);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "bar");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charStrings[0][0] = '~';
	charStrings[0][1] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charStrings[0]);
	context->expectCall(context, context->beginStructure, charStrings[0], (size_t) 4);
	context->expectCall(context, context->readFloat, "advance", 4.0f);
	context->expectCall(context, context->readFloat, "glyph_offset", 3.0f);
	context->expectCall(context, context->readFloat, "glyph_width", 3.5f);
	context->expectCall(context, context->readString, "atlas_key", "c");
	context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 1);
	context->expectCall(context, context->readNextDictionaryKey, kernKey2);
	context->expectCall(context, context->readFloat, kernKey2, 1.0f);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		charStrings[charIndex][0] = ' ' + charIndex - 1;
		charStrings[charIndex][1] = 0;
		context->expectCall(context, context->readNextDictionaryKey, charStrings[charIndex]);
		context->expectCall(context, context->beginStructure, charStrings[charIndex], (size_t) 4);
		context->expectCall(context, context->readFloat, "advance", 5.0f);
		context->expectCall(context, context->readFloat, "glyph_offset", 4.0f);
		context->expectCall(context, context->readFloat, "glyph_width", 4.5f);
		context->expectCall(context, context->readString, "atlas_key", "d");
		context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 0);
		context->expectCall(context, context->endDictionary);
		context->expectCall(context, context->endStructure);
	}
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	memset(&font, 0x00, sizeof(font));
	success = GLBitmapFont_loadSerializedData(&font, (DeserializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(!strcmp(font.atlasName, "bar"), "Expected \"bar\" but got \"%s\"", font.atlasName);
	TestCase_assert(font.atlas == NULL, "Expected NULL but got %p", font.atlas);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].advance == 4.0f, "Expected 4.0 but got %f", font.characters[GLBITMAPFONT_NUM_CHARS - 1].advance);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].glyphOffset == 3.0f, "Expected 3.0 but got %f", font.characters[GLBITMAPFONT_NUM_CHARS - 1].glyphOffset);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].glyphWidth == 3.5f, "Expected 3.5 but got %f", font.characters[GLBITMAPFONT_NUM_CHARS - 1].glyphWidth);
	TestCase_assert(!strcmp(font.characters[GLBITMAPFONT_NUM_CHARS - 1].atlasKey, "c"), "Expected \"c\" but got \"%s\"", font.characters[GLBITMAPFONT_NUM_CHARS - 1].atlasKey);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernCharCount == 1, "Expected 1 but got " SIZE_T_FORMAT, font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernCharCount);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars[0].previous == 'b', "Expected 'a' but got '%c'", font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars[0].previous);
	TestCase_assert(font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", font.characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars[0].offset);
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS - 1; charIndex++) {
		TestCase_assert(font.characters[charIndex].advance == 5.0f, "Expected 5.0 but got %f (char %u)", font.characters[charIndex].advance, charIndex);
		TestCase_assert(font.characters[charIndex].glyphOffset == 4.0f, "Expected 4.0 but got %f (char %u)", font.characters[charIndex].glyphOffset, charIndex);
		TestCase_assert(font.characters[charIndex].glyphWidth == 4.5f, "Expected 4.5 but got %f (char %u)", font.characters[charIndex].glyphWidth, charIndex);
		TestCase_assert(!strcmp(font.characters[charIndex].atlasKey, "d"), "Expected \"d\" but got \"%s\" (char %u)", font.characters[charIndex].atlasKey, charIndex);
		TestCase_assert(font.characters[charIndex].kernCharCount == 0, "Expected 0 but got " SIZE_T_FORMAT " (char %u)", font.characters[charIndex].kernCharCount, charIndex);
		TestCase_assert(font.characters[charIndex].kernChars == NULL, "Expected NULL but got %p (char %u)", font.characters[charIndex].kernChars, charIndex);
	}
	font.dispose(&font);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charStrings[0][0] = ' ';
	charStrings[0][1] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charStrings[0]);
	context->expectCall(context, context->beginStructure, charStrings[0], (size_t) 4);
	context->expectCall(context, context->readFloat, "advance", 2.0f);
	context->expectCall(context, context->readFloat, "glyph_offset", 1.0f);
	context->expectCall(context, context->readFloat, "glyph_width", 1.5f);
	context->expectCall(context, context->readString, "atlas_key", "e");
	context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 2);
	context->expectCall(context, context->readNextDictionaryKey, kernKey1);
	context->expectCall(context, context->readFloat, kernKey1, -1.0f);
	context->expectCall(context, context->readNextDictionaryKey, kernKey2);
	context->expectCall(context, context->readFloat, kernKey2, -2.0f);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		charStrings[charIndex][0] = ' ' + charIndex;
		charStrings[charIndex][1] = 0;
		context->expectCall(context, context->readNextDictionaryKey, charStrings[charIndex]);
		context->expectCall(context, context->beginStructure, charStrings[charIndex], (size_t) 4);
		context->expectCall(context, context->readFloat, "advance", 4.0f);
		context->expectCall(context, context->readFloat, "glyph_offset", 3.0f);
		context->expectCall(context, context->readFloat, "glyph_width", 3.5f);
		context->expectCall(context, context->readString, "atlas_key", "f");
		context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 0);
		context->expectCall(context, context->endDictionary);
		context->expectCall(context, context->endStructure);
	}
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	fontPtr = GLBitmapFont_deserialize((DeserializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(fontPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(fontPtr->atlasName, "foo"), "Expected \"foo\" but got \"%s\"", fontPtr->atlasName);
	TestCase_assert(fontPtr->atlas == NULL, "Expected NULL but got %p", fontPtr->atlas);
	TestCase_assert(fontPtr->characters[0].advance == 2.0f, "Expected 2.0 but got %f", fontPtr->characters[0].advance);
	TestCase_assert(fontPtr->characters[0].glyphOffset == 1.0f, "Expected 1.0 but got %f", fontPtr->characters[0].glyphOffset);
	TestCase_assert(fontPtr->characters[0].glyphWidth == 1.5f, "Expected 1.5 but got %f", fontPtr->characters[0].glyphWidth);
	TestCase_assert(!strcmp(fontPtr->characters[0].atlasKey, "e"), "Expected \"e\" but got \"%s\"", fontPtr->characters[0].atlasKey);
	TestCase_assert(fontPtr->characters[0].kernCharCount == 2, "Expected 2 but got " SIZE_T_FORMAT, fontPtr->characters[0].kernCharCount);
	TestCase_assert(fontPtr->characters[0].kernChars != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fontPtr->characters[0].kernChars[0].previous == 'a', "Expected 'a' but got '%c'", fontPtr->characters[0].kernChars[0].previous);
	TestCase_assert(fontPtr->characters[0].kernChars[0].offset == -1.0f, "Expected -1.0 but got %f", fontPtr->characters[0].kernChars[0].offset);
	TestCase_assert(fontPtr->characters[0].kernChars[1].previous == 'b', "Expected 'b' but got '%c'", fontPtr->characters[0].kernChars[1].previous);
	TestCase_assert(fontPtr->characters[0].kernChars[1].offset == -2.0f, "Expected -2.0 but got %f", fontPtr->characters[0].kernChars[1].offset);
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		TestCase_assert(fontPtr->characters[charIndex].advance == 4.0f, "Expected 4.0 but got %f (char %u)", fontPtr->characters[charIndex].advance, charIndex);
		TestCase_assert(fontPtr->characters[charIndex].glyphOffset == 3.0f, "Expected 3.0 but got %f (char %u)", fontPtr->characters[charIndex].glyphOffset, charIndex);
		TestCase_assert(fontPtr->characters[charIndex].glyphWidth == 3.5f, "Expected 3.5 but got %f (char %u)", fontPtr->characters[charIndex].glyphWidth, charIndex);
		TestCase_assert(!strcmp(fontPtr->characters[charIndex].atlasKey, "f"), "Expected \"f\" but got \"%s\" (char %u)", fontPtr->characters[charIndex].atlasKey, charIndex);
		TestCase_assert(fontPtr->characters[charIndex].kernCharCount == 0, "Expected 0 but got " SIZE_T_FORMAT " (char %u)", fontPtr->characters[charIndex].kernCharCount, charIndex);
		TestCase_assert(fontPtr->characters[charIndex].kernChars == NULL, "Expected NULL but got %p (char %u)", fontPtr->characters[charIndex].kernChars, charIndex);
	}
	fontPtr->dispose(fontPtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "bar");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charStrings[0][0] = '~';
	charStrings[0][1] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charStrings[0]);
	context->expectCall(context, context->beginStructure, charStrings[0], (size_t) 4);
	context->expectCall(context, context->readFloat, "advance", 4.0f);
	context->expectCall(context, context->readFloat, "glyph_offset", 3.0f);
	context->expectCall(context, context->readFloat, "glyph_width", 3.5f);
	context->expectCall(context, context->readString, "atlas_key", "g");
	context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 1);
	context->expectCall(context, context->readNextDictionaryKey, kernKey2);
	context->expectCall(context, context->readFloat, kernKey2, 1.0f);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		charStrings[charIndex][0] = ' ' + charIndex - 1;
		charStrings[charIndex][1] = 0;
		context->expectCall(context, context->readNextDictionaryKey, charStrings[charIndex]);
		context->expectCall(context, context->beginStructure, charStrings[charIndex], (size_t) 4);
		context->expectCall(context, context->readFloat, "advance", 5.0f);
		context->expectCall(context, context->readFloat, "glyph_offset", 4.0f);
		context->expectCall(context, context->readFloat, "glyph_width", 4.5f);
		context->expectCall(context, context->readString, "atlas_key", "h");
		context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 0);
		context->expectCall(context, context->endDictionary);
		context->expectCall(context, context->endStructure);
	}
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	fontPtr = GLBitmapFont_deserialize((DeserializationContext *) context);
	
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(fontPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(fontPtr->atlasName, "bar"), "Expected \"bar\" but got \"%s\"", fontPtr->atlasName);
	TestCase_assert(fontPtr->atlas == NULL, "Expected NULL but got %p", fontPtr->atlas);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].advance == 4.0f, "Expected 4.0 but got %f", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].advance);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].glyphOffset == 3.0f, "Expected 3.0 but got %f", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].glyphOffset);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].glyphWidth == 3.5f, "Expected 3.5 but got %f", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].glyphWidth);
	TestCase_assert(!strcmp(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].atlasKey, "g"), "Expected \"g\" but got \"%s\"", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].atlasKey);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernCharCount == 1, "Expected 1 but got " SIZE_T_FORMAT, fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernCharCount);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars[0].previous == 'b', "Expected 'a' but got '%c'", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars[0].previous);
	TestCase_assert(fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", fontPtr->characters[GLBITMAPFONT_NUM_CHARS - 1].kernChars[0].offset);
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS - 1; charIndex++) {
		TestCase_assert(fontPtr->characters[charIndex].advance == 5.0f, "Expected 5.0 but got %f (char %u)", fontPtr->characters[charIndex].advance, charIndex);
		TestCase_assert(fontPtr->characters[charIndex].glyphOffset == 4.0f, "Expected 4.0 but got %f (char %u)", fontPtr->characters[charIndex].glyphOffset, charIndex);
		TestCase_assert(fontPtr->characters[charIndex].glyphWidth == 4.5f, "Expected 4.5 but got %f (char %u)", fontPtr->characters[charIndex].glyphWidth, charIndex);
		TestCase_assert(!strcmp(fontPtr->characters[charIndex].atlasKey, "h"), "Expected \"h\" but got \"%s\" (char %u)", fontPtr->characters[charIndex].atlasKey, charIndex);
		TestCase_assert(fontPtr->characters[charIndex].kernCharCount == 0, "Expected 0 but got " SIZE_T_FORMAT " (char %u)", fontPtr->characters[charIndex].kernCharCount, charIndex);
		TestCase_assert(fontPtr->characters[charIndex].kernChars == NULL, "Expected NULL but got %p (char %u)", fontPtr->characters[charIndex].kernChars, charIndex);
	}
	fontPtr->dispose(fontPtr);
	
	for (failIndex = 0; failIndex < 19 + 9 * (GLBITMAPFONT_NUM_CHARS - 1); failIndex++) {
		context = TestDeserializationContext_create(&jmpEnv);
		if (setjmp(jmpEnv) != 0) {
			TestCase_assert(false, "%s", context->error);
		}
		
		context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
		context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
		context->expectCall(context, context->readString, "atlas_name", "foo");
		context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
		charStrings[0][0] = ' ';
		charStrings[0][1] = 0;
		context->expectCall(context, context->readNextDictionaryKey, charStrings[0]);
		context->expectCall(context, context->beginStructure, charStrings[0], (size_t) 4);
		context->expectCall(context, context->readFloat, "advance", 2.0f);
		context->expectCall(context, context->readFloat, "glyph_offset", 1.0f);
		context->expectCall(context, context->readFloat, "glyph_width", 1.5f);
		context->expectCall(context, context->readString, "atlas_key", "i");
		context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 2);
		context->expectCall(context, context->readNextDictionaryKey, kernKey1);
		context->expectCall(context, context->readFloat, kernKey1, -1.0f);
		context->expectCall(context, context->readNextDictionaryKey, kernKey2);
		context->expectCall(context, context->readFloat, kernKey2, -2.0f);
		context->expectCall(context, context->endDictionary);
		context->expectCall(context, context->endStructure);
		for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
			charStrings[charIndex][0] = ' ' + charIndex;
			charStrings[charIndex][1] = 0;
			context->expectCall(context, context->readNextDictionaryKey, charStrings[charIndex]);
			context->expectCall(context, context->beginStructure, charStrings[charIndex], (size_t) 4);
			context->expectCall(context, context->readFloat, "advance", 4.0f);
			context->expectCall(context, context->readFloat, "glyph_offset", 3.0f);
			context->expectCall(context, context->readFloat, "glyph_width", 3.5f);
			context->expectCall(context, context->readString, "atlas_key", "j");
			context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 0);
			context->expectCall(context, context->endDictionary);
			context->expectCall(context, context->endStructure);
		}
		context->expectCall(context, context->endDictionary);
		context->expectCall(context, context->endStructure);
		context->failNthCall(context, failIndex, 1);
		
		memset(&font, 0x00, sizeof(font));
		success = GLBitmapFont_loadSerializedData(&font, (DeserializationContext *) context);
		
		context->finish(context);
		context->dispose(context);
		
		TestCase_assert(!success, "GLBitmapFont_loadSerializedData didn't return false when deserialization call %d failed", failIndex);
		
		context = TestDeserializationContext_create(&jmpEnv);
		if (setjmp(jmpEnv) != 0) {
			TestCase_assert(false, "%s", context->error);
		}
		
		context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
		context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
		context->expectCall(context, context->readString, "atlas_name", "foo");
		context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
		charStrings[0][0] = ' ';
		charStrings[0][1] = 0;
		context->expectCall(context, context->readNextDictionaryKey, charStrings[0]);
		context->expectCall(context, context->beginStructure, charStrings[0], (size_t) 4);
		context->expectCall(context, context->readFloat, "advance", 2.0f);
		context->expectCall(context, context->readFloat, "glyph_offset", 1.0f);
		context->expectCall(context, context->readFloat, "glyph_width", 1.5f);
		context->expectCall(context, context->readString, "atlas_key", "k");
		context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 2);
		context->expectCall(context, context->readNextDictionaryKey, kernKey1);
		context->expectCall(context, context->readFloat, kernKey1, -1.0f);
		context->expectCall(context, context->readNextDictionaryKey, kernKey2);
		context->expectCall(context, context->readFloat, kernKey2, -2.0f);
		context->expectCall(context, context->endDictionary);
		context->expectCall(context, context->endStructure);
		for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
			charStrings[charIndex][0] = ' ' + charIndex;
			charStrings[charIndex][1] = 0;
			context->expectCall(context, context->readNextDictionaryKey, charStrings[charIndex]);
			context->expectCall(context, context->beginStructure, charStrings[charIndex], (size_t) 4);
			context->expectCall(context, context->readFloat, "advance", 4.0f);
			context->expectCall(context, context->readFloat, "glyph_offset", 3.0f);
			context->expectCall(context, context->readFloat, "glyph_width", 3.5f);
			context->expectCall(context, context->readString, "atlas_key", "l");
			context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 0);
			context->expectCall(context, context->endDictionary);
			context->expectCall(context, context->endStructure);
		}
		context->expectCall(context, context->endDictionary);
		context->expectCall(context, context->endStructure);
		context->failNthCall(context, failIndex, 1);
		
		fontPtr = GLBitmapFont_deserialize((DeserializationContext *) context);
		
		context->finish(context);
		context->dispose(context);
		
		TestCase_assert(fontPtr == NULL, "GLBitmapFont_deserialize didn't return NULL when deserialization call %d failed", failIndex);
	}
}

static void testSerialization() {
	GLBitmapFont * font;
	struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS];
	struct GLBitmapFont_kernEntry testKernEntry = {'a', 1.0f};
	struct GLBitmapFont_charEntry testCharEntry1 = {1.0f, 0.0f, 0.5f, "a", 0, NULL};
	struct GLBitmapFont_charEntry testCharEntry2 = {7.0f, 6.0f, 6.5f, "b", 1, &testKernEntry};
	unsigned int charIndex;
	TestSerializationContext * context;
	jmp_buf jmpEnv;
	char charStrings[GLBITMAPFONT_NUM_CHARS][2];
	
	characters[GLBITMAPFONT_NUM_CHARS - 1] = testCharEntry2;
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS - 1; charIndex++) {
		characters[charIndex] = testCharEntry1;
	}
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "glbitmapfont");
	context->expectCall(context, context->writeUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->writeString, "atlas_name", "bar");
	context->expectCall(context, context->beginDictionary, "characters");
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS - 1; charIndex++) {
		charStrings[charIndex][0] = ' ' + charIndex;
		charStrings[charIndex][1] = 0;
		context->expectCall(context, context->beginStructure, charStrings[charIndex]);
		context->expectCall(context, context->writeFloat, "advance", 1.0f);
		context->expectCall(context, context->writeFloat, "glyph_offset", 0.0f);
		context->expectCall(context, context->writeFloat, "glyph_width", 0.5f);
		context->expectCall(context, context->writeString, "atlas_key", "a");
		context->expectCall(context, context->beginDictionary, "kerning_table");
		context->expectCall(context, context->endDictionary);
		context->expectCall(context, context->endStructure);
	}
	context->expectCall(context, context->beginStructure, "~");
	context->expectCall(context, context->writeFloat, "advance", 7.0f);
	context->expectCall(context, context->writeFloat, "glyph_offset", 6.0f);
	context->expectCall(context, context->writeFloat, "glyph_width", 6.5f);
	context->expectCall(context, context->writeString, "atlas_key", "b");
	context->expectCall(context, context->beginDictionary, "kerning_table");
	context->expectCall(context, context->writeFloat, "a", 1.0f);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	font = GLBitmapFont_create(characters);
	font->atlasName = "bar";
	GLBitmapFont_serialize(font, (SerializationContext *) context);
	font->dispose(font);
	
	context->finish(context);
	context->dispose(context);
	
	characters[0] = testCharEntry1;
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		characters[charIndex] = testCharEntry2;
	}
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "glbitmapfont");
	context->expectCall(context, context->writeUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->writeString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters");
	context->expectCall(context, context->beginStructure, " ");
	context->expectCall(context, context->writeFloat, "advance", 1.0f);
	context->expectCall(context, context->writeFloat, "glyph_offset", 0.0f);
	context->expectCall(context, context->writeFloat, "glyph_width", 0.5f);
	context->expectCall(context, context->writeString, "atlas_key", "a");
	context->expectCall(context, context->beginDictionary, "kerning_table");
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	for (charIndex = 1; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		charStrings[charIndex][0] = ' ' + charIndex;
		charStrings[charIndex][1] = 0;
		context->expectCall(context, context->beginStructure, charStrings[charIndex]);
		context->expectCall(context, context->writeFloat, "advance", 7.0f);
		context->expectCall(context, context->writeFloat, "glyph_offset", 6.0f);
		context->expectCall(context, context->writeFloat, "glyph_width", 6.5f);
		context->expectCall(context, context->writeString, "atlas_key", "b");
		context->expectCall(context, context->beginDictionary, "kerning_table");
		context->expectCall(context, context->writeFloat, "a", 1.0f);
		context->expectCall(context, context->endDictionary);
		context->expectCall(context, context->endStructure);
	}
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	font = GLBitmapFont_create(characters);
	font->atlasName = "foo";
	GLBitmapFont_serialize(font, (SerializationContext *) context);
	font->dispose(font);
	
	context->finish(context);
	context->dispose(context);
}

static void testDeserializationErrors() {
	GLBitmapFont font, * fontPtr;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	char charString[3];
	bool success;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charString[0] = '\x1F';
	charString[1] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charString);
	success = GLBitmapFont_loadSerializedData(&font, (DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(!success, "Expected false but got true");
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charString[0] = '\x1F';
	charString[1] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charString);
	fontPtr = GLBitmapFont_deserialize((DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(fontPtr == NULL, "Expected NULL but got %p", fontPtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charString[0] = '\x7F';
	charString[1] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charString);
	success = GLBitmapFont_loadSerializedData(&font, (DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(!success, "Expected false but got true");
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charString[0] = '\x7F';
	charString[1] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charString);
	fontPtr = GLBitmapFont_deserialize((DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(fontPtr == NULL, "Expected NULL but got %p", fontPtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charString[0] = ' ';
	charString[1] = ' ';
	charString[2] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charString);
	success = GLBitmapFont_loadSerializedData(&font, (DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(!success, "Expected false but got true");
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charString[0] = ' ';
	charString[1] = ' ';
	charString[2] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charString);
	fontPtr = GLBitmapFont_deserialize((DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(fontPtr == NULL, "Expected NULL but got %p", fontPtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charString[0] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charString);
	success = GLBitmapFont_loadSerializedData(&font, (DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(!success, "Expected false but got true");
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	charString[0] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charString);
	fontPtr = GLBitmapFont_deserialize((DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(fontPtr == NULL, "Expected NULL but got %p", fontPtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS - 1);
	success = GLBitmapFont_loadSerializedData(&font, (DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(!success, "Expected false but got true");
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS - 1);
	fontPtr = GLBitmapFont_deserialize((DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(fontPtr == NULL, "Expected NULL but got %p", fontPtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS + 1);
	success = GLBitmapFont_loadSerializedData(&font, (DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(!success, "Expected false but got true");
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS + 1);
	fontPtr = GLBitmapFont_deserialize((DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(fontPtr == NULL, "Expected NULL but got %p", fontPtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	context->expectCall(context, context->readNextDictionaryKey, " ");
	context->expectCall(context, context->beginStructure, " ", (size_t) 4);
	context->expectCall(context, context->readFloat, "advance", 2.0f);
	context->expectCall(context, context->readFloat, "glyph_offset", 1.0f);
	context->expectCall(context, context->readFloat, "glyph_width", 1.5f);
	context->expectCall(context, context->readString, "atlas_key", "a");
	context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 2);
	context->expectCall(context, context->readNextDictionaryKey, "a");
	context->expectCall(context, context->readFloat, "a", -1.0f);
	context->expectCall(context, context->readNextDictionaryKey, "a");
	success = GLBitmapFont_loadSerializedData(&font, (DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(!success, "Expected false but got true");
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->readString, "atlas_name", "foo");
	context->expectCall(context, context->beginDictionary, "characters", (size_t) GLBITMAPFONT_NUM_CHARS);
	context->expectCall(context, context->readNextDictionaryKey, " ");
	context->expectCall(context, context->beginStructure, " ", (size_t) 4);
	context->expectCall(context, context->readFloat, "advance", 2.0f);
	context->expectCall(context, context->readFloat, "glyph_offset", 1.0f);
	context->expectCall(context, context->readFloat, "glyph_width", 1.5f);
	context->expectCall(context, context->readString, "atlas_key", "b");
	context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 2);
	context->expectCall(context, context->readNextDictionaryKey, "a");
	context->expectCall(context, context->readFloat, "a", -1.0f);
	context->expectCall(context, context->readNextDictionaryKey, "a");
	fontPtr = GLBitmapFont_deserialize((DeserializationContext *) context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(fontPtr == NULL, "Expected NULL but got %p", fontPtr);
}

static void writeMeasurementTestChars1(struct GLBitmapFont_charEntry * characters) {
	unsigned int charIndex;
	static struct GLBitmapFont_kernEntry kernEntries[3] = {{'b', -0.25f}, {'c', -0.5f}, {'d', -1.0f}};
	
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		characters[charIndex].kernCharCount = 0;
		characters[charIndex].kernChars = NULL;
	}
	characters['a' - ' '].advance = 1.0f;
	characters['b' - ' '].advance = 4.0f;
	characters['c' - ' '].advance = 16.0f;
	characters['d' - ' '].advance = 64.0f;
	characters['d' - ' '].kernCharCount = 3;
	characters['d' - ' '].kernChars = kernEntries;
}

static void writeMeasurementTestChars2(struct GLBitmapFont_charEntry * characters) {
	unsigned int charIndex;
	static struct GLBitmapFont_kernEntry kernEntries[3] = {{'a', -0.5f}, {'b', -0.125f}, {'d', -0.25f}};
	
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		characters[charIndex].kernCharCount = 0;
		characters[charIndex].kernChars = NULL;
	}
	characters['a' - ' '].advance = 8.0f;
	characters['b' - ' '].advance = 0.5f;
	characters['c' - ' '].advance = 2.0f;
	characters['d' - ' '].advance = 32.0f;
	characters['d' - ' '].kernCharCount = 3;
	characters['d' - ' '].kernChars = kernEntries;
}

#define TEST_EPSILON 0.00001f

static void testMeasureString() {
	struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS];
	GLBitmapFont * font;
	float width;
	
	writeMeasurementTestChars1(characters);
	font = GLBitmapFont_create(characters);
	width = GLBitmapFont_measureString(font, "a", 1);
	TestCase_assert(fabs(width - 1.0f) < TEST_EPSILON, "Expected 1.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "b", 1);
	TestCase_assert(fabs(width - 4.0f) < TEST_EPSILON, "Expected 4.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "c", 1);
	TestCase_assert(fabs(width - 16.0f) < TEST_EPSILON, "Expected 16.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "aa", 2);
	TestCase_assert(fabs(width - 2.0f) < TEST_EPSILON, "Expected 2.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "ab", 2);
	TestCase_assert(fabs(width - 5.0f) < TEST_EPSILON, "Expected 5.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "ac", 2);
	TestCase_assert(fabs(width - 17.0f) < TEST_EPSILON, "Expected 17.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "bb", 2);
	TestCase_assert(fabs(width - 8.0f) < TEST_EPSILON, "Expected 8.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "bc", 2);
	TestCase_assert(fabs(width - 20.0f) < TEST_EPSILON, "Expected 20.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "cc", 2);
	TestCase_assert(fabs(width - 32.0f) < TEST_EPSILON, "Expected 32.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "\tabc\x7F", 5);
	TestCase_assert(fabs(width - 21.0f) < TEST_EPSILON, "Expected 21.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "ad", 2);
	TestCase_assert(fabs(width - 65.0f) < TEST_EPSILON, "Expected 65.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "bd", 2);
	TestCase_assert(fabs(width - 67.75f) < TEST_EPSILON, "Expected 67.75 but got %f", width);
	width = GLBitmapFont_measureString(font, "cd", 2);
	TestCase_assert(fabs(width - 79.5f) < TEST_EPSILON, "Expected 79.5 but got %f", width);
	font->dispose(font);
	
	writeMeasurementTestChars2(characters);
	font = GLBitmapFont_create(characters);
	width = GLBitmapFont_measureString(font, "a", 1);
	TestCase_assert(fabs(width - 8.0f) < TEST_EPSILON, "Expected 8.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "b", 1);
	TestCase_assert(fabs(width - 0.5f) < TEST_EPSILON, "Expected 0.5 but got %f", width);
	width = GLBitmapFont_measureString(font, "c", 1);
	TestCase_assert(fabs(width - 2.0f) < TEST_EPSILON, "Expected 2.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "aa", 2);
	TestCase_assert(fabs(width - 16.0f) < TEST_EPSILON, "Expected 16.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "ab", 2);
	TestCase_assert(fabs(width - 8.5f) < TEST_EPSILON, "Expected 8.5 but got %f", width);
	width = GLBitmapFont_measureString(font, "ac", 2);
	TestCase_assert(fabs(width - 10.0f) < TEST_EPSILON, "Expected 10.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "bb", 2);
	TestCase_assert(fabs(width - 1.0f) < TEST_EPSILON, "Expected 1.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "bc", 2);
	TestCase_assert(fabs(width - 2.5f) < TEST_EPSILON, "Expected 2.5 but got %f", width);
	width = GLBitmapFont_measureString(font, "cc", 2);
	TestCase_assert(fabs(width - 4.0f) < TEST_EPSILON, "Expected 4.0 but got %f", width);
	width = GLBitmapFont_measureString(font, "\tabc\x7F", 5);
	TestCase_assert(fabs(width - 10.5f) < TEST_EPSILON, "Expected 10.5 but got %f", width);
	width = GLBitmapFont_measureString(font, "ad", 2);
	TestCase_assert(fabs(width - 39.5f) < TEST_EPSILON, "Expected 39.5 but got %f", width);
	width = GLBitmapFont_measureString(font, "bd", 2);
	TestCase_assert(fabs(width - 32.375f) < TEST_EPSILON, "Expected 32.375 but got %f", width);
	width = GLBitmapFont_measureString(font, "cd", 2);
	TestCase_assert(fabs(width - 34.0f) < TEST_EPSILON, "Expected 34.0 but got %f", width);
	font->dispose(font);
}

static void testIndexAtWidth() {
	struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS];
	GLBitmapFont * font;
	size_t charIndex;
	bool leadingEdge;
	
	writeMeasurementTestChars1(characters);
	font = GLBitmapFont_create(characters);
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, -1.0f, NULL);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, -1.0f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 0.49f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 0.51f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 2.99f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 3.01f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 12.99f, &leadingEdge);
	TestCase_assert(charIndex == 2, "Expected 2 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 13.01f, &leadingEdge);
	TestCase_assert(charIndex == 2, "Expected 2 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 22.0f, &leadingEdge);
	TestCase_assert(charIndex == 2, "Expected 2 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	
	charIndex = GLBitmapFont_indexAtWidth(font, "ba", 2, 1.99f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "ba", 2, 2.01f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "ba", 2, 4.49f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "ba", 2, 4.51f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "ba", 2, 5.01f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	
	charIndex = GLBitmapFont_indexAtWidth(font, "cdd", 3, 7.865f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "cdd", 3, 7.885f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "cdd", 3, 47.365f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "cdd", 3, 47.385f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "cdd", 3, 110.74f, &leadingEdge);
	TestCase_assert(charIndex == 2, "Expected 2 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "cdd", 3, 110.76f, &leadingEdge);
	TestCase_assert(charIndex == 2, "Expected 2 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "\tcdd\x7F", 5, 110.74f, &leadingEdge);
	TestCase_assert(charIndex == 3, "Expected 3 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "\tcdd\x7F", 5, 110.76f, &leadingEdge);
	TestCase_assert(charIndex == 3, "Expected 3 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	font->dispose(font);
	
	writeMeasurementTestChars2(characters);
	font = GLBitmapFont_create(characters);
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, -1.0f, NULL);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, -1.0f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 3.99f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 4.01f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 8.24f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 8.26f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 9.49f, &leadingEdge);
	TestCase_assert(charIndex == 2, "Expected 2 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 9.51f, &leadingEdge);
	TestCase_assert(charIndex == 2, "Expected 2 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "abc", 3, 11.5f, &leadingEdge);
	TestCase_assert(charIndex == 2, "Expected 2 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	
	charIndex = GLBitmapFont_indexAtWidth(font, "ba", 2, 0.24f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "ba", 2, 0.26f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "ba", 2, 4.49f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "ba", 2, 4.51f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "ba", 2, 9.5f, NULL);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	
	charIndex = GLBitmapFont_indexAtWidth(font, "add", 3, 3.865f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "add", 3, 3.885f, &leadingEdge);
	TestCase_assert(charIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "add", 3, 23.5525f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "add", 3, 23.5725f, &leadingEdge);
	TestCase_assert(charIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "add", 3, 55.3025f, &leadingEdge);
	TestCase_assert(charIndex == 2, "Expected 2 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "add", 3, 55.3225f, &leadingEdge);
	TestCase_assert(charIndex == 2, "Expected 2 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	charIndex = GLBitmapFont_indexAtWidth(font, "\tadd\x7F", 5, 55.3025f, &leadingEdge);
	TestCase_assert(charIndex == 3, "Expected 3 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(leadingEdge, "Expected true but got false");
	charIndex = GLBitmapFont_indexAtWidth(font, "\tadd\x7F", 5, 55.3225f, &leadingEdge);
	TestCase_assert(charIndex == 3, "Expected 3 but got " SIZE_T_FORMAT, charIndex);
	TestCase_assert(!leadingEdge, "Expected false but got true");
	font->dispose(font);
}

static void testFormatVersionVerification() {
	GLBitmapFont * fontPtr;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION + 1);
	fontPtr = GLBitmapFont_deserialize(context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(fontPtr == NULL, "GLBitmapFont_deserialize didn't return NULL when format_version was too high");
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readUInt16, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION - 1);
	fontPtr = GLBitmapFont_deserialize(context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(fontPtr == NULL, "GLBitmapFont_deserialize didn't return NULL when format_version was too low");
}

TEST_SUITE(GLBitmapFontTest,
           testInit,
           testDeserialization,
           testSerialization,
           testDeserializationErrors,
           testMeasureString,
           testIndexAtWidth,
           testFormatVersionVerification)
