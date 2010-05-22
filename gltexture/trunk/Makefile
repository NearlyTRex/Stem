.PHONY: all
all: library unittest testharness test include

UNAME = ${shell uname}
ifeq (${UNAME},Linux)
HOST_PLATFORM = linux
else ifeq (${UNAME},Darwin)
HOST_PLATFORM = macosx
else
HOST_PLATFORM = windows
endif

define newline_and_tab

	
endef

TARGET_PLATFORMS_macosx = macosx iphonesimulator iphoneos
TARGET_PLATFORMS_linux = linux
TARGET_PLATFORMS_windows = windows

include version

PROJECT_NAME = gltexture
IPHONE_SDK_VERSION = 3.1
IPHONESIMULATOR_APPLICATIONS_DIR = ${HOME}/Library/Application Support/iPhone Simulator/3.1.3/Applications
RESOURCE_RULES_PLIST = /Developer/Platforms/MacOSX.platform/ResourceRules.plist
IPHONESIMULATOR_PATH = "/Developer/Platforms/iPhoneSimulator.platform/Developer/Applications/iPhone Simulator.app"
CODESIGN_IDENTITY = "iPhone Developer"
SVNROOT = http://sacredsoftware.net/svn/misc#master_source_only

LIBRARY_TARGETS = library
EXECUTABLE_TARGETS = unittest
APPLICATION_TARGETS = testharness
TARGETS = ${LIBRARY_TARGETS} ${EXECUTABLE_TARGETS} ${APPLICATION_TARGETS}
CONFIGURATIONS = debug profile release
PLATFORMS = ${filter ${TARGET_PLATFORMS_${HOST_PLATFORM}},macosx iphonesimulator iphoneos linux windows}
ARCHS = ppc i386 i686 x86_64 armv6 armv7
ANALYZERS = splint clang

TARGET_NAME_library = libstem_gltexture
TARGET_NAME_unittest = gltexture_unittest
TARGET_NAME_testharness = gltexture_testharness
HUMAN_READABLE_TARGET_NAME_testharness = GLTexture

#Per-target configurations
CONFIGURATIONS_library = debug profile release
CONFIGURATIONS_unittest = debug
CONFIGURATIONS_testharness = debug profile

#Per-target platforms
PLATFORMS_library = ${filter ${PLATFORMS},macosx iphonesimulator iphoneos linux windows}
PLATFORMS_unittest = ${filter ${PLATFORMS},macosx iphonesimulator linux windows}
PLATFORMS_testharness = ${filter ${PLATFORMS},macosx iphonesimulator iphoneos linux windows}

#Per-target compile/link settings
CCFLAGS_unittest = -I test_source -include build/intermediate/TestList.h

#Per-target analyzer settings
CLANGFLAGS_unittest = ${CCFLAGS_unittest}
SPLINTFLAGS_unittest = ${CCFLAGS_unittest}

#Per-configuration compile/link settings
CCFLAGS_debug = -g
CCFLAGS_profile = -g -O3
CCFLAGS_release = -O3

#Per-platform compile/link settings
CC_macosx_ppc = /usr/bin/gcc-4.2 -arch ppc
CC_macosx_i386 = /usr/bin/clang -arch i386
CC_macosx_x86_64 = /usr/bin/clang -arch x86_64
AR_macosx = /usr/bin/ar
RANLIB_macosx = /usr/bin/ranlib
SPLINT_macosx = /usr/local/bin/splint
CLANG_macosx = /usr/bin/clang
SDKROOT_macosx = /Developer/SDKs/MacOSX10.5.sdk
ARCHS_macosx = ppc i386 x86_64
CCFLAGS_macosx = -isysroot ${SDKROOT_macosx} -mmacosx-version-min=10.5
LINKFLAGS_macosx = -isysroot ${SDKROOT_macosx} -mmacosx-version-min=10.5 -framework OpenGL -framework GLUT -framework CoreFoundation -lz

CC_iphonesimulator_i386 = /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/gcc-4.2 -arch i386
AR_iphonesimulator = /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/ar
RANLIB_iphonesimulator = /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/ranlib
SPLINT_iphonesimulator = /usr/local/bin/splint
CLANG_iphonesimulator = /usr/bin/clang
SDKROOT_iphonesimulator = /Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator${IPHONE_SDK_VERSION}.sdk
ARCHS_iphonesimulator = i386
CCFLAGS_iphonesimulator = -isysroot ${SDKROOT_iphonesimulator} -mmacosx-version-min=10.5 -D__IPHONE_OS_VERSION_MIN_REQUIRED=31000
LINKFLAGS_iphonesimulator = -isysroot ${SDKROOT_iphonesimulator} -mmacosx-version-min=10.5 -framework Foundation -framework UIKit -framework QuartzCore -framework OpenGLES -framework CoreFoundation -lz

CC_iphoneos_armv6 = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2 -arch armv6
CC_iphoneos_armv7 = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2 -arch armv7
AR_iphoneos = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ar
RANLIB_iphoneos = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ranlib
SPLINT_iphoneos = /usr/local/bin/splint
CLANG_iphoneos = /usr/bin/clang
SDKROOT_iphoneos = /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS${IPHONE_SDK_VERSION}.sdk
ARCHS_iphoneos = armv6 armv7
CCFLAGS_iphoneos = -isysroot ${SDKROOT_iphoneos} -miphoneos-version-min=${IPHONE_SDK_VERSION}
LINKFLAGS_iphoneos = -isysroot ${SDKROOT_iphoneos} -miphoneos-version-min=${IPHONE_SDK_VERSION} -framework Foundation -framework UIKit -framework QuartzCore -framework OpenGLES -framework CoreFoundation -lz

CC_linux_i686 = /usr/bin/gcc
AR_linux = /usr/bin/ar
RANLIB_linux = /usr/bin/ranlib
SPLINT_linux = /usr/local/bin/splint
CLANG_linux = /usr/local/bin/clang
ARCHS_linux = i686
CCFLAGS_linux = 
LINKFLAGS_linux = -lm -ldl -Wl,-E -lGL -lz

CC_windows_i686 = C:/MinGW/bin/gcc.exe
AR_windows = C:/MinGW/bin/ar.exe
RANLIB_windows = C:/MinGW/bin/ranlib.exe
SPLINT_windows = C:/splint-3.1.1/bin/splint.exe
CLANG_windows = C:/llvm/bin/clang.exe
ARCHS_windows = i686
CCFLAGS_windows = -DFREEGLUT_STATIC
LINKFLAGS_windows = -lfreeglut_static -lopengl32 -lglu32 -lwinmm -lgdi32 -mwindows -mconsole

#General compile/link settings
DEFINE_CCFLAGS = -DVERSION_MAJOR=${VERSION_MAJOR}u -DVERSION_MINOR=${VERSION_MINOR}u -DVERSION_TWEAK=${VERSION_TWEAK}u
WARNING_CCFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror
INCLUDE_CCFLAGS = -I source -I include -I include/glew
OTHER_CCFLAGS = -std=gnu99
CCFLAGS = ${DEFINE_CCFLAGS} ${WARNING_CCFLAGS} ${INCLUDE_CCFLAGS} ${OTHER_CCFLAGS}

FRAMEWORK_LINKFLAGS = 
LIBRARY_LINKFLAGS = 
OTHER_LINKFLAGS = 
LINKFLAGS = ${FRAMEWORK_LINKFLAGS} ${LIBRARY_LINKFLAGS} ${OTHER_LINKFLAGS}

#General analyzer settings
CLANGFLAGS = 
CLANGFLAGS_windows = -I C:/MinGW/include -I C:/MinGW/lib/gcc/mingw32/4.3.2/include
SPLINTFLAGS = 

#Per-target dependencies

PROJECT_LIBRARY_DEPENDENCIES_unittest = library
PROJECT_LIBRARY_DEPENDENCIES_testharness = library
STEM_LIBRARY_DEPENDENCIES_unittest = glgraphics/libstem_glgraphics_noglew.a 3dmath/libstem_3dmath.a serialization/libstem_serialization.a stemobject/libstem_stemobject.a
STEM_LIBRARY_DEPENDENCIES_testharness = 3dmath/libstem_3dmath.a pngimageio/libstem_pngimageio.a bitmapimage/libstem_bitmapimage.a serialization/libstem_serialization.a utilities/libstem_utilities.a stemobject/libstem_stemobject.a
THIRDPARTY_LIBRARY_DEPENDENCIES_unittest = 
THIRDPARTY_LIBRARY_DEPENDENCIES_testharness = libpng/libpng.a

#Per-target per-platform dependencies
STEM_LIBRARY_DEPENDENCIES_testharness_macosx = glutshell/libstemshell_glut.a glgraphics/libstem_glgraphics.a
STEM_LIBRARY_DEPENDENCIES_testharness_windows = glutshell/libstemshell_glut.a glgraphics/libstem_glgraphics.a
STEM_LIBRARY_DEPENDENCIES_testharness_linux = glutshell/libstemshell_glut.a glgraphics/libstem_glgraphics.a
STEM_LIBRARY_DEPENDENCIES_testharness_iphonesimulator = eaglshell/libstemshell_eagl.a glgraphics/libstem_glgraphics_noglew.a
STEM_LIBRARY_DEPENDENCIES_testharness_iphoneos = eaglshell/libstemshell_eagl.a glgraphics/libstem_glgraphics_noglew.a
THIRDPARTY_LIBRARY_DEPENDENCIES_testharness_macosx = zlib/libz.a glew/libglew.a
THIRDPARTY_LIBRARY_DEPENDENCIES_testharness_windows = zlib/libz.a glew/libglew.a
THIRDPARTY_LIBRARY_DEPENDENCIES_testharness_linux = glew/libglew.a

#Per-target source file lists

SOURCES_library = \
	source/gltexture/GLTexture.c

SOURCES_unittest = \
	test_source/unittest/framework/unittest_main.c \
	test_source/unittest/framework/TestList.c \
	test_source/unittest/TestDeserializationContext.c \
	test_source/unittest/TestSerializationContext.c \
	${SOURCES_unittest_suites}

SOURCES_unittest_suites = \
	test_source/unittest/suites/GLTextureTest.c

SOURCES_testharness = \
	test_source/testharness/TestHarness_main.c

INCLUDES = \
	source/gltexture/GLTexture.h

RESOURCES_testharness = \
	test_resources/basn0g01.png \
	test_resources/basn2c16.png \
	test_resources/basn4a16.png \
	test_resources/basn6a16.png \
	test_resources/reference3x2gray.png
RESOURCES_unittest = 

ANALYZER_EXCLUDE_SOURCES_clang = 
ANALYZER_EXCLUDE_SOURCES_splint = \
	source/gltexture/GLTexture.c \
	test_source/testharness/TestHarness_main.c \
	${SOURCES_unittest}

PREREQS_unittest = build/intermediate/TestList.h

build/intermediate/TestList.h:
	mkdir -p build/intermediate
	echo '#define SUITE_FILE_LIST ${foreach file,${SOURCES_unittest_suites},"${basename ${notdir ${file}}}",} NULL' > $@



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
	${foreach source,${SOURCES_$1}, \
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



define compile_template #(target, configuration, platform, arch, source_file)
build/intermediate/$1-$2-$3-$4/${notdir ${basename $5}}.o: $5 ${PREREQS_$1} | build/intermediate/$1-$2-$3-$4
	${CC_$3_$4} ${CCFLAGS} ${CCFLAGS_$1} ${CCFLAGS_$2} ${CCFLAGS_$3} -c -o $$@ $5
endef

#Produces object build targets for all source files in each configuration/platform/arch
${foreach target,${TARGETS}, \
	${foreach configuration,${CONFIGURATIONS}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${foreach arch,${ARCHS_${platform}}, \
				${foreach source,${SOURCES_${target}}, \
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
	${foreach library,${PROJECT_LIBRARY_DEPENDENCIES_$1} ${PROJECT_LIBRARY_DEPENDENCIES_$1_$3}, \
		build/${library}/$2-$3/${TARGET_NAME_${library}}.a \
	} \
	${foreach library,${STEM_LIBRARY_DEPENDENCIES_$1} ${STEM_LIBRARY_DEPENDENCIES_$1_$3}, \
		lib/${dir ${library}}$2-$3/${notdir ${library}} \
	} \
	${foreach library,${THIRDPARTY_LIBRARY_DEPENDENCIES_$1} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$1_$3}, \
		lib/${dir ${library}}$3/${notdir ${library}} \
	}
endef

#Produces executable build targets for each arch/platform/target for executable and application targets
${foreach target,${EXECUTABLE_TARGETS} ${APPLICATION_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${foreach arch,${ARCHS_${platform}}, \
				${eval ${call executable_template,${target},${configuration},${platform},${arch},${TARGET_NAME_${target}},${call library_dependency_template,${target},${configuration},${platform}}}} \
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
			${eval THIN_BINARIES_${target}_${configuration}_${platform} = ${call thin_binary_list_template,${target},${configuration},${platform},${TARGET_NAME_${target}}}} \
		} \
	} \
}

#Produces THIN_BINARIES_${target}_${configuration}_${platform} variables for each target/configuration/platform for application targets
${foreach target,${APPLICATION_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval THIN_BINARIES_${target}_${configuration}_${platform} = ${call thin_binary_list_template,${target},${configuration},${platform},${TARGET_NAME_${target}}}} \
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

define copy_target_resources #(target, resources_dir)
	${foreach resource,${RESOURCES_$1}, \
		cp -r ${resource} $2${newline_and_tab} \
	}
	find $2 -name .svn -print0 | xargs -0 rm -rf
endef

define assemble_executable_macosx #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_$3} ${RESOURCES_$1} | build/$1/$2-$3
	lipo -create -output $$@ ${THIN_BINARIES_$1_$2_$3}
	${call copy_target_resources,$1,$${dir $$@}}
endef

define assemble_executable_linux #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_$3} ${RESOURCES_$1} | build/$1/$2-$3
	cp ${THIN_BINARIES_$1_$2_$3} $$@
	${call copy_target_resources,$1,$${dir $$@}}
endef

define assemble_executable_windows #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}.exe: ${THIN_BINARIES_$1_$2_$3} ${RESOURCES_$1} | build/$1/$2-$3
	cp ${THIN_BINARIES_$1_$2_$3} $$@
	${call copy_target_resources,$1,$${dir $$@}}
endef

#Produces final executable build targets
${foreach target,${EXECUTABLE_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval ${call assemble_executable_${HOST_PLATFORM},${target},${configuration},${platform}}} \
		} \
	} \
}

PLIST_FILE_testharness_macosx = test_resources/Info_testharness_macosx.plist

PLIST_FILE_testharness_iphonesimulator = test_resources/Info_testharness_iphone.plist
PLIST_PLATFORM_CASED_iphonesimulator = iPhoneSimulator
PLIST_PLATFORM_LOWER_iphonesimulator = iphonesimulator
PLIST_SDK_NAME_iphonesimulator = iphonesimulator${IPHONE_SDK_VERSION}

PLIST_FILE_testharness_iphoneos = test_resources/Info_testharness_iphone.plist
PLIST_PLATFORM_CASED_iphoneos = iPhoneOS
PLIST_PLATFORM_LOWER_iphoneos = iphoneos
PLIST_SDK_NAME_iphoneos = iphoneos${IPHONE_SDK_VERSION}

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
	${call copy_target_resources,$1,$5}
endef

define assemble_application_macosx #(target, configuration)
build/$1/$2-macosx/${TARGET_NAME_$1}.app/Contents/MacOS/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_macosx} ${RESOURCES_$1} | build/$1/$2-macosx
	${call create_app_bundle,$1,macosx,$${dir $$@},build/$1/$2-macosx/${TARGET_NAME_$1}.app/Contents,build/$1/$2-macosx/${TARGET_NAME_$1}.app/Contents/Resources}
	lipo -create -output $$@ ${THIN_BINARIES_$1_$2_macosx}
endef

define assemble_application_iphonesimulator #(target, configuration)
build/$1/$2-iphonesimulator/${TARGET_NAME_$1}.app/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_iphonesimulator} ${RESOURCES_$1} | build/$1/$2-iphonesimulator
	${call create_app_bundle,$1,iphonesimulator,$${dir $$@},$${dir $$@},$${dir $$@}}
	lipo -create -output $$@ ${THIN_BINARIES_$1_$2_iphonesimulator}
endef

define assemble_application_iphoneos #(target, configuration)
build/$1/$2-iphoneos/${TARGET_NAME_$1}.app/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_iphoneos} ${RESOURCES_$1} | build/$1/$2-iphoneos
	${call create_app_bundle,$1,iphoneos,$${dir $$@},$${dir $$@},$${dir $$@}}
	lipo -create -output $$@ ${THIN_BINARIES_$1_$2_iphoneos}
endef

define assemble_application_linux #(target, configuration)
build/$1/$2-linux/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_linux} ${RESOURCES_$1} | build/$1/$2-linux
	mkdir -p build/$1/$2-linux/Resources
	${call copy_target_resources,$1,build/$1/$2-linux/Resources}
	cp ${THIN_BINARIES_$1_$2_linux} $$@
endef

define assemble_application_windows #(target, configuration)
build/$1/$2-windows/${TARGET_NAME_$1}.exe: ${THIN_BINARIES_$1_$2_windows} ${RESOURCES_$1} | build/$1/$2-windows
	mkdir -p build/$1/$2-windows/Resources
	${call copy_target_resources,$1,build/$1/$2-windows/Resources}
	cp ${THIN_BINARIES_$1_$2_windows} $$@
endef

#Produces final application build targets
${foreach target,${APPLICATION_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval ${call assemble_application_${platform},${target},${configuration}}} \
		} \
	} \
}

define library_template_macosx #(target, configuration)
	build/$1/$2-macosx/${TARGET_NAME_$1}.a
endef

define library_template_iphonesimulator #(target, configuration)
	build/$1/$2-iphonesimulator/${TARGET_NAME_$1}.a
endef

define library_template_iphoneos #(target, configuration)
	build/$1/$2-iphoneos/${TARGET_NAME_$1}.a
endef

define library_template_linux #(target, configuration)
	build/$1/$2-linux/${TARGET_NAME_$1}.a
endef

define library_template_windows #(target, configuration)
	build/$1/$2-windows/${TARGET_NAME_$1}.a
endef

define executable_template_macosx #(target, configuration)
	build/$1/$2-macosx/${TARGET_NAME_$1}
endef

define executable_template_iphonesimulator #(target, configuration)
	build/$1/$2-iphonesimulator/${TARGET_NAME_$1}
endef

define executable_template_iphoneos #(target, configuration)
	build/$1/$2-iphoneos/${TARGET_NAME_$1}
endef

define executable_template_linux #(target, configuration)
	build/$1/$2-linux/${TARGET_NAME_$1}
endef

define executable_template_windows #(target, configuration)
	build/$1/$2-windows/${TARGET_NAME_$1}.exe
endef

define application_template_macosx #(target, configuration)
	build/$1/$2-macosx/${TARGET_NAME_$1}.app/Contents/MacOS/${TARGET_NAME_$1}
endef

define application_template_iphonesimulator #(target, configuration)
	build/$1/$2-iphonesimulator/${TARGET_NAME_$1}.app/${TARGET_NAME_$1}
endef

define application_template_iphoneos #(target, configuration)
	build/$1/$2-iphoneos/${TARGET_NAME_$1}.app/${TARGET_NAME_$1}
endef

define application_template_linux #(target, configuration)
	build/$1/$2-linux/${TARGET_NAME_$1}
endef

define application_template_windows #(target, configuration)
	build/$1/$2-windows/${TARGET_NAME_$1}.exe
endef

define target_template #(target, target_type)
.PHONY: $1
$1: ${foreach configuration,${CONFIGURATIONS_$1},${foreach platform,${PLATFORMS_$1},${call $2_template_${platform},$1,${configuration}}}}
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

.PHONY: test
test: unittest ${foreach platform,${PLATFORMS_unittest},run_unittests_${platform}}

.PHONY: run_unittests_macosx
run_unittests_macosx:
	./build/unittest/debug-macosx/${TARGET_NAME_unittest} "${CURDIR}/build/unittest/debug-macosx"

.PHONY: run_unittests_iphonesimulator
run_unittests_iphonesimulator:
	DYLD_ROOT_PATH=${SDKROOT_iphonesimulator} \
	./build/unittest/debug-iphonesimulator/${TARGET_NAME_unittest} "${CURDIR}/build/unittest/debug-iphonesimulator"

.PHONY: run_unittests_linux
run_unittests_linux:
	./build/unittest/debug-linux/${TARGET_NAME_unittest} "${CURDIR}/build/unittest/debug-linux"

.PHONY: run_unittests_windows
run_unittests_windows:
	./build/unittest/debug-windows/${TARGET_NAME_unittest}.exe "${CURDIR}/build/unittest/debug-windows"

define analyze_file_template_clang #(target, platform, file)
build/analyzer-results/clang-$1-$2/${basename ${notdir $3}}.txt: $3 ${PREREQS_$1} | build/analyzer-results/clang-$1-$2
	${CLANG_$2} --analyze ${INCLUDE_CCFLAGS} ${CLANGFLAGS} ${CLANGFLAGS_$1} ${CLANGFLAGS_$2} -o $${basename $$@}.plist $3 > $$@ 2>&1; true
	@cat $$@
endef

define analyze_file_template_splint #(target, platform, file)
build/analyzer-results/splint-$1-$2/${basename ${notdir $3}}.txt: $3 ${PREREQS_$1} | build/analyzer-results/splint-$1-$2
	${SPLINT_$2} ${INCLUDE_CCFLAGS} ${SPLINTFLAGS} ${SPLINTFLAGS_$1} ${SPLINTFLAGS_$2} $3 > $$@ 2>&1; true
	@cat $$@
endef

define analyzed_sources_template #(analyzer, target)
	${sort ${filter-out ${ANALYZER_EXCLUDE_SOURCES_$1},${SOURCES_$2}}}
endef

define analyzer_output_template #(analyzer, target, platform)
	${foreach file,${call analyzed_sources_template,$1,$2}, \
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
			${foreach file,${call analyzed_sources_template,${analyzer},${target}}, \
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

.PHONY: include
include: ${INCLUDES}
	mkdir -p build/include
	${foreach include_file,$^, \
		mkdir -p build/include/${notdir ${patsubst %/,%,${dir ${include_file}}}}${newline_and_tab} \
		cp ${include_file} build/include/${notdir ${patsubst %/,%,${dir ${include_file}}}}${newline_and_tab} \
	}

.PHONY: clean
clean:
	rm -rf build
	rm -rf dist #master_source_only

define install_target_iphonesimulator_template #(target)
.PHONY: install_$1_iphonesimulator
install_$1_iphonesimulator: $1
	-killall "iPhone Simulator"
	rm -rf "${IPHONESIMULATOR_APPLICATIONS_DIR}/${TARGET_NAME_$1}"
	mkdir -p "${IPHONESIMULATOR_APPLICATIONS_DIR}/${TARGET_NAME_$1}/Documents"
	mkdir -p "${IPHONESIMULATOR_APPLICATIONS_DIR}/${TARGET_NAME_$1}/Library/Preferences"
	mkdir -p "${IPHONESIMULATOR_APPLICATIONS_DIR}/${TARGET_NAME_$1}/tmp"
	cp -r "build/$1/debug-iphonesimulator/${TARGET_NAME_$1}.app" "${IPHONESIMULATOR_APPLICATIONS_DIR}/${TARGET_NAME_$1}"
	defaults write com.apple.iphonesimulator SimulateSDKRoot -string "${SDKROOT_iphonesimulator}"
	open ${IPHONESIMULATOR_PATH}
endef

define add_blob_header #(source_file, target_file)
	ruby -e "contents = \"\"; File.open(\"$1\", \"r\") {|file| contents = file.read}; File.open(\"$2\", \"w\") {|file| file.write(\"\xFA\xDE\x71\x71\"); file.write([contents.length + 8].pack(\"N\")); file.write(contents)}"
endef

define codesign_target_iphoneos_template #(target)
.PHONY: codesign_$1_iphoneos
codesign_$1_iphoneos: $1
	sed -e "s/\$$$${PRODUCT_NAME}/${TARGET_NAME_$1}/g" test_resources/Entitlements.plist > build/intermediate/Entitlements.plist
	${call add_blob_header,build/intermediate/Entitlements.plist,build/intermediate/Entitlements.xcent}
	export CODESIGN_ALLOCATE=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/codesign_allocate
	${foreach configuration,${CONFIGURATIONS_$1},\
		cp "${RESOURCE_RULES_PLIST}" "build/$1/${configuration}-iphoneos/${TARGET_NAME_$1}.app"${newline_and_tab} \
		/usr/bin/codesign -f -s ${CODESIGN_IDENTITY} --resource-rules=${RESOURCE_RULES_PLIST} --entitlements=build/intermediate/Entitlements.xcent "build/$1/${configuration}-iphoneos/${TARGET_NAME_$1}.app"${newline_and_tab} \
	}
endef

${foreach target,${APPLICATION_TARGETS}, \
	${eval ${call install_target_iphonesimulator_template,${target}}} \
	${eval ${call codesign_target_iphoneos_template,${target}}} \
}

#master_source_only_begin
.PHONY: full_dist
full_dist: clean all
	mkdir dist dist/include dist/library dist/testharness
	cp -r build/include/* dist/include
	cp -r build/library/* dist/library
	cp -r build/testharness/* dist/testharness

.PHONY: append_dist
append_dist: all
	cp -r build/library/* dist/library
	cp -r build/testharness/* dist/testharness

.PHONY: commit_dist
commit_dist:
	svn import --no-ignore -m "Automated release of ${PROJECT_NAME} ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK} from ${HOST_PLATFORM}" dist ${SVNROOT}/Releases/${PROJECT_NAME}/${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}

define filter_source_dist_file #(infile, outfile)
	ruby -e "print STDIN.read.gsub(/^.*#master_source_only\b.*$\n?/, '').gsub(/\n?#master_source_only_begin[\s\S]*?#master_source_only_end/, '').gsub(/#source_dist_only/, '')" < "$1" > "$2"
endef

.PHONY: source_dist
source_dist:
	mkdir -p build/source_dist/include
	mkdir -p build/source_dist/lib
	mkdir -p build/source_dist/source/gltexture
	mkdir -p build/source_dist/test_resources
	mkdir -p build/source_dist/test_source/testharness
	mkdir -p build/source_dist/test_source/unittest/framework
	mkdir -p build/source_dist/test_source/unittest/suites
	
	${call filter_source_dist_file,Makefile,build/source_dist/Makefile}
	cp Changes.txt License.txt ReadMe.txt version build/source_dist
	
	cp -r include/* build/source_dist/include
	cp -r lib/* build/source_dist/lib
	find build/source_dist/include -name .svn -print0 | xargs -0 rm -rf
	find build/source_dist/lib -name .svn -print0 | xargs -0 rm -rf
	
	cp source/gltexture/* build/source_dist/source/gltexture
	
	cp test_resources/* build/source_dist/test_resources
	
	cp test_source/testharness/* build/source_dist/test_source/testharness
	cp test_source/unittest/framework/* build/source_dist/test_source/unittest/framework
	cp test_source/unittest/suites/* build/source_dist/test_source/unittest/suites
#master_source_only_end
