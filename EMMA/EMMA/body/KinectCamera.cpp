#include "KinectCamera.h"
#include "util.h"

#include <thread>
#include <chrono>
#include <QtConcurrent>



// Constructor
KinectCamera::KinectCamera():
    CvCamera()
{
	bodies = { { nullptr } };
	// Initialize
    //initialize();
}

// Destructor
KinectCamera::~KinectCamera()
{
    // Finalize
    finalize();
}

// Processing
CameraData KinectCamera::run()
{
        // Update Data
        update();

        // Draw Data
        draw();

		// test joint points for calculating the angle of right knee
		// JointType j1 = JointType_HipRight;
		// JointType j2 = JointType_KneeRight;
		// JointType j3 = JointType_AnkleRight;
		// Angle calc
		//	calcAngle(j1, j2, j3, angle);

		// Send Data
	CameraData result;
	result.joints = JointPositions(jposition);
	result.jorientations = JointOrientations(jorientation);
	result.frame = retrieveFrame();

	return result;
}

/** Initialize
 *  @throws camera_not_found_error (see initializeSensor)
 */
int KinectCamera::initialize()
{
    cv::setUseOptimized( true );
	

    // Initialize Sensor
    initializeSensor()

    // Initialize Color
    initializeColor();
	
    // Initialize Body
    initializeBody();
   
    // Wait a Few Seconds until begins to Retrieve Data from Sensor ( about 2000-[ms] )
    std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
	
	return 0;
}

// Initialize Sensor
inline void KinectCamera::initializeSensor()
{
    // Open Sensor
    if( GetDefaultKinectSensor( &kinect ) != S_OK )
    {
        throw camera_not_found_error();
    }

    if(kinect->Open() != S_OK)
    {
        // Check Availability
        BOOLEAN isAvailable = FALSE;
        ERROR_CHECK(body->get_IsTracked( &isAvailable ) );
        if( !isAvailable )
        {
            throw camera_not_available_error();
        }
	// Check Existence of an open stream
	BOOLEAN isOpen = FALSE;
	ERROR_CHECK( kinect->get_IsOpen( &isOpen ) );
	if( !isOpen )
        {
	    throw camera_inactive_error();
	}
    }

    // Retrieve Coordinate Mapper
    ERROR_CHECK( kinect->get_CoordinateMapper( &coordinateMapper ) );
}

// Initialize Color
inline void KinectCamera::initializeColor()
{
    // Open Color Reader
    ComPtr<IColorFrameSource> colorFrameSource;
    ERROR_CHECK( kinect->get_ColorFrameSource( &colorFrameSource ) );
    ERROR_CHECK( colorFrameSource->OpenReader( &colorFrameReader ) );

    // Retrieve Color Description
    ComPtr<IFrameDescription> colorFrameDescription;
    ERROR_CHECK( colorFrameSource->CreateFrameDescription( ColorImageFormat::ColorImageFormat_Bgra, &colorFrameDescription ) );
    ERROR_CHECK( colorFrameDescription->get_Width( &colorWidth ) ); // 1920
    ERROR_CHECK( colorFrameDescription->get_Height( &colorHeight ) ); // 1080
    ERROR_CHECK( colorFrameDescription->get_BytesPerPixel( &colorBytesPerPixel ) ); // 4

    // Allocation Color Buffer
    colorBuffer.resize( colorWidth * colorHeight * colorBytesPerPixel );
}

// Initialize Body
inline void KinectCamera::initializeBody()
{
    // Open Body Reader
    ComPtr<IBodyFrameSource> bodyFrameSource;
    ERROR_CHECK( kinect->get_BodyFrameSource( &bodyFrameSource ) );
    ERROR_CHECK( bodyFrameSource->OpenReader( &bodyFrameReader ) );

    // Initialize Body Buffer
    QtConcurrent::blockingMap(bodies.begin(), bodies.end(), []( IBody*& body ){
        SafeRelease( body );
    } );

    // Color Table for Visualization
    colors[0] = cv::Vec3b( 255,   0,   0 ); // Blue
    colors[1] = cv::Vec3b(   0, 255,   0 ); // Green
    colors[2] = cv::Vec3b(   0,   0, 255 ); // Red
    colors[3] = cv::Vec3b( 255, 255,   0 ); // Cyan
    colors[4] = cv::Vec3b( 255,   0, 255 ); // Magenta
    colors[5] = cv::Vec3b(   0, 255, 255 ); // Yellow
}

// Finalize
void KinectCamera::finalize()
{
    cv::destroyAllWindows();

    // Release Body Buffer
    QtConcurrent::blockingMap( bodies.begin(), bodies.end(), []( IBody*& body ){
        SafeRelease( body );
    } );

    // Close Sensor
    if( kinect != nullptr ){
        kinect->Close();
    }
}

// Update Data
void KinectCamera::update()
{
    // Update Color
    updateColor();

    // Update Body
    updateBody();
}

// Update Color
inline void KinectCamera::updateColor()
{
    // Retrieve Color Frame
    ComPtr<IColorFrame> colorFrame;
    const HRESULT ret = colorFrameReader->AcquireLatestFrame( &colorFrame );
    if( FAILED( ret ) ){
        return;
    }

    // Convert Format ( YUY2 -> BGRA )
    ERROR_CHECK( colorFrame->CopyConvertedFrameDataToArray( static_cast<UINT>( colorBuffer.size() ), &colorBuffer[0], ColorImageFormat::ColorImageFormat_Bgra ) );
}

// Update Body
inline void KinectCamera::updateBody()
{
    // Retrieve Body Frame
    ComPtr<IBodyFrame> bodyFrame;
    const HRESULT ret = bodyFrameReader->AcquireLatestFrame( &bodyFrame );
    if( FAILED( ret ) ){
        return;
    }

    // Release Previous Bodies
    QtConcurrent::blockingMap( bodies.begin(), bodies.end(), []( IBody*& body ){
        SafeRelease( body );
    } );

    // Retrieve Body Data
    ERROR_CHECK( bodyFrame->GetAndRefreshBodyData( static_cast<UINT>( bodies.size() ), &bodies[0] ) );
}

// Draw Data
void KinectCamera::draw()
{
    // Draw Color
    drawColor();

    // Draw Body
    drawBody();
}

// Draw Color
inline void KinectCamera::drawColor()
{
    // Create cv::Mat from Color Buffer
    colorMat = cv::Mat( colorHeight, colorWidth, CV_8UC4, &colorBuffer[0] );
}

// Draw Body
inline void KinectCamera::drawBody()
{
	std::array<int,BODY_COUNT> bodyrange;
	std::list<uint> jointNotTracked;
	
	for (int i = 0; i < BODY_COUNT; i++)
	{
		bodyrange[i] = i;
	}
	// Draw Body Data to Color Data
    QtConcurrent::blockingMap( bodyrange.begin(), bodyrange.end(), [&]( const int count ){
        const ComPtr<IBody> body = bodies[count];
        if( body == nullptr ){
            return;
        }

        // Check Body Tracked
        BOOLEAN tracked = FALSE;
        ERROR_CHECK( body->get_IsTracked( &tracked ) );
        if( !tracked ){
            return;
        }

        // Retrieve Joints
        std::array<Joint, JointType::JointType_Count> joints;
        ERROR_CHECK( body->GetJoints( static_cast<UINT>( joints.size() ), &joints[0] ) );

        QtConcurrent::blockingMap( joints.begin(), joints.end(), [&]( const Joint& joint ){
            // Check Joint Tracked
            if( joint.TrackingState == TrackingState::TrackingState_NotTracked ){
				jointNotTracked.push_back(joint.JointType);
                return;
            }

			// Save Joint Position
			QVector3D j(
					joint.Position.X,
					joint.Position.Y,
					joint.Position.Z
					);
			
			jposition[joint.JointType] = j;

            // Draw Joint Position
            drawEllipse( colorMat, joint, 5, colors[count] );

            // Draw Left Hand State
            if( joint.JointType == JointType::JointType_HandLeft ){
                HandState handState;
                TrackingConfidence handConfidence;
                ERROR_CHECK( body->get_HandLeftState( &handState ) );
                ERROR_CHECK( body->get_HandLeftConfidence( &handConfidence ) );

                drawHandState( colorMat, joint, handState, handConfidence );
            }

            // Draw Right Hand State
            if( joint.JointType == JointType::JointType_HandRight ){
                HandState handState;
                TrackingConfidence handConfidence;
                ERROR_CHECK( body->get_HandRightState( &handState ) );
                ERROR_CHECK( body->get_HandRightConfidence( &handConfidence ) );

                drawHandState( colorMat, joint, handState, handConfidence );
            }
        } );

        
        // Retrieve Joint Orientations
        std::array<JointOrientation, JointType::JointType_Count> orientations;
        ERROR_CHECK( body->GetJointOrientations( JointType::JointType_Count, &orientations[0] ) );
        
		QtConcurrent::blockingMap(orientations.begin(), orientations.end(), [&](const JointOrientation& orientation){
			
			// Check Joint Tracked
			for (std::list<uint>::iterator it = jointNotTracked.begin(); it != jointNotTracked.end(); ++it)
			{
				if (*it == orientation.JointType)
					return;
			}
			
			QVector4D orient(
				orientation.Orientation.x,
				orientation.Orientation.y,
				orientation.Orientation.z,
				orientation.Orientation.w
			);

			jorientation[orientation.JointType] = orient;
		});

        // Retrieve Amount of Body Lean
        /*PointF amount;
        ERROR_CHECK( body->get_Lean( &amount ) );*/

		// Draw Skeleton
		drawSkeleton(colorMat, body, colors[count]);
    } );



}

// Draw Ellipse
inline void KinectCamera::drawEllipse( cv::Mat& image, const Joint& joint, const int radius, const cv::Vec3b& color, const int thickness )
{
    if( image.empty() ){
        return;
    }

    // Convert Coordinate System and Draw Joint
    ColorSpacePoint colorSpacePoint;
    ERROR_CHECK( coordinateMapper->MapCameraPointToColorSpace( joint.Position, &colorSpacePoint ) );
    const int x = static_cast<int>( colorSpacePoint.X + 0.5f );
    const int y = static_cast<int>( colorSpacePoint.Y + 0.5f );
    if( ( 0 <= x ) && ( x < image.cols ) && ( 0 <= y ) && ( y < image.rows-10 ) ){
        cv::circle( image, cv::Point( x, y ), radius, static_cast<cv::Scalar>( color ), thickness, cv::LINE_AA );
    }
}

// Draw Hand State
inline void KinectCamera::drawHandState( cv::Mat& image, const Joint& joint, HandState handState, TrackingConfidence handConfidence )
{
    if( image.empty() ){
        return;
    }

    // Check Tracking Confidence
    if( handConfidence != TrackingConfidence::TrackingConfidence_High ){
        return;
    }

    // Draw Hand State 
    const int radius = 75;
    const cv::Vec3b blue = cv::Vec3b( 128, 0, 0 ), green = cv::Vec3b( 0, 128, 0 ), red = cv::Vec3b( 0, 0, 128 );
    switch( handState ){
        // Open
        case HandState::HandState_Open:
            drawEllipse( image, joint, radius, green, 5 );
            break;
        // Close
        case HandState::HandState_Closed:
            drawEllipse( image, joint, radius, red, 5 );
            break;
        // Lasso
        case HandState::HandState_Lasso:
            drawEllipse( image, joint, radius, blue, 5 );
            break;
        default:
            break;
    }
}

// Draw Skeleton
inline void KinectCamera::drawSkeleton(cv::Mat& image, const ComPtr<IBody> body, const cv::Vec3b& color, const int thickness)
{
	// Check Body Tracked
	BOOLEAN tracked = FALSE;
	ERROR_CHECK(body->get_IsTracked(&tracked));
	if (!tracked){
		return;
	}

	// Retrieve Joints
	std::array<Joint, JointType::JointType_Count> joints;
	ERROR_CHECK(body->GetJoints(static_cast<UINT>(joints.size()), &joints[0]));

	drawLine(image, joints[JointType_Head], joints[JointType_Neck], color);
	drawLine(image, joints[JointType_Neck], joints[JointType_SpineShoulder], color);
	drawLine(image, joints[JointType_SpineShoulder], joints[JointType_ShoulderLeft], color);
	drawLine(image, joints[JointType_SpineShoulder], joints[JointType_ShoulderRight], color);
	drawLine(image, joints[JointType_SpineShoulder], joints[JointType_SpineMid], color);
	drawLine(image, joints[JointType_ShoulderLeft], joints[JointType_ElbowLeft], color);
	drawLine(image, joints[JointType_ShoulderRight], joints[JointType_ElbowRight], color);
	drawLine(image, joints[JointType_ElbowLeft], joints[JointType_WristLeft], color);
	drawLine(image, joints[JointType_ElbowRight], joints[JointType_WristRight], color);
	drawLine(image, joints[JointType_WristLeft], joints[JointType_HandLeft], color);
	drawLine(image, joints[JointType_WristRight], joints[JointType_HandRight], color);
	drawLine(image, joints[JointType_HandLeft], joints[JointType_HandTipLeft], color);
	drawLine(image, joints[JointType_HandRight], joints[JointType_HandTipRight], color);
	drawLine(image, joints[JointType_HandTipLeft], joints[JointType_ThumbLeft], color);
	drawLine(image, joints[JointType_HandTipRight], joints[JointType_ThumbRight], color);
	drawLine(image, joints[JointType_SpineMid], joints[JointType_SpineBase], color);
	drawLine(image, joints[JointType_SpineBase], joints[JointType_HipLeft], color);
	drawLine(image, joints[JointType_SpineBase], joints[JointType_HipRight], color);
	drawLine(image, joints[JointType_HipLeft], joints[JointType_KneeLeft], color);
	drawLine(image, joints[JointType_HipRight], joints[JointType_KneeRight], color);
	drawLine(image, joints[JointType_KneeLeft], joints[JointType_AnkleLeft], color);
	drawLine(image, joints[JointType_KneeRight], joints[JointType_AnkleRight], color);
	drawLine(image, joints[JointType_AnkleLeft], joints[JointType_FootLeft], color);
	drawLine(image, joints[JointType_AnkleRight], joints[JointType_FootRight], color);

}

inline void KinectCamera::drawLine(cv::Mat& image, const Joint& joint1, const Joint& joint2, const cv::Vec3b& color, const int thickness)
{
	// Convert Coordinate System and Draw Joint
	ColorSpacePoint colorSpacePoint;
	HRESULT hr=coordinateMapper->MapCameraPointToColorSpace(joint1.Position, &colorSpacePoint);
	if (FAILED(hr))
	{
		return;
	}
	const int x1 = static_cast<int>(colorSpacePoint.X + 0.5f);
	const int y1 = static_cast<int>(colorSpacePoint.Y + 0.5f);

	hr = coordinateMapper->MapCameraPointToColorSpace(joint2.Position, &colorSpacePoint);
	if (FAILED(hr))
	{
		return;
	}
	const int x2 = static_cast<int>(colorSpacePoint.X + 0.5f);
	const int y2 = static_cast<int>(colorSpacePoint.Y + 0.5f);

	if ((0 <= x1) && (x1 < image.cols) && (0 <= y1) && (y1 < image.rows - 10) &&
		(0 <= x2) && (x2 < image.cols) && (0 <= y2) && (y2 < image.rows - 10)){
		//cv::circle(image, cv::Point(x, y), radius, static_cast<cv::Scalar>(color), thickness, cv::LINE_AA);
		cv::line(image, cv::Point(x1, y1), cv::Point(x2, y2), static_cast<cv::Scalar>(color), 2, cv::LINE_AA);
	}
}

// 
// Show Data
Mat KinectCamera::retrieveFrame()
{
    // Show Body
    return showBody();
}

// Show Body
inline Mat KinectCamera::showBody()
{
    // Resize Image
    cv::Mat resizeMat;
    const double scale = 0.5;
    cv::resize(colorMat, resizeMat, cv::Size(), scale, scale);

 	// Return image
	return resizeMat;
}


//void KinectCamera::calcAngle( JointType j1,  JointType j2,  JointType j3, double* angle)
//{
	//QVector3D v1_XYZ ( (jposition[j1].X - jposition[j2].X), (jposition[j1].Y - jposition[j2].Y), (jposition[j1].Z - jposition[j2].Z)) ;
	//QVector3D v2_XYZ ( (jposition[j3].X - jposition[j2].X), (jposition[j3].Y - jposition[j2].Y), (jposition[j3].Z - jposition[j2].Z) );
//	double inner_product = calcInnerProduct(v1_XYZ, v2_XYZ);
	//double v1_length = calcVectorLength(v1_XYZ);
	//double v2_length = calcVectorLength(v2_XYZ);
	//angle=
//}

//double calcInnerProduct(const QVector3D  v1, const QVector3D v2){
	//return v1.x() * v2.x() + v1.y() * v2.y() + v1.z()* v2.z();
//}
/*
double calcVectorLength(const QVector3D v){
	return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[3], 2));
}

*/
