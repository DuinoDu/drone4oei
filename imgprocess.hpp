#include <opencv2/opencv.hpp>

bool detectRed(cv::Mat& img, cv::Point2d& objPoint)
{
    objPoint.x = 0;
    objPoint.y = 0;
    return true;
}

bool whereIsRed(cv::Mat& img) // true for right, false for left
{
    return true;
}
