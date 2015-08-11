#include <iostream>
#include <perf_opencv.h>
#include <perf_opencv_cl.h>
#include <perf_vipm.h>
#include <perf_arrayfire.h>


using namespace std;
#define RUN_COUNT 1
#define SQSIDE 40
#define IMWIDTH 500
#define IMHEIGHT 500

//class test_boxfilter_cvcl : public CLCVImagePerfTest {
//public:
//
//    SET_NAME("Box filter OpenCV with OpenCL");
//
//    test_boxfilter_cvcl() : CLCVImagePerfTest(IMWIDTH,IMHEIGHT) {
//        setSqSide(SQSIDE);
//        setExecutionCount(RUN_COUNT);
//    }
//
//    void execute() {
//        cv::boxFilter(wrappedSrcImageDevice, wrappedDstImageDevice, wrappedDstImageDevice.depth(), cvSize(13,13),
//                      cvPoint(-1,-1), true, cv::BORDER_REFLECT101);
//    }
//};
//
//class test_boxfilter_cv : public CVImagePerfTest {
//public:
//
//    SET_NAME("Box filter OpenCV");
//
//    test_boxfilter_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
//        setSqSide(SQSIDE);
//        setExecutionCount(RUN_COUNT);
//    }
//
//    void execute() {
//        cv::boxFilter(wrappedSrcImageHost, wrappedDstImageHost, wrappedDstImageHost.depth(), cvSize(13, 13),
//                      cvPoint(-1,-1), true, cv::BORDER_REFLECT101);
//    }
//};
//
//class test_boxfilter_vipm : public VipmImagePerfTest {
//public:
//
//    SET_NAME("Box filter VIPM")
//    vodi_strel_shape shape;
//
//    test_boxfilter_vipm() : VipmImagePerfTest(IMWIDTH,IMHEIGHT) {
//        shape.sel_anchor.pi_x = 6;
//        shape.sel_anchor.pi_y = 6;
//        shape.sel_size.sz_height = 13;
//        shape.sel_size.sz_width = 13;
//        setSqSide(SQSIDE);
//        setExecutionCount(RUN_COUNT);
//    }
//
//    void execute() {
//        VipmFilter(NULL, memstorage, wrappedDstImage, wrappedSrcImage, NULL, VipmK_BLUR_FILTER, &shape, NULL);
//    }
//
//};
//

class test_boxfilter_af : public AFImagePerfTest {
public:
    SET_NAME("Box filter Arrayfire");

    af::array kernel;

    test_boxfilter_af() : AFImagePerfTest(IMWIDTH,IMHEIGHT) {
        setSqSide(SQSIDE);
        setExecutionCount(RUN_COUNT);
        kernel = af::gaussiankernel(13, 13, 1.0, 1.0);
//        try {
//            char *d = wrappedSrcImageHost.host<char>();
//            for (int i = 0; i < 300; i++) printf("%d ", d[i]);
//        }
//        catch (af::exception &e) {
//            cout<<e<<endl;
//            throw("Error in init");
//        }
    }

    void execute() {
        try {
            wrappedDstImageHost = af::convolve(wrappedSrcImageHost, kernel, true);
        }
        catch(af::exception& e) {
            cout<<"Error executing"<<endl<<e<<endl;
            throw("Error executing");
        }
    }
};


int main() {
    try {
//        RUN_TEST(test_boxfilter_af);
//        printf("CV OpenCL CPU\n");
//        setenv("OPENCV_OPENCL_DEVICE",":CPU:", 1);
//        RUN_TEST(test_boxfilter_cvcl);
//        printf("CV OpenCL GPU\n");
//        setenv("OPENCV_OPENCL_DEVICE","NVIDIA CUDA:GPU:", 1);
//        RUN_TEST(test_boxfilter_cvcl);
//        RUN_TEST(test_boxfilter_cv);
//        RUN_TEST(test_boxfilter_vipm);
        test_boxfilter_af tst = test_boxfilter_af();
        tst.writeSrcImage("check.jpg");
    }
    catch (char const* e)
    {
        cout<<e<<endl;
    }

    return 0;
}