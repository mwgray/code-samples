cmake_minimum_required(VERSION 3.5)
project(LoggingExample)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions( -D__LOGGING_ENABLED__ )

set(SOURCE_FILES Subscription.cpp Logger.cpp SimpleLogger.cpp main.cpp)
add_executable(LoggingExample ${SOURCE_FILES})