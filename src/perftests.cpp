
#include <perftests.h>
#include <chrono>
#include <cmath>
#include <cstring>

/*
 *
 * MAIN FUNCTIONS
 *
 */

#define NSEC 1000000000
#define USEC 1000000

#ifdef _WIN32
    @TODO implement
    //http://stackoverflow.com/questions/9262270/color-console-output-with-c-in-windows
    #define COLOR_PRINT(name) static void print_ ## name(const char *text) {\
        printf(text);\
    }
COLOR_PRINT(red)
COLOR_PRINT(green)
COLOR_PRINT(blue)
COLOR_PRINT(purple)
COLOR_PRINT(cyan)
#else
#define COLOR_PRINT(name,specifier) static void print_ ## name(const char *text) {\
        printf("\x1b[" specifier "m");\
        printf(text);\
        printf("\x1b[0m");\
    }
COLOR_PRINT(red, "31")
COLOR_PRINT(green, "32")
COLOR_PRINT(blue, "34")
COLOR_PRINT(purple, "35")
COLOR_PRINT(cyan, "36")

#endif

uint64_t ImagePerfTest::run() {
    upload_time.clear();
    download_time.clear();
    execution_time.clear();

    auto total_start = std::chrono::high_resolution_clock::now();
    for (int i; i < executionCount; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        uploadToDevice();
        auto upload = std::chrono::high_resolution_clock::now();
        execute();
        auto exec = std::chrono::high_resolution_clock::now();
        downloadFromDevice();
        auto download = std::chrono::high_resolution_clock::now();
        this->upload_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(upload - start).count());
        this->execution_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(exec - upload).count());
        this->download_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(download - exec).count());
    }
    auto total_stop = std::chrono::high_resolution_clock::now();
    total_time = std::chrono::duration_cast<std::chrono::microseconds>(total_stop - total_start).count();
}

void ImagePerfTest::execute() {
    throw("Function Execute is not defined");
}

static inline void uint2str(char buf[], uint64_t number) {
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%lu", number);
}

static inline void dbl2str(char buf[], double number) {
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%f", number);
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
    double mean_download = 0;
    double mean_execution = 0;
    char buf[255];

    print_red(name().c_str()); printf("\n");
    print_cyan("Total test time: "); uint2str(buf, total_time); print_cyan(buf); print_cyan("usec\n");
    print_cyan("Total runs: "); uint2str(buf, runs); print_cyan(buf); print_cyan("\n\n");

    for (int i; i < runs; i++) {
        total_upload += upload_time[i];
        total_download += download_time[i];
        total_execution += execution_time[i];
    }

    print_blue("Total upload time: "); uint2str(buf, total_upload); print_blue(buf); print_blue("usec\n");
    print_blue("Total execution time: "); uint2str(buf, total_execution); print_blue(buf); print_blue("usec\n");
    print_blue("Total download time: "); uint2str(buf, total_download); print_blue(buf); print_blue("usec\n\n");

    mean_upload = total_upload / runs;
    mean_download = total_download / runs;
    mean_execution = total_execution / runs;

    print_purple("Mean upload time: "); uint2str(buf, mean_upload); print_purple(buf); print_purple("usec\n");
    print_purple("Mean execution time: "); uint2str(buf, mean_execution); print_purple(buf); print_purple("usec\n");
    print_purple("Mean download time: "); uint2str(buf, mean_download); print_purple(buf); print_purple("usec\n\n");

    for (int i; i < runs; i++) {
        stdev_upload += ((double)upload_time[i] - mean_upload);
        stdev_download += ((double)download_time[i] - mean_download);
        stdev_execution += ((double)execution_time[i] - mean_execution);
    }
    stdev_upload /= runs;
    stdev_download /= runs;
    stdev_execution /= runs;

    stdev_upload = sqrt(stdev_upload);
    stdev_download = sqrt(stdev_download);
    stdev_execution = sqrt(stdev_execution);

    print_green("STD upload time: "); dbl2str(buf, stdev_upload); print_green(buf); print_green("usec\n");
    print_green("STD execution time: "); dbl2str(buf, stdev_execution); print_green(buf); print_green("usec\n");
    print_green("STD download time: "); dbl2str(buf, stdev_download); print_green(buf); print_green("usec\n\n");

    print_red("================================================================================");
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
    checkerBoard();
    buffer2wrapped();
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

std::string ImagePerfTest::name() {
    return std::string("This is dafault test name. Please specify other!");
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
