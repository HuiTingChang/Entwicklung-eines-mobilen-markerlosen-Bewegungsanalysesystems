#pragma once
#include <QMap>

// Normalized Mass and Length of Body Segments (Standard Human)
// *MASS is segment mass/total body mass
// *COM is proximal displacement of center of mass/segment length
// adapted from DA Winter:
// "Biomechanics and Motor Control of Human Movement", 3rd edition
// (John Wiley & Sons 2005)
// via http://www.ele.uri.edu/faculty/vetter/BME207/anthropometric-data.pdf

#define HAND_MASS			0.006
#define HAND_COM			0.506

#define FOREARM_MASS		0.016
#define FOREARM_COM			0.430


#define UPPERARM_MASS		0.028
#define UPPERARM_COM		0.436

#define FOOT_MASS			0.0145
#define FOOT_COM			0.500

#define LOWERLEG_MASS		0.0465
#define LOWERLEG_COM		0.433

#define UPPERLEG_MASS		0.100
#define UPPERLEG_COM		0.433

#define HEAD_NECK_MASS		0.081
#define HEAD_NECK_COM		1.000

#define TRUNK_MASS			0.497
#define TRUNK_COM			0.500

namespace EMMA
{
const int JOINTS_COUNT =	25;

/**
* corresponds to the joints model of kinect v2.0, see
* http://peted.azurewebsites.net/avateering-with-kinect-v2-joint-orientations/
*/
enum Joints: unsigned int
{
	SpineBase	=		0,
	SpineMid	=		1,
	Neck		=		2,
	Head		=		3,
	ShoulderLeft	=	4,
	ElbowLeft	=		5,
	WristLeft	=		6,
	HandLeft	=		7,
	ShoulderRight	=	8,
	ElbowRight	=		9,
	WristRight	=		10,
	HandRight	=		11,
	HipLeft		=		12,
	KneeLeft	=		13,
	AnkleLeft	=		14,
	FootLeft	=		15,
	HipRight	=		16,
	KneeRight	=		17,
	AnkleRight	=		18,
	FootRight	=		19,
	SpineShoulder	=	20,
	HandTipLeft	=		21,
	ThumbLeft	=		22,
	HandTipRight	=	23,
	ThumbRight	=		24
};

enum Bones: unsigned int
{
	Root_SpineBase	=		0,  // pseudo-bone for compatibility
	SpineBase_SpineMid	=		1,
	SpineShoulder_Neck		=		2,
	Neck_Head		=		3,
	SpineShoulder_ShoulderLeft	=	4,
	ShoulderLeft_ElbowLeft	=		5,
	ElbowLeft_WristLeft	=		6,
	WristLeft_HandLeft	=		7,
	SpineShoulder_ShoulderRight	=	8,
	ShoulderRight_ElbowRight	=		9,
	ElbowRight_WristRight	=		10,
	WristRight_HandRight	=		11,
	SpineBase_HipLeft		=		12,
	HipLeft_KneeLeft	=		13,
	KneeLeft_AnkleLeft	=		14,
	AnkleLeft_FootLeft	=		15,
	SpineBase_HipRight	=		16,
	HipRight_KneeRight	=		17,
	KneeRight_AnkleRight	=		18,
	AnkleRight_FootRight	=		19,
	SpineMid_SpineShoulder	=	20,
	HandLeft_HandTipLeft	=		21,
	HandLeft_ThumbLeft	=		22,
	HandRight_HandTipRight	=	23,
	HandRight_ThumbRight	=		24
};
}

class Winter_Anatomy
{
public:
static const QMap<EMMA::Bones,double> BONE_COM;
static const QMap<EMMA::Bones,double> BONE_MASS;
static EMMA::Bones get_parent_bone(EMMA::Joints joint);
static EMMA::Joints get_child_joint(EMMA::Bones bone);
};
