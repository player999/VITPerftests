
#ifndef PERFTESTS_PERFTESTS_H
# define PERFTESTS_PERFTESTS_H
# include <gtest/gtest.h>
# include <stdint.h>
# include <stdlib.h>
# include <vector>

class ImagePerfTest {
private:
    //Helpers
    void allocateImage();
    void checkerBoard();
    void buffer2wrapped();

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

    //Main functions
    virtual void execute() = 0;
    uint64_t run();
    virtual void uploadToDevice();
    virtual void downloadFromDevice();
    std::string name();
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

    // Input and output
    virtual void readImage(const char *path) = 0;
    virtual void writeSrcImage(const char *path) = 0;
    virtual void writeDstImage(const char *path) = 0;

};

#endif //PERFTESTS_PERFTESTS_H
