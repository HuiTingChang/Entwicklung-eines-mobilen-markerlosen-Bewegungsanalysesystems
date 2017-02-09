#include "cvCapture.h"

#include <QWidget>
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

Capture::Capture(QObject* parent):
	QThread(parent)
{
}

void Capture::run()
{
	// routine for thread start...
	exec();
}

void Capture::update() {
	
	double angle = 0;
	JointPositions jointPos;
	emit jointReady(jointPos);

	Mat frame = kinect.run(jointPos);

	if (frame.empty())
	{
		qDebug() << "read frame failed";
	}
	else
	{
		qDebug() << "read frame succeeded!!!";
		emit matReady(frame);
	}
}
