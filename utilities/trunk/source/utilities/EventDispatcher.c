/*
  Copyright (c) 2011 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
*/

#include "utilities/EventDispatcher.h"
#include <string.h>

struct EventTarget {
	Atom eventID;
	EventDispatcherCallback callback;
	void * context;
};

EventDispatcher * EventDispatcher_create(void * owner) {
	stemobject_create_implementation(EventDispatcher, init, owner)
}

void EventDispatcher_init(compat_type(EventDispatcher *) selfPtr, void * owner) {
	EventDispatcher * self = selfPtr;
	
	StemObject_init(self);
	
	self->dispose = EventDispatcher_dispose;
	self->registerForEvent = EventDispatcher_registerForEvent;
	self->unregisterForEvent = EventDispatcher_unregisterForEvent;
	self->dispatchEvent = EventDispatcher_dispatchEvent;
	
	self->owner = owner;
	self->numberOfTargets = 0;
	self->targetListSize = 1;
	self->targets = (struct EventTarget *) malloc(sizeof(struct EventTarget) * self->targetListSize);
}

void EventDispatcher_dispose(compat_type(EventDispatcher *) selfPtr) {
	EventDispatcher * self = selfPtr;
	
	free(self->targets);
	StemObject_dispose(selfPtr);
}

void EventDispatcher_registerForEvent(compat_type(EventDispatcher *) selfPtr, Atom eventID, EventDispatcherCallback callback, void * context) {
	EventDispatcher * self = selfPtr;
	
	if (self->numberOfTargets >= self->targetListSize) {
		self->targetListSize *= 2;
		self->targets = (struct EventTarget *) realloc(self->targets, sizeof(struct EventTarget) * self->targetListSize);
	}
	
	self->targets[self->numberOfTargets].eventID = eventID;
	self->targets[self->numberOfTargets].callback = callback;
	self->targets[self->numberOfTargets].context = context;
	self->numberOfTargets++;
}

void EventDispatcher_unregisterForEvent(compat_type(EventDispatcher *) selfPtr, Atom eventID, EventDispatcherCallback callback, void * context) {
	EventDispatcher * self = selfPtr;
	size_t targetIndex;
	
	for (targetIndex = 0; targetIndex < self->numberOfTargets; targetIndex++) {
		if (eventID == self->targets[targetIndex].eventID && callback == self->targets[targetIndex].callback && context == self->targets[targetIndex].context) {
			self->numberOfTargets--;
			for (; targetIndex < self->numberOfTargets; targetIndex++) {
				self->targets[targetIndex] = self->targets[targetIndex + 1];
			}
			break;
		}
	}
}

bool EventDispatcher_dispatchEvent(compat_type(EventDispatcher *) selfPtr, Atom eventID, void * eventData) {
	EventDispatcher * self = selfPtr;
	size_t targetIndex;
	size_t numberOfTargetsCopy;
	struct EventTarget * targetsCopy;
	bool eventHandled, anyEventsHandled;
	
	numberOfTargetsCopy = self->numberOfTargets;
	targetsCopy = malloc(sizeof(struct EventTarget) * numberOfTargetsCopy);
	memcpy(targetsCopy, self->targets, sizeof(struct EventTarget) * numberOfTargetsCopy);
	
	anyEventsHandled = false;
	for (targetIndex = 0; targetIndex < numberOfTargetsCopy; targetIndex++) {
		if (eventID == targetsCopy[targetIndex].eventID) {
			eventHandled = targetsCopy[targetIndex].callback(self->owner, eventID, eventData, targetsCopy[targetIndex].context);
			
			if (eventHandled) {
				anyEventsHandled = true;
			}
		}
	}
	
	free(targetsCopy);
	
	return anyEventsHandled;
}
