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
  SET_NAME(std::string("Box filter Arrayfire 3: ") + devices_names[dtype]);

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

template<DeviceType dtype = kOriginal>
class test_resize_af : PlatformSelector<dtype>, public AFImagePerfTest {
public:
    SET_NAME(std::string("Resize 2x2 Arrayfire 3: ") + devices_names[dtype]);

    test_resize_af() : AFImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        wrappedDstImageHost = af::resize(0.5f, 0.5f, wrappedSrcImageHost, AF_INTERP_BILINEAR);
    }
};

template<DeviceType dtype = kOriginal>
class test_erode_af : PlatformSelector<dtype>, public AFImagePerfTest {
public:
#if defined(AF_ORIGINAL)
    SET_NAME(std::string("Erode 1x13 Arrayfire 3: ") + devices_names[dtype]);
#else
    SET_NAME(std::string("Erode 3x3 Arrayfire 3: ") + devices_names[dtype]);
#endif

    af::array mask;

    test_erode_af() : AFImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
#if defined(AF_ORIGINAL)
        mask =  af::constant(1.0f, 1, 13, f32);
#else
        mask =  af::constant(1.0f, 3, 3, f32);
#endif
    }

    void Execute() {
        wrappedDstImageHost = af::erode(wrappedSrcImageHost, mask);
    }
};

template<DeviceType dtype = kOriginal>
class test_otsu_af : PlatformSelector<dtype>, public AFImagePerfTest {
public:
    SET_NAME(std::string("Otsu Arrayfire 3: ") + devices_names[dtype]);

    test_otsu_af() : AFImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        // TODO: Does not work
        unsigned total = wrappedSrcImageHost.elements();
        af::array hist  = af::histogram(wrappedSrcImageHost, 256, 0.0f, 255.0f);
        af::array wts   = af::array(af::seq(256));
        af::array wtB   = af::accum(hist);
        af::array wtF   = total - wtB;
        af::array sumB  = af::accum(wts * hist);
        af::array meanB = sumB / wtB;
        af::array meanF = (sumB(255) - sumB) / wtF;
        af::array mDiff = meanB-meanF;

        af::array interClsVar= wtB * wtF * mDiff * mDiff;
        float max        = af::max<float>(interClsVar);
        float threshold2 = where(interClsVar == max).scalar<float>();
        af::array threshIdx  = where(interClsVar >= max);
        float threshold1 = threshIdx.elements() > 0 ? threshIdx.scalar<float>() : 0.0f;

        wrappedDstImageHost = 255.0f*(wrappedSrcImageHost > ((threshold1 + threshold2) / 2.0f));
    }
};

template<DeviceType dtype = kOriginal>
class test_hist_af : PlatformSelector<dtype>, public AFImagePerfTest {
public:
    SET_NAME(std::string("Histogram Arrayfire 3: ") + devices_names[dtype]);
    NO_OUTPUT_IMAGE

    test_hist_af() : AFImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        af::array hist  = af::histogram(wrappedSrcImageHost, 256, 0.0f, 255.0f);
    }
};

template<DeviceType dtype = kOriginal>
class test_compare_af : PlatformSelector<dtype>, public AFImagePerfTest {
public:
    SET_NAME(std::string("Compare Arrayfire 3: ") + devices_names[dtype]);

    test_compare_af() : AFImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        wrappedDstImageHost = 255.0f * (wrappedSrcImageHost >= 128.0);
    }
};

//@TODO: Rewrite me in new manner
#if defined(AF_ORIGINAL) || defined(AF_CUDA)
REGISTER_TEST(test_boxfilter_af<kOriginal>);
REGISTER_TEST(test_resize_af<kOriginal>);
REGISTER_TEST(test_erode_af<kOriginal>);
// REGISTER_TEST(test_otsu_af<kOriginal>);
REGISTER_TEST(test_hist_af<kOriginal>);
REGISTER_TEST(test_compare_af<kOriginal>);
#endif

#if defined(AF_NVIDIA_GPU)
REGISTER_TEST(test_boxfilter_af<kNvidiaCUDA>);
REGISTER_TEST(test_resize_af<kNvidiaCUDA>);
REGISTER_TEST(test_erode_af<kNvidiaCUDA>);
// REGISTER_TEST(test_otsu_af<kNvidiaCUDA>);
REGISTER_TEST(test_hist_af<kNvidiaCUDA>);
REGISTER_TEST(test_compare_af<kNvidiaCUDA>);
#endif

#if defined(AF_INTEL_CPU)
REGISTER_TEST(test_boxfilter_af<kIntelOpenCL>);
REGISTER_TEST(test_resize_af<kIntelOpenCL>);
REGISTER_TEST(test_erode_af<kIntelOpenCL>);
// REGISTER_TEST(test_otsu_af<kIntelOpenCL>);
REGISTER_TEST(test_hist_af<kIntelOpenCL>);
REGISTER_TEST(test_compare_af<kIntelOpenCL>);
#endif

#if defined(AF_INTEL_GPU)
REGISTER_TEST(test_boxfilter_af<kIntelGenOCLDriver>);
REGISTER_TEST(test_resize_af<kIntelGenOCLDriver>);
REGISTER_TEST(test_erode_af<kIntelGenOCLDriver>);
// REGISTER_TEST(test_otsu_af<kIntelGenOCLDriver>);
REGISTER_TEST(test_hist_af<kIntelGenOCLDriver>);
REGISTER_TEST(test_compare_af<kIntelGenOCLDriver>);
#endif


#endif //PERFTESTS_ARRAYFIRE_TESTS_H
