/*
  Copyright (c) 2013 Alex Diener
  
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

#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__

#include "stemobject/StemObject.h"
#include "utilities/Atom.h"
#include <stdlib.h>

typedef struct EventDispatcher EventDispatcher;

/* Signature for event handler callbacks.
   
   sender: Object that dispatched the event. More specifically, the object passed to EventDispatcher_create.
   eventID: Name of event that was triggered
   eventData: Arbitrary data passed by dispatcher. Its format is known by convention depending on the event ID being dispatched.
   context: Value passed as context to registerForEvent
   
   This function should return true if the event was handled, or false if it was ignored. */
typedef bool (* EventDispatcherCallback)(void * sender, Atom eventID, void * eventData, void * context);

struct EventTarget;

#define EventDispatcher_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	void * owner; \
	\
	size_t numberOfTargets; \
	size_t targetListSize; \
	struct EventTarget * targets; \
	\
	void (* registerForEvent)(self_type * self, Atom eventID, EventDispatcherCallback callback, void * context); \
	void (* unregisterForEvent)(self_type * self, Atom eventID, EventDispatcherCallback callback, void * context); \
	bool (* dispatchEvent)(self_type * self, Atom eventID, void * eventData);

stemobject_struct_definition(EventDispatcher)

/* Allocate and initialize a new EventDispatcher object. owner will be passed to event callbacks as
   the sender parameter. */
EventDispatcher * EventDispatcher_create(void * owner);

/* Initialize an already allocated EventDispatcher. owner will be passed to event callbacks as the
   sender parameter. */
bool EventDispatcher_init(EventDispatcher * self, void * owner);

/* Free all memory allocated by EventDispatcher (including self if allocated with
   EventDispatcher_create()), and remove all registered listeners. */
void EventDispatcher_dispose(EventDispatcher * self);

/* Register for notification of events of type eventID */
void EventDispatcher_registerForEvent(EventDispatcher * self, Atom eventID, EventDispatcherCallback callback, void * context);

/* Remove a previous registration for events of type eventID. Callback and context must match the pointers passed to registerForEvent. */
void EventDispatcher_unregisterForEvent(EventDispatcher * self, Atom eventID, EventDispatcherCallback callback, void * context);

/* Dispatch an event to all registered listeners for that event ID. Returns true if any listener is
   registered and returns true from its handler callback. */
bool EventDispatcher_dispatchEvent(EventDispatcher * self, Atom eventID, void * eventData);

#endif
