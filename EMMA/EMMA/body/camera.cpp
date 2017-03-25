#include "camera.h"
#include "util.h"

#include <thread>
#include <chrono>
#include <array>
#include "qvector3d.h"
#include <QtConcurrent>
#include <ppl.h>
#include <math.h>

// Constructor
Kinect::Kinect()
{
	bodies = { { nullptr } };
	// Initialize
    initialize();
}

// Destructor
Kinect::~Kinect()
{
    // Finalize
    finalize();
}

// Processing
Mat Kinect::run(double* angle)
{
    // Main Loop
//    while( true )
//	{
        // Update Data
        update();

        // Draw Data
        draw();

		// test joint points for calculating the angle of right knee
		JointType j1 = JointType_HipRight;
		JointType j2 = JointType_KneeRight;
		JointType j3 = JointType_AnkleRight;
		// Angle calc
	//	calcAngle(j1, j2, j3, angle);

        // Send Data
        return show();

 //   }
}

// Initialize
void Kinect::initialize()
{
    cv::setUseOptimized( true );

    // Initialize Sensor
    initializeSensor();

    // Initialize Color
    initializeColor();

    // Initialize Body
    initializeBody();

    // Wait a Few Seconds until begins to Retrieve Data from Sensor ( about 2000-[ms] )
    std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
}

// Initialize Sensor
inline void Kinect::initializeSensor()
{
    // Open Sensor
    ERROR_CHECK( GetDefaultKinectSensor( &kinect ) );

    ERROR_CHECK( kinect->Open() );

    // Check Open
    BOOLEAN isOpen = FALSE;
    ERROR_CHECK( kinect->get_IsOpen( &isOpen ) );
    if( !isOpen ){
        throw std::runtime_error( "failed IKinectSensor::get_IsOpen( &isOpen )" );
    }

    // Retrieve Coordinate Mapper
    ERROR_CHECK( kinect->get_CoordinateMapper( &coordinateMapper ) );
}

// Initialize Color
inline void Kinect::initializeColor()
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
inline void Kinect::initializeBody()
{
    // Open Body Reader
    ComPtr<IBodyFrameSource> bodyFrameSource;
    ERROR_CHECK( kinect->get_BodyFrameSource( &bodyFrameSource ) );
    ERROR_CHECK( bodyFrameSource->OpenReader( &bodyFrameReader ) );

    // Initialize Body Buffer
	QtConcurrent::map(bodies.begin(), bodies.end(), []( IBody*& body ){
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
void Kinect::finalize()
{
    cv::destroyAllWindows();

    // Release Body Buffer
	QtConcurrent::map( bodies.begin(), bodies.end(), []( IBody*& body ){
        SafeRelease( body );
    } );

    // Close Sensor
    if( kinect != nullptr ){
        kinect->Close();
    }
}

// Update Data
void Kinect::update()
{
    // Update Color
    updateColor();

    // Update Body
    updateBody();
}

// Update Color
inline void Kinect::updateColor()
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
inline void Kinect::updateBody()
{
    // Retrieve Body Frame
    ComPtr<IBodyFrame> bodyFrame;
    const HRESULT ret = bodyFrameReader->AcquireLatestFrame( &bodyFrame );
    if( FAILED( ret ) ){
        return;
    }

    // Release Previous Bodies
	QtConcurrent::map( bodies.begin(), bodies.end(), []( IBody*& body ){
        SafeRelease( body );
    } );

    // Retrieve Body Data
    ERROR_CHECK( bodyFrame->GetAndRefreshBodyData( static_cast<UINT>( bodies.size() ), &bodies[0] ) );
}

// Draw Data
void Kinect::draw()
{
    // Draw Color
    drawColor();

    // Draw Body
    drawBody();
}

// Draw Color
inline void Kinect::drawColor()
{
    // Create cv::Mat from Color Buffer
    colorMat = cv::Mat( colorHeight, colorWidth, CV_8UC4, &colorBuffer[0] );
}

// Draw Body
inline void Kinect::drawBody()
{
	std::array<int,BODY_COUNT> bodyrange;
	for (int i = 0; i < BODY_COUNT; i++)
	{
		bodyrange[i] = i;
	}
	// Draw Body Data to Color Data
	QtConcurrent::map( bodyrange.begin(), bodyrange.end(), [&]( const int count ){
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

		QtConcurrent::map( joints.begin(), joints.end(), [&]( const Joint& joint ){
            // Check Joint Tracked
            if( joint.TrackingState == TrackingState::TrackingState_NotTracked ){
                return;
            }

			// Save Joint Position
			jposition[joint.JointType] = joint.Position;

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

        /*
        // Retrieve Joint Orientations
        std::array<JointOrientation, JointType::JointType_Count> orientations;
        ERROR_CHECK( body->GetJointOrientations( JointType::JointType_Count, &orientations[0] ) );
        */

        
        // Retrieve Amount of Body Lean
        /*PointF amount;
        ERROR_CHECK( body->get_Lean( &amount ) );*/
        
		// Draw Skeleton
		drawSkeleton(colorMat, body, colors[count]);
    } );

	

}

// Draw Ellipse
inline void Kinect::drawEllipse( cv::Mat& image, const Joint& joint, const int radius, const cv::Vec3b& color, const int thickness )
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
inline void Kinect::drawHandState( cv::Mat& image, const Joint& joint, HandState handState, TrackingConfidence handConfidence )
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
inline void Kinect::drawSkeleton(cv::Mat& image, const ComPtr<IBody> body, const cv::Vec3b& color, const int thickness)
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

	drawLine(image, joints[3], joints[2], color);
	drawLine(image, joints[2], joints[20], color);
	drawLine(image, joints[20], joints[4], color);
	drawLine(image, joints[20], joints[8], color);
	drawLine(image, joints[20], joints[1], color);
	drawLine(image, joints[4], joints[5], color);
	drawLine(image, joints[8], joints[9], color);
	drawLine(image, joints[5], joints[6], color);
	drawLine(image, joints[9], joints[10], color);
	drawLine(image, joints[6], joints[7], color);
	drawLine(image, joints[10], joints[11], color);
	drawLine(image, joints[7], joints[21], color);
	drawLine(image, joints[11], joints[23], color);
	drawLine(image, joints[21], joints[22], color);
	drawLine(image, joints[23], joints[24], color);
	drawLine(image, joints[1], joints[0], color);
	drawLine(image, joints[0], joints[12], color);
	drawLine(image, joints[0], joints[16], color);
	drawLine(image, joints[12], joints[13], color);
	drawLine(image, joints[16], joints[17], color);
	drawLine(image, joints[13], joints[14], color);
	drawLine(image, joints[17], joints[18], color);
	drawLine(image, joints[14], joints[15], color);
	drawLine(image, joints[18], joints[19], color);
	
}

inline void Kinect::drawLine(cv::Mat& image, const Joint& joint1, const Joint& joint2, const cv::Vec3b& color, const int thickness)
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
Mat Kinect::show()
{
    // Show Body
    return showBody();
}

// Show Body
inline Mat Kinect::showBody()
{
 /*   if( colorMat.empty() ){
        return;
    }*/

    // Resize Image
	//Mat frame = cv::imread("C:/SWE 16_17/Gruppe-13/Kinect_Body_Code/Body_Widget/penguin2.jpg");
    cv::Mat resizeMat;
    const double scale = 0.5;
    //cv::resize( frame, resizeMat, cv::Size(), scale, scale );
	cv::resize(colorMat, resizeMat, cv::Size(), scale, scale);

    // Show Image
    //cv::imshow( "Body", resizeMat );

	// Return image
	return resizeMat;
}


//void Kinect::calcAngle( JointType j1,  JointType j2,  JointType j3, double* angle)
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
