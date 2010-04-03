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

TARGET_PLATFORMS_macosx = macosx iphonesimulator iphoneos
TARGET_PLATFORMS_linux = linux
TARGET_PLATFORMS_windows = windows

include version

PROJECT_NAME = template_stemlib
IPHONE_SDK_VERSION = 3.1
IPHONESIMULATOR_APPLICATIONS_DIR = ${HOME}/Library/Application Support/iPhone Simulator/User/Applications
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

TARGET_NAME_library = libstem_template
TARGET_NAME_unittest = template_unittest
TARGET_NAME_testharness = template_testharness
HUMAN_READABLE_TARGET_NAME_testharness = StemTemplate

#Per-target configurations
CONFIGURATIONS_library = debug profile release
CONFIGURATIONS_unittest = debug
CONFIGURATIONS_testharness = debug profile

#Per-target platforms
PLATFORMS_library = ${filter ${PLATFORMS},macosx iphonesimulator iphoneos linux windows}
PLATFORMS_unittest = ${filter ${PLATFORMS},macosx iphonesimulator linux windows}
PLATFORMS_testharness = ${filter ${PLATFORMS},macosx iphonesimulator iphoneos linux windows}

#Per-target compile/link settings
CCFLAGS_unittest = -I test_source -DSUITE_FILE_LIST='${foreach file,${SOURCES_unittest_suites},"${basename ${notdir ${file}}}",} NULL'

#Per-configuration compile/link settings
CCFLAGS_debug = -g
CCFLAGS_profile = -g -O3
CCFLAGS_release = -O3

#Per-platform compile/link settings
CC_macosx_ppc = /usr/bin/gcc-4.2 -arch ppc
CC_macosx_i386 = /Developer/usr/bin/clang -arch i386
CC_macosx_x86_64 = /Developer/usr/bin/clang -arch x86_64
AR_macosx = /usr/bin/ar
RANLIB_macosx = /usr/bin/ranlib
SDKROOT_macosx = /Developer/SDKs/MacOSX10.5.sdk
ARCHS_macosx = ppc i386 x86_64
CCFLAGS_macosx = -isysroot ${SDKROOT_macosx} -mmacosx-version-min=10.5
LINKFLAGS_macosx = -isysroot ${SDKROOT_macosx} -mmacosx-version-min=10.5

CC_iphonesimulator_i386 = /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/gcc-4.2 -arch i386
AR_iphonesimulator = /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/ar
RANLIB_iphonesimulator = /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/ranlib
SDKROOT_iphonesimulator = /Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator${IPHONE_SDK_VERSION}.sdk
ARCHS_iphonesimulator = i386
CCFLAGS_iphonesimulator = -isysroot ${SDKROOT_iphonesimulator} -mmacosx-version-min=10.5 -D__IPHONE_OS_VERSION_MIN_REQUIRED=30000
LINKFLAGS_iphonesimulator = -isysroot ${SDKROOT_iphonesimulator} -mmacosx-version-min=10.5

CC_iphoneos_armv6 = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2 -arch armv6
CC_iphoneos_armv7 = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2 -arch armv7
AR_iphoneos = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ar
RANLIB_iphoneos = /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ranlib
SDKROOT_iphoneos = /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS${IPHONE_SDK_VERSION}.sdk
ARCHS_iphoneos = armv6 armv7
CCFLAGS_iphoneos = -isysroot ${SDKROOT_iphoneos} -miphoneos-version-min=${IPHONE_SDK_VERSION}
LINKFLAGS_iphoneos = -isysroot ${SDKROOT_iphoneos} -miphoneos-version-min=${IPHONE_SDK_VERSION}

CC_linux_i686 = /usr/bin/gcc
AR_linux = /usr/bin/ar
RANLIB_linux = /usr/bin/ranlib
ARCHS_linux = i686
CCFLAGS_linux = 
LINKFLAGS_linux = -lm -ldl -Wl,-E

CC_windows_i686 = \\MinGW\\bin\\gcc.exe
AR_windows = \\MinGW\\bin\\ar.exe
RANLIB_windows = \\MinGW\\bin\\ranlib.exe
ARCHS_windows = i686
CCFLAGS_windows = 
LINKFLAGS_windows = 

#General compile/link settings
DEFINE_CCFLAGS = -DVERSION_MAJOR=${VERSION_MAJOR}u -DVERSION_MINOR=${VERSION_MINOR}u -DVERSION_TWEAK=${VERSION_TWEAK}u
WARNING_CCFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror
INCLUDE_CCFLAGS = -I source
OTHER_CCFLAGS = -std=gnu99
CCFLAGS = ${DEFINE_CCFLAGS} ${WARNING_CCFLAGS} ${INCLUDE_CCFLAGS} ${OTHER_CCFLAGS}

FRAMEWORK_LINKFLAGS = 
LIBRARY_LINKFLAGS = 
OTHER_LINKFLAGS = 
LINKFLAGS = ${FRAMEWORK_LINKFLAGS} ${LIBRARY_LINKFLAGS} ${OTHER_LINKFLAGS}

#Per-target dependencies

LIBRARY_DEPENDENCIES_unittest =
LIBRARY_DEPENDENCIES_testharness =

#Per-target source file lists

SOURCES_library = \
	source/HelloWorld.c

SOURCES_unittest = \
	test_source/unittest/framework/unittest_main.c \
	test_source/unittest/framework/TestList.c \
	${SOURCES_unittest_suites}

SOURCES_unittest_suites = \
	test_source/unittest/suites/HelloWorldTest.c

SOURCES_testharness = \
	test_source/testharness/TestHarness_main.c

SOURCES = ${sort ${foreach target,${TARGETS},${SOURCES_${target}}}}

INCLUDES = \
	source/HelloWorld.h

RESOURCES_testharness = 



define configuration_object_list_template #(target, configuration)
	${foreach platform,${PLATFORMS_${1}}, \
		${call platform_object_list_template,${1},${2},${platform}} \
	}
endef

define platform_object_list_template #(target, configuration, platform)
	${foreach arch,${ARCHS_${3}}, \
		${call arch_object_list_template,${1},${2},${3},${arch}} \
	}
endef

define arch_object_list_template #(target, configuration, platform, arch)
	${foreach source,${SOURCES_${1}}, \
		build/intermediate/${2}-${3}-${4}/${notdir ${basename ${source}}}.o \
	}
endef

#Produces OBJECTS_${target}_${configuration} variables for each permutation of target and configuration in that target
${foreach target,${TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${eval OBJECTS_${target}_${configuration} = ${call configuration_object_list_template,${target},${configuration}}} \
	} \
}



define compile_template #(target, configuration, platform, arch, source_file)
build/intermediate/${2}-${3}-${4}/${notdir ${basename ${5}}}.o: ${5}
	mkdir -p build/intermediate/${2}-${3}-${4}
	${CC_${3}_${4}} ${CCFLAGS} ${CCFLAGS_${1}} ${CCFLAGS_${2}} ${CCFLAGS_${3}} -c -o $$@ $$^
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
build/intermediate/${2}-${3}-${4}/${5}: ${call arch_object_list_template,${1},${2},${3},${4}}
	${AR_${3}} rc $$@ $$^
	${RANLIB_${3}} $$@
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
build/intermediate/${2}-${3}-${4}/${5}: ${call arch_object_list_template,${1},${2},${3},${4}} ${6}
	${CC_${3}_${4}} -o $$@ $$^ ${LINKFLAGS} ${LINKFLAGS_${3}}
endef

define library_dependency_template #(target, configuration, platform)
	build/library/debug-${3}/${TARGET_NAME_library}.a \
	${foreach library,${LIBRARY_DEPENDENCIES_${1}}, \
		lib/${dir ${library}}${configuration}-${platform}/${notdir ${library}} \
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



define thin_binary_list_template #(target_name, configuration, platform)
	${foreach arch,${ARCHS_${3}}, \
		build/intermediate/${2}-${3}-${arch}/${1} \
	}
endef

#Produces THIN_BINARIES_${target}_${configuration}_${platform} variables for each target/configuration/platform for library targets
${foreach target,${LIBRARY_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval THIN_BINARIES_${target}_${configuration}_${platform} = ${call thin_binary_list_template,${TARGET_NAME_${target}}.a,${configuration},${platform}}} \
		} \
	} \
}

#Produces THIN_BINARIES_${target}_${configuration}_${platform} variables for each target/configuration/platform for executable targets
${foreach target,${EXECUTABLE_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval THIN_BINARIES_${target}_${configuration}_${platform} = ${call thin_binary_list_template,${TARGET_NAME_${target}},${configuration},${platform}}} \
		} \
	} \
}

#Produces THIN_BINARIES_${target}_${configuration}_${platform} variables for each target/configuration/platform for application targets
${foreach target,${APPLICATION_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval THIN_BINARIES_${target}_${configuration}_${platform} = ${call thin_binary_list_template,${TARGET_NAME_${target}},${configuration},${platform}}} \
		} \
	} \
}



define assemble_library_macosx #(target, configuration, platform)
build/${1}/${2}-${3}/${TARGET_NAME_${1}}.a: ${THIN_BINARIES_${1}_${2}_${3}}
	mkdir -p $${dir $$@}
	lipo -create -output $$@ $$^
endef

define assemble_library_linux #(target, configuration, platform)
build/${1}/${2}-${3}/${TARGET_NAME_${1}}.a: ${THIN_BINARIES_${1}_${2}_${3}}
	mkdir -p $${dir $$@}
	cp $$^ $$@
endef

define assemble_library_windows #(target, configuration, platform)
build/${1}/${2}-${3}/${TARGET_NAME_${1}}.a: ${THIN_BINARIES_${1}_${2}_${3}}
	mkdir -p $${dir $$@}
	cp $$^ $$@
endef

#Produces final library build targets
${foreach target,${LIBRARY_TARGETS}, \
	${foreach configuration,${CONFIGURATIONS_${target}}, \
		${foreach platform,${PLATFORMS_${target}}, \
			${eval ${call assemble_library_${HOST_PLATFORM},${target},${configuration},${platform}}} \
		} \
	} \
}

define assemble_executable_macosx #(target, configuration, platform)
build/${1}/${2}-${3}/${TARGET_NAME_${1}}: ${THIN_BINARIES_${1}_${2}_${3}}
	mkdir -p $${dir $$@}
	lipo -create -output $$@ $$^
endef

define assemble_executable_linux #(target, configuration, platform)
build/${1}/${2}-${3}/${TARGET_NAME_${1}}: ${THIN_BINARIES_${1}_${2}_${3}}
	mkdir -p $${dir $$@}
	cp $$^ $$@
endef

define assemble_executable_windows #(target, configuration, platform)
build/${1}/${2}-${3}/${TARGET_NAME_${1}}.exe: ${THIN_BINARIES_${1}_${2}_${3}}
	mkdir -p $${dir $$@}
	cp $$^ $$@
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
	mkdir -p ${3} ${4} ${5}
	sed -e "s/\$$$${PRODUCT_NAME}/${TARGET_NAME_${1}}/g" \
	    -e "s/\$$$${HUMAN_READABLE_PRODUCT_NAME}/${HUMAN_READABLE_TARGET_NAME_${1}}/g" \
	    -e "s/\$$$${VERSION}/${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}/g" \
	    -e "s/\$$$${COPYRIGHT_YEAR}/"`date +%Y`"/g" \
	    -e "s/\$$$${BUILD_NUMBER}/0/g" \
	    -e "s/\$$$${PLATFORM_CASED}/${PLIST_PLATFORM_CASED_${2}}/g" \
	    -e "s/\$$$${PLATFORM_LOWER}/${PLIST_PLATFORM_LOWER_${2}}/g" \
	    -e "s/\$$$${SDK}/${PLIST_SDK_NAME_${2}}/g" \
	    ${PLIST_FILE_${1}_${2}} > ${4}/Info.plist
	echo "APPL????" > ${4}/PkgInfo
	${foreach resource,${RESOURCES_${1}}, \
		cp ${resource} ${5}; \
	}
endef

define assemble_application_macosx #(target, configuration)
build/${1}/${2}-macosx/${TARGET_NAME_${1}}.app/Contents/MacOS/${TARGET_NAME_${1}}: ${THIN_BINARIES_${1}_${2}_macosx} ${RESOURCES_${1}}
	${call create_app_bundle,${1},macosx,$${dir $$@},$${dir $$@}../,$${dir $$@}../Resources/}
	lipo -create -output $$@ ${THIN_BINARIES_${1}_${2}_macosx}
endef

define assemble_application_iphonesimulator #(target, configuration)
build/${1}/${2}-iphonesimulator/${TARGET_NAME_${1}}.app/${TARGET_NAME_${1}}: ${THIN_BINARIES_${1}_${2}_iphonesimulator} ${RESOURCES_${1}}
	${call create_app_bundle,${1},iphonesimulator,$${dir $$@},$${dir $$@},$${dir $$@}}
	lipo -create -output $$@ ${THIN_BINARIES_${1}_${2}_iphonesimulator}
endef

define assemble_application_iphoneos #(target, configuration)
build/${1}/${2}-iphoneos/${TARGET_NAME_${1}}.app/${TARGET_NAME_${1}}: ${THIN_BINARIES_${1}_${2}_iphoneos} ${RESOURCES_${1}}
	${call create_app_bundle,${1},iphoneos,$${dir $$@},$${dir $$@},$${dir $$@}}
	lipo -create -output $$@ ${THIN_BINARIES_${1}_${2}_iphoneos}
endef

define assemble_application_linux #(target, configuration)
build/${1}/${2}-linux/${TARGET_NAME_${1}}: ${THIN_BINARIES_${1}_${2}_linux} ${RESOURCES_${1}}
	mkdir -p $${dir $$@}
	cp ${THIN_BINARIES_${1}_${2}_linux} $$@
endef

define assemble_application_windows #(target, configuration)
build/${1}/${2}-windows/${TARGET_NAME_${1}}.exe: ${THIN_BINARIES_${1}_${2}_windows} ${RESOURCES_${1}}
	mkdir -p $${dir $$@}
	cp ${THIN_BINARIES_${1}_${2}_windows} $$@
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
	build/${1}/${2}-macosx/${TARGET_NAME_${1}}.a
endef

define library_template_iphonesimulator #(target, configuration)
	build/${1}/${2}-iphonesimulator/${TARGET_NAME_${1}}.a
endef

define library_template_iphoneos #(target, configuration)
	build/${1}/${2}-iphoneos/${TARGET_NAME_${1}}.a
endef

define library_template_linux #(target, configuration)
	build/${1}/${2}-linux/${TARGET_NAME_${1}}.a
endef

define library_template_windows #(target, configuration)
	build/${1}/${2}-windows/${TARGET_NAME_${1}}.a
endef

define executable_template_macosx #(target, configuration)
	build/${1}/${2}-macosx/${TARGET_NAME_${1}}
endef

define executable_template_iphonesimulator #(target, configuration)
	build/${1}/${2}-iphonesimulator/${TARGET_NAME_${1}}
endef

define executable_template_iphoneos #(target, configuration)
	build/${1}/${2}-iphoneos/${TARGET_NAME_${1}}
endef

define executable_template_linux #(target, configuration)
	build/${1}/${2}-linux/${TARGET_NAME_${1}}
endef

define executable_template_windows #(target, configuration)
	build/${1}/${2}-windows/${TARGET_NAME_${1}}.exe
endef

define application_template_macosx #(target, configuration)
	build/${1}/${2}-macosx/${TARGET_NAME_${1}}.app/Contents/MacOS/${TARGET_NAME_${1}}
endef

define application_template_iphonesimulator #(target, configuration)
	build/${1}/${2}-iphonesimulator/${TARGET_NAME_${1}}.app/${TARGET_NAME_${1}}
endef

define application_template_iphoneos #(target, configuration)
	build/${1}/${2}-iphoneos/${TARGET_NAME_${1}}.app/${TARGET_NAME_${1}}
endef

define application_template_linux #(target, configuration)
	build/${1}/${2}-linux/${TARGET_NAME_${1}}
endef

define application_template_windows #(target, configuration)
	build/${1}/${2}-windows/${TARGET_NAME_${1}}.exe
endef

define target_template #(target, target_type)
.PHONY: ${1}
${1}: ${foreach configuration,${CONFIGURATIONS_${1}},${foreach platform,${PLATFORMS_${1}},${call ${2}_template_${platform},${1},${configuration}}}}
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
	./build/unittest/debug-macosx/${TARGET_NAME_unittest}

.PHONY: run_unittests_iphonesimulator
run_unittests_iphonesimulator:
	DYLD_ROOT_PATH=${SDKROOT_iphonesimulator} \
	./build/unittest/debug-iphonesimulator/${TARGET_NAME_unittest}

.PHONY: run_unittests_linux
run_unittests_linux:
	./build/unittest/debug-linux/${TARGET_NAME_unittest}

.PHONY: run_unittests_windows
run_unittests_windows:
	./build/unittest/debug-windows/${TARGET_NAME_unittest}.exe

.PHONY: include
include: ${INCLUDES}
	mkdir -p build/include
	${foreach includeFile,$^, \
		mkdir -p build/include/${notdir ${patsubst %/,%,${dir ${includeFile}}}}; \
		cp ${includeFile} build/include/${notdir ${patsubst %/,%,${dir ${includeFile}}}}; \
	}

.PHONY: clean
clean:
	rm -rf build
	rm -rf dist
	rm -rf dist_append

.PHONY: install_testharness_iphonesimulator
install_testharness_iphonesimulator: testharness
	-killall "iPhone Simulator"
	rm -rf "${IPHONESIMULATOR_APPLICATIONS_DIR}/${TARGET_NAME_testharness}"
	mkdir -p "${IPHONESIMULATOR_APPLICATIONS_DIR}/${TARGET_NAME_testharness}/Documents"
	mkdir -p "${IPHONESIMULATOR_APPLICATIONS_DIR}/${TARGET_NAME_testharness}/Library/Preferences"
	mkdir -p "${IPHONESIMULATOR_APPLICATIONS_DIR}/${TARGET_NAME_testharness}/tmp"
	cp -r "build/testharness/debug-iphonesimulator/${TARGET_NAME_testharness}.app" "${IPHONESIMULATOR_APPLICATIONS_DIR}/${TARGET_NAME_testharness}"
	open ${IPHONESIMULATOR_PATH}

define add_blob_header #(source_file, target_file)
	ruby -e "contents = \"\"; File.open(\"${1}\", \"r\") {|file| contents = file.read}; File.open(\"${2}\", \"w\") {|file| file.write(\"\xFA\xDE\x71\x71\"); file.write([contents.length + 8].pack(\"N\")); file.write(contents)}"
endef

.PHONY: codesign_testharness_iphoneos
codesign_testharness_iphoneos: testharness
	${call add_blob_header,support/Entitlements.plist,build/intermediate/Entitlements.xcent}
	export CODESIGN_ALLOCATE=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/codesign_allocate
	${foreach configuration,${CONFIGURATIONS_testharness},\
		cp "${RESOURCE_RULES_PLIST}" "build/testharness/${configuration}-iphoneos/${TARGET_NAME_testharness}.app"; \
		/usr/bin/codesign -f -s ${CODESIGN_IDENTITY} --resource-rules=${RESOURCE_RULES_PLIST} --entitlements=build/intermediate/Entitlements.xcent "build/testharness/${configuration}-iphoneos/${TARGET_NAME_testharness}.app"; \
	}

#master_source_only_begin
.PHONY: full_dist
full_dist: clean all
	mkdir dist dist/include dist/library dist/testharness
	cp -r build/include/* dist/include
	cp -r build/library/* dist/library
	cp -r build/testharness/* dist/testharness
	svn import --no-ignore -m "Automated release from ${HOST_PLATFORM}" dist ${SVNROOT}/Releases/${PROJECT_NAME}/${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}

.PHONY: append_dist
append_dist: clean all
	svn co ${SVNROOT}/Releases/${PROJECT_NAME}/${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK} dist_append
	cp -r build/library/* dist_append/library
	cp -r build/testharness/* dist_append/testharness
	svn add --no-ignore dist_append/library/* dist_append/testharness/*
	svn commit -m "Automated release append from ${HOST_PLATFORM}" dist_append

define filter_source_dist_file #(infile, outfile)
	ruby -e "print STDIN.read.gsub(/^.*#master_source_only\b.*$\n?/, '').gsub(/\n?#master_source_only_begin[\s\S]*?#master_source_only_end/, '').gsub(/#source_dist_only/, '')" < "${1}" > "${2}"
endef

.PHONY: source_dist
source_dist:
	mkdir -p build/source_dist/source
	mkdir -p build/source_dist/support
	mkdir -p build/source_dist/test_resources
	mkdir -p build/source_dist/test_source/testharness
	mkdir -p build/source_dist/test_source/unittest/framework
	mkdir -p build/source_dist/test_source/unittest/suites
	
	${call filter_source_dist_file,Makefile,build/source_dist/Makefile}
	cp Changes.txt License.txt ReadMe.txt version build/source_dist
	
	cp source/* build/source_dist/source
	
	cp support/* build/source_dist/support
	cp test_resources/* build/source_dist/test_resources
	
	cp test_source/testharness/* build/source_dist/test_source/testharness
	cp test_source/unittest/framework/* build/source_dist/test_source/unittest/framework
	cp test_source/unittest/suites/* build/source_dist/test_source/unittest/suites
#master_source_only_end
