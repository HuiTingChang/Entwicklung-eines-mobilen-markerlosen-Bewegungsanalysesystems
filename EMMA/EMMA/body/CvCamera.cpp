#include <QtConcurrent>

#include "CvCamera.h"
#include "Anatomy.h"
using namespace std;

static QMap<CvCamera::State, QString> create_map()
{
	QMap<CvCamera::State, QString>map;

	map.insert(CvCamera::State::CONNECTED, QString("connected"));
	map.insert(CvCamera::State::DISCONNECTED, QString("disconnected"));

	return map;
}
const QMap<CvCamera::State, QString> CvCamera::STATE_DESCRIPTION = create_map();


CvCamera::CvCamera(int destinationWidth, int destinationHeight):
    destinationSize(destinationWidth, destinationHeight),
    state(State::DISCONNECTED)
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
    SpacePoint meaningless_joint(
            13, // x
            14, // y
            15  // z
            );
    Quaternion meaningless_quaternion(
            1.2, // w
            1.3, // x
            1.4, // y
            1.5  // z
            );
    JointOrientations jo_tmp;
    for(unsigned int i=0; i<EMMA::JOINTS_COUNT; ++i)
    {
        // default-construct them
        jo_tmp[i];
    }
    auto p_task = QtConcurrent::map(
                j_tmp.begin(), j_tmp.end(),
                [meaningless_joint](
                SpacePoint& itemp
                ){
        itemp += meaningless_joint;
    });
    auto q_task = QtConcurrent::map(
                jo_tmp.begin(), jo_tmp.end(),
                [meaningless_quaternion](
                Quaternion& itemq
                ){
        itemq += meaningless_quaternion;
    });
    p_task.waitForFinished();
    q_task.waitForFinished();
    CameraData result(frame, j_tmp, jo_tmp);

    return result;
}

void CvCamera::initialize()
{
}

const CvCamera::State *CvCamera::get_state_ptr() const
{
    return &state;
}
