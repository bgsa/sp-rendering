cmake_minimum_required(VERSION 3.0.0)

if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

set (MAJOR_VERSION 0)
set (MINOR_VERSION 1)
set (PATCH_VERSION 0)
project(SpRendering VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

option(OPERATING_SYSTEM "Define which operating system will be built for"  "WINDOWS")
option(ARCH_BITS        "Define the compilation to 64 bits"                "32")
option(BUILD_TYPE       "Define the build type (Debug or Release"          "Debug")
option(OPENCL_ENABLED   "Define if OpenCL will be enabled"   ON)
option(OPENCL_DIR       "Define the OpenCL home folder OR Environment Var %OPENCL_HOME% Ex.: C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v9.2" $ENV{OPENCL_HOME})

if ( ${OPENCL_ENABLED} )
	set (OPENCL_DEFINITION "-DOPENCL_ENABLED")

	if ( "${OPENCL_DIR}" STREQUAL "OFF" )
		set (OPENCL_DIR $ENV{OPENCL_HOME})
	endif()
else()
	set (OPENCL_DEFINITION "")
endif()

find_package(OpenGL REQUIRED)

set (PROJECT_DIR  ${CMAKE_CURRENT_SOURCE_DIR})
set (SOLUTION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_DIR}/lib)
set (SP_FOUNDATION_DIR ${SOLUTION_DIR}/sp-foundation)
set (SP_PHYSICS_DIR ${SOLUTION_DIR}/sp-physics)
set (VENDOR_DIR ${SOLUTION_DIR}/vendor)

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if ( ${ARCH_BITS} STREQUAL "64" )
	set (ARCH "x86_64")
	set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m64 -fPIC -std=c11" )
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -fPIC -std=c++11" )
	set (OS_LIB_DIR       /usr/lib/x86_64-linux-gnu)
	set (OS_BIN_DIR       /usr/bin/x86_64-linux-gnu)
else()
	set (ARCH "x86")	
	set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m32 -fPIC -std=c11" )
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -fPIC -std=c++11" )
	set (OS_LIB_DIR       /usr/local/lib)
	set (OS_BIN_DIR       /usr/local/bin)
endif()

if ( ${OPERATING_SYSTEM} STREQUAL "WINDOWS" )
	if( ${BUILD_SHARED_LIBS} )
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
		set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   /MDd")
	else()
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
		set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   /MTd")
	endif()

	set (SP_FOUNDATION_LIB_NAME "SpFoundation.lib")
	set (SP_PHYSICS_LIB_NAME    "SpPhysics.lib")
	set (CUSTOM_DEFINITIONS "-D_CRT_SECURE_NO_WARNINGS")
	set (OS_INCLUDE_DIR .)

elseif ( ${OPERATING_SYSTEM} STREQUAL "LINUX" OR ${OPERATING_SYSTEM} STREQUAL "OSX" )
	set (SP_FOUNDATION_LIB_NAME "libSpFoundation.a")
	set (SP_PHYSICS_LIB_NAME "libSpPhysics.a"   )
	set (CUSTOM_DEFINITIONS "-Wno-dangling-else" "-Wno-ignored-attributes")
	set (OS_INCLUDE_DIR /usr/local/include)
else()
	message(FATAL_ERROR "Operating System not defined!")
endif()

file(GLOB SOURCES "src/*.cpp")

include_directories(
	src
	${SP_FOUNDATION_DIR}/src
	${SP_PHYSICS_DIR}/src
	${VENDOR_DIR}/include
	/usr/local/include
)

link_directories(
	${VENDOR_DIR}/lib/${BUILD_TYPE}
	${SP_FOUNDATION_DIR}/lib/${BUILD_TYPE}
	${SP_PHYSICS_DIR}/lib/${BUILD_TYPE}
	${OS_LIB_DIR}
	${OS_BIN_DIR}
)

if ( ${BUILD_SHARED_LIBS} )
	add_library(${PROJECT_NAME} SHARED ${SOURCES})
else()
	add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

if     ( ${OPERATING_SYSTEM} STREQUAL "WINDOWS" )
    add_definitions(-DAPI_EXPORT -DOPENGL_ENABLED ${OPENCL_DEFINITION} ${CUSTOM_DEFINITIONS} -DWINDOWS)
elseif ( ${OPERATING_SYSTEM} STREQUAL "LINUX"   )
    add_definitions(-DAPI_EXPORT -DOPENGL_ENABLED ${OPENCL_DEFINITION} ${CUSTOM_DEFINITIONS} -DLINUX)
elseif ( ${OPERATING_SYSTEM} STREQUAL "OSX"     )
    add_definitions(-DAPI_EXPORT -DOPENGL_ENABLED ${OPENCL_DEFINITION} ${CUSTOM_DEFINITIONS} -DOSX)
else()
    message(FATAL_ERROR "Operating System not defined!")
endif()

target_link_libraries(${PROJECT_NAME} 
	${SP_FOUNDATION_LIB_NAME}
	${SP_PHYSICS_LIB_NAME}
	${OpenCL_LIBRARIES}
)

if ( ${OPERATING_SYSTEM} STREQUAL "OSX" )
	target_link_libraries(${PROJECT_NAME}
	    "-framework Cocoa"
	    "-framework IOKit"
	    "-framework CoreVideo"
	)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
