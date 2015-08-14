
#include "perf_arrayfire.h"
#include <cstring>

using namespace af;

/* CONSTRUCTOR */
AFImagePerfTest::AFImagePerfTest(uint32_t height, uint32_t width)
    : ImagePerfTest (height, width) {
    buffer2wrapped();
}

AFImagePerfTest::~AFImagePerfTest() {
    free(hostSrcData);
}

/* INPUT-OUTPUT*/

void AFImagePerfTest::ReadImage(const char *path) {
    wrappedSrcImageHost = loadImage(path);
}

void AFImagePerfTest::WriteSrcImage(const char *path) const {
    saveImage(path, wrappedSrcImageHost);
}

void AFImagePerfTest::WriteDstImage(const char *path) const {
    saveImage(path, wrappedDstImageHost);
}

/* INTERNALS */
void AFImagePerfTest::buffer2wrapped() {
    free(hostSrcData);
    hostSrcData = (float *)malloc(sizeof(float) * getImageHeight() * getImageWidth());
    for (int i = 0; i < getImageHeight(); i++) {
        float *row_dst = hostSrcData + i * getImageWidth();
        uint8_t *row_src = imgBuffer + i * getImageWidth();
        for (int j = 0; j < getImageWidth(); j++) {
            row_dst[j] = (float) row_src[j];
        }
    }
    wrappedSrcImageHost = af::array(getImageHeight(), getImageWidth(), hostSrcData);
}

void AFImagePerfTest::SelectPlatform(const char *plaf) {
    int devcount = af::devicecount();
    char pname[255];
    char dname[255];
    char dtool[255];
    char dcomp[255];

    for (int i=0; i < devcount; i++) {
        af::setDevice(i);
        af::deviceprop(dname, pname, dtool, dcomp);
        if (!strcmp(dtool, plaf)) {
            break;
        }
        if (i == devcount - 1) throw("ArrayFire platform not found");
    };
}