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

#pragma pack(pop)

#ifdef __cplusplus
}
#endif
#endif
