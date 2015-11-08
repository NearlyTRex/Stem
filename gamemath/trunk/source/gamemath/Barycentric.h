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

#ifndef __Barycentric_H__
#define __Barycentric_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gamemath/Vector2f.h"
#include "gamemath/Vector2x.h"
#include "gamemath/Vector3f.h"
#include "gamemath/Vector3x.h"

Vector3f cartesianToBarycentric2f(Vector2f position, Vector2f vertex0, Vector2f vertex1, Vector2f vertex2);
Vector3f cartesianToBarycentric3f(Vector3f position, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2);
Vector3x cartesianToBarycentric2x(Vector2x position, Vector2x vertex0, Vector2x vertex1, Vector2x vertex2);
Vector3x cartesianToBarycentric3x(Vector3x position, Vector3x vertex0, Vector3x vertex1, Vector3x vertex2);
Vector2f barycentricToCartesian2f(Vector3f position, Vector2f vertex0, Vector2f vertex1, Vector2f vertex2);
Vector3f barycentricToCartesian3f(Vector3f position, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2);
Vector2x barycentricToCartesian2x(Vector3x position, Vector2x vertex0, Vector2x vertex1, Vector2x vertex2);
Vector3x barycentricToCartesian3x(Vector3x position, Vector3x vertex0, Vector3x vertex1, Vector3x vertex2);

#ifdef __cplusplus
}
#endif
#endif
