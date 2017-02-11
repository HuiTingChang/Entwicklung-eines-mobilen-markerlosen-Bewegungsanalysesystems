#pragma once
#include <QPoint>
#include <QSize>

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

};

