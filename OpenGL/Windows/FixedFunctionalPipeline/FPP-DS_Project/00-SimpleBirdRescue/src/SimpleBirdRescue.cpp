#pragma once
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "Humanoid.h"

#include <gl/gl.h>	// graphic library
#include <gl/glu.h> // graphic library utility

#include "dsheader.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")

//#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"winmm.lib")

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

// Global Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable
FILE* gpFile = NULL;
HWND ghwnd = NULL;
HDC	ghdc = NULL;
HGLRC ghrc = NULL;
DWORD dwStyle = NULL;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

// for music
#define MUSIC_TIMER 100

// flags
bool gbMoonCameraFlag = false;
bool gbHumCameraFlag = false;
bool gbBirdCameraFlag = false;

bool gbFullscreen = false;
bool gbActiveWindow = false;
bool gbPolygonMode = false;			// Ploygon Mode
bool gbLight = false;
bool gbIntroFlag = true;			// 1st flag to be enable
bool gbHDisplayFlag = true;
bool gbIntroHum = false;
bool gbIntroBird = true;
bool gbAccept1 = true;
bool gbRescueBird = false;
bool gbExitDemoFlag = false;		// last flag to be enable
bool gbResetCamera = false;
bool gbRunCamera = false;
bool gbLaser = false;
bool gbStartAction = false;
bool gbMoveShoulder = false;
bool gbByeBye = false;

// Display List For TREE 
GLuint guiTreeList;

//Global resize's height width
int giWidth = 0;
int giHeight = 0;

//Alpha
GLfloat gfAlphaFor1stIntoduction = 2.0f;
GLfloat gfAlphaForLastIntoduction = 0.0f;

//scale

GLfloat gfScaleFor1stIntoduction = 1.0f;

//Font Rendering 
unsigned int listBase;

// Tree
GLfloat gfTimTim = 0.0f;

//Bird
#define R glColor3f(1.0f , 0.0f, 0.0f);
#define O glColor3f(1.0f, 165.0f/255.0f ,0.0f);
#define Y glColor3f(1.0f, 1.0f ,0.0f);
#define G glColor3f(0.0f, 128.0f/255.0f ,0.0f);
#define B glColor3f(0.0f, 0.0f, 1.0f);
#define I glColor3f(75.0f/255.0f, 0.0f, 130.0f/255.0f);
#define V glColor3f(238.0f/255.0f, 130.0f/255.0f, 238.0f/255.0f);

//RGB: (186, 139, 87)
//GLfloat owlBodyColor[4] = { 0.72f, 0.54f, 0.34f, 1.0f };

//RGB: (244, 194, 194)
GLfloat owlBodyColor[4] = { 0.90f, 0.32f, 0.50f, 1.0f };
//RGB: (252, 245, 102)For Yellow
GLfloat owlEyesColor[4] = { 1.0f, 1.0f, 0.4f, 1.0f };
GLfloat owlBeakColor[4] = { 1.0f, 0.17f, 0.33f, 1.0f };
//Wings Moment
GLfloat gfWingsAngle = 0.0f;

//LIGHTS
GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { -9.0f, 0.0f, 10.0f, 1.0f };

//colors
//cyan eye rgb(19, 210, 255)
GLfloat materialAmbient[4];
GLfloat materialDiffuse[4];
GLfloat materialSpecular[4];
GLfloat materialShineness;

//Translation of Body
GLfloat gftX = 4.0f, gftY = 17.0f, gftZ = 0.0f;

//Translation of Bird
GLfloat gftXBird = -7.0f, gftYBird = 10.0f, gftZBird = 2.0f;
GLfloat gftXJail = -7.0f, gftYJail = 10.0f, gftZJail = 1.9f;

// Humanoid Variables
#define HWALK_SPEED 0.2f
GLfloat inclineAngle = 0.0f;
GLint shoulder = 180;
GLint elbow = 0;
GLint heap = 180;
GLint knee = 0;
GLfloat gfAngleHead = 0.0f;
bool gbHeadFlag = true;
GLfloat gfAngleChest = 0.0f;
GLUquadric* quadric = NULL;
GLfloat hbeat = 1.0f;
GLfloat gfLaserheight = 0.0f;

//Texture
GLuint guiMoon_texture;
GLuint guiBg_texture;
GLuint guiGround_texture;

//DS VARIABLES
CAMERA* cameraMoon;
CAMERA* cameraHum;
CAMERA* cameraBird;
CAMERA* Temp;

//camera
GLfloat camX = 0.0f, camY = 0.0f, camZ = 0.0f;

//bezierCurve For Moon
//GLfloat xcoord[4] = { 0.000f, 14.000f, 7.399f, 31.800f };
//GLfloat ycoord[4] = { 0.0f, 16.6999f, -6.999f, 24.899f };
//GLfloat zcoord[4] = { 22.0f, -5.999, -26.600, -3.8999 };

//bezierCurve For Humonid
//GLfloat xcoord[4] = { 0.000f, -17.000f, 34.299f, 0.000f };
//GLfloat ycoord[4] = { 1.300f, -8.200f, -6.999f, -6.398f };
//GLfloat zcoord[4] = { 14.0f, -5.999, -26.600, 22.58f };

//bezierCurve For Bird
//GLfloat xcoord[4] = { -4.999f, -4.999f, -28.501f, -2.899f };
//GLfloat ycoord[4] = { 0.000f, 0.000f, 0.000f, 0.000f };
//GLfloat zcoord[4] = { 7.0f, 12.000f, -26.600, 1.167f };

//Exit
GLfloat gfTheEndY = -22.0f;
GLfloat gfStartY = -10.0f;
//timeline for camera
#define TSPEED 0.0007f;
//#define TSPEED 0.1f;
GLfloat t = 1.0f;
int curr = 0;
GLfloat x = 0, y = 0, z = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	// Function Prototype
	void initialize(void);  // init
	void display(void);		// draw
	void ToggleFullscreen(void);


	WNDCLASSEX wndclass = { sizeof(WNDCLASSEX) };
	MSG msg = {};
	HWND hwnd = NULL;
	TCHAR szAppName[255] = TEXT("CLASS_MYOPENGL_WINDOW");

	bool bDone = false;		// used for while loop


	if (fopen_s(&gpFile, "log.txt", "w") != 0) {
		MessageBox(NULL, TEXT("FILE CREATION FAILED PROGRAM WILL EXIT..."), TEXT("ERROR"), MB_ICONEXCLAMATION | MB_OK);
		exit(0);
	}
	else {
		fprintf(gpFile, "File \"log.txt\" Created... Program Started Succesfully!\n");
	}

	// Initilize WNDCLASS

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Custom Window Own Device Context|  CS_OWNDC => Keep My DC Intact In Memory Don Discard Dont Move.
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	RegisterClassEx(&wndclass);
	fprintf(gpFile, "Class Registered Successfully\n");

	//Create Window In Memory

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,	//newly added parametr Beacuse CreateWindowEx
		szAppName,
		TEXT("SIMPLE BIRD RESCUE | v1.0"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		((GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2),
		((GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2),
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Assign global Handle With Current Window Handle
	ghwnd = hwnd;
	fprintf(gpFile, "Create Window Successfull\n");

	// Calling User Define Function 
	initialize();

	// Show Window
	ShowWindow(hwnd, iCmdShow);
	fprintf(gpFile, "ShowWindow() Window Successfull\n");

	// Replacement of UpdateWindow
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	
	//Toggle Fullscreen
	ToggleFullscreen();
	SetTimer(hwnd, MUSIC_TIMER, 10, NULL);


	// THE GAME LOOP
	while (bDone == false) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			if (msg.message == WM_QUIT) {

				bDone = true;

			}
			else {

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
		}
		else {

			if (gbActiveWindow == true) {
				// Here You Should Call Update() For OpenGL Rendering 
				// Here You Should Call display() For OpenGL Rendering 
				display();
			}
		}

	}

	if (gpFile) {
		fclose(gpFile);
		gpFile = NULL;
	}

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	// Local Function Prototype
	void ToggleFullscreen(void);
	void display(void);
	void resize(int, int);
	void uninitialize(void);

	// Local Variable
	PAINTSTRUCT ps = {};
	HDC hdc = NULL;
	RECT rc = {};	

	static char str[256];

	switch (iMsg) {

	case WM_CREATE:

		fprintf(gpFile, "WM_CREATE Recieved\n");
		break;

	case WM_SETFOCUS:			// This Message Comes When Application Window Get Activated
		gbActiveWindow = true;

		break;

	case WM_KILLFOCUS:			// // This Message Comes When Application Window Is Get Unfocus
		gbActiveWindow = false;

		break;

	case WM_ERASEBKGND:
		return(0); // SOMETHING NEW

	case WM_SIZE:
		fprintf(gpFile, "WM_SIZE Recieved\n");
		resize(LOWORD(lParam), HIWORD(lParam));

		break;

	case WM_KEYDOWN:
		fprintf(gpFile, "WM_KEYDONW Recieved\n");

		switch (wParam) {

		case VK_ESCAPE:

			DestroyWindow(hwnd);

			break;

		case 0x46:
		case 0x66:
			// Call ToggleFullscreen()
			ToggleFullscreen();

			break;

		case VK_RIGHT:
			gftX += HWALK_SPEED;
			//inclineAngle = inclineAngle != 30 ? inclineAngle - 0.1 : inclineAngle + 0.1;

			break;

		case VK_LEFT:
			gftX -= HWALK_SPEED;
			//inclineAngle = inclineAngle != 30 ? inclineAngle + 0.1 : inclineAngle - 0.1;
			break;
		case VK_UP:
			gftZ += HWALK_SPEED;
			break;
		case VK_DOWN:
			gftZ -= HWALK_SPEED;
			break;
		
		default:
			break;

		}
		break;

	case WM_TIMER:
		KillTimer(hwnd, wParam);
		PlaySound(MAKEINTRESOURCE(TRACK), NULL, SND_RESOURCE | SND_ASYNC);

		break;

	case WM_CHAR:

		switch (LOWORD(wParam)) {

		case 'P':
		case 'p':
			// Polygoan Mode Toggle
			gbPolygonMode = gbPolygonMode ? false : true;
			if (gbPolygonMode == true) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;

			// humanoid moment

		case 'r':
			gfAngleHead = gfAngleHead - 1.4f;

			break;

		case 'R':

			gfAngleHead = gfAngleHead + 1.4f;

			break;

		case 'S':

			shoulder = (shoulder + 3) % 360;

			break;

		case 's':

			shoulder = (shoulder - 3) % 360;

			break;

		case 'E':

			elbow = (elbow + 3) % 360;

			break;

		case 'e':

			elbow = (elbow - 3) % 360;

			break;

		/*case 'L':
		case 'l':

			gbLight = gbLight ? false : true;

			if (gbLight == true) {
				glEnable(GL_LIGHTING);
			}
			else {
				glDisable(GL_LIGHTING);
			}

			break;*/
		case '1':
		case '!':
			//RescueBird
			gbRescueBird = true;
		//	gbLaser = false;
			break;
		case '2':
		case '@':
			gbLaser = true;
			break;
		case 'Q':
		case 'q':
			gbExitDemoFlag = true;
			break;
		
		case 'n':
			curr = (curr + 1) % 4;
			break;

		case 'N':
			if (curr == 0) {
				curr = 3;
			}
			else {
				curr = (curr - 1) % 4;
			}
			break;
		/*
		case 'x':
			xcoord[curr] += 0.1f;
			break;
		case 'X':
			xcoord[curr] -= 0.1f;
			break;

		case 'y':
			ycoord[curr] += 0.1f;
			break;
		case 'Y':
			ycoord[curr] -= 0.1f;
			break;

		case 'z':
			zcoord[curr] += 0.1f;
			break;
		case 'Z':
			zcoord[curr] -= 0.1f;
			break;*/

		case 'j':
		case 'J':
			camX -= 0.1f;
			break;
		case 'L':
		case 'l':
			camX += 0.1f;
		case 'i':
		case 'I':
			camZ -= 0.1f;
			break;
		case 'k':
		case 'K':
			camZ += 0.1f;
			break;
		default:
			break;
		}
		/*sprintf(str, "curr = %d , x = %f , y = %f, z = %f camX = %f, camY = %f, camZ = %f", curr, cameraMoon->xcoord[curr], cameraMoon->ycoord[curr], cameraMoon->zcoord[curr], camX, camY, camZ);
		SetWindowTextA(hwnd, str);*/
		break;
	case WM_CLOSE:
		fprintf(gpFile, "WM_CLOSE Recieved\n");
		DestroyWindow(hwnd);

		break;

	case WM_DESTROY:

		fprintf(gpFile, "WM_DESTROY Recieved\n");
		PlaySound(NULL, NULL, SND_RESOURCE | SND_ASYNC);
		uninitialize();
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void) {

	//Local Variable
	MONITORINFO mi = { sizeof(MONITORINFO) };


	if (gbFullscreen == false) {

		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);			// Retrieves information about the specified window , Here Retrives Current Styel Of Given Window

		//Check If Window Has WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
		if (dwStyle & WS_OVERLAPPEDWINDOW) {

			if ((GetWindowPlacement(ghwnd, &wpPrev)) && (GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))) {

				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

				SetWindowPos(ghwnd,
					HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}

		ShowCursor(FALSE);
		gbFullscreen = true;

	}
	else {

		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		gbFullscreen = false;
	}

}

void initialize(void) {
	fprintf(gpFile, "ENRTY initialize()\n");

	// Function Prototype
	void resize(int, int);
	unsigned int CreateBitmapFont(char*, int);
	bool LoadGLTexture(GLuint*, TCHAR[]);

	// Local Variable
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	// code
	ghdc = GetDC(ghwnd);

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR)); // or memset((void*)&pfd, 0, sizeof(PIXELFORMATEDECRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	//Error Checking
	if (iPixelFormatIndex == 0) {
		fprintf(gpFile, "ChoosePixelFormat() Failed\n");
		DestroyWindow(ghwnd);
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE) {
		fprintf(gpFile, "SetPixelFormat() Failed\n");
		DestroyWindow(ghwnd);
	}

	ghrc = wglCreateContext(ghdc); // Bridging API or Windowing System Integration (WSI)

	if (ghrc == NULL) {
		fprintf(gpFile, "wglCreateContext() Failed\n");
		DestroyWindow(ghwnd);
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE) {
		fprintf(gpFile, "wglMakeCurrent() Failed\n");
		DestroyWindow(ghwnd);
	}
	// Memory Allocation
	ZeroMemory(&cameraMoon, sizeof(CAMERA));
	ZeroMemory(&cameraHum, sizeof(CAMERA));
	ZeroMemory(&cameraBird, sizeof(CAMERA));

	cameraMoon = (CAMERA*)malloc(sizeof(CAMERA));
	cameraHum = (CAMERA*)malloc(sizeof(CAMERA));
	cameraBird = (CAMERA*)malloc(sizeof(CAMERA));
	
	if (cameraMoon == NULL || cameraHum == NULL || cameraBird == NULL) {
		fprintf(gpFile, "Cannot Allocate Memory To struct Camera\n");
		DestroyWindow(ghwnd);
	}
	
	//____________________________________

	cameraMoon->EyeCoord[0] = 0.0f;
	cameraMoon->EyeCoord[0] = 0.0f;
	cameraMoon->EyeCoord[0] = 8.0f;

	cameraMoon->CenterCoord[0] = 13.0f;
	cameraMoon->CenterCoord[1] = 10.0f;
	cameraMoon->CenterCoord[2] = -10.0f;

	//bezierCurve For Moon
	cameraMoon->xcoord[0] = 0.000f; cameraMoon->xcoord[1] = 14.000f; cameraMoon->xcoord[2] =  7.399f; cameraMoon->xcoord[3] = 31.800f;
	cameraMoon->ycoord[0] = 0.000f; cameraMoon->ycoord[1] = 16.699f; cameraMoon->ycoord[2] = -6.999f; cameraMoon->ycoord[3] = 24.899f;
	cameraMoon->zcoord[0] = 22.00f; cameraMoon->zcoord[1] = -5.999f; cameraMoon->zcoord[2] = -26.600; cameraMoon->zcoord[3] = -3.899f;

	cameraMoon->NextLink = cameraHum;
	//____________________________________
	
	cameraHum->EyeCoord[0] = 0.0f;
	cameraHum->EyeCoord[1] = 0.0f;
	cameraHum->EyeCoord[2] = 22.0f;

	cameraHum->CenterCoord[0] = 4.0f;
	cameraHum->CenterCoord[1] = -2.5f;
	cameraHum->CenterCoord[2] = 0.0f;

	//bezierCurve For Humonid
	cameraHum->xcoord[0] = 0.000f; cameraHum->xcoord[1] = -17.000f; cameraHum->xcoord[2] = 34.299f; cameraHum->xcoord[3] =  0.000f;
	cameraHum->ycoord[0] = 1.300f; cameraHum->ycoord[1] =  -8.200f; cameraHum->ycoord[2] = -6.999f; cameraHum->ycoord[3] = -6.398f;
	cameraHum->zcoord[0] = 14.00f; cameraHum->zcoord[1] = -5.999f; cameraHum->zcoord[2] = -26.600f; cameraHum->zcoord[3] = 22.580f;

	cameraHum->NextLink = cameraBird;

	//____________________________________

	cameraBird->EyeCoord[0] = 0.0f;
	cameraBird->EyeCoord[1] = 0.0f;
	cameraBird->EyeCoord[2] = 22.0f;

	cameraBird->CenterCoord[0] = -7.0f;
	cameraBird->CenterCoord[1] = 0.0f;
	cameraBird->CenterCoord[2] = 2.0f;
	

	//bezierCurve For Bird
	cameraBird->xcoord[0] = -4.999f; cameraBird->xcoord[1] = -4.999f; cameraBird->xcoord[2] = -28.501f; cameraBird->xcoord[3] = -2.899f;
	cameraBird->ycoord[0] =  0.000f; cameraBird->ycoord[1] =  0.000f; cameraBird->ycoord[2] =   0.000f; cameraBird->ycoord[3] =  0.000f;
	cameraBird->zcoord[0] =  7.000f; cameraBird->zcoord[1] = 12.000f; cameraBird->zcoord[2] = -26.600f; cameraBird->zcoord[3] =  1.167f;

	cameraBird->NextLink = NULL;
	//____________________________________


	//Font Rendering
	listBase = CreateBitmapFont("Comic Sans MS", 48);	// create the bitmap font
	
	//Display List For Tree

	guiTreeList = glGenLists(1);
	glNewList(guiTreeList, GL_COMPILE);
		// code
		glPushMatrix();

		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glScalef(1.5f, 1.5f, 2.5f);
		glPushMatrix();

		glColor3f(0.8f, 0.42f, 0.0f);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.25f, 0.0f, 3.5f, 60, 60);

		for (GLfloat i = 0.0f; i < 2.0f; i += 0.4f) {
			glPushMatrix();

			glColor3f(0.0f, 1.0f - (i / 2.0f), 0.0f);
			glTranslatef(0.0f, 0.0f, 3.5f - (i + 0.5));
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.2f + i, 0.0f, 0.5f, 60, 60);

			glPopMatrix();
		}

		glPopMatrix();

		glPopMatrix();

	glEndList();

	//Loading Texture (userdefine function)
	LoadGLTexture(&guiMoon_texture, MAKEINTRESOURCE(MOON_BITMAP));
	LoadGLTexture(&guiBg_texture, MAKEINTRESOURCE(BG_BITMAP));	
	LoadGLTexture(&guiGround_texture, MAKEINTRESOURCE(GROUND_BITMAP));
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Blending 
	glEnable(GL_BLEND);
	
	// Mandatory Function To Cheaking Alpha Bits With Reference Value
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Accessory Funtion To Remove Jagged Edges While Kepping Depth Effect Intact
	glShadeModel(GL_SMOOTH);

	// Mandatory Function To Initialise Depth Buffer With All Ones
	glClearDepth(1.0f);

	// Mandatory Function To Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	// Mandatory Function To Cheaking Depth Bits With Reference Value
	glDepthFunc(GL_LEQUAL);

	// Accessory Funtion To Smoothing Corner Shapes
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Lights Properties
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

	glEnable(GL_LIGHT1);

	
	// Warm-up Call To Resize
	resize(WIN_WIDTH, WIN_HEIGHT);

	fprintf(gpFile, "EXIT initialize()\n");
}

bool LoadGLTexture(GLuint* texture,TCHAR resourceid[]) {

	bool bResult = false;
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	//code
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), resourceid, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	if (hBitmap) {
		bResult = true;

		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		DeleteObject(hBitmap);

	}
	return(bResult);

}

void resize(int width, int height) {
	fprintf(gpFile, "ENRTY resize()\n");

	// code
	if (height == 0) {
		height = 1;
	}
	giWidth = width;
	giHeight = height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 2000.0f);

	fprintf(gpFile, "EXIT resize()\n");
}

void display(void) {

	//Function Prototype
	void update(void);
	void HDisplay(void);
	void OwlHead(void);
	void Tree(void);
	void Jail(void);
	void Moon(void);
	void Star(void);
	void World(void);
	void Intro(void);
	void IntroText(void);
	void ExitText(void);

	//temp function 
	void DrawPath(void);

	//Local VAriables
	//static GLfloat t = 1.0f;
	// code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	if (gbRunCamera) {

		if (t >= 0.0f) {

			camX = (pow(1.0f - t, 3.0f)) * (cameraMoon->xcoord[0]) + 3.0f * t * (pow(1.0f - t, 2.0f)) * (cameraMoon->xcoord[1]) + 3.0f * (pow(t, 2.0f)) * (1.0f - t) * (cameraMoon->xcoord[2]) + (pow(t, 3.0f)) * (cameraMoon->xcoord[3]);
			camY = (pow(1.0f - t, 3.0f)) * (cameraMoon->ycoord[0]) + 3.0f * t * (pow(1.0f - t, 2.0f)) * (cameraMoon->ycoord[1]) + 3.0f * (pow(t, 2.0f)) * (1.0f - t) * (cameraMoon->ycoord[2]) + (pow(t, 3.0f)) * (cameraMoon->ycoord[3]);
			camZ = (pow(1.0f - t, 3.0f)) * (cameraMoon->zcoord[0]) + 3.0f * t * (pow(1.0f - t, 2.0f)) * (cameraMoon->zcoord[1]) + 3.0f * (pow(t, 2.0f)) * (1.0f - t) * (cameraMoon->zcoord[2]) + (pow(t, 3.0f)) * (cameraMoon->zcoord[3]);

			t -= TSPEED;	
		}
	//	For Moon
		gluLookAt(	camX, camY, camZ,	/*Eye-Coordinate*/
					cameraMoon->CenterCoord[0], cameraMoon->CenterCoord[1], cameraMoon->CenterCoord[2],	/*Center-Coordinate*/
					0.0f, 1.0f, 0.0f);	/*Up-Vector*/
	// For Humonoid
		//gluLookAt(camX, camY, camZ,	/*Eye-Coordinate*/
		//	cameraHum->CenterCoord[0], cameraHum->CenterCoord[1], cameraHum->CenterCoord[2],	/*Center-Coordinate*/
		//	0.0f, 1.0f, 0.0f);	/*Up-Vector*/
	// For Bird
		//gluLookAt(camX, camY, camZ,	/*Eye-Coordinate*/
		//	cameraBird->CenterCoord[0], cameraBird->CenterCoord[1], cameraBird->CenterCoord[2],	/*Center-Coordinate*/
		//	0.0f, 1.0f, 0.0f);	/*Up-Vector*/

	}

	if (gbResetCamera) {
		gluLookAt(0, 0, 24,	/*Eye-Coordinate*/
			0.0f, 0.0f, 0.0f,	/*Center-Coordinate*/
			0.0f, 1.0f, 0.0f);	/*Up-Vector*/
	}
	/*glPushMatrix();
	glDisable(GL_LIGHTING);
		DrawPath();
	glEnable(GL_LIGHTING);
	glPopMatrix();*/

	if (gbIntroFlag) {
		
		gluLookAt(0, 0, 22,	/*Eye-Coordinate*/
			0.0f, 0.0f, 0.0f,	/*Center-Coordinate*/
			0.0f, 1.0f, 0.0f);	/*Up-Vector*/

		glDisable(GL_LIGHTING);
		glPushMatrix();
			Intro();
		glPopMatrix();

		glPushMatrix();
			IntroText();
		glPopMatrix();
		glEnable(GL_LIGHTING);

	}

	if (gbExitDemoFlag) {

		glDisable(GL_LIGHTING);
		glPushMatrix();
		Intro();
		glPopMatrix();

		glPushMatrix();
		ExitText();
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}

	if (gbHDisplayFlag) {
		glPushMatrix();
			HDisplay();
		glPopMatrix();
	}

		
	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	World();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	glPushMatrix();
		OwlHead();
	glPopMatrix();

	for (float i = -5.0f; i < 5.0f; i += 1.0f) {
		glPushMatrix();
			glTranslatef(i*3.5, -10.5f, -10.0f);
			//Tree();
			glCallList(guiTreeList);
			glTranslatef(i*3.5f, -0.0f, -10.0f);
			//Tree();
			glCallList(guiTreeList);
			glTranslatef(i * 3.5f, -0.0f, -10.0f);
			//Tree();
			glCallList(guiTreeList);
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, guiMoon_texture);
	Moon();
	glPopMatrix();

	glPushMatrix();
		Jail();
	glPopMatrix();
	
	update();

	SwapBuffers(ghdc);
}

//void DrawPath() {
//	// General Formula For 3 Point 
//	// P = (1−t)^3P1 + 3(1−t)^2tP2 + 3(1−t)t^2P3 + t^3P4
//	
//	//formula for X
//	//glTranslatef(0.0f, 50.0f, -3.0f);
//	glColor4f(1, 1, 1, 1);
//	glBegin(GL_LINE_STRIP);
//		for (t = 0.0f; t <= 1.0f; t += 0.1f) {
//			/*x = (pow(1.0f - t, 3.0f)) * (-22.0f) + 3.0f * t * (pow(1.0f - t, 2.0f)) * (-50.0f) + 3.0f * (pow(t, 2.0f)) * (1.0f - t) * (50.0f) + (pow(t, 3.0f)) * (22.0f);
//			y = (pow(1.0f - t, 3.0f)) * (-22.0f) + 3.0f * t * (pow(1.0f - t, 2.0f)) * (-50.0f) + 3.0f * (pow(t, 2.0f)) * (1.0f - t) * (0.0f) + (pow(t, 3.0f)) * (-22.0f);
//			z = (pow(1.0f - t, 3.0f)) * (-22.0f) + 3.0f * t * (pow(1.0f - t, 2.0f)) * (-50.0f) + 3.0f * (pow(t, 2.0f)) * (1.0f - t) * (50.0f) + (pow(t, 3.0f)) * (22.0f);*/
//
//			x = (pow(1.0f - t, 3.0f)) * (xcoord[0]) + 3.0f * t * (pow(1.0f - t, 2.0f)) * (xcoord[1]) + 3.0f * (pow(t, 2.0f)) * (1.0f - t) * (xcoord[2]) + (pow(t, 3.0f)) * (xcoord[3]);
//			y = (pow(1.0f - t, 3.0f)) * (ycoord[0]) + 3.0f * t * (pow(1.0f - t, 2.0f)) * (ycoord[1]) + 3.0f * (pow(t, 2.0f)) * (1.0f - t) * (ycoord[2]) + (pow(t, 3.0f)) * (ycoord[3]);
//			z = (pow(1.0f - t, 3.0f)) * (zcoord[0]) + 3.0f * t * (pow(1.0f - t, 2.0f)) * (zcoord[1]) + 3.0f * (pow(t, 2.0f)) * (1.0f - t) * (zcoord[2]) + (pow(t, 3.0f)) * (zcoord[3]);
//
//			glVertex3f(x, y, z);
//		}
//	glEnd();
//
//	glPointSize(5.0f);
//
//	glBegin(GL_POINTS);
//		glColor4f(1, 0, 0, 1);
//		glVertex3f(xcoord[0], ycoord[0], zcoord[0]);
//		glVertex3f(xcoord[1], ycoord[1], zcoord[1]);
//		glVertex3f(xcoord[2], ycoord[2], zcoord[2]);
//		glVertex3f(xcoord[3], ycoord[3], zcoord[3]);
//	glEnd();
//	glColor4f(1, 1, 1, 1);
//	//formula for Y
//	//formula for Z
//	
//}

void Intro(void) {


	glTranslatef(0.0f, 0.0f, 0.0f);

	glColor4f(0, 0, 0, gfScaleFor1stIntoduction);
	//glScalef(gfScaleFor1stIntoduction, gfScaleFor1stIntoduction,1.0f);
	glBegin(GL_QUADS);

	glVertex3f(20.0f, 10.0f, 3.0f);
	glVertex3f(-20.0f,10.0f, 3.0f);
	glVertex3f(-20.0f, -10.0f,3.0f);
	glVertex3f(20.0f, -10.0f, 3.0f);

	glEnd();
	

}

void IntroText(void) {

	//Function Prototype
	void PrintString(unsigned int, char*);

	//code
	glColor4f(1.0f, 1.0f, 1.0f, gfAlphaFor1stIntoduction);
	V
	glRasterPos3f(-2.2, 2.0f, 5.0f);
	PrintString(listBase, "- WELCOME -");
	I
	glRasterPos3f(-1.1, 1.0f, 5.0f);
	PrintString(listBase, " TO ");
	Y
	glRasterPos3f(-4.5, 0.0f, 5.0f);
	PrintString(listBase, " SIMPLE BIRD RESCUE GAMEPLAY | v1.0 ");
	O
	glRasterPos3f(-4.5, -1.0f, 5.0f);
	PrintString(listBase, " Group Member: Rohit Kulkarni ");

	G
	glRasterPos3f(-4.5, -2.0f, 5.0f);
	PrintString(listBase, " Group Name: Texture Group");

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}

void ExitText(void) {

	//Function Prototype
	void PrintString(unsigned int, char*);

	//code
	glTranslatef(0.0f, gfTheEndY, 4.0f);
	glColor4f(1.0f, 1.0f, 1.0f, gfAlphaForLastIntoduction);

	R
	glRasterPos3f(-2.5, 15.0f, 5.0f);
	PrintString(listBase, "- Technologies & Softwares -");

	G
	glRasterPos3f(-3.0, 14.0f, 5.0f);
	PrintString(listBase, "Win32 API For Native Windowing");

	B
	glRasterPos3f(-3.6, 13.0f, 5.0f);
	PrintString(listBase, "OpenGL (The Soul Of Real Time Rendering)");

	Y
	glRasterPos3f(-5.3, 12.0f, 5.0f);
	PrintString(listBase,"Photoshop-CC17 For Color Correction And Texture Editing");
	
	R
	glRasterPos3f(-3.2, 11.0f, 5.0f);
	PrintString(listBase, "Audacity 2.4 For Audio Fade In-Out");
	
	G
	glRasterPos3f(-1.0, 9.0f, 5.0f);
	PrintString(listBase, "- Books -");

	B
	glRasterPos3f(-4.0, 8.0f, 5.0f);
	PrintString(listBase, "OpenGL Programming Guide(RED BOOK) 3rd Edition");

	Y
	glRasterPos3f(-2.8, 7.0f, 5.0f);
	PrintString(listBase, "OpenGL Super Bible 3rd Edition");

	R
	glRasterPos3f(-1.5, 5.0f, 5.0f);
	PrintString(listBase, "- Soundtrack -");

	G
	glRasterPos3f(-3.0, 4.0f, 5.0f);
	PrintString(listBase, "Transformers: Dark Of The Moon");
	
	B
	glRasterPos3f(-2.3, 3.0f, 5.0f);
	PrintString(listBase, "Artist: Steve Jablonsky");

	R
	glRasterPos3f(-2.0, 1.0f, 5.0f);
	PrintString(listBase, "- SPECAIL THANKS -");

	O
	glRasterPos3f(-1.9, 0.0f, 5.0f);
	PrintString(listBase, "Archana Jethale (GL)");

	O
	glRasterPos3f(-1.4, -1.0f, 5.0f);
	PrintString(listBase, "Pradnya Ma'am");

	O
	glRasterPos3f(-1.4, -2.0f, 5.0f);
	PrintString(listBase, "Gokhale Ma'am");

	O
	glRasterPos3f(-0.3, -3.0f, 5.0f);
	PrintString(listBase, "AND");

	O
	glRasterPos3f(-2.4,-4.0f, 5.0f);
	PrintString(listBase, "DR. VIJAY D. GOKHALE SIR");

	R
	glRasterPos3f(-1.2,-6.0f, 5.0f);
	PrintString(listBase, "- THE END -");


}

void HDisplay(void) {

	void HHead(void);
	void HSmile(void);
	void HNeck(void);
	void HChest(void);
	void HRightHand(void);
	void HLeftHand(void);
	void HRightLeg(void);
	void HLeftLeg(void);
	void HWheel(void);
	void HHeart(void);

	glTranslatef(gftX, gftY, gftZ);
	glRotatef(gfAngleHead-30, 0.0f, 1.0f, 0.0f);
	
	glRotatef(inclineAngle, 1.0f, 0.0f, 0.0f);
	glPushMatrix();

	HHead();
	HSmile();
	HNeck();
	HChest();
	HLeftHand();
	HRightHand();
	HLeftLeg();
	HRightLeg();
	HWheel();
	HHeart();
	glPopAttrib();
	glPopMatrix();

}

void OwlHead(void) {

	//Function Prototype

	//Code
	glTranslatef(gftXBird, gftYBird, gftZBird);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	glPushMatrix();

			//Owl Body
			glPushMatrix();
				glScalef(1.0f, 1.24f, 1.0f);
				glColor4fv(owlBodyColor);
				quadric = gluNewQuadric();
				gluSphere(quadric, 0.35f, 100, 100);
			glPopMatrix();

			//Owl Beak
			glPushMatrix();
				glTranslatef(0.0f, 0.0f, 0.285f);
				glScalef(0.9f, 0.8f, 0.2f);
				glRotatef((GLfloat)180, 0.0f, 0.0f, 1.0f);
				glRotatef((GLfloat)15, 1.0f, 0.0f, 0.0f);
				glColor4fv(owlBeakColor);
				glTranslatef(0.0f, 0.0f, 0.1f);
				quadric = gluNewQuadric();
				gluSphere(quadric, 0.3, 5, 200);
			glPopMatrix();

			//eys-outer cirle
			glPushMatrix();
			
			glTranslatef(-0.12f, 0.1f, 0.28f);
				glScalef(1.0f, 1.0f, 0.5f);
				glColor4fv(owlEyesColor);
				quadric = gluNewQuadric();
				gluSphere(quadric, 0.2, 100, 100);

				//eye pupil
				glColor3f(0.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, 0.2f);
				quadric = gluNewQuadric();
				gluDisk(quadric, 0.0f, 0.1f, 30, 30);

				glTranslatef(0.05f, 0.05f, 0.01f);
				glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
				quadric = gluNewQuadric();
				gluDisk(quadric, 0.0f, 0.02f, 30, 30);

				//Ears
				glTranslatef(-0.04f, 0.24f, -0.6f);
				glColor4f(1.0f, 0.76f, 0.85f, 1.0f);
				quadric = gluNewQuadric();
				gluPartialDisk(quadric, 0.0f, 0.04f, 30, 30, -110.0f, 180.0f);

			glPopMatrix();

			//eys-outer cirle
			glPushMatrix();

				glTranslatef(0.12f, 0.1f, 0.28f);
				glScalef(1.0f, 1.0f, 0.5f);
				glColor4fv(owlEyesColor);
				quadric = gluNewQuadric();
				gluSphere(quadric, 0.2, 100, 100);

				//eyepupil
				glColor3f(0.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, 0.2f);
				quadric = gluNewQuadric();
				gluDisk(quadric, 0.0f, 0.1f, 30, 30);

				glTranslatef(0.05f, 0.05f, 0.01f);
				glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
				quadric = gluNewQuadric();
				gluDisk(quadric, 0.0f, 0.02f, 30, 30);

				//Ears
				glTranslatef(-0.04f, 0.24f, -0.6f);
				glColor4f(1.0f, 0.76f, 0.85f, 1.0f);
				quadric = gluNewQuadric();
				gluPartialDisk(quadric, 0.0f, 0.04f, 30, 30, -90, 220.0f);

			glPopMatrix();

			//LEGS
			glColor4f(1.0f, 0.76f, 0.85f, 1.0f);
		
			glPushMatrix();

				glTranslatef(-0.1f, -0.39f, 0.0f);
				glRotatef((GLfloat)90, 1.0f, 0.0f, 0.0f);
				quadric = gluNewQuadric();
				gluCylinder(quadric, 0.05f, 0.01f, 0.2f, 30, 30);

				glTranslatef(0.0f, 0.0f, 0.19f);
				glScalef(1.0f, 1.0f, 0.3f);
				quadric = gluNewQuadric();
				gluSphere(quadric, 0.04f, 60, 60);

			glPopMatrix();

			glPushMatrix();

				glTranslatef(0.1f, -0.39f, 0.0f);
				glRotatef((GLfloat)90, 1.0f, 0.0f, 0.0f);
				quadric = gluNewQuadric();
				gluCylinder(quadric, 0.05f, 0.01f, 0.2f, 30, 30);

				glTranslatef(0.0f, 0.0f, 0.19f);
				glScalef(1.0f, 1.0f, 0.3f);
				quadric = gluNewQuadric();
				gluSphere(quadric, 0.04f, 60, 60);

			glPopMatrix();

			glPushMatrix();

				glTranslatef(0.30f, 0.0f, 0.0f);
				glScalef(0.8f, 0.8f, 1.0f);
				glRotatef((GLfloat)90.0f, 1.0f, 0.0f, 0.0f);
				glRotatef((GLfloat)-sin(gfWingsAngle) * 60, 0.0f, 1.0f, 1.0f);

				glBegin(GL_TRIANGLE_FAN);

				glColor3f(0.90f, 0.32f, 0.50f);
				glVertex3f(0.0f, 0.0f, 0.0f);

				R
				glVertex3f(0.1f, 0.3f, 0.0f);
				glVertex3f(0.2f, 0.3f, 0.0f);

				O
				glVertex3f(0.2f, 0.2f, 0.0f);
				glVertex3f(0.3f, 0.2f, 0.0f);

				Y
				glVertex3f(0.3f, 0.1f, 0.0f);
				glVertex3f(0.4f, 0.1f, 0.0f);

				G
				glVertex3f(0.5f, 0.0f, 0.0f);
				glVertex3f(0.5f, 0.0f, 0.0f);

				B
				glVertex3f(0.4f, -0.1f, 0.0f);
				glVertex3f(0.3f, -0.1f, 0.0f);

				I
				glVertex3f(0.3f, -0.2f, 0.0f);
				glVertex3f(0.2f, -0.2f, 0.0f);

				V
				glVertex3f(0.2f, -0.3f, 0.0f);
				glVertex3f(0.1f, -0.3f, 0.0f);

				glEnd();
	
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.30f, 0.0f, 0.0f);
			glScalef(0.8f, 0.8f, 1.0f);
			glRotatef((GLfloat)90.0f, 1.0f, 0.0f, 0.0f);
			glRotatef((GLfloat)sin(gfWingsAngle) * 60, 0.0f, 1.0f, 1.0f);


			glBegin(GL_TRIANGLE_FAN);

			glColor3f(0.90f, 0.32f, 0.50f);
			glVertex3f(0.0f, 0.0f, 0.0f);

			R
			glVertex3f(-0.1f, 0.3f, 0.0f);
			glVertex3f(-0.2f, 0.3f, 0.0f);

			O
			glVertex3f(-0.2f, 0.2f, 0.0f);
			glVertex3f(-0.3f, 0.2f, 0.0f);

			Y
			glVertex3f(-0.3f, 0.1f, 0.0f);
			glVertex3f(-0.4f, 0.1f, 0.0f);

			G
			glVertex3f(-0.5f, 0.0f, 0.0f);
			glVertex3f(-0.5f, 0.0f, 0.0f);

			B
			glVertex3f(-0.4f, -0.1f, 0.0f);
			glVertex3f(-0.3f, -0.1f, 0.0f);

			I
			glVertex3f(-0.3f, -0.2f, 0.0f);
			glVertex3f(-0.2f, -0.2f, 0.0f);

			V
			glVertex3f(-0.2f, -0.3f, 0.0f);
			glVertex3f(-0.1f, -0.3f, 0.0f);

			glEnd();

		glPopMatrix();

	//tail
		glPushMatrix();
		
			glTranslatef(0.0f, -0.2f, -0.345f);
			glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
			glScalef(1.2f, 0.6f, 1.0f);
			quadric = gluNewQuadric();
			gluSphere(quadric, 0.04f, 60, 60);

		glPopMatrix();

	glPopMatrix();

}

void Tree(void) {

	//Function Prototype
	void Star(void);

	// code
	glPushMatrix();

		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glPushMatrix();

			glColor3f(0.8f, 0.42f, 0.0f);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.25f, 0.0f, 3.5f, 60, 60);

			for (GLfloat i = 0.0f; i < 2.0f; i += 0.4f) {
				glPushMatrix();

				glColor3f(0.0f, 1.0f - (i / 2.0f), 0.0f);
				glTranslatef(0.0f, 0.0f, 3.5f - (i + 0.5));
				quadric = gluNewQuadric();
				gluCylinder(quadric, 0.2f + i, 0.0f, 0.5f, 60, 60);

				glPopMatrix();
			}

		glPopMatrix();

	glPopMatrix();
}

void Star(void) {

	glScalef(0.3f, 0.3f, 1.0f);
	glColor4f(1.0, 1.0, 1.0f, 1.0f);
	glPushMatrix();
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.4f, 0.0f);
		glVertex3f(-0.1f, -0.1f, 0.0f);
		glVertex3f(0.1f, -0.1f, 0.0f);
		glEnd();
	glPopMatrix();

	glPushMatrix();

	glRotatef(71.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.4f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glRotatef(135.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.4f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();

	glRotatef(225.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.4f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glRotatef(289.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.4f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	glEnd();
	glPopMatrix();

}

void Jail(void) {

	//Function Prototype

	// code
	glTranslatef(gftXJail, gftYJail, gftZJail);
	glPushMatrix();
		materialAmbient[0] = 0.19225f;	//r
		materialAmbient[1] = 0.19225f;	//g
		materialAmbient[2] = 0.19225f;	//b
		
		materialDiffuse[0] = 0.75754f;	//r
		materialDiffuse[1] = 0.75754f;	//g
		materialDiffuse[2] = 0.75754f;	//b

		materialSpecular[0] = 0.508273f;	//r
		materialSpecular[1] = 0.508273f;	//g
		materialSpecular[2] = 0.508273f;	//b

		materialShineness = 0.4f * 128;

	if (gbRescueBird == false) {
		materialAmbient[3] = 1.0f;		//a
		materialDiffuse[3] = 1.0f;		//a
		materialSpecular[3] = 1.0f;		//a

	}
	else {
		materialAmbient[3] = 0.0f;		//a
		materialDiffuse[3] = 0.0f;		//a
		materialSpecular[3] = 0.0f;		//a
	}
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glBegin(GL_QUADS);

	//Top
	glNormal3f(0.0f, 1.0f, 0.0f);

	glVertex3f(1.1f, 1.0f, 1.1f);
	glVertex3f(-1.1f, 1.0f, 1.1f);
	glVertex3f(-1.1f, 1.0f, -1.1f);
	glVertex3f(1.1f, 1.0f, -1.1f);

	//Bottom Face
	glNormal3f(0.0f, -1.0f, 0.0f);

	glVertex3f(1.1f, -1.0f, 1.1f);
	glVertex3f(-1.1f, -1.0f, 1.1f);
	glVertex3f(-1.1f, -1.0f, -1.1f);
	glVertex3f(1.1f, -1.0f, -1.1f);

	glEnd();

	glRotatef((GLfloat)90.0f, 1.0f, 0.0f, 0.0f);

	for (GLfloat x = -1.0f; x <= 1.0f; x += 0.5f) {

		glPushMatrix();

		glTranslatef(x, 1.0f, -1.0f);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.03f, 0.03f, 2.0f, 3, 3);

		glPopMatrix();

		glPushMatrix();

		glTranslatef(-1.0f, x, -1.0f);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.03f, 0.03f, 2.0f, 3, 3);

		glPopMatrix();

		glPushMatrix();

		glTranslatef(x, -1.0f, -1.0f);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.03f, 0.03f, 2.0f, 3, 3);

		glPopMatrix();

		glPushMatrix();

		glTranslatef(1.0f, x, -1.0f);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.03f, 0.03f, 2.0f, 3, 3);

		glPopMatrix();
	}

	glRotatef((GLfloat)-90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef((GLfloat)90.0f, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	glTranslatef(1.0f, 0.0f, 0.0f);
	quadric = gluNewQuadric();
	gluPartialDisk(quadric, 0.3f, 0.4f, 30, 30, 0.0f, 180.0f);

	glPopMatrix();

	glPopMatrix();

}

void HHead(void) {



	glPushMatrix();

		glBegin(GL_QUADS);

		materialAmbient[0] = 0.19225f;	//r
		materialAmbient[1] = 0.19225f;	//g
		materialAmbient[2] = 0.19225f;	//b
		materialAmbient[3] = 1.0f;		//a

		materialDiffuse[0] = 0.75754f;	//r
		materialDiffuse[1] = 0.75754f;	//g
		materialDiffuse[2] = 0.75754f;	//b
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

		//Front Face
		glNormal3f(0.0f, 0.0f, 1.0f);

		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		//eyes

		materialAmbient[0] = 0.0f;	//r
		materialAmbient[1] = 0.1f;	//g
		materialAmbient[2] = 0.06f;	//b
		materialAmbient[3] = 1.0f;		//a

		materialDiffuse[0] = 0.0f;	//r
		materialDiffuse[1] = 0.90980392f;	//g
		materialDiffuse[2] = 0.90980392f;	//b
		materialDiffuse[3] = 1.0f;		//a

		materialSpecular[0] = 0.90196078f;	//r
		materialSpecular[1] = 0.90196078f;	//g
		materialSpecular[2] = 0.50196078f;	//b
		materialSpecular[3] = 1.0f;		//a

		materialShineness = 128;

		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
		glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);
		
		glNormal3f(0.0f, 0.0f, 1.0f);

		glVertex3f(0.8f, 0.7f, 1.0f);
		glVertex3f(-0.8f, 0.7f, 1.0f);
		glVertex3f(-0.8f, 0.4f, 1.0f);
		glVertex3f(0.8f, 0.4f, 1.0f);
		
		glEnd();
		
		glBegin(GL_QUADS);

		materialAmbient[0] = 0.19225f;	//r
		materialAmbient[1] = 0.19225f;	//g
		materialAmbient[2] = 0.19225f;	//b
		materialAmbient[3] = 1.0f;		//a

		materialDiffuse[0] = 0.75754f;	//r
		materialDiffuse[1] = 0.75754f;	//g
		materialDiffuse[2] = 0.75754f;	//b
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

		//Right Face
		glNormal3f(1.0f, 0.0f, 0.0f);

		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

		//Back Face
		glNormal3f(0.0f, 0.0f, -1.0f);

		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

		//Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);

		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);

		//Top Face
		glNormal3f(0.0f, 1.0f, 0.0f);

		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);

		////Bottom Face
		glNormal3f(0.0f, -1.0f, 0.0f);

		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

		glEnd();

	glPopMatrix();
}

void ThrowLaser(void) {

	materialAmbient[0] = 1.0f;	//r
	materialAmbient[1] = 0.01175f;		//g
	materialAmbient[2] = 0.01175f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 1.0f;	//r
	materialDiffuse[1] = 0.04136f;	//g
	materialDiffuse[2] = 0.04136f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 1.0f;	//r
	materialSpecular[1] = 0.626959f;	//g
	materialSpecular[2] = 0.626959f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 0.6f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glPushMatrix();
	glTranslatef(-0.6f,0.5f,1.0);
	quadric = gluNewQuadric();
	gluCylinder(quadric, 0.0f, 0.09f, gfLaserheight, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6f, 0.5f,1.0f);
	quadric = gluNewQuadric();
	gluCylinder(quadric, 0.0f, 0.09f, gfLaserheight, 10, 10);
	glPopMatrix();

}

void HSmile(void) {

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.1f;	//g
	materialAmbient[2] = 0.06f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.0f;	//r
	materialDiffuse[1] = 0.90980392f;	//g
	materialDiffuse[2] = 0.90980392f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.90196078f;	//r
	materialSpecular[1] = 0.90196078f;	//g
	materialSpecular[2] = 0.50196078f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 1.1f);
		quadric = gluNewQuadric();
		gluPartialDisk(quadric, 0.4, 0.5, 100, 50, 120, 120);
	glPopMatrix();

}

void HNeck(void) {
	// Function Prototype

	//code
	glPushMatrix();

		glTranslatef(0.0f, -1.0f, 0.0f);
		glRotatef(90, 1.0f, 0.0f, 0.0f);

		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.4, 0.4, 0.5, 30, 30);

	glPopMatrix();
}

void HChest(void) {
	// Function Prototype

	//code
	glPushMatrix();

		glTranslatef(0.0f, -2.8f, 0.0f);
		glRotatef(gfAngleChest, 0.0f, 1.0f, 0.0f);
		glScalef(1.0f, 1.0f, 0.65f);

		glPushMatrix();
		materialAmbient[0] = 0.19225f;	//r
		materialAmbient[1] = 0.19225f;	//g
		materialAmbient[2] = 0.19225f;	//b
		materialAmbient[3] = 1.0f;		//a

		materialDiffuse[0] = 0.75754f;	//r
		materialDiffuse[1] = 0.75754f;	//g
		materialDiffuse[2] = 0.75754f;	//b
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

			quadric = gluNewQuadric();
			gluSphere(quadric, 1.45f, 6, 6);
		glPopMatrix();

	glPopMatrix();	

}

void HRightHand(void) {

	glPushMatrix();

		glTranslatef(-1.8f, -2.5f, 0.0f);
		glRotatef((GLfloat)90, 0.0f, 0.0f, 1.0f);
		glRotatef((GLfloat)shoulder, 0.0f, 1.0f, 0.0f);

		glPushMatrix();

			glScalef(1.0f, 0.5f, 0.8f);
			quadric = gluNewQuadric();
			gluSphere(quadric, 0.5f, 50, 50);

		glPopMatrix();

			glTranslatef(0.6f, 0.0f, 0.0f);
			glRotatef((GLfloat)elbow, 0.0f, 1.0f, 0.0f);
			glTranslatef(1.0f, 0.0f, 0.0f);

		glPushMatrix();

			glScalef(2.0f, 0.5f, 0.8f);
			quadric = gluNewQuadric();
			gluSphere(quadric, 0.5f, 50, 50);
	
			if (gbLaser) {
				glPushMatrix();

					materialAmbient[0] = 1.0f;	//r
					materialAmbient[1] = 0.01175f;		//g
					materialAmbient[2] = 0.01175f;	//b
					materialAmbient[3] = 1.0f;		//a

					materialDiffuse[0] = 1.0f;	//r
					materialDiffuse[1] = 0.04136f;	//g
					materialDiffuse[2] = 0.04136f;	//b
					materialDiffuse[3] = 1.0f;		//a

					materialSpecular[0] = 1.0f;	//r
					materialSpecular[1] = 0.626959f;	//g
					materialSpecular[2] = 0.626959f;	//b
					materialSpecular[3] = 1.0f;		//a

					materialShineness = 0.6f * 128;

					glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

					glRotatef(90, 0.0f, 1.0f, 0.0f);
					glTranslatef(0.0f, 0.0f, 0.0f);
					quadric = gluNewQuadric();
					gluCylinder(quadric, 0.09f, 0.1f, gfLaserheight, 10, 10);

					materialAmbient[0] = 0.19225f;	//r
					materialAmbient[1] = 0.19225f;	//g
					materialAmbient[2] = 0.19225f;	//b
					materialAmbient[3] = 1.0f;		//a

					materialDiffuse[0] = 0.75754f;	//r
					materialDiffuse[1] = 0.75754f;	//g
					materialDiffuse[2] = 0.75754f;	//b
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

				glPopMatrix();
			}
		glPopMatrix();

	glPopMatrix();
}

void HLeftHand(void) {

	glPushMatrix();

		glTranslatef(1.8f, -2.5f, 0.0f);
		glRotatef((GLfloat)90, 0.0f, 0.0f, 1.0f);
		glRotatef((GLfloat)shoulder, 0.0f, 1.0f, 0.0f);

		glPushMatrix();

			glScalef(1.0f, 0.5f, 0.8f);
			quadric = gluNewQuadric();
			gluSphere(quadric, 0.5f, 50, 50);

		glPopMatrix();

			glTranslatef(0.6f, 0.0f, 0.0f);
			glRotatef((GLfloat)elbow, 0.0f, 1.0f, 0.0f);
			glTranslatef(1.0f, 0.0f, 0.0f);

		glPushMatrix();

			glScalef(2.0f, 0.5f, 0.8f);
			quadric = gluNewQuadric();
			gluSphere(quadric, 0.5f, 50, 50);
			if (gbLaser) {
				glPushMatrix();

					materialAmbient[0] = 1.0f;	//r
					materialAmbient[1] = 0.01175f;		//g
					materialAmbient[2] = 0.01175f;	//b
					materialAmbient[3] = 1.0f;		//a

					materialDiffuse[0] = 1.0f;	//r
					materialDiffuse[1] = 0.04136f;	//g
					materialDiffuse[2] = 0.04136f;	//b
					materialDiffuse[3] = 1.0f;		//a

					materialSpecular[0] = 1.0f;	//r
					materialSpecular[1] = 0.626959f;	//g
					materialSpecular[2] = 0.626959f;	//b
					materialSpecular[3] = 1.0f;		//a

					materialShineness = 0.6f * 128;

					glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

					glRotatef(90, 0.0f, 1.0f, 0.0f);
					glTranslatef(0.0f, 0.0f, 0.0f);
					quadric = gluNewQuadric();
					gluCylinder(quadric, 0.09f, 0.1f, gfLaserheight, 10, 10);

					materialAmbient[0] = 0.19225f;	//r
					materialAmbient[1] = 0.19225f;	//g
					materialAmbient[2] = 0.19225f;	//b
					materialAmbient[3] = 1.0f;		//a

					materialDiffuse[0] = 0.75754f;	//r
					materialDiffuse[1] = 0.75754f;	//g
					materialDiffuse[2] = 0.75754f;	//b
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
				glPopMatrix();
			}
		glPopMatrix();

	glPopMatrix();
}

void HRightLeg(void) {

	glPushMatrix();

	glTranslatef(-0.8f, -4.5f, 0.0f);
	glRotatef((GLfloat)180, 1.0f, 0.0f, 0.0f);

	glPushMatrix();

	glScalef(0.5f, 0.99f, 0.8f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.6f, 100, 100);

	glPopMatrix();

	glTranslatef(0.0f, 0.6f, 0.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glRotatef((GLfloat)0, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.7f, 0.0f, 0.0f);
	glRotatef(-10, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	glScalef(1.5f, 0.5f, 0.8f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.5f, 100, 100);

	glPopMatrix();

	glPopMatrix();
}

void HLeftLeg(void) {

	glPushMatrix();

	glTranslatef(0.8f, -4.5f, 0.0f);
	glRotatef((GLfloat)180, 1.0f, 0.0f, 0.0f);

	glPushMatrix();

	glScalef(0.5f, 0.99f, 0.8f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.6f, 100, 100);

	glPopMatrix();

	glTranslatef(0.0f, 0.6f, 0.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glRotatef((GLfloat)0, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.7f, 0.0f, 0.0f);
	glRotatef(10, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	glScalef(1.5f, 0.5f, 0.8f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.5f, 100, 100);

	glPopMatrix();

	glPopMatrix();
}

void HWheel(void) {

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.1f;	//g
	materialAmbient[2] = 0.06f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.0f;	//r
	materialDiffuse[1] = 0.90980392f;	//g
	materialDiffuse[2] = 0.90980392f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = 0.90196078f;	//r
	materialSpecular[1] = 0.90196078f;	//g
	materialSpecular[2] = 0.50196078f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glPushMatrix();
		
		glTranslatef(-0.25f, -6.0f, -0.0);
		glScalef(2.9f, 2.9f, 2.9f);
		glRotatef(90, 0.0, 1.0f, 0.0f);
		glRotatef(gftX*2, 0.0, 0.0f, 1.0f);
		
		glPushMatrix();

			quadric = gluNewQuadric();
			gluDisk(quadric, 0.2f, 0.4, 50, 50);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.2f, 0.2f, 0.2f, 50, 50);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.4f, 0.4f, 0.2f, 50, 50);
		
			glPopMatrix();
		
		glTranslatef(0.0f, 0.0f, 0.2);
		quadric = gluNewQuadric();
		gluDisk(quadric, 0.2f, 0.4, 30, 30);

	glPopMatrix();
}

void HHeart(void) {

	materialAmbient[0] = 0.0f;	//r
	materialAmbient[1] = 0.0f;	//g
	materialAmbient[2] = 0.0f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = fabs(sin(hbeat) * 0.5f);	//r
	materialDiffuse[1] = 0.0f;	//g
	materialDiffuse[2] = 0.0f;	//b
	materialDiffuse[3] = 1.0f;		//a

	materialSpecular[0] = fabs(sin(hbeat) * 0.7f);	//r
	materialSpecular[1] = 0.6f;	//g
	materialSpecular[2] = 0.6f;	//b
	materialSpecular[3] = 1.0f;		//a

	materialShineness = 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShineness);

	glTranslatef(0.0f, -2.4f, 0.89f);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);

	glPushMatrix();
	glRotatef((GLfloat)180, 0.0f, 0.0f, 1.0f);
	glScalef(1.0f, 1.1f, 0.3f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.25, 5, 200);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.12f, 0.1f, 0.0f);
	glScalef(1.0f, 1.0f, 0.3f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.12f, 0.1f, 0.0f);
	glScalef(1.0f, 1.0f, 0.3f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2, 100, 100);
	glPopMatrix();

	glPopMatrix();

	
}

void Moon(void) {

	materialAmbient[0] = 0.19225f;	//r
	materialAmbient[1] = 0.19225f;	//g
	materialAmbient[2] = 0.19225f;	//b
	materialAmbient[3] = 1.0f;		//a

	materialDiffuse[0] = 0.75754f;	//r
	materialDiffuse[1] = 0.75754f;	//g
	materialDiffuse[2] = 0.75754f;	//b
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

	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(13.0f, 12.0f, -10.0f);	// Moon Position
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluSphere(quadric, 4.0f, 50, 50);

}

void World(void) {


	
	glBindTexture(GL_TEXTURE_2D, guiBg_texture);

	glBegin(GL_QUADS);
	//Behind Moon
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(70.0f, 40.0f, -70.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-70.0f, 40.0f, -70.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-70.0f, 0.0f, -70.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(70.0f,  0.0f, -70.0f);

	//Left
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-70, 40.0f, -70.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-70, 40.0f, 70.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-70, 0.0f, 70.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-70, 0.0f, -70.0f);

	//Infront Of Moon
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(70.0f, 40.0f, 70.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-70.0f, 40.0f, 70.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-70.0f, 0.0f, 70.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(70.0f, 0.0f, 70.0f);

	//right
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(70, 40.0f, -70.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(70, 40.0f, 70.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(70, 0.0f, 70.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(70, 0.0f, -70.0f);


	glEnd();

	//GROUND
	glBindTexture(GL_TEXTURE_2D, guiGround_texture);
	glBegin(GL_QUADS);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(70.0f, -12.0f, -70.0f);
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-70.0f, -12.0f, -70.0f);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-70.0f, -12.0f, 70.0f);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(70.0f, -12.0f, 70.0f);
	glEnd();

	
}

void update(void) {

	// Function Prototype

	//code
	if (!gbIntroFlag) {
		hbeat += 0.04;
		gfWingsAngle += 0.05;

	}
	
	if (gbIntroFlag) {
		gfScaleFor1stIntoduction >= 1.0f ? gfAlphaFor1stIntoduction -= 0.007f : gfAlphaFor1stIntoduction = 0.0f;
		if (gfAlphaFor1stIntoduction <= 0.0f) {
			gfScaleFor1stIntoduction -= 0.01f;
		
			if (gfScaleFor1stIntoduction <= 0.0f) {
				gfScaleFor1stIntoduction = 0.0f;
				gbIntroFlag = false;
				gbMoonCameraFlag = true;
				gbRunCamera = true;
				gbHumCameraFlag = false;
				gbBirdCameraFlag = false;
				gbIntroHum = true;
				fprintf(gpFile, "HERE 1\n");
			}
		}
	}

	//going for humonoid camera
	if (gbMoonCameraFlag) {
		if (t<=0.01f) {
			Temp = cameraMoon;
			cameraMoon = cameraMoon->NextLink; // move to next camera postion, here moving to humonoid
			t = 1; //reset timeline
			gbMoonCameraFlag = false;
			gbHumCameraFlag = true;
			fprintf(gpFile, "HERE 2\n");

		}
	}
	//going for bird camera
	if (gbHumCameraFlag) {
		//if (camX >= cameraHum->xcoord[3] || camY >= cameraHum->ycoord[3] || camZ >= cameraHum->zcoord[3]) {
		if (t<=0.01f) {
			cameraMoon = cameraMoon->NextLink; // move to next camera postion, here moving to humonoid
			t = 1; //reset timeline
			gbMoonCameraFlag = false;
			gbHumCameraFlag = false;
			gbBirdCameraFlag = true;
			fprintf(gpFile, "HERE 3\n");

		}
	}

	if (gbBirdCameraFlag) {
		if (t <= 0.01f) {
			gbRunCamera = false;
			gbResetCamera = true;
			gbStartAction = true;
		}
	}
	

	if (gbIntroHum) {
		
		gftY = gftY >= 0 ? gftY - 0.05 : 0;
		gbIntroHum = gftY <= 0.0f ? false : true;
		gbIntroBird = gbIntroHum ? false : true;
	}

	if (gbIntroBird) {

		gftYBird = gftYBird >= 0.0f ? gftYBird - 0.03f : 0;
		gftYJail = gftYJail >= 0.0f ? gftYJail - 0.03f : 0;
		gbIntroBird = gftYBird <= 0.0f ? false : true;

	}
	static GLfloat iterator1 = 0.0f;
	if (gbRescueBird) {
		gftZBird += 0.009f;
		gftXBird += 0.02f;
		gftYBird = sin(iterator1 += 0.01);

		if (gftXBird >= 12.5f) {
			gbRescueBird = false;
			gbExitDemoFlag = true;
		}

			gbLaser = false;
			gbByeBye = true;
			gbStartAction = false;

	}

	if (gbExitDemoFlag) {
		gfScaleFor1stIntoduction += 0.02f;
		gfAlphaForLastIntoduction += 0.01f;
		gfTheEndY += 0.015f;
		if (gfScaleFor1stIntoduction >= 15.0f) {
			gfScaleFor1stIntoduction = 15.0f;
			gbIntroFlag = false;
			gbIntroHum = false;
			gbIntroBird = false;
			gbRescueBird = false;
			
		}

	}

	if (gbStartAction && gftY <=0.0f) {
		gfAngleHead -= 0.4f;
		if (gfAngleHead <= -50.0f) {
			gfAngleHead = -50.0f;

		}
	}

	if (gfAngleHead <= -50) {
		gftX -= 0.04f;
		if (gftX <= -2.8) {
			gftX = -2.8f;
			gbMoveShoulder = true;
		}
	}

	if (gbMoveShoulder) {
		shoulder = (shoulder + 1) % 360;
		if (shoulder >=300) {
			shoulder = 300;
		}
	}

	if (gbByeBye) {
		gbMoveShoulder = false;

		gfAngleHead += 0.2f;
		
		if (gfAngleHead >= 50.0f) {
			gfAngleHead = 50.0f;
		}

		if (shoulder != 180) {
			shoulder = (shoulder - 1)%360;
		}
		
	}
	if (gbLaser) {
		gfLaserheight += 0.009f;
		if (gfLaserheight >= 1.5f) {
			gfLaserheight = 1.5f;
		}

		shoulder = 300+(12 * sin(hbeat));
	}
}

unsigned int CreateBitmapFont(char* fontName, int fontSize) {

	//Font Rendering
	HFONT hFont;
	unsigned int base;

	base = glGenLists(96);

	//Font Creation



	if (strcmpi(fontName, "symbol") == 0) {
		hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, SYMBOL_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, fontName);

	}
	else {
		hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, fontName);

	}
	// Error Checking
	if (!hFont) {
		fprintf(gpFile, "CreateFont() Failed\n");
		DestroyWindow(ghwnd);
	}

	//Select A Device Context For The Font
	SelectObject(ghdc, hFont);

	//Prepare The Bitmap Font
	wglUseFontBitmaps(ghdc, 32, 96, base);

	return base;
}

void PrintString(unsigned int base, char* str) {

	if ((base == 0) || (str == NULL)) {
		return;
	}

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);
	glPopAttrib();

}

void ClearFont(unsigned int base) {

	if (base != 0) {
		glDeleteLists(base, 96);
	}

}

void uninitialize(void) {
	fprintf(gpFile, "ENRTY uninitialize()\n");

	// code
	if (cameraBird) {
		free(cameraBird);
		cameraBird = NULL;
	}
	if (cameraHum) {
		free(cameraHum);
		cameraHum = NULL;
	}
	if (Temp) {
		free(Temp);
		Temp = NULL;
	}
	if (cameraMoon) {
		free(cameraMoon);
		cameraMoon = NULL;
	}
	if (gbFullscreen == true) {

		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}

	if (wglGetCurrentContext() == ghrc) {
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc) {
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc) {
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	gluDeleteQuadric(quadric);

}

