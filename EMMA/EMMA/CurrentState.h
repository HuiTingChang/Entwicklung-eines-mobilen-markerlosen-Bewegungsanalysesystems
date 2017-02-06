// non-standard but widely supported preprocessor directive designed to cause
// the current source file to be included only once in a single compilation
#pragma once

#include <QPoint>
#include <QMap>
#include <QVector3D>
#include <QDataStream>

#define JOINTS_COUNT 25

/*
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
*/

// Point in the reference space
typedef QVector3D SpacePoint;

// center of gravity (schwerpunkt)
/*
typedef struct _CGSpacePoint
{
	_CGSpacePoint(float xx, float yy) : X(xx), Y(yy)
	{}

	float X;
	float Y;
} 	CGSpacePoint;
*/

// Point on the Balance Board
typedef QPoint BoardPoint;

typedef QMap<uint,SpacePoint> JointPositions;

class CurrentState
{

private:
	// from camera
	JointPositions joints;
	SpacePoint centOfGv;
	QMap<uint, float> angles;

	// from BalanceBoard
	BoardPoint centOfPr;
	float gewicht;

public:

	CurrentState();
	~CurrentState();

	// setter Methods
	void set_centOfPr(BoardPoint cop);
	void set_centOfGv();
	void set_gewicht(float g);
	void set_jointPositions(JointPositions j);
	void set_angles();

	// getter Methods
	float get_gewicht() const;
	BoardPoint get_centOfPr() const;
	SpacePoint get_centOfGv() const;
	JointPositions get_joints() const;
	QMap<uint, float> get_angles() const;

	QDataStream& __outStreamOperator(QDataStream& out) const;

private:

	QMap<uint, float> angleMeasurement();
	SpacePoint centerOfGravityMeasurement();
};

QDataStream& operator<<(QDataStream& out, const CurrentState& state); 
