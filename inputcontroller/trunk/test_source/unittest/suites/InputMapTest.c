#include <string.h>
#include "unittest/framework/TestSuite.h"
#include "unittest/framework/TestDeserializationContext.h"
#include "unittest/framework/TestSerializationContext.h"
#include "inputcontroller/InputMap.h"

static void verifyInit(InputMap * inputMap, int callingLine) {
	TestCase_assert(inputMap != NULL, "Expected non-NULL but got NULL (line %d)", callingLine);
	TestCase_assert(inputMap->keyboardBindingCount == 0, "Expected 0 but got %u (line %d)", inputMap->keyboardBindingCount, callingLine);
	TestCase_assert(inputMap->gamepadMapCount == 0, "Expected 0 but got %u (line %d)", inputMap->gamepadMapCount, callingLine);
	TestCase_assert(inputMap->dispose       == InputMap_dispose,       "Expected %p but got %p (line %d)", InputMap_dispose,       inputMap->dispose,       callingLine);
	TestCase_assert(inputMap->isKeyBound    == InputMap_isKeyBound,    "Expected %p but got %p (line %d)", InputMap_isKeyBound,    inputMap->isKeyBound,    callingLine);
	TestCase_assert(inputMap->bindKey       == InputMap_bindKey,       "Expected %p but got %p (line %d)", InputMap_bindKey,       inputMap->bindKey,       callingLine);
	TestCase_assert(inputMap->unbindKey     == InputMap_unbindKey,     "Expected %p but got %p (line %d)", InputMap_unbindKey,     inputMap->unbindKey,     callingLine);
	TestCase_assert(inputMap->isButtonBound == InputMap_isButtonBound, "Expected %p but got %p (line %d)", InputMap_isButtonBound, inputMap->isButtonBound, callingLine);
	TestCase_assert(inputMap->bindButton    == InputMap_bindButton,    "Expected %p but got %p (line %d)", InputMap_bindButton,    inputMap->bindButton,    callingLine);
	TestCase_assert(inputMap->unbindButton  == InputMap_unbindButton,  "Expected %p but got %p (line %d)", InputMap_unbindButton,  inputMap->unbindButton,  callingLine);
	TestCase_assert(inputMap->isAxisBound   == InputMap_isAxisBound,   "Expected %p but got %p (line %d)", InputMap_isAxisBound,   inputMap->isAxisBound,   callingLine);
	TestCase_assert(inputMap->bindAxis      == InputMap_bindAxis,      "Expected %p but got %p (line %d)", InputMap_bindAxis,      inputMap->bindAxis,      callingLine);
	TestCase_assert(inputMap->unbindAxis    == InputMap_unbindAxis,    "Expected %p but got %p (line %d)", InputMap_unbindAxis,    inputMap->unbindAxis,    callingLine);
}

static void testInit() {
	InputMap inputMap, * inputMapPtr;
	
	memset(&inputMap, 0x00, sizeof(InputMap));
	InputMap_init(&inputMap);
	verifyInit(&inputMap, __LINE__);
	inputMap.dispose(&inputMap);
	
	memset(&inputMap, 0xFF, sizeof(InputMap));
	InputMap_init(&inputMap);
	verifyInit(&inputMap, __LINE__);
	inputMap.dispose(&inputMap);
	
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
	TestCase_assert(!inputMap->isKeyBound(inputMap, actionID, keyCode), "Key unexpectedly reported as bound (keyCode = %u, actionID = \"%s\") (line %d)", keyCode, charCode, actionID, callingLine);
	TestCase_assert(!isKeyBound(inputMap, actionID, keyCode, charCode), "Unexpectedly found bound key (keyCode = %u, charCode = %u, actionID = \"%s\") (line %d)", keyCode, charCode, actionID, callingLine);
}

static void verifyKeyBound(InputMap * inputMap, Atom actionID, unsigned int keyCode, unsigned int charCode, int callingLine) {
	TestCase_assert(inputMap->isKeyBound(inputMap, actionID, keyCode), "Key unexpectedly reported as unbound (keyCode = %u, charCode = %u, actionID = \"%s\") (line %d)", keyCode, charCode, actionID, callingLine);
	TestCase_assert(isKeyBound(inputMap, actionID, keyCode, charCode), "Couldn't find bound key (keyCode = %u, charCode = %u, actionID = \"%s\") (line %d)", keyCode, charCode, actionID, callingLine);
}

static void testKeyboardBindings() {
	InputMap * inputMap;
	
	inputMap = InputMap_create();
	verifyKeyNotBound(inputMap, ATOM("a"), 1, 3, __LINE__);
	verifyKeyNotBound(inputMap, ATOM("b"), 1, 2, __LINE__);
	
	inputMap->bindKey(inputMap, ATOM("a"), 1, 3);
	verifyKeyBound(inputMap, ATOM("a"), 1, 3, __LINE__);
	
	inputMap->bindKey(inputMap, ATOM("a"), 1, 2);
	verifyKeyBound(inputMap, ATOM("a"), 1, 2, __LINE__);
	TestCase_assert(!isKeyBound(inputMap, ATOM("a"), 1, 3), "Expected false but got true");
	
	inputMap->bindKey(inputMap, ATOM("a"), 2, 4);
	verifyKeyBound(inputMap, ATOM("a"), 1, 2, __LINE__);
	verifyKeyBound(inputMap, ATOM("a"), 2, 4, __LINE__);
	
	inputMap->bindKey(inputMap, ATOM("b"), 1, 2);
	verifyKeyBound(inputMap, ATOM("a"), 1, 2, __LINE__);
	verifyKeyBound(inputMap, ATOM("a"), 2, 4, __LINE__);
	verifyKeyBound(inputMap, ATOM("b"), 1, 2, __LINE__);
	
	inputMap->unbindKey(inputMap, ATOM("a"), 1);
	verifyKeyNotBound(inputMap, ATOM("a"), 1, 2, __LINE__);
	verifyKeyBound(inputMap, ATOM("a"), 2, 4, __LINE__);
	verifyKeyBound(inputMap, ATOM("b"), 1, 2, __LINE__);
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
	TestCase_assert(!inputMap->isButtonBound(inputMap, actionID, vendorID, productID, buttonID), "Button unexpectedly reported as bound (vendorID = %u, productID = %u, buttonID = %u, actionID = \"%s\") (line %d)", vendorID, productID, buttonID, actionID, callingLine);
	TestCase_assert(!isButtonBound(inputMap, actionID, vendorID, productID, buttonID), "Unexpectedly found bound button (vendorID = %u, productID = %u, buttonID = %u, actionID = \"%s\") (line %d)", vendorID, productID, buttonID, actionID, callingLine);
}

static void verifyButtonBound(InputMap * inputMap, Atom actionID, int vendorID, int productID, unsigned int buttonID, int callingLine) {
	TestCase_assert(inputMap->isButtonBound(inputMap, actionID, vendorID, productID, buttonID), "Button unexpectedly reported as unbound (vendorID = %u, productID = %u, buttonID = %u, actionID = \"%s\") (line %d)", vendorID, productID, buttonID, actionID, callingLine);
	TestCase_assert(isButtonBound(inputMap, actionID, vendorID, productID, buttonID), "Couldn't find bound button (vendorID = %u, productID = %u, buttonID = %u, actionID = \"%s\") (line %d)", vendorID, productID, buttonID, actionID, callingLine);
}

static void testButtonBindings() {
	InputMap * inputMap;
	
	inputMap = InputMap_create();
	verifyButtonNotBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	verifyButtonNotBound(inputMap, ATOM("a"), 0, 0, 1, __LINE__);
	verifyButtonNotBound(inputMap, ATOM("a"), 1, 1, 1, __LINE__);
	verifyButtonNotBound(inputMap, ATOM("b"), 0, 0, 0, __LINE__);
	
	inputMap->bindButton(inputMap, ATOM("a"), 0, 0, 0);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	
	inputMap->bindButton(inputMap, ATOM("a"), 0, 0, 1);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 1, __LINE__);
	
	inputMap->bindButton(inputMap, ATOM("a"), 1, 1, 1);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 1, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 1, 1, 1, __LINE__);
	
	inputMap->bindButton(inputMap, ATOM("b"), 0, 0, 0);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 1, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 1, 1, 1, __LINE__);
	verifyButtonBound(inputMap, ATOM("b"), 0, 0, 0, __LINE__);
	
	inputMap->unbindButton(inputMap, ATOM("a"), 0, 0, 0);
	verifyButtonNotBound(inputMap, ATOM("a"), 0, 0, 0, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 0, 0, 1, __LINE__);
	verifyButtonBound(inputMap, ATOM("a"), 1, 1, 1, __LINE__);
	verifyButtonBound(inputMap, ATOM("b"), 0, 0, 0, __LINE__);
}

static bool isAxisBound(InputMap * inputMap, Atom actionID, int vendorID, int productID, unsigned int axisID, int direction, float triggerThreshold, float releaseThreshold) {
	unsigned int gamepadMapIndex, bindingIndex;
	
	for (gamepadMapIndex = 0; gamepadMapIndex < inputMap->gamepadMapCount; gamepadMapIndex++) {
		if (inputMap->gamepadMaps[gamepadMapIndex].vendorID != vendorID ||
		    inputMap->gamepadMaps[gamepadMapIndex].productID != productID) {
			continue;
		}
		for (bindingIndex = 0; bindingIndex < inputMap->gamepadMaps[gamepadMapIndex].axisBindingCount; bindingIndex++) {
			if (inputMap->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].actionID == actionID &&
					inputMap->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].axisID == axisID &&
					inputMap->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].direction == direction &&
					inputMap->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].triggerThreshold == triggerThreshold &&
					inputMap->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].releaseThreshold == releaseThreshold) {
				return true;
			}
		}
	}
	return false;
}

static void verifyAxisNotBound(InputMap * inputMap, Atom actionID, int vendorID, int productID, unsigned int axisID, int direction, float triggerThreshold, float releaseThreshold, int callingLine) {
	TestCase_assert(!inputMap->isAxisBound(inputMap, actionID, vendorID, productID, axisID, direction), "Axis unexpectedly reported as bound (vendorID = %u, productID = %u, axisID = %u, direction = %d, triggerThreshold = %f, releaseThreshold = %f, actionID = \"%s\") (line %d)", vendorID, productID, axisID, direction, triggerThreshold, releaseThreshold, actionID, callingLine);
	TestCase_assert(!isAxisBound(inputMap, actionID, vendorID, productID, axisID, direction, triggerThreshold, releaseThreshold), "Unexpectedly found bound axis (vendorID = %u, productID = %u, axisID = %u, direction = %d, triggerThreshold = %f, releaseThreshold = %f, actionID = \"%s\") (line %d)", vendorID, productID, axisID, direction, triggerThreshold, releaseThreshold, actionID, callingLine);
}

static void verifyAxisBound(InputMap * inputMap, Atom actionID, int vendorID, int productID, unsigned int axisID, int direction, float triggerThreshold, float releaseThreshold, int callingLine) {
	TestCase_assert(inputMap->isAxisBound(inputMap, actionID, vendorID, productID, axisID, direction), "axis unexpectedly reported as unbound (vendorID = %u, productID = %u, axisID = %u, direction = %d, triggerThreshold = %f, releaseThreshold = %f, actionID = \"%s\") (line %d)", vendorID, productID, axisID, direction, triggerThreshold, releaseThreshold, actionID, callingLine);
	TestCase_assert(isAxisBound(inputMap, actionID, vendorID, productID, axisID, direction, triggerThreshold, releaseThreshold), "Couldn't find bound axis (vendorID = %u, productID = %u, axisID = %u, actionID, direction = %d, triggerThreshold = %f, releaseThreshold = %f = \"%s\") (line %d)", vendorID, productID, axisID, direction, triggerThreshold, releaseThreshold, actionID, callingLine);
}

static void testAxisBindings() {
	InputMap * inputMap;
	
	inputMap = InputMap_create();
	verifyAxisNotBound(inputMap, ATOM("a"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisNotBound(inputMap, ATOM("a"), 0, 0, 1, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisNotBound(inputMap, ATOM("a"), 1, 1, 1, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisNotBound(inputMap, ATOM("b"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	
	inputMap->bindAxis(inputMap, ATOM("a"), 0, 0, 0, 1, 1.0f, 0.0f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	
	inputMap->bindAxis(inputMap, ATOM("a"), 0, 0, 1, 1, 1.0f, 0.0f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1, 1.0f, 0.0f, __LINE__);
	
	inputMap->bindAxis(inputMap, ATOM("a"), 0, 0, 1, 1, 0.5f, 0.0f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1, 0.5f, 0.0f, __LINE__);
	TestCase_assert(!isAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1, 1.0f, 0.0f), "Expected false but got true");
	
	inputMap->bindAxis(inputMap, ATOM("a"), 0, 0, 1, 1, 0.5f, 0.5f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1, 0.5f, 0.5f, __LINE__);
	TestCase_assert(!isAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1, 0.5f, 0.0f), "Expected false but got true");
	
	inputMap->bindAxis(inputMap, ATOM("a"), 0, 0, 1, -1, 0.5f, 0.5f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, -1, 0.5f, 0.5f, __LINE__);
	
	inputMap->bindAxis(inputMap, ATOM("a"), 1, 1, 1, 1, 1.0f, 0.0f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, -1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 1, 1, 1, 1, 1.0f, 0.0f, __LINE__);
	
	inputMap->bindAxis(inputMap, ATOM("b"), 0, 0, 0, 1, 1.0f, 0.0f);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, -1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 1, 1, 1, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("b"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	
	inputMap->unbindAxis(inputMap, ATOM("a"), 0, 0, 0, 1);
	verifyAxisNotBound(inputMap, ATOM("a"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, 1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 0, 0, 1, -1, 0.5f, 0.5f, __LINE__);
	verifyAxisBound(inputMap, ATOM("a"), 1, 1, 1, 1, 1.0f, 0.0f, __LINE__);
	verifyAxisBound(inputMap, ATOM("b"), 0, 0, 0, 1, 1.0f, 0.0f, __LINE__);
}

static void testSerialization() {
}

static void testDeserialization() {
}

TEST_SUITE(InputMapTest,
           testInit,
           testKeyboardBindings,
           testButtonBindings,
           testAxisBindings,
           testSerialization,
           testDeserialization)
