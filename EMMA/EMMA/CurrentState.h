// non-standard but widely supported preprocessor directive designed to cause
// the current source file to be included only once in a single compilation
#pragma once

#include <string>
#include <QDateTime>
#include <QPoint>
#include <QMap>
#include <QVector3D>
#include <QVector4D>
#include <QDataStream>
#include "Anatomy.h"


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
	/**
	* milliseconds since Epoch (in UTC) of the last update trigger (values
	* can be older if no update was received)
	*/
	qint64 timestamp;
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
	bool operator==(const CurrentState& other) const;
	Q_SLOT void update();

	// setter Methods
	void set_centOfPr(BoardPoint cop);
	void set_centOfGv();
	void set_gewicht(float g);
	void set_jointPositions(JointPositions j);
	void set_angles(JointOrientations o);

	// getter Methods
	QDateTime get_timestamp() const;
	float get_gewicht() const;
	BoardPoint get_centOfPr() const;
	SpacePoint get_centOfGv() const;
	JointPositions get_joints() const;
	JointOrientations get_angles() const;

	QDataStream& __outStreamOperator(QDataStream& out) const;
	static CurrentState read_next_from_stream(QDataStream& input);
	operator std::string(void) const;

private:

	JointOrientations angleMeasurement();
	float angleSizeCalc();
	SpacePoint centerOfGravityMeasurement();
};

QDataStream& operator<<(QDataStream& out, const CurrentState& state); 
