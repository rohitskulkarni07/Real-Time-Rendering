#include <Windows.h>
#include <stdio.h>
#include <math.h>

#include "MyWindowWithIcon.h"

#include <gl/gl.h>	// graphic library
#include <gl/glu.h> // graphic library utility

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")


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

bool gbFullscreen = false;
bool gbActiveWindow = false;

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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	// Function Prototype
	void initialize(void);  // init
	void display(void);		// draw

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
		TEXT("24 - Spheres With Light And Materials"),
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

		default:
			break;
		}

		break;
	case WM_CHAR:

		switch (LOWORD(wParam)) {

		case 'F':
		case 'f':
			// Call ToggleFullscreen()
			ToggleFullscreen();
			break;

		case 'A':
		case 'a':
			gbAnimate = gbAnimate ? false : true;
			break;
		//Light Toggle
		case 'L':
		case 'l':
			gbLight = gbLight ? false : true;
			if (gbLight == true) {
				glEnable(GL_LIGHTING);
			}
			else {
				glDisable(GL_LIGHTING);
			}
			break;

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

		case 'x':
		case 'X':
			keypressed = 1;
			gfAngleX = 0.0f;

			break;

		case 'y':
		case 'Y':
			keypressed = 2;
			gfAngleY = 0.0f;

			break;

		case 'z':
		case 'Z':
			keypressed = 3;
			gfAngleZ = 0.0f;
			break;

		default:
			break;
		}
		break;

	case WM_CLOSE:
		fprintf(gpFile, "WM_CLOSE Recieved\n");
		DestroyWindow(hwnd);

		break;

	case WM_DESTROY:

		fprintf(gpFile, "WM_DESTROY Recieved\n");

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

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f); // Dark Gray Background As Some Materials Are Black
	
	// Warm-up Call To Resize
	resize(WIN_WIDTH, WIN_HEIGHT);

	fprintf(gpFile, "EXIT initialize()\n");
}


void resize(int width, int height) {

	fprintf(gpFile, "ENRTY resize()\n");
	// code
	if (height == 0) {
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

	

	fprintf(gpFile, "EXIT resize()\n");
}


void display(void) {

	// Function Prototype
	void update(void);
	void Draw24Sphere(void);
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
	Draw24Sphere();

	update();
	SwapBuffers(ghdc);
}

void uninitialize(void) {
	fprintf(gpFile, "ENRTY uninitialize()\n");

	// code
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

	if (quadric) {
		for (int i = 0; i < 24; i++) {
			gluDeleteQuadric(quadric[i]);
			quadric[i] = NULL;
		}
	}

}

void update(void) {

	if (gbAnimate == true) {

		if (keypressed == 1) {
			gfAngleX = gfAngleX + 1.0f;
		}
		else if (keypressed == 2) {
			gfAngleY = gfAngleY + 1.0f;
		}
		else if (keypressed == 3) {
			gfAngleZ = gfAngleZ + 1.0f;
		}
	}
}


void Draw24Sphere(void) {

	// Function Prototype

	// Code

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

