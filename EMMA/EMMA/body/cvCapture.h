#include <QObject>
#include <QBasicTimer>

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

class Capture : public QObject {
	Q_OBJECT
	QBasicTimer m_timer;
	QScopedPointer<cv::VideoCapture> m_videoCapture;

public:
	Capture(QObject * parent = 0);
	Q_SIGNAL void started();
	Q_SLOT void start();
	Q_SLOT void stop();
	Q_SIGNAL void matReady(const cv::Mat &);
	Q_SIGNAL void jointReady(const JointPositions&);

	bool getIsStopped();

private:
	void timerEvent(QTimerEvent * ev);
	bool m_stopNow;
	bool isStopped;
	
	// Constructor Initialisation wird aufgerufen
	Kinect kinect;



};
