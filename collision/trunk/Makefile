PROJECT_NAME = collision

# VERSION_MAJOR, VERSION_MINOR, and VERSION_TWEAK are available as preprocessor macros for all source files in the project
VERSION_MAJOR = 0
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
HUMAN_READABLE_TARGET_NAME_testharness = Collision\ Resolver

# Patterns: PLATFORMS, PLATFORMS_${target}
PLATFORMS = macosx iphonesimulator iphoneos linux32 linux64 win32 win64

# Patterns: CCFLAGS, CCFLAGS_${target}, CCFLAGS_${configuration}, CCFLAGS_${platform}, CCFLAGS_${target}_${configuration}, CCFLAGS_${target}_${platform}, CCFLAGS_${configuration}_${platform}, CCFLAGS_${target}_${configuration}_${platform}
CCFLAGS = 
CCFLAGS_macosx = -Wformat-extra-args
CCFLAGS_win32 = -Wno-unknown-pragmas
CCFLAGS_win64 = -Wno-unknown-pragmas

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
	stemobject/latest

STEM_LIBRARY_DEPENDENCIES_unittest = \
	unittest/latest \
	utilities/latest

STEM_LIBRARY_DEPENDENCIES_testharness = \
	glbitmapfont/latest \
	jsonserialization/latest \
	pngimageio/latest \
	resourcemanager/latest \
	screenmanager/latest

STEM_LIBRARY_DEPENDENCIES_testharness_macosx = nsopenglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_win32 = wglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_win64 = wglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_linux32 = glxshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_linux64 = glxshell/latest

# THIRDPARTY_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each thirdparty library to be linked when building
# Patterns: THIRDPARTY_LIBRARY_DEPENDENCIES, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}, THIRDPARTY_LIBRARY_DEPENDENCIES_${platform}, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}_${platform}
THIRDPARTY_LIBRARY_DEPENDENCIES = 

# Additional build prerequisites per target
# Patterns: PREREQS, PREREQS_${target}
PREREQS = 

SOURCES_library = \
	source/${PROJECT_NAME}/CollisionBox3D.c \
	source/${PROJECT_NAME}/CollisionCapsule.c \
	source/${PROJECT_NAME}/CollisionCircle.c \
	source/${PROJECT_NAME}/CollisionObject.c \
	source/${PROJECT_NAME}/CollisionRecord.c \
	source/${PROJECT_NAME}/CollisionRect2D.c \
	source/${PROJECT_NAME}/CollisionResolver.c \
	source/${PROJECT_NAME}/CollisionSphere.c \
	source/${PROJECT_NAME}/CollisionStaticTrimesh.c \
	source/${PROJECT_NAME}/IntersectionManager.c \
	source/${PROJECT_NAME}/StandardIntersectionHandlers.c

# Executable targets named "unittest" should usually specify only SOURCES_unittest_suites, which must contain only test suite files.
# If files other than test suites are required, specify SOURCES_unittest.
SOURCES_unittest_suites = \
	source/unittest/CollisionBox3DTest.c \
	source/unittest/CollisionCapsuleTest.c \
	source/unittest/CollisionCircleTest.c \
	source/unittest/CollisionObjectTest.c \
	source/unittest/CollisionRecordTest.c \
	source/unittest/CollisionRect2DTest.c \
	source/unittest/CollisionResolverTest.c \
	source/unittest/CollisionSphereTest.c \
	source/unittest/CollisionStaticTrimeshTest.c \
	source/unittest/IntersectionManagerTest.c \
	source/unittest/StandardIntersectionHandlersTest.c

SOURCES_testharness = \
	source/testharness/BouncingBallScreen.c \
	source/testharness/ResourceWiring.c \
	source/testharness/SingleFrameScreen.c \
	source/testharness/TestHarness_globals.c \
	source/testharness/TestHarness_main.c

# Public-facing include files to be distributed with the library, if any
INCLUDES = \
	source/${PROJECT_NAME}/CollisionBox3D.h \
	source/${PROJECT_NAME}/CollisionCapsule.h \
	source/${PROJECT_NAME}/CollisionCircle.h \
	source/${PROJECT_NAME}/CollisionObject.h \
	source/${PROJECT_NAME}/CollisionRecord.h \
	source/${PROJECT_NAME}/CollisionRect2D.h \
	source/${PROJECT_NAME}/CollisionResolver.h \
	source/${PROJECT_NAME}/CollisionShared.h \
	source/${PROJECT_NAME}/CollisionSphere.h \
	source/${PROJECT_NAME}/CollisionStaticTrimesh.h \
	source/${PROJECT_NAME}/IntersectionManager.h \
	source/${PROJECT_NAME}/StandardIntersectionHandlers.h

# Patterns: RESOURCES, RESOURCES_${target}, RESOURCES_${platform}, RESOURCES_${target}_${platform}
RESOURCES_testharness = \
	resources/verdana_atlas.json \
	resources/verdana_font.json \
	resources/verdana_texture.json \
	resources/verdana.png

# Pattern: ANALYZER_EXCLUDE_SOURCES_${analyzer}
ANALYZER_EXCLUDE_SOURCES_clang = 
ANALYZER_EXCLUDE_SOURCES_splint = ${SOURCES_unittest_suites}

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
