#include <iostream>		// Input-Output Stream
#include <stdio.h>		// For C Standard Input-Output Function
#include <stdlib.h>		// For C Standard library Functions ex. exit()
#include <memory.h> 	// Memset
#include <math.h>

#include <X11/Xlib.h>	// XLib Releted API
#include <X11/Xutil.h>	// XVisualInfo Releted Function
#include <X11/XKBlib.h> // Releted To Keyboard symbol conversion
#include <X11/keysym.h> // KeySym Releted API
#include <GL/glx.h>		// For glx API ding API Library
#include <GL/gl.h>		// For OpenGL API
#include <GL/glu.h>		// For OpenGL Utilities

// namespaces
using namespace std;   
 
#define PI 3.141592653589793238

//flags
bool bFullscreen = false;	

//XWindow Releted Variables
Display *gpDisplay = NULL;			// Pointer To Display Struct
XVisualInfo *gpXVisualInfo = NULL;	// Pointer To XVisualInfo Struct, Information Releted to Visual Information screen, depth, color
Colormap gColormap;
Window gWindow;
GLXContext gGlxContext;

// Window Height And Width 
int giWindowWidth  = 800;
int giWindowHeight = 600;

GLfloat gfAngle = 0.0f;


bool gbAnimate = false;			// Animation Flag
bool gbLight = false;			// By Default Lighting Is Off
bool gbPolygonMode = false;		// Ploygon Mode

//angle for rotating shapes
GLfloat gfAngleX = 0.0f;
GLfloat gfAngleY = 0.0f;
GLfloat gfAngleZ = 0.0f;

//Lights Changes
GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };	
GLfloat lightDeffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };	
GLfloat lightPosition[] = { 0.0f, 3.0f, 3.0f, 0.0f };	// Directional Light

// Light Model
GLfloat light_Model_Ambient[] = { 0.2f, 0.2f ,0.2f, 1.0f };
GLfloat light_Model_Local_Viewer[] = { 0.0f };

// Quadric For Spheres
GLUquadric* quadric[24];

// Key-Pressed
GLuint keypressed = 0;

// Entry-point Function
int main(void) {

	// Function Prototype
	void CreateWindow(void);		
	void ToggleFullscreen(void);
	
	
	//bluescreen changes
	void initialize(void);
	void resize(int, int);
	void display(void);
	void uninitialize(void);
	
	// Variable Declarations
	int winWidth = giWindowWidth;
	int winHeight = giWindowHeight;
	
	//bluescreen changes
	bool bDone = false;
	

	// Code
	CreateWindow();

	//bluescreen changes
	initialize();
	

	// Game Loop
	
	XEvent event;	// XEvent is Union Having Many Members out of them 33 are Struct Members 
	KeySym keysym;	// To Store The Key-symbol
	
	while(bDone == false) {
		
		
		while(XPending(gpDisplay)) {
			
			XNextEvent(gpDisplay, &event);
			
			switch(event.type) {
				
				case MapNotify:		// similar to WM_CREATE
					break;
					
				case KeyPress:
					
					keysym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
					
					switch(keysym) {
						
						case XK_Escape: // Key symbol for Escape Key 
							//bluescreen changes
							bDone = true;
							
							
						case XK_F:	// Key-Symbol for 'F' Key
						case XK_f:	// Key-Symbol for 'f' Key
							
							if(bFullscreen == false) {
								ToggleFullscreen();
								bFullscreen = true;
							}
							
							else {
								ToggleFullscreen();
								bFullscreen = false;
							}
							break;

						case XK_A:
						case XK_a:
							gbAnimate = gbAnimate ? false : true;
							break;

						//Light Toggle
						case XK_L:
						case XK_l:
							gbLight = gbLight ? false : true;
							if (gbLight == true) {
								glEnable(GL_LIGHTING);
							}
							else {
								glDisable(GL_LIGHTING);
							}
							break;

						case XK_P:
						case XK_p:
							// Polygoan Mode Toggle
							gbPolygonMode = gbPolygonMode ? false : true;
							if (gbPolygonMode == true) {
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							}
							else {
								glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							}
							break;

						case XK_x:
						case XK_X:
							keypressed = 1;
							gfAngleX = 0.0f;
							break;

						case XK_y:
						case XK_Y:
							keypressed = 2;
							gfAngleY = 0.0f;

							break;

						case XK_z:
						case XK_Z:
							keypressed = 3;
							gfAngleZ = 0.0f;
							break;
					
						default:
							break;
						}
					
					break;

				case ButtonPress:
					switch(event.xbutton.button) {
						case 1:// left mouse button
							break;
						case 2:// middle mouse button
							break;
						case 3:// right mouse button
							break;
						case 4:// middle wheel up mouse button
							break;
						case 5:// middle wheel down mouse button
							break;
						default:
							break;
					}
					break;

				case MotionNotify: 	// pointer move / curosor move
					break;
					
				case ConfigureNotify: 	// similar to WM_SIZE releted StrucutreNotifyMask
					
					winWidth = event.xconfigure.width;
					winHeight = event.xconfigure.height;
					
					// bluescreen changes
					resize(winWidth, winHeight);
					
					break;
					
				case Expose:		// similar to WM_PAINT
					break;
					
				case DestroyNotify: 	// similar to WM_DESTROY
					break;
					
				case 33: 		// similar to DestroyNotify For dependent on Window Manager
					bDone = true;
					
				default:
					break;
			}
		}
		
		// Here Call Update Function
		if(bFullscreen){
			display();

		}
	}
	
	uninitialize();
	
	return(0);

}



void CreateWindow(void) {
	
	// Function Prototypes
	void uninitialize(void);

	// Variable Declarations
	XSetWindowAttributes winAttribs; // Object To struct which Holds Information about Window 
	int defaultScreen;
	int defaultDepth;
	int styleMask;

	static int frameBufferAttribute[] = {	GLX_DOUBLEBUFFER,
						True,
						GLX_RGBA, 
						GLX_RED_SIZE,	8,
						GLX_GREEN_SIZE,	8,
						GLX_BLUE_SIZE,	8,
						GLX_ALPHA_SIZE,	8,
						GLX_DEPTH_SIZE, 24,	
						None 	};

	/*	V4L(Video For Linux) recommends bit-size of depth around 24 or below, V4L Works Under Device Driver: Which further Classified into 2 Parts VA-API(Video Accelarartion) And VDPAU(Video Driver Presentation Unix API) Components Above This Layer MESA cames Into Picture*/					
	
	// Code
							
	gpDisplay = XOpenDisplay(NULL);
	
	// Error-Checking
	if(gpDisplay == NULL) {
		printf("Error : Unable To Open X Display.\nExitting Now...\n");
		uninitialize();
		exit(1);	// Exit Status:	0 successfull, 1 abortive,  -1 errornous
	}
	
	// To Get Default Screen/Current Screen Connected To Current Graphic Card
	defaultScreen = XDefaultScreen(gpDisplay);
	
	// To Get Current Color Depth for Default Screen Connected to Current GPU
	
	gpXVisualInfo = glXChooseVisual(gpDisplay, defaultScreen, frameBufferAttribute);
	
	// Allocating Memory To gpXVisualInfo 
	// gpXVisualInfo = (XVisualInfo *)malloc(sizeof(XVisualInfo));

	// Error-Checking
	if(gpXVisualInfo == NULL) {
		printf("Error : Unable To Allocate Memory For Visual Information.\nExiting Now...\n");
		uninitialize();
		exit(1);
	}
	
	XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, gpXVisualInfo);
	
	/* Status status = XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, gpXVisualInfo);
	
	if(status == 0) {
		printf("Error : Unable To Get A Visual.\nExiting Now...\n");
		uninitialize();
		exit(1);
	}
	*/
	
	//Error-Checking
	
	if(gpXVisualInfo == NULL) {
		printf("Error : Unable To Get A Visual.\nExiting Now...\n");
		uninitialize();
		exit(1);
	}

	
	winAttribs.border_pixel = 0;		// Defualt Border Color
	winAttribs.background_pixmap = 0;	// No background Image
	winAttribs.background_pixel = BlackPixel(gpDisplay, defaultScreen);	// Background Color : Black

	
	
	
	winAttribs.colormap = XCreateColormap(gpDisplay, RootWindow(gpDisplay, gpXVisualInfo->screen), gpXVisualInfo->visual, AllocNone);
	gColormap = winAttribs.colormap;
	
	winAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;

	
	styleMask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;
	
	gWindow = XCreateWindow(gpDisplay,
				RootWindow(gpDisplay, gpXVisualInfo->screen),
				0,
				0,
				giWindowWidth,
				giWindowHeight,
				0,
				gpXVisualInfo->depth,
				InputOutput,
				gpXVisualInfo->visual,
				styleMask,
				&winAttribs);
	
	if(!gWindow) {
		printf("Error : Failed To Create Main Window.\nExiting Now...\n");
		uninitialize();
		exit(1);
	}

	// Caption To Window
	XStoreName(gpDisplay,gWindow, "RTR2020-004 Rohit Sudhir Kulkarni");
	Atom windowManagerDelete = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(gpDisplay, gWindow, &windowManagerDelete,1);

	//Show Window On Display
	XMapWindow(gpDisplay, gWindow);

}



void initialize(void) {

	// Function Prototype
	void resize(int, int);
	GLuint LoadBitmapAsTexture(const char*);
	
	// Local Variable	
	
	// Code
	gGlxContext = glXCreateContext(gpDisplay, gpXVisualInfo, NULL, GL_TRUE);
	
	glXMakeCurrent(gpDisplay, gWindow, gGlxContext);
	
	// Depth Changel
	glShadeModel(GL_SMOOTH);
	
	glClearDepth(1.0f);
	
	glEnable(GL_DEPTH_TEST);
	
	glDepthFunc(GL_LEQUAL);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//________________________________________________

	// Give Normal To Given/Used Geometry
	glEnable(GL_AUTO_NORMAL);

	// Normalize Normals In 0-1
	glEnable(GL_NORMALIZE);
	
	// Lighting Model Changes
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_Model_Ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, light_Model_Local_Viewer);

	// Lighting Changes
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDeffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_LIGHT0);

	// Give Memory To Quadric

	for (int i = 0; i < 24; i++) {
		quadric[i] =  gluNewQuadric();
	}

	//Clear Color
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	
	resize(giWindowWidth, giWindowHeight);
}

void resize(int width, int height) {

	// code
	if (height <= 0) {
		height = 1;
	}
	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);


	// Here We Have Used Orthographic View To Avoid Barrel Distortaion
	
	if (width<= height) {

		glOrtho(0.0f,
			15.5f,
			0.0f,
			15.5 * ((GLfloat)height / (GLfloat)width),
			-10.0f,
			10.0f);
	}
	else {

		glOrtho(0.0f,
			15.5f * ((GLfloat)width/(GLfloat)height),
			0.0f,
			15.5,
			-10.0f,
			10.0f);
	}
	
}


void display(void) {
		// Function Prototype
	void update(void);
	void draw24Sphere(void);
	// code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	if (keypressed == 1) {
		glRotatef(gfAngleX, 1.0f, 0.0f, 0.0f);
		lightPosition[1] = gfAngleX;
		lightPosition[0] = 0.0f;
		lightPosition[2] = 0.0f;

	}
	else if (keypressed == 2) {
		glRotatef(gfAngleY, 0.0f, 1.0f, 0.0f);
		lightPosition[2] = gfAngleY;
		lightPosition[1] = 0.0f;
		lightPosition[0] = 0.0f;
	}
	else if (keypressed == 3) {
		glRotatef(gfAngleZ, 0.0f, 0.0f, 1.0f);
		lightPosition[0] = gfAngleZ;
		lightPosition[2] = 0.0f;
		lightPosition[1] = 0.0f;
	}
	//Light Position Modified, Update New Value
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//Now Draw Spheres
	draw24Sphere();

	update();

	glXSwapBuffers(gpDisplay,gWindow);
}

void update(void) {

	if (gbAnimate == true) {

		if (keypressed == 1) {
			gfAngleX = gfAngleX + 0.07f;
		}
		else if (keypressed == 2) {
			gfAngleY = gfAngleY + 0.07f;
		}
		else if (keypressed == 3) {
			gfAngleZ = gfAngleZ + 0.07f;
		}
	}

}



void ToggleFullscreen(void) {

	// Variable Declarations
	Atom wm_state; 		// Window's Current State
	Atom fullscreen;	// fullscreen state's atom
	XEvent xev={0};		// Create Object Of Event And Make All Of Members Zero

	// Code
	
	wm_state = XInternAtom(gpDisplay, "_NET_WM_STATE", False); 	// _NET_WM_STATE Currnet Window State Accros The Network

	memset(&xev, 0, sizeof(xev)); 	// use memset to zero out or initalise all to zero use either one 

	xev.type = ClientMessage;			// Type Of Message of Client (Send To Client)
	xev.xclient.window = gWindow;			// To Whcih Window to send message
	xev.xclient.message_type = wm_state;		// What Order to send
	xev.xclient.format = 32;			// Order Byte 32 bit message
	xev.xclient.data.l[0] = bFullscreen ? 0 : 1;	// Toggle The State fullscreen to normal or normal to fullscreen
	
	fullscreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
	xev.xclient.data.l[1] = fullscreen;

	XSendEvent(gpDisplay, RootWindow(gpDisplay, gpXVisualInfo->screen),False,StructureNotifyMask,&xev);

}


void uninitialize(void) {
	
	
	GLXContext currentGLXContext;
	
	currentGLXContext = glXGetCurrentContext();
	
	for(int i = 23; i <= 0; i--) {
		gluDeleteQuadric(quadric[i]);
		quadric[i] = NULL;

	}
	if(currentGLXContext == gGlxContext) {
	
		glXMakeCurrent(gpDisplay, 0, 0);
	}
	
	if(gGlxContext) {
		
		glXDestroyContext(gpDisplay, gGlxContext);
	}
	
	if(gWindow) {
		// unallocate memory given to window
		XDestroyWindow(gpDisplay, gWindow);
	
	}
	
	if(gColormap) {
		
		// Free Color-Map
		XFreeColormap(gpDisplay,gColormap);
	}
	
	if(gpXVisualInfo) {
		// Free gpXVisualInfo
		free(gpXVisualInfo);
		gpXVisualInfo = NULL;
	}
	
	if(gpDisplay) {
		// Dissconect the Display To Client App
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}
	
	
}

void draw24Sphere(void) {

	static GLfloat materialAmbient[4];
	static GLfloat materialDiffuse[4];
	static GLfloat materialSpecular[4];

	static GLfloat materialShineness;

	// Polygoan Mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//_________________________ Emerald 1st Sphere On First Coloumn _________________________
	materialAmbient[0] = 0.0215f;	//r
	materialAmbient[1] = 0.1745f;	//g
	materialAmbient[2] = 0.0215f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.07568f;	//r
	materialDiffuse[1] = 0.61424f;	//g
	materialDiffuse[2] = 0.07568f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.633f;	//r
	materialSpecular[1] = 0.727811f;	//g
	materialSpecular[2] = 0.633f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.6 * 128;
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(2.5f, 14.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//_________________________ Jade 2nd Sphere On First Coloumn _________________________
	materialAmbient[0] = 0.135f;	//r
	materialAmbient[1] = 0.2255f;	//g
	materialAmbient[2] = 0.1575f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.54;	//r
	materialDiffuse[1] = 0.89;	//g
	materialDiffuse[2] = 0.63f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.316228;	//r
	materialSpecular[1] = 0.316228;	//g
	materialSpecular[2] = 0.316228;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.1f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(2.5f, 11.64f, 0.0f);

	gluSphere(quadric[1], 1.0f, 30, 30);


	//___________________ Obsidian 3rd Sphere On First Coloumn _______________________________

	materialAmbient[0] = 0.05375f;	//r
	materialAmbient[1] = 0.05f;	//g
	materialAmbient[2] = 0.06625f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.18275f;	//r
	materialDiffuse[1] = 0.17f;	//g
	materialDiffuse[2] = 0.22525f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.332741f;	//r
	materialSpecular[1] = 0.328634f;	//g
	materialSpecular[2] = 0.346434f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.3f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(2.5f, 9.31f, 0.0f);

	gluSphere(quadric[2], 1.0f, 30, 30);


	//___________________ Pearl 4TH Sphere On First Coloumn _______________________________

	materialAmbient[0] = 0.25f;	//r
	materialAmbient[1] = 0.20725f;		//g
	materialAmbient[2] = 0.20725f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 1.0f;	//r
	materialDiffuse[1] = 0.829f;	//g
	materialDiffuse[2] = 0.829f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.296648f;	//r
	materialSpecular[1] = 0.296648f;	//g
	materialSpecular[2] = 0.296648f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.088f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(2.5f, 6.98f, 0.0f);

	gluSphere(quadric[3], 1.0f, 30, 30);

	//___________________ Ruby 5th Sphere On First Coloumn _______________________________


	materialAmbient[0] = 0.1745f;	//r
	materialAmbient[1] = 0.01175f;		//g
	materialAmbient[2] = 0.01175f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.61424f;	//r
	materialDiffuse[1] = 0.04136f;	//g
	materialDiffuse[2] = 0.04136f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.727811f;	//r
	materialSpecular[1] = 0.626959f;	//g
	materialSpecular[2] = 0.626959f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.6f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(2.5f, 4.65f, 0.0f);

	gluSphere(quadric[4], 1.0f, 30, 30);

	//___________________ Turquoise 6th Sphere On First Coloumn _______________________________

	materialAmbient[0] = 0.1f;	//r
	materialAmbient[1] = 0.18725f;		//g
	materialAmbient[2] = 0.1745f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.396f;	//r
	materialDiffuse[1] = 0.74151f;	//g
	materialDiffuse[2] = 0.69102f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.297254f;	//r
	materialSpecular[1] = 0.30829f;	//g
	materialSpecular[2] = 0.306678f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.1f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(2.5f, 2.32f, 0.0f);

	gluSphere(quadric[5], 1.0f, 30, 30);

	//_____________________________________________________________________________________________
	//_____________________________________________________________________________________________

	//___________________________ Brass 1st Sphere On Second Coloumn ___________________________

	materialAmbient[0] = 0.329412f;	//r
	materialAmbient[1] = 0.223529f;	//g
	materialAmbient[2] = 0.027451f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.780392f;	//r
	materialDiffuse[1] = 0.568627f;	//g
	materialDiffuse[2] = 0.113725f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.992157f;	//r
	materialSpecular[1] = 0.941176f;	//g
	materialSpecular[2] = 0.807843f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.21794872f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(10.5f, 14.0f, 0.0f);

	gluSphere(quadric[6], 1.0f, 30, 30);

	//___________________________ Bronze 2nd Sphere On Second Coloumn ___________________________

	materialAmbient[0] = 0.2125f;	//r
	materialAmbient[1] = 0.1275f;	//g
	materialAmbient[2] = 0.054f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.714f;	//r
	materialDiffuse[1] = 0.4284f;	//g
	materialDiffuse[2] = 0.18144f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.39354f;	//r
	materialSpecular[1] = 0.271906f;	//g
	materialSpecular[2] = 0.166721f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.2f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(10.5f, 11.64f, 0.0f);

	gluSphere(quadric[7], 1.0f, 30, 30);

	//___________________________ Chrome 3rd Sphere On Second Coloumn ___________________________

	materialAmbient[0] = 0.25f;	//r
	materialAmbient[1] = 0.25f;	//g
	materialAmbient[2] = 0.25f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.4f;	//r
	materialDiffuse[1] = 0.4f;	//g
	materialDiffuse[2] = 0.4f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.774597f;	//r
	materialSpecular[1] = 0.774597f;	//g
	materialSpecular[2] = 0.774597f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.6f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(10.5f, 9.31f, 0.0f);

	gluSphere(quadric[8], 1.0f, 30, 30);

	//___________________________ Copper 4th Sphere On Second Coloumn ___________________________

	materialAmbient[0] = 0.19125f;	//r
	materialAmbient[1] = 0.0735f;	//g
	materialAmbient[2] = 0.0225f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.7038f;	//r
	materialDiffuse[1] = 0.27048f;	//g
	materialDiffuse[2] = 0.0828f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.256777f;	//r
	materialSpecular[1] = 0.137622f;	//g
	materialSpecular[2] = 0.086014f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.1 * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(10.5f, 6.98f, 0.0f);

	gluSphere(quadric[9], 1.0f, 30, 30);

	//___________________________ Gold 5th Sphere On Second Coloumn ___________________________

	materialAmbient[0] = 0.24725f;	//r
	materialAmbient[1] = 0.1995f;	//g
	materialAmbient[2] = 0.0745f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.75164f;	//r
	materialDiffuse[1] = 0.60648f;	//g
	materialDiffuse[2] = 0.2264f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.628281f;	//r
	materialSpecular[1] = 0.555802f;	//g
	materialSpecular[2] = 0.366065f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.4f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(10.5f, 4.65f, 0.0f);

	gluSphere(quadric[10], 1.0f, 30, 30);

	//___________________________ Silver 6th Sphere On Second Coloumn ___________________________

	materialAmbient[0] = 0.19225f;	//r
	materialAmbient[1] = 0.19225f;	//g
	materialAmbient[2] = 0.19225f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.50754f;	//r
	materialDiffuse[1] = 0.50754f;	//g
	materialDiffuse[2] = 0.50754f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.508273f;	//r
	materialSpecular[1] = 0.508273f;	//g
	materialSpecular[2] = 0.508273f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.4f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(10.5f, 2.320f, 0.0f);

	gluSphere(quadric[11], 1.0f, 30, 30);


	//_____________________________________________________________________________________________
	//_____________________________________________________________________________________________

	//___________________________ Black 1st Sphere On Third Coloumn ___________________________

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.0f;	//g
	materialAmbient[2] = 0.0f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.01f;	//r
	materialDiffuse[1] = 0.01f;	//g
	materialDiffuse[2] = 0.01f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.50f;	//r
	materialSpecular[1] = 0.50f;	//g
	materialSpecular[2] = 0.50f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.25 * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(18.0f, 14.0f, 0.0f);

	gluSphere(quadric[12], 1.0f, 30, 30);

	//___________________________ Cyan 2nd Sphere On Third Coloumn ___________________________

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.1f;	//g
	materialAmbient[2] = 0.06f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.0f;	//r
	materialDiffuse[1] = 0.50980392f;	//g
	materialDiffuse[2] = 0.50980392f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.50196078f;	//r
	materialSpecular[1] = 0.50196078f;	//g
	materialSpecular[2] = 0.50196078f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.25f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(18.0f, 11.64f, 0.0f);

	gluSphere(quadric[13], 1.0f, 30, 30);

	//___________________________ Green 3rd Sphere On Third Coloumn ___________________________

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.0f;	//g
	materialAmbient[2] = 0.0f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.1f;	//r
	materialDiffuse[1] = 0.35f;	//g
	materialDiffuse[2] = 0.1f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.45f;	//r
	materialSpecular[1] = 0.55f;	//g
	materialSpecular[2] = 0.45f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.25f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(18.0f, 9.31f, 0.0f);

	gluSphere(quadric[14], 1.0f, 30, 30);

	//___________________________ Red 4th Sphere On Third Coloumn ___________________________

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.0f;	//g
	materialAmbient[2] = 0.0f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.5f;	//r
	materialDiffuse[1] = 0.0f;	//g
	materialDiffuse[2] = 0.0f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.7f;	//r
	materialSpecular[1] = 0.6f;	//g
	materialSpecular[2] = 0.6f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.25 * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(18.0f, 6.98f, 0.0f);

	gluSphere(quadric[15], 1.0f, 30, 30);

	//___________________________ White 5th Sphere On Third Coloumn ___________________________

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.0f;	//g
	materialAmbient[2] = 0.0f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.55f;	//r
	materialDiffuse[1] = 0.55f;	//g
	materialDiffuse[2] = 0.55f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.70f;	//r
	materialSpecular[1] = 0.70f;	//g
	materialSpecular[2] = 0.70f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.25f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(18.0f, 4.65f, 0.0f);

	gluSphere(quadric[16], 1.0f, 30, 30);

	//___________________________ Yellow Plastic 6th Sphere On Third Coloumn ___________________________

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.0f;	//g
	materialAmbient[2] = 0.0f;	//b
	materialAmbient[3] = 1.0f;	//a

	materialDiffuse[0] = 0.5f;	//r
	materialDiffuse[1] = 0.5f;	//g
	materialDiffuse[2] = 0.0f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.60f;	//r
	materialSpecular[1] = 0.60f;	//g
	materialSpecular[2] = 0.50f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.25f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(18.0f, 2.320f, 0.0f);

	gluSphere(quadric[17], 1.0f, 30, 30);

	//_____________________________________________________________________________________________
	//_____________________________________________________________________________________________
	
	//___________________________ Black 1st Sphere On Fourth Coloumn ___________________________

	materialAmbient[0] = 0.02f;	//r
	materialAmbient[1] = 0.02f;	//g
	materialAmbient[2] = 0.02f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.01f;	//r
	materialDiffuse[1] = 0.01f;	//g
	materialDiffuse[2] = 0.01f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.4f;	//r
	materialSpecular[1] = 0.4f;	//g
	materialSpecular[2] = 0.4f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.078125f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(25.0f, 14.0f, 0.0f);

	gluSphere(quadric[18], 1.0f, 30, 30);

	//___________________________ Cyan 2nd Sphere On Fourth Coloumn ___________________________

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.05f;	//g
	materialAmbient[2] = 0.05f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.4f;	//r
	materialDiffuse[1] = 0.5f;	//g
	materialDiffuse[2] = 0.5f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.04f;	//r
	materialSpecular[1] = 0.7f;	//g
	materialSpecular[2] = 0.7f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.078125f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(25.0f, 11.64f, 0.0f);

	gluSphere(quadric[19], 1.0f, 30, 30);

	//___________________________ Green 3rd Sphere On Fourth Coloumn ___________________________

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.05f;	//g
	materialAmbient[2] = 0.0f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.4f;	//r
	materialDiffuse[1] = 0.5f;	//g
	materialDiffuse[2] = 0.4f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.04f;	//r
	materialSpecular[1] = 0.7f;	//g
	materialSpecular[2] = 0.04f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.078125f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(25.0f, 9.31f, 0.0f);

	gluSphere(quadric[20], 1.0f, 30, 30);

	//___________________________ Red 4th Sphere On Fourth Coloumn ___________________________

	materialAmbient[0] = 0.05f;	//r
	materialAmbient[1] = 0.0f;	//g
	materialAmbient[2] = 0.0f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.5f;	//r
	materialDiffuse[1] = 0.4f;	//g
	materialDiffuse[2] = 0.4f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.7f;	//r
	materialSpecular[1] = 0.04f;	//g
	materialSpecular[2] = 0.04f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.078125f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(25.0f, 6.98f, 0.0f);

	gluSphere(quadric[21], 1.0f, 30, 30);

	//___________________________ White 5th Sphere On Fourth Coloumn ___________________________

	materialAmbient[0] = 0.05f;	//r
	materialAmbient[1] = 0.05f;	//g
	materialAmbient[2] = 0.05f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.5f;	//r
	materialDiffuse[1] = 0.5f;	//g
	materialDiffuse[2] = 0.5f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.7f;	//r
	materialSpecular[1] = 0.7f;	//g
	materialSpecular[2] = 0.7f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.078125f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(25.0f, 4.65f, 0.0f);

	gluSphere(quadric[22], 1.0f, 30, 30);

	//___________________________ Yellow Rubber 6th Sphere On Fourth Coloumn ___________________________

	materialAmbient[0] = 0.05f;	//r
	materialAmbient[1] = 0.05f;	//g
	materialAmbient[2] = 0.0f;	//b
	materialAmbient[3] = 1.0f;	//a

	materialDiffuse[0] = 0.5f;	//r
	materialDiffuse[1] = 0.5f;	//g
	materialDiffuse[2] = 0.4f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.7f;	//r
	materialSpecular[1] = 0.7f;	//g
	materialSpecular[2] = 0.04f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.078125f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(25.0f, 2.320f, 0.0f);

	gluSphere(quadric[23], 1.0f, 30, 30);
}


/*
	COMPILE: 	g++ -o OGLBlueScreen OpenGLBlueScreen.cpp -lX11 -lGL -lGLU
	RUN: 		./OGLBlueScreen 

 */
	
