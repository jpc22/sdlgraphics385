// code009.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

//=========================================================//
//=========================================================//
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <windows.h>					// included in all Windows apps
#include <winuser.h>          // Windows constants
#undef main
//=========================================================//
//=========================================================//
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//=========================================================//
//=========================================================//
#define window_width  1840
#define window_height 1000
//=========================================================//
//=========================================================//
// person position in the environment
void move_camera(void);
static void keyboard(void);
void update_camera();

GLdouble  g_playerPos[] = { 0.0, 1, 10.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 0.0;
GLfloat   change_collor = 1.0;
float rad = 0;
const float DEFAULT_SPEED = 0.4f;
//=========================================================//
//=========================================================//
// Collision detection
GLfloat p1_radius = 0.3f;
GLfloat p2_radius = 0.3f;
GLfloat p1_x = -2.0f;
GLfloat p2_x = 2.0f;
GLfloat p1_y = 2.0f;
GLfloat p2_y = 2.0f;
GLfloat p1_z = 5.0f;
GLfloat p2_z = 5.0f;
GLfloat change_direction = 1.0;
//=========================================================//
const int   WORLD_SIZE = 250;
static void text_onScreen(int row, int col, const char *fmt, ...);
//=========================================================//
// quadric objects

GLUquadricObj *g_normalObject = NULL;
void init_data(void);
GLvoid  DrawGround();
GLvoid  DrawNormalObjects(GLfloat rotation);

void setup_sceneEffects(void);
void makeSound(void);
void backgroundMusic(void);
void openingAudio(void);
void closingAudio(void);

//=========================================================//
//=========================================================//
// Keydown booleans
bool key[321];
// Process pending events
bool events()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN: key[event.key.keysym.sym] = true;   break;
		case SDL_KEYUP: key[event.key.keysym.sym] = false;   break;
		case SDL_QUIT: return false; break;
		}
	}
	return true;
}
//=========================================================//
//=========================================================//
static void text_onScreen(int row, int col, const char *fmt, ...)
{
	static char buf[256];
	int viewport[4];
	va_list args;

	va_start(args, fmt);
	(void)vsprintf_s(buf, fmt, args);
	va_end(args);

	glGetIntegerv(GL_VIEWPORT, viewport);

	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, viewport[2], 0, viewport[3], -1, 1);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
//=========================================================//
//=========================================================//
void setup_sceneEffects(void)
{
	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// enable using glColor to change material properties
	// we'll use the default glColorMaterial setting (ambient and diffuse)
	glEnable(GL_COLOR_MATERIAL);

	// set the default blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set up the fog parameters for reflections
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, -100.0);
	glFogf(GL_FOG_END, 100.0);

	// enable line anti-aliasing and make the lines slightly bigger than default
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.5f);
}
//=========================================================//
//=========================================================//
void init_data(void)
{
	setup_sceneEffects();
	// create a normal quadric (uses default settings)
	g_normalObject = gluNewQuadric();

}
//=========================================================//
//=========================================================//
GLvoid DrawGround()
{ // enable blending for anti-aliased lines
	glEnable(GL_BLEND);

	// set the color to a bright blue
	glColor3f(0.5f, 0.7f, 1.0f);

	// draw the lines
	glBegin(GL_LINES);
	for (int x = -WORLD_SIZE; x < WORLD_SIZE; x += 6)
	{
		glVertex3i(x, 0, -WORLD_SIZE);
		glVertex3i(x, 0, WORLD_SIZE);
	}

	for (int z = -WORLD_SIZE; z < WORLD_SIZE; z += 6)
	{
		glVertex3i(-WORLD_SIZE, 0, z);
		glVertex3i(WORLD_SIZE, 0, z);
	}
	glEnd();

	// turn blending off
	glDisable(GL_BLEND);
} // end DrawGround()
  //=========================================================//
  //=========================================================//
GLvoid drawCollision()
{
	GLfloat alphaTransparency = 0.5;
	GLfloat distance;

	// move forward and backward
	p1_x += 0.05f*change_direction;
	p2_x -= 0.05f*change_direction;

	if (p1_x > 2) {
		change_direction = 1.0f - 2.0f;
	}
	if (p1_x < -2) {
		change_direction = 1.0f;
	}
	// check-collision
	distance = sqrt(((p1_x - p2_x) * (p1_x - p2_x))
		+ ((p1_y - p2_y) * (p1_y - p2_y))
		+ ((p1_z - p2_z) * (p1_z - p2_z)));
	if (distance <= p2_radius + p1_radius) {
		// Red :: collision
		change_collor = 0.0;
		makeSound();
	}
	else {
		// Yellow :: no collision
		change_collor = 1.0;
	}
	// enable blending for transparent sphere
	glEnable(GL_BLEND);     // Turn Blending On
	glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// first sphere collides against the other
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor4f(1.0f, change_collor, 0.0f, alphaTransparency);
	glTranslatef(p1_x, p1_y, p1_z);
	gluSphere(g_normalObject, p1_radius, 16, 10);
	glPopMatrix();
	// second sphere collides against the first
	//
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor4f(1.0f, change_collor, 0.0f, alphaTransparency);
	glTranslatef(p2_x, p2_y, p2_z);
	gluSphere(g_normalObject, p2_radius, 16, 10);
	glPopMatrix();

	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
}
//=========================================================//
//=========================================================//
GLvoid DrawNormalObjects(GLfloat rotation)
{
	// make sure the random color values we get are the same every time
	srand(200);

	// save the existing color properties
	glPushAttrib(GL_CURRENT_BIT);

	drawCollision();

	// a cylinder
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor3f(1, 1, 1);
	glTranslatef(-25.0, 0.0, -4.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(g_normalObject, 1.0, 1.0, 3.0, 32, 4);
	glPopMatrix();

	// a tapered cylinder
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor3f(1, 0, 0);
	glTranslatef(5.0, 0.0, 3.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(g_normalObject, 2.0, 0.5, 2.0, 32, 4);
	glPopMatrix();

	// a disk with a hole in it
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor3f(1, 1, 0);
	glTranslatef(50.0, 1.5, -20.0);
	glRotatef(rotation * 10.0f, 0.0, 1.0, 0.0);
	gluDisk(g_normalObject, 0.7, 1.0, 32, 4);
	glPopMatrix();

	// a Cone
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor3f(0, 1, 0);
	glTranslatef(-150.0, 0.5, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(g_normalObject, 1.0, 0.0, 2.0, 32, 2);
	glPopMatrix();

	// a bouncing sphere
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor3f(1, 0, 1);

	// use a sine wave pattern to make the sphere look like it's bouncing
	glTranslatef(5.0f, 3.0f + (float)sin(float(3.14159f*rotation / 90.0f)), 50.0f);
	gluSphere(g_normalObject, 2.0f, 32, 20);
	glPopMatrix();

	// restore the previous color values
	glPopAttrib();
} // end DrawNormalObjects()
  //=========================================================//
  //=========================================================//
void update_camera()
{
	// don't allow the player to wander past the "edge of the world"
	if (g_playerPos[0] < -(WORLD_SIZE - 50))
		g_playerPos[0] = -(WORLD_SIZE - 50);
	if (g_playerPos[0] > (WORLD_SIZE - 50))
		g_playerPos[0] = (WORLD_SIZE - 50);
	if (g_playerPos[2] < -(WORLD_SIZE - 50))
		g_playerPos[2] = -(WORLD_SIZE - 50);
	if (g_playerPos[2] > (WORLD_SIZE - 50))
		g_playerPos[2] = (WORLD_SIZE - 50);

	// calculate the player's angle of rotation in radians
	float rad = float(3.14159 * g_viewAngle / 180.0f);
	// use the players view angle to correctly set up the view matrix
	g_lookAt[0] = float(g_playerPos[0] + 100 * cos(rad));
	g_lookAt[2] = float(g_playerPos[2] + 100 * sin(rad));

	rad = float(3.13149 * g_elevationAngle / 180.0f);

	g_lookAt[1] = float(g_playerPos[1] + 100 * sin(rad));

	// clear the modelview matrix
	glLoadIdentity();

	// setup the view matrix
	gluLookAt(g_playerPos[0], g_playerPos[1], g_playerPos[2],
		g_lookAt[0], g_lookAt[1], g_lookAt[2],
		0.0, 1.0, 0.0);
}
//=========================================================//
//=========================================================//
//void move_camera(int specialKEY,char normalKEY)
void move_camera(void)
{
	// looking up
	if (key[SDLK_a]) {
		g_elevationAngle += 2.0;
	}
	// looking down
	if (key[SDLK_z]) {
		g_elevationAngle -= 2.0;
	}

	if (key[SDLK_RIGHT]) {
		g_viewAngle += 2.0;
		// calculate camera rotation angle radians
		rad = float(3.14159 * g_viewAngle / 180.0f);
	}
	if (key[SDLK_LEFT]) {
		g_viewAngle -= 2.0;
		// calculate camera rotation angle radians
		rad = float(3.14159 * g_viewAngle / 180.0f);
	}
	if (key[SDLK_UP]) {
		g_playerPos[2] += sin(rad) * DEFAULT_SPEED;
		g_playerPos[0] += cos(rad) * DEFAULT_SPEED;
	}
	if (key[SDLK_DOWN]) {
		g_playerPos[2] -= sin(rad) * DEFAULT_SPEED;
		g_playerPos[0] -= cos(rad) * DEFAULT_SPEED;
	}
}
//=========================================================//
//=========================================================//
//static void keyboard(unsigned char key, int x, int y)
static void keyboard(void)
{
	move_camera();
}
//=========================================================//
//=========================================================//
static void display(void)
{
	keyboard();
	update_camera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);

	// position the light
	GLfloat pos[4] = { 5.0, 5.0, 5.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	// rotation is used for animation
	static GLfloat rotation = 0.0;
	// it's increased by one every frame
	rotation += 1.0;
	// and ranges between 0 and 360
	if (rotation > 360.0)
		rotation = 0.0;
	// draw all of our objects in their normal position
	DrawNormalObjects(rotation);

	glColor3d(0.1, 0.1, 0.4);

	text_onScreen(0, 3, "Prof. Adriano Cavalcanti / A385 Computer Graphics / UAA");
	text_onScreen(2, 3, "- look up or down: A / Z");
	text_onScreen(3, 3, "- look right/left: arrows ->/<-");
	text_onScreen(4, 3, "- walk forward/backward: arrows UP/Down");

	DrawGround();

	//SDL_GL_SwapBuffers();
}
//=========================================================//
//=========================================================//
void main_loop_function()
{
	float angle = 0.0f;
	while (events())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(0, 0, -10);
		glRotatef(angle, 0, 0, 1);
		glBegin(GL_QUADS);
		glColor3ub(255, 000, 000); glVertex2f(-1, 1);
		glColor3ub(000, 255, 000); glVertex2f(1, 1);
		glColor3ub(000, 000, 255); glVertex2f(1, -1);
		glColor3ub(255, 255, 000); glVertex2f(-1, -1);
		glEnd();

		display();
	}
}
//=========================================================//
//=========================================================//
// Initialze OpenGL perspective matrix
void GL_Setup(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	gluPerspective(45, (float)width / height, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}
//=========================================================//
//=========================================================//
// Sound variables
int channel;				//Channel on which our sound is played
int audio_rate = 22050;			//Frequency of audio playback
Uint16 audio_format = AUDIO_S16SYS; 	//Format of the audio we're playing
int audio_channels = 2;			//2 channels = stereo
int audio_buffers = 640;		//Size of the audio buffers in memory
Mix_Music *mus, *mus2;  // Background Music
Mix_Chunk *wav, *wav2;  // For Sounds
						//=========================================================//
						//=========================================================//
void openingAudio(void) {
	//Initialize SDL_mixer with our chosen audio settings
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}
	mus = Mix_LoadMUS("res/ff2prlde.mid");
	mus2 = Mix_LoadMUS("res/aria.mp3");
	wav = Mix_LoadWAV("res/po_p2k.wav");
	wav2 = Mix_LoadWAV("res/start.wav");
}
//=========================================================//
//=========================================================//
void closingAudio(void) {
	//Release the memory allocated to our sound
	Mix_FreeChunk(wav);
	Mix_FreeChunk(wav2);
	Mix_FreeMusic(mus);
	Mix_FreeMusic(mus2);
	//Need to make sure that SDL_mixer and SDL have a chance to clean up
	Mix_CloseAudio();
}
//=========================================================//
//=========================================================//
void backgroundMusic(void) {
	Mix_PlayMusic(mus, 1); //Music loop=1
}
//=========================================================//
//=========================================================//
void makeSound(void) {
	backgroundMusic();
	Mix_PlayChannel(-1, wav, 0);
}
//=========================================================//
//=========================================================//
int main()
{
	// Initialize SDL with best video mode
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error:" << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	std::string imagePath = "res/hello.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
	if (bmp == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	//A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
	for (int i = 0; i < 3; ++i) {
		//First clear the renderer
		SDL_RenderClear(ren);
		//Draw the texture
		SDL_RenderCopy(ren, tex, NULL, NULL);
		//Update the screen
		SDL_RenderPresent(ren);
		//Take a quick break after all that hard work
		SDL_Delay(1000);
	}
	std::cout << "complete" << std::endl;

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	//init_data();
	//openingAudio();

	//Not used in SDL2!
	//
	//const SDL_VideoInfo* info = SDL_GetVideoInfo();
	//int vidFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
	//if (info->hw_available) { vidFlags |= SDL_HWSURFACE; }
	//else { vidFlags |= SDL_SWSURFACE; }
	//int bpp = info->vfmt->BitsPerPixel;
	//SDL_SetVideoMode(window_width, window_height, bpp, vidFlags);
	/*
	GL_Setup(window_width, window_height);

	// environment background color
	glClearColor(0.9, 0.9, 0.7, 1);//(1,1,1,1);
								   // deepth efect to objects
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// light and material in the environment
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	main_loop_function();
	closingAudio();
	*/
}
//=========================================================//
//=========================================================//

