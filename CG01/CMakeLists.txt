cmake_minimum_required(VERSION 3.6)
project(OpenGL)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/Users/brennotondato/Documents/Tutorials/OpenGL")
add_executable(OpenGL ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(OpenGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})