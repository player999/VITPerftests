#include <iostream>
#include <perf_opencv.h>
#include "perf_opencv_cl.h"

using namespace std;

int main() {
    try {
        CLCVImagePerfTest test_function = CLCVImagePerfTest(500, 500);
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