#include <QDebug>
#include <QPoint>
#include "ApplicationData.h"
#include "INIReader.h"

#define INI_PATH "emma.ini"

// Klasse fuer die globalen Variablen
ApplicationData::ApplicationData()
{
	// default values	



	// Test Things 
	weight = 0; 
	centOfPr = QPoint(0, 0);
	widgetWork = true;
	boardConnected = false;  



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
