
#ifndef PERFTESTS_PERF_VIPM_H
# define PERFTESTS_PERF_VIPM_H
# include "perftests.h"
# include <Vodi/utils/arrio/Filepath.h>
# include <Vodi/TypesP.h>
# include <Vodi/services/Vipm.h>

class VipmImagePerfTest : public ImagePerfTest {
public:
    VipmImagePerfTest(uint32_t h, uint32_t w);
    ~VipmImagePerfTest();
    void readImage(const char *path);
    void writeSrcImage(const char *path);
    void writeDstImage(const char *path);
    void buffer2wrapped();
    struct vodi_imgparm imparm;

    struct vodi_image2 *wrappedSrcImage = (vodi_image2 *)NULL;
    struct vodi_image2 *wrappedDstImage = (vodi_image2 *)NULL;

    vodi_memstg_t *memstorage = (vodi_memstg_t *) NULL;
};

#endif //PERFTESTS_PERF_VIPM_H
