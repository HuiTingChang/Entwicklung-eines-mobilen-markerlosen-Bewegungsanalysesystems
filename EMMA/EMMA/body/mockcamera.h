#ifndef MOCKINECT_H
#define MOCKINECT_H

#include <opencv2/opencv.hpp>

class MockCamera
{
public:
    MockCamera();
    cv::Mat run(double* angle);
private:
    cv::Mat colorMat;
};

#endif // MOCKINECT_H
