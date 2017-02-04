#include "CurrentState.h"

CurrentState::CurrentState() : 
centOfPr(QPoint(0, 0)), centOfGv(CGSpacePoint(0.0, 0.0)), gewicht(0.0)
{}

CurrentState::~CurrentState()
{}

float CurrentState::get_gewicht() const
{
	return gewicht;
}

QPoint CurrentState::get_centOfPr() const
{
	return centOfPr;
}

CGSpacePoint CurrentState::get_centOfGv() const
{
	return centOfGv;
}

QMap<uint, SpacePoint> CurrentState::get_joints() const
{
	return joints;
}

QMap<uint, float> CurrentState::get_angles() const
{
	return angles;
}

void CurrentState::set_centOfPr(QPoint cop)
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

void CurrentState::set_jointPositions(QMap<uint, SpacePoint> j)
{
	joints = j;
}

void CurrentState::set_angles()
{
	angles = angleMeasurement();
}

QMap<uint, float> CurrentState::angleMeasurement()
{
	QMap<uint, float> angles;

	// TODO Hui-Ting Code

	return angles;
}

CGSpacePoint CurrentState::centerOfGravityMeasurement()
{
	CGSpacePoint cog(0.0,0.0);

	// TODO Hui-Ting Code

	return cog;
}