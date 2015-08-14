#include <iostream>

#if defined(CV_ORIGINAL)
# include "perf_opencv.h"
#endif

#if defined(CV_INTEL_CPU) || defined(CV_INTEL_GPU) || defined(CV_NVIDIA_GPU)
# include "perf_opencv_cl.h"
#endif

#if defined(SDK)
# include <perf_vipm.h>
#endif

#if defined(AF_ORIGINAL) || defined(AF_CUDA) || defined(AF_INTEL_CPU) || defined(AF_INTEL_GPU) || defined(AF_NVIDIA_GPU)
# include "perf_arrayfire.h"
#endif

using namespace std;

#if defined(CV_ORIGINAL)
class test_boxfilter_cv : public CVImagePerfTest {
public:

    SET_NAME("Box filter OpenCV 3");

    test_boxfilter_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
    }

    void execute() {
        cv::boxFilter(wrappedSrcImageHost, wrappedDstImageHost, wrappedDstImageHost.depth(), cvSize(13, 13),
                      cvPoint(-1,-1), true, cv::BORDER_REFLECT101);
    }
};

class test_resize_cv : public CVImagePerfTest {
public:

    SET_NAME("Resize 2x2 OpenCV 3");

    test_resize_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
    }

    void execute() {
        cv::resize(wrappedSrcImageHost, wrappedDstImageHost, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
    }
};

class test_integral_cv : public CVImagePerfTest {
public:

    SET_NAME("Integral image OpenCV 3");

    test_integral_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
    }

    void execute() {
        cv::integral(wrappedSrcImageHost, wrappedDstImageHost, wrappedSrcImageHost.depth());
    }
};

class test_morphology_cv : public CVImagePerfTest {
public:

    SET_NAME("Erode image OpenCV 3");

    cv::Mat kernel;
    cv::Point anchor;

    test_morphology_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
        kernel = cv::Mat::ones(1, 13, CV_8UC1);
        anchor = cv::Point(-1, -1);
    }

    void execute() {
        cv::erode(wrappedSrcImageHost, wrappedDstImageHost, kernel, anchor, 1, cv::BORDER_REFLECT101);
    }
};

class test_otsu_cv : public CVImagePerfTest {
public:

    SET_NAME("Otsu threshold OpenCV 3");
    NO_OUTPUT_IMAGE

    double thresh;

    test_otsu_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
    }

    void execute() {
        thresh = cv::threshold(wrappedSrcImageHost, wrappedDstImageHost, 0, 255, cv::THRESH_OTSU | cv::THRESH_BINARY);
    }
};

class test_hist_cv : public CVImagePerfTest {
public:

    SET_NAME("Histogram OpenCV 3");
    NO_OUTPUT_IMAGE

    int histSize = 256;
    float histRange[2] = {0,256};
    const float *rangePtr = {histRange};

    test_hist_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
    }

    void execute() {
        cv::calcHist(&wrappedSrcImageHost, 1, 0, cv::Mat(), wrappedDstImageHost, 1, &histSize, &rangePtr,
                     true, false);
    }
};

class test_compare_cv : public CVImagePerfTest {
public:

    SET_NAME("Compare OpenCV 3");

    test_compare_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
    }

    void execute() {
        cv::compare(wrappedSrcImageHost, 0.5, wrappedDstImageHost, cv::CMP_GE);
    }
};

#endif

#if defined(CV_INTEL_CPU) || defined(CV_INTEL_GPU) || defined(CV_NVIDIA_GPU)
class test_boxfilter_cvcl : public CLCVImagePerfTest {
public:

    SET_NAME("Box filter OpenCV with OpenCL");

    test_boxfilter_cvcl() : CLCVImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
    }

    void execute() {
        cv::boxFilter(wrappedSrcImageDevice, wrappedDstImageDevice, wrappedDstImageDevice.depth(), cvSize(13,13),
                      cvPoint(-1,-1), true, cv::BORDER_REFLECT101);
    }
};
#endif

#if defined(SDK)
# include "perf_vipm.h"
class test_boxfilter_vipm : public VipmImagePerfTest {
public:

    SET_NAME("Box filter VIPM")
    vodi_strel_shape shape;

    test_boxfilter_vipm() : VipmImagePerfTest(IMWIDTH,IMHEIGHT) {
        shape.sel_anchor.pi_x = 6;
        shape.sel_anchor.pi_y = 6;
        shape.sel_size.sz_height = 13;
        shape.sel_size.sz_width = 13;
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
    }

    void execute() {
        VipmFilter(NULL, memstorage, wrappedDstImage, wrappedSrcImage, NULL, VipmK_BLUR_FILTER, &shape, NULL);
    }

};

class test_resize_vipm : public VipmImagePerfTest {
public:

    SET_NAME("Resize 2x2 VIPM")

    test_resize_vipm() : VipmImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
        wrappedDstImage->img_width = wrappedSrcImage->img_width / 2;
        wrappedDstImage->img_height = wrappedSrcImage->img_height / 2;
        wrappedDstImage->img_wstride /= 2;
    }

    void execute() {
        VipmResize(NULL, memstorage, wrappedDstImage, wrappedSrcImage, NULL, VipmK_CUBIC_INTERPOL, NULL);
    }

};
#endif

#if defined(AF_ORIGINAL) || defined(AF_CUDA) || defined(AF_INTEL_CPU) || defined(AF_INTEL_GPU) || defined(AF_NVIDIA_GPU)
class test_boxfilter_af : public AFImagePerfTest {
public:
    SET_NAME("Box filter Arrayfire");

    af::array kernel;

    test_boxfilter_af() : AFImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
        kernel = af::constant(1.0f/9.0f, 3, 3, f32);
    }

    void execute() {
        wrappedDstImageHost = af::convolve2(wrappedSrcImageHost, kernel);
    }
};
#endif

int main() {
    try {

#if defined(CV_ORIGINAL)
        RUN_TEST(test_boxfilter_cv);
        RUN_TEST(test_resize_cv);
        RUN_TEST(test_integral_cv);
        RUN_TEST(test_morphology_cv);
        RUN_TEST(test_otsu_cv);
        RUN_TEST(test_hist_cv);
        RUN_TEST(test_compare_cv);
#endif

#if defined(CV_INTEL_CPU)
        printf("CV OpenCL CPU\n");
        setenv("OPENCV_OPENCL_DEVICE",":CPU:", 1);
        RUN_TEST(test_boxfilter_cvcl);
#endif

#if defined(CV_NVIDIA_GPU)
        printf("CV OpenCL GPU\n");
        setenv("OPENCV_OPENCL_DEVICE","NVIDIA CUDA:GPU:", 1);
        RUN_TEST(test_boxfilter_cvcl);
#endif

#if defined(SDK)
        RUN_TEST(test_boxfilter_vipm);
        RUN_TEST(test_resize_vipm);
#endif

#if defined(AF_ORIGINAL)
        RUN_TEST(test_boxfilter_af);
#endif

#if defined(AF_CUDA)
        AFImagePerfTest::selectPlatform("NVIDIA_CUDA");
        RUN_TEST(test_boxfilter_af);
#endif

#if defined(AF_INTEL_CPU)
        AFImagePerfTest::selectPlatform("Intel(R) OpenCL");
        RUN_TEST(test_boxfilter_af);
#endif

#if defined(AF_INTEL_GPU)
        AFImagePerfTest::selectPlatform("Intel Gen OCL Driver");
        RUN_TEST(test_boxfilter_af);
#endif
    }
    catch (char const* e)
    {
        cout<<e<<endl;
        return -1;
    }

    return 0;
}