#pragma once
#include "CollisionSphereObject.h"
class Character :
	public CollisionSphereObject
{
public:
	Character();
	Character(GLfloat radius);
	Character(GLfloat radius, GLfloat pos[3]);
	~Character();
	
private:
};

