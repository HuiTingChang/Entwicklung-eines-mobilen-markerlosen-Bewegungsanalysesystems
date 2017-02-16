#pragma once
#include <QPoint>
#include <QSize>

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#undef ACCESS_MASK

class ApplicationData // Klasse fuer die globalen Variablen
{
public:
	ApplicationData();
	~ApplicationData();


public : 
	QPoint centOfPr;
	QSize bodyRenderSize;
	bool widgetWork;
	bool boardConnected;
	bool cameraConnected;
	double weight;
	bool balanceDataUpdated;
	bool cameraDataUpdated;
	int main_timer_interval_ms;
	int board_show_interval_ms; 
	bool calibrationStart; 
 
	cv::vector<cv::Mat> rvecs;
	cv::vector<cv::Mat> tvecs;
 
	cv::Mat cameraMatrix ;


};

