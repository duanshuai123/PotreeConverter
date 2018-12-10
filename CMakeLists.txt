cmake_minimum_required(VERSION 2.8)

project(Potree)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

#if(UNIX)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -pthread -lstdc++ -lstdc++fs -lm")
#    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lstdc++fs" )
#endif()

set(LASZIP_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/3rd_Laz")
set(LASZIP_LIBRARY "${CMAKE_SOURCE_DIR}/3rd_Laz/liblaszip.so")

add_subdirectory(PotreeConverter)
