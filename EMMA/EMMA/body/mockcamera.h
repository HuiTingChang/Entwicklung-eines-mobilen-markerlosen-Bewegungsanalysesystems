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
    cv::Mat colorMat;
};

#endif // MOCKINECT_H
