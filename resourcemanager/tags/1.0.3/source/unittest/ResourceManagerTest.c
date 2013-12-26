#include "unittest/TestSuite.h"
#include "resourcemanager/ResourceManager.h"

static void testInit() {
	ResourceManager resourceManager, * resourceManagerPtr;
	bool success;
	
	memset(&resourceManager, 0x00, sizeof(resourceManager));
	success = ResourceManager_init(&resourceManager);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(resourceManager.dispose == ResourceManager_dispose, "Expected %p but got %p", ResourceManager_dispose, resourceManager.dispose);
	TestCase_assert(resourceManager.addTypeHandler == ResourceManager_addTypeHandler, "Expected %p but got %p", ResourceManager_addTypeHandler, resourceManager.addTypeHandler);
	TestCase_assert(resourceManager.addResource == ResourceManager_addResource, "Expected %p but got %p", ResourceManager_addResource, resourceManager.addResource);
	TestCase_assert(resourceManager.referenceResource == ResourceManager_referenceResource, "Expected %p but got %p", ResourceManager_referenceResource, resourceManager.referenceResource);
	TestCase_assert(resourceManager.releaseResource == ResourceManager_releaseResource, "Expected %p but got %p", ResourceManager_releaseResource, resourceManager.releaseResource);
	resourceManager.dispose(&resourceManager);
	
	memset(&resourceManager, 0xFF, sizeof(resourceManager));
	success = ResourceManager_init(&resourceManager);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(resourceManager.dispose == ResourceManager_dispose, "Expected %p but got %p", ResourceManager_dispose, resourceManager.dispose);
	TestCase_assert(resourceManager.addTypeHandler == ResourceManager_addTypeHandler, "Expected %p but got %p", ResourceManager_addTypeHandler, resourceManager.addTypeHandler);
	TestCase_assert(resourceManager.addResource == ResourceManager_addResource, "Expected %p but got %p", ResourceManager_addResource, resourceManager.addResource);
	TestCase_assert(resourceManager.referenceResource == ResourceManager_referenceResource, "Expected %p but got %p", ResourceManager_referenceResource, resourceManager.referenceResource);
	TestCase_assert(resourceManager.releaseResource == ResourceManager_releaseResource, "Expected %p but got %p", ResourceManager_releaseResource, resourceManager.releaseResource);
	resourceManager.dispose(&resourceManager);
	
	resourceManagerPtr = ResourceManager_create();
	TestCase_assert(resourceManagerPtr != NULL, "Expected non-NULL but got NULL");
	if (resourceManagerPtr == NULL) { return; } // Suppress clang warning
	TestCase_assert(resourceManagerPtr->dispose == ResourceManager_dispose, "Expected %p but got %p", ResourceManager_dispose, resourceManagerPtr->dispose);
	TestCase_assert(resourceManagerPtr->addTypeHandler == ResourceManager_addTypeHandler, "Expected %p but got %p", ResourceManager_addTypeHandler, resourceManagerPtr->addTypeHandler);
	TestCase_assert(resourceManagerPtr->addResource == ResourceManager_addResource, "Expected %p but got %p", ResourceManager_addResource, resourceManagerPtr->addResource);
	TestCase_assert(resourceManagerPtr->referenceResource == ResourceManager_referenceResource, "Expected %p but got %p", ResourceManager_referenceResource, resourceManagerPtr->referenceResource);
	TestCase_assert(resourceManagerPtr->releaseResource == ResourceManager_releaseResource, "Expected %p but got %p", ResourceManager_releaseResource, resourceManagerPtr->releaseResource);
	resourceManagerPtr->dispose(resourceManagerPtr);
}

static char * loadContext;
static char * resourceNameToLoad;
static void * resourceToLoad;
static unsigned int loadResourceCalls;
static unsigned int unloadResourceCalls;

static void * loadResource(const char * resourceName, void * context) {
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
	
	resourceManager = ResourceManager_create();
	TestCase_assert(resourceManager != NULL, "Expected non-NULL but got NULL");
	if (resourceManager == NULL) { return; } // Suppress clang warning
	
	resourceNameToLoad = "bar";
	resourceToLoad = "foo";
	resourceManager->addResource(resourceManager, "type", resourceNameToLoad, resourceToLoad);
	resource = resourceManager->referenceResource(resourceManager, "type", "resource");
	TestCase_assert(resource == NULL, "Expected NULL but got %p", resource);
	
	loadContext = NULL;
	resourceManager->addTypeHandler(resourceManager, "type", loadResource, unloadResource, loadContext);
	resourceManager->addResource(resourceManager, "type", resourceNameToLoad, resourceToLoad);
	resource = resourceManager->referenceResource(resourceManager, "type", resourceNameToLoad);
	TestCase_assert(resource == resourceToLoad, "Expected %p but got %p", resourceToLoad, resource);
	TestCase_assert(loadResourceCalls == 0, "Expected 0 but got %u", loadResourceCalls);
	
	resourceManager->releaseResource(resourceManager, "type", resourceNameToLoad);
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	
	resourceManager->releaseResource(resourceManager, "type", resourceNameToLoad);
	TestCase_assert(unloadResourceCalls == 1, "Expected 1 but got %u", unloadResourceCalls);
	
	resourceManager->dispose(resourceManager);
}

static void testReferenceResource() {
	ResourceManager * resourceManager;
	void * resource;
	
	loadResourceCalls = 0;
	unloadResourceCalls = 0;
	
	resourceManager = ResourceManager_create();
	TestCase_assert(resourceManager != NULL, "Expected non-NULL but got NULL");
	if (resourceManager == NULL) { return; } // Suppress clang warning
	resource = resourceManager->referenceResource(resourceManager, "type", resourceNameToLoad);
	TestCase_assert(resource == NULL, "Expected NULL but got %p", resource);
	
	loadContext = "bar";
	resourceNameToLoad = "resource";
	resourceToLoad = "baz";
	resourceManager->addTypeHandler(resourceManager, "type", loadResource, unloadResource, loadContext);
	
	resourceManager->releaseResource(resourceManager, "type", "resource");
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	
	TestCase_assert(loadResourceCalls == 0, "Expected 0 but got %u", loadResourceCalls);
	resource = resourceManager->referenceResource(resourceManager, "type", resourceNameToLoad);
	TestCase_assert(resource == resourceToLoad, "Expected %p but got %p", resourceToLoad, resource);
	TestCase_assert(loadResourceCalls == 1, "Expected 1 but got %u", loadResourceCalls);
	
	resource = resourceManager->referenceResource(resourceManager, "type", resourceNameToLoad);
	TestCase_assert(resource == resourceToLoad, "Expected %p but got %p", resourceToLoad, resource);
	TestCase_assert(loadResourceCalls == 1, "Expected 1 but got %u", loadResourceCalls);
	
	resourceManager->releaseResource(resourceManager, "type", "resource");
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	
	resourceManager->releaseResource(resourceManager, "type", "resource");
	TestCase_assert(unloadResourceCalls == 1, "Expected 1 but got %u", unloadResourceCalls);
	
	resource = resourceManager->referenceResource(resourceManager, "type", resourceNameToLoad);
	TestCase_assert(resource == resourceToLoad, "Expected %p but got %p", resourceToLoad, resource);
	TestCase_assert(loadResourceCalls == 2, "Expected 2 but got %u", loadResourceCalls);
	
	resourceManager->releaseResource(resourceManager, "type", "resource");
	TestCase_assert(unloadResourceCalls == 2, "Expected 2 but got %u", unloadResourceCalls);
	
	resourceManager->dispose(resourceManager);
}

static void testOptionalityOfCallbacks() {
	ResourceManager * resourceManager;
	void * resource;
	
	resourceManager = ResourceManager_create();
	TestCase_assert(resourceManager != NULL, "Expected non-NULL but got NULL");
	if (resourceManager == NULL) { return; } // Suppress clang warning
	
	loadContext = NULL;
	loadResourceCalls = 0;
	unloadResourceCalls = 0;
	resourceManager->addTypeHandler(resourceManager, "load only", loadResource, NULL, NULL);
	resourceManager->addTypeHandler(resourceManager, "unload only", NULL, unloadResource, NULL);
	
	resource = resourceManager->referenceResource(resourceManager, "unload only", "foo");
	TestCase_assert(resource == NULL, "Expected NULL but got %p", resource);
	TestCase_assert(loadResourceCalls == 0, "Expected 0 but got %u", loadResourceCalls);
	
	resourceManager->addResource(resourceManager, "load only", "foo", "bar");
	resourceManager->releaseResource(resourceManager, "load only", "foo");
	TestCase_assert(unloadResourceCalls == 0, "Expected 0 but got %u", unloadResourceCalls);
	
	resourceManager->dispose(resourceManager);
}

static void testNULLResources() {
	ResourceManager * resourceManager;
	
	resourceManager = ResourceManager_create();
	TestCase_assert(resourceManager != NULL, "Expected non-NULL but got NULL");
	if (resourceManager == NULL) { return; } // Suppress clang warning
	
	resourceManager->addTypeHandler(resourceManager, "type", loadResource, unloadResource, loadContext);
	resourceNameToLoad = "resource";
	resourceToLoad = NULL;
	
	resourceManager->referenceResource(resourceManager, "type", resourceNameToLoad);
	TestCase_assert(resourceManager->resourceCount == 0, "Expected 0 but got %u\n", (unsigned int) resourceManager->resourceCount);
	
	resourceToLoad = "!NULL";
	resourceManager->referenceResource(resourceManager, "type", resourceNameToLoad);
	TestCase_assert(resourceManager->resourceCount == 1, "Expected 1 but got %u\n", (unsigned int) resourceManager->resourceCount);
	
	resourceManager->dispose(resourceManager);
}

TEST_SUITE(ResourceManagerTest,
           testInit,
           testAddResource,
           testReferenceResource,
           testOptionalityOfCallbacks,
           testNULLResources)
