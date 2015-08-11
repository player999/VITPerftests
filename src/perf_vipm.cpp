#include <perf_vipm.h>
#include <Vodi/Vodilib.h>
#include <Vodi/mm/MemstgP.h>
#include <Bo/aorp/Mld.h>
#include <cstring>

VipmImagePerfTest::VipmImagePerfTest(uint32_t height, uint32_t width) : ImagePerfTest (height, width) {
    bo_status_t status;
    memstorage = _VodiSMEMSTGopen(memstorage, NULL, NULL);
    status = AorpMldLoad("vipm", NULL, 0, 0, NULL);
    if (BoS_FAILURE(status)) throw "Could not load VIPM module";
    buffer2wrapped();
}

VipmImagePerfTest::~VipmImagePerfTest() {
    _VodiSMEMSTGclear(memstorage);
    if(wrappedSrcImage) _VodiARRdestroy((vodi_array_t)wrappedSrcImage, memstorage);
    if(wrappedDstImage) _VodiARRdestroy((vodi_array_t)wrappedDstImage, memstorage);
    AorpMldUnload("vipm", NULL);
}

void VipmImagePerfTest::readImage(const char *path) {
    vodi_image oldimage;
    VodiARRIOPread(path, &oldimage, NULL, NULL);
    if (wrappedSrcImage) _VodiARRdestroy((vodi_array_t)wrappedSrcImage, memstorage);
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

void VipmImagePerfTest::writeSrcImage(const char *path) {
    struct vodi_image oldimage;
    oldimage.img_base = wrappedSrcImage->img_base;
    oldimage.img_bpp = wrappedSrcImage->img_bpp;
    oldimage.img_height = getImageHeight();
    oldimage.img_width = getImageWidth();
    oldimage.img_flags = 0;
    VodiARRIOPwrite(path, NULL, &oldimage, NULL, NULL, NULL);
}

void VipmImagePerfTest::writeDstImage(const char *path) {
    struct vodi_image oldimage;
    oldimage.img_base = wrappedDstImage->img_base;
    oldimage.img_bpp = wrappedDstImage->img_bpp;
    oldimage.img_height = getImageHeight();
    oldimage.img_width = getImageWidth();
    oldimage.img_flags = 0;
    VodiARRIOPwrite(path, NULL, &oldimage, NULL, NULL, NULL);
}

void VipmImagePerfTest::buffer2wrapped() {
    struct vodi_imgparm parms;
    if (wrappedSrcImage) _VodiARRdestroy((vodi_array_t)wrappedSrcImage, memstorage);
    parms.igp_elemtype = _VodiK_ELEMTYPE_UCHAR;
    parms.igp_width = getImageWidth();
    parms.igp_height = getImageHeight();
    parms.igp_wstride = __myc_align(getImageWidth(), 4);
    parms.igp_nchannels = 1;
    parms.igp_bpp = 8;
    parms.igp_clrs = VodiK_UNK_CLRS;
    parms.igp_origin = _VodiK_TL_ORIGIN;
    wrappedSrcImage = (struct vodi_image2 *) _VodiIMGinit(NULL, &parms, memstorage, NULL);
    wrappedDstImage = (struct vodi_image2 *) _VodiIMGinit(NULL, &parms, memstorage, NULL);
    uint8_t *base = (uint8_t *)wrappedSrcImage->img_base;
    uint32_t stride = wrappedSrcImage->img_wstride;
    for(int i = 0; i < getImageHeight(); i++) {
        uint8_t *row_dst = base + stride * i;
        uint8_t *row_src = imgBuffer + i * getImageWidth();
        memcpy(row_dst, row_src, getImageWidth());
    }
}
