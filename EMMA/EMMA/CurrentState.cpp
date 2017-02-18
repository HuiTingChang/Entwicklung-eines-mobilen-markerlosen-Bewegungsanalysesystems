#include "CurrentState.h"

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

	// TODO Hui-Ting Code, if necessary

	return angles;
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
	out << joints << angles;
	return out;
}

QDataStream& operator<<(QDataStream& out, const CurrentState& state)
{
	return state.__outStreamOperator(out);
}

CurrentState CurrentState::read_next_from_stream(QDataStream& input)
{
	CurrentState result;
	input >> result.joints >> result.angles;
	return result;
}

CurrentState::operator std::string(void) const
{
	return "demonstrational string cast!!!: " + (joints.isEmpty()? "{}": std::to_string(joints.first().z())) + ", " + std::to_string(angles.size());
}
