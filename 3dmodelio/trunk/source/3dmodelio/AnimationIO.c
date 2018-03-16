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

#include "3dmodelio/AnimationIO.h"
#include <string.h>

Animation * Animation_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	stemobject_deserialize_implementation(Animation, deserializationContext, loadSerializedData)
}

bool Animation_loadSerializedData(Animation * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	const char * formatType, * nameString, * boneIDString, * markerNameString;
	unsigned int keyframeCount, keyframeIndex, boneIndex, markerCount, markerIndex;
	struct AnimationKeyframe * keyframes;
	struct AnimationMarker * markers;
	bool loop, success;
	
	context->beginStructure(context, ANIMATION_FORMAT_TYPE);
	formatVersion = context->readUInt16(context, "format_version");
	if (context->status != SERIALIZATION_ERROR_OK || formatVersion > ANIMATION_FORMAT_VERSION) {
		return false;
	}
	formatType = context->readString(context, "format_type");
	if (context->status != SERIALIZATION_ERROR_OK || strcmp(formatType, ANIMATION_FORMAT_TYPE)) {
		return false;
	}
	
	nameString = context->readStringNullable(context, "name");
	loop = context->readBoolean(context, "loop");
	keyframeCount = context->beginArray(context, "keyframes");
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	keyframes = malloc(sizeof(*keyframes) * keyframeCount);
	for (keyframeIndex = 0; keyframeIndex < keyframeCount; keyframeIndex++) {
		context->beginStructure(context, "keyframe");
		keyframes[keyframeIndex].interval = context->readDouble(context, "interval");
		keyframes[keyframeIndex].boneCount = context->beginDictionary(context, "bones");
		keyframes[keyframeIndex].bones = malloc(sizeof(*keyframes[keyframeIndex].bones) * keyframes[keyframeIndex].boneCount);
		for (boneIndex = 0; boneIndex < keyframes[keyframeIndex].boneCount; boneIndex++) {
			boneIDString = context->readNextDictionaryKey(context);
			context->beginStructure(context, boneIDString);
			context->beginStructure(context, "offset");
			keyframes[keyframeIndex].bones[boneIndex].offset.x = context->readFloat(context, "x");
			keyframes[keyframeIndex].bones[boneIndex].offset.y = context->readFloat(context, "y");
			keyframes[keyframeIndex].bones[boneIndex].offset.z = context->readFloat(context, "z");
			context->endStructure(context);
			context->beginStructure(context, "offset_curve");
			keyframes[keyframeIndex].bones[boneIndex].incomingOffsetBezierHandle.x = context->readFloat(context, "x_in");
			keyframes[keyframeIndex].bones[boneIndex].incomingOffsetBezierHandle.y = context->readFloat(context, "y_in");
			keyframes[keyframeIndex].bones[boneIndex].outgoingOffsetBezierHandle.x = context->readFloat(context, "x_out");
			keyframes[keyframeIndex].bones[boneIndex].outgoingOffsetBezierHandle.y = context->readFloat(context, "y_out");
			context->endStructure(context);
			context->beginStructure(context, "scale");
			keyframes[keyframeIndex].bones[boneIndex].scale.x = context->readFloat(context, "x");
			keyframes[keyframeIndex].bones[boneIndex].scale.y = context->readFloat(context, "y");
			keyframes[keyframeIndex].bones[boneIndex].scale.z = context->readFloat(context, "z");
			context->endStructure(context);
			context->beginStructure(context, "scale_curve");
			keyframes[keyframeIndex].bones[boneIndex].incomingScaleBezierHandle.x = context->readFloat(context, "x_in");
			keyframes[keyframeIndex].bones[boneIndex].incomingScaleBezierHandle.y = context->readFloat(context, "y_in");
			keyframes[keyframeIndex].bones[boneIndex].outgoingScaleBezierHandle.x = context->readFloat(context, "x_out");
			keyframes[keyframeIndex].bones[boneIndex].outgoingScaleBezierHandle.y = context->readFloat(context, "y_out");
			context->endStructure(context);
			context->beginStructure(context, "rotation");
			keyframes[keyframeIndex].bones[boneIndex].rotation.x = context->readFloat(context, "x");
			keyframes[keyframeIndex].bones[boneIndex].rotation.y = context->readFloat(context, "y");
			keyframes[keyframeIndex].bones[boneIndex].rotation.z = context->readFloat(context, "z");
			keyframes[keyframeIndex].bones[boneIndex].rotation.w = context->readFloat(context, "w");
			context->endStructure(context);
			context->beginStructure(context, "rotation_curve");
			keyframes[keyframeIndex].bones[boneIndex].incomingRotationBezierHandle.x = context->readFloat(context, "x_in");
			keyframes[keyframeIndex].bones[boneIndex].incomingRotationBezierHandle.y = context->readFloat(context, "y_in");
			keyframes[keyframeIndex].bones[boneIndex].outgoingRotationBezierHandle.x = context->readFloat(context, "x_out");
			keyframes[keyframeIndex].bones[boneIndex].outgoingRotationBezierHandle.y = context->readFloat(context, "y_out");
			context->endStructure(context);
			context->endStructure(context);
			
			if (context->status != SERIALIZATION_ERROR_OK) {
				break;
			}
			keyframes[keyframeIndex].bones[boneIndex].boneID = Atom_fromString(boneIDString);
		}
		context->endDictionary(context);
		context->endStructure(context);
		
		if (context->status != SERIALIZATION_ERROR_OK) {
			break;
		}
	}
	context->endArray(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		unsigned int keyframeIndex2;
		
		for (keyframeIndex2 = 0; keyframeIndex2 <= keyframeIndex && keyframeIndex2 < keyframeCount; keyframeIndex2++) {
			free(keyframes[keyframeIndex2].bones);
		}
		free(keyframes);
		return false;
	}
	
	markerCount = context->beginDictionary(context, "markers");
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	markers = malloc(sizeof(*markers) * markerCount);
	for (markerIndex = 0; markerIndex < markerCount; markerIndex++) {
		markerNameString = context->readNextDictionaryKey(context);
		markers[markerIndex].time = context->readDouble(context, markerNameString);
		if (context->status != SERIALIZATION_ERROR_OK) {
			break;
		}
		markers[markerIndex].name = Atom_fromString(markerNameString);
	}
	context->endDictionary(context);
	
	context->endStructure(context);
	if (context->status != SERIALIZATION_ERROR_OK) {
		for (keyframeIndex = 0; keyframeIndex < keyframeCount; keyframeIndex++) {
			free(keyframes[keyframeIndex].bones);
		}
		free(keyframes);
		free(markers);
		return false;
	}
	
	success = Animation_init(self, Atom_fromString(nameString), loop, keyframeCount, keyframes, markerCount, markers);
	for (keyframeIndex = 0; keyframeIndex < keyframeCount; keyframeIndex++) {
		free(keyframes[keyframeIndex].bones);
	}
	free(keyframes);
	free(markers);
	return success;
}

void Animation_serialize(Animation * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	unsigned int keyframeIndex, boneIndex, markerIndex;
	
	context->beginStructure(context, ANIMATION_FORMAT_TYPE);
	context->writeUInt16(context, "format_version", ANIMATION_FORMAT_VERSION);
	context->writeString(context, "format_type", ANIMATION_FORMAT_TYPE);
	context->writeStringNullable(context, "name", self->name);
	context->writeBoolean(context, "loop", self->loop);
	context->beginArray(context, "keyframes");
	for (keyframeIndex = 0; keyframeIndex < self->keyframeCount; keyframeIndex++) {
		context->beginStructure(context, "keyframe");
		context->writeDouble(context, "interval", self->keyframes[keyframeIndex].interval);
		context->beginDictionary(context, "bones");
		for (boneIndex = 0; boneIndex < self->keyframes[keyframeIndex].boneCount; boneIndex++) {
			context->beginStructure(context, self->keyframes[keyframeIndex].bones[boneIndex].boneID);
			context->beginStructure(context, "offset");
			context->writeFloat(context, "x", self->keyframes[keyframeIndex].bones[boneIndex].offset.x);
			context->writeFloat(context, "y", self->keyframes[keyframeIndex].bones[boneIndex].offset.y);
			context->writeFloat(context, "z", self->keyframes[keyframeIndex].bones[boneIndex].offset.z);
			context->endStructure(context);
			context->beginStructure(context, "offset_curve");
			context->writeFloat(context, "x_in",  self->keyframes[keyframeIndex].bones[boneIndex].incomingOffsetBezierHandle.x);
			context->writeFloat(context, "y_in",  self->keyframes[keyframeIndex].bones[boneIndex].incomingOffsetBezierHandle.y);
			context->writeFloat(context, "x_out", self->keyframes[keyframeIndex].bones[boneIndex].outgoingOffsetBezierHandle.x);
			context->writeFloat(context, "y_out", self->keyframes[keyframeIndex].bones[boneIndex].outgoingOffsetBezierHandle.y);
			context->endStructure(context);
			context->beginStructure(context, "scale");
			context->writeFloat(context, "x", self->keyframes[keyframeIndex].bones[boneIndex].scale.x);
			context->writeFloat(context, "y", self->keyframes[keyframeIndex].bones[boneIndex].scale.y);
			context->writeFloat(context, "z", self->keyframes[keyframeIndex].bones[boneIndex].scale.z);
			context->endStructure(context);
			context->beginStructure(context, "scale_curve");
			context->writeFloat(context, "x_in",  self->keyframes[keyframeIndex].bones[boneIndex].incomingScaleBezierHandle.x);
			context->writeFloat(context, "y_in",  self->keyframes[keyframeIndex].bones[boneIndex].incomingScaleBezierHandle.y);
			context->writeFloat(context, "x_out", self->keyframes[keyframeIndex].bones[boneIndex].outgoingScaleBezierHandle.x);
			context->writeFloat(context, "y_out", self->keyframes[keyframeIndex].bones[boneIndex].outgoingScaleBezierHandle.y);
			context->endStructure(context);
			context->beginStructure(context, "rotation");
			context->writeFloat(context, "x", self->keyframes[keyframeIndex].bones[boneIndex].rotation.x);
			context->writeFloat(context, "y", self->keyframes[keyframeIndex].bones[boneIndex].rotation.y);
			context->writeFloat(context, "z", self->keyframes[keyframeIndex].bones[boneIndex].rotation.z);
			context->writeFloat(context, "w", self->keyframes[keyframeIndex].bones[boneIndex].rotation.w);
			context->endStructure(context);
			context->beginStructure(context, "rotation_curve");
			context->writeFloat(context, "x_in",  self->keyframes[keyframeIndex].bones[boneIndex].incomingRotationBezierHandle.x);
			context->writeFloat(context, "y_in",  self->keyframes[keyframeIndex].bones[boneIndex].incomingRotationBezierHandle.y);
			context->writeFloat(context, "x_out", self->keyframes[keyframeIndex].bones[boneIndex].outgoingRotationBezierHandle.x);
			context->writeFloat(context, "y_out", self->keyframes[keyframeIndex].bones[boneIndex].outgoingRotationBezierHandle.y);
			context->endStructure(context);
			context->endStructure(context);
		}
		context->endDictionary(context);
		context->endStructure(context);
	}
	context->endArray(context);
	context->beginDictionary(context, "markers");
	for (markerIndex = 0; markerIndex < self->markerCount; markerIndex++) {
		context->writeDouble(context, self->markers[markerIndex].name, self->markers[markerIndex].time);
	}
	context->endDictionary(context);
	context->endStructure(context);
}
