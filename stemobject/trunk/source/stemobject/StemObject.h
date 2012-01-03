/*
  Copyright (c) 2012 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __STEM_OBJECT_H__
#define __STEM_OBJECT_H__

#include <stdbool.h>

#define compat_type(type) void *
#define private_ivar(name) _private_##name
#define protected_ivar(name) _protected_##name
#define call_super(function, self, args...) SUPERCLASS##_##function((SUPERCLASS *) self, ##args)

#define stemobject_create_implementation(class_name, init_suffix, init_args...) \
	class_name * self; \
	\
	self = malloc(sizeof(class_name)); \
	class_name##_##init_suffix(self, ##init_args); \
	self->protected_ivar(allocated) = true; \
	return self;

#define stemobject_copy_implementation(class_name, initcopy_suffix, init_args...) \
	class_name * self = selfPtr; \
	class_name * copy; \
	\
	copy = malloc(sizeof(class_name)); \
	class_name##_##initcopy_suffix(copy, self, ##init_args); \
	copy->protected_ivar(allocated) = true; \
	return copy;

typedef struct StemObject StemObject;

#define StemObject_structContents(self_type) \
	bool protected_ivar(allocated); \
	\
	void (* dispose)(self_type * self);

struct StemObject {
	StemObject_structContents(StemObject)
};

StemObject * StemObject_create();
void StemObject_init(compat_type(StemObject *) selfPtr);

void StemObject_dispose(StemObject * self);

#endif
