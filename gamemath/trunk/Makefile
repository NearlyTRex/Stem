PROJECT_NAME = gamemath

# VERSION_MAJOR, VERSION_MINOR, and VERSION_TWEAK are available as preprocessor macros for all source files in the project
VERSION_MAJOR = 3
VERSION_MINOR = 1
VERSION_TWEAK = 0
PROJECT_VERSION = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_TWEAK}

LIBRARY_TARGETS = library
EXECUTABLE_TARGETS = unittest profiler
APPLICATION_TARGETS = testharness

# TARGET_NAME_${target} required for each target of any type; HUMAN_READABLE_TARGET_NAME_${target} required for each application target
#TARGET_NAME_library = libstem_${PROJECT_NAME}
#TARGET_NAME_unittest = ${PROJECT_NAME}_unittest
#TARGET_NAME_testharness = ${PROJECT_NAME}_testharness
TARGET_NAME_profiler = ${PROJECT_NAME}_profiler
HUMAN_READABLE_TARGET_NAME_testharness = GameMath

# Patterns: PLATFORMS, PLATFORMS_${target}
PLATFORMS = macosx iphonesimulator iphoneos linux32 linux64 win32 win64
PLATFORMS_profiler = macosx linux32 linux64 win32 win64
TARGET_PLATFORMS_profiler = ${filter ${TARGET_PLATFORMS},${PLATFORMS_profiler}}
CONFIGURATIONS_profiler = profile

# Patterns: CCFLAGS, CCFLAGS_${target}, CCFLAGS_${configuration}, CCFLAGS_${platform}, CCFLAGS_${target}_${configuration}, CCFLAGS_${target}_${platform}, CCFLAGS_${configuration}_${platform}, CCFLAGS_${target}_${configuration}_${platform}
CCFLAGS = -I build/intermediate/generated
CCFLAGS_win32 = -Wno-unused-but-set-variable
CCFLAGS_win64 = -Wno-unused-but-set-variable
CCFLAGS_linux32 = -Wno-unused-but-set-variable
CCFLAGS_linux64 = -Wno-unused-but-set-variable

# Patterns: LINKFLAGS, LINKFLAGS_${target}, LINKFLAGS_${platform}, LINKFLAGS_${target}_${platform}
LINKFLAGS_library_linux32 = -lm
LINKFLAGS_library_linux64 = -lm

# PROJECT_LIBRARY_DEPENDENCIES_* refers to entries in LIBRARY_TARGETS to be linked when building the specified target
# Patterns: PROJECT_LIBRARY_DEPENDENCIES_${target}, PROJECT_LIBRARY_DEPENDENCIES_${target}_${platform}
PROJECT_LIBRARY_DEPENDENCIES_unittest = library
PROJECT_LIBRARY_DEPENDENCIES_profiler = library
PROJECT_LIBRARY_DEPENDENCIES_testharness = library

# STEM_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each stem library to be linked when building
# Patterns: STEM_LIBRARY_DEPENDENCIES, STEM_LIBRARY_DEPENDENCIES_${target}, STEM_LIBRARY_DEPENDENCIES_${platform}, STEM_LIBRARY_DEPENDENCIES_${target}_${platform}
STEM_LIBRARY_DEPENDENCIES_unittest = unittest/latest
STEM_LIBRARY_DEPENDENCIES_testharness = shell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_macosx = nsopenglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_win32 = wglshell/latest
STEM_LIBRARY_DEPENDENCIES_testharness_win64 = wglshell/latest

# THIRDPARTY_LIBRARY_DEPENDENCIES is specified as ${PROJECT_NAME}/${PROJECT_VERSION} for each thirdparty library to be linked when building
# Patterns: THIRDPARTY_LIBRARY_DEPENDENCIES, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}, THIRDPARTY_LIBRARY_DEPENDENCIES_${platform}, THIRDPARTY_LIBRARY_DEPENDENCIES_${target}_${platform}
THIRDPARTY_LIBRARY_DEPENDENCIES = 

TYPE_SUFFIX_float = f
TYPE_SUFFIX_double = d
TYPE_SUFFIX_int = i
TYPE_SUFFIX_fixed16_16 = x

GENERATED_SOURCES = \
	${foreach type_suffix,f d i x, \
		${foreach axis_count,2 3 4, \
			build/intermediate/generated/${PROJECT_NAME}/Vector${axis_count}${type_suffix}.h \
			build/intermediate/generated/${PROJECT_NAME}/Vector${axis_count}${type_suffix}.c \
		} \
		build/intermediate/generated/${PROJECT_NAME}/Rect4${type_suffix}.h \
		build/intermediate/generated/${PROJECT_NAME}/Rect4${type_suffix}.c \
		build/intermediate/generated/${PROJECT_NAME}/Box6${type_suffix}.h \
		build/intermediate/generated/${PROJECT_NAME}/Box6${type_suffix}.c \
	} \
	build/intermediate/generated/${PROJECT_NAME}/RectBoxConversions.h \
	build/intermediate/generated/${PROJECT_NAME}/VectorConversions.h

# Additional build prerequisites per target
# Patterns: PREREQS, PREREQS_${target}
PREREQS = ${GENERATED_SOURCES}

SOURCES_library = \
	source/${PROJECT_NAME}/Barycentric.c \
	source/${PROJECT_NAME}/BezierCurve.c \
	source/${PROJECT_NAME}/FixedPoint.c \
	source/${PROJECT_NAME}/Interpolation.c \
	source/${PROJECT_NAME}/Matrix4x4f.c \
	source/${PROJECT_NAME}/Matrix4x4x.c \
	source/${PROJECT_NAME}/MouseCoordinateTransforms.c \
	source/${PROJECT_NAME}/ParameterSpline.c \
	source/${PROJECT_NAME}/Quaternionf.c \
	source/${PROJECT_NAME}/Quaternionx.c \
	build/intermediate/generated/${PROJECT_NAME}/Box6d.c \
	build/intermediate/generated/${PROJECT_NAME}/Box6f.c \
	build/intermediate/generated/${PROJECT_NAME}/Box6i.c \
	build/intermediate/generated/${PROJECT_NAME}/Box6x.c \
	build/intermediate/generated/${PROJECT_NAME}/Rect4d.c \
	build/intermediate/generated/${PROJECT_NAME}/Rect4f.c \
	build/intermediate/generated/${PROJECT_NAME}/Rect4i.c \
	build/intermediate/generated/${PROJECT_NAME}/Rect4x.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector2d.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector3d.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector4d.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector2f.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector3f.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector4f.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector2i.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector3i.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector4i.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector2x.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector3x.c \
	build/intermediate/generated/${PROJECT_NAME}/Vector4x.c

# Executable targets named "unittest" should usually specify only SOURCES_unittest_suites, which must contain only test suite files.
# If files other than test suites are required, specify SOURCES_unittest.
SOURCES_unittest_suites = \
	source/unittest/BarycentricTest.c \
	source/unittest/BezierCurveTest.c \
	source/unittest/FixedPointTest.c \
	source/unittest/InterpolationTest.c \
	source/unittest/Matrix4x4fTest.c \
	source/unittest/Matrix4x4xTest.c \
	source/unittest/MouseCoordinateTransformsTest.c \
	source/unittest/ParameterSplineTest.c \
	source/unittest/RectBoxConversionsTest.c \
	source/unittest/RectBoxTest.c \
	source/unittest/QuaternionfTest.c \
	source/unittest/QuaternionxTest.c \
	source/unittest/VectorConversionsTest.c \
	source/unittest/VectorTest.c

SOURCES_profiler = \
	source/profiler/Profiler_main.c

SOURCES_testharness = \
	source/testharness/TestHarness_main.c

# Public-facing include files to be distributed with the library, if the project builds one
INCLUDES = \
	source/${PROJECT_NAME}/Barycentric.h \
	source/${PROJECT_NAME}/BezierCurve.h \
	source/${PROJECT_NAME}/FixedPoint.h \
	source/${PROJECT_NAME}/Interpolation.h \
	source/${PROJECT_NAME}/Matrix4x4f.h \
	source/${PROJECT_NAME}/Matrix4x4x.h \
	source/${PROJECT_NAME}/MouseCoordinateTransforms.h \
	source/${PROJECT_NAME}/ParameterSpline.h \
	source/${PROJECT_NAME}/Quaternionf.h \
	source/${PROJECT_NAME}/Quaternionx.h \
	build/intermediate/generated/${PROJECT_NAME}/Box6d.h \
	build/intermediate/generated/${PROJECT_NAME}/Box6f.h \
	build/intermediate/generated/${PROJECT_NAME}/Box6i.h \
	build/intermediate/generated/${PROJECT_NAME}/Box6x.h \
	build/intermediate/generated/${PROJECT_NAME}/Rect4d.h \
	build/intermediate/generated/${PROJECT_NAME}/Rect4f.h \
	build/intermediate/generated/${PROJECT_NAME}/Rect4i.h \
	build/intermediate/generated/${PROJECT_NAME}/Rect4x.h \
	build/intermediate/generated/${PROJECT_NAME}/RectBoxConversions.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector2d.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector3d.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector4d.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector2f.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector3f.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector4f.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector2i.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector3i.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector4i.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector2x.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector3x.h \
	build/intermediate/generated/${PROJECT_NAME}/Vector4x.h \
	build/intermediate/generated/${PROJECT_NAME}/VectorConversions.h

# Patterns: RESOURCES, RESOURCES_${target}, RESOURCES_${platform}, RESOURCES_${target}_${platform}
RESOURCES = 

# Pattern: ANALYZER_EXCLUDE_SOURCES_${analyzer}
ANALYZER_EXCLUDE_SOURCES_clang = 
ANALYZER_EXCLUDE_SOURCES_splint = ${SOURCES_unittest}

CLANGFLAGS = -I build/intermediate/generated
SPLINTFLAGS = -I build/intermediate/generated

DEPENDENCY_SED_COMMANDS = \
	s,source/${PROJECT_NAME}/Vector\(\\d\),build/intermediate/generated/${PROJECT_NAME}/Vector\\1,g; \
	s,source/${PROJECT_NAME}/Rect\(\\d\),build/intermediate/generated/${PROJECT_NAME}/Rect\\1,g; \
	s,source/${PROJECT_NAME}/Box\(\\d\),build/intermediate/generated/${PROJECT_NAME}/Box\\1,g; \
	s,source/${PROJECT_NAME}/VectorConversions,build/intermediate/generated/${PROJECT_NAME}/VectorConversions,g; \
	s,source/${PROJECT_NAME}/RectBoxConversions,build/intermediate/generated/${PROJECT_NAME}/RectBoxConversions,g

# Pattern: PLIST_FILE_${target}_${platform} (required for application targets on macosx, iphoneos, and iphonesimulator)
PLIST_FILE_testharness_macosx = resources/Info_testharness_macosx.plist
PLIST_FILE_testharness_iphonesimulator = resources/Info_testharness_iphone.plist
PLIST_FILE_testharness_iphoneos = resources/Info_testharness_iphone.plist

INSTALLED_TARGETS = library testharness profiler

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

build/intermediate/generated/${PROJECT_NAME}:
	mkdir -p $@

define generated_vector_template #(axis_count, numeric_type)
build/intermediate/generated/${PROJECT_NAME}/Vector$1${TYPE_SUFFIX_$2}.h: source/${PROJECT_NAME}/Vector.h.erb | build/intermediate/generated/${PROJECT_NAME}
	axis_count=$1 numeric_type=$2 erb -T - $$^ > $$@
build/intermediate/generated/${PROJECT_NAME}/Vector$1${TYPE_SUFFIX_$2}.c: source/${PROJECT_NAME}/Vector.c.erb | build/intermediate/generated/${PROJECT_NAME}
	axis_count=$1 numeric_type=$2 erb -T - $$^ > $$@
endef

define generated_rectbox_template #(shape_name, numeric_type)
build/intermediate/generated/${PROJECT_NAME}/$1${TYPE_SUFFIX_$2}.h: source/${PROJECT_NAME}/RectBox.h.erb | build/intermediate/generated/${PROJECT_NAME}
	shape_name=$1 numeric_type=$2 erb -T - $$^ > $$@
build/intermediate/generated/${PROJECT_NAME}/$1${TYPE_SUFFIX_$2}.c: source/${PROJECT_NAME}/RectBox.c.erb | build/intermediate/generated/${PROJECT_NAME}
	shape_name=$1 numeric_type=$2 erb -T - $$^ > $$@
endef

define generated_header_template #(name)
build/intermediate/generated/${PROJECT_NAME}/$1.h: source/${PROJECT_NAME}/$1.h.erb | build/intermediate/generated/${PROJECT_NAME}
	erb -T - $$^ > $$@
endef

${foreach numeric_type,float double int fixed16_16, \
	${foreach axis_count,2 3 4, \
		${eval ${call generated_vector_template,${axis_count},${numeric_type}}} \
	} \
	${eval ${call generated_rectbox_template,Rect4,${numeric_type}}} \
	${eval ${call generated_rectbox_template,Box6,${numeric_type}}} \
}
${eval ${call generated_header_template,VectorConversions}}
${eval ${call generated_header_template,RectBoxConversions}}
