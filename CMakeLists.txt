cmake_minimum_required(VERSION 3.2)
project(Perftests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include src/gtest /home/player999/Work/opencv3/include)
link_directories(/home/player999/Work/opencv3/build/lib)
link_libraries(opencv_imgcodecs opencv_imgproc opencv_core OpenCL pthread)

set(SOURCE_FILES src/main.cpp src/perftests.cpp src/perf_opencv_cl.cpp src/perf_opencv.cpp include/perftests.h src/gtest/src/gtest-all.cc)
add_executable(perftests ${SOURCE_FILES} )