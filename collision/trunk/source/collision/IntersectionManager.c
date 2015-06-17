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
#include "collision/StandardIntersectionHandlers.h"
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
	self->private_ivar(handlerCount) = 0;
	self->private_ivar(handlers) = NULL;
}

bool IntersectionManager_init(IntersectionManager * self) {
	sharedInit(self);
	return true;
}

bool IntersectionManager_initWithStandardHandlers(IntersectionManager * self) {
	sharedInit(self);
	IntersectionManager_addStandardHandlers(self);
	return true;
}

void IntersectionManager_dispose(IntersectionManager * self) {
	free(self->private_ivar(handlers));
	call_super(dispose, self);
}

void IntersectionManager_addStandardHandlers(IntersectionManager * self) {
	IntersectionManager_setHandler(self, COLLISION_SHAPE_RECT, COLLISION_SHAPE_RECT, intersectionHandler_rect_rect);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_RECT, COLLISION_SHAPE_CIRCLE, intersectionHandler_rect_circle);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_RECT, COLLISION_SHAPE_LINE_2D, intersectionHandler_rect_line2D);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_RECT, COLLISION_SHAPE_POLYGON, intersectionHandler_rect_polygon);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_CIRCLE, COLLISION_SHAPE_CIRCLE, intersectionHandler_circle_circle);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_CIRCLE, COLLISION_SHAPE_LINE_2D, intersectionHandler_circle_line2D);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_CIRCLE, COLLISION_SHAPE_POLYGON, intersectionHandler_circle_polygon);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_LINE_2D, COLLISION_SHAPE_LINE_2D, intersectionHandler_line2D_line2D);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_LINE_2D, COLLISION_SHAPE_POLYGON, intersectionHandler_line2D_polygon);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_POLYGON, COLLISION_SHAPE_POLYGON, intersectionHandler_polygon_polygon);
	
	IntersectionManager_setHandler(self, COLLISION_SHAPE_BOX, COLLISION_SHAPE_BOX, intersectionHandler_box_box);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_BOX, COLLISION_SHAPE_SPHERE, intersectionHandler_box_sphere);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_BOX, COLLISION_SHAPE_LINE_3D, intersectionHandler_box_line3D);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_BOX, COLLISION_SHAPE_CYLINDER, intersectionHandler_box_cylinder);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_BOX, COLLISION_SHAPE_CAPSULE, intersectionHandler_box_capsule);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_BOX, COLLISION_SHAPE_TRIMESH, intersectionHandler_box_trimesh);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_SPHERE, COLLISION_SHAPE_SPHERE, intersectionHandler_sphere_sphere);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_SPHERE, COLLISION_SHAPE_LINE_3D, intersectionHandler_sphere_line3D);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_SPHERE, COLLISION_SHAPE_CYLINDER, intersectionHandler_sphere_cylinder);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_SPHERE, COLLISION_SHAPE_CAPSULE, intersectionHandler_sphere_capsule);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_SPHERE, COLLISION_SHAPE_TRIMESH, intersectionHandler_sphere_trimesh);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_LINE_3D, COLLISION_SHAPE_LINE_3D, intersectionHandler_line3D_line3D);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_LINE_3D, COLLISION_SHAPE_CYLINDER, intersectionHandler_line3D_cylinder);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_LINE_3D, COLLISION_SHAPE_CAPSULE, intersectionHandler_line3D_capsule);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_LINE_3D, COLLISION_SHAPE_TRIMESH, intersectionHandler_line3D_trimesh);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_CYLINDER, COLLISION_SHAPE_CYLINDER, intersectionHandler_cylinder_cylinder);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_CYLINDER, COLLISION_SHAPE_CAPSULE, intersectionHandler_cylinder_capsule);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_CYLINDER, COLLISION_SHAPE_TRIMESH, intersectionHandler_cylinder_trimesh);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_CAPSULE, COLLISION_SHAPE_CAPSULE, intersectionHandler_capsule_capsule);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_CAPSULE, COLLISION_SHAPE_TRIMESH, intersectionHandler_capsule_trimesh);
	IntersectionManager_setHandler(self, COLLISION_SHAPE_TRIMESH, COLLISION_SHAPE_TRIMESH, intersectionHandler_trimesh_trimesh);
}

void IntersectionManager_setHandler(IntersectionManager * self, int type1, int type2, IntersectionHandler handler) {
	size_t handlerIndex;
	
	for (handlerIndex = 0; handlerIndex < self->private_ivar(handlerCount); handlerIndex++) {
		if (self->private_ivar(handlers)[handlerIndex].type1 == type1 && self->private_ivar(handlers)[handlerIndex].type2 == type2) {
			self->private_ivar(handlers)[handlerIndex].handler = handler;
			return;
		}
	}
	
	self->private_ivar(handlers) = realloc(self->private_ivar(handlers), sizeof(*self->private_ivar(handlers)) * (self->private_ivar(handlerCount) + 1));
	self->private_ivar(handlers)[self->private_ivar(handlerCount)].type1 = type1;
	self->private_ivar(handlers)[self->private_ivar(handlerCount)].type2 = type2;
	self->private_ivar(handlers)[self->private_ivar(handlerCount)].handler = handler;
	self->private_ivar(handlerCount)++;
}

IntersectionHandler IntersectionManager_getHandler(IntersectionManager * self, int type1, int type2) {
	size_t handlerIndex;
	
	for (handlerIndex = 0; handlerIndex < self->private_ivar(handlerCount); handlerIndex++) {
		if (self->private_ivar(handlers)[handlerIndex].type1 == type1 && self->private_ivar(handlers)[handlerIndex].type2 == type2) {
			return self->private_ivar(handlers)[handlerIndex].handler;
		}
	}
	return NULL;
}

bool IntersectionManager_callHandler(IntersectionManager * self, CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	IntersectionHandler handler;
	
	handler = IntersectionManager_getHandler(self, object1->shapeType, object2->shapeType);
	if (handler == NULL) {
		return false;
	}
	return handler(object1, object2, outTime, outNormal);
}
