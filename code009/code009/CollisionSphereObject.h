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
private:
	std::vector<CollisionSphereObject*> * colObjects;
	void assignID();
	GLvoid updateCollisions();
};

