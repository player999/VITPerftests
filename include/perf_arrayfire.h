
#ifndef PERFTESTS_PERF_ARRAYFIRE_H
#define PERFTESTS_PERF_ARRAYFIRE_H

# include "perftests.h"
# include <arrayfire.h>

class AFImagePerfTest : public ImagePerfTest {
public:
    AFImagePerfTest(uint32_t height, uint32_t width);
    ~AFImagePerfTest();
    void ReadImage(const char *path);
    void WriteSrcImage(const char *path) const;
    void WriteDstImage(const char *path) const;
    void buffer2wrapped();
    af::array wrappedSrcImageHost;
    af::array wrappedDstImageHost;
    float *hostSrcData = NULL;
    static void SelectPlatform(const char *plaf);
};

#endif //PERFTESTS_PERF_ARRAYFIRE_H
