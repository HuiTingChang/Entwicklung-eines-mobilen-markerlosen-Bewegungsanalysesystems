#include "Anatomy.h"

namespace {
QMap<EMMA::Bones, double> get_com_winter()
{
    QMap<EMMA::Bones,double> com;
    com[EMMA::Root_SpineBase]	=		0,
    com[EMMA::SpineBase_SpineMid]	=		1,
    com[EMMA::SpineShoulder_Neck]		=		0,
    com[EMMA::Neck_Head]		=		HEAD_NECK_COM,
    com[EMMA::SpineShoulder_ShoulderLeft]	=	0,
    com[EMMA::ShoulderLeft_ElbowLeft]	=		UPPERARM_COM,
    com[EMMA::ElbowLeft_WristLeft]	=		FOREARM_COM,
    com[EMMA::WristLeft_HandLeft]	=		HAND_COM,
    com[EMMA::SpineShoulder_ShoulderRight]	=	0,
    com[EMMA::ShoulderRight_ElbowRight]	=		UPPERLEG_COM,
    com[EMMA::ElbowRight_WristRight]	=		FOREARM_COM,
    com[EMMA::WristRight_HandRight]	=		HAND_COM,
    com[EMMA::SpineBase_HipLeft]		=		0,
    com[EMMA::HipLeft_KneeLeft]	=		UPPERLEG_COM,
    com[EMMA::KneeLeft_AnkleLeft]	=		LOWERLEG_COM,
    com[EMMA::AnkleLeft_FootLeft]	=		FOOT_COM,
    com[EMMA::SpineBase_HipRight]	=		0,
    com[EMMA::HipRight_KneeRight]	=		UPPERLEG_COM,
    com[EMMA::KneeRight_AnkleRight]	=		LOWERLEG_COM,
    com[EMMA::AnkleRight_FootRight]	=		FOOT_COM,
    com[EMMA::SpineMid_SpineShoulder]	=	0,
    com[EMMA::HandLeft_HandTipLeft]	=		0,
    com[EMMA::HandLeft_ThumbLeft]	=		0,
    com[EMMA::HandRight_HandTipRight]	=	0,
    com[EMMA::HandRight_ThumbRight]	=		0;
    return com;
}

QMap<EMMA::Bones, double> get_mass_winter()
{
    QMap<EMMA::Bones,double> mass;
    mass[EMMA::Root_SpineBase]	=		0,
    mass[EMMA::SpineBase_SpineMid]	=		TRUNK_MASS/2,
    mass[EMMA::SpineShoulder_Neck]		=		0,
    mass[EMMA::Neck_Head]		=		HEAD_NECK_MASS,
    mass[EMMA::SpineShoulder_ShoulderLeft]	=	0,
    mass[EMMA::ShoulderLeft_ElbowLeft]	=		UPPERARM_MASS,
    mass[EMMA::ElbowLeft_WristLeft]	=		FOREARM_MASS,
    mass[EMMA::WristLeft_HandLeft]	=		HAND_MASS,
    mass[EMMA::SpineShoulder_ShoulderRight]	=	0,
    mass[EMMA::ShoulderRight_ElbowRight]	=		UPPERARM_MASS,
    mass[EMMA::ElbowRight_WristRight]	=		FOREARM_MASS,
    mass[EMMA::WristRight_HandRight]	=		HAND_MASS,
    mass[EMMA::SpineBase_HipLeft]		=		0,
    mass[EMMA::HipLeft_KneeLeft]	=		UPPERLEG_MASS,
    mass[EMMA::KneeLeft_AnkleLeft]	=		LOWERLEG_MASS,
    mass[EMMA::AnkleLeft_FootLeft]	=		FOOT_MASS,
    mass[EMMA::SpineBase_HipRight]	=		0,
    mass[EMMA::HipRight_KneeRight]	=		UPPERLEG_MASS,
    mass[EMMA::KneeRight_AnkleRight]	=		LOWERLEG_MASS,
    mass[EMMA::AnkleRight_FootRight]	=		FOOT_MASS,
    mass[EMMA::SpineMid_SpineShoulder]	=	TRUNK_MASS/2,
    mass[EMMA::HandLeft_HandTipLeft]	=		0,
    mass[EMMA::HandLeft_ThumbLeft]	=		0,
    mass[EMMA::HandRight_HandTipRight]	=	0,
    mass[EMMA::HandRight_ThumbRight]	=		0;
    return mass;
}
}

const QMap<EMMA::Bones,double> Winter_Anatomy::BONE_COM = get_com_winter();
const QMap<EMMA::Bones,double> Winter_Anatomy::BONE_MASS = get_mass_winter();

EMMA::Bones Winter_Anatomy::get_parent_bone(EMMA::Joints joint)
{
    return (EMMA::Bones) (unsigned int) joint;
}

EMMA::Joints Winter_Anatomy::get_child_joint(EMMA::Bones bone)
{
    return (EMMA::Joints) (unsigned int) bone;
}
