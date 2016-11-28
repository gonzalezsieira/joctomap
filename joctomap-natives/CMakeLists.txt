cmake_minimum_required(VERSION 2.8)
project(joctomap_natives)

set(CMAKE_CXX_STANDARD 11)

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

include_directories(include)
include_directories(/usr/lib/jvm/java-8-openjdk-amd64/include)
include_directories(/usr/lib/jvm/java-8-openjdk-amd64/include/linux)
include_directories(octomap/octomap/include)
include_directories(octomap/dynamicEDT3D/include)

add_subdirectory(octomap)

add_library(joctomap_natives SHARED ${SOURCE_FILES})

target_link_libraries(joctomap_natives octomap)
