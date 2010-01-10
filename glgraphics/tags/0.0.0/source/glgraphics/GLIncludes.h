#ifndef __GL_INCLUDES_H__
#define __GL_INCLUDES_H__

#ifdef __APPLE__
	#include <TargetConditionals.h>
	#if TARGET_OS_IPHONE
		#define TARGET_OPENGL_ES 1
		#include <OpenGLES/ES1/gl.h>
		#include <OpenGLES/ES1/glext.h>
	#elif TARGET_OS_MAC
		#define TARGET_OPENGL_ES 0
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
		#include <OpenGL/glext.h>
	#else
		#error Unknown platform (not TARGET_OS_MAC or TARGET_OS_IPHONE)
	#endif
#else
	#define TARGET_OPENGL_ES 0
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glext.h>
#endif

#endif
