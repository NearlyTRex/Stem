/*
  Copyright (c) 2014 Alex Diener
  
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

#ifndef __VertexTypes_H__
#define __VertexTypes_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "glgraphics/GLIncludes.h"
#include <stdint.h>

// TODO: This is incomplete; is it even useful for anything?
#define VERTEX_MASK_P2F (1 << 0)
#define VERTEX_MASK_P3F (1 << 1)
#define VERTEX_MASK_P4F (1 << 0 | 1 << 1)
#define VERTEX_MASK_T2F (1 << 2)
#define VERTEX_MASK_T3F (1 << 3)
#define VERTEX_MASK_T4F (1 << 2 | 1 << 3)
#define VERTEX_MASK_N3F (1 << 4)
#define VERTEX_MASK_C3F (1 << 5)
#define VERTEX_MASK_C4F (1 << 5 | 1 << 6)

#define VERTEX_TYPE_P2F             (VERTEX_MASK_P2F)
#define VERTEX_TYPE_P2F_T2F         (VERTEX_MASK_P2F | VERTEX_MASK_T2F)
#define VERTEX_TYPE_P2F_C4F         (VERTEX_MASK_P2F | VERTEX_MASK_C4F)
#define VERTEX_TYPE_P2F_T2F_C4F     (VERTEX_MASK_P2F | VERTEX_MASK_T2F | VERTEX_MASK_C4F)
#define VERTEX_TYPE_P3F             (VERTEX_MASK_P3F)
#define VERTEX_TYPE_P3F_T2F         (VERTEX_MASK_P3F | VERTEX_MASK_T2F)
#define VERTEX_TYPE_P3F_C4F         (VERTEX_MASK_P3F | VERTEX_MASK_C4F)
#define VERTEX_TYPE_P3F_T2F_C4F     (VERTEX_MASK_P3F | VERTEX_MASK_T2F | VERTEX_MASK_C4F)
#define VERTEX_TYPE_P3F_N3F         (VERTEX_MASK_P3F | VERTEX_MASK_N3F)
#define VERTEX_TYPE_P3F_N3F_C4F     (VERTEX_MASK_P3F | VERTEX_MASK_N3F | VERTEX_MASK_C4F)
#define VERTEX_TYPE_P3F_T2F_N3F     (VERTEX_MASK_P3F | VERTEX_MASK_T2F | VERTEX_MASK_N3F)
#define VERTEX_TYPE_P3F_T2F_N3F_C4F (VERTEX_MASK_P3F | VERTEX_MASK_T2F | VERTEX_MASK_N3F | VERTEX_MASK_C4F)

// Vertices can be interchangeably represented as arrays of multiples of the appropriate
// number of floats for the type, or as one of the structures defined below.

#pragma pack(push, 4)

struct vertex_p2f {
	GLfloat position[2];
};

struct vertex_p2f_t2f {
	GLfloat position[2];
	GLfloat texCoords[2];
};

struct vertex_p2f_c4f {
	GLfloat position[2];
	GLfloat color[4];
};

struct vertex_p2f_t2f_c4f {
	GLfloat position[2];
	GLfloat texCoords[2];
	GLfloat color[4];
};

struct vertex_p3f {
	GLfloat position[3];
};

struct vertex_p3f_t2f {
	GLfloat position[3];
	GLfloat texCoords[2];
};

struct vertex_p3f_c4f {
	GLfloat position[3];
	GLfloat color[4];
};

struct vertex_p3f_t2f_c4f {
	GLfloat position[3];
	GLfloat texCoords[2];
	GLfloat color[4];
};

struct vertex_p3f_n3f {
	GLfloat position[3];
	GLfloat normal[3];
};

struct vertex_p3f_n3f_c4f {
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat color[4];
};

struct vertex_p3f_t2f_n3f {
	GLfloat position[3];
	GLfloat texCoords[2];
	GLfloat normal[3];
};

struct vertex_p3f_t2f_n3f_c4f {
	GLfloat position[3];
	GLfloat texCoords[2];
	GLfloat normal[3];
	GLfloat color[4];
};

// TODO: This doesn't conform to everything above for other vertex types
struct vertex_p3f_t2f_n3f_c4f_b4u_w4f {
	GLfloat position[3];
	GLfloat texCoords[2];
	GLfloat normal[3];
	GLfloat color[4];
	GLuint boneIndexes[4];
	GLfloat boneWeights[4];
};

typedef struct Color4f {
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;
} Color4f;

#pragma pack(pop)

#define COLOR4f(red, green, blue, alpha) ((Color4f) {red, green, blue, alpha})

static inline Color4f Color4f_fromRGBA8888(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
	return (Color4f) {red / (float) 0xFF, green / (float) 0xFF, blue / (float) 0xFF, alpha / (float) 0xFF};
}

static inline Color4f Color4f_fromRGBA32(uint32_t rgba) {
	return (Color4f) {(rgba >> 24) / (float) 0xFF, (rgba >> 16 & 0xFF) / (float) 0xFF, (rgba >> 8 & 0xFF) / (float) 0xFF, (rgba & 0xFF) / (float) 0xFF};
}

#ifdef __cplusplus
}
#endif
#endif
