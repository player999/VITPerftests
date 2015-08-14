//
// Created by vadym on 14.08.15.
//

#ifndef PERFTESTS_ARRAYFIRE_TESTS_H
# define PERFTESTS_ARRAYFIRE_TESTS_H

# include "perf_arrayfire.h"

template<DeviceType dtype = kOriginal>
struct PlatformSelector {
    PlatformSelector() {
        AFImagePerfTest::SelectPlatform(AFImagePerfTest::devices_names[dtype]);
    }
};

template<DeviceType dtype = kOriginal>
class test_boxfilter_af : PlatformSelector<dtype>, public AFImagePerfTest {
 public:
  SET_NAME(std::string("Box filter Arrayfire: ") + devices_names[dtype]);

  af::array kernel;

  test_boxfilter_af() : AFImagePerfTest(IMWIDTH, IMHEIGHT) {
    set_sq_side(SQSIDE);
    set_execution_count(RUN_COUNT);
    kernel = af::constant(1.0f / 9.0f, 3, 3, f32);
  }

  void Execute() {
    wrappedDstImageHost = af::convolve2(wrappedSrcImageHost, kernel);
  }
};


#if defined(AF_ORIGINAL)
REGISTER_TEST(test_boxfilter_af<kOriginal>);
#endif

#if defined(AF_CUDA)
REGISTER_TEST(test_boxfilter_af<kNvidiaCUDA>);
#endif

#if defined(AF_INTEL_CPU)
REGISTER_TEST(test_boxfilter_af<kIntelOpenCL>);
#endif

#if defined(AF_INTEL_GPU)
REGISTER_TEST(test_boxfilter_af<kIntelGenOCLDriver>);
#endif


#endif //PERFTESTS_ARRAYFIRE_TESTS_H
