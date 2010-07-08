#include "glbitmapfont/GLBitmapFont.h"
#include "unittest/framework/TestSuite.h"
#include "unittest/TestDeserializationContext.h"
#include "unittest/TestSerializationContext.h"

static void testInit() {
	GLBitmapFont font, * fontPtr;
	struct GLBitmapFont_charEntry characters[95];
	struct GLBitmapFont_kernEntry testKernEntry = {'a', 1.0f};
	struct GLBitmapFont_charEntry testCharEntry1 = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 0, NULL};
	struct GLBitmapFont_charEntry testCharEntry2 = {6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 1, &testKernEntry};
	unsigned int charIndex;
	
	characters[0] = testCharEntry1;
	for (charIndex = 1; charIndex < 95; charIndex++) {
		characters[charIndex] = testCharEntry2;
	}
	
	GLBitmapFont_init(&font, 1.0f, characters);
	TestCase_assert(font.dispose == GLBitmapFont_dispose, "Expected %p but got %p", GLBitmapFont_dispose, font.dispose);
	TestCase_assert(font.measureString == GLBitmapFont_measureString, "Expected %p but got %p", GLBitmapFont_measureString, font.measureString);
	TestCase_assert(font.indexAtWidth == GLBitmapFont_indexAtWidth, "Expected %p but got %p", GLBitmapFont_indexAtWidth, font.indexAtWidth);
	TestCase_assert(font.drawString == GLBitmapFont_drawString, "Expected %p but got %p", GLBitmapFont_drawString, font.drawString);
	TestCase_assert(font.emHeight == 1.0f, "Expected 1.0 but got %f", font.emHeight);
	TestCase_assert(font.characters[0].preadvance == 0.0f, "Expected 0.0 but got %f", font.characters[0].preadvance);
	TestCase_assert(font.characters[0].advance == 1.0f, "Expected 1.0 but got %f", font.characters[0].advance);
	TestCase_assert(font.characters[0].textureLeft == 2.0f, "Expected 2.0 but got %f", font.characters[0].textureLeft);
	TestCase_assert(font.characters[0].textureRight == 3.0f, "Expected 3.0 but got %f", font.characters[0].textureRight);
	TestCase_assert(font.characters[0].textureBottom == 4.0f, "Expected 4.0 but got %f", font.characters[0].textureBottom);
	TestCase_assert(font.characters[0].textureTop == 5.0f, "Expected 5.0 but got %f", font.characters[0].textureTop);
	TestCase_assert(font.characters[0].kernCharCount == 0, "Expected 0 but got %zu", font.characters[0].kernCharCount);
	TestCase_assert(font.characters[0].kernChars == NULL, "Expected NULL but got %p", font.characters[0].kernChars);
	for (charIndex = 1; charIndex < 95; charIndex++) {
		TestCase_assert(font.characters[charIndex].preadvance == 6.0f, "Expected 6.0 but got %f", font.characters[charIndex].preadvance);
		TestCase_assert(font.characters[charIndex].advance == 7.0f, "Expected 7.0 but got %f", font.characters[charIndex].advance);
		TestCase_assert(font.characters[charIndex].textureLeft == 8.0f, "Expected 8.0 but got %f", font.characters[charIndex].textureLeft);
		TestCase_assert(font.characters[charIndex].textureRight == 9.0f, "Expected 9.0 but got %f", font.characters[charIndex].textureRight);
		TestCase_assert(font.characters[charIndex].textureBottom == 10.0f, "Expected 10.0 but got %f", font.characters[charIndex].textureBottom);
		TestCase_assert(font.characters[charIndex].textureTop == 11.0f, "Expected 11.0 but got %f", font.characters[charIndex].textureTop);
		TestCase_assert(font.characters[charIndex].kernCharCount == 1, "Expected 1 but got %zu", font.characters[charIndex].kernCharCount);
		TestCase_assert(font.characters[charIndex].kernChars != NULL, "Expected non-NULL but got NULL");
		TestCase_assert(font.characters[charIndex].kernChars != testCharEntry2.kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(font.characters[charIndex].kernChars != font.characters[charIndex - 1].kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(font.characters[charIndex].kernChars[0].previous == 'a', "Expected a but got %c", font.characters[charIndex].kernChars[0].previous);
		TestCase_assert(font.characters[charIndex].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", font.characters[charIndex].kernChars[0].offset);
	}
	font.dispose(&font);
	
	fontPtr = GLBitmapFont_create(1.0f, characters);
	TestCase_assert(fontPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fontPtr->dispose == GLBitmapFont_dispose, "Expected %p but got %p", GLBitmapFont_dispose, fontPtr->dispose);
	TestCase_assert(fontPtr->measureString == GLBitmapFont_measureString, "Expected %p but got %p", GLBitmapFont_measureString, fontPtr->measureString);
	TestCase_assert(fontPtr->indexAtWidth == GLBitmapFont_indexAtWidth, "Expected %p but got %p", GLBitmapFont_indexAtWidth, fontPtr->indexAtWidth);
	TestCase_assert(fontPtr->drawString == GLBitmapFont_drawString, "Expected %p but got %p", GLBitmapFont_drawString, fontPtr->drawString);
	TestCase_assert(fontPtr->emHeight == 1.0f, "Expected 1.0 but got %f", fontPtr->emHeight);
	TestCase_assert(fontPtr->characters[0].preadvance == 0.0f, "Expected 0.0 but got %f", fontPtr->characters[0].preadvance);
	TestCase_assert(fontPtr->characters[0].advance == 1.0f, "Expected 1.0 but got %f", fontPtr->characters[0].advance);
	TestCase_assert(fontPtr->characters[0].textureLeft == 2.0f, "Expected 2.0 but got %f", fontPtr->characters[0].textureLeft);
	TestCase_assert(fontPtr->characters[0].textureRight == 3.0f, "Expected 3.0 but got %f", fontPtr->characters[0].textureRight);
	TestCase_assert(fontPtr->characters[0].textureBottom == 4.0f, "Expected 4.0 but got %f", fontPtr->characters[0].textureBottom);
	TestCase_assert(fontPtr->characters[0].textureTop == 5.0f, "Expected 5.0 but got %f", fontPtr->characters[0].textureTop);
	TestCase_assert(fontPtr->characters[0].kernCharCount == 0, "Expected 0 but got %zu", fontPtr->characters[0].kernCharCount);
	TestCase_assert(fontPtr->characters[0].kernChars == NULL, "Expected NULL but got %p", fontPtr->characters[0].kernChars);
	for (charIndex = 1; charIndex < 95; charIndex++) {
		TestCase_assert(fontPtr->characters[charIndex].preadvance == 6.0f, "Expected 6.0 but got %f", fontPtr->characters[charIndex].preadvance);
		TestCase_assert(fontPtr->characters[charIndex].advance == 7.0f, "Expected 7.0 but got %f", fontPtr->characters[charIndex].advance);
		TestCase_assert(fontPtr->characters[charIndex].textureLeft == 8.0f, "Expected 8.0 but got %f", fontPtr->characters[charIndex].textureLeft);
		TestCase_assert(fontPtr->characters[charIndex].textureRight == 9.0f, "Expected 9.0 but got %f", fontPtr->characters[charIndex].textureRight);
		TestCase_assert(fontPtr->characters[charIndex].textureBottom == 10.0f, "Expected 10.0 but got %f", fontPtr->characters[charIndex].textureBottom);
		TestCase_assert(fontPtr->characters[charIndex].textureTop == 11.0f, "Expected 11.0 but got %f", fontPtr->characters[charIndex].textureTop);
		TestCase_assert(fontPtr->characters[charIndex].kernCharCount == 1, "Expected 1 but got %zu", fontPtr->characters[charIndex].kernCharCount);
		TestCase_assert(fontPtr->characters[charIndex].kernChars != NULL, "Expected non-NULL but got NULL");
		TestCase_assert(fontPtr->characters[charIndex].kernChars != testCharEntry2.kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(fontPtr->characters[charIndex].kernChars != fontPtr->characters[charIndex - 1].kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(fontPtr->characters[charIndex].kernChars[0].previous == 'a', "Expected a but got %c", fontPtr->characters[charIndex].kernChars[0].previous);
		TestCase_assert(fontPtr->characters[charIndex].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", fontPtr->characters[charIndex].kernChars[0].offset);
	}
	fontPtr->dispose(fontPtr);
	
	characters[0] = testCharEntry2;
	characters[94] = testCharEntry1;
	
	GLBitmapFont_init(&font, 2.0f, characters);
	TestCase_assert(font.dispose == GLBitmapFont_dispose, "Expected %p but got %p", GLBitmapFont_dispose, font.dispose);
	TestCase_assert(font.measureString == GLBitmapFont_measureString, "Expected %p but got %p", GLBitmapFont_measureString, font.measureString);
	TestCase_assert(font.indexAtWidth == GLBitmapFont_indexAtWidth, "Expected %p but got %p", GLBitmapFont_indexAtWidth, font.indexAtWidth);
	TestCase_assert(font.drawString == GLBitmapFont_drawString, "Expected %p but got %p", GLBitmapFont_drawString, font.drawString);
	TestCase_assert(font.emHeight == 2.0f, "Expected 2.0 but got %f", font.emHeight);
	TestCase_assert(font.characters[94].preadvance == 0.0f, "Expected 0.0 but got %f", font.characters[94].preadvance);
	TestCase_assert(font.characters[94].advance == 1.0f, "Expected 1.0 but got %f", font.characters[94].advance);
	TestCase_assert(font.characters[94].textureLeft == 2.0f, "Expected 2.0 but got %f", font.characters[94].textureLeft);
	TestCase_assert(font.characters[94].textureRight == 3.0f, "Expected 3.0 but got %f", font.characters[94].textureRight);
	TestCase_assert(font.characters[94].textureBottom == 4.0f, "Expected 4.0 but got %f", font.characters[94].textureBottom);
	TestCase_assert(font.characters[94].textureTop == 5.0f, "Expected 5.0 but got %f", font.characters[94].textureTop);
	TestCase_assert(font.characters[94].kernCharCount == 0, "Expected 0 but got %zu", font.characters[94].kernCharCount);
	TestCase_assert(font.characters[94].kernChars == NULL, "Expected NULL but got %p", font.characters[94].kernChars);
	for (charIndex = 0; charIndex < 94; charIndex++) {
		TestCase_assert(font.characters[charIndex].preadvance == 6.0f, "Expected 6.0 but got %f", font.characters[charIndex].preadvance);
		TestCase_assert(font.characters[charIndex].advance == 7.0f, "Expected 7.0 but got %f", font.characters[charIndex].advance);
		TestCase_assert(font.characters[charIndex].textureLeft == 8.0f, "Expected 8.0 but got %f", font.characters[charIndex].textureLeft);
		TestCase_assert(font.characters[charIndex].textureRight == 9.0f, "Expected 9.0 but got %f", font.characters[charIndex].textureRight);
		TestCase_assert(font.characters[charIndex].textureBottom == 10.0f, "Expected 10.0 but got %f", font.characters[charIndex].textureBottom);
		TestCase_assert(font.characters[charIndex].textureTop == 11.0f, "Expected 11.0 but got %f", font.characters[charIndex].textureTop);
		TestCase_assert(font.characters[charIndex].kernCharCount == 1, "Expected 1 but got %zu", font.characters[charIndex].kernCharCount);
		TestCase_assert(font.characters[charIndex].kernChars != NULL, "Expected non-NULL but got NULL");
		TestCase_assert(font.characters[charIndex].kernChars != testCharEntry2.kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(font.characters[charIndex].kernChars != font.characters[charIndex + 1].kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(font.characters[charIndex].kernChars[0].previous == 'a', "Expected a but got %c", font.characters[charIndex].kernChars[0].previous);
		TestCase_assert(font.characters[charIndex].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", font.characters[charIndex].kernChars[0].offset);
	}
	font.dispose(&font);
	
	fontPtr = GLBitmapFont_create(2.0f, characters);
	TestCase_assert(fontPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fontPtr->dispose == GLBitmapFont_dispose, "Expected %p but got %p", GLBitmapFont_dispose, fontPtr->dispose);
	TestCase_assert(fontPtr->measureString == GLBitmapFont_measureString, "Expected %p but got %p", GLBitmapFont_measureString, fontPtr->measureString);
	TestCase_assert(fontPtr->indexAtWidth == GLBitmapFont_indexAtWidth, "Expected %p but got %p", GLBitmapFont_indexAtWidth, fontPtr->indexAtWidth);
	TestCase_assert(fontPtr->drawString == GLBitmapFont_drawString, "Expected %p but got %p", GLBitmapFont_drawString, fontPtr->drawString);
	TestCase_assert(fontPtr->emHeight == 2.0f, "Expected 2.0 but got %f", fontPtr->emHeight);
	TestCase_assert(fontPtr->characters[94].preadvance == 0.0f, "Expected 0.0 but got %f", fontPtr->characters[94].preadvance);
	TestCase_assert(fontPtr->characters[94].advance == 1.0f, "Expected 1.0 but got %f", fontPtr->characters[94].advance);
	TestCase_assert(fontPtr->characters[94].textureLeft == 2.0f, "Expected 2.0 but got %f", fontPtr->characters[94].textureLeft);
	TestCase_assert(fontPtr->characters[94].textureRight == 3.0f, "Expected 3.0 but got %f", fontPtr->characters[94].textureRight);
	TestCase_assert(fontPtr->characters[94].textureBottom == 4.0f, "Expected 4.0 but got %f", fontPtr->characters[94].textureBottom);
	TestCase_assert(fontPtr->characters[94].textureTop == 5.0f, "Expected 5.0 but got %f", fontPtr->characters[94].textureTop);
	TestCase_assert(fontPtr->characters[94].kernCharCount == 0, "Expected 0 but got %zu", fontPtr->characters[94].kernCharCount);
	TestCase_assert(fontPtr->characters[94].kernChars == NULL, "Expected NULL but got %p", fontPtr->characters[94].kernChars);
	for (charIndex = 0; charIndex < 94; charIndex++) {
		TestCase_assert(fontPtr->characters[charIndex].preadvance == 6.0f, "Expected 6.0 but got %f", fontPtr->characters[charIndex].preadvance);
		TestCase_assert(fontPtr->characters[charIndex].advance == 7.0f, "Expected 7.0 but got %f", fontPtr->characters[charIndex].advance);
		TestCase_assert(fontPtr->characters[charIndex].textureLeft == 8.0f, "Expected 8.0 but got %f", fontPtr->characters[charIndex].textureLeft);
		TestCase_assert(fontPtr->characters[charIndex].textureRight == 9.0f, "Expected 9.0 but got %f", fontPtr->characters[charIndex].textureRight);
		TestCase_assert(fontPtr->characters[charIndex].textureBottom == 10.0f, "Expected 10.0 but got %f", fontPtr->characters[charIndex].textureBottom);
		TestCase_assert(fontPtr->characters[charIndex].textureTop == 11.0f, "Expected 11.0 but got %f", fontPtr->characters[charIndex].textureTop);
		TestCase_assert(fontPtr->characters[charIndex].kernCharCount == 1, "Expected 1 but got %zu", fontPtr->characters[charIndex].kernCharCount);
		TestCase_assert(fontPtr->characters[charIndex].kernChars != NULL, "Expected non-NULL but got NULL");
		TestCase_assert(fontPtr->characters[charIndex].kernChars != testCharEntry2.kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(fontPtr->characters[charIndex].kernChars != fontPtr->characters[charIndex + 1].kernChars, "Pointers expected to differ but didn't");
		TestCase_assert(fontPtr->characters[charIndex].kernChars[0].previous == 'a', "Expected a but got %c", fontPtr->characters[charIndex].kernChars[0].previous);
		TestCase_assert(fontPtr->characters[charIndex].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", fontPtr->characters[charIndex].kernChars[0].offset);
	}
	fontPtr->dispose(fontPtr);
}

static void testDeserialization() {
	GLBitmapFont font;//, * fontPtr;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	unsigned int charIndex;
	char charStrings[95][2];
	char * kernKey1 = "a", * kernKey2 = "b";
	bool success;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readString, "texture_name", "foo");
	context->expectCall(context, context->readFloat, "em_height", 1.0f);
	context->expectCall(context, context->beginDictionary, "characters", (size_t) 95);
	charStrings[0][0] = ' ';
	charStrings[0][1] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charStrings[0]);
	context->expectCall(context, context->beginStructure, charStrings[0], (size_t) 4);
	context->expectCall(context, context->readFloat, "preadvance", 1.0f);
	context->expectCall(context, context->readFloat, "advance", 2.0f);
	context->expectCall(context, context->beginArray, "texture_bounds", (size_t) 4);
	context->expectCall(context, context->readFloat, NULL, 0.0f);
	context->expectCall(context, context->readFloat, NULL, 1.0f);
	context->expectCall(context, context->readFloat, NULL, 2.0f);
	context->expectCall(context, context->readFloat, NULL, 3.0f);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 2);
	context->expectCall(context, context->readNextDictionaryKey, kernKey1);
	context->expectCall(context, context->readFloat, kernKey1, -1.0f);
	context->expectCall(context, context->readNextDictionaryKey, kernKey2);
	context->expectCall(context, context->readFloat, kernKey2, -2.0f);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	for (charIndex = 1; charIndex < 95; charIndex++) {
		charStrings[charIndex][0] = ' ' + charIndex;
		charStrings[charIndex][1] = 0;
		context->expectCall(context, context->readNextDictionaryKey, charStrings[charIndex]);
		context->expectCall(context, context->beginStructure, charStrings[charIndex], (size_t) 4);
		context->expectCall(context, context->readFloat, "preadvance", 3.0f);
		context->expectCall(context, context->readFloat, "advance", 4.0f);
		context->expectCall(context, context->beginArray, "texture_bounds", (size_t) 4);
		context->expectCall(context, context->readFloat, NULL, 5.0f);
		context->expectCall(context, context->readFloat, NULL, 6.0f);
		context->expectCall(context, context->readFloat, NULL, 7.0f);
		context->expectCall(context, context->readFloat, NULL, 8.0f);
		context->expectCall(context, context->endArray);
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
	TestCase_assert(!strcmp(font.textureName, "foo"), "Expected \"foo\" but got \"%s\"", font.textureName);
	TestCase_assert(font.texture == NULL, "Expected NULL but got %p", font.texture);
	TestCase_assert(font.emHeight == 1.0f, "Expected 1.0 but got %f", font.emHeight);
	TestCase_assert(font.characters[0].preadvance == 1.0f, "Expected 1.0 but got %f", font.characters[0].preadvance);
	TestCase_assert(font.characters[0].advance == 2.0f, "Expected 2.0 but got %f", font.characters[0].advance);
	TestCase_assert(font.characters[0].textureLeft == 0.0f, "Expected 0.0 but got %f", font.characters[0].textureLeft);
	TestCase_assert(font.characters[0].textureRight == 1.0f, "Expected 1.0 but got %f", font.characters[0].textureRight);
	TestCase_assert(font.characters[0].textureBottom == 2.0f, "Expected 2.0 but got %f", font.characters[0].textureBottom);
	TestCase_assert(font.characters[0].textureTop == 3.0f, "Expected 3.0 but got %f", font.characters[0].textureTop);
	TestCase_assert(font.characters[0].kernCharCount == 2, "Expected 2 but got %zu", font.characters[0].kernCharCount);
	TestCase_assert(font.characters[0].kernChars != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(font.characters[0].kernChars[0].previous == 'a', "Expected 'a' but got '%c'", font.characters[0].kernChars[0].previous);
	TestCase_assert(font.characters[0].kernChars[0].offset == -1.0f, "Expected -1.0 but got %f", font.characters[0].kernChars[0].offset);
	TestCase_assert(font.characters[0].kernChars[1].previous == 'b', "Expected 'b' but got '%c'", font.characters[0].kernChars[1].previous);
	TestCase_assert(font.characters[0].kernChars[1].offset == -2.0f, "Expected -2.0 but got %f", font.characters[0].kernChars[1].offset);
	for (charIndex = 1; charIndex < 95; charIndex++) {
		TestCase_assert(font.characters[charIndex].preadvance == 3.0f, "Expected 3.0 but got %f (char %u)", font.characters[charIndex].preadvance, charIndex);
		TestCase_assert(font.characters[charIndex].advance == 4.0f, "Expected 4.0 but got %f (char %u)", font.characters[charIndex].advance, charIndex);
		TestCase_assert(font.characters[charIndex].textureLeft == 5.0f, "Expected 5.0 but got %f (char %u)", font.characters[charIndex].textureLeft, charIndex);
		TestCase_assert(font.characters[charIndex].textureRight == 6.0f, "Expected 6.0 but got %f (char %u)", font.characters[charIndex].textureRight, charIndex);
		TestCase_assert(font.characters[charIndex].textureBottom == 7.0f, "Expected 7.0 but got %f (char %u)", font.characters[charIndex].textureBottom, charIndex);
		TestCase_assert(font.characters[charIndex].textureTop == 8.0f, "Expected 8.0 but got %f (char %u)", font.characters[charIndex].textureTop, charIndex);
		TestCase_assert(font.characters[charIndex].kernCharCount == 0, "Expected 0 but got %zu (char %u)", font.characters[charIndex].kernCharCount, charIndex);
		TestCase_assert(font.characters[charIndex].kernChars == NULL, "Expected NULL but got %p (char %u)", font.characters[charIndex].kernChars, charIndex);
	}
	font.dispose(&font);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, "glbitmapfont", (size_t) 3);
	context->expectCall(context, context->readString, "texture_name", "bar");
	context->expectCall(context, context->readFloat, "em_height", 2.0f);
	context->expectCall(context, context->beginDictionary, "characters", (size_t) 95);
	charStrings[0][0] = '~';
	charStrings[0][1] = 0;
	context->expectCall(context, context->readNextDictionaryKey, charStrings[0]);
	context->expectCall(context, context->beginStructure, charStrings[0], (size_t) 4);
	context->expectCall(context, context->readFloat, "preadvance", 3.0f);
	context->expectCall(context, context->readFloat, "advance", 4.0f);
	context->expectCall(context, context->beginArray, "texture_bounds", (size_t) 4);
	context->expectCall(context, context->readFloat, NULL, 1.0f);
	context->expectCall(context, context->readFloat, NULL, 2.0f);
	context->expectCall(context, context->readFloat, NULL, 3.0f);
	context->expectCall(context, context->readFloat, NULL, 4.0f);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->beginDictionary, "kerning_table", (size_t) 1);
	context->expectCall(context, context->readNextDictionaryKey, kernKey2);
	context->expectCall(context, context->readFloat, kernKey2, 1.0f);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	for (charIndex = 1; charIndex < 95; charIndex++) {
		charStrings[charIndex][0] = ' ' + charIndex - 1;
		charStrings[charIndex][1] = 0;
		context->expectCall(context, context->readNextDictionaryKey, charStrings[charIndex]);
		context->expectCall(context, context->beginStructure, charStrings[charIndex], (size_t) 4);
		context->expectCall(context, context->readFloat, "preadvance", 4.0f);
		context->expectCall(context, context->readFloat, "advance", 5.0f);
		context->expectCall(context, context->beginArray, "texture_bounds", (size_t) 4);
		context->expectCall(context, context->readFloat, NULL, 2.0f);
		context->expectCall(context, context->readFloat, NULL, 3.0f);
		context->expectCall(context, context->readFloat, NULL, 4.0f);
		context->expectCall(context, context->readFloat, NULL, 5.0f);
		context->expectCall(context, context->endArray);
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
	TestCase_assert(!strcmp(font.textureName, "bar"), "Expected \"bar\" but got \"%s\"", font.textureName);
	TestCase_assert(font.texture == NULL, "Expected NULL but got %p", font.texture);
	TestCase_assert(font.emHeight == 2.0f, "Expected 2.0 but got %f", font.emHeight);
	TestCase_assert(font.characters[94].preadvance == 3.0f, "Expected 3.0 but got %f", font.characters[94].preadvance);
	TestCase_assert(font.characters[94].advance == 4.0f, "Expected 4.0 but got %f", font.characters[94].advance);
	TestCase_assert(font.characters[94].textureLeft == 1.0f, "Expected 1.0 but got %f", font.characters[94].textureLeft);
	TestCase_assert(font.characters[94].textureRight == 2.0f, "Expected 2.0 but got %f", font.characters[94].textureRight);
	TestCase_assert(font.characters[94].textureBottom == 3.0f, "Expected 3.0 but got %f", font.characters[94].textureBottom);
	TestCase_assert(font.characters[94].textureTop == 4.0f, "Expected 4.0 but got %f", font.characters[94].textureTop);
	TestCase_assert(font.characters[94].kernCharCount == 1, "Expected 1 but got %zu", font.characters[94].kernCharCount);
	TestCase_assert(font.characters[94].kernChars != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(font.characters[94].kernChars[0].previous == 'b', "Expected 'a' but got '%c'", font.characters[94].kernChars[0].previous);
	TestCase_assert(font.characters[94].kernChars[0].offset == 1.0f, "Expected 1.0 but got %f", font.characters[94].kernChars[0].offset);
	for (charIndex = 0; charIndex < 94; charIndex++) {
		TestCase_assert(font.characters[charIndex].preadvance == 4.0f, "Expected 4.0 but got %f (char %u)", font.characters[charIndex].preadvance, charIndex);
		TestCase_assert(font.characters[charIndex].advance == 5.0f, "Expected 5.0 but got %f (char %u)", font.characters[charIndex].advance, charIndex);
		TestCase_assert(font.characters[charIndex].textureLeft == 2.0f, "Expected 2.0 but got %f (char %u)", font.characters[charIndex].textureLeft, charIndex);
		TestCase_assert(font.characters[charIndex].textureRight == 3.0f, "Expected 3.0 but got %f (char %u)", font.characters[charIndex].textureRight, charIndex);
		TestCase_assert(font.characters[charIndex].textureBottom == 4.0f, "Expected 4.0 but got %f (char %u)", font.characters[charIndex].textureBottom, charIndex);
		TestCase_assert(font.characters[charIndex].textureTop == 5.0f, "Expected 5.0 but got %f (char %u)", font.characters[charIndex].textureTop, charIndex);
		TestCase_assert(font.characters[charIndex].kernCharCount == 0, "Expected 0 but got %zu (char %u)", font.characters[charIndex].kernCharCount, charIndex);
		TestCase_assert(font.characters[charIndex].kernChars == NULL, "Expected NULL but got %p (char %u)", font.characters[charIndex].kernChars, charIndex);
	}
	font.dispose(&font);
}

static void testSerialization() {
	GLBitmapFont * font;
	struct GLBitmapFont_charEntry characters[95];
	struct GLBitmapFont_kernEntry testKernEntry = {'a', 1.0f};
	struct GLBitmapFont_charEntry testCharEntry1 = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 0, NULL};
	struct GLBitmapFont_charEntry testCharEntry2 = {6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 1, &testKernEntry};
	unsigned int charIndex;
	//TestSerializationContext * context;
	
	characters[0] = testCharEntry2;
	for (charIndex = 1; charIndex < 95; charIndex++) {
		characters[charIndex] = testCharEntry1;
	}
	
	font = GLBitmapFont_create(1.0f, characters);
	//GLBitmapFont_serialize(font, context);
	font->dispose(font);
}

static void testDeserializationErrors() {
	// Character in dictionary out of range
	// Multiple characters in character dictionary key
	// No characters in character dictionary key
	// Wrong number of charcter dictionary keys
}

TEST_SUITE(GLBitmapFontTest, testInit, testDeserialization, testSerialization, testDeserializationErrors)
