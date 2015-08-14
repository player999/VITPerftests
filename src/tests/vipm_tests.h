//
// Created by vadym on 14.08.15.
//

#ifndef PERFTESTS_VIPM_TESTS_H
# define PERFTESTS_VIPM_TESTS_H

# include "perf_vipm.h"

class test_boxfilter_vipm : public VipmImagePerfTest {
 public:

  SET_NAME("Box filter VIPM")
  vodi_strel_shape shape;

  test_boxfilter_vipm() : VipmImagePerfTest(IMWIDTH, IMHEIGHT) {
    shape.sel_anchor.pi_x = 6;
    shape.sel_anchor.pi_y = 6;
    shape.sel_size.sz_height = 13;
    shape.sel_size.sz_width = 13;
    set_sq_side(SQSIDE);
    set_execution_count(RUN_COUNT);
  }

  void Execute() {
    VipmFilter(NULL, memstorage, wrappedDstImage, wrappedSrcImage, NULL, VipmK_BLUR_FILTER, &shape, NULL);
  }

};

class test_resize_vipm : public VipmImagePerfTest {
 public:

  SET_NAME("Resize 2x2 VIPM")

  test_resize_vipm() : VipmImagePerfTest(IMWIDTH,IMHEIGHT) {
    set_sq_side(SQSIDE);
    set_execution_count(RUN_COUNT);
    wrappedDstImage->img_width = wrappedSrcImage->img_width / 2;
    wrappedDstImage->img_height = wrappedSrcImage->img_height / 2;
    wrappedDstImage->img_wstride /= 2;
  }

  void Execute() {
    VipmResize(NULL, memstorage, wrappedDstImage, wrappedSrcImage, NULL, VipmK_CUBIC_INTERPOL, NULL);
  }

};

REGISTER_TEST(test_boxfilter_vipm);
REGISTER_TEST(test_resize_vipm);

#endif //PERFTESTS_VIPM_TESTS_H
