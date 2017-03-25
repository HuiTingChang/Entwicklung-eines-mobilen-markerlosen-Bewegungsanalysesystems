#include "CurrentState.h"

using namespace EMMA;

CurrentState::CurrentState():
	centOfGv(SpacePoint(0.0, 0.0, 0.0)),
	centOfPr(BoardPoint(0, 0)),
	gewicht(0.0)
{}

CurrentState::~CurrentState()
{}

bool CurrentState::operator==(const CurrentState& other) const
{
	return joints == other.joints;
}

void CurrentState::update()
{
	timestamp = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
}

QDateTime CurrentState::get_timestamp() const
{
	return QDateTime::fromMSecsSinceEpoch(timestamp, Qt::UTC);
}

float CurrentState::get_gewicht() const
{
	return gewicht;
}

BoardPoint CurrentState::get_centOfPr() const
{
	return centOfPr;
}

SpacePoint CurrentState::get_centOfGv() const
{
	return centOfGv;
}

JointPositions CurrentState::get_joints() const
{
	return joints;
}

JointOrientations CurrentState::get_angles() const
{
	return angles;
}

void CurrentState::set_centOfPr(BoardPoint cop)
{
	centOfPr = cop;
}

void CurrentState::set_centOfGv()
{
	centOfGv = centerOfGravityMeasurement();
}

void CurrentState::set_gewicht(float g)
{
	gewicht = g;
}

void CurrentState::set_jointPositions(JointPositions j)
{
	joints = j;
}

void CurrentState::set_angles(JointOrientations o)
{
	angles = o; // angleMeasurement();
}

JointOrientations CurrentState::angleMeasurement()
{
	JointOrientations angles;
	return angles;
}

/*
Invalid jointNumber: 0, 3, 15, 19, 20, 21, 22, 23, 24
*/
float CurrentState::angleSizeCalc(EMMA::Joints jointNumber)
{
	SpacePoint j1, j2, j3;
	j2 = joints[jointNumber];
	float angle = 0;
	switch (jointNumber){
	case SpineMid:		// case 1
		j1 = joints[SpineBase];
		j3 = joints[SpineShoulder];
		break;
	case Neck:			// case 2
		j1 = joints[Head];
		j3 = joints[SpineShoulder];
		break;
	case ShoulderLeft:	// case 4
		j1 = joints[ElbowLeft];
		j3 = joints[SpineShoulder];
		break;
	case ElbowLeft:		// case 5
		j1 = joints[ShoulderLeft];
		j3 = joints[WristLeft];
		break;
	case WristLeft:		// case 6
		j1 = joints[HandLeft];
		j3 = joints[ElbowLeft];
		break;
	case HandLeft:		// case 7
		j1 = joints[HandTipLeft];
		j3 = joints[WristLeft];
		break;
	case ShoulderRight: // case 8
		j1 = joints[SpineShoulder];
		j3 = joints[ElbowRight];
		break;
	case ElbowRight:// case 9
		j1 = joints[ShoulderRight];
		j3 = joints[WristRight];
		break;
	case WristRight: // case 10
		j1 = joints[HandRight];
		j3 = joints[ElbowRight];
		break;
	case HandRight: // case 11
		j1 = joints[HandTipRight];
		j3 = joints[WristRight];
		break;
	case HipLeft: // case 12
		j1 = joints[SpineBase];
		j3 = joints[KneeLeft];
		break;
	case KneeLeft: // case 13
		j1 = joints[HipLeft];
		j3 = joints[AnkleLeft];
		break;
	case AnkleLeft: // case 14
		j1 = joints[KneeLeft];
		j3 = joints[FootLeft];
		break;
	case HipRight: // case 16
		j1 = joints[SpineBase];
		j3 = joints[KneeRight];
		break;
	case KneeRight: // case 17
		j1 = joints[HipRight];
		j3 = joints[AnkleRight];
		break;
	case AnkleRight: // case 18
		j1 = joints[KneeRight];
		j3 = joints[FootRight];
		break;
	case SpineBase:
	case Head:
	case FootLeft:
	case FootRight:
	case SpineShoulder:
	case HandTipLeft:
	case ThumbLeft:
	case HandTipRight:
	case ThumbRight:
		throw invalid_mechanical_parameters_error("Planar angle calculation for leaf nodes undefined!");
	}

	return angle;
}

SpacePoint CurrentState::centerOfGravityMeasurement()
{
	SpacePoint cog(0.0, 0.0, 0.0);

	cog[0] = TRUNK_MASS*joints[SpineBase][0] + HAND_MASS*joints[HandLeft][0] +
		FOREARM_MASS*joints[ElbowLeft][0] + UPPERARM_MASS*joints[ShoulderLeft][0] +
		FOOT_MASS*joints[FootLeft][0] + LOWERLEG_MASS*joints[KneeLeft][0] +
		UPPERLEG_MASS*joints[HipLeft][0] + HEAD_NECK_MASS*joints[Neck][0];

	cog[1] = TRUNK_MASS*joints[SpineBase][1] + HAND_MASS*joints[HandLeft][1] +
		FOREARM_MASS*joints[ElbowLeft][1] + UPPERARM_MASS*joints[ShoulderLeft][1] +
		FOOT_MASS*joints[FootLeft][1] + LOWERLEG_MASS*joints[KneeLeft][1] +
		UPPERLEG_MASS*joints[HipLeft][1] + HEAD_NECK_MASS*joints[Neck][1];
			
	cog[2] = TRUNK_MASS*joints[SpineBase][2] + HAND_MASS*joints[HandLeft][2] +
		FOREARM_MASS*joints[ElbowLeft][2] + UPPERARM_MASS*joints[ShoulderLeft][2] +
		FOOT_MASS*joints[FootLeft][2] + LOWERLEG_MASS*joints[KneeLeft][2] +
		UPPERLEG_MASS*joints[HipLeft][2] + HEAD_NECK_MASS*joints[Neck][2];
	
	return cog;
}

QDataStream& CurrentState::__outStreamOperator(QDataStream& out) const
{
	// see http://doc.qt.io/qt-5/datastreamformat.html for what
	// types are supported out of the box
	out << timestamp << joints << angles;
	return out;
}

QDataStream& operator<<(QDataStream& out, const CurrentState& state)
{
	return state.__outStreamOperator(out);
}

CurrentState CurrentState::read_next_from_stream(QDataStream& input)
{
	CurrentState result;
	input >> result.timestamp >> result.joints >> result.angles;
	return result;
}

CurrentState::operator std::string(void) const
{
	return "demonstrational string cast!!!: " + (joints.isEmpty()? "{}": std::to_string(joints.first().z())) + ", " + std::to_string(angles.size());
}
