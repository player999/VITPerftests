#ifndef PERFTESTS_PERF_OPENCV_CLGPU_H
# define PERFTESTS_PERF_OPENCV_CLGPU_H
# include <perf_opencv.h>
# include <opencv2/core/ocl.hpp>

class CLCVImagePerfTest : public CVImagePerfTest {
public:
    CLCVImagePerfTest(uint32_t h, uint32_t w);
    void uploadToDevice();
    void downloadFromDevice();
    cv::UMat wrappedSrcImageDevice;
    cv::UMat wrappedDstImageDevice;
};

#endif //PERFTESTS_PERF_OPENCV_CLGPU_H
