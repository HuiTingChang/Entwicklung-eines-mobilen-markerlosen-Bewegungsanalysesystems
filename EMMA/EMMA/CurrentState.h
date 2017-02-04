// non-standard but widely supported preprocessor directive designed to cause 
// the current source file to be included only once in a single compilation
//#pragma once

#include <QPoint>
#include <QMap>

struct SpacePoint
{
	SpacePoint() : X(0.0), Y(0.0), Z(0.0)
	{}
	SpacePoint(float xx, float yy, float zz) : X(xx), Y(yy), Z(zz)
	{}

	float X;
	float Y;
	float Z;
};

// center of gravity (schwerpunkt)
typedef struct _CGSpacePoint
{
	_CGSpacePoint(float xx, float yy) : X(xx), Y(yy)
	{}

	float X;
	float Y;
} 	CGSpacePoint;


class CurrentState
{

private:
	// from BalanceBoard
	QPoint centOfPr;
	CGSpacePoint centOfGv;
	float gewicht;

	// from camera
	QMap<uint, SpacePoint> joints;
	QMap<uint, float> angles;

public:

	CurrentState();
	~CurrentState();

	// setter Methods
	void set_centOfPr(QPoint cop);
	void set_centOfGv();
	void set_gewicht(float g);
	void set_jointPositions(QMap<uint, SpacePoint> j);
	void set_angles();

	// getter Methods
	float get_gewicht() const;
	QPoint get_centOfPr() const;
	CGSpacePoint get_centOfGv() const;
	QMap<uint, SpacePoint> get_joints() const;
	QMap<uint, float> get_angles() const;

private:

	QMap<uint, float> angleMeasurement();
	CGSpacePoint centerOfGravityMeasurement();
};