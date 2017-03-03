#include <QObject>
#include <QFuture>
#include <QLinkedList>
#include <QMutex>
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

class CameraCapture: public QObject
{
	Q_OBJECT
public:
	CameraCapture();
	~CameraCapture();
	void waitAllForFinished();
	CvCamera::State get_state();
	Q_SLOT void update();
	Q_SIGNAL void matReady(const cv::Mat &);
	Q_SIGNAL void jointReady(const JointPositions&, const JointOrientations&);
	Q_SIGNAL void cameraStateChanged(CvCamera::State state);

private:
	QFuture<void> kinect_init_future;
	KinectCamera kinect;
	QLinkedList<QFuture<void>> updateQueue;
	QMutex updateQueueMutex;
};
