cmake_minimum_required(VERSION 3.5)
project(WordCacheExample)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp WordCache.cpp ../Logging/SimpleLogger.cpp ../Logging/Logger.cpp ../Logging/Subscription.cpp ../Singleton/Singleton.cpp)
add_executable(WordCacheExample ${SOURCE_FILES})