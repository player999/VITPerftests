//
// Created by vadym on 14.08.15.
//

#ifndef PERFTESTS_CLCV_TESTS_H
# define PERFTESTS_CLCV_TESTS_H

# include "perf_opencv_cl.h"

# define SET_NAME_CLCV(x)\
std::string Name() const {\
    switch(dtype) {\
        case CLCVImagePerfTest::DeviceType::CV_CL_CPU:\
            return std::string(x " CPU" );\
        case CLCVImagePerfTest::DeviceType::CV_CL_INTEL_GPU:\
            return std::string(x " Intel GPU");\
        case CLCVImagePerfTest::DeviceType::CV_CL_NVIDIA_GPU:\
            return std::string(x " NVIDIA GPU");\
        default:\
            return std::string(x);\
    }\
}

template<CLCVImagePerfTest::DeviceType dtype>
struct DeviceSelector {
    DeviceSelector() { CLCVImagePerfTest::SetOpenCLDevice(dtype); }
};

template <CLCVImagePerfTest::DeviceType dtype>
class test_boxfilter_cvcl : DeviceSelector<dtype>, public CLCVImagePerfTest {
public:

  SET_NAME_CLCV("Box filter OpenCV with OpenCL");

  test_boxfilter_cvcl() : CLCVImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
  }

  void Execute() {
      cv::boxFilter(wrappedSrcImageDevice, wrappedDstImageDevice,
          wrappedDstImageDevice.depth(), cvSize(13,13), cvPoint(-1,-1), true,
          cv::BORDER_REFLECT101);
  }
};

template <CLCVImagePerfTest::DeviceType dtype>
class test_resize_cvcl : DeviceSelector<dtype>, public CLCVImagePerfTest {
public:

    SET_NAME_CLCV("Resize OpenCV 2x2 with OpenCL");

    test_resize_cvcl() : CLCVImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        cv::resize(wrappedSrcImageDevice, wrappedDstImageDevice, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
    }
};

template <CLCVImagePerfTest::DeviceType dtype>
class test_integral_cvcl : DeviceSelector<dtype>, public CLCVImagePerfTest {
public:

    SET_NAME_CLCV("Integral image OpenCV with OpenCL");

    test_integral_cvcl() : CLCVImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        cv::integral(wrappedSrcImageDevice, wrappedDstImageDevice, wrappedSrcImageDevice.depth());
    }
};

template <CLCVImagePerfTest::DeviceType dtype>
class test_morphology_cvcl : DeviceSelector<dtype>, public CLCVImagePerfTest {
public:

    SET_NAME_CLCV("Erode 1x13 OpenCV with OpenCL");

    cv::UMat kernel;
    cv::Point anchor;

    test_morphology_cvcl() : CLCVImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        kernel = cv::UMat::ones(1, 13, CV_8UC1);
        anchor = cv::Point(-1, -1);
    }

    void Execute() {
        cv::erode(wrappedSrcImageDevice, wrappedDstImageDevice, kernel, anchor, 1, cv::BORDER_REFLECT101);
    }
};

template <CLCVImagePerfTest::DeviceType dtype>
class test_tophat_cvcl : DeviceSelector<dtype>, public CLCVImagePerfTest {
public:

    SET_NAME_CLCV("Tophat 1x13 OpenCV with OpenCL");

    cv::UMat kernel;
    cv::Point anchor;

    test_tophat_cvcl() : CLCVImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        kernel = cv::UMat::ones(1, 13, CV_8UC1);
        anchor = cv::Point(-1, -1);
    }

    void Execute() {
        cv::morphologyEx(wrappedSrcImageDevice, wrappedDstImageDevice,
            CV_MOP_TOPHAT, kernel, anchor, 1, cv::BORDER_REFLECT101);
    }
};

template <CLCVImagePerfTest::DeviceType dtype>
class test_otsu_cvcl : DeviceSelector<dtype>, public CLCVImagePerfTest {
public:

    SET_NAME_CLCV("Otsu OpenCV with OpenCL");

    double thresh;

    test_otsu_cvcl() : CLCVImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        thresh = cv::threshold(wrappedSrcImageDevice, wrappedDstImageDevice, 0, 255,
                               cv::THRESH_OTSU | cv::THRESH_BINARY);
    }
};

template <CLCVImagePerfTest::DeviceType dtype>
class test_hist_cvcl : DeviceSelector<dtype>, public CLCVImagePerfTest {
public:

    SET_NAME_CLCV("Hist OpenCV with OpenCL");
    NO_OUTPUT_IMAGE

    test_hist_cvcl() : CLCVImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        int histSize = 256;
        const float hranges[2] = { 0.f, 256.f };
        const float *ranges[] = { hranges };
        cv::calcHist(&wrappedSrcImageHost, 1, 0, cv::UMat(), wrappedDstImageDevice, 1,
                     &histSize, ranges);
    }
};

template <CLCVImagePerfTest::DeviceType dtype>
class test_compare_cvcl : DeviceSelector<dtype>, public CLCVImagePerfTest {
public:

    SET_NAME_CLCV("Compare OpenCV with OpenCL");

    test_compare_cvcl() : CLCVImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        cv::compare(wrappedSrcImageDevice, 0.5, wrappedDstImageDevice, cv::CMP_GE);
    }
};

#if defined(CV_INTEL_CPU)
#  define REGISTER_CLCPU_TEST(test_type, classname) \
	REGISTER_TEST(TestPlatform::kOpenCVCLCPU3 , test_type, \
    classname<CLCVImagePerfTest::DeviceType::CV_CL_CPU>)
REGISTER_CLCPU_TEST(TestType::kBoxFilter, test_boxfilter_cvcl);
REGISTER_CLCPU_TEST(TestType::kResize, test_resize_cvcl);
REGISTER_CLCPU_TEST(TestType::kIntegral, test_integral_cvcl);
REGISTER_CLCPU_TEST(TestType::kMorphology, test_morphology_cvcl);
REGISTER_CLCPU_TEST(TestType::kTopHat, test_tophat_cvcl);
REGISTER_CLCPU_TEST(TestType::kOtsu, test_otsu_cvcl);
REGISTER_CLCPU_TEST(TestType::kHist, test_hist_cvcl);
REGISTER_CLCPU_TEST(TestType::kCompare, test_compare_cvcl);
#endif

#if defined(CV_INTEL_GPU)
#  define REGISTER_CLGPU_TEST(test_type, classname) \
	REGISTER_TEST(TestPlatform::kOpenCV3CLIntelGPU, test_type, \
    classname<CLCVImagePerfTest::DeviceType::CV_CL_INTEL_GPU>)
REGISTER_CLGPU_TEST(TestType::kBoxFilter, test_boxfilter_cvcl);
REGISTER_CLGPU_TEST(TestType::kResize, test_resize_cvcl);
REGISTER_CLGPU_TEST(TestType::kIntegral, test_integral_cvcl);
REGISTER_CLGPU_TEST(TestType::kMorphology, test_morphology_cvcl);
REGISTER_CLGPU_TEST(TestType::kTopHat, test_tophat_cvcl);
REGISTER_CLGPU_TEST(TestType::kOtsu, test_otsu_cvcl);
REGISTER_CLGPU_TEST(TestType::kHist, test_hist_cvcl);
REGISTER_CLGPU_TEST(TestType::kCompare, test_compare_cvcl);
#endif

#if defined(CV_NVIDIA_GPU)
#  define REGISTER_CLCUDAGPU_TEST(test_type, classname) \
	REGISTER_TEST(TestPlatform::kOpenCV3CLCudaGPU , test_type,
	classname<CLCVImagePerfTest::DeviceType::CV_CL_NVIDIA_GPU>)
REGISTER_CLCUDAGPU_TEST(TestType::kBoxFilter, test_boxfilter_cvcl);
REGISTER_CLCUDAGPU_TEST(TestType::kResize, test_resize_cvcl);
REGISTER_CLCUDAGPU_TEST(TestType::kIntegral, test_integral_cvcl);
REGISTER_CLCUDAGPU_TEST(TestType::kMorphology, test_morphology_cvcl);
REGISTER_CLCUDAGPU_TEST(TestType::kTopHat, test_tophat_cvcl);
REGISTER_CLCUDAGPU_TEST(TestType::kOtsu, test_otsu_cvcl);
REGISTER_CLCUDAGPU_TEST(TestType::kHist, test_hist_cvcl);
REGISTER_CLCUDAGPU_TEST(TestType::kCompare, test_compare_cvcl);
#endif

#endif //PERFTESTS_CLCV_TESTS_H
