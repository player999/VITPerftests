
#include "perf_arrayfire.h"
#include <cstring>

using namespace af;

std::map<DeviceType, const char *> AFImagePerfTest::devices_names = {
    {kOriginal, ""},
    {kNvidiaCUDA, "NVIDIA_CUDA"},
    {kIntelOpenCL, "Intel(R) OpenCL"},
    {kIntelGenOCLDriver, "Intel Gen OCL Driver"}
};


AFImagePerfTest::AFImagePerfTest(uint32_t height, uint32_t width)
    : ImagePerfTest (height, width) {
    buffer2wrapped();
}

AFImagePerfTest::~AFImagePerfTest() {
    free(hostSrcData);
}

void AFImagePerfTest::ReadImage(const char *path) {
    wrappedSrcImageHost = loadImage(path);
}

void AFImagePerfTest::WriteSrcImage(const char *path) const {
    saveImage(path, wrappedSrcImageHost);
}

void AFImagePerfTest::WriteDstImage(const char *path) const {
    saveImage(path, wrappedDstImageHost);
}

void AFImagePerfTest::buffer2wrapped() {
    free(hostSrcData);
    hostSrcData = (float *)malloc(sizeof(float) * image_height() * image_width());
    for (int i = 0; i < image_height(); i++) {
        float *row_dst = hostSrcData + i * image_width();
        uint8_t *row_src = img_buffer + i * image_width();
        for (int j = 0; j < image_width(); j++)
            row_dst[j] = (float) row_src[j];
    }
    wrappedSrcImageHost = af::array(image_height(), image_width(), hostSrcData);
}

void AFImagePerfTest::SelectPlatform(const char *plaf) {
    int devcount = af::devicecount();
    char pname[255];
    char dname[255];
    char dtool[255];
    char dcomp[255];

    if (strlen(plaf) == 0) return; //Nothing to do here

    for (int i = 0; i < devcount; i++) {
        af::setDevice(i);
        af::deviceprop(dname, pname, dtool, dcomp);
        if (!strcmp(dtool, plaf)) break;
        if (i == devcount - 1)
            throw std::runtime_error("ArrayFire platform not found");
    };
    // todo(taras) This function is implemented?
}