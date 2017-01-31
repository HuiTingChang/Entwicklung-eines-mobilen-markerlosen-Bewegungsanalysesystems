#include "ApplicationData.h"
#include "cpp/INIReader.h"

#define INI_PATH "emma.ini"

// Klasse fuer die globalen Variablen
ApplicationData::ApplicationData()
{
	// default values
	widgetWork = true;

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
