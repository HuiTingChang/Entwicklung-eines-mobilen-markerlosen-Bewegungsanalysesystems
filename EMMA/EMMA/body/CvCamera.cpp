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
    for(unsigned int i=0; i < JOINTS_COUNT; ++i)
    {
	// default-construct them
	j_tmp[i];
    }
    QVector3D meaningless_joint(
            13, // x
            14, // y
            15  // z
            );
    QtConcurrent::blockingMap(j_tmp.begin(), j_tmp.end(), [meaningless_joint](QVector3D& item){
        item += meaningless_joint;
    });
    j = j_tmp;

    // Return image
    return resizeMat;
}
