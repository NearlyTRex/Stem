#include "unittest/TestSuite.h"
#include "utilities/Atom.h"
#include "utilities/EventDispatcher.h"

static void * lastSender;

static bool callback1(void * sender, Atom eventID, void * eventData, void * context) {
	*(int *) context += *(int *) eventData;
	TestCase_assert(eventID == Atom_fromString("event1"), "Expected %p \"event1\" but got %p \"%s\"", Atom_fromString("event1"), eventID, eventID);
	lastSender = sender;
	return true;
}

static bool callback2(void * sender, Atom eventID, void * eventData, void * context) {
	*(int *) context += *(int *) eventData;
	TestCase_assert(eventID == Atom_fromString("event2"), "Expected %p \"event2\" but got %p \"%s\"", Atom_fromString("event2"), eventID, eventID);
	lastSender = sender;
	return false;
}

static void testCallbacks() {
	EventDispatcher * dispatcher;
	char * sender1 = "1", * sender2 = "2";
	int callback1Context = 0, callback2Context = 0;
	int incrementation = 2;
	bool handled;
	Atom event1Atom, event2Atom;
	
	dispatcher = EventDispatcher_create(sender1);
	
	event1Atom = Atom_fromString("event1");
	event2Atom = Atom_fromString("event2");
	handled = dispatcher->dispatchEvent(dispatcher, event1Atom, &incrementation);
	TestCase_assert(!handled, "Event handled when no callbacks are registered");
	
	dispatcher->registerForEvent(dispatcher, event1Atom, callback1, &callback1Context);
	handled = dispatcher->dispatchEvent(dispatcher, event1Atom, &incrementation);
	TestCase_assert(handled, "Event not handled when appropriate callback was registered");
	TestCase_assert(callback1Context == 2, "Expected 2 but got %d", callback1Context);
	TestCase_assert(lastSender == sender1, "Expected %p but got %p", sender1, lastSender);
	
	incrementation = 3;
	dispatcher->dispatchEvent(dispatcher, event2Atom, &incrementation);
	TestCase_assert(callback1Context == 2, "Callback called for wrong type of event");
	
	dispatcher->registerForEvent(dispatcher, event2Atom, callback2, &callback2Context);
	handled = dispatcher->dispatchEvent(dispatcher, event2Atom, &incrementation);
	TestCase_assert(callback2Context == 3, "Expected 3 but got %d", callback2Context);
	TestCase_assert(!handled, "Expected false but got true");
	
	dispatcher->unregisterForEvent(dispatcher, event1Atom, callback1, NULL);
	handled = dispatcher->dispatchEvent(dispatcher, event1Atom, &incrementation);
	TestCase_assert(handled, "Event not still handled after unregistering callbacks with the wrong context");
	
	dispatcher->unregisterForEvent(dispatcher, event1Atom, callback1, &callback1Context);
	handled = dispatcher->dispatchEvent(dispatcher, event1Atom, &incrementation);
	TestCase_assert(!handled, "Event still handled after unregistering callbacks");
	
	incrementation = 1;
	handled = dispatcher->dispatchEvent(dispatcher, event2Atom, &incrementation);
	TestCase_assert(callback2Context == 4, "Expected 4 but got %d", callback2Context);
	TestCase_assert(!handled, "Expected false but got true");
	
	dispatcher->dispose(dispatcher);
	
	dispatcher = EventDispatcher_create(sender2);
	dispatcher->registerForEvent(dispatcher, event1Atom, callback1, &callback1Context);
	dispatcher->dispatchEvent(dispatcher, event1Atom, &incrementation);
	TestCase_assert(lastSender == sender2, "Expected %p but got %p", sender2, lastSender);
	
	dispatcher->dispose(dispatcher);
}

TEST_SUITE(EventDispatcherTest, testCallbacks)
