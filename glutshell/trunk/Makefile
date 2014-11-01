.PHONY: all
all: library testharness include

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

TARGET_PLATFORMS_macosx = macosx
TARGET_PLATFORMS_linux = linux32 linux64
TARGET_PLATFORMS_windows = win32 win64

PROJECT_NAME = glutshell

LIBRARY_TARGETS = library
EXECUTABLE_TARGETS = 
APPLICATION_TARGETS = testharness
TARGETS = ${LIBRARY_TARGETS} ${EXECUTABLE_TARGETS} ${APPLICATION_TARGETS}
PLATFORMS = ${filter ${TARGET_PLATFORMS_${HOST_PLATFORM}},macosx linux32 linux64 win32 win64}
ANALYZERS = splint clang

TARGET_NAME_library = libstem_${PROJECT_NAME}
TARGET_NAME_testharness = ${PROJECT_NAME}_testharness
HUMAN_READABLE_TARGET_NAME_testharness = GLUTShell

#Per-target configurations
CONFIGURATIONS_library = debug profile release
CONFIGURATIONS_testharness = debug profile release

#Per-target platforms
PLATFORMS_library = ${filter ${PLATFORMS},macosx linux32 linux64 win32 win64}
PLATFORMS_testharness = ${filter ${PLATFORMS},macosx linux32 linux64 win32 win64}

#Per-configuration compile/link settings
CCFLAGS_debug = -g -DDEBUG
CCFLAGS_profile = -g -O3
CCFLAGS_release = -O3

#Per-platform compile/link settings
SDKROOT_macosx ?= /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk
MACOSX_VERSION_MIN ?= 10.6
CC_macosx_i386 ?= ${shell xcrun --sdk ${SDKROOT_macosx} --find cc} -arch i386
CC_macosx_x86_64 ?= ${shell xcrun --sdk ${SDKROOT_macosx} --find cc} -arch x86_64
AR_macosx ?= ${shell xcrun --sdk ${SDKROOT_macosx} --find ar}
RANLIB_macosx ?= ${shell xcrun --sdk ${SDKROOT_macosx} --find ranlib}
SPLINT_macosx ?= /usr/local/bin/splint
CLANG_macosx ?= ${shell xcrun --sdk ${SDKROOT_macosx} --find clang}
ARCHS_macosx ?= i386 x86_64
CCFLAGS_macosx += -isysroot ${SDKROOT_macosx} -mmacosx-version-min=${MACOSX_VERSION_MIN} -Wno-deprecated-declarations -Wno-int-to-void-pointer-cast
LINKFLAGS_macosx += -isysroot ${SDKROOT_macosx} -mmacosx-version-min=${MACOSX_VERSION_MIN} -framework OpenGL -framework GLUT -framework CoreFoundation -framework ApplicationServices

CC_linux32_i386 ?= /usr/bin/gcc
AR_linux32 ?= /usr/bin/ar
RANLIB_linux32 ?= /usr/bin/ranlib
SPLINT_linux32 ?= /usr/local/bin/splint
CLANG_linux32 ?= /usr/local/bin/clang
ARCHS_linux32 ?= i386
CCFLAGS_linux32 += -m32 -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-result
LINKFLAGS_linux32 += -m32 -lrt -lglut -lGLU -lGL -lm

CC_linux64_x86_64 ?= /usr/bin/gcc
AR_linux64 ?= /usr/bin/ar
RANLIB_linux64 ?= /usr/bin/ranlib
SPLINT_linux64 ?= /usr/local/bin/splint
CLANG_linux64 ?= /usr/local/bin/clang
ARCHS_linux64 ?= x86_64
CCFLAGS_linux64 += -m64 -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-result
LINKFLAGS_linux64 += -m64 -lrt -lglut -lGLU -lGL -lm

MINGW_W32_PATH ?= C:/MinGW
MINGW_W32_VERSION ?= 4.6.2
SPLINT_WIN_PATH ?= C:/splint-3.1.1/bin/splint.exe
CLANG_WIN_PATH ?= C:/llvm/bin/clang.exe
CC_win32_i386 ?= ${MINGW_W32_PATH}/bin/gcc.exe
AR_win32 ?= ${MINGW_W32_PATH}/bin/ar.exe
RANLIB_win32 ?= ${MINGW_W32_PATH}/bin/ranlib.exe
SPLINT_win32 ?= ${SPLINT_WIN_PATH}
CLANG_win32 ?= ${CLANG_WIN_PATH}
ARCHS_win32 ?= i386
CCFLAGS_win32 += -DFREEGLUT_STATIC
LINKFLAGS_win32 += -lfreeglut32_static -lopengl32 -lglu32 -lwinmm -lgdi32
EXECUTABLE_SUFFIX_win32 = .exe

MINGW_W64_PATH ?= C:/MinGW-w64
MINGW_W64_VERSION ?= 4.7.0
CC_win64_x86_64 ?= ${MINGW_W64_PATH}/bin/x86_64-w64-mingw32-gcc.exe
AR_win64 ?= ${MINGW_W64_PATH}/bin/x86_64-w64-mingw32-ar.exe
RANLIB_win64 ?= ${MINGW_W64_PATH}/bin/x86_64-w64-mingw32-ranlib.exe
SPLINT_win64 ?= ${SPLINT_WIN_PATH}
CLANG_win64 ?= ${CLANG_WIN_PATH}
ARCHS_win64 ?= x86_64
CCFLAGS_win64 += -DFREEGLUT_STATIC
LINKFLAGS_win64 += -lfreeglut64_static -lopengl32 -lglu32 -lwinmm -lgdi32
EXECUTABLE_SUFFIX_win64 = .exe

#General compile/link settings
DEFINE_CCFLAGS = -DVERSION_MAJOR=${VERSION_MAJOR}u -DVERSION_MINOR=${VERSION_MINOR}u -DVERSION_TWEAK=${VERSION_TWEAK}u
WARNING_CCFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror
OTHER_CCFLAGS = -std=gnu99
CCFLAGS = ${DEFINE_CCFLAGS} ${WARNING_CCFLAGS} ${OTHER_CCFLAGS}

FRAMEWORK_LINKFLAGS = 
LIBRARY_LINKFLAGS = 
OTHER_LINKFLAGS = 
LINKFLAGS = ${FRAMEWORK_LINKFLAGS} ${LIBRARY_LINKFLAGS} ${OTHER_LINKFLAGS}

LINK_ORDER = \
	library \
	glgraphics \
	glew

#Dependencies (can optionally be per-target or per-target-per-platform)

PROJECT_LIBRARY_DEPENDENCIES_testharness = library
STEM_LIBRARY_DEPENDENCIES_library = \
	glgraphics/1.2.2 \
	shell/2.0.0
STEM_LIBRARY_DEPENDENCIES_testharness = \
	glgraphics/1.2.2 \
	shell/2.0.0
THIRDPARTY_LIBRARY_DEPENDENCIES = glew/1.5.4/libglew.a

#Per-target source file lists

SOURCES_library = \
	source/${PROJECT_NAME}/GLUTShell.c \
	source/${PROJECT_NAME}/GLUTShellCallbacks.c

SOURCES_library_macosx          = source/${PROJECT_NAME}/GLUTShellThreading_pthread.c
SOURCES_library_iphoneos        = source/${PROJECT_NAME}/GLUTShellThreading_pthread.c
SOURCES_library_iphonesimulator = source/${PROJECT_NAME}/GLUTShellThreading_pthread.c
SOURCES_library_linux32         = source/${PROJECT_NAME}/GLUTShellThreading_pthread.c
SOURCES_library_linux64         = source/${PROJECT_NAME}/GLUTShellThreading_pthread.c
SOURCES_library_win32           = source/${PROJECT_NAME}/GLUTShellThreading_windows.c
SOURCES_library_win64           = source/${PROJECT_NAME}/GLUTShellThreading_windows.c

SOURCES_testharness = \
	source/testharness/TestHarness_main.c

#Include files to be distributed with library

INCLUDES = \
	source/${PROJECT_NAME}/GLUTShell.h \
	source/${PROJECT_NAME}/GLUTTarget.h

#Target resources

RESOURCES_testharness = 
RESOURCES_testharness_macosx = 
#...

#General analyzer settings
CLANGFLAGS = 
CLANGFLAGS_win32 = -I ${MINGW_W32_PATH}/include -I ${MINGW_W32_PATH}/lib/gcc/mingw32/${MINGW_W32_VERSION}/include
CLANGFLAGS_win64 = -I ${MINGW_W64_PATH}/include -I ${MINGW_W64_PATH}/lib/gcc/mingw32/${MINGW_W64_VERSION}/include
SPLINTFLAGS = -exportlocal

#Source files excluded from static analysis

ANALYZER_EXCLUDE_SOURCES_clang = 
ANALYZER_EXCLUDE_SOURCES_splint = \
	source/${PROJECT_NAME}/GLUTShell.c \
	source/testharness/TestHarness_main.c



define configuration_object_list_template #(target, configuration)
	${foreach platform,${PLATFORMS_$1}, \
		${call platform_object_list_template,$1,$2,${platform}} \
	}
endef

define platform_object_list_template #(target, configuration, platform)
	${foreach arch,${ARCHS_$3}, \
		${call arch_object_list_template,$1,$2,$3,${arch}} \
	}
endef

define arch_object_list_template #(target, configuration, platform, arch)
	${foreach source,${SOURCES_$1} ${SOURCES_$1_$3}, \
		build/intermediate/$1-$2-$3-$4/${notdir ${basename ${source}}}.o \
	}
endef

#Produces OBJECTS_${target}_${configuration} variables for each permutation of target and configuration in that target
${foreach target,${TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${eval OBJECTS_${target}_${configuration} = ${call configuration_object_list_template,${target},${configuration}}} \
	} \
}



define create_directory_target_template #(dir)
.LOW_RESOLUTION_TIME: $1
$1:
	mkdir -p $1
endef

${foreach target,${TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval ${call create_directory_target_template,build/${target}/${configuration}-${platform}}} \
			${foreach arch,${ARCHS_${platform}}, \
				${eval ${call create_directory_target_template,build/intermediate/${target}-${configuration}-${platform}-${arch}}} \
			} \
		} \
	} \
}
${eval ${call create_directory_target_template,build/intermediate}}



define include_ccflags_template #(target, platform)
-I source \
${foreach stem_dependency,${STEM_LIBRARY_DEPENDENCIES} ${STEM_LIBRARY_DEPENDENCIES_$1} ${STEM_LIBRARY_DEPENDENCIES_$2} ${STEM_LIBRARY_DEPENDENCIES_$1_$2},-I ${STEM_SHARED_DIR}/${stem_dependency}/include} \
${foreach thirdparty_dependency,${THIRDPARTY_LIBRARY_DEPENDENCIES} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$1} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$2} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$1_$2},-I ${STEM_SHARED_DIR}/${dir ${thirdparty_dependency}}include}
endef

define define_ccflags_template #(target, configuration, platform, arch)
-DSTEM_TARGET_$1 -DSTEM_CONFIGURATION_$2 -DSTEM_PLATFORM_$3 -DSTEM_ARCH_$4
endef

define dependency_template #(target, configuration, platform, arch, source_file)
build/intermediate/$1-$2-$3-$4/${notdir ${basename $5}}.d: $5 ${PREREQS_$1} | build/intermediate/$1-$2-$3-$4
	@${CC_$3_$4} ${CCFLAGS} ${CCFLAGS_$1} ${CCFLAGS_$2} ${CCFLAGS_$3} ${call include_ccflags_template,$1,$3} ${call define_ccflags_template,$1,$2,$3,$4} -MM -o $$@.temp $5
	@sed 's,\(${notdir ${basename $5}}\)\.o[ :]*,$${basename $$@}.o $${basename $$@}.d: ,g' < $$@.temp > $$@
	@rm $$@.temp
endef

#Produces dependency build targets for all source files in each configuration/platform/arch
ifeq ($(filter clean full_dist commit_dist analyze analyze_clang analyze_splint,${MAKECMDGOALS}),)
${foreach target,${TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${foreach arch,${ARCHS_${platform}}, \
				${foreach source,${SOURCES_${target}} ${SOURCES_${target}_${platform}}, \
					${eval ${call dependency_template,${target},${configuration},${platform},${arch},${source}}} \
					${eval -include build/intermediate/${target}-${configuration}-${platform}-${arch}/${notdir ${basename ${source}}}.d} \
				} \
			} \
		} \
	} \
}
endif



define compile_template #(target, configuration, platform, arch, source_file)
build/intermediate/$1-$2-$3-$4/${notdir ${basename $5}}.o: $5 ${PREREQS_$1} | build/intermediate/$1-$2-$3-$4
	${CC_$3_$4} ${CCFLAGS} ${CCFLAGS_$1} ${CCFLAGS_$2} ${CCFLAGS_$3} ${call include_ccflags_template,$1,$3} ${call define_ccflags_template,$1,$2,$3,$4} -c -o $$@ $5
endef

#Produces object build targets for all source files in each configuration/platform/arch
${foreach target,${TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${foreach arch,${ARCHS_${platform}}, \
				${foreach source,${SOURCES_${target}} ${SOURCES_${target}_${platform}}, \
					${eval ${call compile_template,${target},${configuration},${platform},${arch},${source}}} \
				} \
			} \
		} \
	} \
}



define library_template #(target, configuration, platform, arch, output_file)
build/intermediate/$1-$2-$3-$4/$5: ${call arch_object_list_template,$1,$2,$3,$4}
	${AR_$3} rc $$@ $$^
	${RANLIB_$3} $$@
endef

#Produces static library build targets for each arch/platform/target for library targets
${foreach target,${LIBRARY_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${foreach arch,${ARCHS_${platform}}, \
				${eval ${call library_template,${target},${configuration},${platform},${arch},${TARGET_NAME_${target}}.a}} \
			} \
		} \
	} \
}



define executable_template #(target, configuration, platform, arch, output_file, dependent_libraries)
build/intermediate/$1-$2-$3-$4/$5: ${call arch_object_list_template,$1,$2,$3,$4} $6
	${CC_$3_$4} -o $$@ $$^ ${LINKFLAGS} ${LINKFLAGS_$3}
endef

define library_dependency_template #(target, configuration, platform)
	${foreach link_library,${LINK_ORDER}, \
		${foreach library,${filter ${link_library}%,${PROJECT_LIBRARY_DEPENDENCIES} ${PROJECT_LIBRARY_DEPENDENCIES_$1} ${PROJECT_LIBRARY_DEPENDENCIES_$3} ${PROJECT_LIBRARY_DEPENDENCIES_$1_$3}}, \
			build/${library}/$2-$3/${TARGET_NAME_${library}}.a \
		} \
		${foreach library,${filter ${link_library}%,${STEM_LIBRARY_DEPENDENCIES} ${STEM_LIBRARY_DEPENDENCIES_$1} ${STEM_LIBRARY_DEPENDENCIES_$3} ${STEM_LIBRARY_DEPENDENCIES_$1_$3}}, \
			${STEM_SHARED_DIR}/${library}/library/$2-$3/libstem_${word 1,${subst /, ,${library}}}.a \
		} \
		${foreach library,${filter ${link_library}%,${THIRDPARTY_LIBRARY_DEPENDENCIES} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$1} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$3} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$1_$3}}, \
			${STEM_SHARED_DIR}/${dir ${library}}library/$3/${notdir ${library}} \
		} \
	}
endef

#Produces executable build targets for each arch/platform/target for executable and application targets
${foreach target,${EXECUTABLE_TARGETS} ${APPLICATION_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${foreach arch,${ARCHS_${platform}}, \
				${eval ${call executable_template,${target},${configuration},${platform},${arch},${TARGET_NAME_${target}}${EXECUTABLE_SUFFIX_${platform}},${call library_dependency_template,${target},${configuration},${platform}}}} \
			} \
		} \
	} \
}



define thin_binary_list_template #(target, configuration, platform, target_name)
	${foreach arch,${ARCHS_$3}, \
		build/intermediate/$1-$2-$3-${arch}/$4 \
	}
endef

#Produces THIN_BINARIES_${target}_${configuration}_${platform} variables for each target/configuration/platform for library targets
${foreach target,${LIBRARY_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval THIN_BINARIES_${target}_${configuration}_${platform} = ${call thin_binary_list_template,${target},${configuration},${platform},${TARGET_NAME_${target}}.a}} \
		} \
	} \
}

#Produces THIN_BINARIES_${target}_${configuration}_${platform} variables for each target/configuration/platform for executable targets
${foreach target,${EXECUTABLE_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval THIN_BINARIES_${target}_${configuration}_${platform} = ${call thin_binary_list_template,${target},${configuration},${platform},${TARGET_NAME_${target}}${EXECUTABLE_SUFFIX_${platform}}}} \
		} \
	} \
}

#Produces THIN_BINARIES_${target}_${configuration}_${platform} variables for each target/configuration/platform for application targets
${foreach target,${APPLICATION_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval THIN_BINARIES_${target}_${configuration}_${platform} = ${call thin_binary_list_template,${target},${configuration},${platform},${TARGET_NAME_${target}}${EXECUTABLE_SUFFIX_${platform}}}} \
		} \
	} \
}



define assemble_library_macosx #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}.a: ${THIN_BINARIES_$1_$2_$3} | build/$1/$2-$3
	lipo -create -output $$@ ${THIN_BINARIES_$1_$2_$3}
endef

define assemble_library_linux #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}.a: ${THIN_BINARIES_$1_$2_$3} | build/$1/$2-$3
	cp ${THIN_BINARIES_$1_$2_$3} $$@
endef

define assemble_library_windows #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}.a: ${THIN_BINARIES_$1_$2_$3} | build/$1/$2-$3
	cp ${THIN_BINARIES_$1_$2_$3} $$@
endef

#Produces final library build targets
${foreach target,${LIBRARY_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval ${call assemble_library_${HOST_PLATFORM},${target},${configuration},${platform}}} \
		} \
	} \
}

define copy_target_resources #(target, platform, resources_dir)
	${if ${strip ${RESOURCES_$1} ${RESOURCES_$1_$2}},mkdir -p $3,}
	${foreach resource,${RESOURCES_$1} ${RESOURCES_$1_$2}, \
		cp -r ${resource} $3${newline_and_tab} \
	}
	${if ${strip ${RESOURCES_$1} ${RESOURCES_$1_$2}},find $3 -name .svn -print0 -or -name .DS_Store -print0 | xargs -0 rm -rf}
endef

define assemble_executable_macosx #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_$3} ${RESOURCES_$1} ${RESOURCES_$1_$3} | build/$1/$2-$3
	lipo -create -output $$@ ${THIN_BINARIES_$1_$2_$3}
	${call copy_target_resources,$1,$3,$${dir $$@}}
endef

define assemble_executable_linux #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_$3} ${RESOURCES_$1} ${RESOURCES_$1_$3} | build/$1/$2-$3
	cp ${THIN_BINARIES_$1_$2_$3} $$@
	${call copy_target_resources,$1,$3,$${dir $$@}}
endef

define assemble_executable_windows #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}.exe: ${THIN_BINARIES_$1_$2_$3} ${RESOURCES_$1} ${RESOURCES_$1_$3} | build/$1/$2-$3
	cp ${THIN_BINARIES_$1_$2_$3} $$@
	${call copy_target_resources,$1,$3,$${dir $$@}}
endef

#Produces final executable build targets
${foreach target,${EXECUTABLE_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval ${call assemble_executable_${HOST_PLATFORM},${target},${configuration},${platform}}} \
		} \
	} \
}

PLIST_FILE_testharness_macosx = resources/Info_testharness_macosx.plist

define create_app_bundle #(target, platform, executable_dir, plist_dir, resources_dir)
	mkdir -p $3 $4 $5
	sed -e "s/\$$$${PRODUCT_NAME}/${TARGET_NAME_$1}/g" \
	    -e "s/\$$$${HUMAN_READABLE_PRODUCT_NAME}/${HUMAN_READABLE_TARGET_NAME_$1}/g" \
	    -e "s/\$$$${VERSION}/${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}/g" \
	    -e "s/\$$$${COPYRIGHT_YEAR}/"`date +%Y`"/g" \
	    -e "s/\$$$${BUILD_NUMBER}/0/g" \
	    -e "s/\$$$${PLATFORM_CASED}/${PLIST_PLATFORM_CASED_$2}/g" \
	    -e "s/\$$$${PLATFORM_LOWER}/${PLIST_PLATFORM_LOWER_$2}/g" \
	    -e "s/\$$$${SDK}/${PLIST_SDK_NAME_$2}/g" \
	    ${PLIST_FILE_$1_$2} > $4/Info.plist
	echo "APPL????" > $4/PkgInfo
	${call copy_target_resources,$1,$2,$5}
endef

define assemble_application_macosx #(target, configuration)
build/$1/$2-macosx/$${HUMAN_READABLE_TARGET_NAME_$1}.app/Contents/MacOS/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_macosx} ${RESOURCES_$1} ${RESOURCES_$1_macosx} | build/$1/$2-macosx
	${call create_app_bundle,$1,macosx,build/$1/$2-macosx/$${HUMAN_READABLE_TARGET_NAME_$1}.app/Contents/MacOS,build/$1/$2-macosx/$${HUMAN_READABLE_TARGET_NAME_$1}.app/Contents,build/$1/$2-macosx/$${HUMAN_READABLE_TARGET_NAME_$1}.app/Contents/Resources}
	lipo -create -output "$$@" ${THIN_BINARIES_$1_$2_macosx}
endef

define assemble_application_linux32 #(target, configuration)
build/$1/$2-linux32/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_linux32} ${RESOURCES_$1} ${RESOURCES_$1_linux32} | build/$1/$2-linux32
	${call copy_target_resources,$1,linux32,build/$1/$2-linux32/Resources}
	cp ${THIN_BINARIES_$1_$2_linux32} "$$@"
endef

define assemble_application_linux64 #(target, configuration)
build/$1/$2-linux64/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_linux64} ${RESOURCES_$1} ${RESOURCES_$1_linux64} | build/$1/$2-linux64
	${call copy_target_resources,$1,linux64,build/$1/$2-linux64/Resources}
	cp ${THIN_BINARIES_$1_$2_linux64} "$$@"
endef

define assemble_application_win32 #(target, configuration)
build/$1/$2-win32/${TARGET_NAME_$1}.exe: ${THIN_BINARIES_$1_$2_win32} ${RESOURCES_$1} ${RESOURCES_$1_win32} | build/$1/$2-win32
	${call copy_target_resources,$1,win32,build/$1/$2-win32/Resources}
	cp ${THIN_BINARIES_$1_$2_win32} "$$@"
endef

define assemble_application_win64 #(target, configuration)
build/$1/$2-win64/${TARGET_NAME_$1}.exe: ${THIN_BINARIES_$1_$2_win64} ${RESOURCES_$1} ${RESOURCES_$1_win64} | build/$1/$2-win64
	${call copy_target_resources,$1,win64,build/$1/$2-win64/Resources}
	cp ${THIN_BINARIES_$1_$2_win64} "$$@"
endef

#Produces final application build targets
${foreach target,${APPLICATION_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval ${call assemble_application_${platform},${target},${configuration}}} \
		} \
	} \
}

define library_dependency_template #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}.a
endef

define executable_dependency_template #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}${EXECUTABLE_SUFFIX_$3}
endef

define application_dependency_template #(target, configuration, platform)
build/$1/$2-$3/${call application_file_template_$3,$1}
endef

define application_file_template_macosx #(target)
$${HUMAN_READABLE_TARGET_NAME_$1}.app/Contents/MacOS/${TARGET_NAME_$1}
endef

define application_file_template_iphonesimulator #(target)
${TARGET_NAME_$1}.app/${TARGET_NAME_$1}
endef

define application_file_template_iphoneos #(target)
${TARGET_NAME_$1}.app/${TARGET_NAME_$1}
endef

define application_file_template_linux32 #(target)
${TARGET_NAME_$1}
endef

define application_file_template_linux64 #(target)
${TARGET_NAME_$1}
endef

define application_file_template_win32 #(target)
${TARGET_NAME_$1}.exe
endef

define application_file_template_win64 #(target)
${TARGET_NAME_$1}.exe
endef

define target_template #(target, target_type)
.PHONY: $1
$1: ${foreach configuration,${CONFIGURATIONS_$1},${foreach platform,${PLATFORMS_$1},${call $2_dependency_template,$1,${configuration},${platform}}}}
endef

${foreach target,${LIBRARY_TARGETS}, \
	${eval ${call target_template,${target},library}} \
}

${foreach target,${EXECUTABLE_TARGETS}, \
	${eval ${call target_template,${target},executable}} \
}

${foreach target,${APPLICATION_TARGETS}, \
	${eval ${call target_template,${target},application}} \
}

define analyze_file_template_clang #(target, platform, file)
build/analyzer-results/clang-$1-$2/${basename ${notdir $3}}.txt: $3 ${PREREQS_$1} | build/analyzer-results/clang-$1-$2
	${CLANG_$2} --analyze ${call include_ccflags_template,$1,$2} ${call define_ccflags_template,$1,analyze,$2,none} ${CLANGFLAGS} ${CLANGFLAGS_$1} ${CLANGFLAGS_$2} -o $${basename $$@}.plist $3 > $$@ 2>&1; true
	@cat $$@
endef

define analyze_file_template_splint #(target, platform, file)
build/analyzer-results/splint-$1-$2/${basename ${notdir $3}}.txt: $3 ${PREREQS_$1} | build/analyzer-results/splint-$1-$2
	${SPLINT_$2} ${call include_ccflags_template,$1,$2} ${call define_ccflags_template,$1,analyze,$2,none} ${SPLINTFLAGS} ${SPLINTFLAGS_$1} ${SPLINTFLAGS_$2} $3 > $$@ 2>&1; true
	@cat $$@
endef

define analyzed_sources_template #(analyzer, target, platform)
	${sort ${filter-out ${ANALYZER_EXCLUDE_SOURCES_$1},${SOURCES_$2} ${SOURCES_$2_$3}}}
endef

define analyzer_output_template #(analyzer, target, platform)
	${foreach file,${call analyzed_sources_template,$1,$2,$3}, \
		build/analyzer-results/$1-$2-$3/${basename ${notdir ${file}}}.txt \
	}
endef

define analyze_target_template #(analyzer, target, platform)
.PHONY: analyze_$1_$2_$3
analyze_$1_$2_$3: ${call analyzer_output_template,$1,$2,$3}
endef

define analyze_template #(analyzer)
.PHONY: analyze_$1
analyze_$1: ${foreach target,${TARGETS},${foreach platform,${PLATFORMS_${target}},analyze_$1_${target}_${platform}}}
endef

${foreach analyzer,${ANALYZERS}, \
	${eval ${call analyze_template,${analyzer}}} \
	${foreach target,${TARGETS}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval ${call analyze_target_template,${analyzer},${target},${platform}}} \
			${foreach file,${call analyzed_sources_template,${analyzer},${target},${platform}}, \
				${eval ${call analyze_file_template_${analyzer},${target},${platform},${file}}} \
			} \
		} \
	} \
}

${foreach analyzer,${ANALYZERS}, \
	${foreach target,${TARGETS}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval ${call create_directory_target_template,build/analyzer-results/${analyzer}-${target}-${platform}}} \
		} \
	} \
}

.PHONY: analyze
analyze: ${foreach analyzer,${ANALYZERS},analyze_${analyzer}}

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
	mkdir -p "${INSTALL_DIR}/include" "${INSTALL_DIR}/library" "${INSTALL_DIR}/testharness"
	cp Changes.txt License.txt ReadMe.txt version ${INSTALL_DIR}
	cp -r build/include/* ${INSTALL_DIR}/include
	cp -r build/library/* ${INSTALL_DIR}/library
	cp -r build/testharness/* ${INSTALL_DIR}/testharness
