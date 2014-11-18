#include <string.h>
#include "serialization/TestDeserializationContext.h"
#include "serialization/TestSerializationContext.h"
#include "unittest/TestSuite.h"
#include "inputcontroller/InputMap.h"

static void verifyInit(InputMap * inputMap, int callingLine) {
	TestCase_assert(inputMap != NULL, "Expected non-NULL but got NULL (line %d)", callingLine);
	TestCase_assert(inputMap->keyboardBindingCount == 0, "Expected 0 but got %u (line %d)", inputMap->keyboardBindingCount, callingLine);
	TestCase_assert(inputMap->keyModifierBindingCount == 0, "Expected 0 but got %u (line %d)", inputMap->keyModifierBindingCount, callingLine);
	TestCase_assert(inputMap->gamepadMapCount == 0, "Expected 0 but got %u (line %d)", inputMap->gamepadMapCount, callingLine);
	TestCase_assert(inputMap->dispose == InputMap_dispose, "Expected %p but got %p (line %d)", InputMap_dispose, inputMap->dispose, callingLine);
}

static void testInit() {
	InputMap inputMap, * inputMapPtr;
	
	memset(&inputMap, 0x00, sizeof(InputMap));
	InputMap_init(&inputMap);
	verifyInit(&inputMap, __LINE__);
	InputMap_dispose(&inputMap);
	
	memset(&inputMap, 0xFF, sizeof(InputMap));
	InputMap_init(&inputMap);
	verifyInit(&inputMap, __LINE__);
	InputMap_dispose(&inputMap);
	
	inputMapPtr = InputMap_create();
	verifyInit(inputMapPtr, __LINE__);
	inputMapPtr->dispose(inputMapPtr);
}

static bool isKeyBound(InputMap * inputMap, Atom actionID, unsigned int keyCode, unsigned int charCode) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < inputMap->keyboardBindingCount; bindingIndex++) {
		if (inputMap->keyboardBindings[bindingIndex].actionID == actionID &&
		    inputMap->keyboardBindings[bindingIndex].keyCode == keyCode &&
		    inputMap->keyboardBindings[bindingIndex].charCode == charCode) {
			return true;
		}
	}
	return false;
}

static void verifyKeyNotBound(InputMap * inputMap, Atom actionID, unsigned int keyCode, unsigned int charCode, int callingLine) {
	TestCase_assert(!InputMap_isKeyBound(inputMap, actionID, keyCode), "Key unexpectedly reported as bound (keyCode = %u, charCode = %u, actionID = \"%s\") (line %d)", keyCode, charCode, actionID, callingLine);
	TestCase_assert(!isKeyBound(inputMap, actionID, keyCode, charCode), "Unexpectedly found bound key (keyCode = %u, charCode = %u, actionID = \"%s\") (line %d)", keyCode, charCode, actionID, callingLine);
}

static void verifyKeyBound(InputMap * inputMap, Atom actionID, unsigned int keyCode, unsigned int charCode, int callingLine) {
	TestCase_assert(InputMap_isKeyBound(inputMap, actionID, keyCode), "Key unexpectedly reported as unbound (keyCode = %u, charCode = %u, actionID = \"%s\") (line %d)", keyCode, charCode, actionID, callingLine);
	TestCase_assert(isKeyBound(inputMap, actionID, keyCode, charCode), "Couldn't find bound key (keyCode = %u, charCode = %u, actionID = \"%s\") (line %d)", keyCode, charCode, actionID, callingLine);
}

static void testKeyboardBindings() {
	InputMap * inputMap;
	
	inputMap = InputMap_create();
	verifyKeyNotBound(inputMap, ATOM("a"), 1, 3, __LINE__);
	verifyKeyNotBound(inputMap, ATOM("b"), 1, 2, __LINE__);
	
	InputMap_bindKey(inputMap, ATOM("a"), 1, 3);
	verifyKeyBound(inputMap, ATOM("a"), 1, 3, __LINE__);
	
	InputMap_bindKey(inputMap, ATOM("a"), 1, 2);
	verifyKeyBound(inputMap, ATOM("a"), 1, 2, __LINE__);
	TestCase_assert(!isKeyBound(inputMap, ATOM("a"), 1, 3), "Expected false but got true");
	
	InputMap_bindKey(inputMap, ATOM("a"), 2, 4);
	verifyKeyBound(inputMap, ATOM("a"), 1, 2, __LINE__);
	verifyKeyBound(inputMap, ATOM("a"), 2, 4, __LINE__);
	
	InputMap_bindKey(inputMap, ATOM("b"), 1, 2);
	verifyKeyBound(inputMap, ATOM("a"), 1, 2, __LINE__);
	verifyKeyBound(inputMap, ATOM("a"), 2, 4, __LINE__);
	verifyKeyBound(inputMap, ATOM("b"), 1, 2, __LINE__);
	
	InputMap_unbindKey(inputMap, ATOM("a"), 1);
	verifyKeyNotBound(inputMap, ATOM("a"), 1, 2, __LINE__);
	verifyKeyBound(inputMap, ATOM("a"), 2, 4, __LINE__);
	verifyKeyBound(inputMap, ATOM("b"), 1, 2, __LINE__);
}

static bool isKeyModifierBound(InputMap * inputMap, Atom actionID, int modifierBit) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < inputMap->keyModifierBindingCount; bindingIndex++) {
		if (inputMap->keyModifierBindings[bindingIndex].actionID == actionID &&
		    inputMap->keyModifierBindings[bindingIndex].modifierBit == modifierBit) {
			return true;
		}
	}
	return false;
}

static void verifyKeyModifierNotBound(InputMap * inputMap, Atom actionID, int modifierBit, int callingLine) {
	TestCase_assert(!InputMap_isKeyModifierBound(inputMap, actionID, modifierBit), "Modifier unexpectedly reported as bound (modifierBit = 0x%X, actionID = \"%s\") (line %d)", modifierBit, actionID, callingLine);
	TestCase_assert(!isKeyModifierBound(inputMap, actionID, modifierBit), "Unexpectedly found bound modifier (modifierBit = 0x%X, actionID = \"%s\") (line %d)", modifierBit, actionID, callingLine);
}

static void verifyKeyModifierBound(InputMap * inputMap, Atom actionID, int modifierBit, int callingLine) {
	TestCase_assert(InputMap_isKeyModifierBound(inputMap, actionID, modifierBit), "Modifier unexpectedly reported as unbound (modifierBit = 0x%X, actionID = \"%s\") (line %d)", modifierBit, actionID, callingLine);
	TestCase_assert(isKeyModifierBound(inputMap, actionID, modifierBit), "Couldn't find bound modifier (modifierBit = 0x%X, actionID = \"%s\") (line %d)", modifierBit, actionID, callingLine);
}

static void testKeyModifierBindings() {
	InputMap * inputMap;
	
	inputMap = InputMap_create();
	verifyKeyModifierNotBound(inputMap, ATOM("a"), 1, __LINE__);
	verifyKeyModifierNotBound(inputMap, ATOM("b"), 2, __LINE__);
	
	InputMap_bindKeyModifier(inputMap, ATOM("a"), 1);
	verifyKeyModifierBound(inputMap, ATOM("a"), 1, __LINE__);
	
	InputMap_bindKeyModifier(inputMap, ATOM("a"), 4);
	verifyKeyModifierBound(inputMap, ATOM("a"), 1, __LINE__);
	verifyKeyModifierBound(inputMap, ATOM("a"), 4, __LINE__);
	
	InputMap_bindKeyModifier(inputMap, ATOM("b"), 2);
	verifyKeyModifierBound(inputMap, ATOM("a"), 1, __LINE__);
	verifyKeyModifierBound(inputMap, ATOM("a"), 4, __LINE__);
	verifyKeyModifierBound(inputMap, ATOM("b"), 2, __LINE__);
	
	InputMap_unbindKeyModifier(inputMap, ATOM("a"), 1);
	verifyKeyModifierNotBound(inputMap, ATOM("a"), 1, __LINE__);
	verifyKeyModifierBound(inputMap, ATOM("a"), 4, __LINE__);
	verifyKeyModifierBound(inputMap, ATOM("b"), 2, __LINE__);
}

static bool isButtonBound(InputMap * inputMap, Atom actionID, int vendorID, int productID, unsigned int buttonID) {
	unsigned int gamepadMapIndex, bindingIndex;
	
	for (gamepadMapIndex = 0; gamepadMapIndex < inputMap->gamepadMapCount; gamepadMapIndex++) {
		if (inputMap->gamepadMaps[gamepadMapIndex].vendorID != vendorID ||
		    inputMap->gamepadMaps[gamepadMapIndex].productID != productID) {
			continue;
		}
		for (bindingIndex = 0; bindingIndex < inputMap->gamepadMaps[gamepadMapIndex].buttonBindingCount; bindingIndex++) {
			if (inputMap->gamepadMaps[gamepadMapIndex].buttonBindings[bindingIndex].actionID == actionID &&
					inputMap->gamepadMaps[gamepadMapIndex].buttonBindings[bindingIndex].buttonID == buttonID) {
				return true;
			}
		}
	}
	return false;
}

static void verifyButtonNotBound(InputMap * inputMap, Atom actionID, int vendorID, int productID, unsigned int buttonID, int callingLine) {
	TestCase_assert(!InputMap_isButtonBound(inputMap, actionID, vendorID, productID, buttonID), "Button unexpectedly reported as bound (vendorID = %u, productID = %u, buttonID = %u, actionID = \"%s\") (line %d)", vendorID, productID, buttonID, actionID, callingLine);
	TestCase_assert(!isButtonBound(inputMap, actionID, vendorID, productID, buttonID), "Unexpectedly found bound button (vendorID = %u, productID = %u, buttonID = %u, actionID = \"%s\") (line %d)", vendorID, productID, buttonID, actionID, callingLine);
}

static void verifyButtonBound(InputMap * inputMap, Atom actionID, int vendorID, int productID, unsigned int buttonID, int callingLine) {
	TestCase_assert(InputMap_isButtonBound(inputMap, actionID, vendorID, productID, buttonID), "Button unexpectedly reported as unbound (vendorID = %u, productID = %u, buttonID = %u, actionID = \"%s\") (line %d)", vendorID, productID, buttonID, actionID, callingLine);
	TestCase_assert(isButtonBound(inputMap, actionID, vendorID, productID, buttonID), "Couldn't find bound button (vendorID = %u, productID = %u, buttonID = %u, actionID = \"%s\") (line %d)", vendorID, productID, buttonID, actionID, callingLine);
}

static void testButtonBindings() {
	InputMap * inputMap;
	
	inputMap = InputMap_create();
	verifyButtonNotBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	verifyButtonNotBound(inputMap, ATOM("a"), 0, 0, 1, __LINE__);
	verifyButtonNotBound(inputMap, ATOM("a"), 1, 1, 1, __LINE__);
	verifyButtonNotBound(inputMap, ATOM("b"), 0, 0, 0, __LINE__);
	
	InputMap_bindButton(inputMap, ATOM("a"), 0, 0, 0);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	
	InputMap_bindButton(inputMap, ATOM("a"), 0, 0, 1);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 1, __LINE__);
	
	InputMap_bindButton(inputMap, ATOM("a"), 1, 1, 1);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 1, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 1, 1, 1, __LINE__);
	
	InputMap_bindButton(inputMap, ATOM("b"), 0, 0, 0);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 1, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 1, 1, 1, __LINE__);
	verifyButtonBound(inputMap, ATOM("b"), 0, 0, 0, __LINE__);
	
	InputMap_unbindButton(inputMap, ATOM("a"), 0, 0, 0);
	verifyButtonNotBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 1, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 1, 1, 1, __LINE__);
	verifyButtonBound(inputMap, ATOM("b"), 0, 0, 0, __LINE__);
}

static bool isAxisBound(InputMap * inputMap, Atom actionID, int vendorID, int productID, unsigned int axisID, float triggerThreshold, float releaseThreshold) {
	unsigned int gamepadMapIndex, bindingIndex;
	
	for (gamepadMapIndex = 0; gamepadMapIndex < inputMap->gamepadMapCount; gamepadMapIndex++) {
		if (inputMap->gamepadMaps[gamepadMapIndex].vendorID != vendorID ||
		    inputMap->gamepadMaps[gamepadMapIndex].productID != productID) {
			continue;
		}
		for (bindingIndex = 0; bindingIndex < inputMap->gamepadMaps[gamepadMapIndex].axisBindingCount; bindingIndex++) {
			if (inputMap->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].actionID == actionID &&
					inputMap->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].axisID == axisID &&
					inputMap->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].triggerThreshold == triggerThreshold &&
					inputMap->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].releaseThreshold == releaseThreshold) {
				return true;
			}
		}
	}
	return false;
}

static void verifyAxisNotBound(InputMap * inputMap, Atom actionID, int vendorID, int productID, unsigned int axisID, float triggerThreshold, float releaseThreshold, int callingLine) {
	TestCase_assert(!InputMap_isAxisBound(inputMap, actionID, vendorID, productID, axisID), "Axis unexpectedly reported as bound (vendorID = %u, productID = %u, axisID = %u, triggerThreshold = %f, releaseThreshold = %f, actionID = \"%s\") (line %d)", vendorID, productID, axisID, triggerThreshold, releaseThreshold, actionID, callingLine);
	TestCase_assert(!isAxisBound(inputMap, actionID, vendorID, productID, axisID, triggerThreshold, releaseThreshold), "Unexpectedly found bound axis (vendorID = %u, productID = %u, axisID = %u, triggerThreshold = %f, releaseThreshold = %f, actionID = \"%s\") (line %d)", vendorID, productID, axisID, triggerThreshold, releaseThreshold, actionID, callingLine);
}

static void verifyAxisBound(InputMap * inputMap, Atom actionID, int vendorID, int productID, unsigned int axisID, float triggerThreshold, float releaseThreshold, int callingLine) {
	TestCase_assert(InputMap_isAxisBound(inputMap, actionID, vendorID, productID, axisID), "axis unexpectedly reported as unbound (vendorID = %u, productID = %u, axisID = %u, triggerThreshold = %f, releaseThreshold = %f, actionID = \"%s\") (line %d)", vendorID, productID, axisID, triggerThreshold, releaseThreshold, actionID, callingLine);
	TestCase_assert(isAxisBound(inputMap, actionID, vendorID, productID, axisID, triggerThreshold, releaseThreshold), "Couldn't find bound axis (vendorID = %u, productID = %u, axisID = %u, actionID, triggerThreshold = %f, releaseThreshold = %f = \"%s\") (line %d)", vendorID, productID, axisID, triggerThreshold, releaseThreshold, actionID, callingLine);
}

static void testAxisBindings() {
	InputMap * inputMap;
	
	inputMap = InputMap_create();
	verifyAxisNotBound(inputMap, ATOM("a"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	verifyAxisNotBound(inputMap, ATOM("a"), 0, 0, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisNotBound(inputMap, ATOM("a"), 1, 1, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisNotBound(inputMap, ATOM("b"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	
	InputMap_bindAxis(inputMap, ATOM("a"), 0, 0, 0, 1.0f, 0.0f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	
	InputMap_bindAxis(inputMap, ATOM("a"), 0, 0, 1, 1.0f, 0.0f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1.0f, 0.0f, __LINE__);
	
	InputMap_bindAxis(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.0f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.0f, __LINE__);
	TestCase_assert(!isAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1.0f, 0.0f), "Expected false but got true");
	
	InputMap_bindAxis(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f, __LINE__);
	TestCase_assert(!isAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.0f), "Expected false but got true");
	
	InputMap_bindAxis(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f, __LINE__);
	
	InputMap_bindAxis(inputMap, ATOM("a"), 1, 1, 1, 1.0f, 0.0f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 1, 1, 1, 1.0f, 0.0f, __LINE__);
	
	InputMap_bindAxis(inputMap, ATOM("b"), 0, 0, 0, 1.0f, 0.0f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 1, 1, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("b"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	
	InputMap_unbindAxis(inputMap, ATOM("a"), 0, 0, 0);
	verifyAxisNotBound(inputMap, ATOM("a"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 1, 1, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("b"), 0, 0, 0, 1.0f, 0.0f, __LINE__);
}

static void testSerialization() {
	InputMap inputMap;
	TestSerializationContext * context;
	jmp_buf jmpEnv;
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	InputMap_init(&inputMap);
	
	context->expectCall(context, context->beginStructure, "input_map");
	context->expectCall(context, context->writeUInt16, "format_version", INPUT_MAP_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->beginArray, "keyboard_bindings");
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->beginArray, "gamepad_maps");
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endStructure);
	
	InputMap_serialize(&inputMap, context);
	
	context->finish(context);
	context->dispose(context);
	InputMap_dispose(&inputMap);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	InputMap_init(&inputMap);
	InputMap_bindKey(&inputMap, ATOM("a"), 1, 2);
	InputMap_bindKey(&inputMap, ATOM("b"), 3, 4);
	InputMap_bindButton(&inputMap, ATOM("a"), 1, 2, 3);
	InputMap_bindButton(&inputMap, ATOM("b"), 1, 2, 4);
	InputMap_bindButton(&inputMap, ATOM("c"), 4, 5, 6);
	InputMap_bindAxis(&inputMap, ATOM("a"), 1, 2, 3, 0.5f, 0.5f);
	InputMap_bindAxis(&inputMap, ATOM("b"), 1, 2, 4, 0.5f, 0.5f);
	InputMap_bindAxis(&inputMap, ATOM("c"), 4, 5, 6, 1.0f, 0.0f);
	
	context->expectCall(context, context->beginStructure, "input_map");
		context->expectCall(context, context->writeUInt16, "format_version", INPUT_MAP_SERIALIZATION_FORMAT_VERSION);
		context->expectCall(context, context->beginArray, "keyboard_bindings");
			context->expectCall(context, context->beginStructure, NULL);
				context->expectCall(context, context->writeString, "action", "a");
				context->expectCall(context, context->writeUInt32, "key_code", 1);
				context->expectCall(context, context->writeUInt32, "char_code", 2);
			context->expectCall(context, context->endStructure);
			context->expectCall(context, context->beginStructure, NULL);
				context->expectCall(context, context->writeString, "action", "b");
				context->expectCall(context, context->writeUInt32, "key_code", 3);
				context->expectCall(context, context->writeUInt32, "char_code", 4);
			context->expectCall(context, context->endStructure);
		context->expectCall(context, context->endArray);
		context->expectCall(context, context->beginArray, "gamepad_maps");
			context->expectCall(context, context->beginStructure, NULL);
				context->expectCall(context, context->writeInt32, "vendor_id", 1);
				context->expectCall(context, context->writeInt32, "product_id", 2);
				context->expectCall(context, context->beginDictionary, "button_bindings");
					context->expectCall(context, context->writeUInt32, "a", 3);
					context->expectCall(context, context->writeUInt32, "b", 4);
				context->expectCall(context, context->endDictionary);
				context->expectCall(context, context->beginArray, "axis_bindings");
					context->expectCall(context, context->beginStructure, NULL);
						context->expectCall(context, context->writeString, "action", "a");
						context->expectCall(context, context->writeUInt32, "axis_id", 3);
						context->expectCall(context, context->writeFloat, "trigger_threshold", 0.5f);
						context->expectCall(context, context->writeFloat, "release_threshold", 0.5f);
					context->expectCall(context, context->endStructure);
					context->expectCall(context, context->beginStructure, NULL);
						context->expectCall(context, context->writeString, "action", "b");
						context->expectCall(context, context->writeUInt32, "axis_id", 4);
						context->expectCall(context, context->writeFloat, "trigger_threshold", 0.5f);
						context->expectCall(context, context->writeFloat, "release_threshold", 0.5f);
					context->expectCall(context, context->endStructure);
				context->expectCall(context, context->endArray);
			context->expectCall(context, context->endStructure);
			context->expectCall(context, context->beginStructure, NULL);
				context->expectCall(context, context->writeInt32, "vendor_id", 4);
				context->expectCall(context, context->writeInt32, "product_id", 5);
				context->expectCall(context, context->beginDictionary, "button_bindings");
					context->expectCall(context, context->writeUInt32, "c", 6);
				context->expectCall(context, context->endDictionary);
				context->expectCall(context, context->beginArray, "axis_bindings");
					context->expectCall(context, context->beginStructure, NULL);
						context->expectCall(context, context->writeString, "action", "c");
						context->expectCall(context, context->writeUInt32, "axis_id", 6);
						context->expectCall(context, context->writeFloat, "trigger_threshold", 1.0f);
						context->expectCall(context, context->writeFloat, "release_threshold", 0.0f);
					context->expectCall(context, context->endStructure);
				context->expectCall(context, context->endArray);
			context->expectCall(context, context->endStructure);
		context->expectCall(context, context->endArray);
	context->expectCall(context, context->endStructure);
	
	InputMap_serialize(&inputMap, context);
	
	context->finish(context);
	context->dispose(context);
	InputMap_dispose(&inputMap);
}

static void setUpBlankDeserializationContext(TestDeserializationContext * context) {
	context->expectCall(context, context->beginStructure, "input_map");
	context->expectCall(context, context->readUInt16, "format_version", INPUT_MAP_SERIALIZATION_FORMAT_VERSION);
	context->expectCall(context, context->beginArray, "keyboard_bindings", 0);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->beginArray, "gamepad_maps", 0);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endStructure);
}

static void verifyBlankInputMap(InputMap * inputMap, unsigned int lineNumber) {
	TestCase_assert(inputMap != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputMap->keyboardBindingCount == 0, "Expected 0 but got %u (%u)", inputMap->keyboardBindingCount, lineNumber);
	TestCase_assert(inputMap->gamepadMapCount == 0, "Expected 0 but got %u (%u)", inputMap->gamepadMapCount, lineNumber);
}

static void setUpBasicDeserializationContext(TestDeserializationContext * context) {
	context->expectCall(context, context->beginStructure, "input_map");
		context->expectCall(context, context->readUInt16, "format_version", INPUT_MAP_SERIALIZATION_FORMAT_VERSION);
		context->expectCall(context, context->beginArray, "keyboard_bindings", 2);
			context->expectCall(context, context->beginStructure, NULL);
				context->expectCall(context, context->readString, "action", "a");
				context->expectCall(context, context->readUInt32, "key_code", 1);
				context->expectCall(context, context->readUInt32, "char_code", 2);
			context->expectCall(context, context->endStructure);
			context->expectCall(context, context->beginStructure, NULL);
				context->expectCall(context, context->readString, "action", "b");
				context->expectCall(context, context->readUInt32, "key_code", 3);
				context->expectCall(context, context->readUInt32, "char_code", 4);
			context->expectCall(context, context->endStructure);
		context->expectCall(context, context->endArray);
		context->expectCall(context, context->beginArray, "gamepad_maps", 2);
			context->expectCall(context, context->beginStructure, NULL);
				context->expectCall(context, context->readInt32, "vendor_id", 1);
				context->expectCall(context, context->readInt32, "product_id", 2);
				context->expectCall(context, context->beginDictionary, "button_bindings", 2);
					context->expectCall(context, context->readNextDictionaryKey, "a");
					context->expectCall(context, context->readUInt32, "a", 3);
					context->expectCall(context, context->readNextDictionaryKey, "b");
					context->expectCall(context, context->readUInt32, "b", 4);
				context->expectCall(context, context->endDictionary);
				context->expectCall(context, context->beginArray, "axis_bindings", 2);
					context->expectCall(context, context->beginStructure, NULL);
						context->expectCall(context, context->readString, "action", "a");
						context->expectCall(context, context->readUInt32, "axis_id", 3);
						context->expectCall(context, context->readFloat, "trigger_threshold", 0.5f);
						context->expectCall(context, context->readFloat, "release_threshold", 0.5f);
					context->expectCall(context, context->endStructure);
					context->expectCall(context, context->beginStructure, NULL);
						context->expectCall(context, context->readString, "action", "b");
						context->expectCall(context, context->readUInt32, "axis_id", 4);
						context->expectCall(context, context->readFloat, "trigger_threshold", 0.5f);
						context->expectCall(context, context->readFloat, "release_threshold", 0.5f);
					context->expectCall(context, context->endStructure);
				context->expectCall(context, context->endArray);
			context->expectCall(context, context->endStructure);
			context->expectCall(context, context->beginStructure, NULL);
				context->expectCall(context, context->readInt32, "vendor_id", 4);
				context->expectCall(context, context->readInt32, "product_id", 5);
				context->expectCall(context, context->beginDictionary, "button_bindings", 1);
					context->expectCall(context, context->readNextDictionaryKey, "c");
					context->expectCall(context, context->readUInt32, "c", 6);
				context->expectCall(context, context->endDictionary);
				context->expectCall(context, context->beginArray, "axis_bindings", 1);
					context->expectCall(context, context->beginStructure, NULL);
						context->expectCall(context, context->readString, "action", "c");
						context->expectCall(context, context->readUInt32, "axis_id", 6);
						context->expectCall(context, context->readFloat, "trigger_threshold", 1.0f);
						context->expectCall(context, context->readFloat, "release_threshold", 0.0f);
					context->expectCall(context, context->endStructure);
				context->expectCall(context, context->endArray);
			context->expectCall(context, context->endStructure);
		context->expectCall(context, context->endArray);
	context->expectCall(context, context->endStructure);
}

static void verifyBasicInputMap(InputMap * inputMap, unsigned int lineNumber) {
	TestCase_assert(inputMap != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputMap->keyboardBindingCount == 2, "Expected 2 but got %u (%u)", inputMap->keyboardBindingCount, lineNumber);
	TestCase_assert(inputMap->keyboardBindings != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputMap->keyboardBindings[0].actionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p) (%u)", ATOM("a"), inputMap->keyboardBindings[0].actionID, inputMap->keyboardBindings[0].actionID, lineNumber);
	TestCase_assert(inputMap->keyboardBindings[0].keyCode == 1, "Expected 1 but got %u (%u)", inputMap->keyboardBindings[0].keyCode, lineNumber);
	TestCase_assert(inputMap->keyboardBindings[0].charCode == 2, "Expected 2 but got %u (%u)", inputMap->keyboardBindings[0].charCode, lineNumber);
	TestCase_assert(inputMap->keyboardBindings[1].actionID == ATOM("b"), "Expected \"a\" (%p) but got \"%s\" (%p) (%u)", ATOM("b"), inputMap->keyboardBindings[1].actionID, inputMap->keyboardBindings[1].actionID, lineNumber);
	TestCase_assert(inputMap->keyboardBindings[1].keyCode == 3, "Expected 3 but got %u (%u)", inputMap->keyboardBindings[1].keyCode, lineNumber);
	TestCase_assert(inputMap->keyboardBindings[1].charCode == 4, "Expected 4 but got %u (%u)", inputMap->keyboardBindings[1].charCode, lineNumber);
	TestCase_assert(inputMap->gamepadMapCount == 2, "Expected 2 but got %u (%u)", inputMap->gamepadMapCount, lineNumber);
	TestCase_assert(inputMap->gamepadMaps != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].vendorID == 1, "Expected 1 but got %d (%u)", inputMap->gamepadMaps[0].vendorID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].productID == 2, "Expected 2 but got %d (%u)", inputMap->gamepadMaps[0].productID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].buttonBindingCount == 2, "Expected 2 but got %d (%u)", inputMap->gamepadMaps[0].buttonBindingCount, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].buttonBindings != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].buttonBindings[0].actionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p) (%u)", ATOM("a"), inputMap->gamepadMaps[0].buttonBindings[0].actionID, inputMap->gamepadMaps[0].buttonBindings[0].actionID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].buttonBindings[0].buttonID == 3, "Expected 3 but got %u (%u)", inputMap->gamepadMaps[0].buttonBindings[0].buttonID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].buttonBindings[1].actionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p) (%u)", ATOM("b"), inputMap->gamepadMaps[0].buttonBindings[1].actionID, inputMap->gamepadMaps[0].buttonBindings[1].actionID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].buttonBindings[1].buttonID == 4, "Expected 4 but got %u (%u)", inputMap->gamepadMaps[0].buttonBindings[1].buttonID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].axisBindingCount == 2, "Expected 2 but got %d (%u)", inputMap->gamepadMaps[0].axisBindingCount, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].axisBindings != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].axisBindings[0].actionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p) (%u)", ATOM("a"), inputMap->gamepadMaps[0].axisBindings[0].actionID, inputMap->gamepadMaps[0].axisBindings[0].actionID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].axisBindings[0].axisID == 3, "Expected 3 but got %u (%u)", inputMap->gamepadMaps[0].axisBindings[0].axisID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].axisBindings[0].triggerThreshold == 0.5f, "Expected 0.5 but got %f (%u)", inputMap->gamepadMaps[0].axisBindings[0].triggerThreshold, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].axisBindings[0].releaseThreshold == 0.5f, "Expected 0.5 but got %f (%u)", inputMap->gamepadMaps[0].axisBindings[0].releaseThreshold, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].axisBindings[1].actionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p) (%u)", ATOM("b"), inputMap->gamepadMaps[0].axisBindings[1].actionID, inputMap->gamepadMaps[0].axisBindings[1].actionID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].axisBindings[1].axisID == 4, "Expected 4 but got %u (%u)", inputMap->gamepadMaps[0].axisBindings[1].axisID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].axisBindings[1].triggerThreshold == 0.5f, "Expected 0.5 but got %f (%u)", inputMap->gamepadMaps[0].axisBindings[1].triggerThreshold, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[0].axisBindings[1].releaseThreshold == 0.5f, "Expected 0.5 but got %f (%u)", inputMap->gamepadMaps[0].axisBindings[1].releaseThreshold, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].vendorID == 4, "Expected 4 but got %d (%u)", inputMap->gamepadMaps[1].vendorID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].productID == 5, "Expected 5 but got %d (%u)", inputMap->gamepadMaps[1].productID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].buttonBindingCount == 1, "Expected 1 but got %d (%u)", inputMap->gamepadMaps[1].buttonBindingCount, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].buttonBindings != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].buttonBindings[0].actionID == ATOM("c"), "Expected \"c\" (%p) but got \"%s\" (%p) (%u)", ATOM("c"), inputMap->gamepadMaps[1].buttonBindings[0].actionID, inputMap->gamepadMaps[1].buttonBindings[0].actionID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].buttonBindings[0].buttonID == 6, "Expected 6 but got %u (%u)", inputMap->gamepadMaps[1].buttonBindings[0].buttonID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].axisBindingCount == 1, "Expected 1 but got %d (%u)", inputMap->gamepadMaps[1].axisBindingCount, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].axisBindings != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].axisBindings[0].actionID == ATOM("c"), "Expected \"c\" (%p) but got \"%s\" (%p) (%u)", ATOM("c"), inputMap->gamepadMaps[1].axisBindings[0].actionID, inputMap->gamepadMaps[1].axisBindings[0].actionID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].axisBindings[0].axisID == 6, "Expected 6 but got %u (%u)", inputMap->gamepadMaps[1].axisBindings[0].axisID, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].axisBindings[0].triggerThreshold == 1.0f, "Expected 1.0 but got %f (%u)", inputMap->gamepadMaps[1].axisBindings[0].triggerThreshold, lineNumber);
	TestCase_assert(inputMap->gamepadMaps[1].axisBindings[0].releaseThreshold == 0.0f, "Expected 0.0 but got %f (%u)", inputMap->gamepadMaps[1].axisBindings[0].releaseThreshold, lineNumber);
}

static void testDeserialization() {
	InputMap inputMap, * inputMapPtr;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	bool success;
	unsigned int failIndex;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	InputMap_init(&inputMap);
	setUpBlankDeserializationContext(context);
	success = InputMap_loadSerializedData(&inputMap, context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(success, "Expected true but got false");
	verifyBlankInputMap(&inputMap, __LINE__);
	InputMap_dispose(&inputMap);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	setUpBlankDeserializationContext(context);
	inputMapPtr = InputMap_deserialize(context);
	context->finish(context);
	context->dispose(context);
	verifyBlankInputMap(inputMapPtr, __LINE__);
	inputMapPtr->dispose(inputMapPtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	InputMap_init(&inputMap);
	setUpBasicDeserializationContext(context);
	success = InputMap_loadSerializedData(&inputMap, context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(success, "Expected true but got false");
	verifyBasicInputMap(&inputMap, __LINE__);
	InputMap_dispose(&inputMap);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	setUpBasicDeserializationContext(context);
	inputMapPtr = InputMap_deserialize(context);
	context->finish(context);
	context->dispose(context);
	verifyBasicInputMap(inputMapPtr, __LINE__);
	inputMapPtr->dispose(inputMapPtr);
	
	for (failIndex = 0; failIndex < 57; failIndex++) {
		context = TestDeserializationContext_create(&jmpEnv);
		if (setjmp(jmpEnv) != 0) {
			TestCase_assert(false, "%s", context->error);
		}
		
		InputMap_init(&inputMap);
		setUpBasicDeserializationContext(context);
		context->failNthCall(context, failIndex, 1);
		success = InputMap_loadSerializedData(&inputMap, context);
		context->finish(context);
		context->dispose(context);
		TestCase_assert(!success, "InputMap_loadSerializedData didn't return false when deserialization call %d failed", failIndex);
		InputMap_dispose(&inputMap);
		
		context = TestDeserializationContext_create(&jmpEnv);
		if (setjmp(jmpEnv) != 0) {
			TestCase_assert(false, "%s", context->error);
		}
		
		setUpBasicDeserializationContext(context);
		context->failNthCall(context, failIndex, 1);
		inputMapPtr = InputMap_deserialize(context);
		context->finish(context);
		context->dispose(context);
		TestCase_assert(inputMapPtr == NULL, "InputMap_deserialize didn't return NULL when deserialization call %d failed", failIndex);
	}
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	InputMap_init(&inputMap);
	context->expectCall(context, context->beginStructure, "input_map");
	context->expectCall(context, context->readUInt16, "format_version", INPUT_MAP_SERIALIZATION_FORMAT_VERSION + 1);
	InputMap_loadSerializedData(&inputMap, context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(!success, "InputMap_loadSerializedData didn't return false when format version was too new");
	InputMap_dispose(&inputMap);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, "input_map");
	context->expectCall(context, context->readUInt16, "format_version", INPUT_MAP_SERIALIZATION_FORMAT_VERSION + 1);
	inputMapPtr = InputMap_deserialize(context);
	context->finish(context);
	context->dispose(context);
	TestCase_assert(inputMapPtr == NULL, "InputMap_deserialize didn't return NULL when format version was too new");
}

TEST_SUITE(InputMapTest,
           testInit,
           testKeyboardBindings,
           testKeyModifierBindings,
           testButtonBindings,
           testAxisBindings,
           testSerialization,
           testDeserialization)
