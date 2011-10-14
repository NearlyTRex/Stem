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

#include "resourcemanager/ResourceManager.h"
#include <stdlib.h>
#include <string.h>

ResourceManager * ResourceManager_create() {
	stemobject_create_implementation(ResourceManager, init)
}

void ResourceManager_init(compat_type(ResourceManager *) selfPtr) {
	ResourceManager * self = selfPtr;
	
	StemObject_init(self);
	
	self->typeHandlerCount = 0;
	self->typeHandlers = NULL;
	self->resourceCount = 0;
	self->resources = NULL;
	
	self->dispose = ResourceManager_dispose;
	self->addTypeHandler = ResourceManager_addTypeHandler;
	self->addResource = ResourceManager_addResource;
	self->referenceResource = ResourceManager_referenceResource;
	self->releaseResource = ResourceManager_releaseResource;
}

void ResourceManager_dispose(compat_type(ResourceManager *) selfPtr) {
	ResourceManager * self = selfPtr;
	unsigned int typeHandlerIndex;
	unsigned int resourceIndex;
	
	for (typeHandlerIndex = 0; typeHandlerIndex < self->typeHandlerCount; typeHandlerIndex++) {
		free(self->typeHandlers[typeHandlerIndex].typeName);
	}
	free(self->typeHandlers);
	
	for (resourceIndex = 0; resourceIndex < self->resourceCount; resourceIndex++) {
		free(self->resources[resourceIndex].resourceName);
	}
	free(self->resources);
	
	StemObject_dispose(self);
}

void ResourceManager_addTypeHandler(compat_type(ResourceManager *) selfPtr,
                                    const char * typeName,
                                    void * (* loadFunction)(const char * resourceName, void * context),
                                    void (* unloadFunction)(void * resource, void * context),
                                    void * context) {
	ResourceManager * self = selfPtr;
	unsigned int typeHandlerIndex;
	
	for (typeHandlerIndex = 0; typeHandlerIndex < self->typeHandlerCount; typeHandlerIndex++) {
		if (!strcmp(self->typeHandlers[typeHandlerIndex].typeName, typeName)) {
			return;
		}
	}
	self->typeHandlers = realloc(self->typeHandlers, sizeof(struct ResourceManager_typeHandler) * (self->typeHandlerCount + 1));
	self->typeHandlers[self->typeHandlerCount].typeName = malloc(strlen(typeName) + 1);
	strcpy(self->typeHandlers[self->typeHandlerCount].typeName, typeName);
	self->typeHandlers[self->typeHandlerCount].loadFunction = loadFunction;
	self->typeHandlers[self->typeHandlerCount].unloadFunction = unloadFunction;
	self->typeHandlers[self->typeHandlerCount].context = context;
	self->typeHandlerCount++;
}

static void addResourceInternal(ResourceManager * self, const char * typeName, const char * resourceName, void * resource) {
	unsigned int resourceIndex;
	
	for (resourceIndex = 0; resourceIndex < self->resourceCount; resourceIndex++) {
		if (!strcmp(self->resources[resourceIndex].typeName, typeName) && !strcmp(self->resources[resourceIndex].resourceName, resourceName)) {
			return;
		}
	}
	
	self->resources = realloc(self->resources, sizeof(struct ResourceManager_resource) * (self->resourceCount + 1));
	self->resources[self->resourceCount].typeName = typeName;
	self->resources[self->resourceCount].resourceName = malloc(strlen(resourceName) + 1);
	strcpy(self->resources[self->resourceCount].resourceName, resourceName);
	self->resources[self->resourceCount].resource = resource;
	self->resources[self->resourceCount].referenceCount = 1;
	self->resourceCount++;
}

void ResourceManager_addResource(compat_type(ResourceManager *) selfPtr,
                                 const char * typeName,
                                 const char * resourceName,
                                 void * resource) {
	ResourceManager * self = selfPtr;
	unsigned int typeHandlerIndex;
	
	for (typeHandlerIndex = 0; typeHandlerIndex < self->typeHandlerCount; typeHandlerIndex++) {
		if (!strcmp(self->typeHandlers[typeHandlerIndex].typeName, typeName)) {
			addResourceInternal(self, self->typeHandlers[typeHandlerIndex].typeName, resourceName, resource);
			break;
		}
	}
}

void * ResourceManager_referenceResource(compat_type(ResourceManager *) selfPtr,
                                         const char * typeName,
                                         const char * resourceName) {
	ResourceManager * self = selfPtr;
	unsigned int resourceIndex;
	unsigned int typeHandlerIndex;
	
	for (resourceIndex = 0; resourceIndex < self->resourceCount; resourceIndex++) {
		if (!strcmp(self->resources[resourceIndex].typeName, typeName) && !strcmp(self->resources[resourceIndex].resourceName, resourceName)) {
			self->resources[resourceIndex].referenceCount++;
			return self->resources[resourceIndex].resource;
		}
	}
	
	for (typeHandlerIndex = 0; typeHandlerIndex < self->typeHandlerCount; typeHandlerIndex++) {
		if (!strcmp(self->typeHandlers[typeHandlerIndex].typeName, typeName)) {
			void * resource;
			
			if (self->typeHandlers[typeHandlerIndex].loadFunction == NULL) {
				return NULL;
			}
			resource = self->typeHandlers[typeHandlerIndex].loadFunction(resourceName, self->typeHandlers[typeHandlerIndex].context);
			if (resource != NULL) {
				addResourceInternal(self, self->typeHandlers[typeHandlerIndex].typeName, resourceName, resource);
			}
			return resource;
		}
	}
	
	return NULL;
}

void ResourceManager_releaseResource(compat_type(ResourceManager *) selfPtr,
                                     const char * typeName,
                                     const char * resourceName) {
	ResourceManager * self = selfPtr;
	unsigned int resourceIndex;
	
	for (resourceIndex = 0; resourceIndex < self->resourceCount; resourceIndex++) {
		if (!strcmp(self->resources[resourceIndex].typeName, typeName) && !strcmp(self->resources[resourceIndex].resourceName, resourceName)) {
			self->resources[resourceIndex].referenceCount--;
			if (self->resources[resourceIndex].referenceCount == 0) {
				unsigned int typeHandlerIndex;
				
				for (typeHandlerIndex = 0; typeHandlerIndex < self->typeHandlerCount; typeHandlerIndex++) {
					if (!strcmp(self->typeHandlers[typeHandlerIndex].typeName, typeName)) {
						if (self->typeHandlers[typeHandlerIndex].unloadFunction != NULL) {
							self->typeHandlers[typeHandlerIndex].unloadFunction(self->resources[resourceIndex].resource, self->typeHandlers[typeHandlerIndex].context);
						}
						free(self->resources[resourceIndex].resourceName);
						self->resourceCount--;
						for (; resourceIndex < self->resourceCount; resourceIndex++) {
							self->resources[resourceIndex] = self->resources[resourceIndex + 1];
						}
						break;
					}
				}
			}
			break;
		}
	}
}
