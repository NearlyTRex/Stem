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

#ifndef __BouncingBallScreen_H__
#define __BouncingBallScreen_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct BouncingBallScreen BouncingBallScreen;

#include "collision/CollisionCircle.h"
#include "collision/CollisionResolver.h"
#include "collision/IntersectionManager.h"
#include "glbitmapfont/GLBitmapFont.h"
#include "resourcemanager/ResourceManager.h"
#include "screenmanager/Screen.h"
#include "utilities/FixedIntervalRunLoop.h"

struct bouncingBall {
	BouncingBallScreen * owner;
	Vector2x velocity;
	double lastCollisionTime;
	CollisionCircle * lastCollisionCircle;
	CollisionCircle circle;
};

#define BOUNCING_BALL_COUNT 20

#define BouncingBallScreen_structContents(self_type) \
	Screen_structContents(self_type) \
	\
	ResourceManager * resourceManager; \
	GLBitmapFont * font; \
	IntersectionManager * intersectionManager; \
	CollisionResolver * resolver; \
	FixedIntervalRunLoop * runLoop; \
	unsigned int timerID; \
	double lastDrawTime; \
	bool backgrounded; \
	bool paused; \
	struct bouncingBall balls[BOUNCING_BALL_COUNT]; \
	struct bouncingBall lastBalls[BOUNCING_BALL_COUNT];

stemobject_struct_definition(BouncingBallScreen)

BouncingBallScreen * BouncingBallScreen_create(ResourceManager * resourceManager);
bool BouncingBallScreen_init(BouncingBallScreen * self, ResourceManager * resourceManager);
void BouncingBallScreen_dispose(BouncingBallScreen * self);

void BouncingBallScreen_activate(BouncingBallScreen * self);
void BouncingBallScreen_deactivate(BouncingBallScreen * self);

#ifdef __cplusplus
}
#endif
#endif
