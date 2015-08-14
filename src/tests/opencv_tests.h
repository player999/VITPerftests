//
// Created by vadym on 14.08.15.
//

#ifndef PERFTESTS_OPENCV_TESTS_H
# define PERFTESTS_OPENCV_TESTS_H

# include "perf_opencv.h"

class test_boxfilter_cv : public CVImagePerfTest {
 public:

  SET_NAME("Box filter OpenCV 3");

  test_boxfilter_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
    setSqSide(SQSIDE);
    setExecutionCount(RUN_COUNT);
  }

  void Execute() {
    cv::boxFilter(wrappedSrcImageHost, wrappedDstImageHost, wrappedDstImageHost.depth(), cvSize(13, 13),
                  cvPoint(-1,-1), true, cv::BORDER_REFLECT101);
  }
};

class test_resize_cv : public CVImagePerfTest {
 public:

  SET_NAME("Resize 2x2 OpenCV 3");

  test_resize_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
    setSqSide(SQSIDE);
    setExecutionCount(RUN_COUNT);
  }

  void Execute() {
    cv::resize(wrappedSrcImageHost, wrappedDstImageHost, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
  }
};

class test_integral_cv : public CVImagePerfTest {
 public:

  SET_NAME("Integral image OpenCV 3");

  test_integral_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
    setSqSide(SQSIDE);
    setExecutionCount(RUN_COUNT);
  }

  void Execute() {
    cv::integral(wrappedSrcImageHost, wrappedDstImageHost, wrappedSrcImageHost.depth());
  }
};

class test_morphology_cv : public CVImagePerfTest {
 public:

  SET_NAME("Erode image OpenCV 3");

  cv::Mat kernel;
  cv::Point anchor;

  test_morphology_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
    setSqSide(SQSIDE);
    setExecutionCount(RUN_COUNT);
    kernel = cv::Mat::ones(1, 13, CV_8UC1);
    anchor = cv::Point(-1, -1);
  }

  void Execute() {
    cv::erode(wrappedSrcImageHost, wrappedDstImageHost, kernel, anchor, 1, cv::BORDER_REFLECT101);
  }
};

class test_otsu_cv : public CVImagePerfTest {
 public:

  SET_NAME("Otsu threshold OpenCV 3");
  NO_OUTPUT_IMAGE

  double thresh;

  test_otsu_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
    setSqSide(SQSIDE);
    setExecutionCount(RUN_COUNT);
  }

  void Execute() {
    thresh = cv::threshold(wrappedSrcImageHost, wrappedDstImageHost, 0, 255, cv::THRESH_OTSU | cv::THRESH_BINARY);
  }
};

class test_hist_cv : public CVImagePerfTest {
 public:

  SET_NAME("Histogram OpenCV 3");
  NO_OUTPUT_IMAGE

  int histSize = 256;
  float histRange[2] = {0,256};
  const float *rangePtr = {histRange};

  test_hist_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
    setSqSide(SQSIDE);
    setExecutionCount(RUN_COUNT);
  }

  void Execute() {
    cv::calcHist(&wrappedSrcImageHost, 1, 0, cv::Mat(), wrappedDstImageHost, 1, &histSize, &rangePtr,
                 true, false);
  }
};

class test_compare_cv : public CVImagePerfTest {
 public:

  SET_NAME("Compare OpenCV 3");

  test_compare_cv() : CVImagePerfTest(IMWIDTH,IMHEIGHT) {
    setSqSide(SQSIDE);
    setExecutionCount(RUN_COUNT);
  }

  void Execute() {
    cv::compare(wrappedSrcImageHost, 0.5, wrappedDstImageHost, cv::CMP_GE);
  }
};

REGISTER_TEST(test_boxfilter_cv);
REGISTER_TEST(test_resize_cv);
REGISTER_TEST(test_integral_cv);
REGISTER_TEST(test_morphology_cv);
REGISTER_TEST(test_otsu_cv);
REGISTER_TEST(test_hist_cv);
REGISTER_TEST(test_compare_cv);

#endif //PERFTESTS_OPENCV_TESTS_H
