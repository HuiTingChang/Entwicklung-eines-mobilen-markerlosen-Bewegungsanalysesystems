#include "trial.h"
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

trial::trial(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	cv::setUseOptimized(true);

	InitializeDefaultSensor();
	initializeColor();

	// Wait a Few Seconds until begins to Retrieve Data from Sensor ( about 2000-[ms] )
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

trial::~trial()
{
	// Finalize
	finalize();
}

// Safe release for interfaces
//template<class Interface>
template<class T>
inline void SafeRelease(T *& pInterfaceToRelease)
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
HRESULT trial::InitializeDefaultSensor()
{
	//IKinectSensor* pSensor=nullptr;
	//HRESULT hResult = E_FAIL;
	//hResult = GetDefaultKinectSensor(&pSensor);
	//if (FAILED(hResult)){
	//	std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
	//	return -1;
	//}

	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
		return hr;
	}

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the body reader
		IBodyFrameSource* pBodyFrameSource = NULL;

		// Open Sensor
		hr = m_pKinectSensor->Open();  // EIGENTLICH WIRD DER SENSOR HIER EINGESCHALTET

		// Check Open
		BOOLEAN isOpen = FALSE;
		hr = m_pKinectSensor->get_IsOpen(&isOpen);
		
		if (FAILED(hr) || !isOpen)
		{
			std::cerr << "failed IKinectSensor::get_IsOpen( &isOpen )" << std::endl;
			return hr;
		}

		// Retrieve Coordinate Mapper
		hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);

		if (FAILED(hr))
		{
			std::cerr << "failed IKinectSensor::get_CoordinateMapper(&m_pCoordinateMapper)" << std::endl;
			return hr;
		}

		// Initialize image color
		initializeColor();

		// Open Body Reader
		hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);

		if (FAILED(hr))
		{
			std::cerr << "failed IKinectSensor::get_BodyFrameSource(&pBodyFrameSource)" << std::endl;
			return hr;
		}

		hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);

		if (FAILED(hr))
		{
			std::cerr << "failed IKinectSensor::OpenReader(&m_pBodyFrameReader)" << std::endl;
			return hr;
		}

		// Initialize Body Buffer
		Concurrency::parallel_for_each(bodies.begin(), bodies.end(), [](IBody*& body)
		{
			SafeRelease(body);
		});

		//SafeRelease(pBodyFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		//SetStatusMessage(L"No ready Kinect found!", 10000, true);
		return E_FAIL;
	}

	// Color Table for Visualization
	colors[0] = cv::Vec3b(255, 0, 0); // Blue
	colors[1] = cv::Vec3b(0, 255, 0); // Green
	colors[2] = cv::Vec3b(0, 0, 255); // Red
	colors[3] = cv::Vec3b(255, 255, 0); // Cyan
	colors[4] = cv::Vec3b(255, 0, 255); // Magenta
	colors[5] = cv::Vec3b(0, 255, 255); // Yellow

	return hr;
}

// Initialize Color
inline void trial::initializeColor()
{
	// Open Color Reader
	IColorFrameSource* colorFrameSource;
	m_pKinectSensor->get_ColorFrameSource(&colorFrameSource);
	colorFrameSource->OpenReader(&m_pColorFrameReader);

	// Retrieve Color Description
	IFrameDescription* colorFrameDescription;
	colorFrameSource->CreateFrameDescription(ColorImageFormat::ColorImageFormat_Bgra, &colorFrameDescription);
	colorFrameDescription->get_Width(&colorWidth); // 1920
	colorFrameDescription->get_Height(&colorHeight); // 1080
	colorFrameDescription->get_BytesPerPixel(&colorBytesPerPixel); // 4

	// Allocation Color Buffer
	colorBuffer.resize(colorWidth * colorHeight * colorBytesPerPixel);
}

// Processing
void trial::run()
{
	// Main Loop
	while (true)
	{
		// Update color
		updateColor();

		// Update Body Data
		// https://msdn.microsoft.com/de-de/library/dn799273.aspx
		updateBody();

		// Draw Body Data
		drawColor();
		drawBody();

		// Show Data
		showBody();

		// Key Check
		const int key = cv::waitKey(10);
		if (key == VK_ESCAPE){
			break;
		}
	}
}

// Update Color
inline void trial::updateColor()
{
	// Retrieve Color Frame
	IColorFrame* colorFrame;
	const HRESULT ret = m_pColorFrameReader->AcquireLatestFrame(&colorFrame);
	if (FAILED(ret))
	{
		return;
	}

	// Convert Format ( YUY2 -> BGRA )
	colorFrame->CopyConvertedFrameDataToArray(static_cast<UINT>(colorBuffer.size()), &colorBuffer[0], ColorImageFormat::ColorImageFormat_Bgra);
}

// Update Body
inline void trial::updateBody()
{
	// Retrieve Body Frame
	IBodyFrame* bodyFrame;
	const HRESULT ret = m_pBodyFrameReader->AcquireLatestFrame(&bodyFrame);
	if (FAILED(ret))
	{
		std::cerr << "failed IBodyFrameReader::AcquireLatestFrame(&bodyFrame)" << std::endl;
		return;
	}

	// Release Previous Bodies and array becomes empty
	Concurrency::parallel_for_each(bodies.begin(), bodies.end(), [](IBody*& body){
		SafeRelease(body);
	});

	// Retrieve Body Data
	if (bodyFrame!=nullptr)
		bodyFrame->GetAndRefreshBodyData(static_cast<UINT>(bodies.size()), &bodies[0]);
}

//Draw Color
inline void trial::drawColor()
{
	// Create cv::Mat from Color Buffer
	colorMat = cv::Mat(colorHeight, colorWidth, CV_8UC4, &colorBuffer[0]);
}

// Draw Body
inline void trial::drawBody()
{
	HRESULT hr;

	// Draw Body Data to Color Data
	Concurrency::parallel_for(0, BODY_COUNT, [&](const int count)
	{
		IBody* body = bodies[count];
		if (body == nullptr)
		{
			return;
		}


		// Check Body Tracked
		BOOLEAN tracked = FALSE;
		body->get_IsTracked(&tracked);
		
		if (!tracked)
		{
			return;
		}

		// Retrieve Joints
		std::array<Joint, JointType::JointType_Count> joints;
		hr = body->GetJoints(static_cast<UINT>(joints.size()), &joints[0]);
		if (!SUCCEEDED(hr))
		{
			return;
		}

		Concurrency::parallel_for_each(joints.begin(), joints.end(), [&](const Joint& joint)
		{
			// Check Joint Tracked
			if (joint.TrackingState == TrackingState::TrackingState_NotTracked){
				return;
			}

			// Draw Joint Position
			drawEllipse(colorMat, joint, 5, colors[count]);

			// Draw Left Hand State
			if (joint.JointType == JointType::JointType_HandLeft)
			{
				HandState handState;
				TrackingConfidence handConfidence;
				hr = body->get_HandLeftState(&handState);
				if (!SUCCEEDED(hr))
				{
					return;
				}
				hr = body->get_HandLeftConfidence(&handConfidence);
				if (!SUCCEEDED(hr))
				{
					return;
				}
				drawHandState(colorMat, joint, handState, handConfidence);
			}

			// Draw Right Hand State
			if (joint.JointType == JointType::JointType_HandRight)
			{
				HandState handState;
				TrackingConfidence handConfidence;
				hr = body->get_HandRightState(&handState);
				if (!SUCCEEDED(hr))
				{
					return;
				}
				hr = body->get_HandRightConfidence(&handConfidence);
				if (!SUCCEEDED(hr))
				{
					return;
				}
				drawHandState(colorMat, joint, handState, handConfidence);
			}
		});
	});
}

// Draw Hand State
inline void trial::drawHandState(cv::Mat& image, const Joint& joint, HandState handState, TrackingConfidence handConfidence)
{
	if (image.empty())
	{
		return;
	}

	// Check Tracking Confidence
	if (handConfidence != TrackingConfidence::TrackingConfidence_High)
	{
		return;
	}

	// Draw Hand State 
	const int radius = 75;
	const cv::Vec3b blue = cv::Vec3b(128, 0, 0), green = cv::Vec3b(0, 128, 0), red = cv::Vec3b(0, 0, 128);
	switch (handState)
	{
		// Open
		case HandState::HandState_Open:
			drawEllipse(image, joint, radius, green, 5);
			break;
		// Close
		case HandState::HandState_Closed:
			drawEllipse(image, joint, radius, red, 5);
			break;
		// Lasso
		case HandState::HandState_Lasso:
			drawEllipse(image, joint, radius, blue, 5);
			break;
		default:
			break;
	}
}

// Draw Ellipse
inline void trial::drawEllipse(cv::Mat& image, const Joint& joint, const int radius, const cv::Vec3b& color, const int thickness)
{
	if (image.empty())
	{
		return;
	}

	// Convert Coordinate System and Draw Joint
	ColorSpacePoint colorSpacePoint;
	m_pCoordinateMapper->MapCameraPointToColorSpace(joint.Position, &colorSpacePoint);
	const int x = static_cast<int>(colorSpacePoint.X + 0.5f);
	const int y = static_cast<int>(colorSpacePoint.Y + 0.5f);
	if ((0 <= x) && (x < image.cols) && (0 <= y) && (y < image.rows)){
		cv::circle(image, cv::Point(x, y), radius, static_cast<cv::Scalar>(color), thickness, CV_AA); // antialiased
	}
}


// Show Body
inline void trial::showBody()
{
	if (colorMat.empty()){
		return;
	}

	// Resize Image
	cv::Mat resizeMat;
	const double scale = 0.5;
	cv::resize(colorMat, resizeMat, cv::Size(), scale, scale);

	// Show Image
	cv::imshow("Body", resizeMat);
}


// Finalize
void trial::finalize()
{
	cv::destroyAllWindows();

	// Release Body Buffer
	Concurrency::parallel_for_each(bodies.begin(), bodies.end(), [](IBody*& body){
		SafeRelease(body);
	});

	// Close Sensor
	if (m_pKinectSensor != nullptr)
	{
		m_pKinectSensor->Close();
	}
}
