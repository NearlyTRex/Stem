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

#ifndef __ResourceManager_H__
#define __ResourceManager_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct ResourceManager ResourceManager;

#include "stemobject/StemObject.h"
#include "utilities/Atom.h"
#include <stdlib.h>

enum ResourceManagerPurgePolicy {
	PURGE_IMMEDIATE, // Resources are unloaded inside ResourceManager_releaseResource() when their reference count drops to 0.
	PURGE_DEFERRED, // Resources are unloaded in ResourceManager_purge*() if their reference count has dropped to 0.
	PURGE_NEVER // Resources are never unloaded.
};

struct ResourceManager_typeHandler {
	Atom typeName;
	void * (* loadFunction)(Atom resourceName, void * context);
	void (* unloadFunction)(void * resource, void * context);
	enum ResourceManagerPurgePolicy purgePolicy;
	void * context;
};

struct ResourceManager_resource {
	Atom resourceName;
	void * resource;
	unsigned int typeHandlerIndex;
	unsigned int referenceCount;
	double zeroReferenceTime;
};

#define ResourceManager_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	double (* timeFunction)(void); \
	\
	size_t typeHandlerCount; \
	size_t private_ivar(typeHandlerAllocatedCount); \
	struct ResourceManager_typeHandler * typeHandlers; \
	\
	size_t resourceCount; \
	size_t private_ivar(resourceAllocatedCount); \
	struct ResourceManager_resource * resources; \
	\
	size_t searchPathCount; \
	size_t private_ivar(searchPathAllocatedCount); \
	char ** searchPaths;

stemobject_struct_definition(ResourceManager)

// timeFunction should be a function that returns a monotonically-increasing time value in seconds, such as Shell_getCurrentTime().
ResourceManager * ResourceManager_create(double (* timeFunction)(void));
bool ResourceManager_init(ResourceManager * self, double (* timeFunction)(void));
void ResourceManager_dispose(ResourceManager * self);

// Registers a type name with ResourceManager, defining callbacks to be invoked when this type of resource needs to be loaded or unloaded.
// The pointer returned from loadFunction will be returned from ResourceManager_referenceResource, and will be passed to unloadFunction
// when the resource needs to be unloaded.
void ResourceManager_addTypeHandler(ResourceManager * self,
                                    Atom typeName,
                                    void * (* loadFunction)(Atom resourceName, void * context),
                                    void (* unloadFunction)(void * resource, void * context),
                                    enum ResourceManagerPurgePolicy purgePolicy,
                                    void * context);

// Adds a loaded resource to ResourceManager with an initial reference count of 1. This is an alternative to having a resource loaded
// using the loadFunction callback provided to ResourceManager_addTypeHandler. The unloadFunction, if one is defined for this type, will
// still be called as normal for this resource if its reference count drops to 0 and the resource is purged.
void ResourceManager_addResource(ResourceManager * self,
                                 Atom typeName,
                                 Atom resourceName,
                                 void * resource);

// Fetches and returns the named resource, incrementing its reference count by 1. If the resource is not currently loaded, the
// loadFunction callback defined for typeName will be invoked to load it. Call ResourceManager_releaseResource after you've finished
// using it to allow it to be unloaded if necessary.
void * ResourceManager_referenceResource(ResourceManager * self,
                                         Atom typeName,
                                         Atom resourceName);

// Decrements the reference count of the named resource by 1. If the resource's reference count drops to 0 and the type has a purge
// policy of PURGE_IMMEDIATE, the unloadFunction callback defined for that type is called and the resource is unloaded.
void ResourceManager_releaseResource(ResourceManager * self,
                                     Atom typeName,
                                     Atom resourceName);

// Unloads all resources whose reference counts have dropped to 0, for types that have the PURGE_DEFERRED policy.
void ResourceManager_purgeAll(ResourceManager * self);

// Unloads all resources whose reference counts dropped to 0 longer ago than the specified number of seconds, for types that have
// the PURGE_DEFERRED policy.
void ResourceManager_purgeAllOlderThan(ResourceManager * self, double age);

// Adds a location to be searched when using ResourceManager_resolveFilePath. The latest added paths are searched first.
// The working directory is not automatically searched. To include it in searches, call this function with an argument of ".".
void ResourceManager_addSearchPath(ResourceManager * self, const char * path);

// Removes a location from the search list. Does nothing if ResourceManager doesn't already know about the specified path.
void ResourceManager_removeSearchPath(ResourceManager * self, const char * path);

// Attempts to resolve a local file name/path to a location on disk, using any search paths established by calls to
// ResourceManager_addSearchPath. The latest added paths are searched first. If no file with the specified name exists in any
// known search path, returns NULL. The returned string is allocated and added to AutoFreePool, so it should not be freed
// directly by the caller.
const char * ResourceManager_resolveFilePath(ResourceManager * self, const char * fileName);

#ifdef __cplusplus
}
#endif
#endif
