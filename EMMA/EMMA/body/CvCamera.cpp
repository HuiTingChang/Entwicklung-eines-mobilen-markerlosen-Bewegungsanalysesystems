#include <QtConcurrent>

#include "CvCamera.h"
using namespace std;

const QMap<CvCamera::State,QString> CvCamera::STATE_DESCRIPTION
{
    {State::CONNECTED, "connected"},
    {State::DISCONNECTED, "disconnected"}
};

CvCamera::CvCamera(int destinationWidth, int destinationHeight):
    destinationSize(destinationWidth, destinationHeight)
{
}

shared_ptr<cv::Mat> CvCamera::get_blank_mat()
{
    return shared_ptr<cv::Mat>(new cv::Mat(destinationSize, cvChannelType));
}

CameraData CvCamera::run()
{
    cv::Mat frame(*get_blank_mat());
    // Resize Image
    //Mat frame = cv::imread("C:/SWE 16_17/Gruppe-13/Kinect_Body_Code/Body_Widget/penguin2.jpg");
    const cv::Scalar color(127,255,0,255);  // BGRA
    cv::Mat f_tmp(colorHeight, colorWidth, cvChannelType, color);
    cv::putText(
                f_tmp, "EMMA", cv::Point(colorWidth/6,colorHeight/2),
                cv::FONT_HERSHEY_SIMPLEX, 13, cv::Scalar::all(255), 26);
    cv::resize(f_tmp, frame, destinationSize);

    // Save Joint Position
    JointPositions j_tmp;
    for(unsigned int i=0; i < EMMA::JOINTS_COUNT; ++i)
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
    JointOrientations jo_tmp;  // XXX
    CameraData result(frame, j_tmp, jo_tmp);

    return result;
}

void CvCamera::initialize()
{
}
