
#include "perftests.h"
#include <chrono>
#include <cmath>
#include <cstdarg>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>

extern char *_g_result_fname;

/*
 *
 * CONSTRUCTORS
 *
 */

ImagePerfTest::ImagePerfTest()
    : image_width_(1000), image_height_(1000), execution_count_(RUN_COUNT), sq_side_(SQSIDE) {
}

ImagePerfTest::ImagePerfTest(uint32_t height, uint32_t width)
    : image_width_(width), image_height_(height),
      execution_count_(RUN_COUNT), sq_side_(SQSIDE)  {
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

void ImagePerfTest::Analyze() {
    total_upload_ = 0;
    total_download_ = 0;
    total_execution_ = 0;
    uint64_t runs = execution_count_;
    stdev_upload_ = 0;
    stdev_download_ = 0;
    stdev_execution_ = 0;

    median_upload_ = 0;
    median_download_ = 0;
    median_execution_ = 0;

    mean_upload_ = 0;
    mean_download_ = 0;
    mean_execution_ = 0;

    for (int i = 0; i < runs; i++) {
        total_upload_ += upload_time_[i];
        total_download_ += download_time_[i];
        total_execution_ += execution_time_[i];
    }

    mean_upload_ = (double)total_upload_ / runs;
    mean_execution_ = (double)total_execution_ / runs;
    mean_download_ = (double)total_download_ / runs;

    int mid = download_time_.size() / 2;
    if (upload_time_.size() % 2) {
        // Odd
        median_upload_ = upload_time_[mid];
        median_execution_ = execution_time_[mid];
        median_download_ = download_time_[mid];
    }
    else {
        // Even
        median_upload_ = (upload_time_[mid - 1] + upload_time_[mid]) / 2;
        median_execution_ = (execution_time_[mid - 1] + execution_time_[mid]) / 2;
        median_download_ = (download_time_[mid - 1] + download_time_[mid]) / 2;
    }

    for (int i = 0; i < runs; i++) {
        stdev_upload_ += pow((double)upload_time_[i] - mean_upload_, 2);
        stdev_download_ += pow((double)download_time_[i] - mean_download_, 2);
        stdev_execution_ += pow((double)execution_time_[i] - mean_execution_, 2);
    }

    stdev_upload_ = sqrt(stdev_upload_ / runs);
    stdev_download_ = sqrt(stdev_download_ / runs);
    stdev_execution_ = sqrt(stdev_execution_ / runs);
}

void ImagePerfTest::ShowAnalysis() const {
    print<kRed>("%s\n", Name().c_str());
    print<kCyan>("Total test time: %llu usec\n", total_time_);
    print<kCyan>("Total runs: %lu times\n\n", execution_count_);

    print<kBLue>("Total upload time: %llu usec\n", total_upload_);
    print<kBLue>("Total execution time: %llu usec\n", total_execution_);
    print<kBLue>("Total download time: %llu usec\n\n", total_download_);

    print<kPurple>("Mean upload time: %lf usec\n", mean_upload_);
    print<kPurple>("Mean execution time: %lf usec\n", mean_execution_);
    print<kPurple>("Mean download time: %lf usec\n\n", mean_download_);

    print<kYellow>("Median upload time: %lf usec\n", median_upload_);
    print<kYellow>("Median execution time: %lf usec\n", median_execution_);
    print<kYellow>("Median download time: %lf usec\n\n", median_download_);

    print<kGreen>("STD upload time: %lf usec\n", stdev_upload_);
    print<kGreen>("STD execution time: %lf usec\n", stdev_execution_);
    print<kGreen>("STD download time: %lf usec\n", stdev_download_);

    print<kRed>("========================================");
    print<kRed>("========================================\n");
}

void ImagePerfTest::WriteCSV() const {
    std::ofstream csv;

    std::ifstream checkfile(_g_result_fname);
    if (!checkfile.good()) {
        checkfile.close();

        csv.open(_g_result_fname);
        csv<<"Name"<<",";
        csv<<"Total"<<",";
        csv<<"Runs"<<",";
        csv<<"Total Up"<<",";
        csv<<"Total Ex"<<",";
        csv<<"Total Dn"<<",";
        csv<<"Mean Up"<<",";
        csv<<"Mean Ex"<<",";
        csv<<"Mean Dn"<<",";
        csv<<"Med Up"<<",";
        csv<<"Med Ex"<<",";
        csv<<"Med Dn"<<",";
        csv<<"Std Up"<<",";
        csv<<"Std Ex"<<",";
        csv<<"Std Dn"<<",";
        csv<<std::endl;
        csv.close();
    }



    csv.open(_g_result_fname, std::ios::app);
    csv<<Name()<<",";
    csv<<total_time_<<",";
    csv<<execution_count_<<",";

    csv<<total_upload_<<",";
    csv<<total_execution_<<",";
    csv<<total_download_<<",";

    csv<<mean_upload_<<",";
    csv<<mean_execution_<<",";
    csv<<mean_download_<<",";

    csv<<median_upload_<<",";
    csv<<median_execution_<<",";
    csv<<median_download_<<",";

    csv<<stdev_upload_<<",";
    csv<<stdev_execution_<<",";
    csv<<stdev_download_<<",";

    csv<<std::endl;
    csv.close();
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

            auto new_end = std::remove_if(all_tests.begin(), all_tests.end(),
                [=](const TestNode &tnode) {
                    return (tnode.test_type == cur_test_type || !order_by_test_type);
                }
            );

            all_tests.erase(new_end, all_tests.end());
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
