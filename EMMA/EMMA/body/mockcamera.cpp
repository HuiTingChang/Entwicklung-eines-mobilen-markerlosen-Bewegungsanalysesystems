#include "mockcamera.h"

MockCamera::MockCamera()
{

}

cv::Mat MockCamera::run(double *angle)
{
    // Resize Image
    //Mat frame = cv::imread("C:/SWE 16_17/Gruppe-13/Kinect_Body_Code/Body_Widget/penguin2.jpg");
    cv::Mat resizeMat;
    const double scale = 0.5;
    cv::resize(colorMat, resizeMat, cv::Size(), scale, scale);
    // Return image
    return resizeMat;
}
