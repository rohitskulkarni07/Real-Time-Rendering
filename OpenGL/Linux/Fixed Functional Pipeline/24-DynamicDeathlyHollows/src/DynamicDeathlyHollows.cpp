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

//calculation

//DISTANCE FORMULA Length Of Sides Of Tirangle

GLfloat distAB = sqrt(pow(-0.5f - 0.0f, 2) + pow(-0.5f - 0.5f, 2));
GLfloat distBC = sqrt(pow(0.5f -(-0.5f), 2) + pow(-0.5f - (-0.5f), 2));
GLfloat distAC = sqrt(pow(0.5f - 0.0f, 2) + pow(-0.5f - 0.5f, 2));

GLfloat s = (distAB + distBC + distAC) / (2.0f);

//RADIUIS OF CIRCLE or Inradius
GLfloat rad = (sqrt(s * (s - distAB) * (s - distBC) * (s - distAC))) / (s);


//x,y of incircle
GLfloat K = (distAB + distBC + distAC);
GLfloat ix = ((distBC * 0.0f) + (distAC * (-0.5f)) + (distAB * (0.5f))) / K;
GLfloat iy = ((distBC * (0.5f)) + (distAC * (-0.5f)) + (distAB * (-0.5f))) / K;

//For Rotation angle
GLfloat gfAngleForLine = 0.0f;
GLfloat gfAngleForTriangle = 0.0f;
GLfloat gfAngleForCircle = 0.0f;

//For Translation
GLfloat gfTranslateLineY = 2.0f;
GLfloat gfTranslateTriangleX = -2.0f;
GLfloat gfTranslateTriangleY = -2.0f;
GLfloat gfTranslateCircleX = 2.0f;
GLfloat gfTranslateCircleY = -2.0f + iy;

//Flags For Animation
bool gbRotateLine = true;
bool gbRotateTriangle = true;
bool gbRotateCircle = true;

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
	void drawLine(void);
	void drawCircle(void);
	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	//draw here
	glLoadIdentity();

	if (gbRotateLine == false) {
		gfTranslateLineY = 0.0f;
	}
	glTranslatef(0.0f, gfTranslateLineY, -3.0f);
	glRotatef(gfAngleForLine,0.0f, 0.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawLine();


	if (gbRotateLine == false) {
		gfTranslateTriangleX = 0.0f;
		gfTranslateTriangleY = 0.0f;
	}
	glLoadIdentity();
	glTranslatef(gfTranslateTriangleX,gfTranslateTriangleY , -3.0f);
	glRotatef(gfAngleForTriangle, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawTriangle();

	if (gbRotateCircle == false) {
		gfTranslateCircleX = ix;
		gfTranslateCircleY = iy;
	}
	glLoadIdentity();
	glTranslatef(gfTranslateCircleX, gfTranslateCircleY,-3.0f);
	glRotatef(gfAngleForCircle, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawCircle();

	update();
	//Change to next frame
	glXSwapBuffers(gpDisplay, gWindow);
	
}

void drawTriangle(void) {
	
	//code
	glBegin(GL_LINES);

		glVertex3f(0.0f, 0.5f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);

		glVertex3f(-0.5f, -0.5f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);

		glVertex3f(0.5f, -0.5f, 0.0f);
		glVertex3f(0.0f, 0.5f, 0.0f);

	glEnd();
}

void drawLine(void) {

	glBegin(GL_LINES);

	glVertex3f(0.0f, 0.5f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);

	glEnd();
}

void drawCircle() {


	glBegin(GL_POINTS);

	for (GLfloat angle = 0.0f; angle <= 2 * PI; angle = angle + 0.001f) {
	
		glVertex3f(rad * cos(angle), rad * sin(angle), 0.0f);
	}

	glEnd();
}

void update(void) {

	
	gfTranslateLineY = gfTranslateLineY - 0.0003f;
	if (gfTranslateLineY <= 0.0f) {
		//gfAngleForLine = 360.0f;
		gbRotateLine = false;
	}

	gfTranslateTriangleX = gfTranslateTriangleX + 0.0003f;
	gfTranslateTriangleY = gfTranslateTriangleY + 0.0003f;
	if (gfTranslateTriangleX >= 0.0f && gfTranslateTriangleY>=0.0f) {
		/*gfAngleForTriangle = 360.0f;*/
		gbRotateTriangle = false;
	}

	gfTranslateCircleX = gfTranslateCircleX - 0.0003f;

	if (gfTranslateCircleX <= ix ) {
		gfTranslateCircleX = ix;
	/*	gfTranslateCircleY = iy;
		gfAngleForCircle = 360.0f;*/
		//gbRotateCircle = false;

	}

	gfTranslateCircleY = gfTranslateCircleY + 0.0003f;
	if (gfTranslateCircleY >= iy) {
		gfTranslateCircleY = iy;
		//gbRotateCircle = false;
	}

	if (gfTranslateCircleX <= ix && gfTranslateCircleY >= iy) {
		gbRotateCircle = false;
	}


	gfAngleForLine = gfAngleForLine + 0.021f;
	if (gfAngleForLine >= 360.0f) {
		gfAngleForLine = 360.0f;
	}
	
	gfAngleForTriangle = gfAngleForTriangle + 0.021f;
	if (gfAngleForTriangle >= 360.0f) {
		gfAngleForTriangle = 360.0f;
	}

	gfAngleForCircle = gfAngleForCircle + 0.021f;
	if (gfAngleForCircle >= 360.0f) {
		gfAngleForCircle = 360.0f;
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
	