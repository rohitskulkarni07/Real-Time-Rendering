#include <iostream>		// Input-Output Stream
#include <stdio.h>		// For C Standard Input-Output Function
#include <stdlib.h>		// For C Standard library Functions ex. exit()
#include <memory.h> 	// Memset
#include <math.h>
#include <stdlib.h>

#include <X11/Xlib.h>	// XLib Releted API
#include <X11/Xutil.h>	// XVisualInfo Releted Function
#include <X11/XKBlib.h> // Releted To Keyboard symbol conversion
#include <X11/keysym.h> // KeySym Releted API
#include <GL/glx.h>		// For glx API ding API Library
#include <GL/gl.h>		// For OpenGL API
#include <GL/glu.h>		// For OpenGL Utilities

//_________FOR AUDIO_________________

#include <AL/al.h>
#include <AL/alut.h>
#define BACKEND "alut"
#include <AL/alc.h>
//__________________________
// namespaces
using namespace std;    

//flags
bool bFullscreen = false;

//for auido state
ALuint  source;
ALint source_state;	

//XWindow Releted Variables
Display *gpDisplay = NULL;			// Pointer To Display Struct
XVisualInfo *gpXVisualInfo = NULL;	// Pointer To XVisualInfo Struct, Information Releted to Visual Information screen, depth, color
Colormap gColormap;
Window gWindow;
GLXContext gGlxContext;

// Window Height And Width 
int giWindowWidth  = 800;
int giWindowHeight = 600;

#define pi 3.141592653589793238
GLfloat gfx, gfy, gfr = 0.2f, gfi;

//Global Float Variable For Translation(x,y) of 'I' 'N' 'D' 'I' 'A'

GLfloat gftLeftIX = -2.1f;
GLfloat gftAX = 2.1f;
GLfloat gftNY = 1.3f;
GLfloat gftRightIY = -1.3f;
GLfloat gfsDZ = 0.0f;
GLfloat gftFlagX = -2.2f;

//Global Float Variable For Translation(x,y) of Fire Fighter
GLfloat gftFireX = -2.0f, gftUpFireY = 1.0f;
GLfloat gftMidFireY = 0.0f;
GLfloat gftLoFireY = -1.0f;

//Global Float Variable For Angle of Fire Fighter
GLfloat gfrUpFireAngle = -45.0f;
GLfloat gfrLoFireAngle = 45.0f;

//Global Float Variable For Color of Backgorund
GLfloat gfcWhite = 0.0f;


//___________Animation Flags_________________________
bool gbAnimateLeftI = true;
bool gbAnimateA = false;
bool gbAnimateN = false;
bool gbAnimateRightI = false;
bool gbAnimateD = false;
bool gbAnimateFighter = false;
bool gbAnimateFlag = false;
bool gbAnimateBg = false;

// Entry-point Function
int main(void) {

	// Function Prototype
	void CreateWindow(void);		
	void ToggleFullscreen(void);
	
	//for audio
	int startup_func_for_audio(void);
	
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

	//audio
	if(startup_func_for_audio() == -1){
		uninitialize();
		exit(0);
	}

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
		display();
	}
	
	uninitialize();
	
	return(0);

}

int startup_func_for_audio(void) {

	//init  openal
	alutInitWithoutContext(NULL,NULL);

	ALCdevice *device;
	ALCcontext *context;
	ALuint buffer;

	device = alcOpenDevice(NULL);
	
	if (!device) {
		fprintf(stderr, "unable to open default device\n");
		return -1;
	}

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		fprintf(stderr, "failed to make default context\n");
		return -1;
	}


	alGenSources((ALuint)1, &source);

	buffer = alutCreateBufferFromFile ("VandeMataram.wav");


	alSourcei(source, AL_BUFFER, buffer);

	alSourcePlay(source);

	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		
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
	
	// Texture Changes
	//glEnable(GL_TEXTURE_2D);
		
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
	void drawI(void);
	void drawN(void);
	void drawD(void);
	void drawA(void);
	void drawFlag(void);
	void drawFighter(void);
	void drawIAFF(void);
	void drawBackground(void);

	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	//audio
	if(source_state == AL_PLAYING){
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	}

	//draw here
	glLoadIdentity();
	glTranslatef(0.0, 0.0f, -3.0f);
	drawBackground();

	//_______Left 'I'__________
	
	glLoadIdentity();
	glTranslatef(gftLeftIX, 0.0f, -2.5f);	//X,Y,Z For Left I Position(-1.2f, 0.0f, -2.5f)
	glLineWidth(20.0f);
	drawI();

	//_______'N'__________

	glLoadIdentity();
	glTranslatef(-0.627f, gftNY, -2.5f);	//X, Y, Z Positin For N (-0.627f, 0.0f, -2.5f);
	glLineWidth(20.0f);
	drawN();
	
	//_______Left 'I'__________

	glLoadIdentity();
	glTranslatef(0.55f, gftRightIY, -2.5f);		//X,Y,Z For Right I Position(0.55f, 0.0f, -2.5f)
	glLineWidth(20.0f);
	drawI();
	
	//_______'D'__________

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -2.5f);			//X, Y, Z For D Position (0.0f, 0.0f, -2.5f);
	glLineWidth(20.0f);
	glScalef(gfsDZ, gfsDZ, 0.0f);
	drawD();

	//_______'A'__________

	glLoadIdentity();
	glTranslatef(gftAX, 0.0f, -2.5f);		//X, Y, Z For Position Of A (1.12f, 0.0f, -2.5f);
	glLineWidth(20.0f);
	drawA();

	glLoadIdentity();
	glTranslatef(gftFlagX, 0.0f, -2.5f);
	glScalef(0.375f, 0.05f, 1.0f);
	drawFlag();

	//_________UPPER FIGHTER_______________________________
	
	glLoadIdentity();
	glTranslatef(gftFireX, cos(gftUpFireY) + 0.985, -2.5f);
	glScalef(0.3f, 0.3f, 1.0f);
	glRotatef(gfrUpFireAngle, 0.0f, 0.0f, 1.0f);
	drawFighter();


	//_________MIDDLE FIGHTER_______________________________

	glLoadIdentity();
	glTranslatef(gftFireX, 0.0f, -2.5f);
	glScalef(0.3f, 0.3f, 1.0f);
	drawFighter();

	//_________LOWER FIGHTER_______________________________

	glLoadIdentity();
	glTranslatef(gftFireX, -cos(gftLoFireY)-0.985f, -2.5f);
	glScalef(0.3f, 0.3f, 1.0f);
	glRotatef(gfrLoFireAngle, 0.0f, 0.0f, 1.0f);
	drawFighter();

	update();

	glXSwapBuffers(gpDisplay, gWindow);
	
}


void update(void) {

	//_____________Update Frames For Left 'I'__________

	if (gbAnimateLeftI == true) {
		gftLeftIX += 0.0001f;
	}
	if (gftLeftIX >= -1.2f) {
		gftLeftIX = -1.2f;
		gbAnimateLeftI = false;
		gbAnimateA = true;
	}
	//_____________Update Frames For  'A'______________

	if (gbAnimateA == true) {
		gftAX -= 0.0001f;
	}
	if (gftAX <= 1.12f) {
		gftAX = 1.12f;
		gbAnimateA = false;
		gbAnimateN = true;
	}
	
	//_____________Update Frames For  'N'_______________

	if (gbAnimateN == true) {
		gftNY -= 0.0001f;
	}
	if (gftNY <= 0.0f) {
		gftNY = 0.0f;
		gbAnimateN = false;
		gbAnimateRightI = true;
	}

	//_____________Update Frames For Right 'I'__________

	if (gbAnimateRightI == true) {
		gftRightIY += 0.0001f;
	}
	if (gftRightIY >= 0.0f) {
		gftRightIY = 0.0f;
		gbAnimateRightI = false;
		gbAnimateD = true;
	}

	//_____________Update Frames For 'D'________________

	if (gbAnimateD == true) {
		gfsDZ += 0.0001f;
	}
	if (gfsDZ >= 1.0f) {
		gfsDZ = 1.0f;
		gbAnimateD = false;
		gbAnimateFighter = true;
		gbAnimateFlag = true;
	}
	//__________Update Frame For Background and Flag_________________

	if (gbAnimateFlag == true) {
		gftFlagX += 0.0001f;
	}
	if (gftFlagX >= 1.12f) {
		gftFlagX = 1.12;
		gbAnimateFlag = false;
	}

	if (gbAnimateBg == true) {
		if (!(gfcWhite >= 1.0f)) {
			gfcWhite += 0.0001f;
		}
	}

	//_____________Update Frames For FireFighters_______________
	
	if (gbAnimateFighter == true) {
		gftFireX += 0.0001f;

		if (gftFireX >= -0.015f && gftFireX <= 1.4f) {

			gfrUpFireAngle = 0;
			gfrLoFireAngle = 0;
			gftUpFireY = -2.969;
			gftLoFireY = 2.969;
		}
		else {

			gfrUpFireAngle += 0.00225;
			gfrLoFireAngle -= 0.00225;
			gftUpFireY += 0.0001f;
			gftLoFireY -= 0.0001f;
		}
	}

	//when to animate bg
	if (gftFireX >= 1.4) {
		gbAnimateBg = true;
	}
//_________________________________________________________________
}


void drawI(void) {


	glBegin(GL_LINES);

		glColor3f(1.0f, 0.6f, 0.2f);		//saffron
		glVertex3f(0.2, 0.213f, 0.0f);
		glVertex3f(-0.2, 0.213f, 0.0f);

		glVertex3f(0.0, 0.2f, 0.0f);
		glVertex3f(0.0, 0.1f, 0.0f);

		glVertex3f(0.0, 0.1f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);	//white
		glVertex3f(0.0, 0.0f, 0.0f);

		glVertex3f(0.0, 0.0f, 0.0f);
		glColor3f(0.07f, 0.53f, 0.03f); //green
		glVertex3f(0.0, -0.1f, 0.0f);

		glVertex3f(0.0, -0.1f, 0.0f);
		glVertex3f(0.0, -0.2f, 0.0f);

		glVertex3f(0.2, -0.213f, 0.0f);
		glVertex3f(-0.2, -0.213f, 0.0f);

	glEnd();
}

void drawN(void) {



	glBegin(GL_LINES);

	glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	glVertex3f(-0.2f, 0.23f, 0.0f);
	glVertex3f(-0.2f, 0.1f, 0.0f);

	glVertex3f(-0.2f, 0.1f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	//white
	glVertex3f(-0.2f, 0.0f, 0.0f);

	glVertex3f(-0.2f, 0.0f, 0.0f);
	glColor3f(0.07f, 0.53f, 0.03f); //green
	glVertex3f(-0.2f, -0.1f, 0.0f);

	glVertex3f(-0.2f, -0.1f, 0.0f);
	glVertex3f(-0.2f, -0.23f, 0.0f);

	//___________________________________

	glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	glVertex3f(-0.2f, 0.2f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	//white	
	glVertex3f(0.0f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);	//white	
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f(0.07f, 0.53f, 0.03f); //green
	glVertex3f(0.2f, -0.2f, 0.0f);


	//___________________________________

	glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	glVertex3f(0.2f, 0.23f, 0.0f);
	glVertex3f(0.2f, 0.1f, 0.0f);

	glVertex3f(0.2f, 0.1f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	//white
	glVertex3f(0.2f, 0.0f, 0.0f);

	glVertex3f(0.2f, 0.0f, 0.0f);
	glColor3f(0.07f, 0.53f, 0.03f); //green
	glVertex3f(0.2f, -0.1f, 0.0f);

	glVertex3f(0.2f, -0.1f, 0.0f);
	glVertex3f(0.2f, -0.23f, 0.0f);

	glEnd();
}

void drawD(void) {


	glBegin(GL_POLYGON);

	glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	glVertex3f(-0.2f, 0.213f, 0.0f);
	glVertex3f(0.1f, 0.213f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	//white
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.1f, -0.213f, 0.0f);
	glColor3f(0.07f, 0.53f, 0.03f); //green
	glVertex3f(-0.2f, -0.213f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	//white
	glVertex3f(-0.2f, 0.213f, 0.0f);


	glEnd();


	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.52f);

	for (gfi = 0; gfi < 360; gfi += 15) {

		gfx = gfr * cos(gfi * (pi / 180.0f));
		gfy = gfr * sin(gfi * (pi / 180.0f));

		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(gfx, gfy, 0.0f);
	}
	//angle i rad = ang in deg * pi /180
	glEnd();
	//_________________________________________________
	glLineWidth(20.0f);
	glBegin(GL_LINE_LOOP);

	glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	glVertex3f(-0.2f, 0.213f, 0.0f);
	glVertex3f(0.1f, 0.213f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	//white

	glVertex3f(0.2f, 0.0f, 0.0f);
	glColor3f(0.07f, 0.53f, 0.03f); //green
	glVertex3f(0.1f, -0.213f, 0.0f);

	glVertex3f(-0.2f, -0.213f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	//white
	glVertex3f(-0.2f, -0.1f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	glVertex3f(-0.2f, 0.2f, 0.0f);

	glEnd();

	//__________ashok chakra_______________________________
	


}

void drawA(void) {


	glBegin(GL_LINES);

	//___________________________________________
	glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	glVertex3f(-0.2f, 0.23f, 0.0f);	//upper dash
	glVertex3f(0.2f, 0.23f, 0.0f);

	//___________________________________________

	//glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	//glVertex3f(-0.2f, 0.0f, 0.0f);	//middle dash
	//glColor3f(1.0f, 1.0f, 1.0f);	//white
	//glVertex3f(-0.1f, 0.0f, 0.0f);	//middle dash

	//glVertex3f(-0.1f, 0.0f, 0.0f);	//middle dash
	//glVertex3f(0.1f, 0.0f, 0.0f);


	//glVertex3f(0.1f, 0.0f, 0.0f);
	//glColor3f(0.07f, 0.53f, 0.03f); //green
	//glVertex3f(0.2f, 0.0f, 0.0f);

	//___________________________________________


	glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	glVertex3f(-0.2f, 0.23f, 0.0f);
	glVertex3f(-0.2f, 0.1f, 0.0f);

	glVertex3f(-0.2f, 0.1f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	//white
	glVertex3f(-0.2f, 0.0f, 0.0f);

	glVertex3f(-0.2f, 0.0f, 0.0f);
	glColor3f(0.07f, 0.53f, 0.03f); //green
	glVertex3f(-0.2f, -0.1f, 0.0f);

	glVertex3f(-0.2f, -0.1f, 0.0f);
	glVertex3f(-0.2f, -0.23f, 0.0f);

	//__________________________________________
	glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	glVertex3f(0.2f, 0.23f, 0.0f);
	glVertex3f(0.2f, 0.1f, 0.0f);

	glVertex3f(0.2f, 0.1f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	//white
	glVertex3f(0.2f, 0.0f, 0.0f);

	glVertex3f(0.2f, 0.0f, 0.0f);
	glColor3f(0.07f, 0.53f, 0.03f); //green
	glVertex3f(0.2f, -0.1f, 0.0f);

	glVertex3f(0.2f, -0.1f, 0.0f);
	glVertex3f(0.2f, -0.23f, 0.0f);
	glEnd();
}

void drawFlag(void) {

	glBegin(GL_QUADS);

	glColor3f(1.0f, 0.6f, 0.2f);

	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.16f, 0.0f);
	glVertex3f(0.5f, 0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	glVertex3f(0.5f, 0.16f, 0.0f);
	glVertex3f(-0.5f, 0.16f, 0.0f);
	glVertex3f(-0.5f, -0.16f, 0.0f);
	glVertex3f(0.5f, -0.16f, 0.0f);

	glColor3f(0.125f, 0.250f, 0.113f);

	glVertex3f(0.5f, -0.16f, 0.0f);
	glVertex3f(-0.5f, -0.16f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);

	glEnd();

	glPointSize(5.5f);
	glBegin(GL_POINTS);

		glColor3f(0.0f, 0.0f, 0.52f);
		glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

}

void drawFighter(void) {

	//Function Prototype

	//Code
	glBegin(GL_POLYGON);

	//Head >
	glColor3f(0.36f, 0.54f, 0.66f);
	glVertex3f(0.5f, 0.0f, 0.0f);//A
	glVertex3f(0.1f, 0.1f, 0.0f);//B
	glVertex3f(0.1f, -0.1f, 0.0f);//B
	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(0.5f, 0.0f, 0.0f);//A

	glEnd();

	glBegin(GL_QUADS);


	//Upper Wing
	glColor3f(0.36f, 0.54f, 0.66f);
	glVertex3f(0.1f, 0.1f, 0.0f);//B
	glVertex3f(0.0f, 0.2f, 0.0f);//C
	glVertex3f(-0.1f, 0.2f, 0.0f);//D
	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(0.0f, 0.1f, 0.0f);//E

	//Lower Wing
	glVertex3f(0.0f, -0.1f, 0.0f);//E
	glColor3f(0.36f, 0.54f, 0.66f);
	glVertex3f(-0.1f, -0.2f, 0.0f);//D
	glVertex3f(0.0f, -0.2f, 0.0f);//C
	glVertex3f(0.1f, -0.1f, 0.0f);//B

	//Cockpit Square
	glColor3f(0.36f, 0.54f, 0.66f);
	glVertex3f(0.1f, 0.1f, 0.0f);//B
	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(-0.1f, 0.1f, 0.0f);//F
	glColor3f(0.36f, 0.54f, 0.66f);
	glVertex3f(-0.1f, -0.1f, 0.0f);//F
	glVertex3f(0.1f, -0.1f, 0.0f);//B

	glEnd();


	glBegin(GL_POLYGON);


	//Middle Body
	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(-0.1f, 0.1f, 0.0f);//F
	glVertex3f(-0.6f, 0.5f, 0.0f);//G
	glVertex3f(-0.7f, 0.5f, 0.0f);//H
	glVertex3f(-0.8f, 0.1f, 0.0f);//I
	glVertex3f(-0.9f, 0.1f, 0.0f);//J
	
	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(-0.95f, 0.05f, 0.0f);//K
	glColor3f(0.36f, 0.54f, 0.66f);

	glVertex3f(-0.95f, -0.05f, 0.0f);//K
	glVertex3f(-0.9f, -0.1f, 0.0f);//J
	glVertex3f(-0.8f, -0.1f, 0.0f);//I
	glVertex3f(-0.7f, -0.5f, 0.0f);//H
	glVertex3f(-0.6f, -0.5f, 0.0f);//G
	glVertex3f(-0.1f, -0.1f, 0.0f);//F

	glEnd();


	glBegin(GL_POLYGON);

	//Upper Missile
	glColor3f(0.36f, 0.54f, 0.66f);
	glVertex3f(-0.4f, 0.55f, 0.0f);//M1
	glVertex3f(-0.5f, 0.6f, 0.0f);//M2
	glVertex3f(-0.8f, 0.6f, 0.0f);//M3
	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(-0.8f, 0.5f, 0.0f);//M4
	glVertex3f(-0.5f, 0.5f, 0.0f);//M5
	glVertex3f(-0.4f, 0.55f, 0.0f);//M1

	glEnd();

	glBegin(GL_POLYGON);

	//Lower Missile
	glColor3f(0.36f, 0.54f, 0.66f);
	glVertex3f(-0.4f, -0.55f, 0.0f);//M1
	glVertex3f(-0.5f, -0.6f, 0.0f);//M2
	glVertex3f(-0.8f, -0.6f, 0.0f);//M3
	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(-0.8f, -0.5f, 0.0f);//M4
	glVertex3f(-0.5f, -0.5f, 0.0f);//M5
	glVertex3f(-0.4f, -0.55f, 0.0f);//M1

	glEnd();


	static GLfloat IAFFG_x = 0.0f, IAFFG_y = 0.0f;
	static GLfloat IAFFW_x = 0.0f, IAFFW_y = 0.0f;
	static GLfloat IAFFS_x = 0.0f, IAFFS_y = 0.0f;

	//Trillion Dollor Line
	glTranslatef(-0.5f, 0.0f, 0.0f);
	//saffron
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.6f, 0.2f);		//saffron
	for (gfi = 0.0f; gfi < 360.0f; gfi = gfi + 0.1) {

		IAFFS_x = 0.2f * cos(gfi * (pi / 180.0f));
		IAFFS_y = 0.2f * sin(gfi * (pi / 180.0f));
		glVertex3f(IAFFS_x, IAFFS_y, 0.0f);

	}
	glEnd();

	//white
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f); //white
	for (gfi = 0.0f; gfi < 360.0f; gfi = gfi + 0.1) {

		IAFFW_x = 0.12f * cos(gfi * (pi / 180.0f));
		IAFFW_y = 0.12f * sin(gfi * (pi / 180.0f));
		glVertex3f(IAFFW_x, IAFFW_y, 0.0f);

	}
	glEnd();

	//green
	glBegin(GL_POLYGON);
	glColor3f(0.07f, 0.53f, 0.03f); //green
	for (gfi = 0.0f; gfi < 360.0f; gfi = gfi + 0.1) {

		IAFFG_x = 0.06f * cos(gfi * (pi / 180.0f));
		IAFFG_y = 0.06f * sin(gfi * (pi / 180.0f));
		glVertex3f(IAFFG_x, IAFFG_y, 0.0f);

	}
	glEnd();

	glTranslatef(-0.9f, 0.0f, 0.0f);
	glScalef(0.9f, 0.16f, 1.0f);


//____________INDIAN FLAG___________________________
	
	glBegin(GL_QUADS);

	glColor3f(1.0f, 0.6f, 0.2f);

	glVertex3f(0.5f, 0.6f, 0.0f);
	glVertex3f(-0.5f, 0.6f, 0.0f);
	glVertex3f(-0.5f, 0.17f, 0.0f);
	glVertex3f(0.5f, 0.17f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	glVertex3f(0.5f, 0.17f, 0.0f);
	glVertex3f(-0.5f, 0.17f, 0.0f);
	glVertex3f(-0.5f, -0.17f, 0.0f);
	glVertex3f(0.5f, -0.17f, 0.0f);

	glColor3f(0.125f, 0.250f, 0.113f);

	glVertex3f(0.5f, -0.17f, 0.0f);
	glVertex3f(-0.5f, -0.17f, 0.0f);
	glVertex3f(-0.5f, -0.6f, 0.0f);
	glVertex3f(0.5f, -0.6f, 0.0f);

	glEnd();

	glPointSize(5.5f);
	glBegin(GL_POINTS);

	glColor3f(0.0f, 0.0f, 0.52f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
}


void drawBackground(void) {


	glBegin(GL_QUADS);

	glColor3f(gfcWhite, gfcWhite, gfcWhite); //white

	glVertex3f(3.0f, 3.0f, 0.0f);
	glVertex3f(-3.0f, 3.0f, 0.0f);
	glVertex3f(-3.0f, 0.1f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 0.1f, 0.0f);

	glVertex3f(3.0f, 0.1f, 0.0f);
	glColor3f(gfcWhite, gfcWhite, gfcWhite); //white
	glVertex3f(-3.0f, 0.1f, 0.0f);
	glVertex3f(-3.0f, -0.1f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, -0.1f, 0.0f);

	glVertex3f(3.0f, -0.1f, 0.0f);
	glColor3f(gfcWhite, gfcWhite, gfcWhite); //white
	glVertex3f(-3.0f, -0.1f, 0.0f);
	glVertex3f(-3.0f, -3.0f, 0.0f);
	glVertex3f(3.0f, -3.0f, 0.0f);

	glEnd();


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
	
