
#ifndef PERFTESTS_CUDA_TESTS_H
# define PERFTESTS_CUDA_TESTS_H

# include "perf_opencv_cuda.h"
# include <opencv2/cudaimgproc.hpp>
# include <opencv2/cudawarping.hpp>

class test_calchist_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("Calc hist CUDA OpenCV 3");
    cv::cuda::GpuMat histogram;

    test_calchist_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        histogram = cv::cuda::GpuMat(1, 256, CV_32SC1);
    }

    void DownloadFromDevice() {}

    void Execute() {
        cv::cuda::calcHist(wrappedSrcImageDevice, histogram);
    }
};

class test_resize_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("Resize 2x2 CUDA OpenCV 3");

    test_resize_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        cv::cuda::resize(wrappedSrcImageDevice, wrappedDstImageDevice, cv::Size(), 0.5, 0.5, CV_INTER_CUBIC);
    }
};

REGISTER_TEST(test_calchist_cudacv);
REGISTER_TEST(test_resize_cudacv);

#endif //PERFTESTS_CUDA_TESTS_H
