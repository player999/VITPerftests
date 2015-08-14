//
// Created by vadym on 14.08.15.
//

#ifndef PERFTESTS_ARRAYFIRE_TESTS_H
# define PERFTESTS_ARRAYFIRE_TESTS_H

# include "arrayfire_tests.h"

class test_boxfilter_af : public AFImagePerfTest {
 public:
  SET_NAME("Box filter Arrayfire");

  af::array kernel;

  test_boxfilter_af() : AFImagePerfTest(IMWIDTH, IMHEIGHT) {
    setSqSide(SQSIDE);
    setExecutionCount(RUN_COUNT);
    kernel = af::constant(1.0f / 9.0f, 3, 3, f32);
  }

  void Execute() {
    wrappedDstImageHost = af::convolve2(wrappedSrcImageHost, kernel);
  }
};

//TODO(vadym) redesign
#if 0
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

#endif

#endif //PERFTESTS_ARRAYFIRE_TESTS_H
