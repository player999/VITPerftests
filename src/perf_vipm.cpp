#include "perf_vipm.h"
#include <Vodi/Vodilib.h>
#include <Vodi/mm/MemstgP.h>
#include <Bo/aorp/Mld.h>
#include <cstring>
#include <stdexcept>

VipmImagePerfTest::VipmImagePerfTest(uint32_t height, uint32_t width)
    : ImagePerfTest (height, width) {
    bo_status_t status;
    memstorage = _VodiSMEMSTGopen(memstorage, NULL, NULL);
    status = AorpMldLoad("vipm", NULL, 0, 0, NULL);
    if (BoS_FAILURE(status))
        throw std::runtime_error("Could not load VIPM module");
    buffer2wrapped();
}

VipmImagePerfTest::~VipmImagePerfTest() {
    _VodiSMEMSTGclear(memstorage);
    if(wrappedSrcImage)
        _VodiARRdestroy((vodi_array_t)wrappedSrcImage, memstorage);
    if(wrappedDstImage)
        _VodiARRdestroy((vodi_array_t)wrappedDstImage, memstorage);
    AorpMldUnload("vipm", NULL);
}

void VipmImagePerfTest::ReadImage(const char *path) {
    vodi_image oldimage;
    VodiARRIOPread(path, &oldimage, NULL, NULL);
    if (wrappedSrcImage)
        _VodiARRdestroy((vodi_array_t)wrappedSrcImage, memstorage);
    struct vodi_imgparm parms;
    parms.igp_elemtype = _VodiK_ELEMTYPE_UCHAR;
    parms.igp_width = oldimage.img_width;
    parms.igp_height = oldimage.img_height;
    parms.igp_wstride = __myc_align(oldimage.img_width, 4);
    parms.igp_nchannels = 1;
    parms.igp_bpp = 8;
    parms.igp_clrs = VodiK_UNK_CLRS;
    parms.igp_origin = _VodiK_TL_ORIGIN;
    wrappedSrcImage = (struct vodi_image2 *) _VodiIMGinit(NULL, &parms, memstorage, NULL);
    uint8_t *base_new = (uint8_t *)wrappedSrcImage->img_base;
    uint8_t *base_old = (uint8_t *)oldimage.img_base;
    memcpy(base_new, base_old, __myc_align(oldimage.img_width, 4) * oldimage.img_height);
}

void VipmImagePerfTest::WriteSrcImage(const char *path) const {
    struct vodi_image oldimage;
    oldimage.img_base = wrappedSrcImage->img_base;
    oldimage.img_bpp = wrappedSrcImage->img_bpp;
    oldimage.img_height = image_height();
    oldimage.img_width = image_width();
    oldimage.img_flags = 0;
    VodiARRIOPwrite(path, NULL, &oldimage, NULL, NULL, NULL);
}

void VipmImagePerfTest::WriteDstImage(const char *path) const {
    struct vodi_image oldimage;
    oldimage.img_base = wrappedDstImage->img_base;
    oldimage.img_bpp = wrappedDstImage->img_bpp;
    oldimage.img_height = image_height();
    oldimage.img_width = image_width();
    oldimage.img_flags = 0;
    VodiARRIOPwrite(path, NULL, &oldimage, NULL, NULL, NULL);
}

void VipmImagePerfTest::buffer2wrapped() {
    if (wrappedSrcImage) _VodiARRdestroy((vodi_array_t)wrappedSrcImage, memstorage);
    imparm.igp_elemtype = _VodiK_ELEMTYPE_UCHAR;
    imparm.igp_width = image_width();
    imparm.igp_height = image_height();
    imparm.igp_wstride = __myc_align(image_width(), 4);
    imparm.igp_nchannels = 1;
    imparm.igp_bpp = 8;
    imparm.igp_clrs = VodiK_UNK_CLRS;
    imparm.igp_origin = _VodiK_TL_ORIGIN;
    wrappedSrcImage = (struct vodi_image2 *) _VodiIMGinit(NULL, &imparm, memstorage, NULL);
    wrappedDstImage = (struct vodi_image2 *) _VodiIMGinit(NULL, &imparm, memstorage, NULL);
    uint8_t *base = (uint8_t *)wrappedSrcImage->img_base;
    uint32_t stride = wrappedSrcImage->img_wstride;
    for(int i = 0; i < image_height(); i++) {
        uint8_t *row_dst = base + stride * i;
        uint8_t *row_src = img_buffer + i * image_width();
        memcpy(row_dst, row_src, image_width());
    }
}
