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

#include "collision/CollisionCircle.h"
#include "collision/CollisionRect2D.h"
#include "collision/CollisionShared.h"
#include "gamemath/Matrix.h"
#include "gamemath/Vector2f.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "shell/Shell.h"
#include "shell/ShellKeyCodes.h"
#include "testharness/SingleFrameScreen.h"
#include "testharness/SharedEvents.h"
#include "testharness/TestHarness_globals.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#define SUPERCLASS Screen

SingleFrameScreen * SingleFrameScreen_create() {
	stemobject_create_implementation(SingleFrameScreen, init)
}

bool SingleFrameScreen_init(SingleFrameScreen * self) {
	call_super(init, self);
	self->dispose = SingleFrameScreen_dispose;
	self->activate = SingleFrameScreen_activate;
	self->deactivate = SingleFrameScreen_deactivate;
	self->intersectionManager = IntersectionManager_createWithStandardHandlers();
	return true;
}

void SingleFrameScreen_dispose(SingleFrameScreen * self) {
	IntersectionManager_dispose(self->intersectionManager);
	call_super(dispose, self);
}

#define ARROW_RADIUS 0.375f

static void getArrowVertices2D(Vector2x position, Vector3x normal, struct vertex_p2f_c4f * outVertices, GLuint * outIndexes, size_t * ioVertexCount, size_t * ioIndexCount) {
	if (outVertices != NULL) {
		struct vertex_p2f_c4f vertex;
		Vector2f positionf = VECTOR2f(xtof(position.x), xtof(position.y));
		Vector2f normalf = VECTOR2f(xtof(normal.x), xtof(normal.y));
		
		vertex.color[0] = 1.0f;
		vertex.color[1] = 0.0f;
		vertex.color[2] = 0.0f;
		vertex.color[3] = 1.0f;
		vertex.position[0] = positionf.x - normalf.x * ARROW_RADIUS;
		vertex.position[1] = positionf.y - normalf.y * ARROW_RADIUS;
		outVertices[*ioVertexCount + 0] = vertex;
		vertex.position[0] = positionf.x + normalf.x * ARROW_RADIUS;
		vertex.position[1] = positionf.y + normalf.y * ARROW_RADIUS;
		outVertices[*ioVertexCount + 1] = vertex;
		vertex.position[0] = positionf.x + normalf.y * ARROW_RADIUS * 0.5f;
		vertex.position[1] = positionf.y - normalf.x * ARROW_RADIUS * 0.5f;
		outVertices[*ioVertexCount + 2] = vertex;
		vertex.position[0] = positionf.x - normalf.y * ARROW_RADIUS * 0.5f;
		vertex.position[1] = positionf.y + normalf.x * ARROW_RADIUS * 0.5f;
		outVertices[*ioVertexCount + 3] = vertex;
	}
	
	if (outIndexes != NULL) {
		outIndexes[*ioIndexCount + 0] = *ioVertexCount + 0;
		outIndexes[*ioIndexCount + 1] = *ioVertexCount + 1;
		outIndexes[*ioIndexCount + 2] = *ioVertexCount + 1;
		outIndexes[*ioIndexCount + 3] = *ioVertexCount + 2;
		outIndexes[*ioIndexCount + 4] = *ioVertexCount + 1;
		outIndexes[*ioIndexCount + 5] = *ioVertexCount + 3;
	}
	
	*ioVertexCount += 4;
	*ioIndexCount += 6;
}

#define CIRCLE_TESSELATIONS 64

#define COLOR_RECT_LAST_POSITION                COLOR4f(0.5f, 0.25f, 0.0f, 1.0f)
#define COLOR_RECT_LAST_POSITION_HIGHLIGHT      COLOR4f(0.75f, 0.625f, 0.125f, 1.0f)
#define COLOR_RECT_POSITION                     COLOR4f(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_RECT_POSITION_HIGHLIGHT           COLOR4f(1.0f, 1.0f, 0.875f, 1.0f)
#define COLOR_RECT_POSITION_COLLIDING           COLOR4f(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_RECT_POSITION_COLLIDING_HIGHLIGHT COLOR4f(1.0f, 0.75f, 0.75f, 1.0f)

#define COLOR_CIRCLE_LAST_POSITION                COLOR4f(0.0f, 0.25f, 0.5f, 1.0f)
#define COLOR_CIRCLE_LAST_POSITION_HIGHLIGHT      COLOR4f(0.125f, 0.625f, 0.75f, 1.0f)
#define COLOR_CIRCLE_POSITION                     COLOR4f(0.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_CIRCLE_POSITION_HIGHLIGHT           COLOR4f(0.875f, 1.0f, 1.0f, 1.0f)
#define COLOR_CIRCLE_POSITION_COLLIDING           COLOR4f(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_CIRCLE_POSITION_COLLIDING_HIGHLIGHT COLOR4f(1.0f, 0.75f, 0.75f, 1.0f)

static void setVertexColor(struct vertex_p2f_c4f * vertex, Color4f color) {
	vertex->color[0] = color.red;
	vertex->color[1] = color.green;
	vertex->color[2] = color.blue;
	vertex->color[3] = color.alpha;
}

static void getCollisionObjectVertices2D(SingleFrameScreen * self, struct vertex_p2f_c4f * outVertices, GLuint * outIndexes, size_t * ioVertexCount, size_t * ioIndexCount) {
	size_t objectIndex;
	CollisionObject * object;
	struct vertex_p2f_c4f vertex;
	bool colliding;
	CollisionRecord collision;
	
	for (objectIndex = 0; objectIndex < self->resolver->objectCount; objectIndex++) {
		object = self->resolver->objects[objectIndex];
		colliding = CollisionResolver_querySingle(self->resolver, object, &collision);
		switch (object->shapeType) {
			case COLLISION_SHAPE_RECT_2D: {
				CollisionRect2D * rect = (CollisionRect2D *) object;
				unsigned int vertexIndex;
				
				if (outVertices != NULL) {
					if (objectIndex == self->selectedObjectIndex) {
						setVertexColor(&vertex, COLOR_RECT_LAST_POSITION_HIGHLIGHT);
					} else {
						setVertexColor(&vertex, COLOR_RECT_LAST_POSITION);
					}
					vertex.position[0] = xtof(rect->lastPosition.x);
					vertex.position[1] = xtof(rect->lastPosition.y);
					outVertices[*ioVertexCount + 0] = vertex;
					vertex.position[0] = xtof(rect->lastPosition.x + rect->lastSize.x);
					outVertices[*ioVertexCount + 1] = vertex;
					vertex.position[1] = xtof(rect->lastPosition.y + rect->lastSize.y);
					outVertices[*ioVertexCount + 2] = vertex;
					vertex.position[0] = xtof(rect->lastPosition.x);
					outVertices[*ioVertexCount + 3] = vertex;
					
					if (colliding) {
						if (objectIndex == self->selectedObjectIndex) {
							setVertexColor(&vertex, COLOR_RECT_POSITION_COLLIDING_HIGHLIGHT);
						} else {
							setVertexColor(&vertex, COLOR_RECT_POSITION_COLLIDING);
						}
					} else {
						if (objectIndex == self->selectedObjectIndex) {
							setVertexColor(&vertex, COLOR_RECT_POSITION_HIGHLIGHT);
						} else {
							setVertexColor(&vertex, COLOR_RECT_POSITION);
						}
					}
					vertex.position[0] = xtof(rect->position.x);
					vertex.position[1] = xtof(rect->position.y);
					outVertices[*ioVertexCount + 4] = vertex;
					vertex.position[0] = xtof(rect->position.x + rect->size.x);
					outVertices[*ioVertexCount + 5] = vertex;
					vertex.position[1] = xtof(rect->position.y + rect->size.y);
					outVertices[*ioVertexCount + 6] = vertex;
					vertex.position[0] = xtof(rect->position.x);
					outVertices[*ioVertexCount + 7] = vertex;
				}
				
				if (outIndexes != NULL) {
					bool solid[4] = {rect->solidBottom, rect->solidRight, rect->solidTop, rect->solidLeft};
					
					for (vertexIndex = 0; vertexIndex < 4; vertexIndex++) {
						if (solid[vertexIndex]) {
							outIndexes[(*ioIndexCount)++] = *ioVertexCount + vertexIndex;
							outIndexes[(*ioIndexCount)++] = *ioVertexCount + (vertexIndex + 1) % 4;
						}
					}
					for (vertexIndex = 0; vertexIndex < 4; vertexIndex++) {
						outIndexes[(*ioIndexCount)++] = *ioVertexCount + 0 + vertexIndex;
						outIndexes[(*ioIndexCount)++] = *ioVertexCount + 4 + vertexIndex;
					}
					for (vertexIndex = 0; vertexIndex < 4; vertexIndex++) {
						if (solid[vertexIndex]) {
							outIndexes[(*ioIndexCount)++] = *ioVertexCount + 4 + vertexIndex;
							outIndexes[(*ioIndexCount)++] = *ioVertexCount + 4 + (vertexIndex + 1) % 4;
						}
					}
					
				} else {
					*ioIndexCount += 8 + 4 * rect->solidLeft + 4 * rect->solidRight + 4 * rect->solidBottom + 4 * rect->solidTop;
				}
				*ioVertexCount += 8;
				
				if (colliding) {
					Vector2x collidingPosition = Vector2x_interpolate(rect->lastPosition, rect->position, collision.time);
					Vector2x collidingSize = Vector2x_interpolate(rect->lastSize, rect->size, collision.time);
					
					if (outVertices != NULL) {
						if (objectIndex == self->selectedObjectIndex) {
							setVertexColor(&vertex, COLOR_RECT_POSITION_HIGHLIGHT);
						} else {
							setVertexColor(&vertex, COLOR_RECT_POSITION);
						}
						vertex.position[0] = xtof(collidingPosition.x);
						vertex.position[1] = xtof(collidingPosition.y);
						outVertices[*ioVertexCount + 0] = vertex;
						vertex.position[0] = xtof(collidingPosition.x + collidingSize.x);
						outVertices[*ioVertexCount + 1] = vertex;
						vertex.position[1] = xtof(collidingPosition.y + collidingSize.y);
						outVertices[*ioVertexCount + 2] = vertex;
						vertex.position[0] = xtof(collidingPosition.x);
						outVertices[*ioVertexCount + 3] = vertex;
					}
					if (outIndexes != NULL) {
						bool solid[4] = {rect->solidBottom, rect->solidRight, rect->solidTop, rect->solidLeft};
						
						for (vertexIndex = 0; vertexIndex < 4; vertexIndex++) {
							if (solid[vertexIndex]) {
								outIndexes[(*ioIndexCount)++] = *ioVertexCount + vertexIndex;
								outIndexes[(*ioIndexCount)++] = *ioVertexCount + (vertexIndex + 1) % 4;
							}
						}
						
					} else {
						*ioIndexCount += 2 * rect->solidLeft + 2 * rect->solidRight + 2 * rect->solidBottom + 2 * rect->solidTop;
					}
					*ioVertexCount += 4;
					
					getArrowVertices2D(VECTOR2x(collidingPosition.x + collidingSize.x / 2, collidingPosition.y + collidingSize.y / 2), collision.normal, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				}
				break;
			}
			case COLLISION_SHAPE_CIRCLE: {
				CollisionCircle * circle = (CollisionCircle *) object;
				unsigned int tesselationIndex;
				
				if (outVertices != NULL) {
					if (objectIndex == self->selectedObjectIndex) {
						setVertexColor(&vertex, COLOR_CIRCLE_LAST_POSITION_HIGHLIGHT);
					} else {
						setVertexColor(&vertex, COLOR_CIRCLE_LAST_POSITION);
					}
					for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
						vertex.position[0] = xtof(circle->lastPosition.x) + xtof(circle->radius) * cos(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
						vertex.position[1] = xtof(circle->lastPosition.y) + xtof(circle->radius) * sin(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
						outVertices[*ioVertexCount + tesselationIndex] = vertex;
					}
					
					if (colliding) {
						if (objectIndex == self->selectedObjectIndex) {
							setVertexColor(&vertex, COLOR_CIRCLE_POSITION_COLLIDING_HIGHLIGHT);
						} else {
							setVertexColor(&vertex, COLOR_CIRCLE_POSITION_COLLIDING);
						}
					} else {
						if (objectIndex == self->selectedObjectIndex) {
							setVertexColor(&vertex, COLOR_CIRCLE_POSITION_HIGHLIGHT);
						} else {
							setVertexColor(&vertex, COLOR_CIRCLE_POSITION);
						}
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
				
				if (colliding) {
					Vector2x collidingPosition = Vector2x_interpolate(circle->lastPosition, circle->position, collision.time);
					
					if (outVertices != NULL) {
						if (objectIndex == self->selectedObjectIndex) {
							setVertexColor(&vertex, COLOR_CIRCLE_POSITION_HIGHLIGHT);
						} else {
							setVertexColor(&vertex, COLOR_CIRCLE_POSITION);
						}
						for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
							vertex.position[0] = xtof(collidingPosition.x) + xtof(circle->radius) * cos(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
							vertex.position[1] = xtof(collidingPosition.y) + xtof(circle->radius) * sin(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
							outVertices[*ioVertexCount + tesselationIndex] = vertex;
						}
					}
					
					if (outIndexes != NULL) {
						for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
							outIndexes[*ioIndexCount + tesselationIndex * 2 + 0] = *ioVertexCount + tesselationIndex;
							outIndexes[*ioIndexCount + tesselationIndex * 2 + 1] = *ioVertexCount + (tesselationIndex + 1) % CIRCLE_TESSELATIONS;
						}
					}
					
					*ioVertexCount += CIRCLE_TESSELATIONS;
					*ioIndexCount += CIRCLE_TESSELATIONS * 2;
					
					getArrowVertices2D(collidingPosition, collision.normal, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				}
				break;
			}
		}
	}
}

static bool draw(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen * self = context;
	static GLuint vertexBufferID, indexBufferID;
	struct vertex_p2f_c4f * vertices;
	size_t vertexCount;
	GLuint * indexes;
	size_t indexCount;
	Matrix matrix;
	
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
	
	return true;
}

static bool keyDown(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen * self = context;
	struct keyEvent * event = eventData;
	
	switch (event->keyCode) {
		case KEYBOARD_TAB:
			if (!self->dragging) {
				if (event->modifiers & MODIFIER_SHIFT_BIT) {
					self->selectedObjectIndex += self->resolver->objectCount - 1;
				} else {
					self->selectedObjectIndex++;
				}
				self->selectedObjectIndex %= self->resolver->objectCount;
				Shell_redisplay();
			}
			break;
			
		case KEYBOARD_I:
		case KEYBOARD_J:
		case KEYBOARD_K:
		case KEYBOARD_L:
			if (self->resolver->objects[self->selectedObjectIndex]->shapeType == COLLISION_SHAPE_RECT_2D) {
				CollisionRect2D * rect = (CollisionRect2D *) self->resolver->objects[self->selectedObjectIndex];
				CollisionRect2D_setSolidity(rect,
				                            event->keyCode == KEYBOARD_J ? !rect->solidLeft   : rect->solidLeft,
				                            event->keyCode == KEYBOARD_L ? !rect->solidRight  : rect->solidRight,
				                            event->keyCode == KEYBOARD_K ? !rect->solidBottom : rect->solidBottom,
				                            event->keyCode == KEYBOARD_I ? !rect->solidTop    : rect->solidTop);
				Shell_redisplay();
			}
			break;
	}
	return false;
}

static Vector2x transformMousePosition(float x, float y) {
	Vector2x result;
	
	result.x = ftox((x - g_viewWidth / 2) / g_viewWidth * 2 * 12 * g_viewRatio);
	result.y = ftox((g_viewHeight / 2 - y) / g_viewHeight * 2 * 12);
	return result;
}

#define DOUBLE_CLICK_INTERVAL 0.25
#define DOUBLE_CLICK_MAX_DISTANCE 4.0f

static bool mouseDown(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen * self = context;
	struct mouseEvent * event = eventData;
	static double lastClickTime;
	static Vector2f lastClickPosition;
	CollisionObject * object = self->resolver->objects[self->selectedObjectIndex];
	double clickTime = Shell_getCurrentTime();
	
	if (clickTime - lastClickTime < DOUBLE_CLICK_INTERVAL && fabs(event->position.x - lastClickPosition.x) <= DOUBLE_CLICK_MAX_DISTANCE && fabs(event->position.y - lastClickPosition.y) <= DOUBLE_CLICK_MAX_DISTANCE) {
		switch (object->shapeType) {
			case COLLISION_SHAPE_RECT_2D: {
				CollisionRect2D * rect = (CollisionRect2D *) object;
				rect->position = rect->lastPosition;
				rect->size = rect->lastSize;
				break;
			}
			case COLLISION_SHAPE_CIRCLE: {
				CollisionCircle * circle = (CollisionCircle *) object;
				circle->position = circle->lastPosition;
				break;
			}
		}
		Shell_redisplay();
		self->dragging = false;
		
	} else {
		self->dragOrigin = transformMousePosition(event->position.x, event->position.y);
		self->draggingLastPosition = shiftKeyDown;
		self->draggingBoth = controlKeyDown;
		self->draggingSize = altKeyDown;
		switch (object->shapeType) {
			case COLLISION_SHAPE_RECT_2D: {
				CollisionRect2D * rect = (CollisionRect2D *) object;
				self->dragStartPosition = rect->position;
				self->dragStartLastPosition = rect->lastPosition;
				self->dragStartSize = rect->size;
				self->dragStartLastSize = rect->lastSize;
				break;
			}
			case COLLISION_SHAPE_CIRCLE: {
				CollisionCircle * circle = (CollisionCircle *) object;
				self->dragStartPosition = circle->position;
				self->dragStartLastPosition = circle->lastPosition;
				self->dragStartRadius = circle->radius;
				break;
			}
		}
		self->dragging = true;
	}
	
	lastClickTime = clickTime;
	lastClickPosition = VECTOR2f(event->position.x, event->position.y);
	
	return true;
}

static bool mouseUp(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen * self = context;
	
	self->dragging = false;
	return true;
}

static bool mouseDragged(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen * self = context;
	struct mouseEvent * event = eventData;
	Vector2x mousePosition = transformMousePosition(event->position.x, event->position.y);
	CollisionObject * object = self->resolver->objects[self->selectedObjectIndex];
	
	switch (object->shapeType) {
		case COLLISION_SHAPE_RECT_2D: {
			CollisionRect2D * rect = (CollisionRect2D *) object;
			if (self->draggingLastPosition || self->draggingBoth) {
				if (self->draggingSize) {
					rect->lastSize.x = self->dragStartLastSize.x + mousePosition.x - self->dragOrigin.x;
					rect->lastSize.y = self->dragStartLastSize.y + mousePosition.y - self->dragOrigin.y;
				} else {
					rect->lastPosition.x = self->dragStartLastPosition.x + mousePosition.x - self->dragOrigin.x;
					rect->lastPosition.y = self->dragStartLastPosition.y + mousePosition.y - self->dragOrigin.y;
				}
			}
			if (!self->draggingLastPosition || self->draggingBoth) {
				if (self->draggingSize) {
					rect->size.x = self->dragStartSize.x + mousePosition.x - self->dragOrigin.x;
					rect->size.y = self->dragStartSize.y + mousePosition.y - self->dragOrigin.y;
				} else {
					rect->position.x = self->dragStartPosition.x + mousePosition.x - self->dragOrigin.x;
					rect->position.y = self->dragStartPosition.y + mousePosition.y - self->dragOrigin.y;
				}
			}
			break;
		}
		case COLLISION_SHAPE_CIRCLE: {
			CollisionCircle * circle = (CollisionCircle *) object;
			if (self->draggingSize) {
				circle->radius = self->dragStartRadius + mousePosition.y - self->dragOrigin.y;
				
			} else {
				if (self->draggingLastPosition || self->draggingBoth) {
					circle->lastPosition.x = self->dragStartLastPosition.x + mousePosition.x - self->dragOrigin.x;
					circle->lastPosition.y = self->dragStartLastPosition.y + mousePosition.y - self->dragOrigin.y;
				}
				if (!self->draggingLastPosition || self->draggingBoth) {
					circle->position.x = self->dragStartPosition.x + mousePosition.x - self->dragOrigin.x;
					circle->position.y = self->dragStartPosition.y + mousePosition.y - self->dragOrigin.y;
				}
			}
			break;
		}
	}
	Shell_redisplay();
	
	return true;
}

static bool resized(Atom eventID, void * eventData, void * context) {
	Shell_redisplay();
	return true;
}

void SingleFrameScreen_activate(SingleFrameScreen * self) {
	self->resolver = CollisionResolver_create(self->intersectionManager, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionRect2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000)));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionRect2D_create(NULL, NULL, VECTOR2x(0x20000, -0x40000), VECTOR2x(0x50000, 0x20000)));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionCircle_create(NULL, NULL, VECTOR2x(-0x30000, 0x00000), 0x10000));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionCircle_create(NULL, NULL, VECTOR2x(-0x20000, 0x50000), 0x08000));
	
	self->dragging = false;
	self->draggingLastPosition = false;
	self->draggingBoth = false;
	self->draggingSize = false;
	self->selectedObjectIndex = 0;
	
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DOWN), mouseDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_UP), mouseUp, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DRAGGED), mouseDragged, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
}

void SingleFrameScreen_deactivate(SingleFrameScreen * self) {
	size_t objectIndex;
	
	for (objectIndex = 0; objectIndex < self->resolver->objectCount; objectIndex++) {
		self->resolver->objects[objectIndex]->dispose(self->resolver->objects[objectIndex]);
	}
	CollisionResolver_dispose(self->resolver);
	
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DOWN), mouseDown, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_UP), mouseUp, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DRAGGED), mouseDragged, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
}
