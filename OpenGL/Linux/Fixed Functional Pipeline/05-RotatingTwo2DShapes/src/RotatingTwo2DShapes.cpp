#include <iostream>	// Input-Output Stream
#include <stdio.h>	// For C Standard Input-Output Function
#include <stdlib.h>	// For C Standard library Functions ex. exit()
#include <memory.h> 	// Memset

#include <X11/Xlib.h>	// XLib Releted API
#include <X11/Xutil.h>	// XVisualInfo Releted Function
#include <X11/XKBlib.h> // Releted To Keyboard symbol conversion
#include <X11/keysym.h> // KeySym Releted API

// Path : /usr/include/GL
#include <GL/glx.h>	// For glx API bridge-ing API Library
#include <GL/gl.h>	// For OpenGL API
#include <GL/glu.h>	// For OpenGL Utilities

// namespaces
using namespace std;    

bool bFullscreen = false;	


Display *gpDisplay = NULL;			// Pointer To Display Struct
XVisualInfo *gpXVisualInfo = NULL;	// Pointer To XVisualInfo Struct, Information Releted to Visual Information screen, depth, color
Colormap gColormap;
Window gWindow;

GLXContext gGlxContext;

// Window Height And Width 
int giWindowWidth  = 800;
int giWindowHeight = 600;

GLfloat triangleAngle = 0;
GLfloat quadAngle = 0;

// Entry-point Function
int main(void) {

	// Function Prototype
	void CreateWindow(void);		
	void ToggleFullscreen(void);
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
	initialize();
	
	// Game Loop	
	XEvent event;		// XEvent is Union Having Many Members out of them 33 are Struct Members 
	KeySym keysym;		// To Store The Key-symbol
	
	while(bDone == false) {
		
		
		while(XPending(gpDisplay)) {
			
			XNextEvent(gpDisplay, &event);
			
			switch(event.type) {
				
				case MapNotify:			// similar to WM_CREATE
					break;
					
				case KeyPress:
					
					keysym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
					
					switch(keysym) {
						
						case XK_Escape: // Key symbol for Escape Key 
							bDone = true;
							
							
						case XK_F:		// Key-Symbol for 'F' Key
						case XK_f:		// Key-Symbol for 'f' Key
							
							if(bFullscreen == false) {
								ToggleFullscreen();
								bFullscreen = true;
							}
							
							else {
								ToggleFullscreen();
								bFullscreen = false;
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

				case MotionNotify: 		// pointer move / curosor move
					break;
					
				case ConfigureNotify: 	// similar to WM_SIZE releted StrucutreNotifyMask
					
					winWidth = event.xconfigure.width;
					winHeight = event.xconfigure.height;					
					resize(winWidth, winHeight);
					
					break;
					
				case Expose:			// similar to WM_PAINT
					break;
					
				case DestroyNotify: 	// similar to WM_DESTROY
					break;
					
				case 33: 				// similar to DestroyNotify For dependent on Window Manager
					bDone = true;
					
				default:
					break;
			}
		}
		
		// Here Call Update Function
		display();
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

	/*	V4L(Video For Linux) recommends bit-size of depth around 24 or below, 
	//V4L Works Under Device Driver: Which further Classified into 2 Parts VA-API(Video Accelarartion) 
	And VDPAU(Video Driver Presentation Unix API) Components Above This Layer MESA cames Into Picture*/					
	
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
	XStoreName(gpDisplay,gWindow, "Rotating Two 2D Shape");
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

	// function prototype
	void update(void);
	void drawTriangle(void);
	void drawQuad(void);

	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-1.5f, 0.0f, -5.0f);
	glRotatef(triangleAngle, 0.0f, 1.0f, 0.0f);
	drawTriangle();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 0.0f, -5.0f);
	glRotatef(quadAngle, 1.0f, 0.0f, 0.0f);
	drawQuad();
	
	update();
	glXSwapBuffers(gpDisplay, gWindow);
	
}

void drawTriangle(void) {
	
	glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();
}

void drawQuad(void) {

	glBegin(GL_QUADS);

		glColor3f(0.0f, 0.0f, 1.0f);

		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();
}


void update(void) {

	triangleAngle+=0.01f;
	if(triangleAngle>=360.0f) {
		triangleAngle = 0;
	}

	quadAngle+=0.01f;
	if(quadAngle>=360.0f) {
		quadAngle = 0;
	}
}

void ToggleFullscreen(void) {

	// Variable Declarations
	Atom wm_state; 		// Window's Current State
	Atom fullscreen;	// fullscreen state's atom
	XEvent xev={0};		// Create Object Of Event And Make All Of Members Zero

	// Code
	
	wm_state = XInternAtom(gpDisplay, "_NET_WM_STATE", False); 	// _NET_WM_STATE Currnet Window State Accros The Network

	memset(&xev, 0, sizeof(xev)); 					// use memset to zero out or initalise all to zero use either one 

	xev.type = ClientMessage;						// Type Of Message of Client (Send To Client)
	xev.xclient.window = gWindow;					// To Whcih Window to send message
	xev.xclient.message_type = wm_state;			// What Order to send
	xev.xclient.format = 32;						// Order Byte 32 bit message
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
	
