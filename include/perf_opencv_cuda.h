#ifndef PERFTESTS_PERF_OPENCV_CUDA_H
# define PERFTESTS_PERF_OPENCV_CUDA_H
# include "perf_opencv.h"
# include <opencv2/core/cuda.hpp>

class CUDACVImagePerfTest : public CVImagePerfTest {
public:

    CUDACVImagePerfTest(uint32_t h, uint32_t w);
    void UploadToDevice();
    void DownloadFromDevice();

    cv::cuda::GpuMat wrappedSrcImageDevice;
    cv::cuda::GpuMat wrappedDstImageDevice;
};
#endif //PERFTESTS_PERF_OPENCV_CUDA_H
