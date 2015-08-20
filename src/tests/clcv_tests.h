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

    SET_NAME_CLCV("Morphology 1x13 OpenCV with OpenCL");

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
REGISTER_TEST(test_boxfilter_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_CPU>);
REGISTER_TEST(test_resize_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_CPU>);
REGISTER_TEST(test_integral_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_CPU>);
REGISTER_TEST(test_morphology_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_CPU>);
REGISTER_TEST(test_otsu_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_CPU>);
REGISTER_TEST(test_hist_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_CPU>);
REGISTER_TEST(test_compare_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_CPU>);
#endif

#if defined(CV_INTEL_GPU)
REGISTER_TEST(test_boxfilter_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_INTEL_GPU>);
REGISTER_TEST(test_resize_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_INTEL_GPU>);
REGISTER_TEST(test_integral_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_INTEL_GPU>);
REGISTER_TEST(test_morphology_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_INTEL_GPU>);
REGISTER_TEST(test_otsu_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_INTEL_GPU>);
REGISTER_TEST(test_hist_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_INTEL_GPU>);
REGISTER_TEST(test_compare_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_INTEL_GPU>);
#endif

#if defined(CV_NVIDIA_GPU)
REGISTER_TEST(test_boxfilter_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_NVIDIA_GPU>);
REGISTER_TEST(test_resize_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_NVIDIA_GPU>);
REGISTER_TEST(test_integral_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_NVIDIA_GPU>);
REGISTER_TEST(test_morphology_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_NVIDIA_GPU>);
REGISTER_TEST(test_otsu_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_NVIDIA_GPU>);
REGISTER_TEST(test_hist_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_NVIDIA_GPU>);
REGISTER_TEST(test_compare_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_NVIDIA_GPU>);
#endif

#endif //PERFTESTS_CLCV_TESTS_H
