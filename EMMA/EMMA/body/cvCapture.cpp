#include "cvCapture.h"

#include <QTimerEvent>
#include <QThread>
#include <QResizeEvent>
#include <QDebug>
#include <QFile>

#include <iostream>                         // cout, endl
#include <cstdint>                         // uint32_t
#include <QLibrary>

using namespace std;

void kinect_initialize(KinectCamera* kc)
{
	try
	{
		kc->initialize();
		emit cameraStateChanged(CvCamera::CONNECTED);
	}
	catch (const camera_error& e)
	{
		emit cameraStateChanged(CvCamera::DISCONNECTED);
	}
}

CameraCapture::CameraCapture(QObject* parent):
    QThread(parent),
    state(CvCamera::DISCONNECTED)
{
}

void CameraCapture::run()
{
}

void Capture::run()
{
	
	// routine for thread start...
	exec();
}

void CameraCapture::update()
{
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
