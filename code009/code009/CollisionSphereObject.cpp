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
	if(drawSphere)
		drawBoundingSphere();
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
	bool IDavailable = true;
	int j = 0;
	do 
	{
		IDavailable = true;
		j++;
		for (int i = 0; i < colObjects->size(); i++)
		{
			if (colObjects->at(i)->id == j)
			{
				IDavailable = false;
			}
		}
		
	} while (!IDavailable);
	id = j;
}

GLvoid CollisionSphereObject::update()
{
	faceAngle_deg += faceAngleSpeed_deg * g_speedMult * 60;
	faceAngle_rad = 3.14159f * faceAngle_deg / 180.0f;
	GLfloat z_pos = g_pos[2];
	GLfloat x_pos = g_pos[0];
	g_pos[2] += g_speed * sin(faceAngle_rad) * g_speedMult;
	g_pos[0] += g_speed * cos(faceAngle_rad) * g_speedMult;
	updateCollisions();
	if (collision_active)
	{
		g_pos[2] = z_pos;
		g_pos[0] = x_pos;
	}
}

GLvoid CollisionSphereObject::updateCollisions()
{
	bool collided = false;
	if (colObjects != nullptr)
	{
		if (colObjects->size() > 1)
		{
			for (int i = 0; i < colObjects->size() && !collided; i++)
			{
				if (id != colObjects->at(i)->id)
				{
					if (collidesWith(colObjects->at(i)))
					{
						collided = true;
					}
				}
			}
		}
	}
	if (collided)
	{
		collision_active = true;
	}
	else collision_active = false;
}
GLvoid CollisionSphereObject::setObjects(std::vector<CollisionSphereObject*> * colObjectsIn)
{
	colObjects = colObjectsIn;
	if(id == 0)
		assignID();
}

GLvoid CollisionSphereObject::drawBoundingSphere()
{
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