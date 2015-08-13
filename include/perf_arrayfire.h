
#ifndef PERFTESTS_PERF_ARRAYFIRE_H
#define PERFTESTS_PERF_ARRAYFIRE_H

# include "perftests.h"
# include <arrayfire.h>

class AFImagePerfTest : public ImagePerfTest {
public:
    AFImagePerfTest(uint32_t height, uint32_t width);
    ~AFImagePerfTest();
    void readImage(const char *path);
    void writeSrcImage(const char *path);
    void writeDstImage(const char *path);
    void buffer2wrapped();
    af::array wrappedSrcImageHost;
    af::array wrappedDstImageHost;
    float *hostSrcData = NULL;
    static void selectPlatform(const char *plaf);
};

#endif //PERFTESTS_PERF_ARRAYFIRE_H
