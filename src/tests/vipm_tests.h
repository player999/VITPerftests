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

class test_erode_vipm : public VipmImagePerfTest {
public:

    SET_NAME("Erode VIPM")
    struct vodi_matrix state;
    vodi_point_t anchor;
    struct vodi_matrix strel_matrix;
    vodi_array_t strel;

    test_erode_vipm() : VipmImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        anchor.pi_x = 6;
        anchor.pi_y = 0;
        unsigned char strel_data[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                        0x00, 0x00, 0x00};
        struct vodi_matparm p;
        _VODI_MATPARM_U8(p, 1, 13, 13, 1);
        strel = _VodiMATinitheader(&strel_matrix, &p, NULL);
        vodi_array_p(strel)->ipar_base = (bo_pointer_t)strel_data;
        VipmInitmorphstate(NULL, memstorage, &state, VipmK_BASIC_MORPH, wrappedSrcImage, strel, &anchor, NULL);
    }

    void Execute() {
        VipmMorphop_1(NULL, memstorage, &state, VipmK_MORPH_ERODE, wrappedDstImage, wrappedSrcImage, NULL, NULL);
    }

};

class test_otsu_vipm : public VipmImagePerfTest {
public:

    SET_NAME("Otsu VIPM")
    struct vipm_threshopts opts;
    struct vipm_threshparm parms[2];

    test_otsu_vipm() : VipmImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        opts.mtho_method = VipmF_THRESH_OTSU_METHOD;
        opts.otsu.mtho_vrange[0] = VipmK_THRESH_DFL_VRANGE;
        opts.otsu.mtho_global_factor = 1;
        opts.otsu.mtho_local_factor = 0;
        parms[0].mthp_cmpop = VipmK_LEQU_CMP;
        parms[1].mthp_cmpop = VipmK_GREATER_CMP;
        parms[0].mthp_val[0] = 0;
        parms[1].mthp_val[0] = 255;
    }

    void Execute() {
        VipmThreshold(NULL, memstorage, wrappedDstImage, wrappedSrcImage, NULL, &opts, 0, NULL);
    }

};

class test_hist_vipm : public VipmImagePerfTest {
public:

    SET_NAME("Histogram Vipm")
    NO_OUTPUT_IMAGE

    struct vipm_histogram hist[1];
    struct vipm_hginitarg iarg;

    test_hist_vipm() : VipmImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        iarg.mhgp_elemtype = _VodiK_ELEMTYPE_UCHAR;
        iarg.mhgp_numscalepoints = VipmK_STD_IVRANGE_NSCALEPT;
        iarg.mhgp_valrange[0] = VipmK_STD_IVRANGE_MIN;
        iarg.mhgp_valrange[1] = VipmK_STD_IVRANGE_MAX;
        iarg.mhgp_datasz.sz_width = wrappedSrcImage->img_width;
        iarg.mhgp_datasz.sz_height = wrappedSrcImage->img_height;
    }

    void Execute() {
        VipmInitHist(NULL, memstorage, VipmK_HG_CALCULATE, &hist[0], &iarg, NULL);
        VipmHistogram(NULL, memstorage, VipmK_HG_CALCULATE, hist, wrappedSrcImage, 0, NULL);
    }

};

class test_compare_vipm : public VipmImagePerfTest {
public:

    SET_NAME("Compare Vipm")
    float rhs = 128.0f;
    test_compare_vipm() : VipmImagePerfTest(IMWIDTH,IMHEIGHT) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        VipmCmp_c(NULL, memstorage, wrappedDstImage, wrappedSrcImage, &rhs, VipmK_GEQU_CMP, NULL);
    }

};

REGISTER_TEST(test_boxfilter_vipm);
REGISTER_TEST(test_resize_vipm);
REGISTER_TEST(test_erode_vipm);
REGISTER_TEST(test_otsu_vipm);
REGISTER_TEST(test_hist_vipm);
REGISTER_TEST(test_compare_vipm);

#endif //PERFTESTS_VIPM_TESTS_H
