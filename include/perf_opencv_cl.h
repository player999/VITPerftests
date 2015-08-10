#ifndef PERFTESTS_PERF_OPENCV_CLGPU_H
# define PERFTESTS_PERF_OPENCV_CLGPU_H
# include <perf_opencv.h>
# include <opencv2/core/ocl.hpp>

class CLCVImagePerfTest : public CvImagePerfTest {
private:
    void execute();
    cv::UMat wrappedSrcImageDevice;
    cv::UMat wrappedDstImageDevice;
public:
    CLCVImagePerfTest(uint32_t h, uint32_t w);
    void uploadToDevice();
    void downloadFromDevice();

};

#endif //PERFTESTS_PERF_OPENCV_CLGPU_H
