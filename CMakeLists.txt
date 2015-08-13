cmake_minimum_required(VERSION 3.2)
project(Perftests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes -Wno-format-security")

include_directories(include src/gtest /home/player999/Work/opencv3/include  /opt1/include /src/Vodi2.world/trunk/src/include/Devel /home/player999/Work/arrayfire/include)
link_directories(/home/player999/Work/opencv3/build/lib  /opt1/lib)
link_libraries(opencv_imgcodecs opencv_imgproc opencv_core OpenCL afopencl pthread Bo Vodi VodiUTILS)

set(SOURCE_FILES src/main.cpp src/perftests.cpp src/perf_opencv_cl.cpp src/perf_opencv.cpp src/perf_vipm.cpp src/perf_arrayfire.cpp)
add_executable(perftests ${SOURCE_FILES} src/perf_arrayfire.cpp)