
#include "perftests.h"
#include <chrono>
#include <cmath>
#include <cstring>
#include <cstdarg>
#include <exception>
#include <stdexcept>
#include <cstdio>

/*
 *
 * CONSTRUCTORS
 *
 */

ImagePerfTest::ImagePerfTest()
    : image_width_(0), image_height_(0), execution_count_(1), sq_side_(20) {
}

ImagePerfTest::ImagePerfTest(uint32_t height, uint32_t width)
    : image_width_(width), image_height_(height),
      execution_count_(1), sq_side_(20)  {
    this->AllocateImage();
    CheckerBoard();
}

std::vector<ImagePerfTest::test_fn> ImagePerfTest::tests = {};
/*
 *
 * MAIN FUNCTIONS
 *
 */

#ifdef _WIN32
#include <Windows.h>

WORD GetConsoleTextAttribute(HANDLE handle) {
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(handle, &csbiInfo);
    return csbiInfo.wAttributes;
}

#define COLOR_PRINT(name, specifier) \
    static void print_ ## name(const char *frmt, ...) {        \
        va_list args;                                          \
        va_start (args, frmt);                                 \
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);       \
        WORD old_color_attr = GetConsoleTextAttribute(handle); \
        SetConsoleTextAttribute(handle, specifier);            \
        vprintf (frmt, args);                                  \
        SetConsoleTextAttribute(handle, old_color_attr);       \
        va_end (args);                                         \
    }

COLOR_PRINT(red, 12)
COLOR_PRINT(green, 10)
COLOR_PRINT(blue, 9)
COLOR_PRINT(purple, 13)
COLOR_PRINT(cyan, 11)
COLOR_PRINT(yellow, 14)
COLOR_PRINT(white, 15)
#else
#define COLOR_PRINT(name, specifier) \
    static void print_ ## name(const char *frmt, ...) {\
    va_list args; \
    va_start (args, frmt); \
    printf("\x1b[" specifier "m");\
    vprintf (frmt, args); \
    printf("\x1b[0m");\
    va_end (args); \
    }

COLOR_PRINT(red, "31")
COLOR_PRINT(green, "32")
COLOR_PRINT(blue, "34")
COLOR_PRINT(purple, "35")
COLOR_PRINT(cyan, "36")

#endif

uint64_t ImagePerfTest::Run() {
    typedef std::chrono::system_clock::time_point time_point;
    auto now = std::chrono::high_resolution_clock::now;

    auto time_diff =  [](time_point &time_end, time_point &time_start) {
      return (uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(
          time_end - time_start).count();
    };

    upload_time_.clear();
    download_time_.clear();
    execution_time_.clear();

    auto total_start = now();
    for (size_t i = 0; i < execution_count_; i++) {
        auto start = now();
        UploadToDevice();
        auto upload = now();
        Execute();
        auto exec = now();
        DownloadFromDevice();
        auto download = now();

        upload_time_.push_back(time_diff(upload, start));
        execution_time_.push_back(time_diff(exec, upload));
        download_time_.push_back(time_diff(download, exec));
    }
    auto total_stop = now();
    total_time_ = time_diff(total_stop, total_start);
    return total_time_;
}

void ImagePerfTest::Execute() {
    throw std::runtime_error("Function Execute is not defined");
}

void ImagePerfTest::ShowAnalysis() const {
    uint64_t total_upload = 0;
    uint64_t total_download = 0;
    uint64_t total_execution = 0;
    uint64_t runs = upload_time_.size();
    double stdev_upload = 0;
    double stdev_download = 0;
    double stdev_execution = 0;
    double mean_upload = 0;
    double mean_download = 0;
    double mean_execution = 0;

    print_red("%s\n", Name().c_str());
    print_cyan("Total test time: %ld usec\n", total_time_);
    print_cyan("Total runs: %ld times\n\n", runs);

    for (int i = 0; i < runs; i++) {
        total_upload += upload_time_[i];
        total_download += download_time_[i];
        total_execution += execution_time_[i];
    }

    print_blue("Total upload time: %ld usec\n", total_upload);
    print_blue("Total execution time: %ld usec\n", total_execution);
    print_blue("Total download time: %ld usec\n\n", total_download);

    mean_upload = (double)total_upload / runs;
    mean_execution = (double)total_execution / runs;
    mean_download = (double)total_download / runs;

    print_purple("Mean upload time: %lf usec\n", mean_upload);
    print_purple("Mean execution time: %lf usec\n", mean_execution);
    print_purple("Mean download time: %lf usec\n\n", mean_download);

    for (int i = 0; i < runs; i++) {
        stdev_upload += pow((double)upload_time_[i] - mean_upload, 2);
        stdev_download += pow((double)download_time_[i] - mean_download, 2);
        stdev_execution += pow((double)execution_time_[i] - mean_execution, 2);
    }

    stdev_upload = sqrt(stdev_upload / runs);
    stdev_download = sqrt(stdev_download / runs);
    stdev_execution = sqrt(stdev_execution / runs);

    print_green("STD upload time: %lf usec\n", stdev_upload);
    print_green("STD execution time: %lf usec\n", stdev_execution);
    print_green("STD download time: %lf usec\n", stdev_download);

    print_red("================================================================================\n");
}


/*
 *
 * HELPERS
 *
 */

void ImagePerfTest::AllocateImage() {
    free(this->img_buffer);
    this->img_buffer = (uint8_t *) malloc(image_width_ * image_height_);
}

void ImagePerfTest::CheckerBoard() {
    int sqHeight = image_height_ / sq_side_ + (image_height_ % sq_side_ != 0);
    int sqWidth = image_width_ / sq_side_ + (image_width_ % sq_side_ != 0);

    //printf("sqHeight %d, sqWidth %d, image_height_ %d, image_width_ %d, sq_side_ %d\n",
    //       sqHeight, sqWidth, image_height_, image_width_, sq_side_);
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
            uint8_t *ptr = img_buffer + sq_side_ * i * image_width_ + j * sq_side_;
            for (size_t k = 0; k < sq_side_; k++) {
                for (size_t l = 0; l < sq_side_; l++) {
                    if (sq_side_ * i + k >= image_height_) continue;
                    if (sq_side_ * j + l >= image_width_) continue;
                    int off = k * image_width_ + l;
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

std::string ImagePerfTest::Name() const {
    return std::string("This is dafault test name. Please specify other!");
}

/*
 *
 * MEMORY
 *
 */

void ImagePerfTest::UploadToDevice() {
    // Default implementation
}

void ImagePerfTest::DownloadFromDevice() {
    // Default implementation
}

void ImagePerfTest::RunAllTests() {
    try {
        for (auto test: ImagePerfTest::tests)
            test();
    }
    catch (std::exception &e) {
        printf("An error is occured, reason: %s\n", e.what());
    }
}

void ImagePerfTest::RegisterTest(ImagePerfTest::test_fn test) {
    ImagePerfTest::tests.push_back(test);
}

void ImagePerfTest::Prelude() {
    /* Do nothing */
}

void ImagePerfTest::Postlude() {
    /* Do nothing */
}
