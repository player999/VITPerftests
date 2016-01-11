
#ifndef PERFTESTS_PERFTESTS_H
# define PERFTESTS_PERFTESTS_H

# include <stdint.h>
# include <stdlib.h>
# include <vector>
# include <string>
# include <iostream>
# include <cstring>

enum TestPlatform {
    kVipmIPP,
    kVipmOpenCV,
    kVipmC,
    kOpenCV3,
    kOpenCVCLCPU3,
    kOpenCV3CLIntelGPU,
    kOpenCV3CLCudaGPU,
    kOpenCV3CUDA,
    kAFIntel
};

enum TestType {
    kBoxFilter,
    kOtsu,
    kMedianFilter,
    kResize,
    kIntegral,
    kMorphology,
    kTopHat,
    kHist,
    kCompare,
    kUnknown
};

class ImagePerfTest {
 public:
    typedef void(*test_fn)();
    struct TestNode {
        test_fn      test_function;
        TestPlatform test_platform;
        TestType     test_type;
    };

    static std::vector<TestNode> tests;

    static void RunAllTests(bool order_by_test_type = false);
    static void RegisterTest(const TestNode &test_node);

    ImagePerfTest();
    ImagePerfTest(uint32_t height, uint32_t width);
    virtual ~ImagePerfTest() {}

    virtual void Prelude();  // Executed before run
    virtual void Postlude(); // Executed after run
    uint64_t Run();
    virtual void Execute() = 0;
    virtual void UploadToDevice();
    virtual void DownloadFromDevice();
    virtual std::string Name() const;

    void Analyze();

    void ShowAnalysis() const;
    void WriteCSV() const;

    void set_image_height(uint32_t height) { image_height_ = height; }
    uint32_t image_height() const { return image_height_; }

    void set_image_width(uint32_t width) { image_width_ = width; }
    uint32_t image_width() const { return image_width_; }

    void set_execution_count(uint32_t count) { execution_count_ = count; }
    uint32_t execution_count() const { return execution_count_; }

    void set_sq_side(uint32_t sqSide) { sq_side_ = sqSide; }
    uint32_t sq_side() const { return sq_side_; }

    virtual void buffer2wrapped();

    // Input and output
    virtual void ReadImage(const char *path) = 0;
    virtual void WriteSrcImage(const char *path) const = 0;
    virtual void WriteDstImage(const char *path) const = 0;

 private:
    //Helpers
    void AllocateImage();
    void CheckerBoard();

 protected:
    uint8_t  *img_buffer = NULL;

 private:
    uint32_t image_width_;
    uint32_t image_height_;
    uint32_t execution_count_;
    //TODO(somebody) add more synthetic types of images!!!
    uint32_t sq_side_;

    std::vector<uint64_t> upload_time_;
    std::vector<uint64_t> download_time_;
    std::vector<uint64_t> execution_time_;
    uint64_t total_time_;

    /* Analysis results */
    uint64_t total_upload_;
    uint64_t total_download_;
    uint64_t total_execution_;

    double stdev_upload_;
    double stdev_download_;
    double stdev_execution_;

    double median_upload_;
    double median_download_;
    double median_execution_;

    double mean_upload_;
    double mean_download_;
    double mean_execution_;
};

# define CREATE_TEST_FN(classname) \
    []() { \
        classname test = classname(); \
        test.Prelude(); \
        test.Run(); \
        test.Postlude(); \
        test.Analyze(); \
        test.ShowAnalysis(); \
        test.WriteCSV(); \
        test.WriteDstImage((test.Name() + ".jpg").c_str()); \
    }

# define SET_NAME(x)  std::string Name() const { return std::string(x); }

# define NO_OUTPUT_IMAGE void WriteDstImage(char const *path) const { }

# define _xcat(x, y) x ## y
# define _cat(x, y) _xcat(x, y)

# define _REGISTER_TEST(platform, test_type, classname, unique) \
    static struct _cat(AnonymousStruct, unique) { \
        _cat(AnonymousStruct, unique) () { \
            auto fn = CREATE_TEST_FN(classname); \
            ImagePerfTest::TestNode tnode = { fn, platform, test_type }; \
            ImagePerfTest::RegisterTest(tnode); \
        } \
    } _cat(AnonymousVar, unique)

#define REGISTER_TEST(platform, test_type, classname) \
    _REGISTER_TEST(platform, test_type, classname, __COUNTER__)

#endif //PERFTESTS_PERFTESTS_H
