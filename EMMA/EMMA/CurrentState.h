// non-standard but widely supported preprocessor directive designed to cause
// the current source file to be included only once in a single compilation
#pragma once

#include <cmath>
#include <string>
#include <QDateTime>
#include <QPoint>
#include <QMap>
#include <QVector3D>
#include <QVector4D>
#include <QDataStream>
#include <QTextStream>
#include <QException>
#include "Anatomy.h"


// Point in the reference space
typedef QVector3D SpacePoint;

// Angles between the vector and x-, y-, and z-axis
typedef QVector3D Angles3D;

// Angle in the reference space
typedef QVector4D Quaternion;

// Point on the Balance Board
typedef QPoint BoardPoint;

typedef QMap<uint,SpacePoint> JointPositions;

// A QMap to store the result of calculation of angles in a relative coordination system
typedef QMap<uint, Angles3D> JointRelativeAngles; 

typedef QMap<uint, Quaternion> JointOrientations;

const double P_PI = std::acos(-1);

class CurrentState
{
private:
	static const bool QTEXTSTREAM_FORMAT_CSV;
	/**
	* milliseconds since Epoch (in UTC) of the last update trigger (values
	* can be older if no update was received)
	*/
	qint64 timestamp;
	// from camera
	JointPositions joints;
	SpacePoint centOfGv;
	JointRelativeAngles angles;

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
	void set_angles(); 

	// getter Methods
	QDateTime get_timestamp() const;
	float get_gewicht() const;
	BoardPoint get_centOfPr() const;
	SpacePoint get_centOfGv() const;
	JointPositions get_joints() const;
	JointRelativeAngles get_angles() const;
	float angleSizeCalc(EMMA::Joints jointNumber);

	QDataStream& __outStreamOperator(QDataStream& out) const;
	QTextStream& __outStreamOperator(QTextStream& out) const;
	static CurrentState read_next_from_stream(QDataStream& input);
	operator std::string(void) const;

private:

	//float angleOf3Points(SpacePoint& p1, SpacePoint& p2, SpacePoint& p3);

	SpacePoint centerOfGravityMeasurement();
	JointRelativeAngles anglesInRelativeCoordinateSystem();
	static EMMA::Joints getParentJoint(EMMA::Joints);
};

/**
* only implemented for QDataStream and QTextStream.
* QDataStream output should contain the complete state information
* without conversion
* QTextStream output is lossy and may contain useful values that are not
* part of the state information
*/
template<class T>
T& operator<<(T& out, const CurrentState& state);

class invalid_mechanical_parameters_error: public QException
{
private:
	const char* what_cstr;
public:
	void raise() const { throw *this; }
	invalid_mechanical_parameters_error *clone() const { return new invalid_mechanical_parameters_error(*this); }
	invalid_mechanical_parameters_error(const char* what): what_cstr(what) {}
	virtual const char* what() const throw()
	{
		return what_cstr? what_cstr: "Invalid parameters for mechanical calculation!";
	}
};
