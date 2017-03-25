#include <QObject>
#include <QBasicTimer>

// OpenCv
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>      // Mat

//#include <windows.h>

// Eigene Dateien
#include "camera.h"

using namespace std;
using namespace cv;

// associated with qRegisterMetaType<cv::Mat>(); in body_widget.cpp
Q_DECLARE_METATYPE(cv::Mat)

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

	bool getIsStopped();

private:
	void timerEvent(QTimerEvent * ev);
	bool m_stopNow;
	bool isStopped;
	
	// Constructor Initialisation wird aufgerufen
	Kinect kinect;



};
