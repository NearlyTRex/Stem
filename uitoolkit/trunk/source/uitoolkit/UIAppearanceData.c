/*
  Copyright (c) 2018 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener alex@ludobloom.com
*/

#include "uitoolkit/UIAppearanceData.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

UIAppearanceData * UIAppearanceData_create(Atom name, Atom atlasName, Atom fontName, struct UIAppearance_metrics metrics) {
	stemobject_create_implementation(UIAppearanceData, init, name, atlasName, fontName, metrics)
}

bool UIAppearanceData_init(UIAppearanceData * self, Atom name, Atom atlasName, Atom fontName, struct UIAppearance_metrics metrics) {
	call_super(init, self);
	self->dispose = UIAppearanceData_dispose;
	self->name = name;
	self->atlasName = atlasName;
	self->fontName = fontName;
	self->metrics = metrics;
	return true;
}

void UIAppearanceData_dispose(UIAppearanceData * self) {
	call_super(dispose, self);
}

UIAppearanceData * UIAppearanceData_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	stemobject_deserialize_implementation(UIAppearanceData, deserializationContext, loadSerializedData)
}

static Color4f readColor4f(DeserializationContext * context, const char * key) {
	Color4f color;
	context->beginStructure(context, key);
	color.red = context->readFloat(context, "red");
	color.green = context->readFloat(context, "green");
	color.blue = context->readFloat(context, "blue");
	color.alpha = context->readFloat(context, "alpha");
	context->endStructure(context);
	return color;
}

static struct UIAppearance_sliceGrid readSliceGrid(DeserializationContext * context, const char * key) {
	struct UIAppearance_sliceGrid sliceGrid;
	context->beginStructure(context, key);
	sliceGrid.leftColumn = context->readFloat(context, "leftColumn");
	sliceGrid.centerColumn = context->readFloat(context, "centerColumn");
	sliceGrid.rightColumn = context->readFloat(context, "rightColumn");
	sliceGrid.bottomRow = context->readFloat(context, "bottomRow");
	sliceGrid.centerRow = context->readFloat(context, "centerRow");
	sliceGrid.topRow = context->readFloat(context, "topRow");
	context->endStructure(context);
	return sliceGrid;
}

bool UIAppearanceData_loadSerializedData(UIAppearanceData * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	const char * formatType, * nameString, * atlasNameString, * fontNameString, * metricName;
	struct UIAppearance_metrics metrics;
	size_t keyCount, keyIndex;
	
	context->beginStructure(context, UIAPPEARANCEDATA_FORMAT_TYPE);
	formatVersion = context->readUInt16(context, "format_version");
	if (context->status != SERIALIZATION_ERROR_OK || formatVersion > UIAPPEARANCEDATA_FORMAT_VERSION) {
		return false;
	}
	formatType = context->readString(context, "format_type");
	if (context->status != SERIALIZATION_ERROR_OK || strcmp(formatType, UIAPPEARANCEDATA_FORMAT_TYPE)) {
		return false;
	}
	
	nameString = context->readStringNullable(context, "name");
	atlasNameString = context->readStringNullable(context, "atlas_name");
	fontNameString = context->readStringNullable(context, "font_name");
	metrics = UIAppearance_defaultMetrics();
	keyCount = context->beginDictionary(context, "metrics");
	for (keyIndex = 0; keyIndex < keyCount; keyIndex++) {
		metricName = context->readNextDictionaryKey(context);
		if (!strcmp(metricName, "fontHeight")) {
			metrics.fontHeight = context->readFloat(context, metricName);
			
		} else if (!strcmp(metricName, "buttonTextPadding")) {
			metrics.buttonTextPadding = context->readFloat(context, metricName);
			
		} else if (!strcmp(metricName, "buttonTextColor")) {
			metrics.buttonTextColor = readColor4f(context, metricName);
			
		} else if (!strcmp(metricName, "buttonSlices")) {
			metrics.buttonSlices = readSliceGrid(context, metricName);
			
		} else if (!strcmp(metricName, "checkboxGraphicWidth")) {
			metrics.checkboxGraphicWidth = context->readFloat(context, metricName);
			
		} else if (!strcmp(metricName, "checkboxGraphicHeight")) {
			metrics.checkboxGraphicHeight = context->readFloat(context, metricName);
			
		} else if (!strcmp(metricName, "checkboxTextPadding")) {
			metrics.checkboxTextPadding = context->readFloat(context, metricName);
			
		} else if (!strcmp(metricName, "checkboxTextColor")) {
			metrics.checkboxTextColor = readColor4f(context, metricName);
			
		} else if (!strcmp(metricName, "radioButtonGraphicWidth")) {
			metrics.radioButtonGraphicWidth = context->readFloat(context, metricName);
			
		} else if (!strcmp(metricName, "radioButtonGraphicHeight")) {
			metrics.radioButtonGraphicHeight = context->readFloat(context, metricName);
			
		} else if (!strcmp(metricName, "radioButtonTextPadding")) {
			metrics.radioButtonTextPadding = context->readFloat(context, metricName);
			
		} else if (!strcmp(metricName, "radioButtonTextColor")) {
			metrics.radioButtonTextColor = readColor4f(context, metricName);
		}
	}
	context->endDictionary(context);
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	
	return UIAppearanceData_init(self, Atom_fromString(nameString), Atom_fromString(atlasNameString), Atom_fromString(fontNameString), metrics);
}

static void writeColor4f(SerializationContext * context, const char * key, Color4f color) {
	context->beginStructure(context, key);
	context->writeFloat(context, "red", color.red);
	context->writeFloat(context, "green", color.green);
	context->writeFloat(context, "blue", color.blue);
	context->writeFloat(context, "alpha", color.alpha);
	context->endStructure(context);
}

static void writeSliceGrid(SerializationContext * context, const char * key, struct UIAppearance_sliceGrid sliceGrid) {
	context->beginStructure(context, key);
	context->writeFloat(context, "leftColumn", sliceGrid.leftColumn);
	context->writeFloat(context, "centerColumn", sliceGrid.centerColumn);
	context->writeFloat(context, "rightColumn", sliceGrid.rightColumn);
	context->writeFloat(context, "bottomRow", sliceGrid.bottomRow);
	context->writeFloat(context, "centerRow", sliceGrid.centerRow);
	context->writeFloat(context, "topRow", sliceGrid.topRow);
	context->endStructure(context);
}

void UIAppearanceData_serialize(UIAppearanceData * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	
	context->beginStructure(context, UIAPPEARANCEDATA_FORMAT_TYPE);
	context->writeUInt16(context, "format_version", UIAPPEARANCEDATA_FORMAT_VERSION);
	context->writeString(context, "format_type", UIAPPEARANCEDATA_FORMAT_TYPE);
	context->writeStringNullable(context, "name", self->name);
	context->writeStringNullable(context, "atlas_name", self->atlasName);
	context->writeStringNullable(context, "font_name", self->fontName);
	context->beginDictionary(context, "metrics");
	context->writeFloat(context, "fontHeight", self->metrics.fontHeight);
	context->writeFloat(context, "buttonTextPadding", self->metrics.buttonTextPadding);
	writeColor4f(context, "buttonTextColor", self->metrics.buttonTextColor);
	writeSliceGrid(context, "buttonSlices", self->metrics.buttonSlices);
	context->writeFloat(context, "checkboxGraphicWidth", self->metrics.checkboxGraphicWidth);
	context->writeFloat(context, "checkboxGraphicHeight", self->metrics.checkboxGraphicHeight);
	context->writeFloat(context, "checkboxTextPadding", self->metrics.checkboxTextPadding);
	writeColor4f(context, "checkboxTextColor", self->metrics.checkboxTextColor);
	context->writeFloat(context, "radioButtonGraphicWidth", self->metrics.radioButtonGraphicWidth);
	context->writeFloat(context, "radioButtonGraphicHeight", self->metrics.radioButtonGraphicHeight);
	context->writeFloat(context, "radioButtonTextPadding", self->metrics.radioButtonTextPadding);
	writeColor4f(context, "radioButtonTextColor", self->metrics.radioButtonTextColor);
	context->endDictionary(context);
	context->endStructure(context);
}

UIAppearance * UIAppearanceData_createUIAppearance(UIAppearanceData * self, TextureAtlas * atlas, GLBitmapFont * font) {
	return UIAppearance_create(atlas, font, self->metrics);
}
