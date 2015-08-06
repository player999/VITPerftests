
#include "perf_opencv.h"

/* CONSTRUCTOR */
CvImagePerfTest::CvImagePerfTest(uint32_t height, uint32_t width) : ImagePerfTest (height, width) {
    buffer2wrapped();
}

/* INPUT-OUTPUT*/

void CvImagePerfTest::readImage(const char *path) {
    wrappedSrcImageHost = cv::imread(path);
}

void CvImagePerfTest::writeSrcImage(const char *path) {
    cv::imwrite(path, wrappedSrcImageHost);
}

void CvImagePerfTest::writeDstImage(const char *path) {
    cv::imwrite(path, wrappedDstImageHost);
}

/* INTERNALS */
void CvImagePerfTest::buffer2wrapped() {
    wrappedSrcImageHost = cv::Mat(getImageHeight(), getImageWidth(), CV_8UC1);
    for(int i; i < getImageHeight(); i++) {
        uint8_t *row_dst = wrappedSrcImageHost.row(i).data;
        uint8_t *row_src = imgBuffer + i * getImageWidth();
        memcpy(row_dst, row_src, getImageWidth());
    }

}

void CvImagePerfTest::execute() {
    std::cout<<"TEST\n";
}
