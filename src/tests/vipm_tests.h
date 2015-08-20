//
// Created by vadym on 14.08.15.
//

#ifndef PERFTESTS_VIPM_TESTS_H
# define PERFTESTS_VIPM_TESTS_H

# include <stdlib.h>
# include "perf_vipm.h"

# define SET_VIPM_NAME(x) std::string Name() const { \
    return std::string(x) + " " + mod_names[vtype]; }

#define VIPM_TEST_CLASS(class_name) \
    template<VipmType vtype = VIPM_DEFAULT> \
    class class_name : public VipmImagePerfTest


VIPM_TEST_CLASS(test_boxfilter_vipm) {
public:
    SET_VIPM_NAME("Box filter VIPM");
    vodi_strel_shape shape;

    test_boxfilter_vipm() : VipmImagePerfTest(IMWIDTH, IMHEIGHT, vtype) {
        shape.sel_anchor.pi_x = 6;
        shape.sel_anchor.pi_y = 6;
        shape.sel_size.sz_height = 13;
        shape.sel_size.sz_width = 13;
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        VipmFilter(module_, memstorage_, wrappedDstImage_, wrappedSrcImage_, 
            NULL, VipmK_BLUR_FILTER, &shape, NULL);
    }

};

VIPM_TEST_CLASS(test_resize_vipm) {
public:

    SET_VIPM_NAME("Resize 2x2 VIPM")

    test_resize_vipm() : VipmImagePerfTest(IMWIDTH, IMHEIGHT, vtype) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        wrappedDstImage_->img_width = wrappedSrcImage_->img_width / 2;
        wrappedDstImage_->img_height = wrappedSrcImage_->img_height / 2;
        wrappedDstImage_->img_wstride /= 2;
    }

    void Execute() {
        VipmResize(module_, memstorage_, wrappedDstImage_, wrappedSrcImage_,
            NULL, VipmK_CUBIC_INTERPOL, NULL);
    }

};

VIPM_TEST_CLASS(test_erode_vipm) {
public:

    SET_VIPM_NAME("Erode VIPM")

    struct vodi_matrix state;
    vodi_point_t anchor;
    struct vodi_matrix strel_matrix;
    vodi_array_t strel;

    test_erode_vipm() : VipmImagePerfTest(IMWIDTH, IMHEIGHT, vtype) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);

        unsigned char strel_data[16] = { 
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00
        };
        struct vodi_matparm p;

        anchor.pi_x = 6;
        anchor.pi_y = 0;

        _VODI_MATPARM_U8(p, 1, 13, 13, 1);
        strel = _VodiMATinitheader(&strel_matrix, &p, NULL);
        vodi_array_p(strel)->ipar_base = (bo_pointer_t)strel_data;
        memset(&state, 0, (sizeof state));
        VipmInitmorphstate(module_, memstorage_, &state, VipmK_BASIC_MORPH,
            wrappedSrcImage_, strel, &anchor, NULL);
    }

    void Execute() {
        VipmMorphop_1(module_, memstorage_, &state, VipmK_MORPH_ERODE, 
            wrappedDstImage_, wrappedSrcImage_, NULL, NULL);
    }

};

VIPM_TEST_CLASS(test_otsu_vipm) {
public:

    SET_VIPM_NAME("Otsu VIPM")
    struct vipm_threshopts opts;
    struct vipm_threshparm parms[2];

    test_otsu_vipm() : VipmImagePerfTest(IMWIDTH, IMHEIGHT, vtype) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        opts.mtho_method = VipmF_THRESH_OTSU_METHOD;
        opts.otsu.mtho_vrange[0] = VipmK_THRESH_DFL_VRANGE;
        opts.otsu.mtho_global_fthresh = VipmK_INVAL_FTHRESH;
        opts.otsu.mtho_global_factor = 1;
        opts.otsu.mtho_local_factor = 0;
        parms[0].mthp_cmpop = VipmK_LEQU_CMP;
        parms[1].mthp_cmpop = VipmK_GREATER_CMP;
        parms[0].mthp_val[0] = 255;
        parms[1].mthp_val[0] = 0;
    }

    void Execute() {
        VipmThreshold(module_, memstorage_, wrappedDstImage_, 
            wrappedSrcImage_, NULL, &opts, 2, parms, NULL);
    }

};

VIPM_TEST_CLASS(test_hist_vipm) {
public:

    SET_VIPM_NAME("Histogram Vipm")
    NO_OUTPUT_IMAGE

    struct vipm_histogram hist[1];
    struct vipm_hginitarg iarg;

    test_hist_vipm() : VipmImagePerfTest(IMWIDTH, IMHEIGHT, vtype) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
        iarg.mhgp_elemtype = _VodiK_ELEMTYPE_UCHAR;
        iarg.mhgp_numscalepoints = VipmK_STD_IVRANGE_NSCALEPT;
        iarg.mhgp_valrange[0] = VipmK_STD_IVRANGE_MIN;
        iarg.mhgp_valrange[1] = VipmK_STD_IVRANGE_MAX;
        iarg.mhgp_datasz.sz_width = wrappedSrcImage_->img_width;
        iarg.mhgp_datasz.sz_height = wrappedSrcImage_->img_height;
    }

    void Execute() {
        VipmInitHist(module_, memstorage_, VipmK_HG_CALCULATE, &hist[0], &iarg, NULL);
        VipmHistogram(module_, memstorage_, VipmK_HG_CALCULATE, hist, wrappedSrcImage_, 0, NULL);
    }
};

VIPM_TEST_CLASS(test_compare_vipm) {
public:
    SET_VIPM_NAME("Compare VIPM")
    test_compare_vipm() : VipmImagePerfTest(IMWIDTH, IMHEIGHT, vtype) {
        set_sq_side(SQSIDE);
        set_execution_count(RUN_COUNT);
    }

    void Execute() {
        float rhs = 128.0f;
        VipmCmp_c(module_, memstorage_, wrappedDstImage_, wrappedSrcImage_,
            &rhs, VipmK_GEQU_CMP, NULL);
    }

};

REGISTER_TEST(test_boxfilter_vipm<VIPM_IPP>);
REGISTER_TEST(test_resize_vipm<VIPM_IPP>);
REGISTER_TEST(test_erode_vipm<VIPM_IPP>);
REGISTER_TEST(test_otsu_vipm<VIPM_IPP>);
REGISTER_TEST(test_hist_vipm<VIPM_IPP>);
REGISTER_TEST(test_compare_vipm<VIPM_IPP>);

REGISTER_TEST(test_boxfilter_vipm<VIPM_OPENCV>);
REGISTER_TEST(test_resize_vipm<VIPM_OPENCV>);
REGISTER_TEST(test_erode_vipm<VIPM_OPENCV>);
REGISTER_TEST(test_otsu_vipm<VIPM_OPENCV>);
REGISTER_TEST(test_hist_vipm<VIPM_OPENCV>);
REGISTER_TEST(test_compare_vipm<VIPM_OPENCV>);

#endif //PERFTESTS_VIPM_TESTS_H
