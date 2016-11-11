cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project("Pi2 Robot Controller")

set(CMAKE_CXX_FLAGS "-std=c++0x -I/usr/local/include -L/usr/local/lib -lcppcms -lbooster -lwiringPi -lraspicam -lraspicam_cv")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenCV REQUIRED)
add_subdirectory(pi2core)

add_executable(testExec main.cc)
target_link_libraries(testExec pi2core cppcms ${OpenCV_LIBS} ${raspicam_LIBS} pthread)
