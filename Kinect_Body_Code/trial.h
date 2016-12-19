#ifndef TRIAL_H
#define TRIAL_H

#include <QtWidgets/QMainWindow>
#include "ui_trial.h"
#include <Windows.h>
#include <Kinect.h>
#include <vector>
#include <array>
#include <iostream>
#include <ppl.h>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace concurrency;


class trial : public QMainWindow
{
	Q_OBJECT

public:
	trial(QWidget *parent = 0);
	~trial();

	// Processing
	void run();

private:

	// Current Kinect Sensor
	IKinectSensor*          m_pKinectSensor;
	
	// Current Coordinate Mapper
	ICoordinateMapper*      m_pCoordinateMapper;

	// Body reader
	IBodyFrameReader*       m_pBodyFrameReader;

	//Color reader
	IColorFrameReader*		m_pColorFrameReader;

	// Body Buffer
	std::array<IBody*, BODY_COUNT> bodies {{nullptr}};
	std::array<cv::Vec3b, BODY_COUNT> colors;

	// Image
	std::vector<BYTE> colorBuffer;
	cv::Mat colorMat;
	int colorWidth;
	int colorHeight;
	unsigned int colorBytesPerPixel;

	/// <summary>
	/// Initialize image color
	/// </summary>
	inline void initializeColor();

	/// <summary>
	/// Update image color
	/// </summary>
	inline void updateColor();

	/// <summary>
	/// Update the body data
	/// </summary>
	inline void updateBody();

	/// <summary>
	/// Show the body data
	/// </summary>
	inline void showBody();

	/// <summary>
	/// Draw Color
	/// </summary>
	inline void drawColor();

	/// <summary>
	/// Draw the body data
	/// </summary>
	inline void drawBody();

	/// <summary>
	// Draw Hand State
	/// </summary>
	inline void drawHandState(cv::Mat& image, const Joint& joint, HandState handState, TrackingConfidence handConfidence);


	/// <summary>
	// Draw circle
	/// </summary>
	inline void drawEllipse(cv::Mat& image, const Joint& joint, const int radius, const cv::Vec3b& color, const int thickness = -1);
	
	/// <summary>
	// Finalize
	/// </summary>
	inline void finalize();


	/// <summary>
	/// Initializes the default Kinect sensor
	/// </summary>
	/// <returns>S_OK on success, otherwise failure code</returns>
	HRESULT                 InitializeDefaultSensor();

	Ui::trialClass ui;
};

#endif // TRIAL_H
