
#ifndef _VISIONTHRESHOLD_GAURD_H
#define _VISIONTHRESHOLD_GAURD_H

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <iostream>
using namespace cv;
int ThresholdOP(cv::Mat source,cv::Mat & dst,int x, int y, bool day);
int FindMaxIntensityRange(cv::Mat source);
#endif
