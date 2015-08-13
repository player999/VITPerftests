
#ifndef PERFTESTS_PERFTESTS_H
# define PERFTESTS_PERFTESTS_H
# include <stdint.h>
# include <stdlib.h>
# include <vector>
# include <string>

class ImagePerfTest {
private:
    //Helpers
    void allocateImage();
    void checkerBoard();

    uint32_t imageWidth = 0;
    uint32_t imageHeight = 0;
    uint32_t executionCount = 1;
    uint32_t sqSide = 20;

    std::vector<uint64_t> upload_time;
    std::vector<uint64_t> download_time;
    std::vector<uint64_t> execution_time;
    uint64_t total_time;

public:
    uint8_t  *imgBuffer = NULL;

    // Constructors
    ImagePerfTest(uint32_t height, uint32_t width);
    ImagePerfTest();
    virtual ~ImagePerfTest() {}

    //Main functions
    virtual void execute() = 0;
    uint64_t run();
    virtual void uploadToDevice();
    virtual void downloadFromDevice();
    virtual std::string name();
    void showAnalysis();

    // Getters and setters
    void setImageHeight(uint32_t height);
    void setImageWidth(uint32_t width);
    uint32_t getImageWidth();
    uint32_t getImageHeight();

    void setExecutionCount(uint32_t);
    uint32_t getExecutionCount();

    void setSqSide(uint32_t);
    uint32_t getSqSide();

    virtual void buffer2wrapped();

    // Input and output
    virtual void readImage(const char *path) = 0;
    virtual void writeSrcImage(const char *path) = 0;
    virtual void writeDstImage(const char *path) = 0;

};

# define RUN_TEST(classname) \
{\
    classname test_function = classname(); \
    test_function.run(); \
    test_function.showAnalysis();\
    std::cout<<std::endl; \
}

# define SET_NAME(x) \
std::string name() { \
    return std::string(x);\
}

#endif //PERFTESTS_PERFTESTS_H
