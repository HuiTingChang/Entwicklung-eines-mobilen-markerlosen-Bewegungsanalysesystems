#ifndef __CAMERA__
#define __CAMERA__

#include <Windows.h>
#include <Kinect.h>
#include <opencv2/opencv.hpp>

#include <vector>
#include <array>

#include <wrl/client.h>
using namespace Microsoft::WRL;
using namespace cv;

class Kinect
{
private:
    // Sensor
    ComPtr<IKinectSensor> kinect;

    // Coordinate Mapper
    ComPtr<ICoordinateMapper> coordinateMapper;

    // Reader
    ComPtr<IColorFrameReader> colorFrameReader;
    ComPtr<IBodyFrameReader> bodyFrameReader;

    // Color Buffer
    std::vector<BYTE> colorBuffer;
    int colorWidth;
    int colorHeight;
    unsigned int colorBytesPerPixel;
    cv::Mat colorMat;

    // Body Buffer
	std::array<IBody*, BODY_COUNT> bodies = { { nullptr } };
    std::array<cv::Vec3b, BODY_COUNT> colors;

	// Joint Positions
	std::map<UINT, CameraSpacePoint> jposition;

	
public:
    // Constructor
    Kinect();

    // Destructor
    ~Kinect();

    // Processing
    Mat run();

private:
    // Initialize
    void initialize();

    // Initialize Sensor
    inline void initializeSensor();

    // Initialize Color
    inline void initializeColor();

    // Initialize Body
    inline void initializeBody();

    // Finalize
    void finalize();

    // Update Data
    void update();

    // Update Color
    inline void updateColor();

    // Update Body
    inline void updateBody();

    // Draw Data
    void draw();

    // Draw Color
    inline void drawColor();

    // Draw Body
    inline void drawBody();
	inline void drawSkeleton(cv::Mat& image, const ComPtr<IBody> body, const cv::Vec3b& color, const int thickness = -1);

    // Draw Circle
    inline void drawEllipse( cv::Mat& image, const Joint& joint, const int radius, const cv::Vec3b& color, const int thickness = -1 );

    // Draw Hand State
    inline void drawHandState( cv::Mat& image, const Joint& joint, HandState handState, TrackingConfidence handConfidence );

	// Draw lines between circles
	inline void Kinect::drawLine(cv::Mat& image, const Joint& joint1, const Joint& joint2, const cv::Vec3b& color, const int thickness=-1);
    
	// Show Data
    Mat show();

    // Show Body
    inline Mat showBody();
};

#endif // __CAMERA__