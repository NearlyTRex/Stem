ResourceManager is a StemObject subclass that provides facilities for managing the loading, retrieval, and unloading of resources. The definition of "resource" is completely up to the client; ResourceManager represents resources as void * and provides hooks for the loading and unloading of them to be implemented by the calling code.

After instantiating a ResourceManager, you must add one or more type handlers in order to be able to do anything meaningful with it. A type handler includes a type name string, an optional callback that loads a resource of that type, an optional callback that unloads a resource of that type, a purge policy, and a context pointer. The context pointer is passed to both the loadFunction and the unloadFunction when they are called.

Once your type handlers are added, there are two ways to make resources available for retrieval from a ResourceManager: you can call addResource() to add one directly, or you can call referenceResource() to allow it to be loaded by your provided loadFunction. If you haven't provided a loadFunction, you must use addResource() for every resource you will subsequently reference.

Once a resource has been loaded, ResourceManager keeps it in its cache along with a reference count. A resource starts with a reference count of 1 when loaded via addResource() or referenceResource(). Subsequent calls to referenceResource() increment the reference count, and calls to releaseResource() decrement it.

When a resource's reference count drops to 0, the behavior depends on the purge policy set for the resource type. For types registered as PURGE_IMMEDIATE, releaseResource() will call your unload function immediately if you've provided one. For types registered as PURGE_DEFERRED, zero-reference-count resources can be unloaded when ResourceManager_purgeAll() or ResourceManager_purgeAllOlderThan() is called. Types registered as PURGE_NEVER will never be unloaded, so providing an unloadFunction callback for that type is unnecessary.

Some considerations when choosing a purge policy:
- Resources should only be marked PURGE_IMMEDIATE if loading and unloading has a very low cost, or if a resource is unlikely to need to be reloaded soon after being released.
- Most resources should be marked PURGE_DEFERRED. By using ResourceManager_purgeAllOlderThan(), unreferenced resources can stick around for a short time after the last user has released them, so the new users can reference them without having to unload and reload in between.
- Resources should only be marked PURGE_NEVER if they need to stay loaded for the program's entire execution time.

FileCatalog is provided as a convenient way to map resource names to files. Its usage is optional.

Example usage:

#include "pngimageio/PNGImageIO.h"
#include "resourcemanager/ResourceManager.h"
#include "shell/Shell.h"
#include <stdio.h>

static ResourceManager * resourceManager;

struct PNGSettings {
	int pixelFormat;
	bool flipVertical;
} defaultPNGSettings = {PNG_PIXEL_FORMAT_AUTOMATIC, false};

static void * loadPNG(Atom resourceName, void * context) {
	struct PNGSettings * settings = context;
	return PNGImageIO_loadPNGFile(resourcePath(resourceName), settings.pixelFormat, settings.flipVertical);
}

static void unloadPNG(void * resource, void * context) {
	BitmapImage_dispose(resource);
}

void Target_init() {
	char * string;
	
	resourceManager = ResourceManager_create(Shell_getCurrentTime);
	resourceManager->addTypeHandler(resourceManager, ATOM("string"), NULL, NULL, PURGE_NEVER, NULL);
	resourceManager->addTypeHandler(resourceManager, ATOM("png"), loadPNG, unloadPNG, PURGE_DEFERRED, &defaultPNGSettings);
	resourceManager->addResource(resourceManager, ATOM("string"), ATOM("hello"), "Hello, world!");
	
	string = resourceManager->referenceResource(resourceManager, ATOM("string"), ATOM("hello"));
	printf("%s\n", string);
	resourceManager->releaseResource(resourceManager, ATOM("string"), ATOM("hello"));
	
	Shell_mainLoop();
}

bool Target_draw() {
	BitmapImage * image;
	
	image = resourceManager->referenceResource(resourceManager, ATOM("png"), ATOM("myimage.png"));
	// Draw image
	resourceManager->releaseResource(resourceManager, ATOM("png"), ATOM("myimage.png"));
	
	ResourceManager_purgeAllOlderThan(resourceManager, 5.0);
	return true;
}
