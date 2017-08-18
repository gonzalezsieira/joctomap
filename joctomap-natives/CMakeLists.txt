# Script for building the shared library libjoctomap_natives.so
# REQUIRES:
#	- Octomap
#	- Cmake 2.8+
#	- variable JDK pointing to the Java SDK installation folder

cmake_minimum_required(VERSION 2.8)
project(joctomap_natives)

file(GLOB SOURCES_CPP src/*.cpp)
file(GLOB SOURCES_H include/*.h)
	
set(SOURCE_FILES
    include/adjacencymap.h
    include/collisionchecker2d.h
    include/collisions.h
    include/definitions.h
    include/h2dmrtransitionfunction.h
    include/iteratorinformation.h
    include/joctree.h
    include/joctreenode.h
    include/joctreeutils.h
    include/leafbbxiterator.h
    include/nativeobject.h
    include/joctreedistancemap.h
    src/adjacencymap.cpp
    src/collisionchecker2d.cpp
    src/collisions.cpp
    src/h2dmrtransitionfunction.cpp
    src/iteratorinformation.cpp
    src/joctree.cpp
    src/joctreenode.cpp
    src/joctreeutils.cpp
    src/leafbbxiterator.cpp
    src/nativeobject.cpp
    src/joctreedistancemap.cpp
    )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set_property(SOURCE ${SOURCES_CPP} PROPERTY COMPILE_FLAGS "-O3 -std=c++0x")
set_property(SOURCE ${SOURCES_H} PROPERTY COMPILE_FLAGS "-O3 -std=c++0x")

# Overwrite if it is debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Release=Debug mode disables all optimizations" )
    set_property(SOURCE ${SOURCES_CPP} PROPERTY COMPILE_FLAGS "-std=c++0x")
    set_property(SOURCE ${SOURCES_H} PROPERTY COMPILE_FLAGS "-std=c++0x")
endif()

message(STATUS "Java SDK location: " ${JDK})

include_directories(include)
include_directories("${JDK}/include")
if(UNIX AND NOT APPLE)
    include_directories("${JDK}/include/linux")
endif()
if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    include_directories("${JDK}/include/darwin")
endif()
include_directories(octomap/octomap/include)
include_directories(octomap/dynamicEDT3D/include)

add_subdirectory(octomap/octomap)
add_subdirectory(octomap/dynamicEDT3D)

add_library(joctomap_natives SHARED ${SOURCE_FILES})

add_dependencies(dynamicedt3d octomap)
add_dependencies(joctomap_natives octomap octomath dynamicedt3d)

target_link_libraries(joctomap_natives octomath octomap dynamicedt3d)
