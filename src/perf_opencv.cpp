
#include <perf_opencv.h>

/* CONSTRUCTOR */
CVImagePerfTest::CVImagePerfTest(uint32_t height, uint32_t width) : ImagePerfTest (height, width) {
    buffer2wrapped();
}

/* INPUT-OUTPUT*/

void CVImagePerfTest::readImage(const char *path) {
    wrappedSrcImageHost = cv::imread(path);
}

void CVImagePerfTest::writeSrcImage(const char *path) {
    cv::imwrite(path, wrappedSrcImageHost);
}

void CVImagePerfTest::writeDstImage(const char *path) {
    cv::imwrite(path, wrappedDstImageHost);
}

/* INTERNALS */
void CVImagePerfTest::buffer2wrapped() {
    wrappedSrcImageHost = cv::Mat(getImageHeight(), getImageWidth(), CV_8UC1);
    for(int i = 0; i < getImageHeight(); i++) {
        uint8_t *row_dst = wrappedSrcImageHost.row(i).data;
        uint8_t *row_src = imgBuffer + i * getImageWidth();
        memcpy(row_dst, row_src, getImageWidth());
    }
}
