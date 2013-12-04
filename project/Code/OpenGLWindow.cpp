// Author:		Vahe Karamian
// Date:			03/01/2006
// Quick Note:	OpenGL Terrain Generation Sample

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include <GL/freeglut.h>
#include "vkgllib.h"
#include "imageloader.h"
#include "md2model.h"
using namespace std;
////// Global Variables
HDC g_HDC;						         // global device context
bool fullScreen = false;		      // true = fullscreen; false = windowed
bool keyPressed[256];			      // holds true for keys that are pressed	

float angle       = 0.0f;				// camera angle 
float radians     = 0.0f;			   // camera angle in radians
bool light = true;bool texture = true; bool fog = false;
////// Mouse/Camera Variables
int mouseX, mouseY;						// mouse coordinates
float cameraX, cameraY, cameraZ;	   // camera coordinates
float lookX, lookY, lookZ;			   // camera look-at coordinates
MD2Model* _model;
MD2Model* _model2;
int _textureId;
//The forward position of the guy relative to an arbitrary floor "tile"
float _guyPos = 0;
float _guyPos2 = 0;
const float FLOOR_TEXTURE_SIZE = 80.0f; //The size of each floor "tile"
float _angle = 30.0f;
// prototypes
void Render( void );

// InitializeTerrain()
// desc: initializes the heightfield terrain data
void InitializeTerrain()
{
	// loop through all of the heightfield points, calculating
	// the coordinates for each point
	for (int z = 0; z < MAP_Z; z++)
	{
		for (int x = 0; x < MAP_X; x++)
		{
			terrain[x][z][0] = float(x)*MAP_SCALE;				
			terrain[x][z][1] = (float)imageData[(z*MAP_Z+x)*3];
			terrain[x][z][2] = -float(z)*MAP_SCALE;
		}
	}
}

void CleanUp()
{
	free(imageData);
	free(landTexture);
	delete _model;

}
//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image->width, image->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image->pixels);
	return textureId;
}
void drawCubers(){

    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    //glRotatef(rotate_y,0,1,0);
    //glRotatef(rotate_x,1,0,0);
    //glEnable(GL_TEXTURE_2D);
   // glBindTexture(GL_TEXTURE_2D,g_texture_obj[1]);
    glBegin(GL_QUADS);
    
    //Front
    glNormal3f(0.0f, 0.0f, 1.0f);
    //glTexCoord2f (0,1); 
    glVertex3f(-1.0f, -1.0f, 1.0f);
    //glTexCoord2f (1,1); 
    glVertex3f(1.0f, -1.0f, 1.0f);
    //glTexCoord2f (1,0); 
    glVertex3f(1.0f, 1.0f, 1.0f);
    //glTexCoord2f (0,0); 
    glVertex3f(-1.0f, 1.0f, 1.0f);
    
    //Right
    glNormal3f(1.0f, 0.0f, 0.0f);
    //glTexCoord2f (0,1); 
    glVertex3f(1.0f, -1.0f, -1.0f);
    //glTexCoord2f (0,0); 
    glVertex3f(1.0f, 1.0f, -1.0f);
    //glTexCoord2f (1,0); 
    glVertex3f(1.0f, 1.0f, 1.0f);
    //glTexCoord2f (1,1); 
    glVertex3f(1.0f, -1.0f, 1.0f);
    
    //Back
    glNormal3f(0.0f, 0.0f, -1.0f);
    //glTexCoord2f (0,1); 
    glVertex3f(-1.0f, -1.0f, -1.0f);
    //glTexCoord2f (0,0); 
    glVertex3f(-1.0f, 1.0f, -1.0f);
    //glTexCoord2f (1,0); 
    glVertex3f(1.0f, 1.0f, -1.0f);
    //glTexCoord2f (1,1); 
    glVertex3f(1.0f, -1.0f, -1.0f);
    
    //Left
    glNormal3f(-1.0f, 0.0f, 0.0f);
    //glTexCoord2f (0,1);    
    glVertex3f(-1.0f, -1.0f, -1.0f);
    //glTexCoord2f (1,1); 
    glVertex3f(-1.0f, -1.0f, 1.0f);
    //glTexCoord2f (1,0); 
    glVertex3f(-1.0f, 1.0f, 1.0f);
    //glTexCoord2f (0,0); 
    glVertex3f(-1.0f, 1.0f, -1.0f);
    
    glEnd();
    //glDisable (GL_TEXTURE_2D);
    glPopMatrix();
}
void drawHouse(){
        //back
        glPushMatrix();
        glTranslatef(0.0,0.0,0.0);
        glScalef(0.5f, 3.0f, 0.1f);
		drawCubers();
        glPopMatrix();

        //stilt1
        glPushMatrix();
        glColor3f(0, 0.5, 1);
        glTranslatef(0.5,0.0,0.0);
        glScalef(0.1f,3.2f, 0.1f);
        drawCubers();
        glPopMatrix();

        //stilt2
        glPushMatrix();
        glColor3f(0, 0.5, 1);
        glTranslatef(-0.5,0.0,0.0);
        glScalef(0.1f,3.2f, 0.1f);
        drawCubers();
        glPopMatrix();

        //stilt3
        glPushMatrix();
        glColor3f(0, 0.5, 1);
        glTranslatef(0.5,0.0,-1.0);
        glScalef(0.1f,3.2f, 0.1f);
        drawCubers();
        glPopMatrix();

        //stilt4
        glPushMatrix();
        glColor3f(0, 0.5, 1);
        glTranslatef(-0.5,0.0,-1.0);
        glScalef(0.1f,3.2f, 0.1f);
        drawCubers();
        glPopMatrix();

        //side
        glPushMatrix();
        glColor3f(0, 1, 1);
        glTranslatef(-0.5,0.0,-0.5);
        glScalef(0.1f,3.0f, 0.9f);
        drawCubers();
        glPopMatrix();

        //side
        glPushMatrix();
        glColor3f(0, 1, 1);
        glTranslatef(0.5,0.0,-0.5);
        glScalef(0.1f,3.0f, 0.9f);
        drawCubers();
        glPopMatrix();

        //front
        glPushMatrix();
        glColor3f(0, 1, 1);
        glTranslatef(0.0,0.0,-1.0);
        glScalef(0.9f,3.0f, 0.1f);
        drawCubers();
        glPopMatrix();

        //ceiling
        glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(0.5f,1.6f,-0.5f);
        glRotatef(45, 0.0f, 0.0f, 1.0f);
        glScalef(0.1f,1.315f, 0.9f);
        drawCubers();
        glPopMatrix();

        glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(-0.5f,1.6f,-0.5f);
        glRotatef(-45, 0.0f, 0.0f, 1.0f);
        glScalef(0.1f,1.55f, 0.9f);
        drawCubers();
        glPopMatrix();

        //Ceiling triangles
        glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(-0.5f,1.6f,-0.5f);
        glRotatef(-45, 0.0f, 0.0f, 1.0f);
        glScalef(0.1f,1.55f, 0.9f);
        glBegin(GL_TRIANGLES);

        glEnd();
        glPopMatrix();

}
void makeOuthouse(){
        //back
        glPushMatrix();
        glTranslatef(0.0,0.0,0.0);
        glScalef(0.9f, 3.0f, 0.1f);
         drawCubers();
        glPopMatrix();

        //stilt1
        glPushMatrix();
        glColor3f(0, 0.5, 1);
        glTranslatef(0.5,0.0,0.0);
        glScalef(0.1f,3.2f, 0.1f);
        drawCubers();
        glPopMatrix();

        //stilt2
        glPushMatrix();
        glColor3f(0, 0.5, 1);
        glTranslatef(-0.5,0.0,0.0);
        glScalef(0.1f,3.2f, 0.1f);
        drawCubers();
        glPopMatrix();

        //stilt3
        glPushMatrix();
        glColor3f(0, 0.5, 1);
        glTranslatef(0.5,0.0,-1.0);
        glScalef(0.1f,3.2f, 0.1f);
        drawCubers();
        glPopMatrix();

        //stilt4
        glPushMatrix();
        glColor3f(0, 0.5, 1);
        glTranslatef(-0.5,0.0,-1.0);
        glScalef(0.1f,3.2f, 0.1f);
        drawCubers();
        glPopMatrix();

        //side
        glPushMatrix();
        glColor3f(0, 1, 1);
        glTranslatef(-0.5,0.0,-0.5);
        glScalef(0.1f,3.0f, 0.9f);
        drawCubers();
        glPopMatrix();

        //side
        glPushMatrix();
        glColor3f(0, 1, 1);
        glTranslatef(0.5,0.0,-0.5);
        glScalef(0.1f,3.0f, 0.9f);
        drawCubers();
        glPopMatrix();

        //front
        glPushMatrix();
        glColor3f(0, 1, 1);
        glTranslatef(0.0,0.0,-1.0);
        glScalef(0.9f,3.0f, 0.1f);
        drawCubers();
        glPopMatrix();

        //ceiling
        glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(0.5f,1.6f,-0.5f);
        glRotatef(45, 0.0f, 0.0f, 1.0f);
        glScalef(0.1f,1.315f, 0.9f);
        drawCubers();
        glPopMatrix();

        glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(-0.5f,1.6f,-0.5f);
        glRotatef(-45, 0.0f, 0.0f, 1.0f);
        glScalef(0.1f,1.55f, 0.9f);
        drawCubers();
        glPopMatrix();

        //Ceiling triangles
        glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(-0.5f,1.6f,-0.5f);
        glRotatef(-45, 0.0f, 0.0f, 1.0f);
        glScalef(0.1f,1.55f, 0.9f);
        glBegin(GL_TRIANGLES);

        glEnd();
        glPopMatrix();
}
// Initialize
// desc: initializes OpenGL
void Initialize()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// clear to black
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //smooth sahdng mode
	glEnable(GL_FOG);
	glShadeModel(GL_SMOOTH);					   // use smooth shading
	glEnable(GL_DEPTH_TEST);					   // hidden surface removal
	glEnable(GL_CULL_FACE);						   // do not calculate inside of poly's
	glFrontFace(GL_CCW);						      // counter clock-wise polygons are out

	glEnable(GL_TEXTURE_2D);					   // enable 2D texturing

	imageData = LoadBitmapFile("terrain2.bmp", &bitmapInfoHeader);

	// initialize the terrain data and load the textures
	InitializeTerrain();
	LoadTextures();
		//Load the model
	_model = MD2Model::load("tallguy.md2");
	if (_model != NULL) {
		_model->setAnimation("run");
	}
		//Load the model
	_model2 = MD2Model::load("tallguy.md2");
	if (_model2 != NULL) {
		_model2->setAnimation("run");
	}
}

//void lightDisplay(bool light){
//	if (light== false)             // If Not Light
//    {
//        glDisable(GL_LIGHTING);     // Disable Lighting
//    }
//    else                    // Otherwise
//    {
//        glEnable(GL_LIGHTING);      // Enable Lighting
//    }
//	
//	//glEnable(GL_LIGHTING); //Enable lighting
//	glEnable(GL_LIGHT0); //Enable light #0
//	glEnable(GL_COLOR_MATERIAL); //Enable material
//    
//	//Add point source light positioned at (5.0, 5.0, -10.0)
//	GLfloat light_Pos0[4] = {55.0, 55.0, 50.0, 1.0}; 
//	GLfloat light_Ka0[4] = {1.0, 0.6, 0.1, 1.0};  //ambient light parameters
//	GLfloat light_Kd0[4] = {0.8, 0.8, 0.8, 1.0}; //diffuse light parameters
//	GLfloat light_Ks0[4] = {0.9, 0.9, 0.9, 1.0}; //specular light parameters
//	glLightfv(GL_LIGHT0, GL_POSITION, light_Pos0);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka0);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd0);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks0);
//}

GLfloat light_Pos0[4] = {800, 400, /*-300*/0, 1.0}; //the position of light 0
GLfloat transformed_light_position[4] = {light_Pos0[0] - 250, light_Pos0[1], light_Pos0[2] - 250, light_Pos0[3]};

float lightRotationAngle = 0;
void updateLightPosition()
{
	lightRotationAngle += 0.005;
	if(lightRotationAngle > 360)
	{
		lightRotationAngle = 0;
	}
	//cout << "update position called! the position is now " << light_Pos0[0] << " " << light_Pos0[1] << " " << light_Pos0[2] << " " << light_Pos0[3] << endl;
	
	light_Pos0[1] = sin(lightRotationAngle) * 400;
	light_Pos0[0] = cos(lightRotationAngle) * 800;
	//cout << "after the update: " << light_Pos0[0] << " " << light_Pos0[1] << " " << light_Pos0[2] << " " << light_Pos0[3] << endl;
	transformed_light_position[0] = light_Pos0[0] + 325;
	transformed_light_position[1] = light_Pos0[1] + 100;
	transformed_light_position[2] = light_Pos0[2] - 300;
	transformed_light_position[3] = light_Pos0[3];
}

int lightTimer = 0;
void lightDisplay(bool light){
	if (light== false)             // If Not Light
    {
        glDisable(GL_LIGHTING);     // Disable Lighting
    }
    else                    // Otherwise
    {
        glEnable(GL_LIGHTING);      // Enable Lighting
    }
	
	//glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_COLOR_MATERIAL); //Enable material
    
	//Add point source light positioned at (5.0, 5.0, -10.0)
	/*GLfloat light_Pos0[4] = {55.0, 55.0, 50.0, 1.0}; */

	//experimental code------------------------------------------------------------------------------
	lightTimer++;
	if(lightTimer > 0)
	{
		updateLightPosition();
		lightTimer = 0;
	}

	GLfloat light_Ka0[4] = /*{1.0, 0.6, 0.1, 1.0}*/ {0.2, 0.2, 0.2, 0.2};  //ambient light parameters
	GLfloat light_Kd0[4] = /*{0.8, 0.8, 0.8, 1.0}*/ {1, 1, 1, 1}; //diffuse light parameters
	GLfloat light_Ks0[4] = {0.9, 0.9, 0.9, 1.0}; //specular light parameters
	//GLfloat transformed_light_position[4] = {light_Pos0[0] - 250, light_Pos0[1], light_Pos0[2] - 250, light_Pos0[3]};
	glLightfv(GL_LIGHT0, GL_POSITION, transformed_light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks0);
}

void fogDisplay(bool fog){
	if(fog)
	{
		glEnable(GL_FOG);
	}else
		glDisable(GL_FOG);
	GLfloat fogColor[4] = {.7, 0.7, 0.7, 0.5};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP );
	glFogf(GL_FOG_DENSITY, 0.002f );
}

void resetKey()
{
	fog = true;
	light = true;
}
// function to set the pixel format for the device context
void SetupPixelFormat(HDC hDC)
{
	int nPixelFormat;					      // our pixel format index

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// size of structure
		1,								         // default version
		PFD_DRAW_TO_WINDOW |			      // window drawing support
		PFD_SUPPORT_OPENGL |			      // OpenGL support
		PFD_DOUBLEBUFFER,				      // double buffering support
		PFD_TYPE_RGBA,					      // RGBA color mode
		32,								      // 32 bit color mode
		0, 0, 0, 0, 0, 0,				      // ignore color bits, non-palettized mode
		0,								         // no alpha buffer
		0,								         // ignore shift bit
		0,								         // no accumulation buffer
		0, 0, 0, 0,						      // ignore accumulation bits
		16,								      // 16 bit z-buffer size
		0,								         // no stencil buffer
		0,								         // no auxiliary buffer
		PFD_MAIN_PLANE,				      // main drawing plane
		0,								         // reserved
		0, 0, 0 };						      // layer masks ignored

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);	// choose best matching pixel format

	SetPixelFormat(hDC, nPixelFormat, &pfd);		// set pixel format to device context
}
// the Windows Procedure event handler
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;					   // rendering context
	static HDC hDC;						// device context
	int width, height;					// window width and height
	int oldMouseX, oldMouseY;

	switch(message)
	{
		case WM_CREATE:					// window is being created

			hDC = GetDC(hwnd);			// get current window's device context
			g_HDC = hDC;
			SetupPixelFormat(hDC);		// call our pixel format setup function

			// create rendering context and make it current
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);

			return 0;
			break;

		case WM_CLOSE:					   // windows is closing

			// deselect rendering context and delete it
			wglMakeCurrent(hDC, NULL);
			wglDeleteContext(hRC);

			// send WM_QUIT to message queue
			PostQuitMessage(0);

			return 0;
			break;

		case WM_SIZE:
			height = HIWORD(lParam);		      // retrieve width and height
			width = LOWORD(lParam);

			if (height==0)					         // don't want a divide by zero
			{
				height=1;					
			}

			glViewport(0, 0, width, height);	   // reset the viewport to new dimensions
			glMatrixMode(GL_PROJECTION);		   // set projection matrix current matrix
			glLoadIdentity();					      // reset projection matrix

			// calculate aspect ratio of window
			gluPerspective(54.0f,(GLfloat)width/(GLfloat)height,1.0f,1000.0f);

			glMatrixMode(GL_MODELVIEW);			// set modelview matrix
			glLoadIdentity();					      // reset modelview matrix

			return 0;
			break;

		case WM_KEYDOWN:					         // is a key pressed?
			keyPressed[wParam] = true;
			return 0;
			break;

		case WM_KEYUP:
			keyPressed[wParam] = false;
			return 0;
			break;

		case WM_MOUSEMOVE:
			// save old mouse coordinates
			oldMouseX = mouseX;
			oldMouseY = mouseY;

			// get mouse coordinates from Windows
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);

			// these lines limit the camera's range
			if (mouseY < 200)
				mouseY = 200;
			if (mouseY > 450)
				mouseY = 450;

			if ((mouseX - oldMouseX) > 0)		      // mouse moved to the right
				angle += 3.0f;
			else if ((mouseX - oldMouseX) < 0)	   // mouse moved to the left
				angle -= 3.0f;

			return 0;
			break;
		default:
			break;
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}

// the main windows entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX windowClass;		// window class
	HWND	   hwnd;			      // window handle
	MSG		   msg;				// message
	bool	   done;			      // flag saying when our app is complete
	DWORD	   dwExStyle;		   // Window Extended Style
	DWORD	   dwStyle;			   // Window Style
	RECT	   windowRect;

	// temp var's
	int width = 800;
	int height = 600;
	int bits = 32;
	//fullScreen = TRUE;
	windowRect.left=(long)0;						// Set Left Value To 0
	windowRect.right=(long)width;					// Set Right Value To Requested Width
	windowRect.top=(long)0;							// Set Top Value To 0
	windowRect.bottom=(long)height;				// Set Bottom Value To Requested Height

	// fill out the window class structure
	windowClass.cbSize			= sizeof(WNDCLASSEX);
	windowClass.style			   = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc		= WndProc;
	windowClass.cbClsExtra		= 0;
	windowClass.cbWndExtra		= 0;
	windowClass.hInstance		= hInstance;
	windowClass.hIcon			   = LoadIcon(NULL, IDI_APPLICATION);	// default icon
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);		// default arrow
	windowClass.hbrBackground	= NULL;								      // don't need background
	windowClass.lpszMenuName	= NULL;								      // no menu
	windowClass.lpszClassName	= "MyClass";
	windowClass.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);		// windows logo small icon

	// register the windows class
	if (!RegisterClassEx(&windowClass))
		return 0;

	if (fullScreen)								                  // fullscreen?
	{
		DEVMODE dmScreenSettings;					               // device mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);	
		dmScreenSettings.dmPelsWidth = width;		            // screen width
		dmScreenSettings.dmPelsHeight = height;		         // screen height
		dmScreenSettings.dmBitsPerPel = bits;		            // bits per pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// 
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// setting display mode failed, switch to windowed
			MessageBox(NULL, "Display mode failed", NULL, MB_OK);
			fullScreen=FALSE;	
		}
	}

	if (fullScreen)								      // Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;				      // Window Extended Style
		dwStyle=WS_POPUP;						         // Windows Style
		ShowCursor(FALSE);						      // Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;					      // Windows Style
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// class registered, so now create our window
	hwnd = CreateWindowEx(NULL,							   // extended style
						  "MyClass",							   // class name
						  WINDOW_TITLE,	                  // Window Title
						  dwStyle | WS_CLIPCHILDREN |
						  WS_CLIPSIBLINGS,
						  0, 0,									      // x,y coordinate
						  windowRect.right - windowRect.left,
						  windowRect.bottom - windowRect.top,	// width, height
						  NULL,									      // handle to parent
						  NULL,									      // handle to menu
						  hInstance,							      // application instance
						  NULL);								         // no extra params

	// check if window creation failed (hwnd would equal NULL)
	if (!hwnd)
		return 0;

	ShowWindow(hwnd, SW_SHOW);			// display the window
	UpdateWindow(hwnd);					// update the window

	done = false;						   // intialize the loop condition variable
	Initialize();						   // initialize OpenGL
	// main message loop
	while (!done)
	{
		PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE);

		if (msg.message == WM_QUIT)		      // do we receive a WM_QUIT message?
		{
			done = true;				            // if so, time to quit the application
		}
		else
		{	
				//Advance the animation
				if (_model != NULL) {
					_model->advance(0.025f);
				}	
				//Update _guyPos
				_guyPos += 0.08f;
				while (_guyPos > FLOOR_TEXTURE_SIZE) {
					_guyPos -= FLOOR_TEXTURE_SIZE;
				}
				//Advance the animation
				if (_model2 != NULL) {
					_model2->advance(0.025f);
				}	
				//Update _guyPos
				_guyPos2 += 0.08f;
				while (_guyPos2 > FLOOR_TEXTURE_SIZE) {
					_guyPos2 -= FLOOR_TEXTURE_SIZE;
				}
			if(keyPressed[VK_RIGHT])
			{
					light = false;				
			}
			if(keyPressed[VK_LEFT])
			{
					fog = false;				
			}
			if(keyPressed[VK_DOWN])
			{
				resetKey();				
			}
			if (keyPressed[VK_ESCAPE])
				done = true;
			else 
			{
				Render();
				TranslateMessage(&msg);		      // translate and dispatch to event queue
				DispatchMessage(&msg);
			}

		}
	}

	CleanUp();

	if (fullScreen)
	{
		ChangeDisplaySettings(NULL,0);		// If So Switch Back To The Desktop
		ShowCursor(TRUE);					      // Show Mouse Pointer
	}

	return msg.wParam;
}

// Render
// desc: handles drawing of scene
void Render()
{
	radians =  float(PI*(angle-90.0f)/180.0f);
	// calculate the camera's position
	cameraX = lookX + sin(radians)*mouseY*2;	   // multiplying by mouseY makes the
	cameraZ = lookZ + cos(radians)*mouseY*2;    // camera get closer/farther away with mouseY
	cameraY = lookY + mouseY / 2.0f;
	// calculate the camera look-at coordinates as the center of the terrain map
	lookX = (MAP_X*MAP_SCALE)/2.0f;
	lookY = 150.0f;
	lookZ = -(MAP_Z*MAP_SCALE)/2.0f;
	// clear screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	glLoadIdentity();

	// set the camera position
	gluLookAt(cameraX, cameraY, cameraZ, lookX, lookY, lookZ, 0.0, 1.0, 0.0);

	lightDisplay(light);
	fogDisplay(fog);

	if (_model != NULL) {
		glPushMatrix();
	    glTranslatef(320,170,-550+_guyPos);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
	//	glScalef(5.0f, 5.0f,5.0f);
		_model->draw();
		glPopMatrix();
	}
	if (_model2 != NULL) {
		glPushMatrix();
	    glTranslatef(320-_guyPos2,190,-550);
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
	//	glScalef(5.0f, 5.0f,5.0f);
		_model2->draw();
		glPopMatrix();
	}
	// set the current texture to the land texture
	glBindTexture(GL_TEXTURE_2D, land);
	glPushMatrix();
	glTranslatef(320,170,-200);
	glScalef(10,10,10);
	makeOuthouse();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(320,170,-130);
	glScalef(10,10,10);
	drawHouse();
	glPopMatrix();


	//Draw a cube that represents the light
	glPushMatrix();
	glTranslatef(transformed_light_position[0], transformed_light_position[1], transformed_light_position[2]);
	glScaled(100, 100, 100);
	drawCubers();
	glPopMatrix();

	//Draw a large cube in the center of the scene
	glPushMatrix();
	glScaled(300, 300, 300);
	drawCubers();
	glPopMatrix();


	// we are going to loop through all of our terrain's data points,
	// but we only want to draw one triangle strip for each set along the x-axis.
	for (int z = 0; z < MAP_Z-1; z++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < MAP_X-1; x++)
		{
			// for each vertex, we calculate the grayscale shade color, 
			// we set the texture coordinate, and we draw the vertex.	
			// draw vertex 0
			glColor3f(terrain[x][z][1]/255.0f, terrain[x][z][1]/255.0f, terrain[x][z][1]/255.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(terrain[x][z][0], terrain[x][z][1], terrain[x][z][2]);

			// draw vertex 1
			glTexCoord2f(1.0f, 0.0f);
			glColor3f(terrain[x+1][z][1]/255.0f, terrain[x+1][z][1]/255.0f, terrain[x+1][z][1]/255.0f);
			glVertex3f(terrain[x+1][z][0], terrain[x+1][z][1], terrain[x+1][z][2]);

			// draw vertex 2
			glTexCoord2f(0.0f, 1.0f);
			glColor3f(terrain[x][z+1][1]/255.0f, terrain[x][z+1][1]/255.0f, terrain[x][z+1][1]/255.0f);
			glVertex3f(terrain[x][z+1][0], terrain[x][z+1][1], terrain[x][z+1][2]);

			// draw vertex 3
			glColor3f(terrain[x+1][z+1][1]/255.0f, terrain[x+1][z+1][1]/255.0f, terrain[x+1][z+1][1]/255.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(terrain[x+1][z+1][0], terrain[x+1][z+1][1], terrain[x+1][z+1][2]);
		}
		glEnd();
	}
	// enable blending
	glEnable(GL_BLEND);

	// enable read-only depth buffer
	glDepthMask(GL_FALSE);

	// set the blend function to what we use for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// set back to normal depth buffer mode (writable)
	glDepthMask(GL_TRUE);

	// disable blending
	glDisable(GL_BLEND);

	glFlush();
	SwapBuffers(g_HDC);			// bring backbuffer to foreground
}