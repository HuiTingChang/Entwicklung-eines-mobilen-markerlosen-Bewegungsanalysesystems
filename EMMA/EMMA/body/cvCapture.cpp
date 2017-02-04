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

Capture::Capture(QObject * parent) : QObject(parent), m_stopNow(false), isStopped(true)
{

}

void Capture::start() 
{
	isStopped = false;
    // Ruft die timerEvent Funktion von QObject (this) auf, die entsprechend ueberschrieben wird.
	m_timer.start(10, this); 
	emit started();
}

void Capture::stop()
{
	isStopped = true;
	m_timer.stop();
}

// check if the grabbing is stopped
bool Capture::getIsStopped()
{
	return isStopped;
}

void Capture::timerEvent(QTimerEvent * ev) {
	
	if (ev->timerId() != m_timer.timerId()) return;

	double angle = 0;
	QMap<uint, CameraSpacePoint> jointPos;
	Mat frame = kinect.run(jointPos);

	if (frame.empty())
	{
		qDebug() << "read frame failed";
	}
	else
	{
		qDebug() << "read frame suceeded!!!";
	}

	emit matReady(frame, jointPos);
}
