#include "cvConverter.h"

#include <QTimerEvent>
#include <QThread>
#include <QDebug>
#include <QResizeEvent>
#include <QLibrary>
#include <QFile>

#include <iostream>                         // cout, endl
#include <cstdint>                         // uint32_t

Converter::Converter(QObject* parent, QSize widget_size):
	QThread(parent),
	m_processAll(true),
	widget_size(widget_size)
{
}

void Converter::setSize(QSize s)
{
	widget_size = s;
}

QSize Converter::get_widget_size()
{
	return widget_size;
}

void Converter::setProcessAll(bool all)
{
	m_processAll = all;
}

// New Frame
void Converter::processFrame(const cv::Mat & frame) {
	if (m_processAll) 
		process(frame);
	else 
		queue(frame);
}

void Converter::timerEvent(QTimerEvent * ev) {
	if (ev->timerId() != m_timer.timerId())
		return;
	process(m_frame);
	m_frame.release();
	m_timer.stop();
}

// vom processFrame aufgerufen
void Converter::queue(const Mat & frame)
{
    if (!m_frame.empty())
        qDebug() << "Converter dropped frame!";
    m_frame = frame;
    if (! m_timer.isActive())
        m_timer.start(50, this);
}

// vom convertMatToQImage aufgerufen
void Converter::matDeleter(void* mat)
{
	delete static_cast<Mat*>(mat);
}


// vom process aufgerufen; To alter
// TODO look at https://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap/
QImage Converter::convertMatToQImage(Mat const& inMat, bool bgr2rgb)
{
	cv::Mat tmp = inMat;
	QImage dest((const uchar*)tmp.data, tmp.cols, tmp.rows, tmp.step,
	   QImage::Format_Indexed8, &matDeleter, new cv::Mat(tmp)); //)
	dest.bits(); // enforce deep copy, see documentation
	return dest;
}

// vom processFrame und timerEvent aufgerufen
void Converter::process(Mat frame) {


	if (frame.empty())
		return;

	cv::Size size(widget_size.width(), widget_size.height());
	bool fit_longer_side = true;
	int compare_wideness_widget_to_frame = frame.rows*widget_size.width() - frame.cols*widget_size.height();
	if (0 < compare_wideness_widget_to_frame && fit_longer_side)
	{
		size.width = size.height * frame.cols / frame.rows;
	}
	else if (0 > compare_wideness_widget_to_frame) // if it's equal, nothing to do
	{
		size.height = size.width * frame.rows / frame.cols;
	}
	
	// vom cv
	resize(frame, frame, size);
	if (!(frame.type() == CV_8UC1 || frame.type() == CV_8U))
		cv::cvtColor(frame, frame, CV_BGR2RGB);

	const QImage image = convertMatToQImage(frame, false);

	emit imageReady(image);
}

