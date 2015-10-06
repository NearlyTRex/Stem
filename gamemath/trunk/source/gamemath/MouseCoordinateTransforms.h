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

#ifndef __MouseCoordinateTransforms_H__
#define __MouseCoordinateTransforms_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gamemath/Vector2f.h"

// Transforms from: coordinates measured in pixels from the top left corner, +y down
//            into: coordinates measured from the center of the viewport, +y up, in units specified by unitsToEdgeY
Vector2f transformMousePosition_signedCenter(Vector2f mousePosition, float viewportWidth, float viewportHeight, float unitsToEdgeY);

// Transforms from: coordinates measured in pixels from the top left corner, +y down
//            into: coordinates measured from the center of the viewport, +y down, in units specified by unitsToEdgeY
Vector2f transformMousePosition_signedCenterFlipped(Vector2f mousePosition, float viewportWidth, float viewportHeight, float unitsToEdgeY);

// Transforms from: coordinates measured in pixels from the top left corner, +y down
//            into: coordinates measured from the center of a centered box, +y up, in units specified by boxWidth and boxHeight
Vector2f transformMousePosition_signedCenterAspect(Vector2f mousePosition, float viewportWidth, float viewportHeight, float boxWidth, float boxHeight);

// Transforms from: coordinates measured in pixels from the top left corner, +y down
//            into: coordinates measured from the center of a centered box, +y down, in units specified by boxWidth and boxHeight
Vector2f transformMousePosition_signedCenterAspectFlipped(Vector2f mousePosition, float viewportWidth, float viewportHeight, float boxWidth, float boxHeight);

// Transforms from: coordinates measured in pixels from the top left corner, +y down
//            into: coordinates measured from the bottom left of a centered box, +y up, in units specified by boxWidth and boxHeight
Vector2f transformMousePosition_aspectBoxLowerLeft(Vector2f mousePosition, float viewportWidth, float viewportHeight, float boxWidth, float boxHeight);

// Transforms from: coordinates measured in pixels from the top left corner, +y down
//            into: coordinates measured from the top left of a centered box, +y down, in units specified by boxWidth and boxHeight
Vector2f transformMousePosition_aspectBoxUpperLeft(Vector2f mousePosition, float viewportWidth, float viewportHeight, float boxWidth, float boxHeight);

#ifdef __cplusplus
}
#endif
#endif
