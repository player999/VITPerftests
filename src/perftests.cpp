
#include <perftests.h>
#include <time.h>
#include <math.h>

using namespace testing;

/*
 *
 * MAIN FUNCTIONS
 *
 */

#define NSEC 1000000000
#define USEC 1000000

inline uint64_t difftime(struct timespec start, struct timespec stop)
{
    uint64_t secs = stop.tv_sec - start.tv_sec;
    uint64_t nsecs;
    if (stop.tv_nsec < start.tv_nsec) {
        secs -= 1;
        nsecs = NSEC + stop.tv_nsec - start.tv_nsec;
    } else {
        nsecs = stop.tv_nsec - start.tv_nsec;
    }
    return secs * USEC + nsecs / 1000;
}

uint64_t ImagePerfTest::run() {
    struct timespec start;
    struct timespec upload;
    struct timespec exec;
    struct timespec download;
    struct timespec total_start;
    struct timespec total_stop;

    upload_time.clear();
    download_time.clear();
    execution_time.clear();
    clock_gettime(CLOCK_MONOTONIC, &total_start);
    for (int i; i < executionCount; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        uploadToDevice();
        clock_gettime(CLOCK_MONOTONIC, &upload);
        execute();
        clock_gettime(CLOCK_MONOTONIC, &exec);
        downloadFromDevice();
        clock_gettime(CLOCK_MONOTONIC, &download);
        this->upload_time.push_back(difftime(start, upload));
        this->download_time.push_back(difftime(exec, download));
        this->execution_time.push_back(difftime(upload, exec));
    }
    clock_gettime(CLOCK_MONOTONIC, &total_stop);
    total_time = difftime(total_start, total_stop);
}

void ImagePerfTest::execute() {
    throw("Function Execute is not defined");
}

void ImagePerfTest::showAnalysis() {
    uint64_t total_upload = 0;
    uint64_t total_download = 0;
    uint64_t total_execution = 0;
    uint64_t runs = upload_time.size();
    double stdev_upload = 0;
    double stdev_download = 0;
    double stdev_execution = 0;
    double mean_upload = 0;
    double mean_downlaod = 0;
    double mean_execution = 0;

    std::cout<<"Total test time: "<<total_time<<"usec"<<std::endl;
    std::cout<<"Total runs: "<<runs<<std::endl;
    std::cout<<std::endl;

    for (int i; i < runs; i++) {
        total_upload += upload_time[i];
        total_download += download_time[i];
        total_execution += execution_time[i];
    }
    std::cout<<"Total upload time: "<<total_upload<<"usec"<<std::endl;
    std::cout<<"Total download time: "<<total_download<<"usec"<<std::endl;
    std::cout<<"Total execution time: "<<total_execution<<"usec"<<std::endl;
    std::cout<<std::endl;

    mean_upload = total_upload / runs;
    mean_downlaod = total_download / runs;
    mean_execution = total_execution / runs;
    std::cout<<"Mean upload time: "<<mean_upload<<"usec"<<std::endl;
    std::cout<<"Mean download time: "<<mean_downlaod<<"usec"<<std::endl;
    std::cout<<"Mean execution time: "<<mean_execution <<"usec"<<std::endl;
    std::cout<<std::endl;

    for (int i; i < runs; i++) {
        stdev_upload += ((double)upload_time[i] - mean_upload);
        stdev_download += ((double)download_time[i] - mean_downlaod);
        stdev_execution += ((double)execution_time[i] - mean_execution);
    }
    stdev_upload /= runs;
    stdev_download /= runs;
    stdev_execution /= runs;

    stdev_upload = sqrt(stdev_upload);
    stdev_download = sqrt(stdev_download);
    stdev_execution = sqrt(stdev_execution);

    std::cout<<"STD upload time: "<<stdev_upload<<"usec"<<std::endl;
    std::cout<<"STD download time: "<<stdev_download<<"usec"<<std::endl;
    std::cout<<"STD execution time: "<<stdev_execution<<"usec"<<std::endl;
}

/*
 *
 * CONSTRUCTORS
 *
 */

ImagePerfTest::ImagePerfTest(uint32_t height, uint32_t width) {
    this->imageHeight = height;
    this->imageWidth = width;
    this->allocateImage();
    checkerBoard();
}

ImagePerfTest::ImagePerfTest() {

}

/*
 *
 * Getters and setters
 *
 */

void ImagePerfTest::setExecutionCount(uint32_t count) {
    executionCount = count;
}

uint32_t ImagePerfTest::getExecutionCount() {
    return executionCount;
}

uint32_t ImagePerfTest::getImageHeight() {
    return imageHeight;
}

uint32_t ImagePerfTest::getImageWidth() {
    return imageWidth;
}

void ImagePerfTest::setImageHeight(uint32_t height) {
    imageHeight = height;
}

void ImagePerfTest::setImageWidth(uint32_t width) {
    imageWidth = width;
}

void ImagePerfTest::setSqSide(uint32_t side) {
    sqSide = side;
}

uint32_t ImagePerfTest::getSqSide() {
    return sqSide;
}

/*
 *
 * HELPERS
 *
 */

void ImagePerfTest::allocateImage() {
    free(this->imgBuffer);
    this->imgBuffer = (uint8_t *) malloc(this->imageWidth * this->imageHeight);
}

void ImagePerfTest::checkerBoard() {
    int sqHeight = (imageHeight % sqSide)?(imageHeight / sqSide + 1):(imageHeight / sqSide);
    int sqWidth = (imageWidth % sqSide)?(imageWidth / sqSide + 1):(imageWidth / sqSide);

    /* Generate chess pattern */
    for (int i = 0; i < sqHeight; i++) {
        for (int j = 0; j < sqWidth; j++) {
            int value;
            /* Determine value of square */
            if ((i % 2 == 0) && (j % 2 == 0)) {
                value = 0;
            }
            else if ((i % 2 == 0) && (j % 2 == 1)) {
                value = 255;
            }
            else if ((i % 2 == 1) && (j % 2 == 0)) {
                value = 255;
            }
            else if ((i % 2 == 1) && (j % 2 == 1)) {
                value = 0;
            }

            /* Fill square with color */
            uint8_t *ptr = imgBuffer + sqSide * i * imageWidth + j * sqSide;
            for (int k = 0; k < sqSide; k++) {
                for (int l = 0; l < sqSide; l++) {
                    if (sqSide * i + k >= imageHeight) continue;
                    if (sqSide * j + l >= imageWidth) continue;
                    int off = k * imageWidth + l;
                    ptr[off] = value;
                }
            }
        }
    }
}

/*
 *
 * INPUT AND OUTPUT
 *
 */

void ImagePerfTest::buffer2wrapped() {
    throw("Function Buffer to wrapped image is not implemented");
    /* TO BE OVERRIDEN */
}

/*
 *
 * MEMORY
 *
 */

void ImagePerfTest::uploadToDevice() {
    // Default implementation
}

void ImagePerfTest::downloadFromDevice() {
    // Default implementation
}