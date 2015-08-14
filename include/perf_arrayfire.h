
#ifndef PERFTESTS_PERF_ARRAYFIRE_H
# define PERFTESTS_PERF_ARRAYFIRE_H

#include <map>
# include "perftests.h"
# include <arrayfire.h>

enum DeviceType {
    kOriginal,
    kIntelOpenCL,
    kIntelGenOCLDriver,
    kNvidiaCUDA
};

class AFImagePerfTest : public ImagePerfTest {
public:
    static std::map<DeviceType, const char *> devices_names;

    AFImagePerfTest(uint32_t height, uint32_t width);
    ~AFImagePerfTest();
    void ReadImage(const char *path);
    void WriteSrcImage(const char *path) const;
    void WriteDstImage(const char *path) const;
    void buffer2wrapped();
    static void SelectPlatform(const char *plaf);

public: //// Why do you want to make this public? maybe protected is better?
    af::array wrappedSrcImageHost;
    af::array wrappedDstImageHost;
    float *hostSrcData = NULL;
};

#endif //PERFTESTS_PERF_ARRAYFIRE_H
