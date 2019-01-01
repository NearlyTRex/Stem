#include "gamemath/Matrix4x4f.h"
#include "glgraphics/Animation.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/Material.h"
#include "glgraphics/MeshRenderable.h"
#include "glgraphics/OrbitCamera.h"
#include "glgraphics/Renderer.h"
#include "glgraphics/SpriteRenderable.h"
#include "glgraphics/TextureAtlas.h"
#include "shell/Shell.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"

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

#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define PROJECTION_FOV 60.0f

static Renderer * renderer;
static Material * material;
static Material * armatureMaterial;
static VertexBuffer * vertexBuffer;
static VertexBuffer * armatureVertexBuffer;
static MeshRenderable * renderable;
static MeshRenderable * armatureRenderable;
static TextureAtlas * atlas;
static SpriteRenderable * sprite;
static Armature * armature;
static Animation * animation;
static AnimationState * animationState;
static OrbitCamera * camera;
static bool shiftKeyDown, controlKeyDown;
static unsigned int viewWidth = 1280, viewHeight = 720;
static float viewRatio = 16.0f / 9.0f;
static double animationStartTime, lastAnimationTime;
static bool animating;
static bool spriteMode;

static bool Target_draw() {
	if (animating && animationState != NULL) {
		AnimationState_resetAllBones(animationState);
		Animation_poseAnimationStateAtTime(animation, animationState, Shell_getCurrentTime() - animationStartTime, 1.0f);
		AnimationState_computeBoneTransforms(animationState);
		Shell_redisplay();
	}
	
	Renderer_clear(renderer);
	if (spriteMode) {
		Renderer_setViewMatrix(renderer, MATRIX4x4f_IDENTITY);
		Renderer_setDrawMode(renderer, RENDERER_2D_OPAQUE);
	} else {
		Renderer_setViewMatrix(renderer, OrbitCamera_getMatrix(camera));
		Renderer_setDrawMode(renderer, RENDERER_3D_OPAQUE);
	}
	Renderer_drawLayer(renderer, 0, false);
	return true;
}

static void initScene1() {
	struct vertex_p3f_t2f_n3f_x4f_c4f vertices[] = {
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
	};
	GLuint indexes[] = {
		0, 1, 2
	};
	
	Renderer_clearAllRenderables(renderer);
	Renderer_setClearColor(renderer, COLOR4f(0.0f, 0.125f, 0.25f, 0.0f));
	Renderer_setLights(renderer, VECTOR3f(0.0f, 8.0f, 8.0f), COLOR4f(1.0f, 1.0f, 0.95f, 1.0f), VECTOR3f(-1.0f, -2.0f, -8.0f), COLOR4f(0.8f, 0.8f, 0.8f, 1.0f), COLOR4f(0.1f, 0.1f, 0.105f, 1.0f));
	Renderer_setProjectionMatrix(renderer, Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, PROJECTION_FOV, viewRatio, 0.5f, 100.0f));
	
	if (sprite != NULL) {
		SpriteRenderable_dispose(sprite);
		sprite = NULL;
	}
	spriteMode = false;
	if (renderable != NULL) {
		MeshRenderable_dispose(renderable);
	}
	if (vertexBuffer != NULL) {
		VertexBuffer_dispose(vertexBuffer);
	}
	vertexBuffer = VertexBuffer_createPTNXC(vertices, sizeof(vertices) / sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f), indexes, sizeof(indexes) / sizeof(GLuint), VERTEX_BUFFER_STORAGE_GPU_ONLY, VERTEX_BUFFER_USAGE_STATIC);
	renderable = MeshRenderable_create(vertexBuffer, material, NULL, MATRIX4x4f_IDENTITY);
	Renderer_addRenderable(renderer, 0, (Renderable *) renderable);
	
	if (camera != NULL) {
		OrbitCamera_dispose(camera);
	}
	camera = OrbitCamera_create();
}

static void initScene2() {
	struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f vertices[] = {
		{{0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}}
	};
	GLuint indexes[] = {
		0, 2, 1,
		1, 2, 3
	};
	struct ArmatureBone bones[] = {
		{ATOM("root"), BONE_INDEX_NOT_FOUND, VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(1.0f, 0.0f, 0.0f), QUATERNIONf_IDENTITY},
		{ATOM("boneLower"), 0, VECTOR3f(0.0f, -1.0f, 0.0f), VECTOR3f(1.0f, -1.0f, 0.0f), QUATERNIONf_IDENTITY},
		{ATOM("boneUpper"), 0, VECTOR3f(0.0f, 1.0f, 0.0f), VECTOR3f(1.0f, 1.0f, 0.0f), QUATERNIONf_IDENTITY}
	};
	struct AnimationBoneKeyframe frame1Bones[] = {
		{ATOM("boneLower"), {-1.0f, 0.0f, 0.0f}, {0.5f, 1.0f}, {0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, QUATERNIONf_IDENTITY, {1.0f, 0.0f}, {0.0f, 0.0f}},
		{ATOM("boneUpper"), {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, QUATERNIONf_IDENTITY, {1.0f, 0.0f}, {0.0f, 0.0f}}
	};
	struct AnimationBoneKeyframe frame2Bones[] = {
		{ATOM("boneLower"), {1.0f, 0.0f, 0.0f}, {0.5f, 1.0f}, {0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, QUATERNIONf_IDENTITY, {1.0f, 0.0f}, {0.0f, 0.0f}},
		{ATOM("boneUpper"), {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, QUATERNIONf_IDENTITY, {1.0f, 0.0f}, {0.0f, 0.0f}}
	};
	struct AnimationKeyframe keyframes[] = {
		{1.0f, sizeof(frame1Bones) / sizeof(frame1Bones[0]), frame1Bones},
		{0.5f, sizeof(frame2Bones) / sizeof(frame2Bones[0]), frame2Bones}
	};
	
	Renderer_clearAllRenderables(renderer);
	Renderer_setClearColor(renderer, COLOR4f(0.25f, 0.0f, 0.125f, 0.0f));
	Renderer_setLights(renderer, VECTOR3f(0.0f, 8.0f, 8.0f), COLOR4f(1.0f, 1.0f, 0.95f, 1.0f), VECTOR3f(-1.0f, -2.0f, -8.0f), COLOR4f(0.8f, 0.8f, 0.8f, 1.0f), COLOR4f(0.1f, 0.1f, 0.105f, 1.0f));
	Renderer_setProjectionMatrix(renderer, Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, PROJECTION_FOV, viewRatio, 0.5f, 100.0f));
	
	if (sprite != NULL) {
		SpriteRenderable_dispose(sprite);
		sprite = NULL;
	}
	spriteMode = false;
	if (armature != NULL) {
		Armature_dispose(armature);
	}
	if (animation != NULL) {
		Animation_dispose(animation);
	}
	if (animationState != NULL) {
		AnimationState_dispose(animationState);
	}
	if (vertexBuffer != NULL) {
		VertexBuffer_dispose(vertexBuffer);
	}
	if (renderable != NULL) {
		MeshRenderable_dispose(renderable);
	}
	armature = Armature_create(ATOM("armature"), sizeof(bones) / sizeof(bones[0]), bones);
	animation = Animation_create(ATOM("animation"), true, sizeof(keyframes) / sizeof(keyframes[0]), keyframes, 0, NULL);
	animationState = AnimationState_create(armature);
	Animation_poseAnimationStateAtTime(animation, animationState, 0.0, 1.0f);
	vertexBuffer = VertexBuffer_createPTNXCBW(vertices, sizeof(vertices) / sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f), indexes, sizeof(indexes) / sizeof(GLuint), VERTEX_BUFFER_STORAGE_GPU_ONLY, VERTEX_BUFFER_USAGE_STATIC);
	renderable = MeshRenderable_create(vertexBuffer, material, animationState, MATRIX4x4f_IDENTITY);
	Renderer_addRenderable(renderer, 0, (Renderable *) renderable);
	
	if (armatureVertexBuffer != NULL ){
		VertexBuffer_dispose(armatureVertexBuffer);
	}
	if (armatureRenderable != NULL) {
		MeshRenderable_dispose(armatureRenderable);
	}
	armatureVertexBuffer = Armature_createDebugVertexBuffer(armature);
	armatureRenderable = MeshRenderable_create(armatureVertexBuffer, armatureMaterial, animationState, MATRIX4x4f_IDENTITY);
	armatureRenderable->visible = false;
	Renderer_addRenderable(renderer, 0, (Renderable *) armatureRenderable);
	
	if (camera != NULL) {
		OrbitCamera_dispose(camera);
	}
	camera = OrbitCamera_create();
	animationStartTime = Shell_getCurrentTime();
	animating = true;
}

static void initScene3() {
	struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f vertices[] = {
		{{-1.0f, -4.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, -4.0f, -1.0f},  {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, -4.0f, 1.0f},   {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, -4.0f, 1.0f},  {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{-1.0f, -4.0f, 1.0f},  {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, -2.0f, 1.0f},  {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, -2.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, -4.0f, -1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{-1.0f, -4.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, -2.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, -2.0f, -1.0f},  {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, -4.0f, -1.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{1.0f, -4.0f, -1.0f},  {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, -2.0f, -1.0f},  {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, -2.0f, 1.0f},   {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, -4.0f, 1.0f},   {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{1.0f, -4.0f, 1.0f},   {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, -2.0f, 1.0f},   {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, -2.0f, 1.0f},  {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, -4.0f, 1.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{-1.0f, -1.0f, 1.0f},  {0.0f, 1.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		{{-1.0f, 0.0f, 1.0f},   {0.0f, 2.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, 0.0f, -1.0f},  {1.0f, 2.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, -1.0f, -1.0f}, {1.0f, 1.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		
		{{-1.0f, -1.0f, -1.0f}, {0.0f, 1.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		{{-1.0f, 0.0f, -1.0f},  {0.0f, 2.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, 0.0f, -1.0f},   {1.0f, 2.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, -1.0f, -1.0f},  {1.0f, 1.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		
		{{1.0f, -1.0f, -1.0f},  {0.0f, 1.5f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		{{1.0f, 0.0f, -1.0f},   {0.0f, 2.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, 0.0f, 1.0f},    {1.0f, 2.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, -1.0f, 1.0f},   {1.0f, 1.5f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		
		{{1.0f, -1.0f, 1.0f},   {0.0f, 1.5f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		{{1.0f, 0.0f, 1.0f},    {0.0f, 2.0f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, 0.0f, 1.0f},   {1.0f, 2.0f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, -1.0f, 1.0f},  {1.0f, 1.5f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		
		{{-1.0f, 2.0f, 1.0f},   {0.0f, 3.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 2, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		{{-1.0f, 4.0f, 1.0f},   {0.0f, 4.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, 4.0f, -1.0f},  {1.0f, 4.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, 2.0f, -1.0f},  {1.0f, 3.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 2, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		
		{{-1.0f, 2.0f, -1.0f},  {0.0f, 3.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1, 2, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		{{-1.0f, 4.0f, -1.0f},  {0.0f, 4.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, 4.0f, -1.0f},   {1.0f, 4.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, 2.0f, -1.0f},   {1.0f, 3.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1, 2, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		
		{{1.0f, 2.0f, -1.0f},   {0.0f, 3.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1, 2, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		{{1.0f, 4.0f, -1.0f},   {0.0f, 4.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, 4.0f, 1.0f},    {1.0f, 4.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, 2.0f, 1.0f},    {1.0f, 3.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1, 2, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		
		{{1.0f, 2.0f, 1.0f},    {0.0f, 3.0f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 2, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		{{1.0f, 4.0f, 1.0f},    {0.0f, 4.0f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, 4.0f, 1.0f},   {1.0f, 4.0f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, 2.0f, 1.0f},   {1.0f, 3.0f}, {0.0f, 0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 2, 0, 0}, {0.5f, 0.5f, 0.0f, 0.0f}},
		
		{{1.0f, 4.0f, -1.0f},   {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},  {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, 4.0f, -1.0f},  {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},  {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-1.0f, 4.0f, 1.0f},   {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f},  {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{1.0f, 4.0f, 1.0f},    {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f},  {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f}, {2, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}}
	};
	GLuint indexes[] = {
		0, 1, 2, 2, 3, 0,
		
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		
		5, 20, 23, 23, 6, 5,
		9, 24, 27, 27, 10, 9,
		13, 28, 31, 31, 14, 13,
		17, 32, 35, 35, 18, 17,
		
		20, 21, 22, 22, 23, 20,
		24, 25, 26, 26, 27, 24,
		28, 29, 30, 30, 31, 28,
		32, 33, 34, 34, 35, 32,
		
		21, 36, 39, 39, 22, 21,
		25, 40, 43, 43, 26, 25,
		29, 44, 47, 47, 30, 29,
		33, 48, 51, 51, 34, 33,
		
		36, 37, 38, 38, 39, 36,
		40, 41, 42, 42, 43, 40,
		44, 45, 46, 46, 47, 44,
		48, 49, 50, 50, 51, 48,
		
		52, 53, 54, 54, 55, 52
	};
	struct ArmatureBone bones[] = {
		{ATOM("root"), BONE_INDEX_NOT_FOUND, VECTOR3f(0.0f, -4.0f, 0.0f), VECTOR3f(0.0f, -1.0f, 0.0f), QUATERNIONf_IDENTITY},
		{ATOM("joint1"), 0, VECTOR3f(0.0f, -1.0f, 0.0f), VECTOR3f(0.0f, 2.0f, 0.0f), QUATERNIONf_IDENTITY},
		{ATOM("joint2"), 1, VECTOR3f(0.0f, 2.0f, 0.0f), VECTOR3f(0.0f, 4.0f, 0.0f), Quaternionf_fromAxisAngle(VECTOR3f(0.0f, 1.0f, 0.0f), M_PI * 0.5)}
	};
	struct AnimationBoneKeyframe frame1Bones[] = {
		{ATOM("joint1"), {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, QUATERNIONf_IDENTITY, {0.5f, 1.0f}, {0.5f, 0.0f}},
		{ATOM("joint2"), {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, QUATERNIONf_IDENTITY, {0.5f, 1.0f}, {0.5f, 0.0f}}
	};
	struct AnimationBoneKeyframe frame2Bones[] = {
		{ATOM("joint1"), {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, Quaternionf_fromAxisAngle(VECTOR3f(0.0f, 0.0f, 1.0f), M_PI * 0.25f), {0.5f, 1.0f}, {0.5f, 0.0f}},
		{ATOM("joint2"), {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, Quaternionf_fromAxisAngle(VECTOR3f(0.0f, 0.0f, 1.0f), M_PI * 0.25f), {0.5f, 1.0f}, {0.5f, 0.0f}}
	};
	struct AnimationKeyframe keyframes[] = {
		{0.5f, sizeof(frame1Bones) / sizeof(frame1Bones[0]), frame1Bones},
		{0.5f, sizeof(frame2Bones) / sizeof(frame2Bones[0]), frame2Bones}
	};
	
	Renderer_clearAllRenderables(renderer);
	Renderer_setClearColor(renderer, COLOR4f(0.125f, 0.0625f, 0.0f, 0.0f));
	Renderer_setLights(renderer, VECTOR3f(2.0f, 8.0f, 8.0f), COLOR4f(1.0f, 1.0f, 0.95f, 1.0f), VECTOR3f(-3.0f, -2.0f, -8.0f), COLOR4f(0.8f, 0.8f, 0.8f, 1.0f), COLOR4f(0.1f, 0.1f, 0.105f, 1.0f));
	Renderer_setProjectionMatrix(renderer, Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, PROJECTION_FOV, viewRatio, 0.5f, 100.0f));
	
	if (sprite != NULL) {
		SpriteRenderable_dispose(sprite);
		sprite = NULL;
	}
	spriteMode = false;
	if (armature != NULL) {
		Armature_dispose(armature);
	}
	if (animation != NULL) {
		Animation_dispose(animation);
	}
	if (animationState != NULL) {
		AnimationState_dispose(animationState);
	}
	if (vertexBuffer != NULL) {
		VertexBuffer_dispose(vertexBuffer);
	}
	if (renderable != NULL) {
		MeshRenderable_dispose(renderable);
	}
	armature = Armature_create(ATOM("armature"), sizeof(bones) / sizeof(bones[0]), bones);
	animation = Animation_create(ATOM("animation"), true, sizeof(keyframes) / sizeof(keyframes[0]), keyframes, 0, NULL);
	animationState = AnimationState_create(armature);
	Animation_poseAnimationStateAtTime(animation, animationState, 0.0, 1.0f);
	vertexBuffer = VertexBuffer_createPTNXCBW(vertices, sizeof(vertices) / sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f), indexes, sizeof(indexes) / sizeof(GLuint), VERTEX_BUFFER_STORAGE_GPU_ONLY, VERTEX_BUFFER_USAGE_STATIC);
	renderable = MeshRenderable_create(vertexBuffer, material, animationState, MATRIX4x4f_IDENTITY);
	Renderer_addRenderable(renderer, 0, (Renderable *) renderable);
	
	if (armatureVertexBuffer != NULL ){
		VertexBuffer_dispose(armatureVertexBuffer);
	}
	if (armatureRenderable != NULL) {
		MeshRenderable_dispose(armatureRenderable);
	}
	armatureVertexBuffer = Armature_createDebugVertexBuffer(armature);
	armatureRenderable = MeshRenderable_create(armatureVertexBuffer, armatureMaterial, animationState, MATRIX4x4f_IDENTITY);
	armatureRenderable->visible = false;
	Renderer_addRenderable(renderer, 0, (Renderable *) armatureRenderable);
	
	if (camera != NULL) {
		OrbitCamera_dispose(camera);
	}
	camera = OrbitCamera_create();
	camera->cameraDistance = 10.0f;
	animationStartTime = Shell_getCurrentTime();
	animating = true;
}

static void initScene4() {
	Renderer_clearAllRenderables(renderer);
	Renderer_setClearColor(renderer, COLOR4f(0.0f, 0.125f, 0.0f, 0.0f));
	Renderer_setProjectionMatrix(renderer, Matrix4x4f_ortho(MATRIX4x4f_IDENTITY, -viewRatio, viewRatio, -1.0f, 1.0f, -1.0f, 1.0f));
	
	if (renderable != NULL) {
		MeshRenderable_dispose(renderable);
		renderable = NULL;
	}
	if (vertexBuffer != NULL) {
		VertexBuffer_dispose(vertexBuffer);
		vertexBuffer = NULL;
	}
	if (sprite != NULL) {
		SpriteRenderable_dispose(sprite);
	}
	spriteMode = true;
	sprite = SpriteRenderable_create(atlas, "key1", VECTOR2f(0.5f, 0.5f), VECTOR2f_ZERO, VECTOR2f(TEXTUREATLAS_SIZE_AUTO, 1.0f), COLOR4f(1.0f, 1.0f, 1.0f, 1.0f));
	Renderer_addRenderable(renderer, 0, (Renderable *) sprite);
}

static void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat) {
	switch (keyCode) {
		case KEYBOARD_1:
			initScene1();
			Shell_redisplay();
			break;
		case KEYBOARD_2:
			initScene2();
			Shell_redisplay();
			break;
		case KEYBOARD_3:
			initScene3();
			Shell_redisplay();
			break;
		case KEYBOARD_4:
			initScene4();
			Shell_redisplay();
			break;
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
			if (renderable != NULL) {
				OrbitCamera_frameBoundingBox(camera, renderable->vertexBuffer->bounds, PROJECTION_FOV, viewRatio);
				Shell_redisplay();
			}
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

#define SPRITE_DRAG_RATIO 0.00625f

static void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	if (spriteMode) {
		sprite->offset.x += x * SPRITE_DRAG_RATIO;
		sprite->offset.y -= y * SPRITE_DRAG_RATIO;
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
		if (spriteMode) {
			Renderer_setProjectionMatrix(renderer, Matrix4x4f_ortho(MATRIX4x4f_IDENTITY, -viewRatio, viewRatio, -1.0f, 1.0f, -1.0f, 1.0f));
		} else {
			Renderer_setProjectionMatrix(renderer, Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, PROJECTION_FOV, viewRatio, 0.5f, 100.0f));
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
	configuration->windowTitle = "GLGraphics";
	configuration->useGLCoreProfile = true;
#elif defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration) {
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
void WGLTarget_configure(void * instance, void * prevInstance, char * commandLine, int command, int argc, const char ** argv, struct WGLShellConfiguration * configuration) {
	configuration->windowTitle = "GLGraphics";
	configuration->useGLCoreProfile = true;
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
void GLXTarget_configure(int argc, const char ** argv, struct GLXShellConfiguration * configuration) {
	configuration->windowTitle = "GLGraphics";
#else
void GLUTTarget_configure(int argc, const char ** argv, struct GLUTShellConfiguration * configuration) {
	configuration->windowTitle = "GLGraphics";
#endif
	configuration->displayMode.depthBuffer = true;
	registerShellCallbacks();
}

void Target_init() {
	unsigned char checkerboardPixels[] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
		0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};
	unsigned char atlasPixels[] = {
		0xFF, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
		0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF
	};
	
	chdir(Shell_getResourcePath());
	
	renderer = Renderer_create(1);
	material = Material_create(COLOR4f(1.0f, 1.0f, 1.0f, 1.0f), 0.875f, 32.0f, 0.0f);
	Material_setTexture(material, MaterialTextureType_color, true, 2, 2, checkerboardPixels);
	armatureMaterial = Material_create(COLOR4f(1.0f, 1.0f, 0.9375f, 1.0f), 0.875f, 32.0f, 0.0f);
	
	atlas = TextureAtlas_create();
	TextureAtlas_setEntry(atlas, "key1", RECT4f(0.0f, 0.75f, 0.0f, 0.75f));
	TextureAtlas_setEntry(atlas, "key2", RECT4f(0.25f, 1.0f, 0.0f, 0.75f));
	TextureAtlas_setEntry(atlas, "key3", RECT4f(0.0f, 0.75f, 0.25f, 1.0f));
	TextureAtlas_setEntry(atlas, "key4", RECT4f(0.25f, 1.0f, 0.25f, 1.0f));
	TextureAtlas_setTexture(atlas, true, 2, 2, atlasPixels);
	
	initScene1();
	Shell_mainLoop();
}
