
#ifndef PERFTESTS_PERF_VIPM_H
# define PERFTESTS_PERF_VIPM_H
# include "perftests.h"

# define HAS_TIMEVAL_STRUC
# define HAS_OFF_TYPE
# include <Vodi/utils/arrio/Filepath.h>
# include <Vodi/TypesP.h>
# include <Vodi/services/Vipm.h>

enum VipmType {
    VIPM_DEFAULT = 0,
    VIPM_IPP,
    VIPM_OPENCV
};

class VipmImagePerfTest : public ImagePerfTest {
public:
    VipmImagePerfTest(uint32_t h, uint32_t w);
    VipmImagePerfTest(uint32_t h, uint32_t w, VipmType modtype);
    ~VipmImagePerfTest();

    const char *modname = NULL;
    aorp_object_t module = NULL;

    void SetVipmType(VipmType vtype);
    void LoadVipmModule();
    void ReadImage(const char *path);
    void WriteSrcImage(const char *path) const;
    void WriteDstImage(const char *path) const;
    void buffer2wrapped();
    struct vodi_imgparm imparm;

    struct vodi_image2 *wrappedSrcImage = (vodi_image2 *)NULL;
    struct vodi_image2 *wrappedDstImage = (vodi_image2 *)NULL;

    vodi_memstg_t *memstorage = (vodi_memstg_t *) NULL;
};

#endif //PERFTESTS_PERF_VIPM_H
