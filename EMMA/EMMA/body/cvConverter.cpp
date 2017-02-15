#include "cvConverter.h"

#include <QTimerEvent>
#include <QThread>
#include <QDebug>
#include <QResizeEvent>
#include <QLibrary>
#include <QFile>

#include <iostream>                         // cout, endl
#include <cstdint>                         // uint32_t

Converter::Converter(QObject* parent, QSize widget_size, ApplicationData* data) :
	QThread(parent),
	m_processAll(true), 
	widget_size(widget_size)
{
	app_data = data; 
	  matCount = 0;

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



void Converter::saveMat(Mat const& tmp){

	vector<Point3f> worldCorner;
	worldCorner.resize(18);
	worldCorner[0] = Point3f(3.9*1 ,3.9, 0);
	worldCorner[1] = Point3f(3.9*2, 3.9, 0);
	worldCorner[2] = Point3f(3.9*3, 3.9, 0);
	worldCorner[3] = Point3f(3.9*4, 3.9, 0);
	worldCorner[4] = Point3f(3.9*5, 3.9, 0);
	worldCorner[5] = Point3f(3.9*6, 3.9, 0);
 
	worldCorner[6] = Point3f(3.9 * 1,2*3.9, 0);
	worldCorner[7] = Point3f(3.9 * 2,2*3.9, 0);
	worldCorner[8] = Point3f(3.9 * 3,2*3.9, 0);
	worldCorner[9] = Point3f(3.9 * 4,2*3.9, 0);
	worldCorner[10] =Point3f(3.9 * 5,2*3.9, 0);
	worldCorner[11] =Point3f(3.9 * 6,2*3.9, 0);
		 
	worldCorner[12] =Point3f(3.9 * 1,3* 3.9, 0);
	worldCorner[13] =Point3f(3.9 * 2,3* 3.9, 0);
	worldCorner[14] =Point3f(3.9 * 3,3* 3.9, 0);
	worldCorner[15] =Point3f(3.9 * 4,3* 3.9, 0);
	worldCorner[16] =Point3f(3.9 * 5,3 * 3.9,0);
	worldCorner[17] =Point3f(3.9 * 6,3 * 3.9,0);


	vector<Point2f> corners;
	Size patternsize(6, 3); //interior number of corners
	 

	Mat im_rgb = tmp.clone();
	Mat im_gray;
	cvtColor(im_rgb, im_gray, CV_RGB2GRAY);

	//imwrite("Gray_Image.jpg", im_gray);


	bool found = findChessboardCorners(im_gray, patternsize, corners, CV_CALIB_CB_ADAPTIVE_THRESH);

	
	Mat cameraMatrix = Mat::eye(3, 3, CV_32F);
	Mat distCoeffs = Mat::zeros(8, 1, CV_32F);
	vector<Mat> rvecs;
	vector<Mat> tvecs;

	 

	calibrateCamera(worldCorner, corners, Size(im_gray.size().width, im_gray.size().height), cameraMatrix, distCoeffs, rvecs, tvecs);







	if ( found)
	qDebug() << "YES! The chessboard is found " ; 
	else 
		qDebug() << "No! The chessboard is not found ";
	//tmp.copyTo(image);

	/*
	// write Mat to file
	string name = "file";
	string number = std::to_string((int)matCount);
	string typ = ".yml";

	cv::FileStorage fs(name + number + typ, cv::FileStorage::WRITE);
	fs << "yourMat" << image;
	


	// second type 

	app_data->mat_array[matCount] = tmp.clone();
	matCount++;
	if (matCount == 30){
		app_data->calibrationStart = false;
		// Dann kann man schon mir Calibration anfangen 
	}
	*/
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

	if (app_data->calibrationStart){
		saveMat(frame);
	}

	emit imageReady(image);
}

