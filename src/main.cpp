#include <iostream>
#if defined(OPENCV_ORIGINAL)
# include <perf_opencv.h>
#endif

#if defined(OPENCV_INTEL_CPU) || defined(OPENCV_INTEL_GPU) | defined(OPENCV_NVIDIA_GPU)
# include <perf_opencv_cl.h>
#endif

#if defined(SDK)
# include <perf_vipm.h>
#endif

#if defined(AF_ORIGINAL) || defined(AF_CUDA) || defined(AF_INTEL_CPU) || defined(AF_INTEL_GPU) || defined(AF_NVIDIA_GPU)
# include <perf_arrayfire.h>
#endif

using namespace std;
#define RUN_COUNT 1
#define SQSIDE 40
#define IMWIDTH 500
#define IMHEIGHT 500

#if defined(OPENCV_ORIGINAL)
class test_boxfilter_cv : public CVImagePerfTest {
public:

    SET_NAME("Box filter OpenCV");

    test_boxfilter_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
    }

    void execute() {
        cv::boxFilter(wrappedSrcImageHost, wrappedDstImageHost, wrappedDstImageHost.depth(), cvSize(13, 13),
                      cvPoint(-1,-1), true, cv::BORDER_REFLECT101);
    }
};
#endif

#if defined(OPENCV_INTEL_CPU) || defined(OPENCV_INTEL_GPU) || defined(OPENCV_NVIDIA_GPU)
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
//            {
//                int devcount = af::devicecount();
//                char pname[255];
//                char dname[255];
//                char dtool[255];
//                char dcomp[255];
//
//                for (int i=0; i < devcount; i++) {
//                    af::setDevice(i);
//                    af::deviceprop(dname, pname, dtool, dcomp);
//                    cout<<dtool<<endl;
//                }
//                return 0;
//            }

#if defined(OPENCV_ORIGINAL)
        RUN_TEST(test_boxfilter_cv);
#endif

#if defined(OPENCV_INTEL_CPU)
        printf("CV OpenCL CPU\n");
        setenv("OPENCV_OPENCL_DEVICE",":CPU:", 1);
        RUN_TEST(test_boxfilter_cvcl);
#endif

#if defined(OPENCV_NVIDIA_GPU)
        printf("CV OpenCL GPU\n");
        setenv("OPENCV_OPENCL_DEVICE","NVIDIA CUDA:GPU:", 1);
        RUN_TEST(test_boxfilter_cvcl);
#endif

#if defined(SDK)
        RUN_TEST(test_boxfilter_vipm);
#endif

#if defined(AF_ORIGINAL)
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