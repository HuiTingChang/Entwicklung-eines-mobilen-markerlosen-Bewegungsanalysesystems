#include "cvCapture.h"

#include <QtConcurrent>
#include <QMutexLocker>
#include <QDebug>

using namespace std;

void kinect_initialize(CameraCapture* capture, KinectCamera* kc)
{
	try
	{
		kc->initialize();
	}
	catch (const camera_error& e)
	{
		// do something?
	}
	emit capture->cameraStateChanged(*(kc->get_state_ptr()));
}

CameraCapture::CameraCapture():
	kinect_init_future(QtConcurrent::run(kinect_initialize, this, &kinect))
{
}

CameraCapture::~CameraCapture()
{
	waitAllForFinished();
}

void CameraCapture::waitAllForFinished()
{
	QMutexLocker queueLock(&updateQueueMutex);
	for (auto i = updateQueue.begin(); i != updateQueue.end(); ++i)
	{
		i->waitForFinished();
	}
}

void CameraCaptureUpdate(CameraCapture* capture, QFuture<void>* kf, KinectCamera* kc)
{
	kf->waitForFinished();
	CameraData data = kc->run();

	emit capture->jointReady(data.jpositions, data.jorientations);

	if (data.frame.empty())
	{
		qDebug() << "read frame failed";
	}
	else
	{
		qDebug() << "read frame succeeded!!!";

		emit capture->matReady(data.frame);
		
	}
}

void CameraCapture::update()
{
	if (updateQueueMutex.tryLock())
	{
		updateQueueMutex.unlock();
		QMutexLocker queueLock(&updateQueueMutex);
		updateQueue.append(QtConcurrent::run(CameraCaptureUpdate, this, &kinect_init_future, &kinect));
		for (auto i = updateQueue.begin(); i != updateQueue.end() && i->isFinished();)
		{
			auto j = i;
			++i;
			updateQueue.erase(j);
		}
	}
}

CvCamera::State CameraCapture::get_state()
{
	return *(kinect.get_state_ptr());
}
