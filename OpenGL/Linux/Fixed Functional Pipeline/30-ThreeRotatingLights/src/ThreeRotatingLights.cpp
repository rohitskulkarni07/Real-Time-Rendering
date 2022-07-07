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


//Lights Changes
bool gbLight = false; // By Default Lighting Is Off
//							 1	   2	 3	   4
GLfloat lightAmbientZero[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuseZero[]  = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightSpecularZero[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPositionZero[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// Positional Light

GLfloat lightAmbientOne[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuseOne[]  = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat lightSpecularOne[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat lightPositionOne[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// Positional Light

GLfloat lightAmbientTwo[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuseTwo[]  = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat lightSpecularTwo[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat lightPositionTwo[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// Positional Light

GLfloat materialAmbient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess = 128.0f;

GLfloat gfLightAngle0 = 0.0f;
GLfloat gfLightAngle1 = 0.0f;
GLfloat gfLightAngle2 = 0.0f;

GLUquadric* quadric = NULL;
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

						case XK_L:
						case XK_l:
							gbLight = gbLight ? false : true;
							if (gbLight == true) {
								glEnable(GL_LIGHTING);
							}else {
								glDisable(GL_LIGHTING);
							}
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

	// Setting Light Properties
	//Initialization Of Light
	
	//_____________LIGHT0____________________________________________
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientZero);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseZero);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularZero);

	glEnable(GL_LIGHT0);

	//_____________LIGHT1_____________________________________________

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbientOne);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuseOne);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecularOne);

	glEnable(GL_LIGHT1);
	
	//______________LIGHT2____________________________________________

	glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbientTwo);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuseTwo);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecularTwo);

	glEnable(GL_LIGHT2);
	//_________________________________________________________________
	
	//Initialization Of Material

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);
	//________________________________________________
		
	//Clear Color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
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

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	
}


void display(void) {

		// Function Prototype
	void update(void);

	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
		gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	
		
		glPushMatrix();

			glRotatef(gfLightAngle0, 1.0f, 0.0f, 0.0f);
			lightPositionZero[1] = gfLightAngle0; //why yth Element of position is angle0 which doing x rotation?????? Ans: Gimble Lock
			glLightfv(GL_LIGHT0, GL_POSITION, lightPositionZero);

		glPopMatrix();

		glPushMatrix();

			glRotatef(gfLightAngle1, 0.0f, 1.0f, 0.0f);
			lightPositionOne[0] = gfLightAngle1; //why xth Element of position is angle1 which doing y rotation??????
			glLightfv(GL_LIGHT1, GL_POSITION, lightPositionOne);

		glPopMatrix();

		glPushMatrix();

			glRotatef(gfLightAngle2, 0.0f, 0.0f, 1.0f);
			lightPositionTwo[0] = gfLightAngle2; //why xth Element of position is angle2 which doing z rotation??????
			glLightfv(GL_LIGHT2, GL_POSITION, lightPositionTwo);

		glPopMatrix();

		glTranslatef(0.0f, 0.0f, -1.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		quadric = gluNewQuadric();
		gluSphere(quadric, 0.9f, 300, 300);

	glPopMatrix();
	
	update();
	//Change to next frame
	glXSwapBuffers(gpDisplay, gWindow);
	
}

void update(void) {
	
	gfLightAngle0 = gfLightAngle0 + 0.04f;

	if (gfLightAngle0 >= 361.0f) {
		gfLightAngle0 = 0.0f;
	}

	gfLightAngle1 = gfLightAngle1 + 0.04f;

	if (gfLightAngle1 >= 361.0f) {
		gfLightAngle1 = 0.0f;
	}

	gfLightAngle2 = gfLightAngle2 + 0.04f;

	if (gfLightAngle2 >= 361.0f) {
		gfLightAngle2 = 0.0f;
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

/*
	COMPILE: 	g++ -o OGLBlueScreen OpenGLBlueScreen.cpp -lX11 -lGL -lGLU
	RUN: 		./OGLBlueScreen 

 */
	
