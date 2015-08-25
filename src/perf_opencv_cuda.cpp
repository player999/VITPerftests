#include "perf_opencv_cuda.h"

namespace cu = cv::cuda;

CUDACVImagePerfTest::CUDACVImagePerfTest(uint32_t height, uint32_t width)
        : CVImagePerfTest(height, width) {
    int rows = wrappedSrcImageHost.rows;
    int cols = wrappedSrcImageHost.cols;
    int type = wrappedSrcImageHost.type();
    wrappedDstImageDevice = cu::GpuMat(rows, cols, type);
    wrappedSrcImageDevice = cu::GpuMat(rows, cols, type);
}

void CUDACVImagePerfTest::UploadToDevice() {
    wrappedSrcImageDevice.upload(wrappedSrcImageHost);
}

void CUDACVImagePerfTest::DownloadFromDevice() {
    wrappedDstImageDevice.download(wrappedDstImageHost);
}