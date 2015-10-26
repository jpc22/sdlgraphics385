#pragma once
class CollisionSphereObject
{
public:
	CollisionSphereObject();
	CollisionSphereObject(GLfloat radius);
	CollisionSphereObject(GLfloat radius, GLfloat pos[3]);
	~CollisionSphereObject();
	GLint id = NULL;
	GLfloat g_pos[3] = { 0.0f, 0.0f, 0.0f };
	GLfloat g_radius;
	GLboolean collision_active = false;
	GLvoid draw();
	GLboolean collidesWith(CollisionSphereObject * other);
	GLvoid setObjects(std::vector<CollisionSphereObject*> * colObjects);
	GLvoid drawBoundingSphere();
	//std::vector<CollisionSphereObject*> collidedWith;
	GLvoid update();
	GLvoid updateCollisions();
	GLboolean drawSphere = true;
	GLfloat g_friction = 0.025f;
	GLfloat g_speed = 0.0f;
	GLfloat g_speedMult = 0.003f;
	GLfloat faceAngle_deg = 0.0f;
	GLfloat faceAngleSpeed_deg = 0.0f;
	GLfloat faceAngle_rad = 3.14159f * faceAngle_deg / 180.0f;
	GLboolean movable = false;
private:
	std::vector<CollisionSphereObject*> * colObjects;
	std::vector<CollisionSphereObject*> collidedWith;
	void assignID();
};

