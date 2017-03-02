#include <sstream>
#include "text/csv/ostream.hpp"
#include "CurrentState.h"

using namespace EMMA;

const bool CurrentState::QTEXTSTREAM_FORMAT_CSV = true;

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
	angles = o; // anglesInRelativeCoordinateSystem();
}

JointRelativeAngles CurrentState::anglesInRelativeCoordinateSystem()
{
	JointRelativeAngles angles;
	// Define the direction of 3 axes
	SpacePoint main_y(0, 1, 0);	// Y axis is vertical
	SpacePoint main_x(joints[ShoulderLeft] - joints[SpineShoulder]); // X is directed along left arm in T-pose
	main_x.setY(0); // X is perpendicular to Y
	main_x.normalized();

	// Returns the normalized vector of a plane defined by vectors relative_x & relative_y.
	SpacePoint main_z = SpacePoint::normal(main_x, main_y);

	// Calculate alpha, beta and gama for each joint, except for SpineBase(0).
	SpacePoint angle, v1, v2;
	float theta, alpha, beta, gama;
	SpacePoint parentJoint;
	for (EMMA::Joints i = SpineMid; i != ThumbRight; i = EMMA::Joints(i + 1)){
		parentJoint = joints[getParentJoint(i)];

		v1 = main_x;
		v2 = joints[i] - parentJoint;
		theta = acos((v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]) / (sqrt(pow(v1[0], 2) + pow(v1[1], 2) + pow(v1[2], 2))*sqrt(pow(v2[0], 2) + pow(v2[1], 2) + pow(v2[2], 2))));
		alpha = theta * 180 / P_PI;
		angle.setX(alpha);

		v1 = main_y;
		theta = acos((v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]) / (sqrt(pow(v1[0], 2) + pow(v1[1], 2) + pow(v1[2], 2))*sqrt(pow(v2[0], 2) + pow(v2[1], 2) + pow(v2[2], 2))));
		beta = theta * 180 / P_PI;
		angle.setY(beta);

		v1 = main_z;
		theta = acos((v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]) / (sqrt(pow(v1[0], 2) + pow(v1[1], 2) + pow(v1[2], 2))*sqrt(pow(v2[0], 2) + pow(v2[1], 2) + pow(v2[2], 2))));
		gama = theta * 180 / P_PI;
		angle.setZ(gama);

		angles[i] = angle;
	}




	return angles;
}

EMMA::Joints CurrentState::getParentJoint(EMMA::Joints jointNumber){
	EMMA::Joints parent;
	switch (jointNumber){
		case SpineBase:
		case HipLeft:
		case HipRight:
		case SpineMid:
			parent = SpineBase;
			break;
		case KneeLeft:
			parent = HipLeft;
			break;
		case KneeRight:
			parent = HipRight;
			break;
		case AnkleLeft:
			parent = KneeLeft;
			break;
		case AnkleRight:
			parent = KneeRight;
			break;
		case FootLeft:
			parent = AnkleLeft;
			break;
		case FootRight:
			parent = AnkleRight;
			break;
		case SpineShoulder:
			parent = SpineMid;
			break;
		case Neck:
			parent = SpineShoulder;
			break;
		case Head:
			parent = Neck;
			break;
		case ShoulderLeft:
		case ShoulderRight:
			parent = SpineShoulder;
			break;
		case ElbowLeft:
			parent = ShoulderLeft;
			break;
		case ElbowRight:
			parent = ShoulderRight;
			break;
		case WristLeft:
			parent = ElbowLeft;
			break;
		case WristRight:
			parent = ElbowRight;
			break;
		case HandLeft:
			parent = WristLeft;
			break;
		case HandRight:
			parent = WristRight;
			break;
		case ThumbLeft:
		case HandTipLeft:
			parent = HandLeft;
			break;
		case ThumbRight:
		case HandTipRight:
			parent = HandRight;
			break;
	}
	return parent;

}

/*
Invalid jointNumber: 0, 3, 15, 19, 20, 21, 22, 23, 24
*/
float CurrentState::angleSizeCalc(EMMA::Joints jointNumber)
{
	SpacePoint j1, j2, j3, v1, v2;
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
	case ElbowRight:	// case 9
		j1 = joints[ShoulderRight];
		j3 = joints[WristRight];
		break;
	case WristRight:	// case 10
		j1 = joints[HandRight];
		j3 = joints[ElbowRight];
		break;
	case HandRight:		// case 11
		j1 = joints[HandTipRight];
		j3 = joints[WristRight];
		break;
	case HipLeft:		// case 12
		j1 = joints[SpineBase];
		j3 = joints[KneeLeft];
		break;
	case KneeLeft:		// case 13
		j1 = joints[HipLeft];
		j3 = joints[AnkleLeft];
		break;
	case AnkleLeft:		// case 14
		j1 = joints[KneeLeft];
		j3 = joints[FootLeft];
		break;
	case HipRight:		// case 16
		j1 = joints[SpineBase];
		j3 = joints[KneeRight];
		break;
	case KneeRight:		// case 17
		j1 = joints[HipRight];
		j3 = joints[AnkleRight];
		break;
	case AnkleRight:	// case 18
		j1 = joints[KneeRight];
		j3 = joints[FootRight];
		break;
	case SpineBase:		// case 20
		j1 = joints[HipLeft];
		j3 = joints[SpineMid];
		break;
	default:
		throw invalid_mechanical_parameters_error("Invalid joint for the calculation of angle size!");
		break;
	}
	float theta;
	v1 = j1 - j2;
	v2 = j3 - j2;
	theta = acos((v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]) / (sqrt(pow(v1[0], 2) + pow(v1[1], 2) + pow(v1[2], 2))*sqrt(pow(v2[0], 2) + pow(v2[1], 2) + pow(v2[2], 2))));
	angle = theta * 180 / P_PI;
	return angle;
}

//float angleOf3Points(SpacePoint& p1, SpacePoint& p2, SpacePoint& p3){
//	float theta, angle;
//	SpacePoint v1, v2;
//	//v1 = p1 - p2;
//	//v2 = p3 - p2;
//	//theta = acos((v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]) / (sqrt(pow(v1[0], 2) + pow(v1[1], 2) + pow(v1[2], 2))*sqrt(pow(v2[0], 2) + pow(v2[1], 2) + pow(v2[2], 2))));
//	//angle = theta * 180 / P_PI;
//	return angle;
//}


// http://www.ele.uri.edu/faculty/vetter/BME207/anthropometric-data.pdf
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

QTextStream& CurrentState::__outStreamOperator(QTextStream& out) const
{
	if(QTEXTSTREAM_FORMAT_CSV)
	{
		std::ostringstream os;
		text::csv::csv_ostream csv_out(os);

		csv_out << QString::number(timestamp).data();
		for(auto j=joints.begin();j != joints.end();++j)
		{
			csv_out
				<< j.value().x()
				<< j.value().y()
				<< j.value().z();
		}
		for(auto j=angles.begin();j != angles.end();++j)
		{
			csv_out
				<< j.value().w()
				<< j.value().x()
				<< j.value().y()
				<< j.value().z();
		}
		out << os.str().data();
	}
	else
	{
		out << timestamp;  // a lot missing!
	}
	endl(out);
	return out;
}

template<class T>
T& operator<<(T& out, const CurrentState& state)
{
	return state.__outStreamOperator(out);
}

template
QDataStream& operator<<(QDataStream& out, const CurrentState& state);
template
QTextStream& operator<<(QTextStream& out, const CurrentState& state);

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
