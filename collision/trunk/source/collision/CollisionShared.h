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

#ifndef __CollisionShared_H__
#define __CollisionShared_H__
#ifdef __cplusplus
extern "C" {
#endif

#define COLLISION_SHAPE_RECT_2D 1
#define COLLISION_SHAPE_CIRCLE 2
#define COLLISION_SHAPE_LINE_2D 3
#define COLLISION_SHAPE_POLYGON 4
// COLLISION_SHAPE_CONCAVE_RECT_2D
// COLLISION_SHAPE_CONCAVE_CIRCLE
// COLLISION_SHAPE_CONCAVE_POLYGON

#define COLLISION_SHAPE_BOX_3D 5
#define COLLISION_SHAPE_SPHERE 6
#define COLLISION_SHAPE_LINE_3D 7
#define COLLISION_SHAPE_CYLINDER 8
#define COLLISION_SHAPE_CAPSULE 9
#define COLLISION_SHAPE_STATIC_TRIMESH 10
// COLLISION_SHAPE_CONCAVE_BOX_3D
// COLLISION_SHAPE_CONCAVE_SPHERE
// COLLISION_SHAPE_CONCAVE_CYLINDER
// COLLISION_SHAPE_CONCAVE_CAPSULE

#ifdef __cplusplus
}
#endif
#endif
