/*
  Copyright (c) 2015 Alex Diener
  
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

#include "collision/IntersectionManager.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

IntersectionManager * IntersectionManager_create() {
	stemobject_create_implementation(IntersectionManager, init)
}

IntersectionManager * IntersectionManager_createWithStandardHandlers() {
	stemobject_create_implementation(IntersectionManager, initWithStandardHandlers)
}

static void sharedInit(IntersectionManager * self) {
	call_super(init, self);
	self->dispose = IntersectionManager_dispose;
}

bool IntersectionManager_init(IntersectionManager * self) {
	sharedInit(self);
	return true;
}

bool IntersectionManager_initWithStandardHandlers(IntersectionManager * self) {
	sharedInit(self);
	return true;
}

void IntersectionManager_dispose(IntersectionManager * self) {
	call_super(dispose, self);
}

void IntersectionManager_setHandler(IntersectionManager * self, int type1, int type2, IntersectionHandler handler) {
}

IntersectionHandler IntersectionManager_getHandler(IntersectionManager * self, int type1, int type2) {
	return NULL;
}

bool IntersectionManager_callHandler(IntersectionManager * self, CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}
