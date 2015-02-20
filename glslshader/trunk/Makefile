PROJECT_NAME = glslshader

# VERSION_MAJOR, VERSION_MINOR, and VERSION_TWEAK are available as preprocessor macros for all source files in the project
VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_TWEAK = 2
PROJECT_VERSION = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}

LIBRARY_TARGETS = library
EXECUTABLE_TARGETS = 
APPLICATION_TARGETS = testharness

# TARGET_NAME_${target} required for each target of any type; HUMAN_READABLE_TARGET_NAME_${target} required for each application target. Default values for TARGET_NAME_* shown below.
#TARGET_NAME_library = libstem_${PROJECT_NAME}
#TARGET_NAME_unittest = ${PROJECT_NAME}_unittest
#TARGET_NAME_testharness = ${PROJECT_NAME}_testharness
HUMAN_READABLE_TARGET_NAME_testharness = GLSLShader

# Patterns: PLATFORMS, PLATFORMS_${target}
PLATFORMS = macosx iphonesimulator iphoneos linux32 linux64 win32 win64

# Patterns: CCFLAGS, CCFLAGS_${target}, CCFLAGS_${configuration}, CCFLAGS_${platform}, CCFLAGS_${target}_${configuration}, CCFLAGS_${target}_${platform}, CCFLAGS_${configuration}_${platform}, CCFLAGS_${target}_${configuration}_${platform}
CCFLAGS_testharness_win32 = -DFREEGLUT_STATIC
CCFLAGS_testharness_win64 = -DFREEGLUT_STATIC
CCFLAGS_linux32 = -Wno-unused-result
CCFLAGS_linux64 = -Wno-unused-result

# Patterns: LINKFLAGS, LINKFLAGS_${target}, LINKFLAGS_${platform}, LINKFLAGS_${target}_${platform}
LINKFLAGS = 

# PROJECT_LIBRARY_DEPENDENCIES_* refers to entries in LIBRARY_TARGETS to be linked when building the specified target
# Patterns: PROJECT_LIBRARY_DEPENDENCIES_${target}, PROJECT_LIBRARY_DEPENDENCIES_${target}_${platform}
PROJECT_LIBRARY_DEPENDENCIES_testharness = library

# STEM_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each stem library to be linked when building
# Patterns: STEM_LIBRARY_DEPENDENCIES, STEM_LIBRARY_DEPENDENCIES_${target}, STEM_LIBRARY_DEPENDENCIES_${platform}, STEM_LIBRARY_DEPENDENCIES_${target}_${platform}
STEM_LIBRARY_DEPENDENCIES = \
	glgraphics/latest \
	stemobject/latest \
	utilities/latest

STEM_LIBRARY_DEPENDENCIES_testharness = \
	shell/latest

STEM_LIBRARY_DEPENDENCIES_testharness_macosx = glutshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_iphonesimulator = eaglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_iphoneos = eaglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_win32 = glutshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_win64 = glutshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_linux32 = glutshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_linux64 = glutshell/latest

# THIRDPARTY_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each thirdparty library to be linked when building
# Patterns: THIRDPARTY_LIBRARY_DEPENDENCIES, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}, THIRDPARTY_LIBRARY_DEPENDENCIES_${platform}, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}_${platform}
THIRDPARTY_LIBRARY_DEPENDENCIES = 

# Additional build prerequisites per target
# Patterns: PREREQS, PREREQS_${target}
PREREQS = 

SOURCES_library = \
	source/${PROJECT_NAME}/GLSLShader.c

SOURCES_testharness = \
	source/testharness/TestHarness_main.c

# Public-facing include files to be distributed with the library, if any
INCLUDES = \
	source/${PROJECT_NAME}/GLSLShader.h

# Patterns: RESOURCES, RESOURCES_${target}, RESOURCES_${platform}, RESOURCES_${target}_${platform}
DESKTOP_SHADER_RESOURCES = \
	resources/desktop/shader1.frag \
	resources/desktop/shader1.vert \
	resources/desktop/shader2.frag \
	resources/desktop/shader2.vert
IPHONE_SHADER_RESOURCES = \
	resources/iphone/shader1.frag \
	resources/iphone/shader1.vert \
	resources/iphone/shader2.frag \
	resources/iphone/shader2.vert

RESOURCES_testharness_macosx = ${DESKTOP_SHADER_RESOURCES}
RESOURCES_testharness_iphonesimulator = ${IPHONE_SHADER_RESOURCES}
RESOURCES_testharness_iphoneos = ${IPHONE_SHADER_RESOURCES}
RESOURCES_testharness_win32 = ${DESKTOP_SHADER_RESOURCES}
RESOURCES_testharness_win64 = ${DESKTOP_SHADER_RESOURCES}
RESOURCES_testharness_linux32 = ${DESKTOP_SHADER_RESOURCES}
RESOURCES_testharness_linux64 = ${DESKTOP_SHADER_RESOURCES}

# Pattern: ANALYZER_EXCLUDE_SOURCES_${analyzer}
ANALYZER_EXCLUDE_SOURCES_clang = 
ANALYZER_EXCLUDE_SOURCES_splint = 

# Pattern: PLIST_FILE_${target}_${platform}
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
