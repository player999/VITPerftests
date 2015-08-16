//
// Created by vadym on 14.08.15.
//

#ifndef PERFTESTS_CLCV_TESTS_H
# define PERFTESTS_CLCV_TESTS_H

# include "perf_opencv_cl.h"

template<CLCVImagePerfTest::DeviceType dtype>
struct DeviceSelector {
    DeviceSelector() { CLCVImagePerfTest::SetOpenCLDevice(dtype); }
};

template <CLCVImagePerfTest::DeviceType dtype>
class test_boxfilter_cvcl : DeviceSelector<dtype>, public CLCVImagePerfTest {
public:

  SET_NAME("Box filter OpenCV with OpenCL");

  test_boxfilter_cvcl() : CLCVImagePerfTest(IMWIDTH, IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
  }

  void Execute() {
      cv::boxFilter(wrappedSrcImageDevice, wrappedDstImageDevice,
          wrappedDstImageDevice.depth(), cvSize(13,13), cvPoint(-1,-1), true,
          cv::BORDER_REFLECT101);
  }
};

#if defined(CV_INTEL_CPU)
REGISTER_TEST(test_boxfilter_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_CPU>);
#endif

#if define defined(CV_INTEL_GPU) || defined(CV_NVIDIA_GPU)
REGISTER_TEST(test_boxfilter_cvcl<CLCVImagePerfTest::DeviceType::CV_CL_GPU>);
#endif

#endif //PERFTESTS_CLCV_TESTS_H
