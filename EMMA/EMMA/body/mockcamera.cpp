#include <vector>
#include <QtConcurrent>

#include "mockcamera.h"
using namespace std;

MockCamera::MockCamera():
	colorMat(colorHeight, colorWidth, cvChannelType)
{

}

cv::Mat MockCamera::run(JointPositions& j)
{
    // Resize Image
    //Mat frame = cv::imread("C:/SWE 16_17/Gruppe-13/Kinect_Body_Code/Body_Widget/penguin2.jpg");
    cv::Mat resizeMat;
    const double scale = 0.5;
    cv::resize(colorMat, resizeMat, cv::Size(), scale, scale);

    // Save Joint Position
    JointPositions j_tmp;
    std::vector<bool> jointsrange(JOINTS_COUNT);
    QtConcurrent::blockingMap(jointsrange.begin(), jointsrange.end(), [&j_tmp]( const bool i ){
            // meaningless
            QVector3D jp_i(
                    13, // x
                    14, // y
                    15  // z
                    );
            j_tmp[i] = jp_i;
    });
    j = j_tmp;

    // Return image
    return resizeMat;
}
