
#ifndef PERFTESTS_CUDA_TESTS_H
# define PERFTESTS_CUDA_TESTS_H

# include "perf_opencv_cuda.h"
# include <opencv2/cudaimgproc.hpp>
# include <opencv2/cudawarping.hpp>
# include <opencv2/cudafilters.hpp>

class test_boxfilter_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("Boxfilter CUDA OpenCV 3");

    test_boxfilter_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        auto filter = cv::cuda::createBoxFilter(CV_8UC1, CV_8UC1, cv::Size(13, 13),
                          cv::Point(-1, -1), cv::BORDER_REFLECT_101);
        filter->apply(wrappedSrcImageDevice, wrappedDstImageDevice);
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

class test_erode_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("Erode 13x1 CUDA OpenCV 3");

    cv::Mat kernel_host;

    test_erode_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        kernel_host = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 13), cv::Point(0, 6));
    }

    void Execute() {
        auto filter =  cv::cuda::createMorphologyFilter(cv::MORPH_ERODE, CV_8UC1, kernel_host, cv::Point(0, 6), 1);
        filter->apply(wrappedSrcImageDevice, wrappedDstImageDevice);
    }
};

class test_tophat_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("Tophat 13x1 CUDA OpenCV 3");

    cv::Mat kernel_host;

    test_tophat_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        kernel_host = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 13), cv::Point(0, 6));

    void Execute() {
        auto filter = cv::cuda::createMorphologyFilter(cv::MORPH_TOPHAT, CV_8UC1, kernel_host, cv::Point(0, 6), 1);
        filter->apply(wrappedSrcImageDevice, wrappedDstImageDevice);
    }
};

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

REGISTER_TEST(test_boxfilter_cudacv);
REGISTER_TEST(test_resize_cudacv);
//REGISTER_TEST(test_integral_cudacv); Does not exist in cv::cuda
REGISTER_TEST(test_erode_cudacv);
REGISTER_TEST(test_tophat_cudacv);
//REGISTER_TEST(test_otsu_cudacv); Does not exist in cv::cuda
REGISTER_TEST(test_calchist_cudacv);
//REGISTER_TEST(test_compare_cudacv); Does not exist in cv::cuda

#endif //PERFTESTS_CUDA_TESTS_H
