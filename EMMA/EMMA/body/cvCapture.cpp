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
using namespace cv;

Capture::Capture(QObject* parent, ApplicationData* data):
QThread(parent), gl_data(data)
{
}

void Capture::run()
{
	try
	{
		kinect.initialize();
		gl_data->cameraConnected = true;
		emit cameraConnected(QString("Connected"));
	}
	catch(const camera_error& e)
	{
		emit cameraConnected(QString("Not connected"));
	}
	
	// routine for thread start...
	exec();
}

void Capture::update() {
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
