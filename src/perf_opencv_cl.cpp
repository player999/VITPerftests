
#include "perf_opencv_cl.h"
#include <CL/cl.hpp>

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

void CLCVImagePerfTest::UploadToDevice() {
    wrappedSrcImageHost.copyTo(wrappedSrcImageDevice);
}

void CLCVImagePerfTest::DownloadFromDevice() {
    wrappedDstImageDevice.copyTo(wrappedDstImageHost);
}

/*
 * OpenCV + OpenCL + GPU
 */

CLCVGPUImagePerfTest::CLCVGPUImagePerfTest(uint32_t h, uint32_t w)
    : CLCVImagePerfTest(h, w) {
}

void CLCVGPUImagePerfTest::SetOpenCLDevice() {
    setenv("OPENCV_OPENCL_DEVICE",":GPU:", 0);
    cvocl::setUseOpenCL(true);
    if (!cvocl::useOpenCL())
        throw("Processing unit not found");
}

/*
 * OpenCV + OpenCL + CPU
 */

CLCVCPUImagePerfTest::CLCVCPUImagePerfTest(uint32_t h, uint32_t w)
    : CLCVImagePerfTest(h, w) {
}

void CLCVCPUImagePerfTest::SetOpenCLDevice() {
    setenv("OPENCV_OPENCL_DEVICE",":CPU:", 0);
    cvocl::setUseOpenCL(true);
    if (!cvocl::useOpenCL())
        throw("Processing unit not found");
}