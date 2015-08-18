#include "perf_vipm.h"
#include <Vodi/Vodilib.h>
#include <Vodi/mm/MemstgP.h>
#include <Vodi/objects/Vipm.h>
#include <Bo/aorp/Mld.h>
#include <cstring>
#include <stdexcept>

#define SAFE_VODI_ARR_DESTROY(anArr, aMemStg) \
    if (anArr) _VodiARRdestroy((vodi_array_t)anArr, aMemStg)

#define SAFE_VODI_ARR_INIT(anImgParm, aMemStg) \
    (struct vodi_image2 *) _VodiIMGinit(NULL, &anImgParm, aMemStg, NULL)

std::map<VipmType, const char *> VipmImagePerfTest::mod_names = {
    { VIPM_DEFAULT, "vipm" },
    { VIPM_IPP, "vipm-ipp" },
    { VIPM_OPENCV, "vipm-opencv" }
};

VipmImagePerfTest::VipmImagePerfTest(uint32_t h, uint32_t w, VipmType modtype) 
    : ImagePerfTest(h, w)
    , wrappedSrcImage_(nullptr)
    , wrappedDstImage_(nullptr)
    , memstorage_(nullptr)
    , module_type_(modtype)
    , module_(nullptr)
    , error_(AORP_ERROR256_ZERO_INITIALIZER) {
	memstorage_ = _VodiSMEMSTGopen(memstorage_, NULL, NULL);
    LoadVipmModule();
    buffer2wrapped();
}

VipmImagePerfTest::~VipmImagePerfTest() {
    _VodiSMEMSTGclear(memstorage_);
    SAFE_VODI_ARR_DESTROY(wrappedSrcImage_, memstorage_);
    SAFE_VODI_ARR_DESTROY(wrappedDstImage_, memstorage_);
    if (module_type_ != VipmType::VIPM_DEFAULT)
        _Art_cobclose_nlk(module_, NULL);
    AorpMldUnload(mod_names[module_type_], NULL);
}

void VipmImagePerfTest::LoadVipmModule() {
    bo_status_t status;

    status = AorpMldLoad(mod_names[module_type_], NULL, 0, 0, NULL);
    if (BoS_FAILURE(status))
        throw std::runtime_error("Could not load VIPM module");

    if (VipmType::VIPM_IPP == module_type_)
        _A_modopen1_nlk(module_, vipmipp, NULL);
    else if (VipmType::VIPM_OPENCV == module_type_)
        _A_modopen1_nlk(module_, vipmopencv, NULL);
    else
        module_ = _G_vipm_service_default;
//#ifdef _WIN32
//    module = NULL;
//#endif
}

void VipmImagePerfTest::ReadImage(const char *path) {
    vodi_image oldimage;
    struct vodi_imgparm parms;

    VodiARRIOPread(path, &oldimage, NULL, NULL);
    SAFE_VODI_ARR_DESTROY(wrappedSrcImage_, memstorage_);

    parms.igp_elemtype = _VodiK_ELEMTYPE_UCHAR;
    parms.igp_width = oldimage.img_width;
    parms.igp_height = oldimage.img_height;
    parms.igp_wstride = __myc_align(oldimage.img_width, 4);
    parms.igp_nchannels = 1;
    parms.igp_bpp = 8;
    parms.igp_clrs = VodiK_UNK_CLRS;
    parms.igp_origin = _VodiK_TL_ORIGIN;
    wrappedSrcImage_ = SAFE_VODI_ARR_INIT(parms, memstorage_);
    uint8_t *base_new = (uint8_t *)wrappedSrcImage_->img_base;
    uint8_t *base_old = (uint8_t *)oldimage.img_base;
    memcpy(base_new, base_old, __myc_align(oldimage.img_width, 4) * oldimage.img_height);
}

void VipmImagePerfTest::WriteVodiImage(const struct vodi_image2 *img,
    const char *path) const {
    struct vodi_image oldimage;
    oldimage.img_base = img->img_base;
    oldimage.img_bpp = img->img_bpp;
    oldimage.img_height = img->img_height;
    oldimage.img_width = img->img_width;
    oldimage.img_flags = 0;
    VodiARRIOPwrite(path, NULL, &oldimage, NULL, NULL, NULL);
}

void VipmImagePerfTest::WriteSrcImage(const char *path) const {
    WriteVodiImage(wrappedSrcImage_, path);
}
void VipmImagePerfTest::WriteDstImage(const char *path) const {
    WriteVodiImage(wrappedDstImage_, path);
}

void VipmImagePerfTest::buffer2wrapped() {
    SAFE_VODI_ARR_DESTROY(wrappedSrcImage_, memstorage_);
    imparm_.igp_elemtype = _VodiK_ELEMTYPE_UCHAR;
    imparm_.igp_width = image_width();
    imparm_.igp_height = image_height();
    imparm_.igp_wstride = __myc_align(image_width(), 4);
    imparm_.igp_nchannels = 1;
    imparm_.igp_bpp = 8;
    imparm_.igp_clrs = VodiK_UNK_CLRS;
    imparm_.igp_origin = _VodiK_TL_ORIGIN;
    wrappedSrcImage_ = SAFE_VODI_ARR_INIT(imparm_, memstorage_);
    wrappedDstImage_ = SAFE_VODI_ARR_INIT(imparm_, memstorage_);

    uint8_t *base = (uint8_t *)wrappedSrcImage_->img_base;
    uint32_t stride = wrappedSrcImage_->img_wstride;

    for(size_t i = 0; i < image_height(); i++) {
        uint8_t *row_dst = base + stride * i;
        uint8_t *row_src = img_buffer + i * image_width();
        memcpy(row_dst, row_src, image_width());
    }
}

