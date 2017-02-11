#ifndef MOCKINECT_H
#define MOCKINECT_H

#include <opencv2/opencv.hpp>

#include "CurrentState.h"

class MockCamera
{
public:
    MockCamera();
    cv::Mat run(JointPositions& j);
private:
    int cvChannelType = CV_8UC4;
    int colorWidth = 1920;
    int colorHeight = 1080;
    cv::Mat colorMat;
};

#endif // MOCKINECT_H
