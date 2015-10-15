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

GLvoid KillerRobot::update()
{
	CollisionSphereObject::update();
	if (g_speed > 0.5f || g_speed < -0.5f)
	{
		g_cycle += g_speed * 0.5f;
		if (g_cycle > 360.0f)
			g_cycle -= 360.0f;
		else if (g_cycle < 0.0f)
			g_cycle += 360.0f;
	}
	else
	{
		
		if (g_cycle > 270.125f && g_cycle < 360.0f)
		{
			g_cycle += 1.0f * 0.25f;
		}
		else if (g_cycle > 180.125f && g_cycle < 270.0f)
		{
			g_cycle -= 1.0f * 0.25f;
		}
		else if (g_cycle > 90.125f && g_cycle < 180.0f)
		{
			g_cycle += 1.0f * 0.25f;
		}
		else if (g_cycle > 0.0f && g_cycle < 90.0f)
		{
			g_cycle -= 1.0f * 0.25f;
		}
		
	}
	g_angle[l_leg] = sin((3.14159f * g_cycle / 180)) * 45.0f;
	g_angle[r_leg] = -sin((3.14159f * g_cycle / 180)) * 45.0f;
	g_angle[l_knee] = fabs(sin((3.14159f * g_cycle / 180)) * 45.0f);
	g_angle[r_knee] = fabs(sin((3.14159f * g_cycle / 180)) * 45.0f);
}

GLvoid KillerRobot::draw()
{
	drawRobot();
	CollisionSphereObject::draw();
}

GLvoid KillerRobot::drawRobot()
{
	glPushAttrib(GL_CURRENT_BIT);
	glEnable(GL_BLEND);     // Turn Blending On
	glEnable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPushMatrix();
	
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor4f(0.75f, 0.75f, 0.75f, 1.0f);
	//glRotatef(faceAngle_deg, 0.0f, 1.0f, 0.0f);
	glTranslatef(g_pos[0], g_pos[1] + 1.0f, g_pos[2]);
	glRotatef(-faceAngle_deg + 90.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.25f, 0.25f, 0.25f);
	//torso/upper body
	glPushMatrix();
	glRotatef(g_angle[torso], 0.0, 1.0, 0.0);
	//chest
	glPushMatrix();
	glScalef(4.0f, 2.0f, 2.0f);
	glutSolidCube(1.0);
	glPopMatrix();
	//Left arm
	drawArm(1.0f);
	//Right arm
	drawArm(-1.0f);
	//head
	glPushMatrix();
	glTranslatef(0.0f, 1.75f, 0.0f);
	glutSolidCylinder(0.4, 0.75, 32, 10);//tin can shape
	glTranslatef(0.0f, 0.25f, 0.0f);
	glScalef(1.25f, 1.25f, 1.25f);
	glutSolidSphere(0.5, 32, 10);// with rounded top
	glTranslatef(0.0f, -1.0f, 0.0f);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.5, 1.0, 32, 10);// and a single eye
	glPopMatrix();
	//stomach
	glPushMatrix();
	glTranslatef(0.0f, -2.0f, 0.0f);
	glScalef(3.0f, 2.0f, 1.5f);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();
	//waist/legs
	glPushMatrix();
	//waist
	glTranslatef(0.0f, -3.0f, 0.0f);
	glPushMatrix();
	glScalef(4.0f, 1.0f, 2.0f);
	glutSolidCube(1);
	glPopMatrix();
	glTranslatef(0.0f, -1.0f, 0.0f);
	glPushMatrix();
	glScalef(0.75f, 1.0f, 2.0f);
	glutSolidCube(1);
	glPopMatrix();
	//left leg
	drawLeg(1.0f);
	//right leg
	drawLeg(-1.0f);
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_BLEND);

	glPopAttrib();
}

GLvoid KillerRobot::drawArm(GLfloat offset)
{
	glPushMatrix();
	glTranslatef(2.8f * offset, 0.75f, 0.0f);
	if (offset > 0)
		glRotatef(g_angle[l_shoulder], 1.0f, 0.0f, 0.0f);
	else
		glRotatef(g_angle[r_shoulder], 1.0f, 0.0f, 0.0f);
	//shoulder
	glPushMatrix();
	glScalef(2.0f, 1.0f, 2.0f);
	glutSolidCube(1.0);
	glPopMatrix();
	glTranslatef(0.2f * offset, -1.25f, 0.0f);
	//bicep
	glPushMatrix();
	glScalef(1.0f, 3.0f, 1.0f);
	glutSolidCube(1.0);
	glPopMatrix();
	//elbow
	glTranslatef(0.5f * offset, -2.0f, 0.0f);
	if (offset > 0)
		glRotatef(g_angle[l_elbow], 1.0f, 0.0f, 0.0f);
	else
		glRotatef(g_angle[r_elbow], 1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(0.9f * offset - offset * 2, 0.0f, 0.0f);
	glRotatef(90.0f * offset, 0.0, 1.0, 0.0f);
	glutSolidCylinder(0.75, 1.2, 32, 10);
	glPopMatrix();
	//forearm
	glTranslatef(-0.50f * offset, 0.0f, 2.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glScalef(1.0f, 3.0f, 1.0f);
	glutSolidCube(1.0);
	glPopMatrix();
	glTranslatef(0.0f, 2.0f, 0.375f);
	glPushMatrix();
	glScalef(1.0f, 1.0f, 0.25f);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}

GLvoid KillerRobot::drawLeg(GLfloat offset)
{
	glPushMatrix();
	glTranslatef(0.5f * offset, -0.0f, 0.0f);
	if (offset > 0)
		glRotatef(g_angle[l_leg], 1.0f, 0.0f, 0.0f);
	else
		glRotatef(g_angle[r_leg], 1.0f, 0.0f, 0.0f);

	glPushMatrix();
	glRotatef(90 * offset, 0.0f, 1.0f, 0.0f);
	glutSolidCylinder(1.0f, 1.5f, 32, 10);
	glPopMatrix();
	glTranslatef(0.75f * offset, -2.0f, 0.0f);
	glPushMatrix();
	glScalef(1.25f, 3.0f, 1.25f);
	glutSolidCube(1);
	glPopMatrix();
	glTranslatef(0.25f * offset - offset, -2.0f, 0.0f);
	if (offset > 0)
		glRotatef(g_angle[l_knee], 1.0f, 0.0f, 0.0f);
	else
		glRotatef(g_angle[r_knee], 1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(90 * offset, 0.0f, 1.0f, 0.0f);
	glScalef(1.0f, 1.0f, 1.0f);
	glutSolidCylinder(0.75f, 1.5f, 32, 10);
	glPopMatrix();
	glTranslatef(0.75f * offset, -1.75f, 0.0f);
	glPushMatrix();
	glScalef(1.25f, 2.75f, 1.25f);
	glutSolidCube(1);
	glPopMatrix();
	glTranslatef(0.0f, -1.5f, 0.5f);
	glScalef(1.25f, 0.5f, 2.5f);
	glutSolidCube(1);
	glPopMatrix();
}