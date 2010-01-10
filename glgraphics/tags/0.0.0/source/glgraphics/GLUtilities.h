#ifndef __GL_UTILITIES_H__
#define __GL_UTILITIES_H__

#include <stdbool.h>

#include "glgraphics/GLIncludes.h"

bool stringToGLConstant(const char * string, GLenum * outResult);

#endif
