#pragma once
#include "CollisionSphereObject.h"
class KillerRobot :
	public CollisionSphereObject
{
public:
	KillerRobot();
	KillerRobot(GLfloat radius);
	KillerRobot(GLfloat radius, GLfloat pos[3]);
	~KillerRobot();
	GLvoid draw();
	GLvoid update();
	GLfloat g_angle[9] =
	{
		0.0f, //torso
		0.0f, //left shoulder
		0.0f, //right shoulder
		0.0f, //left elbow
		0.0f, //right elbow
		0.0f, //left leg
		0.0f, //right leg
		0.0f, //left knee
		0.0f  //right knee
	};
private:
	enum { torso, l_shoulder, r_shoulder, l_elbow, r_elbow, l_leg, r_leg, l_knee, r_knee };
};

