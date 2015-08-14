#ifndef PERFTESTS_PERF_OPENCV_CLGPU_H
# define PERFTESTS_PERF_OPENCV_CLGPU_H
# include "perf_opencv.h"
# include <opencv2/core/ocl.hpp>

class CLCVImagePerfTest : public CVImagePerfTest {
public:
    CLCVImagePerfTest(uint32_t h, uint32_t w);
    void UploadToDevice();
    void DownloadFromDevice();

    virtual void SetOpenCLDevice() = 0;

public:
    cv::UMat wrappedSrcImageDevice;
    cv::UMat wrappedDstImageDevice;
};

class CLCVGPUImagePerfTest : public CLCVImagePerfTest {
public:
    CLCVGPUImagePerfTest(uint32_t h, uint32_t w);
    void SetOpenCLDevice();
};

class CLCVCPUImagePerfTest : public CLCVImagePerfTest {
public:
    CLCVCPUImagePerfTest(uint32_t h, uint32_t w);
    void SetOpenCLDevice();
};

#endif //PERFTESTS_PERF_OPENCV_CLGPU_H
