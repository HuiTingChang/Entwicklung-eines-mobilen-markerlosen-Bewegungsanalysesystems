#pragma once
#include <memory>
#include <QPoint>
#include <QSize>
/*
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#undef ACCESS_MASK
*/
namespace cv
{
	class Mat;
};

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
 
	std::vector<cv::Mat> rvecs;
	std::vector<cv::Mat> tvecs;
 
	std::shared_ptr<cv::Mat> cameraMatrix ;


};

