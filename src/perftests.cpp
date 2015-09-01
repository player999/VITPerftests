
#include "perftests.h"
#include <chrono>
#include <cmath>
#include <cstring>
#include <cstdarg>
#include <exception>
#include <stdexcept>
#include <cstdio>
#include <map>
#include <algorithm>

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

std::vector<ImagePerfTest::TestNode> ImagePerfTest::tests = {};
/*
 *
 * MAIN FUNCTIONS
 *
 */

enum PrinterColour {
    kGreen, kRed, kBLue, kPurple, kCyan, kYellow, kWhite
};


#ifdef _WIN32
# include <Windows.h>

static std::map<PrinterColour, int> _s_colour_specifier = {
    { kGreen,  10 },
    { kRed,    12 },
    { kBLue,    9 },
    { kPurple, 13 },
    { kCyan,   11 },
    { kYellow, 14 },
    { kWhite,  15 }
};

WORD GetConsoleTextAttribute(HANDLE handle) {
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(handle, &csbiInfo);
    return csbiInfo.wAttributes;
}

static void print_in_(PrinterColour pcolour, const char *frmt, va_list args) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD old_color_attr = GetConsoleTextAttribute(handle);
    SetConsoleTextAttribute(handle, _s_colour_specifier[pcolour]);
    vprintf(frmt, args);
    SetConsoleTextAttribute(handle, old_color_attr);
}

#else
static std::map<PrinterColour, const char *> _s_colour_specifier = {
    { kGreen,  "32" },
    { kRed,    "31" },
    { kBLue,   "34" },
    { kPurple, "35" },
    { kCyan,   "36" },
    { kYellow, "33" },
    { kWhite,  "37" }
};

static void print_in_(PrinterColour pcolour, const char *frmt, va_list args) {
    printf("\x1b[%sm", _s_colour_specifier[pcolour]);
    vprintf(frmt, args);
    printf("\x1b[0m");
}
#endif

template <PrinterColour pcolour = PrinterColour::kWhite>
void print(const char *frmt, ...) {
    va_list args;
    va_start(args, frmt);
    print_in_(pcolour, frmt, args);
    va_end(args);
}

uint64_t ImagePerfTest::Run() {
#ifdef _WIN32
    LARGE_INTEGER perf_freq;
    QueryPerformanceFrequency(&perf_freq);

    auto now = []() -> uint64_t {
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        return count.QuadPart;
    };

	auto time_diff =  [perf_freq](uint64_t time_end, uint64_t time_start) {
		return (uint64_t)((time_end - time_start) / (perf_freq.QuadPart / 1000000));
	};

#else
    typedef std::chrono::system_clock::time_point time_point;
    auto now = std::chrono::high_resolution_clock::now;

    auto time_diff = [](time_point &time_end, time_point &time_start) {
        return (uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(
            time_end - time_start).count();
    };
#endif

    upload_time_.clear();
    download_time_.clear();
    execution_time_.clear();

    int warmiter = execution_count() < 5 ? 5 : execution_count() / 10;
    for (int i = 0; i < warmiter; i++) { //Warm-up
        UploadToDevice(); Execute(); DownloadFromDevice();
    }

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

    std::sort(upload_time_.begin(), upload_time_.end());
    std::sort(execution_time_.begin(), execution_time_.end());
    std::sort(download_time_.begin(), download_time_.end());

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

    double median_upload = 0;
    double median_download = 0;
    double median_execution = 0;

    double mean_upload = 0;
    double mean_download = 0;
    double mean_execution = 0;

    print<kRed>("%s\n", Name().c_str());
    print<kCyan>("Total test time: %llu usec\n", total_time_);
    print<kCyan>("Total runs: %llu times\n\n", runs);

    for (int i = 0; i < runs; i++) {
        total_upload += upload_time_[i];
        total_download += download_time_[i];
        total_execution += execution_time_[i];
    }

    print<kBLue>("Total upload time: %llu usec\n", total_upload);
    print<kBLue>("Total execution time: %llu usec\n", total_execution);
    print<kBLue>("Total download time: %llu usec\n\n", total_download);

    mean_upload = (double)total_upload / runs;
    mean_execution = (double)total_execution / runs;
    mean_download = (double)total_download / runs;

    print<kPurple>("Mean upload time: %lf usec\n", mean_upload);
    print<kPurple>("Mean execution time: %lf usec\n", mean_execution);
    print<kPurple>("Mean download time: %lf usec\n\n", mean_download);

    int mid = download_time_.size() / 2;
    if (upload_time_.size() % 2) {
        // Odd
        median_upload = upload_time_[mid];
        median_execution = execution_time_[mid];
        median_download = download_time_[mid];
    }
    else {
        // Even
        median_upload = (upload_time_[mid - 1] + upload_time_[mid]) / 2;
        median_execution = (execution_time_[mid - 1] + execution_time_[mid]) / 2;
        median_download = (download_time_[mid - 1] + download_time_[mid]) / 2;
    }

    print<kYellow>("Median upload time: %lf usec\n", median_upload);
    print<kYellow>("Median execution time: %lf usec\n", median_execution);
    print<kYellow>("Median download time: %lf usec\n\n", median_download);

    for (int i = 0; i < runs; i++) {
        stdev_upload += pow((double)upload_time_[i] - mean_upload, 2);
        stdev_download += pow((double)download_time_[i] - mean_download, 2);
        stdev_execution += pow((double)execution_time_[i] - mean_execution, 2);
    }

    stdev_upload = sqrt(stdev_upload / runs);
    stdev_download = sqrt(stdev_download / runs);
    stdev_execution = sqrt(stdev_execution / runs);

    print<kGreen>("STD upload time: %lf usec\n", stdev_upload);
    print<kGreen>("STD execution time: %lf usec\n", stdev_execution);
    print<kGreen>("STD download time: %lf usec\n", stdev_download);


    print<kRed>("========================================");
    print<kRed>("========================================\n");
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

void ImagePerfTest::RunAllTests(bool order_by_test_type) {
    auto all_tests = tests;

    try {
        while (all_tests.size()) {
            TestType cur_test_type = all_tests[0].test_type;
            std::for_each(all_tests.begin(), all_tests.end(), 
                [=](const TestNode &tnode) {
                    if (tnode.test_type == cur_test_type || !order_by_test_type)
                        tnode.test_function();
                }
            );

            std::remove_if(all_tests.begin(), all_tests.end(),
                [=](const TestNode &tnode) {
                    return (tnode.test_type == cur_test_type || !order_by_test_type);
                }
            );
        }
    }
    catch (std::exception &e) {
        printf("An error is occured, reason: %s\n", e.what());
    }
}

void ImagePerfTest::RegisterTest(const ImagePerfTest::TestNode &tnode) {
    ImagePerfTest::tests.push_back(tnode);
}

void ImagePerfTest::Prelude() {
    /* Do nothing */
}

void ImagePerfTest::Postlude() {
    /* Do nothing */
}
