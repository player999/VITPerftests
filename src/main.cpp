#include <iostream>
#include <perf_opencv.h>
#include "perf_opencv_cl.h"

using namespace std;

class test_boxfilter : public CLCVImagePerfTest {
public:
    test_boxfilter() : CLCVImagePerfTest(500,500) { }
    void execute() {
        cv::boxFilter(wrappedSrcImageDevice, wrappedDstImageDevice, wrappedDstImageDevice.depth(), cvSize(13,13),
                      cvPoint(-1,-1), true, cv::BORDER_REFLECT101);
    }
    string name() {
        return string("Box filter");
    }
};

int main() {
    try {
        test_boxfilter test_function = test_boxfilter();
        cout<<test_function.name()<<endl;
        test_function.setSqSide(40);
        test_function.setExecutionCount(10000);
        test_function.writeSrcImage("picture_src.jpg");
        test_function.run();
        test_function.showAnalysis();
        test_function.writeDstImage("picture_dst.jpg");
    }
    catch (char const* e)
    {
        cout<<e<<endl;
    }

    return 0;
}