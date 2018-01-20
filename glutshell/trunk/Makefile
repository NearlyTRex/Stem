PROJECT_NAME = glutshell

# VERSION_MAJOR, VERSION_MINOR, and VERSION_TWEAK are available as preprocessor macros for all source files in the project
VERSION_MAJOR = 2
VERSION_MINOR = 1
VERSION_TWEAK = 1
PROJECT_VERSION = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}

LIBRARY_TARGETS = library
EXECUTABLE_TARGETS = 
APPLICATION_TARGETS = testharness

# TARGET_NAME_${target} required for each target of any type; HUMAN_READABLE_TARGET_NAME_${target} required for each application target. Default values for TARGET_NAME_* shown below.
#TARGET_NAME_library = libstem_${PROJECT_NAME}
#TARGET_NAME_unittest = ${PROJECT_NAME}_unittest
#TARGET_NAME_testharness = ${PROJECT_NAME}_testharness
HUMAN_READABLE_TARGET_NAME_testharness = GLUTShell

# Patterns: PLATFORMS, PLATFORMS_${target}
PLATFORMS = macosx linux32 linux64 win32 win64

# Patterns: CCFLAGS, CCFLAGS_${target}, CCFLAGS_${configuration}, CCFLAGS_${platform}, CCFLAGS_${target}_${configuration}, CCFLAGS_${target}_${platform}, CCFLAGS_${configuration}_${platform}, CCFLAGS_${target}_${configuration}_${platform}
CCFLAGS_macosx = -Wno-deprecated-declarations -Wno-int-to-void-pointer-cast
CCFLAGS_linux32 = -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-result -DGLEW_STATIC
CCFLAGS_linux64 = -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-result -DGLEW_STATIC
CCFLAGS_win32 = -DFREEGLUT_STATIC -DGLEW_STATIC
CCFLAGS_win64 = -DFREEGLUT_STATIC -DGLEW_STATIC

# Patterns: LINKFLAGS, LINKFLAGS_${target}, LINKFLAGS_${platform}, LINKFLAGS_${target}_${platform}
LINKFLAGS_library_macosx = -framework OpenGL -framework GLUT -framework CoreFoundation -framework ApplicationServices
LINKFLAGS_library_linux32 = -lrt -lglut -lGLU -lGL -lm -lpthread
LINKFLAGS_library_linux64 = -lrt -lglut -lGLU -lGL -lm -lpthread
LINKFLAGS_library_win32 = -lfreeglut32_static -lopengl32 -lglu32 -lwinmm -lgdi32
LINKFLAGS_library_win64 = -lfreeglut64_static -lopengl32 -lglu32 -lwinmm -lgdi32

# PROJECT_LIBRARY_DEPENDENCIES_* refers to entries in LIBRARY_TARGETS to be linked when building the specified target
# Patterns: PROJECT_LIBRARY_DEPENDENCIES_${target}, PROJECT_LIBRARY_DEPENDENCIES_${target}_${platform}
PROJECT_LIBRARY_DEPENDENCIES_testharness = library

# STEM_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each stem library to be linked when building
# Patterns: STEM_LIBRARY_DEPENDENCIES, STEM_LIBRARY_DEPENDENCIES_${target}, STEM_LIBRARY_DEPENDENCIES_${platform}, STEM_LIBRARY_DEPENDENCIES_${target}_${platform}
STEM_LIBRARY_DEPENDENCIES = \
	shell/latest

# THIRDPARTY_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each thirdparty library to be linked when building
# Patterns: THIRDPARTY_LIBRARY_DEPENDENCIES, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}, THIRDPARTY_LIBRARY_DEPENDENCIES_${platform}, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}_${platform}
THIRDPARTY_LIBRARY_DEPENDENCIES_win32 = glew/1.5.4/libglew.a
THIRDPARTY_LIBRARY_DEPENDENCIES_win64 = glew/1.5.4/libglew.a
THIRDPARTY_LIBRARY_DEPENDENCIES_linux32 = glew/1.5.4/libglew.a
THIRDPARTY_LIBRARY_DEPENDENCIES_linux64 = glew/1.5.4/libglew.a

# Additional build prerequisites per target
# Patterns: PREREQS, PREREQS_${target}
PREREQS = 

SOURCES_library = \
	source/${PROJECT_NAME}/GLUTShell.c \
	source/${PROJECT_NAME}/GLUTShellCallbacks.c

SOURCES_library_macosx  = source/${PROJECT_NAME}/GLUTShellThreading_pthread.c
SOURCES_library_linux32 = source/${PROJECT_NAME}/GLUTShellThreading_pthread.c
SOURCES_library_linux64 = source/${PROJECT_NAME}/GLUTShellThreading_pthread.c
SOURCES_library_win32   = source/${PROJECT_NAME}/GLUTShellThreading_windows.c
SOURCES_library_win64   = source/${PROJECT_NAME}/GLUTShellThreading_windows.c

SOURCES_testharness = \
	source/testharness/TestHarness_main.c

# Public-facing include files to be distributed with the library, if any
INCLUDES = \
	source/${PROJECT_NAME}/GLUTShell.h \
	source/${PROJECT_NAME}/GLUTTarget.h

# Patterns: RESOURCES, RESOURCES_${target}, RESOURCES_${platform}, RESOURCES_${target}_${platform}
RESOURCES = 

# Pattern: ANALYZER_EXCLUDE_SOURCES_${analyzer}
ANALYZER_EXCLUDE_SOURCES_clang = 
ANALYZER_EXCLUDE_SOURCES_splint = \
	source/${PROJECT_NAME}/GLUTShell.c \
	source/testharness/TestHarness_main.c

# Pattern: PLIST_FILE_${target}_${platform}
PLIST_FILE_testharness_macosx = resources/Info_testharness_macosx.plist

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
