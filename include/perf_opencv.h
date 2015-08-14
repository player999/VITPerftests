
#ifndef PERFTESTS_PERF_OPENCV_H
# define PERFTESTS_PERF_OPENCV_H

# include "perftests.h"
# include <opencv2/opencv.hpp>


class CVImagePerfTest : public ImagePerfTest {
public:
    CVImagePerfTest(uint32_t height, uint32_t width);

    void ReadImage(const char *path);
    void WriteSrcImage(const char *path) const;
    void WriteDstImage(const char *path) const;
    void buffer2wrapped();
    cv::Mat wrappedSrcImageHost;
    cv::Mat wrappedDstImageHost;
};

#endif //PERFTESTS_PERF_OPENCV_H
