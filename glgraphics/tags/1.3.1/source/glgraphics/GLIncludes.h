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

#ifndef __GLIncludes_H__
#define __GLIncludes_H__
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
	#include <TargetConditionals.h>
	#if TARGET_OS_IPHONE
		#define TARGET_OPENGL_ES 1
		#ifndef GLGRAPHICS_NO_GLEW
			#define GLGRAPHICS_NO_GLEW
		#endif
		#include <OpenGLES/ES1/gl.h>
		#include <OpenGLES/ES1/glext.h>
		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>
	#elif TARGET_OS_MAC
		#define TARGET_OPENGL_ES 0
		#ifndef GLGRAPHICS_NO_GLEW
			#include "GL/glew.h"
		#endif
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
		#include <OpenGL/glext.h>
	#else
		#error Unknown platform (__APPLE__, but not TARGET_OS_MAC or TARGET_OS_IPHONE)
	#endif
#else
	#define TARGET_OPENGL_ES 0
	#ifndef GLGRAPHICS_NO_GLEW
		#include "GL/glew.h"
		#if defined(WIN32)
			#include "GL/wglew.h"
		#elif defined(__linux)
			// glxew.h includes X.h, which defines Atom and Screen, causing conflicts with the Atom in utilities and the Screen in screenmanager
			#define Atom Atom_not
			#define Screen Screen_not
			#include "GL/glxew.h"
			#undef Atom
			#undef Screen
		#endif
	#endif
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glext.h>
#endif

#ifdef __cplusplus
}
#endif
#endif
