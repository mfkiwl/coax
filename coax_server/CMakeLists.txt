cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})

set(COAXHOME ${PROJECT_SOURCE_DIR}/../deploy)
set(COAXTARGET linux)
link_directories(${COAXHOME}/${COAXTARGET}/lib)
include_directories(${COAXHOME}/${COAXTARGET}/include)

set(OSDEF -DLINUX)
set(LIBDEF sbcom pthread rt)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(OSDEF -DLINUX -DMACOSX)
  set(LIBDEF sbcom pthread)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_definitions(-DSBC_HAS_IO -DDEFENSIVE -DSBC_HAS_COMM -DSBC_HAS_PTHREAD ${OSDEF})
rosbuild_add_executable(coax_server src/coax_server.cpp)
target_link_libraries(coax_server ${LIBDEF})

add_definitions(-DSBC_HAS_IO -DDEFENSIVE -DSBC_HAS_COMM -DSBC_HAS_PTHREAD ${OSDEF})
rosbuild_add_executable(coax_repeat src/coax_repeat.cpp)
target_link_libraries(coax_repeat ${LIBDEF})
