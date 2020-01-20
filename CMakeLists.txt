cmake_minimum_required(VERSION 2.8)

project(Potree)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


set(LASZIP_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/3rd_Laz")
set(LASZIP_LIBRARY "${CMAKE_SOURCE_DIR}/3rd_Laz/liblaszip.so")

add_subdirectory(PotreeConverter)
