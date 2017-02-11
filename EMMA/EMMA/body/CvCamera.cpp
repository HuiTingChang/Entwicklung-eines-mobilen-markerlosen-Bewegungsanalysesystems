#include <QtConcurrent>

#include "CvCamera.h"
using namespace std;

CvCamera::CvCamera():
	colorMat(colorHeight, colorWidth, cvChannelType)
{

}

shared_ptr<cv::Mat> CvCamera::get_blank_mat()
{
    return shared_ptr<cv::Mat>(new cv::Mat(colorHeight, colorWidth, cvChannelType));
}

virtual CameraData CvCamera::run()
{
    CameraData result;
    // Resize Image
    //Mat frame = cv::imread("C:/SWE 16_17/Gruppe-13/Kinect_Body_Code/Body_Widget/penguin2.jpg");
    shared_ptr<cv::Mat> colorMat = get_blank_mat();
    const double scale = 0.5;
    cv::resize(*colorMat, result.frame, cv::Size(), scale, scale);

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
    result.joints = j_tmp;

    return result;
}
