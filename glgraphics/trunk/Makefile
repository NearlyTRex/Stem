PROJECT_NAME = glgraphics

# VERSION_MAJOR, VERSION_MINOR, and VERSION_TWEAK are available as preprocessor macros for all source files in the project
VERSION_MAJOR = 2
VERSION_MINOR = 0
VERSION_TWEAK = 0
PROJECT_VERSION = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}

LIBRARY_TARGETS = library
EXECUTABLE_TARGETS = unittest
APPLICATION_TARGETS = testharness

# TARGET_NAME_${target} required for each target of any type; HUMAN_READABLE_TARGET_NAME_${target} required for each application target. Default values for TARGET_NAME_* shown below.
#TARGET_NAME_library = libstem_${PROJECT_NAME}
#TARGET_NAME_unittest = ${PROJECT_NAME}_unittest
#TARGET_NAME_testharness = ${PROJECT_NAME}_testharness
HUMAN_READABLE_TARGET_NAME_testharness = GLGraphics

# Patterns: PLATFORMS, PLATFORMS_${target}
PLATFORMS = macosx iphonesimulator iphoneos linux32 linux64 win32 win64

# Patterns: CCFLAGS, CCFLAGS_${target}, CCFLAGS_${configuration}, CCFLAGS_${platform}, CCFLAGS_${target}_${configuration}, CCFLAGS_${target}_${platform}, CCFLAGS_${configuration}_${platform}, CCFLAGS_${target}_${configuration}_${platform}
CCFLAGS = -I build/intermediate/generated -DUSE_GL3

# Patterns: LINKFLAGS, LINKFLAGS_${target}, LINKFLAGS_${configuration}, LINKFLAGS_${platform}, LINKFLAGS_${target}_${configuration}, LINKFLAGS_${target}_${platform}, LINKFLAGS_${configuration}_${platform}
LINKFLAGS = 

# PROJECT_LIBRARY_DEPENDENCIES_* refers to entries in LIBRARY_TARGETS to be linked when building the specified target
# Patterns: PROJECT_LIBRARY_DEPENDENCIES_${target}, PROJECT_LIBRARY_DEPENDENCIES_${target}_${platform}
PROJECT_LIBRARY_DEPENDENCIES_unittest = library
PROJECT_LIBRARY_DEPENDENCIES_testharness = library

# STEM_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each stem library to be linked when building
# Patterns: STEM_LIBRARY_DEPENDENCIES, STEM_LIBRARY_DEPENDENCIES_${target}, STEM_LIBRARY_DEPENDENCIES_${platform}, STEM_LIBRARY_DEPENDENCIES_${target}_${platform}
STEM_LIBRARY_DEPENDENCIES = \
	gamemath/latest \
	stemobject/latest \
	utilities/latest
STEM_LIBRARY_DEPENDENCIES_unittest = unittest/latest
STEM_LIBRARY_DEPENDENCIES_testharness = shell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_macosx = nsopenglshell/latest

# THIRDPARTY_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each thirdparty library to be linked when building
# Patterns: THIRDPARTY_LIBRARY_DEPENDENCIES, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}, THIRDPARTY_LIBRARY_DEPENDENCIES_${platform}, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}_${platform}
THIRDPARTY_LIBRARY_DEPENDENCIES = 

GENERATED_SOURCES = \
	build/intermediate/generated/${PROJECT_NAME}/StaticVertexShader.h \
	build/intermediate/generated/${PROJECT_NAME}/AnimatedVertexShader.h \
	build/intermediate/generated/${PROJECT_NAME}/LitSurfaceFragmentShader.h

# Additional build prerequisites per target
# Patterns: PREREQS, PREREQS_${target}
PREREQS = ${GENERATED_SOURCES}

SOURCES_library = \
	source/${PROJECT_NAME}/Animation.c \
	source/${PROJECT_NAME}/AnimationState.c \
	source/${PROJECT_NAME}/Armature.c \
	source/${PROJECT_NAME}/GLSLShader.c \
	source/${PROJECT_NAME}/Material.c \
	source/${PROJECT_NAME}/MeshRenderable.c \
	source/${PROJECT_NAME}/OrbitCamera.c \
	source/${PROJECT_NAME}/Renderable.c \
	source/${PROJECT_NAME}/Renderer.c \
	source/${PROJECT_NAME}/VertexBuffer.c

# Executable targets named "unittest" should usually specify only SOURCES_unittest_suites, which must contain only test suite files.
# If files other than test suites are required, specify SOURCES_unittest.
SOURCES_unittest_suites = \
	source/unittest/RenderableTest.c \
	source/unittest/VertexTypesTest.c

SOURCES_testharness = \
	source/testharness/TestHarness_main.c

# Public-facing include files to be distributed with the library, if any
INCLUDES = \
	source/${PROJECT_NAME}/Animation.h \
	source/${PROJECT_NAME}/AnimationState.h \
	source/${PROJECT_NAME}/Armature.h \
	source/${PROJECT_NAME}/GLIncludes.h \
	source/${PROJECT_NAME}/GLSLShader.h \
	source/${PROJECT_NAME}/Material.h \
	source/${PROJECT_NAME}/MeshRenderable.h \
	source/${PROJECT_NAME}/OrbitCamera.h \
	source/${PROJECT_NAME}/Renderable.h \
	source/${PROJECT_NAME}/Renderer.h \
	source/${PROJECT_NAME}/VertexBuffer.h \
	source/${PROJECT_NAME}/VertexTypes.h

# Patterns: RESOURCES, RESOURCES_${target}, RESOURCES_${platform}, RESOURCES_${target}_${platform}
RESOURCES = 

# Pattern: ANALYZER_EXCLUDE_SOURCES_${analyzer}
ANALYZER_EXCLUDE_SOURCES_clang = 
ANALYZER_EXCLUDE_SOURCES_splint = \
	${SOURCES_unittest_suites}

CLANGFLAGS = -I build/intermediate/generated
SPLINTFLAGS = -I build/intermediate/generated

DEPENDENCY_SED_COMMANDS = \
	s,source/${PROJECT_NAME}/DefaultVertexShader,build/intermediate/generated/${PROJECT_NAME}/StaticVertexShader,g; \
	s,source/${PROJECT_NAME}/DefaultVertexShader,build/intermediate/generated/${PROJECT_NAME}/AnimatedVertexShader,g; \
	s,source/${PROJECT_NAME}/DefaultFragmentShader,build/intermediate/generated/${PROJECT_NAME}/LitSurfaceFragmentShader,g

# Pattern: PLIST_FILE_${target}_${platform} (required for application targets on macosx, iphoneos, and iphonesimulator)
PLIST_FILE_testharness_macosx = resources/Info_testharness_macosx.plist
PLIST_FILE_testharness_iphonesimulator = resources/Info_testharness_iphone.plist
PLIST_FILE_testharness_iphoneos = resources/Info_testharness_iphone.plist

INSTALLED_TARGETS = library testharness

# Postamble to invoke stemconfig makefile. Do not modify.
UNAME = ${shell uname}
ifeq (${UNAME},Linux)
STEM_SHARED_DIR ?= /usr/local/stem
else ifeq (${UNAME},Darwin)
STEM_SHARED_DIR ?= /usr/local/stem
else
STEM_SHARED_DIR ?= C:/stem
endif
include ${STEM_SHARED_DIR}/Makefile.global

build/intermediate/generated/${PROJECT_NAME}:
	mkdir -p $@

define generated_header_template #(name)
build/intermediate/generated/${PROJECT_NAME}/$1.h: resources/$2 | build/intermediate/generated/${PROJECT_NAME}
	ruby embed.rb $1 $$^ $$@
endef

${eval ${call generated_header_template,StaticVertexShader,static.vert}}
${eval ${call generated_header_template,AnimatedVertexShader,animated.vert}}
${eval ${call generated_header_template,LitSurfaceFragmentShader,litsurface.frag}}
