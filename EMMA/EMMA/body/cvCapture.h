#include <QThread>
#include <QFuture>
#include "ApplicationData.h"

// OpenCv
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

// Eigene Dateien
#ifdef WIN32
#include "KinectCamera.h"
#endif

#ifndef __CAMERA__
#include "CvCamera.h"
#define KinectCamera CvCamera
#endif

class CameraCapture : public QThread
{
	Q_OBJECT
public:
	CameraCapture(QObject * parent);
	void run();
	CvCamera::State get_state();
	Q_SLOT void update();
	Q_SIGNAL void matReady(const cv::Mat &);
	Q_SIGNAL void jointReady(const JointPositions&, const JointOrientations&);
	Q_SIGNAL void cameraStateChanged(CvCamera::State state);

private:
	// Constructor Initialisation wird aufgerufen
	QFuture<void> kinect_init_future;
	KinectCamera kinect;
	CvCamera::State state;
};
