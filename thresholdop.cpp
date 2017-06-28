//#include "defines.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

//Find the Peak for histogram of a mat
//<Remarks>Works well for small enough Mat's</Remarks>
//Do not Try it for large Mat's
int FindMaxIntensityRange(cv::Mat source){
    /// Establish the number of bins
    int histSize = 256;
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true; bool accumulate = false;

    Mat hist;

    calcHist(&source, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    //Calculating histogram
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));


    double min, max;
    Point minLoc, maxLoc;
    cv::minMaxLoc(hist, &min, &max, &minLoc, &maxLoc);
    //Find the peak of the histogoram
    return maxLoc.y;

}
//OutPut will be GrayScale
int ThresholdOP(Mat source,Mat & dst, int x, int y, bool day){


    cv::Size smallSize(x,y);
    int verticalCount = source.rows / smallSize.height;
    int horizentalCount = source.cols / smallSize.width;;
    std::vector < Mat > colImages;

    int offset = 15;
    if (day) offset = 20;
    if (!day) offset = 15;
    //slice the mat into smaller mats
    //apply FindMaxIntensityRange() and then merge them back again
    for (int y = 0; (y + smallSize.height) < source.rows; y += smallSize.height)
    {
        std::vector < Mat > rowImages;
        for (int x = 0; (x + smallSize.width) < source.cols; x += smallSize.width)
        {
            cv::Rect rect = cv::Rect(x, y, smallSize.width, smallSize.height);
            Mat sliced = source(rect);
            cvtColor(sliced, sliced, CV_BGR2GRAY);
            double thresholdLowerLimit = FindMaxIntensityRange(sliced);

            cv::threshold(sliced, sliced, thresholdLowerLimit + offset, 255, CV_THRESH_BINARY);
            rowImages.push_back(sliced);
            if (rowImages.size() == (source.cols / smallSize.width))
            {
                Mat out;
                cv::hconcat(rowImages, out);
                Mat a = out.clone();
                colImages.push_back(a);
            }
        }
        if (colImages.size()+1 == (source.rows / smallSize.height))
        {
            cv::vconcat(colImages, dst);
            rowImages.push_back(dst);
        }
    }
    return 0;
}


