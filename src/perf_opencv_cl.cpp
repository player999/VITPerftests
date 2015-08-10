
#include "perf_opencv_cl.h"
#include <CL/cl.hpp>

namespace cvocl = cv::ocl;

CLCVImagePerfTest::CLCVImagePerfTest(uint32_t height, uint32_t width) : CvImagePerfTest(height, width) {
    setenv("OPENCV_OPENCL_DEVICE",":CPU:",0);
    cvocl::setUseOpenCL(true);
    if (!cvocl::useOpenCL()) throw("Processing unit not found");
    int rows = wrappedSrcImageHost.rows;
    int cols = wrappedSrcImageHost.cols;
    int type = wrappedSrcImageHost.type();
    cv::UMatUsageFlags flags =(cv::UMatUsageFlags)(cv::USAGE_ALLOCATE_DEVICE_MEMORY | cv::USAGE_ALLOCATE_HOST_MEMORY);
    wrappedDstImageDevice.create(rows, cols, type, flags);
}

void CLCVImagePerfTest::uploadToDevice() {
    wrappedSrcImageHost.copyTo(wrappedSrcImageDevice);
}

void CLCVImagePerfTest::downloadFromDevice() {
    wrappedDstImageDevice.copyTo(wrappedDstImageHost);
}

void CLCVImagePerfTest::execute() {
    cv::boxFilter(wrappedSrcImageDevice, wrappedDstImageDevice, wrappedDstImageDevice.depth(), cvSize(13,13),
                  cvPoint(-1,-1), true, cv::BORDER_REFLECT101);
}