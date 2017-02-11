#ifndef MOCKINECT_H
#define MOCKINECT_H

#include <memory>
#include <opencv2/opencv.hpp>

#include "CurrentState.h"

struct CameraData
{
    cv::Mat frame;
    JointPositions jpositions;
    JointOrientations jorientations;
};

class CvCamera
{
public:
    CvCamera();
    virtual CameraData run();
    virtual void initialize();
protected:
    int cvChannelType = CV_8UC4;
    int colorWidth = 1920;
    int colorHeight = 1080;
    std::shared_ptr<cv::Mat> get_blank_mat();
};

class camera_error: public std::exception
{
public:
	virtual const char* what() const throw()=0;
};

class camera_inactive_error: public camera_error
{
public:
	virtual const char* what() const throw()
	{
		return "Camera does not have any open stream";
	}
};

class camera_not_available_error: public camera_error
{
public:
	virtual const char* what() const throw()
	{
		return "Camera is inactive";
	}
};

class camera_not_found_error: public camera_error
{
public:
	virtual const char* what() const throw()
	{
		return "Camera can not be found";
	}
};

#endif // MOCKINECT_H
