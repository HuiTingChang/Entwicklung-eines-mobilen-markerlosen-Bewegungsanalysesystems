// non-standard but widely supported preprocessor directive designed to cause
// the current source file to be included only once in a single compilation
#pragma once

#include <QPoint>
#include <QMap>
#include <QVector3D>
#include <QVector4D>
#include <QDataStream>

#define JOINTS_COUNT 25

// Point in the reference space
typedef QVector3D SpacePoint;

// Angle in the reference space
typedef QVector4D Quaternion;

// Point on the Balance Board
typedef QPoint BoardPoint;

typedef QMap<uint,SpacePoint> JointPositions;

typedef QMap<uint, Quaternion> JointOrientations;

class CurrentState
{

private:
	// from camera
	JointPositions joints;
	SpacePoint centOfGv;
	JointOrientations angles;

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
	void set_angles(JointOrientations o);

	// getter Methods
	float get_gewicht() const;
	BoardPoint get_centOfPr() const;
	SpacePoint get_centOfGv() const;
	JointPositions get_joints() const;
	JointOrientations get_angles() const;

	QDataStream& __outStreamOperator(QDataStream& out) const;
	static CurrentState read_next_from_stream(QDataStream& input);

private:

	JointOrientations angleMeasurement();
	SpacePoint centerOfGravityMeasurement();
};

QDataStream& operator<<(QDataStream& out, const CurrentState& state); 
