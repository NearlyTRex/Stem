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

#ifndef __StandardIntersectionHandlers_H__
#define __StandardIntersectionHandlers_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "collision/CollisionResolver.h"

#define COLLISION_SHAPE_RECT 1
#define COLLISION_SHAPE_CIRCLE 2
#define COLLISION_SHAPE_LINE_2D 3
#define COLLISION_SHAPE_POLYGON 4

#define COLLISION_SHAPE_BOX 5
#define COLLISION_SHAPE_SPHERE 6
#define COLLISION_SHAPE_LINE_3D 7
#define COLLISION_SHAPE_CYLINDER 8
#define COLLISION_SHAPE_CAPSULE 9
#define COLLISION_SHAPE_TRIMESH 10

// 2D handlers
bool intersectionHandler_rect_rect(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_rect_circle(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_rect_line2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_rect_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_circle_circle(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_circle_line2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_circle_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_line2D_line2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_line2D_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_polygon_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);

// 3D handlers
bool intersectionHandler_box_box(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_box_sphere(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_box_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_box_cylinder(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_box_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_box_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_sphere_sphere(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_sphere_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_sphere_cylinder(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_sphere_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_sphere_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_line3D_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_line3D_cylinder(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_line3D_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_line3D_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_cylinder_cylinder(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_cylinder_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_cylinder_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_capsule_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_capsule_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);
bool intersectionHandler_trimesh_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal);

#ifdef __cplusplus
}
#endif
#endif
