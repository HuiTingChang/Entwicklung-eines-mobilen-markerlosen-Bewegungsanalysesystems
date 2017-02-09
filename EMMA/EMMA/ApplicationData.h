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
	double weight;
	bool balanceDataUpdated;
	bool cameraDataUpdated;

};

