cmake_minimum_required(VERSION 2.8)
project(Perftests)

cmake_policy(SET CMP0015 OLD)

set(OPT_RUN_COUNT "100" CACHE STRING "Times to run test")
set(OPT_SQSIDE "40" CACHE STRING "Side of square on checker board")
set(OPT_IMWIDTH "500" CACHE STRING "Test image width")
set(OPT_IMHEIGHT "500" CACHE STRING "Test image height")

option(CV_ORIGINAL "Non-OpenCL OpenCV 3" OFF)
option(CV_INTEL_CPU "OpenCL on Intel CPU OpenCV 3" OFF)
option(CV_INTEL_GPU "OpenCL on Intel GPU OpenCV 3" OFF)
option(CV_NVIDIA_GPU "OpenCL on NVIDIA GPU OpenCV 3" OFF)

option(SDK "VIT AutoSDK 2.5 vipm module" ON)

option(AF_ORIGINAL "Arrayfire 3 build for CPU" OFF)
option(AF_CUDA "Arrayfire 3 build for CUDA" OFF)
option(AF_INTEL_CPU "Arrayfire 3 build for OpenCL for Intel CPU" OFF)
option(AF_INTEL_GPU "Arrayfire 3 build for OpenCL for Intel GPU" OFF)
option(AF_NVIDIA_GPU "Arrayfire 3 build for OpenCL for NVIDIA GPU" OFF)

# Default SDK paths
if(WIN32)
	set(SDK_INCPATH_PUBLIC_DFLT "D:/home/vadym/devel/opt_vs13_i386/include")
	set(SDK_INCPATH_DEVEL_DFLT "D:/home/vadym/devel/opt_vs13_i386/include/Devel")
	set(SDK_LIBPATH_DFLT "D:/home/vadym/devel/opt_vs13_i386/lib")
else()
	set(SDK_INCPATH_PUBLIC_DFLT "/opt1/include"	)
	set(SDK_INCPATH_DEVEL_DFLT "/src/Vodi2.world/trunk/src/include/Devel")
	set(SDK_LIBPATH_DFLT "/opt1/lib")
endif()

set(CV_INCPATH "/home/player999/Work/opencv3/include" CACHE PATH "OpenCV 3 include path")
set(SDK_INCPATH_PUBLIC "${SDK_INCPATH_PUBLIC_DFLT}" CACHE PATH "VIT AutoSDK 2.5 public include path" )
set(SDK_INCPATH_DEVEL "${SDK_INCPATH_DEVEL_DFLT}" CACHE PATH "VIT AutoSDK 2.5 development include path")
set(AF_INCPATH "/home/player999/Work/arrayfire/include" CACHE PATH "ArrayFire 3 include path")

set(SDK_INCPATH ${SDK_INCPATH} ${SDK_INCPATH_PUBLIC})
set(SDK_INCPATH ${SDK_INCPATH} ${SDK_INCPATH_DEVEL})

set(CV_LIBPATH "/home/player999/Work/opencv3/build/lib" CACHE PATH "OpenCV 3 Libraries path")
set(SDK_LIBPATH "${SDK_LIBPATH_DFLT}" CACHE PATH "AutoSDK 2.5 libraries path")
set(AF_LIBPATH "/usr/lib" CACHE PATH "Arreayfire 3 libraries path")

set(CV_LIBS opencv_imgcodecs opencv_imgproc opencv_core)
if(WIN32)
    set(SDK_LIBS Bo Vodi VodiUTILS)
else()
    set(SDK_LIBS Bo Vodi VodiUTILS pthread)
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


set(PERF_INCPATH "include")
set(PERF_LIBPATH "")
set(PERF_LIBS "")
set(SOURCE_FILES "")

if ((CV_ORIGINAL) OR ((CV_INTEL_CPU) OR (CV_INTEL_GPU OR CV_NVIDIA_GPU)))
    set(PERF_INCPATH ${PERF_INCPATH} ${CV_INCPATH})
    set(PERF_LIBPATH ${PERF_LIBPATH} ${CV_LIBPATH})
    set(PERF_LIBS ${PERF_LIBS} ${CV_LIBS})
endif((CV_ORIGINAL) OR ((CV_INTEL_CPU) OR (CV_INTEL_GPU OR CV_NVIDIA_GPU)))

if(CV_ORIGINAL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCV_ORIGINAL")
    set(SOURCE_FILES ${SOURCE_FILES} src/perf_opencv.cpp include/perf_opencv.h
        src/tests/opencv_tests.h)
endif(CV_ORIGINAL)

if((CV_INTEL_CPU) OR (CV_INTEL_GPU OR CV_NVIDIA_GPU))
    set(SOURCE_FILES ${SOURCE_FILES} src/perf_opencv_cl.cpp
        include/perf_opencv_cl.h src/tests/clcv_tests.h)
    set(PERF_LIBS ${PERF_LIBS} OpenCL)
endif((CV_INTEL_CPU) OR (CV_INTEL_GPU OR CV_NVIDIA_GPU))

if(CV_INTEL_CPU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCV_INTEL_CPU")
endif(CV_INTEL_CPU)

if(CV_INTEL_GPU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCV_INTEL_GPU")
endif(CV_INTEL_GPU)

if(CV_NVIDIA_GPU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCV_NVIDIA_GPU")
endif(CV_NVIDIA_GPU)

if(SDK)
    set(PERF_INCPATH "${PERF_INCPATH}" "${SDK_INCPATH}")
    set(PERF_LIBPATH ${PERF_LIBPATH} ${SDK_LIBPATH})
    set(PERF_LIBS ${PERF_LIBS} ${SDK_LIBS})
    set(SOURCE_FILES ${SOURCE_FILES} src/perf_vipm.cpp include/perf_vipm.h
        src/tests/vipm_tests.h)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSDK")
endif(SDK)

if((AF_ORIGINAL) OR ((AF_CUDA) OR ((AF_INTEL_CPU) OR (AF_INTEL_GPU OR AF_NVIDIA_GPU))))
    set(PERF_LIBPATH ${PERF_LIBPATH} ${AF_LIBPATH})
    set(PERF_INCPATH ${PERF_INCPATH} ${AF_INCPATH})
    set(SOURCE_FILES ${SOURCE_FILES} src/perf_arrayfire.cpp
        include/perf_arrayfire.h src/tests/arrayfire_tests.h)
endif((AF_ORIGINAL) OR ((AF_CUDA) OR ((AF_INTEL_CPU) OR (AF_INTEL_GPU OR AF_NVIDIA_GPU))))

if(AF_ORIGINAL)
    set(PERF_LIBS ${PERF_LIBS} afcpu)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAF_ORIGINAL")
elseif(AF_CUDA)
    set(PERF_LIBS ${PERF_LIBS} afcuda)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAF_CUDA")
elseif((AF_INTEL_CPU) OR (AF_INTEL_GPU OR AF_NVIDIA_GPU))
    set(PERF_LIBS ${PERF_LIBS} afopencl OpenCL)
    if((AF_INTEL_CPU) AND (NOT (AF_INTEL_GPU OR AF_NVIDIA_GPU)))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAF_INTEL_CPU")
    elseif((AF_INTEL_GPU) AND (NOT (AF_INTEL_CPU OR AF_NVIDIA_GPU)))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAF_INTEL_GPU")
    elseif((AF_NVIDIA_GPU) AND (NOT (AF_INTEL_CPU OR AF_INTEL_GPU)))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAF_NVIDIA_GPU")
    else((AF_INTEL_CPU) AND (NOT (AF_INTEL_GPU OR AF_NVIDIA_GPU)))
        message( FATAL_ERROR "You can choose only one option from AF_INTEL_CPU, AF_INTEL_GPU and AF_NVIDIA_GPU" )
    endif((AF_INTEL_CPU) AND (NOT (AF_INTEL_GPU OR AF_NVIDIA_GPU)))
endif(AF_ORIGINAL)

include_directories(${PERF_INCPATH})
link_directories(${PERF_LIBPATH})
link_libraries(${PERF_LIBS})

set(SOURCE_FILES ${SOURCE_FILES} src/perftests.cpp include/perftests.h src/main.cpp)
add_executable(perftests ${SOURCE_FILES})
