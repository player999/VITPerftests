
#include <perf_arrayfire.h>
#include <cstring>

using namespace af;

/* CONSTRUCTOR */
AFImagePerfTest::AFImagePerfTest(uint32_t height, uint32_t width) : ImagePerfTest (height, width) {
    buffer2wrapped();
}

/* INPUT-OUTPUT*/

void AFImagePerfTest::readImage(const char *path) {
    wrappedSrcImageHost = loadImage(path);
}

void AFImagePerfTest::writeSrcImage(const char *path) {
    saveImage(path, wrappedSrcImageHost);
}

void AFImagePerfTest::writeDstImage(const char *path) {
    saveImage(path, wrappedDstImageHost);
}
/*
   0 AF_SUCCESS=0,
   1 AF_ERR_INTERNAL,
   2 AF_ERR_NOMEM,
   3 AF_ERR_DRIVER,
   4 AF_ERR_RUNTIME,
   5 AF_ERR_INVALID_ARRAY,
   6 AF_ERR_ARG,
   7 AF_ERR_SIZE,
   8 AF_ERR_DIFF_TYPE,
   9 AF_ERR_NOT_SUPPORTED,
   10 AF_ERR_NOT_CONFIGURED,
   11 AF_ERR_INVALID_TYPE,
   12 AF_ERR_INVALID_ARG,
   13 AF_ERR_UNKNOWN
 */
/* INTERNALS */
void AFImagePerfTest::buffer2wrapped() {
    try {
        wrappedSrcImageHost = constant(0, getImageHeight(), getImageWidth(), f32);
        float *base = wrappedDstImageHost.host<float>();
        std::cout<<"Size "<<wrappedSrcImageHost.bytes()<<std::endl;
        std::cout<<"Elements "<<wrappedSrcImageHost.elements()<<std::endl;
        std::cout<<"Max address "<<base + wrappedSrcImageHost.elements()<<std::endl;
        std::cout<<"Min address "<<base<<std::endl;
        for (int i = 0; i < getImageHeight(); i++) {
            float *row_dst = base + i * getImageWidth();
            uint8_t *row_src = imgBuffer + i * getImageWidth();
            for (int j = 0; j < getImageWidth(); j++) {
                //row_dst[j] = (float) row_src[j];
                //printf("(%d %d) ", i, j);
                std::cout<<"Cur address "<<row_dst[j]<<std::endl;
            }
        }
    }
    catch(af::exception& e) {
        std::cout<<e<<std::endl;
        throw("Error filling array");
    }
}

