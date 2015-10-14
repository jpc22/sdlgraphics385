#include "stdafx.h"
#include "KillerRobot.h"


KillerRobot::KillerRobot()
{
	CollisionSphereObject();
}

KillerRobot::KillerRobot(GLfloat radius) : CollisionSphereObject(radius)
{
}

KillerRobot::KillerRobot(GLfloat radius, GLfloat pos[3]) : CollisionSphereObject(radius, pos)
{
}

KillerRobot::~KillerRobot()
{
}
