PROJECT_NAME = shell

# VERSION_MAJOR, VERSION_MINOR, and VERSION_TWEAK are available as preprocessor macros for all source files in the project
VERSION_MAJOR = 2
VERSION_MINOR = 1
VERSION_TWEAK = 0
PROJECT_VERSION = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}

# Public-facing include files to be distributed with the library, if the project builds one
INCLUDES = \
	source/${PROJECT_NAME}/Shell.h \
	source/${PROJECT_NAME}/ShellBatteryInfo.h \
	source/${PROJECT_NAME}/ShellCallbacks.h \
	source/${PROJECT_NAME}/ShellKeyCodes.h \
	source/${PROJECT_NAME}/ShellThreads.h

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
