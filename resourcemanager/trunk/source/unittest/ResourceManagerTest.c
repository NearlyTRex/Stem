#include "unittest/TestSuite.h"
#include "resourcemanager/ResourceManager.h"
#include <string.h>

static double currentTime;

static double timeFunction() {
	return currentTime;
}

static void testInit() {
	ResourceManager resourceManager, * resourceManagerPtr;
	bool success;
	
	memset(&resourceManager, 0x00, sizeof(resourceManager));
	success = ResourceManager_init(&resourceManager, timeFunction);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(resourceManager.dispose == ResourceManager_dispose, "Expected %p but got %p", ResourceManager_dispose, resourceManager.dispose);
	TestCase_assert(resourceManager.timeFunction == timeFunction, "Expected %p but got %p", timeFunction, resourceManager.timeFunction);
	resourceManager.dispose(&resourceManager);
	
	memset(&resourceManager, 0xFF, sizeof(resourceManager));
	success = ResourceManager_init(&resourceManager, timeFunction);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(resourceManager.dispose == ResourceManager_dispose, "Expected %p but got %p", ResourceManager_dispose, resourceManager.dispose);
	TestCase_assert(resourceManager.timeFunction == timeFunction, "Expected %p but got %p", timeFunction, resourceManager.timeFunction);
	resourceManager.dispose(&resourceManager);
	
	resourceManagerPtr = ResourceManager_create(timeFunction);
	TestCase_assert(resourceManagerPtr != NULL, "Expected non-NULL but got NULL");
	if (resourceManagerPtr == NULL) { return; } // Suppress clang warning
	TestCase_assert(resourceManagerPtr->dispose == ResourceManager_dispose, "Expected %p but got %p", ResourceManager_dispose, resourceManagerPtr->dispose);
	TestCase_assert(resourceManagerPtr->timeFunction == timeFunction, "Expected %p but got %p", timeFunction, resourceManagerPtr->timeFunction);
	resourceManagerPtr->dispose(resourceManagerPtr);
}

static char * loadContext;
static Atom resourceNameToLoad;
static void * resourceToLoad;
static unsigned int loadResourceCalls;
static unsigned int unloadResourceCalls;

static void * loadResource(Atom resourceName, void * context) {
	loadResourceCalls++;
	TestCase_assert(resourceName == resourceNameToLoad, "Expected %p but got %p", resourceNameToLoad, resourceName);
	TestCase_assert(context == loadContext, "Expected %p but got %p", loadContext, context);
	return resourceToLoad;
}

static void unloadResource(void * resource, void * context) {
	unloadResourceCalls++;
	TestCase_assert(resource == resourceToLoad, "Expected %p but got %p", resourceToLoad, resource);
	TestCase_assert(context == loadContext, "Expected %p but got %p", loadContext, context);
}

static void testAddResource() {
	ResourceManager * resourceManager;
	void * resource;
	
	loadResourceCalls = 0;
	unloadResourceCalls = 0;
	
	resourceManager = ResourceManager_create(timeFunction);
	TestCase_assert(resourceManager != NULL, "Expected non-NULL but got NULL");
	if (resourceManager == NULL) { return; } // Suppress clang warning
	
	resourceNameToLoad = ATOM("bar");
	resourceToLoad = "foo";
	ResourceManager_addResource(resourceManager, ATOM("type"), resourceNameToLoad, resourceToLoad);
	resource = ResourceManager_referenceResource(resourceManager, ATOM("type"), ATOM("resource"));
	TestCase_assert(resource == NULL, "Expected NULL but got %p", resource);
	
	loadContext = NULL;
	ResourceManager_addTypeHandler(resourceManager, ATOM("type"), loadResource, unloadResource, PURGE_IMMEDIATE, loadContext);
	ResourceManager_addResource(resourceManager, ATOM("type"), resourceNameToLoad, resourceToLoad);
	resource = ResourceManager_referenceResource(resourceManager, ATOM("type"), resourceNameToLoad);
	TestCase_assert(resource == resourceToLoad, "Expected %p but got %p", resourceToLoad, resource);
	TestCase_assert(loadResourceCalls == 0, "Expected 0 but got %u", loadResourceCalls);
	
	ResourceManager_releaseResource(resourceManager, resource);
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	
	ResourceManager_releaseResource(resourceManager, resource);
	TestCase_assert(unloadResourceCalls == 1, "Expected 1 but got %u", unloadResourceCalls);
	
	resourceManager->dispose(resourceManager);
}

static void testReferenceResource() {
	ResourceManager * resourceManager;
	void * resource;
	
	loadResourceCalls = 0;
	unloadResourceCalls = 0;
	
	resourceManager = ResourceManager_create(timeFunction);
	TestCase_assert(resourceManager != NULL, "Expected non-NULL but got NULL");
	if (resourceManager == NULL) { return; } // Suppress clang warning
	resource = ResourceManager_referenceResource(resourceManager, ATOM("type"), resourceNameToLoad);
	TestCase_assert(resource == NULL, "Expected NULL but got %p", resource);
	
	loadContext = "bar";
	resourceNameToLoad = ATOM("resource");
	resourceToLoad = "baz";
	ResourceManager_addTypeHandler(resourceManager, ATOM("type"), loadResource, unloadResource, PURGE_IMMEDIATE, loadContext);
	
	TestCase_assert(loadResourceCalls == 0, "Expected 0 but got %u", loadResourceCalls);
	resource = ResourceManager_referenceResource(resourceManager, ATOM("type"), resourceNameToLoad);
	TestCase_assert(resource == resourceToLoad, "Expected %p but got %p", resourceToLoad, resource);
	TestCase_assert(loadResourceCalls == 1, "Expected 1 but got %u", loadResourceCalls);
	
	resource = ResourceManager_referenceResource(resourceManager, ATOM("type"), resourceNameToLoad);
	TestCase_assert(resource == resourceToLoad, "Expected %p but got %p", resourceToLoad, resource);
	TestCase_assert(loadResourceCalls == 1, "Expected 1 but got %u", loadResourceCalls);
	
	ResourceManager_releaseResource(resourceManager, resource);
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	
	ResourceManager_releaseResource(resourceManager, resource);
	TestCase_assert(unloadResourceCalls == 1, "Expected 1 but got %u", unloadResourceCalls);
	
	resource = ResourceManager_referenceResource(resourceManager, ATOM("type"), resourceNameToLoad);
	TestCase_assert(resource == resourceToLoad, "Expected %p but got %p", resourceToLoad, resource);
	TestCase_assert(loadResourceCalls == 2, "Expected 2 but got %u", loadResourceCalls);
	
	ResourceManager_releaseResource(resourceManager, resource);
	TestCase_assert(unloadResourceCalls == 2, "Expected 2 but got %u", unloadResourceCalls);
	
	resourceManager->dispose(resourceManager);
}

static void testOptionalityOfCallbacks() {
	ResourceManager * resourceManager;
	void * resource;
	
	resourceManager = ResourceManager_create(timeFunction);
	TestCase_assert(resourceManager != NULL, "Expected non-NULL but got NULL");
	if (resourceManager == NULL) { return; } // Suppress clang warning
	
	resourceToLoad = "bar";
	loadContext = NULL;
	loadResourceCalls = 0;
	unloadResourceCalls = 0;
	ResourceManager_addTypeHandler(resourceManager, ATOM("load only"), loadResource, NULL, PURGE_IMMEDIATE, NULL);
	ResourceManager_addTypeHandler(resourceManager, ATOM("unload only"), NULL, unloadResource, PURGE_IMMEDIATE, NULL);
	
	resource = ResourceManager_referenceResource(resourceManager, ATOM("unload only"), ATOM("foo"));
	TestCase_assert(resource == NULL, "Expected NULL but got %p", resource);
	TestCase_assert(loadResourceCalls == 0, "Expected 0 but got %u", loadResourceCalls);
	
	ResourceManager_addResource(resourceManager, ATOM("load only"), ATOM("foo"), resourceToLoad);
	ResourceManager_releaseResource(resourceManager, resourceToLoad);
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	
	resourceManager->dispose(resourceManager);
}

static void testNULLResources() {
	ResourceManager * resourceManager;
	
	resourceManager = ResourceManager_create(timeFunction);
	TestCase_assert(resourceManager != NULL, "Expected non-NULL but got NULL");
	if (resourceManager == NULL) { return; } // Suppress clang warning
	
	ResourceManager_addTypeHandler(resourceManager, ATOM("type"), loadResource, unloadResource, PURGE_IMMEDIATE, loadContext);
	resourceNameToLoad = ATOM("resource");
	resourceToLoad = NULL;
	
	ResourceManager_referenceResource(resourceManager, ATOM("type"), resourceNameToLoad);
	TestCase_assert(resourceManager->resourceCount == 0, "Expected 0 but got %u\n", (unsigned int) resourceManager->resourceCount);
	
	resourceToLoad = "!NULL";
	ResourceManager_referenceResource(resourceManager, ATOM("type"), resourceNameToLoad);
	TestCase_assert(resourceManager->resourceCount == 1, "Expected 1 but got %u\n", (unsigned int) resourceManager->resourceCount);
	
	resourceManager->dispose(resourceManager);
}

static void testPurge() {
	ResourceManager * resourceManager;
	void * resource;
	
	resourceManager = ResourceManager_create(timeFunction);
	TestCase_assert(resourceManager != NULL, "Expected non-NULL but got NULL");
	if (resourceManager == NULL) { return; } // Suppress clang warning
	
	ResourceManager_addTypeHandler(resourceManager, ATOM("type_immediate"), loadResource, unloadResource, PURGE_IMMEDIATE, loadContext);
	ResourceManager_addTypeHandler(resourceManager, ATOM("type_deferred"), loadResource, unloadResource, PURGE_DEFERRED, loadContext);
	ResourceManager_addTypeHandler(resourceManager, ATOM("type_never"), loadResource, unloadResource, PURGE_NEVER, loadContext);
	resourceNameToLoad = ATOM("test");
	resourceToLoad = "hi";
	
	resource = ResourceManager_referenceResource(resourceManager, ATOM("type_immediate"), resourceNameToLoad);
	unloadResourceCalls = 0;
	ResourceManager_releaseResource(resourceManager, resource);
	TestCase_assert(unloadResourceCalls == 1, "Expected 1 but got %u", unloadResourceCalls);
	
	resource = ResourceManager_referenceResource(resourceManager, ATOM("type_deferred"), resourceNameToLoad);
	unloadResourceCalls = 0;
	ResourceManager_purgeAll(resourceManager);
	ResourceManager_releaseResource(resourceManager, resource);
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	ResourceManager_purgeAll(resourceManager);
	TestCase_assert(unloadResourceCalls == 1, "Expected 1 but got %u", unloadResourceCalls);
	
	currentTime = 0.0;
	resource = ResourceManager_referenceResource(resourceManager, ATOM("type_deferred"), resourceNameToLoad);
	unloadResourceCalls = 0;
	ResourceManager_releaseResource(resourceManager, resource);
	ResourceManager_purgeAllOlderThan(resourceManager, 1.0);
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	currentTime = 2.0;
	ResourceManager_purgeAllOlderThan(resourceManager, 3.0);
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	ResourceManager_purgeAllOlderThan(resourceManager, 1.0);
	TestCase_assert(unloadResourceCalls == 1, "Expected 1 but got %u", unloadResourceCalls);
	
	resource = ResourceManager_referenceResource(resourceManager, ATOM("type_never"), resourceNameToLoad);
	unloadResourceCalls = 0;
	currentTime = 0.0;
	ResourceManager_releaseResource(resourceManager, resource);
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	ResourceManager_purgeAll(resourceManager);
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	currentTime = 2.0;
	ResourceManager_purgeAllOlderThan(resourceManager, 1.0);
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	
	ResourceManager_dispose(resourceManager);
}

static void testSearchPaths() {
	ResourceManager * resourceManager;
	const char * resolvedPath;
	
	resourceManager = ResourceManager_create(timeFunction);
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile");
	TestCase_assert(resolvedPath == NULL, "Expected NULL but got \"%s\"", resolvedPath);
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile2");
	TestCase_assert(resolvedPath == NULL, "Expected NULL but got \"%s\"", resolvedPath);
	
	ResourceManager_addSearchPath(resourceManager, ".");
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile");
	TestCase_assert(resolvedPath != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(resolvedPath, "./emptyfile"), "Expected \"./emptyfile\", but got \"%s\"", resolvedPath);
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile2");
	TestCase_assert(resolvedPath != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(resolvedPath, "./emptyfile2"), "Expected \"./emptyfile2\", but got \"%s\"", resolvedPath);
	
	ResourceManager_addSearchPath(resourceManager, "subdir");
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile");
	TestCase_assert(resolvedPath != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(resolvedPath, "subdir/emptyfile"), "Expected \"subdir/emptyfile\", but got \"%s\"", resolvedPath);
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile2");
	TestCase_assert(resolvedPath != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(resolvedPath, "./emptyfile2"), "Expected \"./emptyfile2\", but got \"%s\"", resolvedPath);
	
	ResourceManager_removeSearchPath(resourceManager, ".");
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile");
	TestCase_assert(resolvedPath != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(resolvedPath, "subdir/emptyfile"), "Expected \"subdir/emptyfile\", but got \"%s\"", resolvedPath);
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile2");
	TestCase_assert(resolvedPath == NULL, "Expected NULL but got \"%s\"", resolvedPath);
	
	ResourceManager_addSearchPath(resourceManager, ".");
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile");
	TestCase_assert(resolvedPath != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(resolvedPath, "./emptyfile"), "Expected \"./emptyfile\", but got \"%s\"", resolvedPath);
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile2");
	TestCase_assert(resolvedPath != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(resolvedPath, "./emptyfile2"), "Expected \"./emptyfile2\", but got \"%s\"", resolvedPath);
	
	ResourceManager_dispose(resourceManager);
}

static void testDuplicateSearchPaths() {
	ResourceManager * resourceManager;
	const char * resolvedPath;
	
	resourceManager = ResourceManager_create(timeFunction);
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile");
	TestCase_assert(resolvedPath == NULL, "Expected NULL but got \"%s\"", resolvedPath);
	
	ResourceManager_addSearchPath(resourceManager, ".");
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile");
	TestCase_assert(resolvedPath != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(resolvedPath, "./emptyfile"), "Expected \"./emptyfile\", but got \"%s\"", resolvedPath);
	
	ResourceManager_addSearchPath(resourceManager, ".");
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile");
	TestCase_assert(resolvedPath != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(resolvedPath, "./emptyfile"), "Expected \"./emptyfile\", but got \"%s\"", resolvedPath);
	
	ResourceManager_removeSearchPath(resourceManager, ".");
	resolvedPath = ResourceManager_resolveFilePath(resourceManager, "emptyfile");
	TestCase_assert(resolvedPath == NULL, "Expected NULL but got \"%s\"", resolvedPath);
}

TEST_SUITE(ResourceManagerTest,
           testInit,
           testAddResource,
           testReferenceResource,
           testOptionalityOfCallbacks,
           testNULLResources,
           testPurge,
           testSearchPaths,
           testDuplicateSearchPaths)
