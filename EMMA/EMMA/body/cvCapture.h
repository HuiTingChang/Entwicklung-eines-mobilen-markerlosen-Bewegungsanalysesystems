#include <QThread>
#include "ApplicationData.h"

// OpenCv
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>      // Mat

//#include <windows.h>

// Eigene Dateien
#ifdef WIN32
#include "camera.h"
#endif

#ifndef __CAMERA__
#include "mockcamera.h"
#define Kinect MockCamera
#endif

class Capture : public QThread {
	Q_OBJECT
	

public:
	Capture(QObject * parent, ApplicationData *data);
	void run();
	Q_SLOT void update();
	Q_SIGNAL void matReady(const cv::Mat &);
	Q_SIGNAL void jointReady(const JointPositions&);
	Q_SIGNAL void cameraConnected(QString);

private:
	// Constructor Initialisation wird aufgerufen
	Kinect kinect;
	ApplicationData* gl_data;
};
