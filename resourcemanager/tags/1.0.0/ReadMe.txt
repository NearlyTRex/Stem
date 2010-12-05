= Overview =

ResourceManager is a StemObject subclass that provides facilities for managing the loading, retrieval, and unloading of resources. The definition of "resource" is completely up to the client; ResourceManager represents resources as void * and provides hooks for the loading and unloading of them to be implemented by the calling code.

= Basic Usage =

After instantiating a ResourceManager, you must add one or more type handlers in order to be able to do anything meaningful with it. A type handler includes a type name string, an optional callback that loads a resource of that type, an optional callback that unloads a resource of that type, and a context pointer. The context pointer is passed to both the loadFunction and the unloadFunction when they are called.

Once your type handlers are added, there are two ways to make resources available for retrieval from a ResourceManager: you can call addResource() to add one directly, or you can call referenceResource() to allow it to be loaded by your provided loadFunction. If you haven't provided a loadFunction, you must use addResource() for every resource you will subsequently reference.

Once a resource has been loaded, ResourceManager keeps it in its cache along with a reference count. A resource starts with a reference count of 1 when loaded via addResource() or referenceResource(). Subsequent calls to referenceResource() increment the reference count, and calls to releaseResource() decrement it. If the reference count drops to 0 as a result of calling releaseResource(), your unload function (if you've provided one) will be called.

= Example =

static ResourceManager * resourceManager;

static void * loadPNG(const char * resourceName, void * context);
static void unloadPNG(void * resource, void * context);
struct PNGSettings {
	int pixelFormat;
	bool flipVertical;
} defaultPNGSettings = {PNG_PIXEL_FORMAT_AUTOMATIC, false};

void Target_init() {
	char * string;
	
	resourceManager = ResourceManager_create();
	resourceManager->addTypeHandler(resourceManager, "string", NULL, NULL, NULL);
	resourceManager->addTypeHandler(resourceManager, "png", loadPNG, unloadPNG, &defaultPNGSettings);
	resourceManager->addResource(resourceManager, "string", "hello", "Hello, world!");
	
	string = resourceManager->referenceResource(resourceManager, "string", "hello");
	printf("%s\n", string);
	resourceManager->releaseResource(resourceManager, "string", "hello");
	
	Shell_mainLoop();
}

void Target_draw() {
	BitmapImage * image;
	
	image = resourceManager->referenceResource(resourceManager, "png", "myimage.png");
	// Draw image
	resourceManager->releaseResource(resourceManager, "png", "myimage.png");
}

static void * loadPNG(const char * resourceName, void * context) {
	struct PNGSettings * settings = context;
	return PNGImageIO_loadPNGFile(resourcePath(resourceName), settings.pixelFormat, settings.flipVertical);
}

static void unloadPNG(void * resource, void * context) {
	((BitmapImage *) resource)->dispose(resource);
}
