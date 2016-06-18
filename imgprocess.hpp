#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

bool detectRed(cv::Mat& img, cv::Point2d& objPoint)
{
    /*
    Mat hsv;
    Mat mask = Mat(hsv.size(), CV_8UC1);
    Mat mask2 = Mat(hsv.size(), CV_8UC1);

    uchar l1[3] = {0, 50, 50};
    uchar h1[3] = {20, 255, 255};
    Mat redlow1 = Mat(1, 3, CV_8UC1, l1);
    Mat redhigh1 = Mat(1,3,CV_8UC1,h1);
    uchar l2[3] = {160, 50, 50};
    uchar h2[3] = {180, 255, 255};
    Mat redlow2 = Mat(1, 3, CV_8UC1, l2);
    Mat redhigh2 = Mat(1,3,CV_8UC1,h2);

    inRange(hsv, redlow1, redhigh1, mask);
    inRange(hsv, redlow2, redhigh2, mask2);
    bitwise_or(mask, mask2, mask);
    //bitwise_and(frame, frame, res, mask);

    Mat imgTh;
    threshold(mask, imgTh, 100, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    findContours(imgTh, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0));

    if(contours.size() == 0){
        return false;
    }
    int contourSize = contours[0].size();
    for(auto iter = contours.begin(); iter!=contours.end(); ++iter){
        if (iter->size() > contourSize)
            contourSize = iter->size();
    }

    if(contourSize < 100){
        cout << "no red box" << endl;
        return false;
    }
    int flag = 0;
    for(auto iter = contours.begin(); iter!=contours.end(); ++iter){
        flag++;
        if(iter->size() == contourSize)
            break;
    }

    long int x = 0;
    long int y = 0;

    for(auto iter = contours[flag-1].begin(); iter!=contours[flag-1].end(); ++iter){
        x += iter->x;
        y += iter->y;
    }
    x /= contours[flag-1].size();
    y /= contours[flag-1].size();
    objPoint.x = x;
    objPoint.y = y;
    */
    return true;
}

bool whereIsRed(cv::Mat& img) // true for right, false for left
{
    return true;
}
