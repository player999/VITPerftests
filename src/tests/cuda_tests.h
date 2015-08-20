
#ifndef PERFTESTS_CUDA_TESTS_H
# define PERFTESTS_CUDA_TESTS_H

# include "perf_opencv_cuda.h"
# include <opencv2/cudaimgproc.hpp>

class test_calchist_cudacv : public CVImagePerfTest {
public:

    SET_NAME("Box filter CUDA OpenCV 3");

    test_calchist_cudacv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        cv::cuda::calcHist(wrappedSrcImageHost, wrappedDstImageHost);
    }
};

REGISTER_TEST(test_calchist_cudacv);

#endif //PERFTESTS_CUDA_TESTS_H
