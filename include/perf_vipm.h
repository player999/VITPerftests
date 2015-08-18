
#ifndef PERFTESTS_PERF_VIPM_H
# define PERFTESTS_PERF_VIPM_H
# include "perftests.h"

# define HAS_TIMEVAL_STRUC
# define HAS_OFF_TYPE
# include <Vodi/utils/arrio/Filepath.h>
# include <Vodi/TypesP.h>
# include <Vodi/services/Vipm.h>
# include <map>

enum VipmType {
    VIPM_DEFAULT = 0,
    VIPM_IPP,
    VIPM_OPENCV
};

class VipmImagePerfTest : public ImagePerfTest {
public:
    static std::map<VipmType, const char *> mod_names;

    VipmImagePerfTest(uint32_t h, uint32_t w, VipmType modtype = VIPM_DEFAULT);
    ~VipmImagePerfTest();

    void ReadImage(const char *path);
    void WriteSrcImage(const char *path) const;
    void WriteDstImage(const char *path) const;
    void buffer2wrapped();

protected:
    void LoadVipmModule();
    void WriteVodiImage(const struct vodi_image2 *img, const char *path) const;

protected:
    struct vodi_image2  *wrappedSrcImage_;
    struct vodi_image2  *wrappedDstImage_;
    vodi_memstg_t       *memstorage_;
    VipmType             module_type_;
    aorp_object_t        module_;
    struct aorp_error256 error_;
    struct vodi_imgparm  imparm_;
};

#endif //PERFTESTS_PERF_VIPM_H
