PROJECT_NAME = glbitmapfont

# VERSION_MAJOR, VERSION_MINOR, and VERSION_TWEAK are available as preprocessor macros for all source files in the project
VERSION_MAJOR = 2
VERSION_MINOR = 1
VERSION_TWEAK = 0
PROJECT_VERSION = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}

LIBRARY_TARGETS = library
EXECUTABLE_TARGETS = unittest
APPLICATION_TARGETS = testharness

# TARGET_NAME_${target} required for each target of any type; HUMAN_READABLE_TARGET_NAME_${target} required for each application target. Default values for TARGET_NAME_* shown below.
#TARGET_NAME_library = libstem_${PROJECT_NAME}
#TARGET_NAME_unittest = ${PROJECT_NAME}_unittest
#TARGET_NAME_testharness = ${PROJECT_NAME}_testharness
HUMAN_READABLE_TARGET_NAME_testharness = GLBitmapFont

# Patterns: PLATFORMS, PLATFORMS_${target}
PLATFORMS = macosx iphonesimulator iphoneos linux32 linux64 win32 win64
PLATFORMS_unittest = macosx linux32 linux64 win32 win64

# Patterns: CCFLAGS, CCFLAGS_${target}, CCFLAGS_${configuration}, CCFLAGS_${platform}, CCFLAGS_${target}_${configuration}, CCFLAGS_${target}_${platform}, CCFLAGS_${configuration}_${platform}, CCFLAGS_${target}_${configuration}_${platform}
CCFLAGS_linux32 = -Wno-unused-result
CCFLAGS_linux64 = -Wno-unused-result

# Patterns: LINKFLAGS, LINKFLAGS_${target}, LINKFLAGS_${configuration}, LINKFLAGS_${platform}, LINKFLAGS_${target}_${configuration}, LINKFLAGS_${target}_${platform}, LINKFLAGS_${configuration}_${platform}
LINKFLAGS = 
LINKFLAGS_unittest_macosx = -framework OpenGL
LINKFLAGS_unittest_win32 = -lopengl32
LINKFLAGS_unittest_win64 = -lopengl32
LINKFLAGS_unittest_linux32 = -lGL
LINKFLAGS_unittest_linux64 = -lGL

# PROJECT_LIBRARY_DEPENDENCIES_* refers to entries in LIBRARY_TARGETS to be linked when building the specified target
# Patterns: PROJECT_LIBRARY_DEPENDENCIES_${target}, PROJECT_LIBRARY_DEPENDENCIES_${target}_${platform}
PROJECT_LIBRARY_DEPENDENCIES_unittest = library
PROJECT_LIBRARY_DEPENDENCIES_testharness = library

# STEM_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each stem library to be linked when building
# Patterns: STEM_LIBRARY_DEPENDENCIES, STEM_LIBRARY_DEPENDENCIES_${target}, STEM_LIBRARY_DEPENDENCIES_${platform}, STEM_LIBRARY_DEPENDENCIES_${target}_${platform}
STEM_LIBRARY_DEPENDENCIES = \
	gamemath/latest \
	glgraphics/latest \
	serialization/latest \
	stemobject/latest
STEM_LIBRARY_DEPENDENCIES_unittest = \
	utilities/latest \
	unittest/latest
STEM_LIBRARY_DEPENDENCIES_testharness = \
	3dmodelio/latest \
	pngimageio/latest \
	bitmapimage/latest \
	jsonserialization/latest \
	jsonio/latest \
	utilities/latest \
	shell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_macosx = nsopenglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_win32 = wglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_win64 = wglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_linux32 = glxshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_linux64 = glxshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_iphonesimulator = eaglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_iphoneos = eaglshell/latest

# THIRDPARTY_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each thirdparty library to be linked when building
# Patterns: THIRDPARTY_LIBRARY_DEPENDENCIES, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}, THIRDPARTY_LIBRARY_DEPENDENCIES_${platform}, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}_${platform}
THIRDPARTY_LIBRARY_DEPENDENCIES = 

# Additional build prerequisites per target
# Patterns: PREREQS, PREREQS_${target}
PREREQS = 

SOURCES_library = \
	source/${PROJECT_NAME}/GLBitmapFont.c \
	source/${PROJECT_NAME}/TextFlow.c

# Executable targets named "unittest" should usually specify only SOURCES_unittest_suites, which must contain only test suite files.
# If files other than test suites are required, specify SOURCES_unittest.
SOURCES_unittest_suites = \
	source/unittest/GLBitmapFontTest.c

SOURCES_testharness = \
	source/testharness/TestHarness_main.c

# Public-facing include files to be distributed with the library, if any
INCLUDES = \
	source/${PROJECT_NAME}/GLBitmapFont.h \
	source/${PROJECT_NAME}/TextFlow.h

# Patterns: RESOURCES, RESOURCES_${target}, RESOURCES_${platform}, RESOURCES_${target}_${platform}
RESOURCES_testharness = \
	resources/test_font_atlas.json \
	resources/test_font_texture.png \
	resources/test_font.json
RESOURCES_testharness_iphonesimulator = \
	resources/basic.frag \
	resources/basic.vert
RESOURCES_testharness_iphoneos = \
	resources/basic.frag \
	resources/basic.vert

# Pattern: ANALYZER_EXCLUDE_SOURCES_${analyzer}
ANALYZER_EXCLUDE_SOURCES_clang = 
ANALYZER_EXCLUDE_SOURCES_splint = \
	source/${PROJECT_NAME}/GLBitmapFont.c \
	source/testharness/TestHarness_main.c \
	${SOURCES_unittest_suites}

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
