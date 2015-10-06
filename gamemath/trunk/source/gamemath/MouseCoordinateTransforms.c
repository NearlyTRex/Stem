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

#include "gamemath/MouseCoordinateTransforms.h"

Vector2f transformMousePosition_signedCenter(Vector2f mousePosition, float viewportWidth, float viewportHeight, float unitsToEdgeY) {
	Vector2f result;
	
	result.x = (mousePosition.x - viewportWidth * 0.5f) / viewportWidth * 2.0f * unitsToEdgeY * viewportWidth / viewportHeight;
	result.y = (viewportHeight * 0.5f - mousePosition.y) / viewportHeight * 2.0f * unitsToEdgeY;
	return result;
}

Vector2f transformMousePosition_signedCenterFlipped(Vector2f mousePosition, float viewportWidth, float viewportHeight, float unitsToEdgeY) {
	Vector2f result;
	
	result.x = (mousePosition.x - viewportWidth * 0.5f) / viewportWidth * 2.0f * unitsToEdgeY * viewportWidth / viewportHeight;
	result.y = (mousePosition.y - viewportHeight * 0.5f) / viewportHeight * 2.0f * unitsToEdgeY;
	return result;
}

Vector2f transformMousePosition_signedCenterAspect(Vector2f mousePosition, float viewportWidth, float viewportHeight, float boxWidth, float boxHeight) {
	Vector2f result;
	float viewportRatio = viewportWidth / viewportHeight;
	float boxRatio = boxWidth / boxHeight;
	
	result.x = mousePosition.x - viewportWidth * 0.5f;
	result.y = -(mousePosition.y - viewportHeight * 0.5f);
	if (viewportRatio > boxRatio) {
		result.x /= viewportHeight;
		result.y /= viewportHeight;
		result.x *= boxHeight;
		result.y *= boxHeight;
		
	} else {
		result.x /= viewportWidth;
		result.y /= viewportWidth;
		result.x *= boxWidth;
		result.y *= boxWidth;
	}
	return result;
}

Vector2f transformMousePosition_signedCenterAspectFlipped(Vector2f mousePosition, float viewportWidth, float viewportHeight, float boxWidth, float boxHeight) {
	Vector2f result;
	float viewportRatio = viewportWidth / viewportHeight;
	float boxRatio = boxWidth / boxHeight;
	
	result.x = mousePosition.x - viewportWidth * 0.5f;
	result.y = mousePosition.y - viewportHeight * 0.5f;
	if (viewportRatio > boxRatio) {
		result.x /= viewportHeight;
		result.y /= viewportHeight;
		result.x *= boxHeight;
		result.y *= boxHeight;
		
	} else {
		result.x /= viewportWidth;
		result.y /= viewportWidth;
		result.x *= boxWidth;
		result.y *= boxWidth;
	}
	return result;
}

Vector2f transformMousePosition_aspectBoxLowerLeft(Vector2f mousePosition, float viewportWidth, float viewportHeight, float boxWidth, float boxHeight) {
	Vector2f result;
	float viewportRatio = viewportWidth / viewportHeight;
	float boxRatio = boxWidth / boxHeight;
	
	result.x = mousePosition.x;
	result.y = viewportHeight - mousePosition.y;
	if (viewportRatio > boxRatio) {
		result.x -= (viewportWidth - viewportHeight * boxRatio) * 0.5f;
		result.x /= viewportHeight;
		result.y /= viewportHeight;
		result.x *= boxHeight;
		result.y *= boxHeight;
		
	} else {
		result.y -= (viewportHeight - viewportWidth / boxRatio) * 0.5f;
		result.x /= viewportWidth;
		result.y /= viewportWidth;
		result.x *= boxWidth;
		result.y *= boxWidth;
	}
	return result;
}

Vector2f transformMousePosition_aspectBoxUpperLeft(Vector2f mousePosition, float viewportWidth, float viewportHeight, float boxWidth, float boxHeight) {
	Vector2f result;
	float viewportRatio = viewportWidth / viewportHeight;
	float boxRatio = boxWidth / boxHeight;
	
	result.x = mousePosition.x;
	result.y = mousePosition.y;
	if (viewportRatio > boxRatio) {
		result.x -= (viewportWidth - viewportHeight * boxRatio) * 0.5f;
		result.x /= viewportHeight;
		result.x *= boxHeight;
		result.y /= viewportHeight;
		result.y *= boxHeight;
		
	} else {
		result.y -= (viewportHeight - viewportWidth / boxRatio) * 0.5f;
		result.x /= viewportWidth;
		result.x *= boxWidth;
		result.y /= viewportWidth;
		result.y *= boxWidth;
	}
	return result;
}
