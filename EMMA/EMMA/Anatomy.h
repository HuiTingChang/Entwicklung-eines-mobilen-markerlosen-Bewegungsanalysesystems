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
}
