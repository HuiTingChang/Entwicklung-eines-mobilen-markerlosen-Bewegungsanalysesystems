#include "cvCapture.h"

Capture::Capture(QObject * parent) : QObject(parent), m_stopNow(false), isStopped(true)
{

}

void Capture::start() 
{
	isStopped = false;
	// Ruft die timerEvent Funktion von QObject (this) auf, die entsprechend überschrieben wird.
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

	//Mat frame =cv::imread("C:/SWE 16_17/Gruppe-13/Kinect_Body_Code/Body_Widget/penguin2.jpg");
	Mat frame = kinect.run();

	if (frame.empty())
	{
		qDebug() << "read frame failed";
		//m_timer.stop();
		// return;
	}
	else
	{
		qDebug() << "read frame suceeded!!!";
		//         namedWindow("Loaded Image from DLL", CV_WINDOW_AUTOSIZE);
		//         imshow("Loaded Image from DLL", frame);
		//         waitKey(0);
	}


	///// ALTE VERSION //////////

	//    if (!m_videoCapture->read(frame)) { // Blocks until a new frame is ready
	//        m_timer.stop();
	//        return;
	//    }
	//    if (m_stopNow)
	//    {
	//        if (m_videoCapture->isOpened())
	//            m_videoCapture->release();
	//        m_videoCapture.reset();
	//        m_timer.stop();

	//    }


	emit matReady(frame);
}
