/*
  Copyright (c) 2016 Alex Diener
  
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

#include "shell/Shell.h"
#include "shell/ShellKeyCodes.h"
#include "testharness/SharedEvents.h"
#include "testharness/TestHarness_globals.h"
#include "testharness/TrimeshViewerScreen.h"
#include <stdlib.h>

#define SUPERCLASS Screen

TrimeshViewerScreen * TrimeshViewerScreen_create(ResourceManager * resourceManager) {
	stemobject_create_implementation(TrimeshViewerScreen, init, resourceManager)
}

bool TrimeshViewerScreen_init(TrimeshViewerScreen * self, ResourceManager * resourceManager) {
	call_super(init, self);
	self->dispose = TrimeshViewerScreen_dispose;
	return true;
}

void TrimeshViewerScreen_dispose(TrimeshViewerScreen * self) {
	if (self->trimesh != NULL) {
		CollisionStaticTrimesh_dispose(self->trimesh);
		self->trimesh = NULL;
	}
	call_super(dispose, self);
}

static bool draw(Atom eventID, void * eventData, void * context) {
	// Draw loaded trimesh and all of its normals
	return false;
}

static bool keyDown(Atom eventID, void * eventData, void * context) {
	// Load trimesh with key
	return false;
}

static bool mouseDown(Atom eventID, void * eventData, void * context) {
	Shell_setMouseDeltaMode(true);
	Shell_redisplay();
	return true;
}

static bool mouseUp(Atom eventID, void * eventData, void * context) {
	Shell_setMouseDeltaMode(false);
	Shell_redisplay();
	return true;
}

#define CAMERA_LOOK_SENSITIVITY 0x00100
#define CAMERA_ZOOM_SENSITIVITY 0x00060
#define CAMERA_DRAG_SENSITIVITY 0x00060

static bool mouseDragged(Atom eventID, void * eventData, void * context) {
	TrimeshViewerScreen * self = context;
	struct mouseEvent * event = eventData;
	Vector3x offset;
	
	if (g_shiftKeyDown) {
		// Pan
		if (g_controlKeyDown) {
			offset.x = 0x00000;
			offset.y = 0x00000;
			offset.z = -xmul(ftox(event->position.y), xmul(CAMERA_DRAG_SENSITIVITY, self->cameraDistance));
		} else {
			offset.x = xmul(ftox(event->position.x), xmul(CAMERA_DRAG_SENSITIVITY, self->cameraDistance));
			offset.y = -xmul(ftox(event->position.y), xmul(CAMERA_DRAG_SENSITIVITY, self->cameraDistance));
			offset.z = 0x00000;
		}
		offset = Quaternionx_multiplyVector3x(Quaternionx_inverted(self->cameraDirection), offset);
		self->cameraFocus = Vector3x_add(self->cameraFocus, offset);
		Shell_redisplay();
		
	} else if (g_controlKeyDown) {
		// Zoom
		self->cameraDistance += xmul(ftox(event->position.y), xmul(CAMERA_ZOOM_SENSITIVITY, self->cameraDistance));
		if (self->cameraDistance < 0x10000) {
			self->cameraDistance = 0x10000;
		}
		Shell_redisplay();
		
	} else {
		// Rotate
		Quaternionx_rotate(&self->cameraDirection, VECTOR3x_UP, xmul(ftox(event->position.x), CAMERA_LOOK_SENSITIVITY));
		Quaternionx_rotate(&self->cameraDirection, Quaternionx_multiplyVector3x(Quaternionx_inverted(self->cameraDirection), VECTOR3x_RIGHT), xmul(ftox(event->position.y), CAMERA_LOOK_SENSITIVITY));
		Shell_redisplay();
	}
	
	return true;
}

static bool resized(Atom eventID, void * eventData, void * context) {
	Shell_redisplay();
	return true;
}

void TrimeshViewerScreen_activate(TrimeshViewerScreen * self) {
	self->trimesh = NULL;
	self->cameraFocus = VECTOR3x_ZERO;
	self->cameraDirection = QUATERNIONx_IDENTITY;
	self->cameraDistance = 0xF0000;
	
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DOWN), mouseDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_UP), mouseUp, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DRAGGED), mouseDragged, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
	
	Shell_redisplay();
}

void TrimeshViewerScreen_deactivate(TrimeshViewerScreen * self) {
	if (self->trimesh != NULL) {
		CollisionStaticTrimesh_dispose(self->trimesh);
		self->trimesh = NULL;
	}
	
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DOWN), mouseDown, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_UP), mouseUp, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DRAGGED), mouseDragged, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
}
