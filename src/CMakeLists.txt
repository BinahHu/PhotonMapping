cmake_minimum_required (VERSION 2.8)

project(PhotonMapping)

aux_source_directory(. DIR_SRCS)
add_definitions(-fopenmp -std=c++11)

#These two options are set because MaxOS use the fake g++——clang
#You can edit it based on your own systm environment
if(APPLE)
set(CMAKE_C_COMPILER "/opt/local/bin/gcc-mp-8")
set(CMAKE_CXX_COMPILER "/opt/local/bin/g++-mp-8")
endif()

set(CMAKE_EXE_LINKER_FLAGS " -fopenmp")

add_executable(PhotonMapping ${DIR_SRCS})
