#ifndef PERFTESTS_PERF_OPENCV_CLGPU_H
# define PERFTESTS_PERF_OPENCV_CLGPU_H
# include "perf_opencv.h"
# include <opencv2/core/ocl.hpp>

class CLCVImagePerfTest : public CVImagePerfTest {
public:
    enum DeviceType {
        CV_CL_CPU,
        CV_CL_GPU
    };

    CLCVImagePerfTest(uint32_t h, uint32_t w);
    void UploadToDevice();
    void DownloadFromDevice();
    static void SetOpenCLDevice(DeviceType dtype = CV_CL_CPU);

public:
    cv::UMat wrappedSrcImageDevice;
    cv::UMat wrappedDstImageDevice;
};

#endif //PERFTESTS_PERF_OPENCV_CLGPU_H
