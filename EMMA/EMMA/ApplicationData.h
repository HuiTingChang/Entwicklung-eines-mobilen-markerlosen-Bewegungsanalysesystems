#pragma once
#include <QPoint>

class ApplicationData // Klasse fuer die globalen Variablen
{
public:
	ApplicationData();
	~ApplicationData();


public : 
	QPoint centOfPr;
	bool widgetWork;
	bool boardConnected;
	double weight;
	bool balanceDataUpdated;
	bool cameraDataUpdated;

};

