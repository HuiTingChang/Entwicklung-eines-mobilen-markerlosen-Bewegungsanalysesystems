#include "CurrentState.h"

CurrentState::CurrentState():
	centOfGv(SpacePoint(0.0, 0.0, 0.0)),
	centOfPr(BoardPoint(0, 0)),
	gewicht(0.0)
{}

CurrentState::~CurrentState()
{}

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
	SpacePoint cog(0.0,0.0,0.0);

	// TODO Hui-Ting Code

	return cog;
}

QDataStream& CurrentState::__outStreamOperator(QDataStream& out) const
{
	// see http://doc.qt.io/qt-5/datastreamformat.html for what
	// types are supported out of the box
	out << joints;
	return out;
}

QDataStream& operator<<(QDataStream& out, const CurrentState& state)
{
	return state.__outStreamOperator(out);
}
