#include <iostream>

using namespace std;

#if defined(CV_ORIGINAL)
# include "tests/opencv_tests.h"
#endif

#if defined(VIT_SDK)
# include "tests/vipm_tests.h"
#endif

#if defined(AF_ORIGINAL) || defined(AF_CUDA) || defined(AF_INTEL_CPU) || defined(AF_INTEL_GPU) || defined(AF_NVIDIA_GPU)
# include "tests/arrayfire_tests.h"
#endif

#if defined(CV_INTEL_CPU) || defined(CV_INTEL_GPU) || defined(CV_NVIDIA_GPU)
# include "tests/clcv_tests.h"
#endif

#if defined(CV_CUDA)
# include "tests/cuda_tests.h"
#endif

int main() {
    ImagePerfTest::RunAllTests();
    return 0;
}