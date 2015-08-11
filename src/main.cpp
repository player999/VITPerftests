#include <iostream>
#include <perf_opencv.h>
#include <perf_opencv_cl.h>
#include <perf_vipm.h>

using namespace std;

class test_boxfilter_cvcl : public CLCVImagePerfTest {
public:

    SET_NAME("Box filter OpenCV with OpenCL");

    test_boxfilter_cvcl() : CLCVImagePerfTest(500,500) {
        setSqSide(40);
        setExecutionCount(10000);
    }

    void execute() {
        cv::boxFilter(wrappedSrcImageDevice, wrappedDstImageDevice, wrappedDstImageDevice.depth(), cvSize(13,13),
                      cvPoint(-1,-1), true, cv::BORDER_REFLECT101);
    }
};

class test_boxfilter_cv : public CVImagePerfTest {
public:

    SET_NAME("Box filter OpenCV");

    test_boxfilter_cv() : CVImagePerfTest(500,500) {
        setSqSide(40);
        setExecutionCount(10000);
    }

    void execute() {
        cv::boxFilter(wrappedSrcImageHost, wrappedDstImageHost, wrappedDstImageHost.depth(), cvSize(13, 13),
                      cvPoint(-1,-1), true, cv::BORDER_REFLECT101);
    }
};

class test_boxfilter_vipm : public VipmImagePerfTest {
public:

    SET_NAME("Box filter VIPM")
    vodi_strel_shape shape;

    test_boxfilter_vipm() : VipmImagePerfTest(500,500) {
        shape.sel_anchor.pi_x = 6;
        shape.sel_anchor.pi_y = 6;
        shape.sel_size.sz_height = 13;
        shape.sel_size.sz_width = 13;
        setSqSide(40);
        setExecutionCount(10000);
    }

    void execute() {
        VipmFilter(NULL, memstorage, wrappedDstImage, wrappedSrcImage, NULL, VipmK_BLUR_FILTER, &shape, NULL);
    }

};

int main() {
    try {
        RUN_TEST(test_boxfilter_cvcl);
        RUN_TEST(test_boxfilter_cv);
        RUN_TEST(test_boxfilter_vipm);

    }
    catch (char const* e)
    {
        cout<<e<<endl;
    }

    return 0;
}