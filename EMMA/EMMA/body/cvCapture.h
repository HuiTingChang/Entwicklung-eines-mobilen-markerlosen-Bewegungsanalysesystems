#include <QThread>
#include "ApplicationData.h"

// OpenCv
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>      // Mat

//#include <windows.h>

// Eigene Dateien
#ifdef WIN32
#include "KinectCamera.h"
#endif

#ifndef __CAMERA__
#include "CvCamera.h"
#define KinectCamera CvCamera
#endif

class Capture : public QThread {
	Q_OBJECT
	

public:
	Capture(QObject * parent, ApplicationData *data);
	void run();
	Q_SLOT void update();
	Q_SIGNAL void matReady(const cv::Mat &);
	Q_SIGNAL void jointReady(const JointPositions&, const JointOrientations&);
	Q_SIGNAL void cameraConnected(QString);

private:
	// Constructor Initialisation wird aufgerufen
	KinectCamera kinect;
	ApplicationData* gl_data;
};
