cmake_minimum_required(VERSION 3.5)
project(TweenerExample)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Tween.cpp TweenCallback.cpp TweenEquation.cpp Tweener.cpp)
add_executable(TweenerExample ${SOURCE_FILES})