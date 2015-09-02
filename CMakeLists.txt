cmake_minimum_required(VERSION 2.8)
project(Perftests)

cmake_policy(SET CMP0015 OLD)

set(OPT_RUN_COUNT "100" CACHE STRING "Times to run test")
set(OPT_SQSIDE "40" CACHE STRING "Side of square on checker board")
set(OPT_IMWIDTH "500" CACHE STRING "Test image width")
set(OPT_IMHEIGHT "500" CACHE STRING "Test image height")

# Todo(vadym) On Windows need to configure OpenCL paths
option(CV_ORIGINAL "Non-OpenCL OpenCV 3" ON)
option(CV_INTEL_CPU "OpenCL on Intel CPU OpenCV 3" ON)
option(CV_INTEL_GPU "OpenCL on Intel GPU OpenCV 3" ON)
option(CV_NVIDIA_GPU "OpenCL on NVIDIA GPU OpenCV 3" OFF)
option(CV_CUDA "CUDA on NVIDIA GPU OpenCV 3" ON)

option(VIT_SDK "VIT AutoSDK 2.5 vipm module" OFF)

option(AF_ORIGINAL "Arrayfire 3 build for CPU" OFF)
option(AF_CUDA "Arrayfire 3 build for CUDA" OFF)
option(AF_INTEL_CPU "Arrayfire 3 build for OpenCL for Intel CPU" OFF)
option(AF_INTEL_GPU "Arrayfire 3 build for OpenCL for Intel GPU" OFF)
option(AF_NVIDIA_GPU "Arrayfire 3 build for OpenCL for NVIDIA GPU" OFF)

# Default paths
if(WIN32)
    set(VIT_SDK_INCPATH_PUBLIC_DFLT "D:/home/vadym/devel/opt_vs13_i386/include")
    set(VIT_SDK_INCPATH_DEVEL_DFLT "D:/home/vadym/devel/opt_vs13_i386/include/Devel")
    set(VIT_SDK_LIBPATH_DFLT "D:/home/vadym/devel/opt_vs13_i386/lib")
    set(CV_CL_INCPATH_DFLT "C:/Program Files (x86)/Intel/OpenCL SDK/4.6/include")
    set(CV_CL_LIBPATH_DFLT "C:/Program Files (x86)/Intel/OpenCL SDK/4.6/lib/x86")
    set(CV_INCPATH_DFLT "C:/Opencv3/opencv/build/include")
    set(CV_LIBPATH_DFLT "C:/Opencv3/opencv/build/x86/vc12/lib")
else()
    set(VIT_SDK_INCPATH_PUBLIC_DFLT "/opt1/include"	)
    set(VIT_SDK_INCPATH_DEVEL_DFLT "/src/Vodi2.world/trunk/src/include/Devel")
    set(VIT_SDK_LIBPATH_DFLT "/opt1/lib")
    set(CV_INCPATH_DFLT "/home/player999/Work/opencv3/include")
    set(CV_LIBPATH_DFLT "/home/player999/Work/opencv3/build/lib")
endif()

set(CV_INCPATH "${CV_INCPATH_DFLT}" CACHE PATH "OpenCV 3 include path")
set(VIT_SDK_INCPATH_PUBLIC "${VIT_SDK_INCPATH_PUBLIC_DFLT}" CACHE PATH "VIT AutoVIT_SDK 2.5 public include path" )
set(VIT_SDK_INCPATH_DEVEL "${VIT_SDK_INCPATH_DEVEL_DFLT}" CACHE PATH "VIT AutoVIT_SDK 2.5 development include path")
set(AF_INCPATH "/home/player999/Work/arrayfire/include" CACHE PATH "ArrayFire 3 include path")
set(VIT_SDK_INCPATH ${VIT_SDK_INCPATH} ${VIT_SDK_INCPATH_PUBLIC})
set(VIT_SDK_INCPATH ${VIT_SDK_INCPATH} ${VIT_SDK_INCPATH_DEVEL})
if(WIN32)
    set(CV_CL_INCPATH "${CV_CL_INCPATH_DFLT}" CACHE PATH "OpenCL include path")
endif()

set(CV_LIBPATH "${CV_LIBPATH_DFLT}" CACHE PATH "OpenCV 3 Libraries path")
set(VIT_SDK_LIBPATH "${VIT_SDK_LIBPATH_DFLT}" CACHE PATH "AutoVIT_SDK 2.5 libraries path")
set(AF_LIBPATH "/usr/lib" CACHE PATH "Arreayfire 3 libraries path")
if(WIN32)
    set(CV_CL_LIBPATH "${CV_CL_LIBPATH_DFLT}" CACHE PATH "OpenCL libraries path")
    set(VIT_SDK_LIBS Bo Vodi VodiUTILS)
    if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
        set(CV_LIBS opencv_world300d.lib)
    else()
        set(CV_LIBS opencv_world300.lib)
    endif()
else()
    set(VIT_SDK_LIBS Bo Vodi VodiUTILS pthread)
    set(CV_LIBS opencv_imgcodecs opencv_imgproc opencv_core)
    set(CV_CUDA_LIBS opencv_cudaimgproc opencv_cudawarping opencv_cudafilters opencv_cudaarithm)
endif()

set(AF_LIBS "")

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes -Wno-format-security")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRUN_COUNT=${OPT_RUN_COUNT}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSQSIDE=${OPT_SQSIDE}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMWIDTH=${OPT_IMWIDTH}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMHEIGHT=${OPT_IMHEIGHT}")

#OpenCV3
if ((CV_CUDA) OR ((CV_ORIGINAL) OR ((CV_INTEL_CPU) OR (CV_INTEL_GPU OR CV_NVIDIA_GPU))))
    set(CV_INCLUDE ${CV_INCLUDE} ${CV_INCPATH})
    set(CV_LIBDIRS ${CV_LIBDIRS} ${CV_LIBPATH})
    set(CV_SOURCES ${CV_SOURCES} src/perf_opencv.cpp include/perf_opencv.h)
    set(CV_LIBS ${CV_LIBS} ${CV_LIBS})
endif((CV_CUDA) OR ((CV_ORIGINAL) OR ((CV_INTEL_CPU) OR (CV_INTEL_GPU OR CV_NVIDIA_GPU))))

if(CV_ORIGINAL)
    set(CV_FLAGS ${CV_FLAGS} CV_ORIGINAL)
    set(CV_SOURCES ${CV_SOURCES} src/tests/opencv_tests.h)
endif(CV_ORIGINAL)

if((CV_INTEL_CPU) OR (CV_INTEL_GPU OR CV_NVIDIA_GPU))
    set(CV_SOURCES ${CV_SOURCES} src/perf_opencv_cl.cpp
        include/perf_opencv_cl.h src/tests/clcv_tests.h)
    set(CV_LIBS ${CV_LIBS} OpenCL)
    if(WIN32)
        set(CV_INCLUDE ${CV_INCLUDE} ${CV_CL_INCPATH})
        set(CV_INCLUDE ${CV_INCLUDE} ${CV_CL_LIBPATH})
    endif()
endif((CV_INTEL_CPU) OR (CV_INTEL_GPU OR CV_NVIDIA_GPU))

if(CV_INTEL_CPU)
    set(CV_FLAGS ${CV_FLAGS} CV_INTEL_CPU)
endif(CV_INTEL_CPU)

if(CV_INTEL_GPU)
    set(CV_FLAGS ${CV_FLAGS} CV_INTEL_GPU)
endif(CV_INTEL_GPU)

if(CV_NVIDIA_GPU)
    set(CV_FLAGS ${CV_FLAGS} CV_NVIDIA_GPU)
endif(CV_NVIDIA_GPU)

if(CV_CUDA)
    set(CV_SOURCES ${CV_SOURCES} src/perf_opencv_cuda.cpp include/perf_opencv_cuda.h
        src/tests/cuda_tests.h)
    set(CV_LIBS ${CV_CUDA_LIBS} ${CV_LIBS})
    set(CV_FLAGS ${CV_FLAGS} CV_CUDA)
endif(CV_CUDA)

if ((CV_CUDA) OR ((CV_ORIGINAL) OR ((CV_INTEL_CPU) OR (CV_INTEL_GPU OR CV_NVIDIA_GPU))))
    link_directories(${CV_LIBDIRS})
    add_executable(perftests_opencv3 ${CV_SOURCES} src/perftests.cpp include/perftests.h src/main.cpp)
    target_include_directories(perftests_opencv3 PUBLIC ${PROJECT_SOURCE_DIR}/include ${CV_INCLUDE})
    target_link_libraries(perftests_opencv3 ${CV_LIBS})
    target_compile_definitions(perftests_opencv3 PUBLIC ${CV_FLAGS})
endif((CV_CUDA) OR ((CV_ORIGINAL) OR ((CV_INTEL_CPU) OR (CV_INTEL_GPU OR CV_NVIDIA_GPU))))

#SDK
if(VIT_SDK)
    link_directories(${VIT_SDK_LIBPATH})
    add_executable(perftests_vipm src/perf_vipm.cpp include/perf_vipm.h src/tests/vipm_tests.h src/perftests.cpp
                   include/perftests.h src/main.cpp)
    target_include_directories(perftests_vipm PUBLIC ${PROJECT_SOURCE_DIR}/include ${VIT_SDK_INCPATH})
    target_link_libraries(perftests_vipm ${VIT_SDK_LIBS})
    target_compile_definitions(perftests_vipm PUBLIC VIT_SDK)
endif(VIT_SDK)

#TODO: Rewrite for arrayfire
#Arrayfire
#if((AF_ORIGINAL) OR ((AF_CUDA) OR ((AF_INTEL_CPU) OR (AF_INTEL_GPU OR AF_NVIDIA_GPU))))
#    set(PERF_LIBPATH ${PERF_LIBPATH} ${AF_LIBPATH})
#    set(PERF_INCPATH ${PERF_INCPATH} ${AF_INCPATH})
#    set(SOURCE_FILES ${SOURCE_FILES} src/perf_arrayfire.cpp
#        include/perf_arrayfire.h src/tests/arrayfire_tests.h)
#endif((AF_ORIGINAL) OR ((AF_CUDA) OR ((AF_INTEL_CPU) OR (AF_INTEL_GPU OR AF_NVIDIA_GPU))))
#
#if(AF_ORIGINAL)
#    set(PERF_LIBS ${PERF_LIBS} afcpu)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAF_ORIGINAL")
#elseif(AF_CUDA)
#    set(PERF_LIBS ${PERF_LIBS} afcuda)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAF_CUDA")
#elseif((AF_INTEL_CPU) OR (AF_INTEL_GPU OR AF_NVIDIA_GPU))
#    set(PERF_LIBS ${PERF_LIBS} afopencl OpenCL)
#    if((AF_INTEL_CPU) AND (NOT (AF_INTEL_GPU OR AF_NVIDIA_GPU)))
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAF_INTEL_CPU")
#    elseif((AF_INTEL_GPU) AND (NOT (AF_INTEL_CPU OR AF_NVIDIA_GPU)))
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAF_INTEL_GPU")
#    elseif((AF_NVIDIA_GPU) AND (NOT (AF_INTEL_CPU OR AF_INTEL_GPU)))
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAF_NVIDIA_GPU")
#    else((AF_INTEL_CPU) AND (NOT (AF_INTEL_GPU OR AF_NVIDIA_GPU)))
#        message( FATAL_ERROR "You can choose only one option from AF_INTEL_CPU, AF_INTEL_GPU and AF_NVIDIA_GPU" )
#    endif((AF_INTEL_CPU) AND (NOT (AF_INTEL_GPU OR AF_NVIDIA_GPU)))
#endif(AF_ORIGINAL)


