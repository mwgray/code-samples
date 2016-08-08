cmake_minimum_required(VERSION 3.5)
project(WordCacheExample)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp WordCache.cpp)
add_executable(WordCacheExample ${SOURCE_FILES})