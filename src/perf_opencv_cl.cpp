#include <stdlib.h>
#include "perf_opencv_cl.h"
#include <CL/cl.hpp>

#ifdef _WIN32
# define setenv(var, value, override) _putenv(var"="value"0")
#endif
namespace cvocl = cv::ocl;

CLCVImagePerfTest::CLCVImagePerfTest(uint32_t height, uint32_t width)
    : CVImagePerfTest(height, width) {
    SetOpenCLDevice();
    int rows = wrappedSrcImageHost.rows;
    int cols = wrappedSrcImageHost.cols;
    int type = wrappedSrcImageHost.type();
    cv::UMatUsageFlags flags =(cv::UMatUsageFlags)(
        cv::USAGE_ALLOCATE_DEVICE_MEMORY | cv::USAGE_ALLOCATE_HOST_MEMORY);
    wrappedDstImageDevice.create(rows, cols, type, flags);
}

void CLCVImagePerfTest::SetOpenCLDevice(CLCVImagePerfTest::DeviceType dtype) {
    if (dtype == CLCVImagePerfTest::DeviceType::CV_CL_CPU)
        setenv("OPENCV_OPENCL_DEVICE", ":CPU:", 0);
    else if (dtype == CLCVImagePerfTest::DeviceType::CV_CL_INTEL_GPU)
        setenv("OPENCV_OPENCL_DEVICE", "Intel Gen OCL Driver:GPU:", 0);
    else if (dtype == CLCVImagePerfTest::DeviceType::CV_CL_NVIDIA_GPU)
        setenv("OPENCV_OPENCL_DEVICE", "NVIDIA CUDA:GPU:", 0);
    else
        assert(0);

    //printf(getenv("OPENCV_OPENCL_DEVICE"));
    cvocl::setUseOpenCL(true);
    if (!cvocl::useOpenCL())
        throw std::runtime_error("Processing unit not found");
}

void CLCVImagePerfTest::UploadToDevice() {
    wrappedSrcImageHost.copyTo(wrappedSrcImageDevice);
}

void CLCVImagePerfTest::DownloadFromDevice() {
    wrappedDstImageDevice.copyTo(wrappedDstImageHost);
}
