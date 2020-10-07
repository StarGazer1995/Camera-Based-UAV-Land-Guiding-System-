cmake_minimum_required(VERSION 3.17)
project(specialCourse)

set(CMAKE_CXX_STANDARD 11)
set(OpenCV_DIR "/usr/local/opencv4/share/opencv4")
add_executable(specialCourse cpp/src/main.cpp cpp/headers/CalPhoto.h)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
target_link_libraries(specialCourse ${OpenCV_LIBS})