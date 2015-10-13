#include "stdafx.h"
#include "CollisionSphereObject.h"


CollisionSphereObject::CollisionSphereObject() : g_radius(1.0f)
{
	colObjects = nullptr;
}

CollisionSphereObject::CollisionSphereObject(GLfloat radius) : g_radius(radius)
{
	colObjects = nullptr;
}

CollisionSphereObject::CollisionSphereObject(GLfloat radius, GLfloat pos[3]) : g_radius(radius)
{
	colObjects = nullptr;
	g_pos[0] = pos[0];
	g_pos[1] = pos[1];
	g_pos[2] = pos[2];
}

CollisionSphereObject::~CollisionSphereObject()
{
}

GLvoid CollisionSphereObject::draw()
{
	if(colObjects != nullptr)
		updateCollisions();

	//x_pos -= speed * deltaTime;
	GLfloat alphaTransparency = 0.5f;
	GLfloat red = 0.0;
	GLfloat green = 0.0;
	if (collision_active) {
		// Red :: collision
		red = 1.0;
		green = 0.0;
	}
	else {
		// Green :: no collision
		red = 0.0;
		green = 1.0;
	}
	// enable blending for transparent sphere
	glPushAttrib(GL_CURRENT_BIT);
	glEnable(GL_BLEND);     // Turn Blending On
	glEnable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// sphere 
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor4f(red, green, 0.0f, alphaTransparency);
	glTranslatef(g_pos[0], g_pos[1], g_pos[2]);
	//glRotatef(faceAngle_deg, 0.0f, 1.0f, 0.0f);
	glutSolidSphere(g_radius, 64, 40);
	glPopMatrix();


	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
	glPopAttrib();
}

GLboolean CollisionSphereObject::collidesWith(CollisionSphereObject * other)
{
	GLfloat distance =
		  ((g_pos[0] - other->g_pos[0]) * (g_pos[0] - other->g_pos[0]))
		+ ((g_pos[1] - other->g_pos[1]) * (g_pos[1] - other->g_pos[1]))
		+ ((g_pos[2] - other->g_pos[2]) * (g_pos[2] - other->g_pos[2]));
	return distance <= (g_radius + other->g_radius) * (g_radius + other->g_radius);
}

void CollisionSphereObject::assignID()
{
	id = colObjects->size() + 1;
}

GLvoid CollisionSphereObject::updateCollisions()
{
	if (colObjects->size() > 1)
	{
		for (int i = 0; i < colObjects->size(); i++)
		{
			if (id != colObjects->at(i)->id)
			{
				if (collidesWith(colObjects->at(i)))
				{
					collision_active = true;
					//remember
					bool exists = false;
					for (int j = 0; j < collidedWith.size(); j++)
					{
						if (collidedWith[j]->id == colObjects->at(i)->id)
						{
							exists = true;
						}
					}
					if (!exists)
					{
						collidedWith.push_back(colObjects->at(i));
					}
					//
				}
				else
				{
					collision_active = false;
					//forget
					for (int j = 0; j < collidedWith.size(); j++)
					{
						if (collidedWith[j]->id == colObjects->at(i)->id)
						{
							collidedWith.erase(collidedWith.begin() + j);
						}
					}
					//
				}
			}
		}
	}
}
GLvoid CollisionSphereObject::setObjects(std::vector<CollisionSphereObject*> * colObjectsIn)
{
	assignID();
}