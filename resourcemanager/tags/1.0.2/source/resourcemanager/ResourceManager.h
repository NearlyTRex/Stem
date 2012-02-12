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

#define ResourceManager_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	size_t typeHandlerCount; \
	struct ResourceManager_typeHandler * typeHandlers; \
	size_t resourceCount; \
	struct ResourceManager_resource * resources; \
	\
	void (* addTypeHandler)(self_type * self, \
	                        const char * typeName, \
	                        void * (* loadFunction)(const char * resourceName, void * context), \
	                        void (* unloadFunction)(void * resource, void * context), \
	                        void * context); \
	void (* addResource)(self_type * self, \
	                     const char * typeName, \
	                     const char * resourceName, \
	                     void * resource); \
	void * (* referenceResource)(self_type * self, \
	                             const char * typeName, \
	                             const char * resourceName); \
	void (* releaseResource)(self_type * self, \
	                         const char * typeName, \
	                         const char * resourceName);

stemobject_struct_definition(ResourceManager)

ResourceManager * ResourceManager_create();
void ResourceManager_init(ResourceManager * self);
void ResourceManager_dispose(ResourceManager * self);

void ResourceManager_addTypeHandler(ResourceManager * self,
                                    const char * typeName,
                                    void * (* loadFunction)(const char * resourceName, void * context),
                                    void (* unloadFunction)(void * resource, void * context),
                                    void * context);
void ResourceManager_addResource(ResourceManager * self,
                                 const char * typeName,
                                 const char * resourceName,
                                 void * resource);
void * ResourceManager_referenceResource(ResourceManager * self,
                                         const char * typeName,
                                         const char * resourceName);
void ResourceManager_releaseResource(ResourceManager * self,
                                     const char * typeName,
                                     const char * resourceName);

#endif
