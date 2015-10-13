#include "stdafx.h"
#include "Character.h"


Character::Character() : CollisionSphereObject()
{
}

Character::Character(GLfloat radius) : CollisionSphereObject(radius)
{
}

Character::Character(GLfloat radius, GLfloat pos[3]) : CollisionSphereObject(radius, pos)
{
}


Character::~Character()
{
}
