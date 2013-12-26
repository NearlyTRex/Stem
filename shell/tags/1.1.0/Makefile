.PHONY: all
all: include

UNAME = ${shell uname}
ifeq (${UNAME},Linux)
-include ~/.stem.defines
STEM_SHARED_DIR ?= /usr/local/stem
HOST_PLATFORM = linux
else ifeq (${UNAME},Darwin)
-include ~/.stem.defines
STEM_SHARED_DIR ?= /usr/local/stem
HOST_PLATFORM = macosx
else
STEM_SHARED_DIR ?= C:/stem
-include ${STEM_SHARED_DIR}/stem.defines
HOST_PLATFORM = windows
endif

include version

define newline_and_tab

	
endef

PROJECT_NAME = shell

#Include files to be distributed with library

INCLUDES = \
	source/${PROJECT_NAME}/Shell.h \
	source/${PROJECT_NAME}/ShellBatteryInfo.h \
	source/${PROJECT_NAME}/ShellKeyCodes.h \
	source/${PROJECT_NAME}/ShellThreads.h \
	source/${PROJECT_NAME}/Target.h

define create_directory_target_template #(dir)
.LOW_RESOLUTION_TIME: $1
$1:
	mkdir -p $1
endef

${foreach dir,${sort ${foreach include_file,${INCLUDES},build/include/${notdir ${patsubst %/,%,${dir ${include_file}}}}}}, \
	${eval ${call create_directory_target_template,${dir}}} \
}

.PHONY: include
include: ${INCLUDES} | ${foreach include_file,${INCLUDES},build/include/${notdir ${patsubst %/,%,${dir ${include_file}}}}}
	${foreach include_file,${INCLUDES}, \
		cp ${include_file} build/include/${notdir ${patsubst %/,%,${dir ${include_file}}}}${newline_and_tab} \
	}

.PHONY: clean
clean:
	rm -rf build

INSTALL_DIR = ${STEM_SHARED_DIR}/${PROJECT_NAME}/${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}

.PHONY: install
install:
	mkdir -p "${INSTALL_DIR}/include"
	cp Changes.txt License.txt ReadMe.txt version ${INSTALL_DIR}
	cp -r build/include/* ${INSTALL_DIR}/include
