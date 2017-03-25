#ifndef MOCKINECT_H
#define MOCKINECT_H

#include <memory>
#include <QException>
#include <QObject>
#include <QMap>
#include <opencv2/opencv.hpp>

#include "CurrentState.h"

struct CameraData
{
    CameraData()
    {
    }
    CameraData(
            cv::Mat frame,
            JointPositions jpositions,
            JointOrientations jorientations):
        frame(frame),
        jpositions(jpositions),
        jorientations(jorientations)
    {
    }

    cv::Mat frame;
    JointPositions jpositions;
    JointOrientations jorientations;
};

class CvCamera: QObject
{
    Q_OBJECT
public:
    enum State : int
    {
        CONNECTED,
        DISCONNECTED
    };
    
	static const QMap<State,QString> STATE_DESCRIPTION;
	
	
	
    CvCamera(int destinationWidth=960, int destinationHeight=540);
    virtual CameraData run();
    virtual void initialize();
protected:
    int cvChannelType = CV_8UC4;
    int colorWidth = 1920;
    int colorHeight = 1080;
    const cv::Size destinationSize;
    std::shared_ptr<cv::Mat> get_blank_mat();
};

class camera_error: public QException
{
public:
	virtual const char* what() const throw()=0;
};

class camera_inactive_error: public camera_error
{
public:
	void raise() const { throw *this; }
	camera_inactive_error *clone() const { return new camera_inactive_error(*this); }
	virtual const char* what() const throw()
	{
		return "Camera does not have any open stream";
	}
};

class camera_not_available_error: public camera_error
{
public:
	void raise() const { throw *this; }
	camera_not_available_error *clone() const { return new camera_not_available_error(*this); }
	virtual const char* what() const throw()
	{
		return "Camera is inactive";
	}
};

class camera_not_found_error: public camera_error
{
public:
	void raise() const { throw *this; }
	camera_not_found_error *clone() const { return new camera_not_found_error(*this); }
	virtual const char* what() const throw()
	{
		return "Camera can not be found";
	}
};

class camera_has_no_frame_error: public camera_error
{
public:
	void raise() const { throw *this; }
	camera_has_no_frame_error *clone() const { return new camera_has_no_frame_error(*this); }
	virtual const char* what() const throw()
	{
		return "Camera has no frame";
	}
};

#endif // MOCKINECT_H
