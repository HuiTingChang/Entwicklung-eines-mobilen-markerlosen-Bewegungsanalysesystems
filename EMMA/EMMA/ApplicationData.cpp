#include <QDebug>
#include <QPoint>
#include "ApplicationData.h"
#include "INIReader.h"

#define INI_PATH "emma.ini"
using namespace cv; 
// Klasse fuer die globalen Variablen
ApplicationData::ApplicationData():
	// default values	
	centOfPr(0, 0),
	bodyRenderSize(480,367),
	widgetWork(true),
	boardConnected(false),
	cameraConnected(false),
	weight(0),
	balanceDataUpdated(false),
	cameraDataUpdated(false),
	main_timer_interval_ms(50),
	board_show_interval_ms(250),
	calibrationStart(false),
	cameraMatrix(Mat::eye(3, 3, CV_32FC1))
{
	// for available methods see here
	// https://github.com/benhoyt/inih/blob/master/cpp/INIReader.h
	INIReader reader(INI_PATH);
	if(reader.ParseError() < 0)
	{
		qDebug() << "Can't load '" << INI_PATH
			<< "'; using default values.";
	}
	else
	{
		widgetWork = reader.GetBoolean("wii", "widgetWork", widgetWork);
	}
}


ApplicationData::~ApplicationData()
{
}
