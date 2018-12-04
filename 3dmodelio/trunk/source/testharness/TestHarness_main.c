#include "3dmodelio/AnimationIO.h"
#include "3dmodelio/ArmatureIO.h"
#include "3dmodelio/MaterialData.h"
#include "3dmodelio/MeshData.h"
#include "3dmodelio/Obj3DModelIO.h"
#include "3dmodelio/TextureAtlasData.h"
#include "binaryserialization/BinaryDeserializationContext.h"
#include "gamemath/Matrix4x4f.h"
#include "glgraphics/Animation.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/MeshRenderable.h"
#include "glgraphics/OrbitCamera.h"
#include "glgraphics/Renderer.h"
#include "glgraphics/SpriteRenderable.h"
#include "glgraphics/TextureAtlas.h"
#include "jsonserialization/JSONDeserializationContext.h"
#include "pngimageio/PNGImageIO.h"
#include "resourcemanager/ResourceManager.h"
#include "shell/Shell.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"
#include "utilities/AutoFreePool.h"
#include "utilities/IOUtilities.h"
#include "utilities/Ranrot.h"

#if defined(STEM_PLATFORM_macosx)
#include "nsopenglshell/NSOpenGLShell.h"
#include "nsopenglshell/NSOpenGLTarget.h"
#elif defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
#include "eaglshell/EAGLShell.h"
#include "eaglshell/EAGLTarget.h"
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
#include "wglshell/WGLShell.h"
#include "wglshell/WGLTarget.h"
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
#include "glxshell/GLXShell.h"
#include "glxshell/GLXTarget.h"
#else
#include "glutshell/GLUTTarget.h"
#endif

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define PROJECTION_FOV 60.0f

static ResourceManager * resourceManager;
static VertexBuffer * vertexBuffer;
static VertexBuffer * armatureVertexBuffer;
static MeshRenderable * renderable;
static MeshRenderable * armatureRenderable;
static Material * material;
static Material * armatureMaterial;
static Armature * armature;
static Animation * animation;
static AnimationState * animationState;
static Renderer * renderer;
static OrbitCamera * camera;
static bool shiftKeyDown, controlKeyDown;
static unsigned int viewWidth = 1280, viewHeight = 720;
static float viewRatio = 16.0f / 9.0f;
static double animationStartTime, lastAnimationTime;
static bool animating;
static bool textureAtlasMode;
static TextureAtlas * textureAtlas;
static unsigned int spriteCount;
static SpriteRenderable ** spriteRenderables;

static bool Target_draw() {
	if (animating && animationState != NULL && animation != NULL) {
		AnimationState_resetAllBones(animationState);
		Animation_poseAnimationStateAtTime(animation, animationState, Shell_getCurrentTime() - animationStartTime, 1.0f);
		AnimationState_computeBoneTransforms(animationState);
		Shell_redisplay();
	}
	
	Renderer_clear(renderer);
	if (textureAtlasMode) {
		Renderer_setViewMatrix(renderer, MATRIX4x4f_IDENTITY);
		Renderer_setDrawMode(renderer, RENDERER_2D_TRANSLUCENT);
	} else {
		Renderer_setViewMatrix(renderer, OrbitCamera_getMatrix(camera));
		Renderer_setDrawMode(renderer, RENDERER_3D_OPAQUE);
	}
	Renderer_drawLayer(renderer, 0, false);
	AutoFreePool_empty();
	return true;
}

static void setMaterialTexture(Material * material, const char * textureResourceName, enum MaterialTextureType textureType, bool magnifyNearest) {
	BitmapImage * image;
	
	image = ResourceManager_referenceResource(resourceManager, ATOM("png"), textureResourceName);
	if (image != NULL) {
		Material_setTexture(material, textureType, magnifyNearest, image->width, image->height, image->pixels);
		ResourceManager_releaseResource(resourceManager, image);
	} else {
		fprintf(stderr, "Couldn't load texture image \"%s\"\n", textureResourceName);
	}
}

static void purgeAllRenderables() {
	Renderer_clearAllRenderables(renderer);
	if (textureAtlas != NULL) {
		TextureAtlas_dispose(textureAtlas);
		textureAtlas = NULL;
	}
	if (spriteRenderables != NULL) {
		unsigned int spriteIndex;
		
		for (spriteIndex = 0; spriteIndex < spriteCount; spriteIndex++) {
			SpriteRenderable_dispose(spriteRenderables[spriteIndex]);
		}
		free(spriteRenderables);
		spriteRenderables = NULL;
		spriteCount = 0;
	}
	if (material != NULL) {
		Material_dispose(material);
		material = NULL;
	}
	if (armature != NULL) {
		ResourceManager_releaseResource(resourceManager, armature);
		armature = NULL;
	}
	if (animation != NULL) {
		Animation_dispose(animation);
		animation = NULL;
	}
	if (animationState != NULL) {
		AnimationState_dispose(animationState);
		animationState = NULL;
	}
	if (renderable != NULL) {
		MeshRenderable_dispose(renderable);
		renderable = NULL;
	}
	if (vertexBuffer != NULL) {
		VertexBuffer_dispose(vertexBuffer);
		vertexBuffer = NULL;
	}
	if (armatureVertexBuffer != NULL ){
		VertexBuffer_dispose(armatureVertexBuffer);
		armatureVertexBuffer = NULL;
	}
	if (armatureRenderable != NULL) {
		MeshRenderable_dispose(armatureRenderable);
		armatureRenderable = NULL;
	}
	ResourceManager_purgeAll(resourceManager);
}

static void useMesh(MeshData * meshData) {
	purgeAllRenderables();
	
	textureAtlasMode = false;
	animating = false;
	Renderer_setProjectionMatrix(renderer, Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, PROJECTION_FOV, viewRatio, 0.02f, 20.0f));
	Renderer_setClearColor(renderer, COLOR4f(0.0f, 0.125f, 0.25f, 0.0f));
	
	if (meshData->materialName != NULL) {
		MaterialData * materialData;
		
		materialData = ResourceManager_referenceResource(resourceManager, ATOM("material"), meshData->materialName);
		if (materialData != NULL) {
			material = MaterialData_createMaterial(materialData);
			if (materialData->colorMapName != NULL) {
				setMaterialTexture(material, materialData->colorMapName, MaterialTextureType_color, materialData->magnifyColorMapNearest);
			}
			if (materialData->normalMapName != NULL) {
				setMaterialTexture(material, materialData->normalMapName, MaterialTextureType_normal, materialData->magnifyNormalMapNearest);
			}
			ResourceManager_releaseResource(resourceManager, materialData);
		} else {
			fprintf(stderr, "Couldn't load material \"%s\" for mesh \"%s\"\n", meshData->materialName, meshData->name);
		}
	}
	
	if (meshData->armatureName != NULL) {
		armature = ResourceManager_referenceResource(resourceManager, ATOM("armature"), meshData->armatureName);
		if (armature != NULL) {
			animationState = AnimationState_create(armature);
			armatureVertexBuffer = Armature_createDebugVertexBuffer(armature);
			armatureRenderable = MeshRenderable_create(armatureVertexBuffer, armatureMaterial, animationState, MATRIX4x4f_IDENTITY);
			armatureRenderable->visible = false;
			Renderer_addRenderable(renderer, 0, (Renderable *) armatureRenderable);
		} else {
			fprintf(stderr, "Couldn't load armature \"%s\" for mesh \"%s\"\n", meshData->armatureName, meshData->name);
		}
	}
	
	if (animationState != NULL) {
		vertexBuffer = VertexBuffer_createPTNXCBW(meshData->vertices, meshData->vertexCount, meshData->indexes, meshData->indexCount, VERTEX_BUFFER_STORAGE_GPU_ONLY, VERTEX_BUFFER_USAGE_STATIC);
	} else {
		vertexBuffer = VertexBuffer_createPTNXC(meshData->vertices, meshData->vertexCount, meshData->indexes, meshData->indexCount, VERTEX_BUFFER_STORAGE_GPU_ONLY, VERTEX_BUFFER_USAGE_STATIC);
	}
	renderable = MeshRenderable_create(vertexBuffer, material, animationState, MATRIX4x4f_IDENTITY);
	Renderer_addRenderable(renderer, 0, (Renderable *) renderable);
	
	Shell_redisplay();
}

static void useAnimation(Animation * animationToUse) {
	if (animation != NULL) {
		Animation_dispose(animation);
	}
	animation = animationToUse;
	animationStartTime = Shell_getCurrentTime();
	animating = true;
	Shell_redisplay();
}

static void updateSpriteLayout() {
	unsigned int spriteIndex;
	unsigned int columnCount;
	float scale;
	unsigned int maxDimension = 1;
	Vector2f pixelSize;
	
	columnCount = ceil(sqrt(spriteCount)); // TODO: Make rectangular; how?
	for (spriteIndex = 0; spriteIndex < spriteCount; spriteIndex++) {
		pixelSize = TextureAtlas_getEntryDimensions(spriteRenderables[spriteIndex]->atlas, spriteRenderables[spriteIndex]->atlasKey, TEXTUREATLAS_SIZE_AUTO, TEXTUREATLAS_SIZE_AUTO);
		if (pixelSize.x > maxDimension) {
			maxDimension = pixelSize.x;
		}
		if (pixelSize.y > maxDimension) {
			maxDimension = pixelSize.y;
		}
	}
	scale = 2.0f / maxDimension / columnCount;
	// TODO: Add some borders?
	for (spriteIndex = 0; spriteIndex < spriteCount; spriteIndex++) {
		pixelSize = TextureAtlas_getEntryDimensions(spriteRenderables[spriteIndex]->atlas, spriteRenderables[spriteIndex]->atlasKey, TEXTUREATLAS_SIZE_AUTO, TEXTUREATLAS_SIZE_AUTO);
		spriteRenderables[spriteIndex]->offset = VECTOR2f(-1.0f + (spriteIndex % columnCount + 1) * 2.0f / columnCount, -1.0f + (spriteIndex / columnCount + 1) * 2.0f / columnCount);
		spriteRenderables[spriteIndex]->size = VECTOR2f(pixelSize.x * scale, pixelSize.y * scale);
	}
}

static void useTextureAtlas(TextureAtlasData * atlasData) {
	BitmapImage * image;
	
	purgeAllRenderables();
	
	textureAtlasMode = true;
	animating = false;
	Renderer_setProjectionMatrix(renderer, Matrix4x4f_ortho(MATRIX4x4f_IDENTITY, -viewRatio, viewRatio, -1.0f, 1.0f, -1.0f, 1.0f));
	Renderer_setClearColor(renderer, COLOR4f(0.0f, 0.125f, 0.0f, 0.0f));
	
	image = ResourceManager_referenceResource(resourceManager, ATOM("png"), atlasData->textureMapName);
	if (image != NULL) {
		unsigned int spriteIndex;
		
		textureAtlas = TextureAtlasData_createTextureAtlas(atlasData);
		TextureAtlas_setTexture(textureAtlas, atlasData->magnifyNearest, image->width, image->height, image->pixels);
		ResourceManager_releaseResource(resourceManager, image);
		
		spriteCount = atlasData->entryCount;
		spriteRenderables = malloc(sizeof(SpriteRenderable *) * spriteCount);
		for (spriteIndex = 0; spriteIndex < spriteCount; spriteIndex++) {
			spriteRenderables[spriteIndex] = SpriteRenderable_create(textureAtlas, atlasData->entries[spriteIndex].name, VECTOR2f(0.5f, 0.5f), VECTOR2f_ZERO, VECTOR2f_ZERO, COLOR4f(1.0f, 1.0f, 1.0f, 1.0f));
			Renderer_addRenderable(renderer, 0, (Renderable *) spriteRenderables[spriteIndex]);
		}
		updateSpriteLayout();
	} else {
		fprintf(stderr, "Couldn't load texture image \"%s\"\n", atlasData->textureMapName);
	}
	
	Shell_redisplay();
}

static DeserializationContext * guessDeserializationType(const char * filePath) {
	FILE * file;
	int firstChar;
	DeserializationContext * context;
	
	file = fopen(filePath, "rb");
	if (file == NULL) {
		fprintf(stderr, "Couldn't open \"%s\"\n", filePath);
		return NULL;
	}
	firstChar = fgetc(file);
	fclose(file);
	
	switch (firstChar) {
		case '{':
			context = (DeserializationContext *) JSONDeserializationContext_createWithFile(filePath);
			if (context->status != SERIALIZATION_ERROR_OK) {
				fprintf(stderr, "File \"%s\" looks like JSON, but JSONDeserializationContext couldn't parse it.\n", filePath);
				context = NULL;
			}
			break;
		case 'S':
		case 'm':
			context = (DeserializationContext *) BinaryDeserializationContext_createWithFile(filePath);
			break;
		default:
			fprintf(stderr, "Couldn't determine serialization type of file \"%s\" (first character was '%c')\n", filePath, firstChar);
			context = NULL;
			break;
	}
	return context;
}

static const char * filePathForResource(Atom resourceName, Atom type) {
	const char * filePath = NULL;
	
	filePath = ResourceManager_resolveFilePath(resourceManager, resourceName);
	if (filePath == NULL) {
		filePath = resourceName;
	}
	return filePath;
}

static void * deserializeFile(const char * filePath, void * (* deserializeFunction)(compat_type(DeserializationContext *))) {
	DeserializationContext * context;
	void * result;
	
	context = guessDeserializationType(filePath);
	if (context == NULL) {
		return NULL;
	}
	result = deserializeFunction(context);
	if (result == NULL) {
		fprintf(stderr, "Couldn't deserialize \"%s\" (error %d)\n", filePath, context->status);
		context->dispose(context);
		return NULL;
	}
	context->dispose(context);
	return result;
}

static void * loadMeshResource(Atom resourceName, void * context) {
	return deserializeFile(filePathForResource(resourceName, ATOM("mesh")), (void * (*)(void *)) MeshData_deserialize);
}

static void unloadMeshResource(void * resource, void * context) {
	MeshData_dispose(resource);
}

static void * loadMaterialResource(Atom resourceName, void * context) {
	return deserializeFile(filePathForResource(resourceName, ATOM("material")), (void * (*)(void *)) MaterialData_deserialize);
}

static void unloadMaterialResource(void * resource, void * context) {
	MaterialData_dispose(resource);
}

void * loadPNGResource(const char * resourceName, void * context) {
	return PNGImageIO_loadPNGFile(filePathForResource(resourceName, ATOM("png")), BITMAP_PIXEL_FORMAT_RGBA_8888, true);
}

void unloadPNGResource(void * resource, void * context) {
	BitmapImage_dispose(resource);
}

static void * loadArmatureResource(Atom resourceName, void * context) {
	return deserializeFile(filePathForResource(resourceName, ATOM("armature")), (void * (*)(void *)) Armature_deserialize);
}

static void unloadArmatureResource(void * resource, void * context) {
	Armature_dispose(resource);
}

static void loadObjFile(const char * filePath) {
	MeshData * meshData;
	
	meshData = Obj3DModelIO_loadFile(filePath);
	useMesh(meshData);
	MeshData_dispose(meshData);
}

static void loadStem3DMesh(const char * filePath) {
	MeshData * meshData = deserializeFile(filePath, (void * (*)(void *)) MeshData_deserialize);
	
	if (meshData != NULL) {
		ResourceManager_addSearchPath(resourceManager, getDirectory(filePath));
		useMesh(meshData);
		MeshData_dispose(meshData);
	}
}

static void loadTextureAtlas(const char * filePath) {
	TextureAtlasData * atlasData = deserializeFile(filePath, (void * (*)(void *)) TextureAtlasData_deserialize);
	
	if (atlasData != NULL) {
		ResourceManager_addSearchPath(resourceManager, getDirectory(filePath));
		useTextureAtlas(atlasData);
		TextureAtlasData_dispose(atlasData);
	}
}

static void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat) {
	switch (keyCode) {
		case KEYBOARD_O: {
			char filePath[PATH_MAX];
			
			if (Shell_openFileDialog(NULL, filePath, PATH_MAX)) {
				const char * fileExtension = getFileExtension(filePath);
				
				if (!strcmp(fileExtension, "obj")) {
					loadObjFile(filePath);
					
				} else if (!strcmp(fileExtension, "mesh")) {
					loadStem3DMesh(filePath);
					
				} else if (!strcmp(fileExtension, "animation")) {
					Animation * loadedAnimation = deserializeFile(filePath, (void * (*)(void *)) Animation_deserialize);
					if (loadedAnimation != NULL) {
						useAnimation(loadedAnimation);
					}
					
				} else if (!strcmp(fileExtension, "atlas")) {
					loadTextureAtlas(filePath);
					
				} else {
					fprintf(stderr, "Didn't know how to handle file extension \"%s\"\n", fileExtension);
				}
			}
			break;
		}
		case KEYBOARD_TAB:
			if (armatureRenderable != NULL) {
				armatureRenderable->visible = !armatureRenderable->visible;
				renderable->visible = !renderable->visible;
				Shell_redisplay();
			}
			break;
		case KEYBOARD_A:
			if (animating) {
				animating = false;
				lastAnimationTime = Shell_getCurrentTime();
			} else {
				animating = true;
				animationStartTime += Shell_getCurrentTime() - lastAnimationTime;
				Shell_redisplay();
			}
			break;
		case KEYBOARD_F:
			if (!textureAtlasMode) {
				OrbitCamera_frameBoundingBox(camera, renderable->vertexBuffer->bounds, PROJECTION_FOV, viewRatio);
				Shell_redisplay();
			}
			break;
		case KEYBOARD_P: {
			if (animationState == NULL) {
				break;
			}
			if (modifiers & MODIFIER_SHIFT_BIT) {
				AnimationState_resetAllBones(animationState);
			} else {
				unsigned int boneIndex;
				float angle;
				Vector3f axis;
				
				for (boneIndex = 0; boneIndex < armature->boneCount; boneIndex++) {
					axis.x = frand(1.0f);
					axis.y = frand(1.0f);
					axis.z = frand(1.0f);
					angle = frand(0.05f);
					Vector3f_normalize(&axis);
					Quaternionf_rotate(&animationState->boneStates[boneIndex].rotation, axis, angle);
				}
			}
			AnimationState_computeBoneTransforms(animationState);
			Shell_redisplay();
			break;
		}
		case KEYBOARD_SPACEBAR:
			animationStartTime = Shell_getCurrentTime();
			Shell_redisplay();
			break;
	}
}

static void Target_keyUp(unsigned int keyCode, unsigned int modifiers) {
}

static void Target_keyModifiersChanged(unsigned int modifiers) {
	shiftKeyDown = !!(modifiers & MODIFIER_SHIFT_BIT);
	controlKeyDown = !!(modifiers & MODIFIER_CONTROL_BIT);
}

static void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	Shell_setMouseDeltaMode(true);
}

static void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
	Shell_setMouseDeltaMode(false);
}

static void Target_mouseMoved(float x, float y) {
}

static void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	if (textureAtlasMode) {
		// ?
	} else {
		if (shiftKeyDown) {
			if (controlKeyDown) {
				OrbitCamera_offset(camera, 0.0f, 0.0f, y);
			} else {
				OrbitCamera_offset(camera, -x, y, 0.0f);
			}
		
		} else if (controlKeyDown) {
			OrbitCamera_zoom(camera, y);
		
		} else {
			OrbitCamera_rotate(camera, x, y);
		}
	}
	Shell_redisplay();
}

static void Target_scrollWheel(int deltaX, int deltaY) {
}

static void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	viewWidth = newWidth;
	viewHeight = newHeight;
	viewRatio = (float) newWidth / newHeight;
	glViewport(0, 0, viewWidth, viewHeight);
	if (renderer != NULL) {
		if (textureAtlasMode) {
			//updateSpriteLayout();
			Renderer_setProjectionMatrix(renderer, Matrix4x4f_ortho(MATRIX4x4f_IDENTITY, -viewRatio, viewRatio, -1.0f, 1.0f, -1.0f, 1.0f));
		} else {
			Renderer_setProjectionMatrix(renderer, Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, PROJECTION_FOV, viewRatio, 0.02f, 20.0f));
		}
	}
}

static void Target_backgrounded() {
}

static void Target_foregrounded() {
}

static void registerShellCallbacks() {
	Shell_drawFunc(Target_draw);
	Shell_resizeFunc(Target_resized);
	Shell_keyDownFunc(Target_keyDown);
	Shell_keyUpFunc(Target_keyUp);
	Shell_keyModifiersChangedFunc(Target_keyModifiersChanged);
	Shell_mouseDownFunc(Target_mouseDown);
	Shell_mouseUpFunc(Target_mouseUp);
	Shell_mouseMovedFunc(Target_mouseMoved);
	Shell_mouseDraggedFunc(Target_mouseDragged);
	Shell_scrollWheelFunc(Target_scrollWheel);
	Shell_backgroundedFunc(Target_backgrounded);
	Shell_foregroundedFunc(Target_foregrounded);
}

#if defined(STEM_PLATFORM_macosx)
void NSOpenGLTarget_configure(int argc, const char ** argv, struct NSOpenGLShellConfiguration * configuration) {
	configuration->windowTitle = "3DModelIO";
	configuration->windowWidth = viewWidth;
	configuration->windowHeight = viewHeight;
	configuration->useGLCoreProfile = true;
#elif defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration) {
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
void WGLTarget_configure(void * instance, void * prevInstance, char * commandLine, int command, int argc, const char ** argv, struct WGLShellConfiguration * configuration) {
	configuration->windowTitle = "3DModelIO";
	configuration->windowWidth = viewWidth;
	configuration->windowHeight = viewHeight;
	configuration->useGLCoreProfile = true;
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
void GLXTarget_configure(int argc, const char ** argv, struct GLXShellConfiguration * configuration) {
	configuration->windowTitle = "3DModelIO";
#else
void GLUTTarget_configure(int argc, const char ** argv, struct GLUTShellConfiguration * configuration) {
	configuration->windowTitle = "3DModelIO";
#endif
	configuration->displayMode.depthBuffer = true;
	registerShellCallbacks();
}

void Target_init() {
	sdrand(time(NULL));
	stirrand(50);
	chdir(Shell_getResourcePath());
	renderer = Renderer_create(1);
	Renderer_setClearColor(renderer, COLOR4f(0.0f, 0.125f, 0.25f, 0.0f));
	Renderer_setLights(renderer, VECTOR3f(0.0f, 8.0f, 8.0f), COLOR4f(1.0f, 1.0f, 0.95f, 1.0f), VECTOR3f(-1.0f, -2.0f, -8.0f), COLOR4f(0.8f, 0.8f, 0.8f, 1.0f), COLOR4f(0.1f, 0.1f, 0.105f, 1.0f));
	Renderer_setProjectionMatrix(renderer, Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, PROJECTION_FOV, viewRatio, 0.02f, 20.0f));
	
	resourceManager = ResourceManager_create(Shell_getCurrentTime);
	ResourceManager_addTypeHandler(resourceManager, ATOM("mesh"), loadMeshResource, unloadMeshResource, PURGE_DEFERRED, NULL);
	ResourceManager_addTypeHandler(resourceManager, ATOM("material"), loadMaterialResource, unloadMaterialResource, PURGE_DEFERRED, NULL);
	ResourceManager_addTypeHandler(resourceManager, ATOM("png"), loadPNGResource, unloadPNGResource, PURGE_DEFERRED, NULL);
	ResourceManager_addTypeHandler(resourceManager, ATOM("armature"), loadArmatureResource, unloadArmatureResource, PURGE_DEFERRED, NULL);
	
	armatureMaterial = Material_create(COLOR4f(1.0f, 1.0f, 0.9375f, 1.0f), 0.875f, 32.0f, 0.0f);
	
	loadObjFile("suzanne.obj");
	camera = OrbitCamera_create();
	
	Shell_mainLoop();
}
