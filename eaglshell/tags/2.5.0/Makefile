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

iphone_sdk_version_integer = ${subst .,0,$1}${word ${words ${wordlist 2, ${words ${subst ., ,$1}}, ${subst ., ,$1}}}, 00}

TARGET_PLATFORMS_macosx = iphonesimulator iphoneos
TARGET_PLATFORMS_linux = 
TARGET_PLATFORMS_windows = 

PROJECT_NAME = eaglshell
IPHONE_BUILD_SDK_VERSION ?= 4.2
IPHONE_DEPLOYMENT_TARGET_VERSION = 3.1
CODESIGN_IDENTITY = "iPhone Developer"

LIBRARY_TARGETS = library
EXECUTABLE_TARGETS = 
APPLICATION_TARGETS = testharness
TARGETS = ${LIBRARY_TARGETS} ${EXECUTABLE_TARGETS} ${APPLICATION_TARGETS}
CONFIGURATIONS = debug profile release
PLATFORMS = ${filter ${TARGET_PLATFORMS_${HOST_PLATFORM}},iphonesimulator iphoneos}
ANALYZERS = splint clang

TARGET_NAME_library = libstem_eaglshell
TARGET_NAME_testharness = ${PROJECT_NAME}_testharness
HUMAN_READABLE_TARGET_NAME_testharness = EAGLShell

#Per-target configurations
CONFIGURATIONS_library = debug profile release
CONFIGURATIONS_testharness = debug profile release

#Per-target platforms
PLATFORMS_library = ${filter ${PLATFORMS},iphonesimulator iphoneos}
PLATFORMS_testharness = ${filter ${PLATFORMS},iphonesimulator iphoneos}

#Per-target compile/link settings

#Per-target analyzer settings

#Per-configuration compile/link settings
CCFLAGS_debug = -g -DDEBUG
CCFLAGS_profile = -g -O3
CCFLAGS_release = -O3

#Per-platform compile/link settings
CC_iphonesimulator_i386 = /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/gcc-4.2 -arch i386
AR_iphonesimulator = /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/ar
RANLIB_iphonesimulator = /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/ranlib
SPLINT_iphonesimulator = /usr/local/bin/splint
CLANG_iphonesimulator = /usr/bin/clang
SDKROOT_iphonesimulator = /Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator${IPHONE_BUILD_SDK_VERSION}.sdk
ARCHS_iphonesimulator = i386
CCFLAGS_iphonesimulator = -isysroot ${SDKROOT_iphonesimulator} -fobjc-abi-version=2 -fobjc-legacy-dispatch -D__IPHONE_OS_VERSION_MIN_REQUIRED=${call iphone_sdk_version_integer,${IPHONE_DEPLOYMENT_TARGET_VERSION}} -x objective-c
LINKFLAGS_iphonesimulator = -isysroot ${SDKROOT_iphonesimulator} -Xlinker -objc_abi_version -Xlinker 2 -framework Foundation -framework UIKit -framework QuartzCore -framework OpenGLES -framework CoreFoundation

CC_iphoneos_armv6 = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2 -arch armv6
CC_iphoneos_armv7 = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2 -arch armv7
AR_iphoneos = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ar
RANLIB_iphoneos = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ranlib
SPLINT_iphoneos = /usr/local/bin/splint
CLANG_iphoneos = /usr/bin/clang
SDKROOT_iphoneos = /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS${IPHONE_BUILD_SDK_VERSION}.sdk
ARCHS_iphoneos = armv6 armv7
CCFLAGS_iphoneos = -isysroot ${SDKROOT_iphoneos} -miphoneos-version-min=${IPHONE_DEPLOYMENT_TARGET_VERSION} -fobjc-abi-version=2 -fobjc-legacy-dispatch -x objective-c
LINKFLAGS_iphoneos = -isysroot ${SDKROOT_iphoneos} -miphoneos-version-min=${IPHONE_DEPLOYMENT_TARGET_VERSION} -Xlinker -objc_abi_version -Xlinker 2 -framework Foundation -framework UIKit -framework QuartzCore -framework OpenGLES -framework CoreFoundation

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
	glgraphics

#Dependencies (can optionally be per-target or per-target-per-platform)

PROJECT_LIBRARY_DEPENDENCIES_testharness = library
STEM_LIBRARY_DEPENDENCIES = \
	glgraphics/1.1.2_noglew
STEM_SOURCE_DEPENDENCIES = 
THIRDPARTY_LIBRARY_DEPENDENCIES = 

#Per-target source file lists

SOURCES_library = \
	source/${PROJECT_NAME}/main.m \
	source/${PROJECT_NAME}/EAGLShell.m \
	source/${PROJECT_NAME}/EAGLShellApplication.m \
	source/${PROJECT_NAME}/EAGLView.m

SOURCES_testharness = \
	test_source/testharness/TestHarness_main.c

#Include files to be distributed with library

INCLUDES = \
	source/${PROJECT_NAME}/EAGLShell.h \
	source/${PROJECT_NAME}/EAGLTarget.h

#Target resources

RESOURCES_testharness = \
	test_resources/Default.png \
	test_resources/Default@2x.png \
	test_resources/Default_ipad.png \
	test_resources/Fragment.frag \
	test_resources/Vertex.vert

#General analyzer settings
CLANGFLAGS = -isysroot ${SDKROOT_iphonesimulator} -D__IPHONE_OS_VERSION_MIN_REQUIRED=${call iphone_sdk_version_integer,${IPHONE_DEPLOYMENT_TARGET_VERSION}}
SPLINTFLAGS = -exportlocal

#Source files excluded from static analysis

ANALYZER_EXCLUDE_SOURCES_clang = 
ANALYZER_EXCLUDE_SOURCES_splint = \
	source/${PROJECT_NAME}/main.m \
	source/${PROJECT_NAME}/EAGLShell.m \
	source/${PROJECT_NAME}/EAGLShellApplication.m \
	source/${PROJECT_NAME}/EAGLView.m \
	test_source/testharness/TestHarness_main.c



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
-I source -I include \
${foreach stem_dependency,${STEM_LIBRARY_DEPENDENCIES} ${STEM_LIBRARY_DEPENDENCIES_$1} ${STEM_LIBRARY_DEPENDENCIES_$1_$2},-I ${STEM_SHARED_DIR}/${stem_dependency}/include} \
${foreach thirdparty_dependency,${THIRDPARTY_LIBRARY_DEPENDENCIES} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$1} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$1_$2},-I ${STEM_SHARED_DIR}/${dir ${thirdparty_dependency}}include} \
${foreach source_dependency,${STEM_SOURCE_DEPENDENCIES} ${STEM_SOURCE_DEPENDENCIES_$1} ${STEM_SOURCE_DEPENDENCIES_$1_$2},-I dep/${word 1,${subst /, ,${source_dependency}}}/source}
endef

define define_ccflags_template #(target, configuration, platform, arch)
-DSTEM_TARGET_$1 -DSTEM_CONFIGURATION_$2 -DSTEM_PLATFORM_$3 -DSTEM_ARCH_$4
endef

define dependency_template #(target, configuration, platform, arch, source_file)
build/intermediate/$1-$2-$3-$4/${notdir ${basename $5}}.d: $5 ${PREREQS_$1} | build/intermediate/$1-$2-$3-$4
	${CC_$3_$4} ${CCFLAGS} ${CCFLAGS_$1} ${CCFLAGS_$2} ${CCFLAGS_$3} ${call include_ccflags_template,$1,$3} ${call define_ccflags_template,$1,$2,$3,$4} -MM -o $$@.temp $5
	sed 's,\(${notdir ${basename $5}}\)\.o[ :]*,$${basename $$@}.o $${basename $$@}.d: ,g' < $$@.temp > $$@
	rm $$@.temp
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
		${foreach library,${filter ${link_library}%,${PROJECT_LIBRARY_DEPENDENCIES} ${PROJECT_LIBRARY_DEPENDENCIES_$1} ${PROJECT_LIBRARY_DEPENDENCIES_$1_$3}}, \
			build/${library}/$2-$3/${TARGET_NAME_${library}}.a \
		} \
		${foreach library,${filter ${link_library}%,${STEM_LIBRARY_DEPENDENCIES} ${STEM_LIBRARY_DEPENDENCIES_$1} ${STEM_LIBRARY_DEPENDENCIES_$1_$3}}, \
			${STEM_SHARED_DIR}/${library}/library/$2-$3/libstem_${word 1,${subst /, ,${library}}}.a \
		} \
		${foreach library,${filter ${link_library}%,${STEM_SOURCE_DEPENDENCIES} ${STEM_SOURCE_DEPENDENCIES_$1} ${STEM_SOURCE_DEPENDENCIES_$1_$3}}, \
			dep/${word 1,${subst /, ,${library}}}/build/${word 2,${subst /, ,${library}}}/$2-$3/${word 3,${subst /, ,${library}}} \
		} \
		${foreach library,${filter ${link_library}%,${THIRDPARTY_LIBRARY_DEPENDENCIES} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$1} ${THIRDPARTY_LIBRARY_DEPENDENCIES_$1_$3}}, \
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



define dependency_submake_template #(dependency)
.PHONY: $1
$1:
	${MAKE} -C dep/${word 1,${subst /, ,$1}}
endef

#Invokes make for each source dependency
${foreach dependency,${sort ${foreach target,${TARGETS},${foreach platform,${PLATFORMS_${target}},${STEM_SOURCE_DEPENDENCIES} ${STEM_SOURCE_DEPENDENCIES_${target}} ${STEM_SOURCE_DEPENDENCIES_${target}_${platform}}}}}, \
	${eval ${call dependency_submake_template,${dependency}}} \
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

#Produces final library build targets
${foreach target,${LIBRARY_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval ${call assemble_library_${HOST_PLATFORM},${target},${configuration},${platform}}} \
		} \
	} \
}

define copy_target_resources #(target, platform, resources_dir)
	${foreach resource,${RESOURCES_$1} ${RESOURCES_$1_$2}, \
		cp -r ${resource} $3${newline_and_tab} \
	}
	find $3 -name .svn -or -name .DS_Store -print0 | xargs -0 rm -rf
endef

define assemble_executable_macosx #(target, configuration, platform)
build/$1/$2-$3/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_$3} ${RESOURCES_$1} ${RESOURCES_$1_$3} | build/$1/$2-$3
	lipo -create -output $$@ ${THIN_BINARIES_$1_$2_$3}
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

PLIST_FILE_testharness_iphonesimulator = test_resources/Info_testharness_iphone.plist
PLIST_PLATFORM_CASED_iphonesimulator = iPhoneSimulator
PLIST_PLATFORM_LOWER_iphonesimulator = iphonesimulator
PLIST_SDK_NAME_iphonesimulator = iphonesimulator${IPHONE_BUILD_SDK_VERSION}

PLIST_FILE_testharness_iphoneos = test_resources/Info_testharness_iphone.plist
PLIST_PLATFORM_CASED_iphoneos = iPhoneOS
PLIST_PLATFORM_LOWER_iphoneos = iphoneos
PLIST_SDK_NAME_iphoneos = iphoneos${IPHONE_BUILD_SDK_VERSION}

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

define assemble_application_iphonesimulator #(target, configuration)
build/$1/$2-iphonesimulator/${TARGET_NAME_$1}.app/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_iphonesimulator} ${RESOURCES_$1} ${RESOURCES_$1_iphonesimulator} | build/$1/$2-iphonesimulator
	${call create_app_bundle,$1,iphonesimulator,$${dir $$@},$${dir $$@},$${dir $$@}}
	lipo -create -output $$@ ${THIN_BINARIES_$1_$2_iphonesimulator}
endef

define assemble_application_iphoneos #(target, configuration)
build/$1/$2-iphoneos/${TARGET_NAME_$1}.app/${TARGET_NAME_$1}: ${THIN_BINARIES_$1_$2_iphoneos} ${RESOURCES_$1} ${RESOURCES_$1_iphoneos} | build/$1/$2-iphoneos
	${call create_app_bundle,$1,iphoneos,$${dir $$@},$${dir $$@},$${dir $$@}}
	lipo -create -output $$@ ${THIN_BINARIES_$1_$2_iphoneos}
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
${STEM_SOURCE_DEPENDENCIES} ${STEM_SOURCE_DEPENDENCIES_$1} ${STEM_SOURCE_DEPENDENCIES_$1_$3} build/$1/$2-$3/${TARGET_NAME_$1}.a
endef

define executable_dependency_template #(target, configuration, platform)
${STEM_SOURCE_DEPENDENCIES} ${STEM_SOURCE_DEPENDENCIES_$1} ${STEM_SOURCE_DEPENDENCIES_$1_$3} build/$1/$2-$3/${TARGET_NAME_$1}${EXECUTABLE_SUFFIX_$3}
endef

define application_dependency_template #(target, configuration, platform)
${STEM_SOURCE_DEPENDENCIES} ${STEM_SOURCE_DEPENDENCIES_$1} ${STEM_SOURCE_DEPENDENCIES_$1_$3} build/$1/$2-$3/${call application_file_template_$3,$1}
endef

define application_file_template_iphonesimulator #(target)
${TARGET_NAME_$1}.app/${TARGET_NAME_$1}
endef

define application_file_template_iphoneos #(target)
${TARGET_NAME_$1}.app/${TARGET_NAME_$1}
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
	${foreach dependency,${sort ${foreach target,${TARGETS},${foreach platform,${PLATFORMS_${target}},${STEM_SOURCE_DEPENDENCIES} ${STEM_SOURCE_DEPENDENCIES_${target}} ${STEM_SOURCE_DEPENDENCIES_${target}_${platform}}}}}, \
		${MAKE} -C dep/${word 1,${subst /, ,${dependency}}} clean${newline_and_tab} \
	}

TARGET_SUFFIX_ipad = _ipad
TARGET_SUFFIX_iphone4 = _iphone4
IPHONE_SDK_VERSION_iphone ?= 4.2
IPHONE_SDK_VERSION_ipad ?= 3.2
IPHONE_SDK_VERSION_iphone4 ?= 4.2
IPHONESIMULATOR_APPLICATIONS_DIR_iphone ?= ${HOME}/Library/Application Support/iPhone Simulator/${IPHONE_SDK_VERSION_iphone}/Applications
IPHONESIMULATOR_APPLICATIONS_DIR_ipad ?= ${HOME}/Library/Application Support/iPhone Simulator/${IPHONE_SDK_VERSION_ipad}/Applications
IPHONESIMULATOR_APPLICATIONS_DIR_iphone4 ?= ${HOME}/Library/Application Support/iPhone Simulator/${IPHONE_SDK_VERSION_iphone4}/Applications
SIMULATE_DEVICE_iphone = iPhone
SIMULATE_DEVICE_ipad = iPad
SIMULATE_DEVICE_iphone4 = iPhone 4
SIMULATE_SDKROOT_iphone = /Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator${IPHONE_SDK_VERSION_iphone}.sdk
SIMULATE_SDKROOT_ipad = /Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator${IPHONE_SDK_VERSION_ipad}.sdk
SIMULATE_SDKROOT_iphone4 = /Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator${IPHONE_SDK_VERSION_iphone4}.sdk
IPHONE_SIMULATOR_PATH ?= /Developer/Platforms/iPhoneSimulator.platform/Developer/Applications/iPhone Simulator.app

define install_target_iphonesimulator_template #(target, simulate_device)
.PHONY: install_$1_iphonesimulator${TARGET_SUFFIX_$2}
install_$1_iphonesimulator${TARGET_SUFFIX_$2}: $1
	killall "iPhone Simulator"; true
	rm -rf "${IPHONESIMULATOR_APPLICATIONS_DIR_$2}/${TARGET_NAME_$1}"
	mkdir -p "${IPHONESIMULATOR_APPLICATIONS_DIR_$2}/${TARGET_NAME_$1}/Documents"
	mkdir -p "${IPHONESIMULATOR_APPLICATIONS_DIR_$2}/${TARGET_NAME_$1}/Library/Preferences"
	mkdir -p "${IPHONESIMULATOR_APPLICATIONS_DIR_$2}/${TARGET_NAME_$1}/tmp"
	cp -r "build/$1/debug-iphonesimulator/${TARGET_NAME_$1}.app" "${IPHONESIMULATOR_APPLICATIONS_DIR_$2}/${TARGET_NAME_$1}"
	defaults write com.apple.iphonesimulator SimulateDevice -string "${SIMULATE_DEVICE_$2}"
	defaults write com.apple.iphonesimulator SimulateSDKRoot -string "${SIMULATE_SDKROOT_$2}"
	defaults write com.apple.iphonesimulator currentSDKRoot -string "${SIMULATE_SDKROOT_$2}"
	open "${IPHONE_SIMULATOR_PATH}"
endef

define add_blob_header #(source_file, target_file)
	ruby -e "contents = \"\"; File.open(\"$1\", \"r\") {|file| contents = file.read}; File.open(\"$2\", \"w\") {|file| file.write(\"\xFA\xDE\x71\x71\"); file.write([contents.length + 8].pack(\"N\")); file.write(contents)}"
endef

RESOURCE_RULES_PLIST = /Developer/Platforms/MacOSX.platform/ResourceRules.plist

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
	${eval ${call install_target_iphonesimulator_template,${target},iphone}} \
	${eval ${call install_target_iphonesimulator_template,${target},ipad}} \
	${eval ${call install_target_iphonesimulator_template,${target},iphone4}} \
	${eval ${call codesign_target_iphoneos_template,${target}}} \
}

INSTALL_DIR = ${STEM_SHARED_DIR}/${PROJECT_NAME}/${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}

.PHONY: install
install:
	mkdir -p "${INSTALL_DIR}/include" "${INSTALL_DIR}/library" "${INSTALL_DIR}/testharness"
	cp Changes.txt License.txt ReadMe.txt version ${INSTALL_DIR}
	cp -r build/include/* ${INSTALL_DIR}/include
	cp -r build/library/* ${INSTALL_DIR}/library
	cp -r build/testharness/* ${INSTALL_DIR}/testharness
