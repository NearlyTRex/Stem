/*
  Copyright (c) 2011 Alex Diener
  
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

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

typedef struct ResourceManager ResourceManager;

#include "stemobject/StemObject.h"
#include <stdlib.h>

struct ResourceManager_typeHandler {
	char * typeName;
	void * (* loadFunction)(const char * resourceName, void * context);
	void (* unloadFunction)(void * resource, void * context);
	void * context;
};

struct ResourceManager_resource {
	const char * typeName;
	char * resourceName;
	void * resource;
	unsigned int referenceCount;
};

#define ResourceManager_structContents \
	StemObject_structContents \
	\
	size_t typeHandlerCount; \
	struct ResourceManager_typeHandler * typeHandlers; \
	size_t resourceCount; \
	struct ResourceManager_resource * resources; \
	\
	void (* addTypeHandler)(compat_type(ResourceManager *) self, \
	                        const char * typeName, \
	                        void * (* loadFunction)(const char * resourceName, void * context), \
	                        void (* unloadFunction)(void * resource, void * context), \
	                        void * context); \
	void (* addResource)(compat_type(ResourceManager *) self, \
	                     const char * typeName, \
	                     const char * resourceName, \
	                     void * resource); \
	void * (* referenceResource)(compat_type(ResourceManager *) self, \
	                             const char * typeName, \
	                             const char * resourceName); \
	void (* releaseResource)(compat_type(ResourceManager *) self, \
	                         const char * typeName, \
	                         const char * resourceName);

struct ResourceManager {
	ResourceManager_structContents
};

ResourceManager * ResourceManager_create();
void ResourceManager_init(compat_type(ResourceManager *) selfPtr);
void ResourceManager_dispose(compat_type(ResourceManager *) selfPtr);

void ResourceManager_addTypeHandler(compat_type(ResourceManager *) selfPtr,
                                    const char * typeName,
                                    void * (* loadFunction)(const char * resourceName, void * context),
                                    void (* unloadFunction)(void * resource, void * context),
                                    void * context);
void ResourceManager_addResource(compat_type(ResourceManager *) selfPtr,
                                 const char * typeName,
                                 const char * resourceName,
                                 void * resource);
void * ResourceManager_referenceResource(compat_type(ResourceManager *) selfPtr,
                                         const char * typeName,
                                         const char * resourceName);
void ResourceManager_releaseResource(compat_type(ResourceManager *) selfPtr,
                                     const char * typeName,
                                     const char * resourceName);

#endif
