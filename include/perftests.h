
#ifndef PERFTESTS_PERFTESTS_H
# define PERFTESTS_PERFTESTS_H
# include <stdint.h>
# include <stdlib.h>
# include <vector>
# include <string>

class ImagePerfTest {
 public:
    typedef void (* test_fn)();
    static std::vector<test_fn> tests;

    static void RunAllTests();
    static void RegisterTest(test_fn test);

    ImagePerfTest();
    ImagePerfTest(uint32_t height, uint32_t width);
    virtual ~ImagePerfTest() {}

    uint64_t Run();
    virtual void Execute() = 0;
    virtual void UploadToDevice();
    virtual void DownloadFromDevice();
    virtual std::string Name() const;
    void ShowAnalysis() const;

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
    uint32_t sq_side_;

    std::vector<uint64_t> upload_time_;
    std::vector<uint64_t> download_time_;
    std::vector<uint64_t> execution_time_;
    uint64_t total_time_;
};

# define RUN_TEST(classname) \
    {\
        classname test_function = classname(); \
        test_function.Run(); \
        test_function.ShowAnalysis(); \
        test_function.WriteDstImage("out.jpg"); \
        std::cout << std::endl; \
    }

# define CREATE_TEST_FN(classname) []() { RUN_TEST(classname); }

# define SET_NAME(x)  std::string Name() const { return std::string(x); }

# define NO_OUTPUT_IMAGE void WriteDstImage(const char *path) const { }

# define _xcat(x, y) x ## y
# define _cat(x, y) _xcat(x, y)

# define _ANAME _cat(Anonymous, __LINE__)

# define REGISTER_TEST(classname) \
    static struct _ANAME { \
        _ANAME(){ ImagePerfTest::RegisterTest(CREATE_TEST_FN(classname)); } \
    } _cat(_ANAME, var)

#endif //PERFTESTS_PERFTESTS_H

