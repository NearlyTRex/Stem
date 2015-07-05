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

#include "collision/CollisionRect2D.h"
#include "collision/CollisionShared.h"
#include "gamemath/Matrix.h"
#include "gamemath/Vector2f.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "shell/Shell.h"
#include "shell/ShellKeyCodes.h"
#include "testharness/BouncingBallScreen.h"
#include "testharness/SharedEvents.h"
#include "testharness/TestHarness_globals.h"
#include "utilities/Ranrot.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define SUPERCLASS Screen
#define FRAME_INTERVAL (1.0 / 60.0)
#define GRAVITY 0x00080
#define ELASTICITY 0x0E000
#define BALL_AREA_X 0x0F0000
#define BALL_AREA_Y 0x0B0000
#define RADIUS_MIN 0x02000
#define RADIUS_MAX 0x10000
#define INITIAL_VELOCITY_MAX 0x08000
#define COLLISION_DURATION 0.25

BouncingBallScreen * BouncingBallScreen_create() {
	stemobject_create_implementation(BouncingBallScreen, init)
}

bool BouncingBallScreen_init(BouncingBallScreen * self) {
	call_super(init, self);
	self->dispose = BouncingBallScreen_dispose;
	self->activate = BouncingBallScreen_activate;
	self->deactivate = BouncingBallScreen_deactivate;
	self->intersectionManager = IntersectionManager_createWithStandardHandlers();
	self->backgrounded = false;
	self->paused = false;
	return true;
}

void BouncingBallScreen_dispose(BouncingBallScreen * self) {
	if (self->runLoop != NULL) {
		FixedIntervalRunLoop_dispose(self->runLoop);
	}
	IntersectionManager_dispose(self->intersectionManager);
	call_super(dispose, self);
}

static void stepSimulation(BouncingBallScreen * self) {
	unsigned int ballIndex;
	double currentTime = Shell_getCurrentTime();
	
	for (ballIndex = 0; ballIndex < BOUNCING_BALL_COUNT; ballIndex++) {
		if (currentTime - self->balls[ballIndex].lastCollisionTime > COLLISION_DURATION) {
			self->balls[ballIndex].lastCollisionCircle = NULL;
		}
		self->balls[ballIndex].velocity.y -= GRAVITY;
		CollisionCircle_updatePosition(&self->balls[ballIndex].circle, Vector2x_add(self->balls[ballIndex].circle.position, self->balls[ballIndex].velocity));
	}
	memcpy(self->lastBalls, self->balls, sizeof(self->balls));
	CollisionResolver_resolveAll(self->resolver);
}

static void run(void * context) {
	stepSimulation(context);
}

void ballCollision(CollisionRecord collision, fixed16_16 timesliceSize) {
	CollisionCircle * circle = (CollisionCircle *) collision.object1;
	struct bouncingBall * ball = (struct bouncingBall *) circle->owner;
	BouncingBallScreen * self = ball->owner;
	Vector2x reflectedVelocity;
	
	ball->lastCollisionTime = Shell_getCurrentTime();
	if (collision.object2->shapeType == COLLISION_SHAPE_CIRCLE) {
		ball->lastCollisionCircle = (CollisionCircle *) collision.object2;
		
		if (Vector2x_magnitude(Vector2x_subtract(circle->lastPosition, ball->lastCollisionCircle->lastPosition)) > circle->radius + ball->lastCollisionCircle->radius + 0x10000) {
			unsigned int ballIndex1, ballIndex2;
			
			printf("Suspicious circle collision!\n");
			for (ballIndex1 = 0; ballIndex1 < BOUNCING_BALL_COUNT; ballIndex1++) {
				if (&self->balls[ballIndex1].circle == circle) {
					break;
				}
			}
			for (ballIndex2 = 0; ballIndex2 < BOUNCING_BALL_COUNT; ballIndex2++) {
				if (&self->balls[ballIndex2].circle == ball->lastCollisionCircle) {
					break;
				}
			}
			if (ballIndex1 < BOUNCING_BALL_COUNT && ballIndex2 < BOUNCING_BALL_COUNT) {
				printf("  circle1: {0x%05X, 0x%05X} -> {0x%05X, 0x%05X}, 0x%05X\n", self->lastBalls[ballIndex1].circle.lastPosition.x, self->lastBalls[ballIndex1].circle.lastPosition.y, self->lastBalls[ballIndex1].circle.position.x, self->lastBalls[ballIndex1].circle.position.y, self->lastBalls[ballIndex1].circle.radius);
				printf("  circle2: {0x%05X, 0x%05X} -> {0x%05X, 0x%05X}, 0x%05X\n", self->lastBalls[ballIndex2].circle.lastPosition.x, self->lastBalls[ballIndex2].circle.lastPosition.y, self->lastBalls[ballIndex2].circle.position.x, self->lastBalls[ballIndex2].circle.position.y, self->lastBalls[ballIndex2].circle.radius);
			}
		}
	} else {
		ball->lastCollisionCircle = NULL;
	}
	reflectedVelocity = Vector2x_reflect(ball->velocity, VECTOR2x(collision.normal.x, collision.normal.y));
	ball->velocity = Vector2x_interpolate(ball->velocity, reflectedVelocity, 0x08000 + ELASTICITY / 2);
	circle->position = Vector2x_add(circle->lastPosition, Vector2x_multiplyScalar(ball->velocity, timesliceSize));
}

#define CIRCLE_TESSELATIONS 64

#define COLOR_RECT        COLOR4f(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_CIRCLE_LAST COLOR4f(0.0f, 0.25f, 0.5f, 1.0f)
#define COLOR_CIRCLE      COLOR4f(0.0f, 1.0f, 1.0f, 1.0f)

static void setVertexColor(struct vertex_p2f_c4f * vertex, Color4f color) {
	vertex->color[0] = color.red;
	vertex->color[1] = color.green;
	vertex->color[2] = color.blue;
	vertex->color[3] = color.alpha;
}

static void getCollisionObjectVertices2D(BouncingBallScreen * self, struct vertex_p2f_c4f * outVertices, GLuint * outIndexes, size_t * ioVertexCount, size_t * ioIndexCount) {
	size_t objectIndex;
	CollisionObject * object;
	struct vertex_p2f_c4f vertex;
	bool colliding;
	CollisionRecord collision;
	double currentTime = Shell_getCurrentTime();
	
	for (objectIndex = 0; objectIndex < self->resolver->objectCount; objectIndex++) {
		object = self->resolver->objects[objectIndex];
		colliding = CollisionResolver_querySingle(self->resolver, object, &collision);
		switch (object->shapeType) {
			case COLLISION_SHAPE_RECT_2D: {
				CollisionRect2D * rect = (CollisionRect2D *) object;
				unsigned int vertexIndex;
				
				if (outVertices != NULL) {
					setVertexColor(&vertex, COLOR_RECT);
					vertex.position[0] = xtof(rect->position.x);
					vertex.position[1] = xtof(rect->position.y);
					outVertices[*ioVertexCount + 0] = vertex;
					vertex.position[0] = xtof(rect->position.x + rect->size.x);
					outVertices[*ioVertexCount + 1] = vertex;
					vertex.position[1] = xtof(rect->position.y + rect->size.y);
					outVertices[*ioVertexCount + 2] = vertex;
					vertex.position[0] = xtof(rect->position.x);
					outVertices[*ioVertexCount + 3] = vertex;
				}
				
				if (outIndexes != NULL) {
					for (vertexIndex = 0; vertexIndex < 4; vertexIndex++) {
						outIndexes[*ioIndexCount + vertexIndex * 2 + 0] = *ioVertexCount + vertexIndex;
						outIndexes[*ioIndexCount + vertexIndex * 2 + 1] = *ioVertexCount + (vertexIndex + 1) % 4;
					}
				}
				
				*ioVertexCount += 8;
				*ioIndexCount += 8;
				
				break;
			}
			case COLLISION_SHAPE_CIRCLE: {
				CollisionCircle * circle = (CollisionCircle *) object;
				unsigned int tesselationIndex;
				struct bouncingBall * ball = circle->owner, * lastBall = NULL;
				unsigned int ballIndex;
				
				for (ballIndex = 0; ballIndex < BOUNCING_BALL_COUNT - 1; ballIndex++) {
					if (&self->balls[ballIndex].circle == circle) {
						break;
					}
				}
				lastBall = &self->lastBalls[ballIndex];
				
				if (outVertices != NULL) {
					setVertexColor(&vertex, COLOR_CIRCLE_LAST);
					for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
						vertex.position[0] = xtof(lastBall->circle.lastPosition.x) + xtof(lastBall->circle.radius) * cos(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
						vertex.position[1] = xtof(lastBall->circle.lastPosition.y) + xtof(lastBall->circle.radius) * sin(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
						outVertices[*ioVertexCount + tesselationIndex] = vertex;
					}
					
					if (currentTime - ball->lastCollisionTime < COLLISION_DURATION) {
						vertex.color[0] = 1.0f - (currentTime - ball->lastCollisionTime) / COLLISION_DURATION;
						vertex.color[1] = (currentTime - ball->lastCollisionTime) / COLLISION_DURATION;
						vertex.color[2] = (currentTime - ball->lastCollisionTime) / COLLISION_DURATION;
					} else {
						setVertexColor(&vertex, COLOR_CIRCLE);
					}
					for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
						vertex.position[0] = xtof(circle->position.x) + xtof(circle->radius) * cos(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
						vertex.position[1] = xtof(circle->position.y) + xtof(circle->radius) * sin(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
						outVertices[*ioVertexCount + CIRCLE_TESSELATIONS + tesselationIndex] = vertex;
					}
				}
				
				if (outIndexes != NULL) {
					float angle;
					int bestEdgeIndex;
					
					for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
						outIndexes[*ioIndexCount + tesselationIndex * 2 + 0] = *ioVertexCount + tesselationIndex;
						outIndexes[*ioIndexCount + tesselationIndex * 2 + 1] = *ioVertexCount + (tesselationIndex + 1) % CIRCLE_TESSELATIONS;
					}
					
					angle = atan2(circle->position.y - circle->lastPosition.y, circle->position.x - circle->lastPosition.x);
					bestEdgeIndex = round((angle + M_PI / 2) * CIRCLE_TESSELATIONS / (M_PI * 2));
					bestEdgeIndex = (bestEdgeIndex % CIRCLE_TESSELATIONS + CIRCLE_TESSELATIONS) % CIRCLE_TESSELATIONS;
					outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 0] = *ioVertexCount + bestEdgeIndex;
					outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 1] = *ioVertexCount + bestEdgeIndex + CIRCLE_TESSELATIONS;
					bestEdgeIndex += CIRCLE_TESSELATIONS / 2;
					bestEdgeIndex %= CIRCLE_TESSELATIONS;
					outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 2] = *ioVertexCount + bestEdgeIndex;
					outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 3] = *ioVertexCount + bestEdgeIndex + CIRCLE_TESSELATIONS;
					
					for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
						outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 4 + tesselationIndex * 2] = *ioVertexCount + CIRCLE_TESSELATIONS + tesselationIndex;
						outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 5 + tesselationIndex * 2] = *ioVertexCount + CIRCLE_TESSELATIONS + (tesselationIndex + 1) % CIRCLE_TESSELATIONS;
					}
				}
				
				*ioVertexCount += CIRCLE_TESSELATIONS * 2;
				*ioIndexCount += CIRCLE_TESSELATIONS * 4 + 4;
				
				if (ball->lastCollisionCircle != NULL) {
					if (outVertices != NULL) {
						setVertexColor(&vertex, COLOR4f(1.0f - (currentTime - ball->lastCollisionTime) / COLLISION_DURATION, 0.0f, 0.0f, 1.0f));
						vertex.position[0] = xtof(ball->circle.position.x);
						vertex.position[1] = xtof(ball->circle.position.y);
						outVertices[*ioVertexCount] = vertex;
						vertex.position[0] = xtof(ball->lastCollisionCircle->position.x);
						vertex.position[1] = xtof(ball->lastCollisionCircle->position.y);
						outVertices[*ioVertexCount + 1] = vertex;
					}
					if (outIndexes != NULL) {
						outIndexes[*ioIndexCount] = *ioVertexCount;
						outIndexes[*ioIndexCount + 1] = *ioVertexCount + 1;
					}
					*ioVertexCount += 2;
					*ioIndexCount += 2;
				}
				
				break;
			}
		}
	}
}

static bool draw(Atom eventID, void * eventData, void * context) {
	BouncingBallScreen * self = context;
	static GLuint vertexBufferID, indexBufferID;
	struct vertex_p2f_c4f * vertices;
	size_t vertexCount;
	GLuint * indexes;
	size_t indexCount;
	Matrix matrix;
	
	FixedIntervalRunLoop_run(self->runLoop);
	
	if (vertexBufferID == 0) {
		glGenBuffersARB(1, &vertexBufferID);
		glGenBuffersARB(1, &indexBufferID);
	}
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	vertexCount = indexCount = 0;
	getCollisionObjectVertices2D(self, NULL, NULL, &vertexCount, &indexCount);
	glBindBufferARB(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(struct vertex_p2f_c4f) * vertexCount, NULL, GL_STREAM_DRAW);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, NULL, GL_STREAM_DRAW);
	vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	indexes = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertexCount = indexCount = 0;
	getCollisionObjectVertices2D(self, vertices, indexes, &vertexCount, &indexCount);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	
	matrix = Matrix_ortho(MATRIX_IDENTITY, -12.0f * g_viewRatio, 12.0f * g_viewRatio, -12.0f, 12.0f, -1.0f, 1.0f);
	glLoadMatrixf(matrix.m);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_p2f_c4f), (void *) offsetof(struct vertex_p2f_c4f, position));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_p2f_c4f), (void *) offsetof(struct vertex_p2f_c4f, color));
	glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	if (!self->paused && !self->backgrounded) {
		Shell_redisplay();
	}
	return true;
}

static bool keyDown(Atom eventID, void * eventData, void * context) {
	BouncingBallScreen * self = context;
	struct keyEvent * event = eventData;
	
	switch (event->keyCode) {
		case KEYBOARD_P:
			self->paused = !self->paused;
			if (self->paused) {
				FixedIntervalRunLoop_pause(self->runLoop);
			} else {
				FixedIntervalRunLoop_resume(self->runLoop);
				Shell_redisplay();
			}
			break;
			
		case KEYBOARD_CLOSE_BRACKET:
			if (self->paused) {
				stepSimulation(self);
				Shell_redisplay();
			}
			break;
	}
	return true;
}

static bool backgrounded(Atom eventID, void * eventData, void * context) {
	BouncingBallScreen * self = context;
	
	self->backgrounded = true;
	FixedIntervalRunLoop_pause(self->runLoop);
	return true;
}

static bool foregrounded(Atom eventID, void * eventData, void * context) {
	BouncingBallScreen * self = context;
	
	self->backgrounded = false;
	if (!self->paused) {
		FixedIntervalRunLoop_resume(self->runLoop);
		Shell_redisplay();
	}
	return true;
}

static bool resized(Atom eventID, void * eventData, void * context) {
	Shell_redisplay();
	return true;
}

void BouncingBallScreen_activate(BouncingBallScreen * self) {
	unsigned int ballIndex;
	int randomSeed;
	
	self->resolver = CollisionResolver_create(self->intersectionManager, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionRect2D_create(NULL, NULL, VECTOR2x(BALL_AREA_X, BALL_AREA_Y), VECTOR2x(BALL_AREA_X * -2, BALL_AREA_Y * -2)));
	
	if (g_fixedRandomSeed) {
		randomSeed = g_randomSeed;
	} else {
		randomSeed = time(NULL);
		printf("Random seed: %d\n", randomSeed);
	}
	sdrand(randomSeed);
	
	for (ballIndex = 0; ballIndex < BOUNCING_BALL_COUNT; ballIndex++) {
		self->balls[ballIndex].owner = self;
		self->balls[ballIndex].velocity.x = irand() % INITIAL_VELOCITY_MAX;
		self->balls[ballIndex].velocity.y = irand() % INITIAL_VELOCITY_MAX;
		self->balls[ballIndex].lastCollisionTime = 0.0;
		self->balls[ballIndex].lastCollisionCircle = NULL;
		CollisionCircle_init(&self->balls[ballIndex].circle, &self->balls[ballIndex], ballCollision, VECTOR2x(irand() % (BALL_AREA_X - RADIUS_MAX), irand() % (BALL_AREA_Y - RADIUS_MAX)), RADIUS_MIN + uirand() % (RADIUS_MAX - RADIUS_MIN));
		CollisionCircle_updatePosition(&self->balls[ballIndex].circle, Vector2x_add(self->balls[ballIndex].circle.position, self->balls[ballIndex].velocity));
		CollisionResolver_addObject(self->resolver, (CollisionObject *) &self->balls[ballIndex].circle);
	}
	memcpy(self->lastBalls, self->balls, sizeof(self->balls));
	CollisionResolver_resolveAll(self->resolver);
	
	self->runLoop = FixedIntervalRunLoop_create(Shell_getCurrentTime, FRAME_INTERVAL, run, self);
	FixedIntervalRunLoop_setTolerance(self->runLoop, FRAME_INTERVAL * 0.375f);
	if (self->paused) {
		FixedIntervalRunLoop_pause(self->runLoop);
	}
	
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_BACKGROUNDED), backgrounded, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_FOREGROUNDED), foregrounded, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
	
	Shell_redisplay();
}

void BouncingBallScreen_deactivate(BouncingBallScreen * self) {
	self->resolver->objects[0]->dispose(self->resolver->objects[0]);
	CollisionResolver_dispose(self->resolver);
	FixedIntervalRunLoop_dispose(self->runLoop);
	self->runLoop = NULL;
	
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_BACKGROUNDED), backgrounded, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_FOREGROUNDED), foregrounded, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
}
