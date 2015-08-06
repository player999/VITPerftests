#include <iostream>
#include <perf_opencv.h>

using namespace std;

int main() {
    try {
        CvImagePerfTest  test_function = CvImagePerfTest(510, 510);
        test_function.setSqSide(40);
        test_function.writeSrcImage("picture.jpg");
        test_function.run();
        test_function.showAnalysis();
    }
    catch (exception& e)
    {
        cout<<e.what()<<endl;
    }

    return 0;
}