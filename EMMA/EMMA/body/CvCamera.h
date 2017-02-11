#ifndef MOCKINECT_H
#define MOCKINECT_H

#include <memory>
#include <opencv2/opencv.hpp>

#include "CurrentState.h"

struct CameraData
{
    cv::Mat frame;
    JointPositions joints;
};

class CvCamera
{
public:
    CvCamera();
    virtual CameraData run();
protected:
    int cvChannelType = CV_8UC4;
    int colorWidth = 1920;
    int colorHeight = 1080;
    std::shared_ptr<cv::Mat> get_blank_mat();
};

#endif // MOCKINECT_H
