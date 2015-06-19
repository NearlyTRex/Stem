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

#ifndef __IntersectionManager_H__
#define __IntersectionManager_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct IntersectionManager IntersectionManager;

#include "collision/CollisionObject.h"
#include "gamemath/FixedPoint.h"
#include "gamemath/Vector3x.h"
#include "stemobject/StemObject.h"
#include <stdlib.h>

// IntersectionHandler rules:
// - Return true and write to outTime (if not null) and outNormal (if not null) if an intersection is detected
// - Return false and do not modify outTime or outNormal if no intersection is detected
// - Values written to outTime MUST be in the range of [0x00000, 0x10000]; if computed time is outside this range, return false instead
// - Values written to outNormal must be normalized, and should point away from object1's trajectory
// - Values written to outNormal for 2D shapes must have the z component set to 0x00000
typedef bool (* IntersectionHandler)(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);

struct IntersectionHandlerEntry {
	int type1;
	int type2;
	IntersectionHandler handler;
};

#define IntersectionManager_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	size_t private_ivar(handlerCount); \
	struct IntersectionHandlerEntry * private_ivar(handlers);

stemobject_struct_definition(IntersectionManager)

IntersectionManager * IntersectionManager_create();
IntersectionManager * IntersectionManager_createWithStandardHandlers();
bool IntersectionManager_init(IntersectionManager * self);
bool IntersectionManager_initWithStandardHandlers(IntersectionManager * self);
void IntersectionManager_dispose(IntersectionManager * self);
void IntersectionManager_addStandardHandlers(IntersectionManager * self);
void IntersectionManager_setHandler(IntersectionManager * self, int type1, int type2, IntersectionHandler handler);
IntersectionHandler IntersectionManager_getHandler(IntersectionManager * self, int type1, int type2);
bool IntersectionManager_callHandler(IntersectionManager * self, CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);

#ifdef __cplusplus
}
#endif
#endif
