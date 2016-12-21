# How to create a c++  app with Qt and Kinekt support

This procedure will make a sample C++ app with Qt using Kinect body functions.

Original build instructions can be found at the [UnaNancyOwen/Kinect2Sample repository]
(https://github.com/UnaNancyOwen/Kinect2Sample/blob/master/HOWTOBUILD.md).

Sample apps based on the code from above repository are licensed under the
[MIT License](https://github.com/UnaNancyOwen/Kinect2Sample/blob/master/LICENSE).

## Get Visual Studio

Pre Install
VisualStudio2013 / 2015
-> e. g. from Microsoft Imagine:
https://e5.onthehub.com/WebStore/OfferingDetails.aspx?o=631da0e7-2038-e311-93f6-b8ca3a5db7a1&pmv=00000000-0000-0000-0000-000000000000&ws=77000217-8b6f-e011-971f-0030487d8897&vsro=8

**Make sure to install (enable) Visual C++ 2015 Tools for Windows Desktop needed for Qt GUI projects!** 

## Get the opensource (LGPL) Qt5 library

Weiter verwenden wir Qt5 als Bibliothek für die GUI Erstellung. Diese bekommt Ihr unter
folgender Adresse:

* Qt 5.6.0 (for VS 2013, Win 32 bit)
-> http://download.qt.io/official_releases/qt/5.6/5.6.0/qt-opensource-windows-x86-msvc2013-5.6.0.exe
* Qt 5.7.1 (for VS 2015, Win 64 bit)
-> http://download.qt.io/official_releases/qt/5.7/5.7.1/qt-opensource-windows-x86-msvc2015_64-5.7.1.exe

More options can be found at [the "Download Qt Opensource" page](https://www.qt.io/download-open-source/)

## Get the OpenCV libraries for Image Processing

Zur vollständigen Installation von OpenCV Bibliotheken und ihre Einbindung in VS
wird die Durchführung der entsprechenden Schritte unter dem folgenden Link empfohlen

* How to Install OpenCV 2.4.11 in Visual Studio C++ 2013
-> https://marcomuraresearch.wordpress.com/2015/04/16/install-opencv-visual-studio/

## Get the Visual Studio integration

Zur Integration in die Entwicklungsumgebung Visual Studio brauchen wir dann noch
folgendes Programmpaket:

* Qt VS Add-in 1.2.4 (for VS 2013) -> http://download.qt.io/official_releases/vsaddin/qt-vs-addin-1.2.4-opensource.exe
* Qt VS Add-in 2.0.0 (for VS 2015) -> http://download.qt.io/official_releases/vsaddin/qt-vs-tools-msvc2015-2.0.0.vsix

## Bind the installed library into Visual Studio

1. In Visual Studio, navigate Qt VS Tools -> Qt Options
2. click on "Add", name can be arbitrary, look for a folder like `C:\Qt\Qt<YOUR_QT_VERSION>\<YOUR_QT_VERSION>\msvc<YOUR_VS_VERSION>`, where you can find the required `qmake.exe`. If it doesn’t complain, click on OK
3. voilà there’s the new Qt entry, so click on OK and you should now be able to compile Qt stuff



Kinect Sensor v2.0 (for XBox One, with Windows adapter)

Get started by installing  Kinect SDK 2.0 (ships with Runtime 2.0) and following the Kinect for Windows Programming Guide. 
https://msdn.microsoft.com/en-us/library/dn782037.aspx

Hardware Requirements:
USB 3.0, preferably Intel or NEC host controller

Required drivers
We’ll use this: Kinect for Windows Runtime v2.0
https://www.microsoft.com/en-us/download/details.aspx?id=44559
runs on Windows 8, 8.1, 10 

IDE
We’ll use this: Kinect for Windows SDK v2.0
https://www.microsoft.com/en-us/download/details.aspx?id=44561
examples can be found at
https://github.com/UnaNancyOwen/Kinect2Sample


## Create a project

1. Datei -> Neu-> Vorlagen-> Visual Basic -> Qt5 Projekt -> QtApplication -> Check points ( Core, GUI, Widgets) -> Weiter -> Basis Klasse -> Qwidget auswählen 

## Linken

1.  Project Eigenschaften -> C/C++  -> Allgemein -> Zusätzliche Includeverzeichisse ->Bearbeiten -> 
    In neue Line Pfard zu header hinzufügen
    `C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\inc`
    
2.  Project Eigenschaften -> Linker -> Allgemein -> Zusätzliche  Bibliothekverzeichnisse -> Bearbeiten- >
    In neue Line Pfard zu *.lib hinzufügen
    
    `C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\Lib\x86`
    
    ( `$(QTDIR)\lib`  muss schon vorhandeln sein)
   
3.  Project Eigenschaften -> Linker -> Eingabe-> Zusätzliche  Abhängigkeiten-> Bearbeiten- >
    `kinect20.lib`
    
    ( Müssen shon da sein 
    `qtmaind.lib`
    `Qt5Cored.lib`
    `Qt5Guid.lib`
    `Qt5Widgetsd.lib`) 
4.  In header 
    
    ```cpp
#include <Kinect.h>
   ```
    
    Mehr Beispile hier:
    https://github.com/UnaNancyOwen/Kinect2Sample


## Testqt.h

```cpp
#ifndef TESTQT_H
#define TESTQT_H

#include <QtWidgets/QWidget>
#include "ui_testqt.h"
#include <Kinect.h>

class TestQT : public QWidget
{
	Q_OBJECT

public:
	TestQT(QWidget *parent = 0);
	~TestQT();

private:
	// Current Kinect
	IKinectSensor*          m_pKinectSensor;
	ICoordinateMapper*      m_pCoordinateMapper;

	// Body reader
	IBodyFrameReader*       m_pBodyFrameReader;

	Ui::TestQTClass ui;

	/// <summary>
	/// Initializes the default Kinect sensor
	/// </summary>
	/// <returns>S_OK on success, otherwise failure code</returns>
	HRESULT                 InitializeDefaultSensor();
};

#endif // TESTQT_H
```


## Testqt.cpp

```cpp
#include "testqt.h"
#include <iostream>
TestQT::TestQT(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	InitializeDefaultSensor();
}

TestQT::~TestQT()
{

}

// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

/// <summary>
/// Initializes the default Kinect sensor
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT TestQT::InitializeDefaultSensor()
{
	IKinectSensor* pSensor;
	HRESULT hResult = S_OK;
	hResult = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hResult)){
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
		return -1;
	}

	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		return hr;
	}

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the body reader
		IBodyFrameSource* pBodyFrameSource = NULL;

		hr = m_pKinectSensor->Open();  // EIGENTLICH WIRD DER SENSOR HIER  EINGESCHALTET

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
		}

		SafeRelease(pBodyFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		//SetStatusMessage(L"No ready Kinect found!", 10000, true);
		return E_FAIL;
	}

	return hr;
}
```

## Main

```cpp
#include "testqt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TestQT w;
	w.show();
	return a.exec();
}
```