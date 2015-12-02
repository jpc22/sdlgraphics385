// code009.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "CollisionSphereObject.h"
//#include "Character.h"
#include "KillerRobot.h"

//=========================================================//
//=========================================================//
#define window_width  1125
#define window_height 750
SDL_Window* displayWindow;
SDL_GLContext mainGLContext;
//=========================================================//
GLfloat oldTime = 0.0f;
GLfloat timeFromStart = 0.0f;
GLfloat rotation = 0.0f;
GLfloat deltaTime = 0.0f;
//=========================================================//
// person position in the environment
//void move_camera(void);
//static void keyboard(void);
void update_camera();

GLdouble  g_playerPos[] = { 0.0, 1, 10.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 0.0;
float rad = 0;
const float DEFAULT_SPEED = 0.015f;
//=========================================================//
//=========================================================//
GLfloat change_direction = 1.0;
//=========================================================//
const int   WORLD_SIZE = 250;
static void text_onScreen(int row, int col, const char *fmt, ...);
//=========================================================//
// quadric objects

GLUquadricObj *g_normalObject = NULL;
void init_data(void);
GLvoid  DrawGround();

void setup_sceneEffects(void);
void makeSound(void);
void backgroundMusic(void);
void openingAudio(void);
void closingAudio(void);

//=========================================================//
std::vector<CollisionSphereObject*> * myColObjects;
GLfloat o_pos[] = { -0.0f, 0.0f, 0.0f };
CollisionSphereObject * sun = new CollisionSphereObject(2.0f, o_pos);
GLfloat o_pos2[] = { -10.0f, 0.0f, 0.0f };
CollisionSphereObject * earth = new CollisionSphereObject(0.5f, o_pos2);
GLfloat o_pos3[] = { -12.0f, 0.0f, 0.0f };
CollisionSphereObject * moon = new CollisionSphereObject(0.1f, o_pos3);
GLfloat o_pos4[] = { -5.0f, 0.0f, 0.0f };
CollisionSphereObject * dwarf = new CollisionSphereObject(1.0f, o_pos4);
//GLfloat o_pos3[] = { 4.0f, 2.0f, 2.0f };
//KillerRobot * myRobot = new KillerRobot(1.0f, o_pos3);
//=========================================================//
// Keydown booleans
//bool key[321];
bool shift = false;
bool camera_dwarf = false;
//smooth key events
GLfloat g_elevationAngleVel = 0.0f;
GLfloat g_viewAngleVel = 0.0f;
GLfloat g_playerPosXVel = 0.0f;
GLfloat g_playerPosYVel = 0.0f;
GLfloat g_playerPosZVel = 0.0f;
GLfloat g_playerPosXStrafeVel = 0.0f;
GLfloat g_playerPosZStrafeVel = 0.0f;
GLfloat xv = 0.0f;
GLfloat zv = 0.0f;
// Process pending events
bool events()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
		{

		}break;
		case SDL_MOUSEBUTTONDOWN:
		{

		}break;
		case SDL_MOUSEBUTTONUP:
		{

		}break;
		case SDL_KEYDOWN: 
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_g:
				{
					dwarf->g_speed = 1.0f;
				}break;
				case SDLK_3:
				{
					if (camera_dwarf) camera_dwarf = false;
					else camera_dwarf = true;
				}break;
				case SDLK_LSHIFT:
				shift = true; break;
				case SDLK_r:{
					g_elevationAngleVel = 1.0;
				}break;
				case SDLK_f: {
					g_elevationAngleVel = -1.0;
				}break;
				case SDLK_e: {
						g_viewAngleVel = 1.0f;
				}break;
				case SDLK_q: {
						g_viewAngleVel = -1.0f;
				}break;
				case SDLK_w: {	
					if (!shift)
					{
						g_playerPosXVel = 1.0f;
						g_playerPosZVel = 1.0f;
					}
					else
						zv = 1.0f;
				}break;
				case SDLK_s: {
					if (!shift)
					{
						g_playerPosXVel = -1.0f;
						g_playerPosZVel = -1.0f;
					}
					else
						zv = -1.0f;
				}break;
				case SDLK_d: {
					if (!shift)
					{
						g_playerPosXStrafeVel = 1.0f;
						g_playerPosZStrafeVel = 1.0f;
					}
					else
					xv = 1.0f;
				}break;
				case SDLK_a: {
					if (!shift)
					{
						g_playerPosXStrafeVel = -1.0f;
						g_playerPosZStrafeVel = -1.0f;
					}
					else
					xv = -1.0f;
				}break;
				case SDLK_SPACE: {
					g_playerPosYVel = 1.0f;
				}break;
				case SDLK_LCTRL: {
					g_playerPosYVel = -1.0f;
				}break;
			}
		}
		break;
		case SDL_KEYUP:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LSHIFT:
			{
				shift = false;
				xv = 0.0f;
				zv = 0.0f;
			}
				 break;
			case SDLK_r: {		
				g_elevationAngleVel = 0.0f;
			}break;				
			case SDLK_f: {
				g_elevationAngleVel = 0.0f;
			}break;
			case SDLK_e: {
				g_viewAngleVel = 0.0f;
			}break;
			case SDLK_q: {
				g_viewAngleVel = 0.0f;
			}break;
			case SDLK_w: {
				g_playerPosXVel = 0.0f;
				g_playerPosZVel = 0.0f;
				zv = 0.0f;
			}break;
			case SDLK_s: {
				g_playerPosXVel = 0.0f;
				g_playerPosZVel = 0.0f;
				zv = 0.0f;
			}break;
			case SDLK_d: {
				g_playerPosXStrafeVel = 0.0f;
				g_playerPosZStrafeVel = 0.0f;
				xv = 0.0f;
			}break;
			case SDLK_a: {
				g_playerPosXStrafeVel = 0.0f;
				g_playerPosZStrafeVel = 0.0f;
				xv = 0.0f;
			}break;
			case SDLK_SPACE: {
				g_playerPosYVel = 0.0f;
			}break;
			case SDLK_LCTRL: {
				g_playerPosYVel = 0.0f;
			}break;
			}
		}
		break;
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

//=========================================================//
//=========================================================//

  //=========================================================//
  //=========================================================//
void update_camera()
{
	g_elevationAngle += g_elevationAngleVel * DEFAULT_SPEED * deltaTime * 2.0;
	g_viewAngle += g_viewAngleVel * DEFAULT_SPEED * deltaTime * 2.0;
	// calculate camera rotation angle radians
	rad = float(3.14159 * g_viewAngle / 180.0f);
	g_playerPos[2] += g_playerPosZVel * sin(rad) * DEFAULT_SPEED * deltaTime;
	g_playerPos[1] += g_playerPosYVel * DEFAULT_SPEED * deltaTime / 2.5;
	g_playerPos[0] += g_playerPosXVel * cos(rad) * DEFAULT_SPEED * deltaTime;
	g_playerPos[2] += g_playerPosZStrafeVel * sin(rad + 3.14159 / 2) * DEFAULT_SPEED * deltaTime;
	g_playerPos[0] += g_playerPosXStrafeVel * cos(rad + 3.14159 / 2) * DEFAULT_SPEED * deltaTime;
	// don't allow the player to wander past the "edge of the world"
	if (g_playerPos[0] < -(WORLD_SIZE - 50))
		g_playerPos[0] = -(WORLD_SIZE - 50);
	if (g_playerPos[0] > (WORLD_SIZE - 50))
		g_playerPos[0] = (WORLD_SIZE - 50);
	if (g_playerPos[2] < -(WORLD_SIZE - 50))
		g_playerPos[2] = -(WORLD_SIZE - 50);
	if (g_playerPos[2] > (WORLD_SIZE - 50))
		g_playerPos[2] = (WORLD_SIZE - 50);

	if (camera_dwarf)
	{
		g_lookAt[0] = dwarf->g_pos[0];
		g_lookAt[2] = dwarf->g_pos[2];
		g_lookAt[1] = dwarf->g_pos[1];
	}

	else
	{
		// use the players view angle to correctly set up the view matrix
		g_lookAt[0] = float(g_playerPos[0] + 100 * cos(rad));
		g_lookAt[2] = float(g_playerPos[2] + 100 * sin(rad));

		rad = float(3.14159 * g_elevationAngle / 180.0f);

		g_lookAt[1] = float(g_playerPos[1] + 100 * sin(rad));
	}
	

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

//=========================================================//
//=========================================================//
void update()
{
	/*
	for (int i = 0; i < myColObjects->size(); i++)
	{
		myColObjects->at(i)->update();
	}
	*/
	
	if (shift)
	{
		dwarf->g_pos[0] += xv * DEFAULT_SPEED * deltaTime;
		dwarf->g_pos[2] += zv * DEFAULT_SPEED * deltaTime;
	}

	sun->update();
	earth->update();
	moon->update();
	dwarf->update();

	update_camera();
}
//=========================================================//
//=========================================================//
static void display(void)
{
	//keyboard();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);

	// position the light
	GLfloat pos[4] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	/*
	for (int i = 0; i < myColObjects->size(); i++)
	{
		myColObjects->at(i)->draw();
	}
	*/	
	sun->draw();
	earth->draw();
	moon->draw();
	dwarf->draw();
	glColor3d(0.1, 0.1, 0.4);


	DrawGround();

	SDL_GL_SwapWindow(displayWindow);
	//SDL_GL_SwapBuffers();
}
//=========================================================//
//=========================================================//
void main_loop_function()
{
	float angle = 0.0f;
	while (events())
	{
		//frame independent animations and movement
		timeFromStart = SDL_GetTicks();
		// rotation is used for animation
		rotation = fmod((timeFromStart / 100.0f), 360.0f);
		// deltaTime is used for smooth movement speed at different framerate
		deltaTime = timeFromStart - oldTime;
		oldTime = timeFromStart;

		
		update();
		

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
	//backgroundMusic();
	//Mix_PlayChannel(-1, wav, 0);
}
//=========================================================//
//=========================================================//
int main(int argc, char *argv[])
{
	myColObjects = new std::vector<CollisionSphereObject *>();
	myColObjects->push_back(sun);
	myColObjects->push_back(earth);
	myColObjects->push_back(moon);
	myColObjects->push_back(dwarf);
	for (int i = 0; i < myColObjects->size(); i++)
	{
		myColObjects->at(i)->setObjects(myColObjects);
		//std::cout << myColObjects->at(i)->id << std::endl;
	}
	

	// Initialize SDL with best video mode
	SDL_Init(SDL_INIT_VIDEO);
	/*
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
	*/

	displayWindow = SDL_CreateWindow("", 0, 0, window_width, window_height, SDL_WINDOW_OPENGL|SDL_WINDOW_BORDERLESS);

	mainGLContext = SDL_GL_CreateContext(displayWindow);

	init_data();
	openingAudio();

	//Not used in SDL2!
	//
	//const SDL_VideoInfo* info = SDL_GetVideoInfo();
	//int vidFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
	//if (info->hw_available) { vidFlags |= SDL_HWSURFACE; }
	//else { vidFlags |= SDL_SWSURFACE; }
	//int bpp = info->vfmt->BitsPerPixel;
	//SDL_SetVideoMode(window_width, window_height, bpp, vidFlags);

	GL_Setup(window_width, window_height);
	glutInit(&argc, argv);
	glewInit();

	// environment background color
	glClearColor(0.0, 0.0, 0.0, 1);//(1,1,1,1);
								   // deepth efect to objects
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// light and material in the environment
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	main_loop_function();
	closingAudio();
	SDL_GL_DeleteContext(mainGLContext);
	SDL_DestroyWindow(displayWindow);
	return 0;
}
//=========================================================//
//=========================================================//

