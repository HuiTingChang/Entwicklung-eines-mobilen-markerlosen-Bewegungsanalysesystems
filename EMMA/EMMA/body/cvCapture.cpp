#include "cvCapture.h"

#include <QtConcurrent>
#include <QTimerEvent>
#include <QThread>
#include <QResizeEvent>
#include <QDebug>
#include <QFile>

#include <iostream>                         // cout, endl
#include <cstdint>                         // uint32_t
#include <QLibrary>

using namespace std;

void kinect_initialize(CameraCapture* capture, KinectCamera* kc)
{
	try
	{
		kc->initialize();
		emit capture->cameraStateChanged(CvCamera::CONNECTED);
	}
	catch (const camera_error& e)
	{
		emit capture->cameraStateChanged(CvCamera::DISCONNECTED);
	}
}

CameraCapture::CameraCapture(QObject* parent):
	QThread(parent),
	kinect_init_future(QtConcurrent::run(kinect_initialize, this, &kinect)),
	state(CvCamera::DISCONNECTED)
{
}

void CameraCapture::run()
{
	try
	{
		kinect_init_future.waitForFinished();
		emit cameraStateChanged(CvCamera::CONNECTED);
	}
	catch(const camera_error& e)
	{
		emit cameraStateChanged(CvCamera::DISCONNECTED);
	}
	// routine for thread start...
	exec();
}

void CameraCapture::update()
{
	kinect_init_future.waitForFinished();
	CameraData data = kinect.run();

	emit jointReady(data.jpositions, data.jorientations);

	if (data.frame.empty())
	{
		qDebug() << "read frame failed";
	}
	else
	{
		qDebug() << "read frame succeeded!!!";

		emit matReady(data.frame);
		
	}
}

CvCamera::State CameraCapture::get_state()
{
    return state;
}
