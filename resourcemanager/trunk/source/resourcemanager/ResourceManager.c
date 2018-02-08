/*
  Copyright (c) 2018 Alex Diener
  
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
#include "utilities/AutoFreePool.h"
#include "utilities/IOUtilities.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SUPERCLASS StemObject

// TODO: Some sort of mechanism for threadsafe preloading?
// Preloading probably wants to place resources in a loaded, 0 reference count, nonpurgeable state, though with some special way to purge
// Careful order of operations to allow for atomic list access and lock-free multithreading

ResourceManager * ResourceManager_create(double (* timeFunction)(void)) {
	stemobject_create_implementation(ResourceManager, init, timeFunction)
}

bool ResourceManager_init(ResourceManager * self, double (* timeFunction)(void)) {
	call_super(init, self);
	
	self->timeFunction = timeFunction;
	
	self->typeHandlerCount = 0;
	self->private_ivar(typeHandlerAllocatedCount) = 8;
	self->typeHandlers = malloc(sizeof(*self->typeHandlers) * self->private_ivar(typeHandlerAllocatedCount));
	
	self->resourceCount = 0;
	self->private_ivar(resourceAllocatedCount) = 32;
	self->resources = malloc(sizeof(*self->resources) * self->private_ivar(resourceAllocatedCount));
	
	self->searchPathCount = 0;
	self->private_ivar(searchPathAllocatedCount) = 4;
	self->searchPaths = malloc(sizeof(*self->searchPaths) * self->private_ivar(searchPathAllocatedCount));
	
	self->dispose = ResourceManager_dispose;
	return true;
}

static void unloadResource(ResourceManager * self, unsigned int resourceIndex) {
	if (self->typeHandlers[self->resources[resourceIndex].typeHandlerIndex].unloadFunction != NULL) {
		self->typeHandlers[self->resources[resourceIndex].typeHandlerIndex].unloadFunction(self->resources[resourceIndex].resource, self->typeHandlers[self->resources[resourceIndex].typeHandlerIndex].context);
	}
	self->resourceCount--;
	for (; resourceIndex < self->resourceCount; resourceIndex++) {
		self->resources[resourceIndex] = self->resources[resourceIndex + 1];
	}
}

void ResourceManager_dispose(ResourceManager * self) {
	size_t resourceIndex, searchPathIndex;
	
	for (resourceIndex = self->resourceCount - 1; resourceIndex < self->resourceCount; resourceIndex--) {
		unloadResource(self, resourceIndex);
	}
	free(self->resources);
	free(self->typeHandlers);
	for (searchPathIndex = 0; searchPathIndex < self->searchPathCount; searchPathIndex++) {
		free(self->searchPaths[searchPathIndex]);
	}
	free(self->searchPaths);
	
	call_super(dispose, self);
}

void ResourceManager_addTypeHandler(ResourceManager * self,
                                    Atom typeName,
                                    void * (* loadFunction)(Atom resourceName, void * context),
                                    void (* unloadFunction)(void * resource, void * context),
                                    enum ResourceManagerPurgePolicy purgePolicy,
                                    void * context) {
	unsigned int typeHandlerIndex;
	
	for (typeHandlerIndex = 0; typeHandlerIndex < self->typeHandlerCount; typeHandlerIndex++) {
		if (self->typeHandlers[typeHandlerIndex].typeName == typeName) {
			return;
		}
	}
	if (self->private_ivar(typeHandlerAllocatedCount) <= self->typeHandlerCount) {
		self->private_ivar(typeHandlerAllocatedCount) *= 2;
		self->typeHandlers = realloc(self->typeHandlers, sizeof(*self->typeHandlers) * self->private_ivar(typeHandlerAllocatedCount));
	}
	self->typeHandlers[self->typeHandlerCount].typeName = typeName;
	self->typeHandlers[self->typeHandlerCount].loadFunction = loadFunction;
	self->typeHandlers[self->typeHandlerCount].unloadFunction = unloadFunction;
	self->typeHandlers[self->typeHandlerCount].purgePolicy = purgePolicy;
	self->typeHandlers[self->typeHandlerCount].context = context;
	self->typeHandlerCount++;
}

static void addResourceInternal(ResourceManager * self, unsigned int typeHandlerIndex, Atom resourceName, void * resource) {
	unsigned int resourceIndex;
	
	for (resourceIndex = 0; resourceIndex < self->resourceCount; resourceIndex++) {
		if (self->resources[resourceIndex].typeHandlerIndex == typeHandlerIndex && self->resources[resourceIndex].resourceName == resourceName) {
			return;
		}
	}
	
	if (self->private_ivar(resourceAllocatedCount) <= self->resourceCount) {
		self->private_ivar(resourceAllocatedCount) *= 2;
		self->resources = realloc(self->resources, sizeof(*self->resources) * self->private_ivar(resourceAllocatedCount));
	}
	self->resources[self->resourceCount].resourceName = resourceName;
	self->resources[self->resourceCount].resource = resource;
	self->resources[self->resourceCount].typeHandlerIndex = typeHandlerIndex;
	self->resources[self->resourceCount].referenceCount = 1;
	self->resourceCount++;
}

void ResourceManager_addResource(ResourceManager * self,
                                 Atom typeName,
                                 Atom resourceName,
                                 void * resource) {
	unsigned int typeHandlerIndex;
	
	for (typeHandlerIndex = 0; typeHandlerIndex < self->typeHandlerCount; typeHandlerIndex++) {
		if (self->typeHandlers[typeHandlerIndex].typeName == typeName) {
			addResourceInternal(self, typeHandlerIndex, resourceName, resource);
			break;
		}
	}
}

void * ResourceManager_referenceResource(ResourceManager * self,
                                         Atom typeName,
                                         Atom resourceName) {
	unsigned int resourceIndex;
	unsigned int typeHandlerIndex;
	void * resource;
	
	for (typeHandlerIndex = 0; typeHandlerIndex < self->typeHandlerCount; typeHandlerIndex++) {
		if (self->typeHandlers[typeHandlerIndex].typeName == typeName) {
			break;
		}
	}
	if (typeHandlerIndex >= self->typeHandlerCount) {
		return NULL;
	}
	
	for (resourceIndex = 0; resourceIndex < self->resourceCount; resourceIndex++) {
		if (self->resources[resourceIndex].typeHandlerIndex == typeHandlerIndex && self->resources[resourceIndex].resourceName == resourceName) {
			self->resources[resourceIndex].referenceCount++;
			return self->resources[resourceIndex].resource;
		}
	}
	
	if (self->typeHandlers[typeHandlerIndex].loadFunction == NULL) {
		return NULL;
	}
	resource = self->typeHandlers[typeHandlerIndex].loadFunction(resourceName, self->typeHandlers[typeHandlerIndex].context);
	if (resource != NULL) {
		addResourceInternal(self, typeHandlerIndex, resourceName, resource);
	}
	return resource;
}

void ResourceManager_unrecognizedRelease(ResourceManager * self, void * resource) {
	fprintf(stderr, "Warning: Resource %p was requested to be released, but ResourceManager %p doesn't currently know about it. Break on ResourceManager_unrecognizedRelease to debug.\n", resource, self);
}

void ResourceManager_releaseResource(ResourceManager * self,
                                     void * resource) {
	unsigned int resourceIndex;
	bool found = false;
	
	for (resourceIndex = 0; resourceIndex < self->resourceCount; resourceIndex++) {
		if (self->resources[resourceIndex].resource == resource) {
			found = true;
			if (self->resources[resourceIndex].referenceCount == 0) {
				fprintf(stderr, "Warning: Resource \"%s\" of type \"%s\" (%p) overreleased\n", self->resources[resourceIndex].resourceName, self->typeHandlers[self->resources[resourceIndex].typeHandlerIndex].typeName, resource);
				break;
			}
			self->resources[resourceIndex].referenceCount--;
			if (self->resources[resourceIndex].referenceCount == 0) {
				if (self->typeHandlers[self->resources[resourceIndex].typeHandlerIndex].purgePolicy == PURGE_IMMEDIATE) {
					unloadResource(self, resourceIndex);
				} else if (self->typeHandlers[self->resources[resourceIndex].typeHandlerIndex].purgePolicy == PURGE_DEFERRED) {
					self->resources[resourceIndex].zeroReferenceTime = self->timeFunction();
				}
			}
			break;
		}
	}
	if (!found) {
		ResourceManager_unrecognizedRelease(self, resource);
	}
}

void ResourceManager_purgeAll(ResourceManager * self) {
	unsigned int resourceIndex;
	
	for (resourceIndex = self->resourceCount - 1; resourceIndex < self->resourceCount; resourceIndex--) {
		if (self->resources[resourceIndex].referenceCount == 0 &&
		    self->typeHandlers[self->resources[resourceIndex].typeHandlerIndex].purgePolicy == PURGE_DEFERRED) {
			unloadResource(self, resourceIndex);
		}
	}
}

void ResourceManager_purgeAllOlderThan(ResourceManager * self, double age) {
	unsigned int resourceIndex;
	double currentTime = self->timeFunction();
	
	for (resourceIndex = self->resourceCount - 1; resourceIndex < self->resourceCount; resourceIndex--) {
		if (self->resources[resourceIndex].referenceCount == 0 &&
		    self->typeHandlers[self->resources[resourceIndex].typeHandlerIndex].purgePolicy == PURGE_DEFERRED &&
		    self->resources[resourceIndex].zeroReferenceTime + age <= currentTime) {
			unloadResource(self, resourceIndex);
		}
	}
}


void ResourceManager_addSearchPath(ResourceManager * self, const char * path) {
	size_t searchPathIndex;
	
	for (searchPathIndex = 0; searchPathIndex < self->searchPathCount; searchPathIndex++) {
		if (!strcmp(self->searchPaths[searchPathIndex], path)) {
			return;
		}
	}
	if (self->searchPathCount >= self->private_ivar(searchPathAllocatedCount)) {
		self->private_ivar(searchPathAllocatedCount) *= 2;
		self->searchPaths = realloc(self->searchPaths, sizeof(*self->searchPaths) * self->private_ivar(searchPathAllocatedCount));
	}
	self->searchPaths[self->searchPathCount++] = strdup(path);
}

void ResourceManager_removeSearchPath(ResourceManager * self, const char * path) {
	size_t searchPathIndex;
	
	for (searchPathIndex = 0; searchPathIndex < self->searchPathCount; searchPathIndex++) {
		if (!strcmp(self->searchPaths[searchPathIndex], path)) {
			free(self->searchPaths[searchPathIndex]);
			self->searchPathCount--;
			for (; searchPathIndex < self->searchPathCount; searchPathIndex++) {
				self->searchPaths[searchPathIndex] = self->searchPaths[searchPathIndex + 1];
			}
			break;
		}
	}
}

const char * ResourceManager_resolveFilePath(ResourceManager * self, const char * fileName) {
	size_t searchPathIndex;
	char testPath[PATH_MAX];
	struct stat statbuf;
	
	for (searchPathIndex = self->searchPathCount - 1; searchPathIndex < self->searchPathCount; searchPathIndex--) {
		snprintf_safe(testPath, PATH_MAX, "%s%s%s", self->searchPaths[searchPathIndex], self->searchPaths[searchPathIndex][strlen(self->searchPaths[searchPathIndex]) - 1] == '/' ? "" : "/", fileName);
		if (!stat(testPath, &statbuf)) {
			return AutoFreePool_add(strdup(testPath));
		}
	}
	return NULL;
}
