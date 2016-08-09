cmake_minimum_required(VERSION 3.5)
project(LoggingExample)

OPTION(LOGGING_ENABLED "Enable logging define" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

IF(LOGGING_ENABLED)
    add_definitions( -D__LOGGING_ENABLED__ )
ENDIF(LOGGING_ENABLED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Subscription.cpp Logger.cpp SimpleLogger.cpp main.cpp ../Singleton/Singleton.cpp)
add_executable(LoggingExample ${SOURCE_FILES})