
#ifndef PERFTESTS_CUDA_TESTS_H
# define PERFTESTS_CUDA_TESTS_H

# include "perf_opencv_cuda.h"
# include <opencv2/cudaimgproc.hpp>
# include <opencv2/cudawarping.hpp>
# include <opencv2/cudafilters.hpp>
# include <opencv2/cudaarithm.hpp>

class test_boxfilter_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("Boxfilter CUDA OpenCV 3");

    cv::Ptr<cv::cuda::Filter> filter;

    test_boxfilter_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        filter = cv::cuda::createBoxFilter(CV_8UC1, CV_8UC1, cv::Size(13, 13),
                                           cv::Point(-1, -1), cv::BORDER_REFLECT_101);
    }

    void Execute() {
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

class test_integral_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("Integral image CUDA OpenCV 3");

    test_integral_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        cv::cuda::integral(wrappedSrcImageDevice, wrappedDstImageDevice);
    }
};

class test_erode_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("Erode 13x1 CUDA OpenCV 3");

    cv::Mat kernel_host;
    cv::Ptr<cv::cuda::Filter> filter;

    test_erode_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        kernel_host = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 13), cv::Point(0, 6));
        filter =  cv::cuda::createMorphologyFilter(cv::MORPH_ERODE, CV_8UC1, kernel_host, cv::Point(0, 6), 1);
    }

    void Execute() {
        filter->apply(wrappedSrcImageDevice, wrappedDstImageDevice);
    }
};

class test_tophat_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("Tophat 13x1 CUDA OpenCV 3");

    cv::Mat kernel_host;
    cv::Ptr<cv::cuda::Filter> filter;

    test_tophat_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        kernel_host = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 13), cv::Point(0, 6));
        filter = cv::cuda::createMorphologyFilter(cv::MORPH_TOPHAT, CV_8UC1, kernel_host, cv::Point(0, 6), 1);
    }

    void Execute() {
        filter->apply(wrappedSrcImageDevice, wrappedDstImageDevice);
    }
};

class test_otsu_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("OTSU CUDA OpenCV 3");

    test_otsu_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        cv::cuda::threshold(wrappedSrcImageDevice, wrappedDstImageDevice, 128.0, 255.0, cv::THRESH_OTSU);
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

class test_compare_cudacv : public CUDACVImagePerfTest {
public:

    SET_NAME("Compare CUDA OpenCV 3");

    test_compare_cudacv() : CUDACVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        cv::cuda::compare(wrappedSrcImageDevice, 128, wrappedDstImageDevice, cv::CMP_GE);
    }
};

# define REGISTER_CVCUDA_TEST(test_type, classname) \
	REGISTER_TEST(TestPlatform::kOpenCV3CUDA, test_type, classname)

REGISTER_CVCUDA_TEST(kBoxFilter, test_boxfilter_cudacv);
REGISTER_CVCUDA_TEST(kResize, test_resize_cudacv);
REGISTER_CVCUDA_TEST(kIntegral, test_integral_cudacv);
REGISTER_CVCUDA_TEST(kMorphology, test_erode_cudacv);
REGISTER_CVCUDA_TEST(kTopHat, test_tophat_cudacv);
REGISTER_CVCUDA_TEST(kHist, test_calchist_cudacv);
REGISTER_CVCUDA_TEST(kCompare, test_compare_cudacv);

#endif //PERFTESTS_CUDA_TESTS_H
