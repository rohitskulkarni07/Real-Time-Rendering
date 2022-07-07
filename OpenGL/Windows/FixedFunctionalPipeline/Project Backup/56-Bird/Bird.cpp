#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "Bird.h"

#include <gl/gl.h>	// graphic library
#include <gl/glu.h> // graphic library utility

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")

//#pragma comment(lib,"kernel32.lib")
//#pragma comment(lib,"winmm.lib")


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

bool gbPolyMode = false; // Polygon Mode

//Lights Changes
bool gbLight = false; // By Default Lighting Is Off

GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 0.1f };			//Gray
GLfloat lightDeffuse[] = { 1.0f, 0.0f, 0.0f, 0.1f };			//white
GLfloat lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };			//position of light

//colors
// color macro

#define R glColor3f(255.0f/255.0f , 0.0f, 0.0f);
#define O glColor3f(255.0f/255.0f, 165.0f/255.0f ,0.0f);
#define Y glColor3f(255.0f/255.0f, 255.0f/255.0f ,0.0f);
#define G glColor3f(0.0f, 128.0f/255.0f ,0.0f);
#define B glColor3f(0.0f, 0.0f, 255.0f/255.0f);
#define I glColor3f(75.0f/255.0f, 0.0f, 130.0f/255.0f);
#define V glColor3f(238.0f/255.0f, 130.0f/255.0f, 238.0f/255.0f);
//RGB: (186, 139, 87)
//GLfloat owlBodyColor[4] = { 0.72f, 0.54f, 0.34f, 1.0f };

//RGB: (244, 194, 194)
GLfloat owlBodyColor[4] = {0.90f, 0.32f, 0.50f, 1.0f};

//RGB: (252, 245, 102)For Yellow
GLfloat owlEyesColor[4] = { 1.0f, 1.0f, 0.4f, 1.0f };

GLfloat owlBeakColor[4] = { 1.0f, 0.17f, 0.33f, 1.0f };

//Wings Moment
GLfloat gfWingsAngle = 0.0f;

//Translation of Body
GLfloat gftX = 0.0f, gftY = 0.0f, gftZ = 0.0f;

// Humanoid Variables

#define HWALK_SPEED 0.2f

GLint shoulder = 270;
GLint elbow = 0;
GLint heap = 180;
GLint knee = 0;
GLfloat gfAngleHead = 0.0f;
bool gbHeadFlag = true;
GLfloat gfAngleChest = 0.0f;
GLUquadric* quadric = NULL;
GLfloat hbeat = 1.0f;
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
		TEXT("Humanoid"),
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

		case 0x46:
		case 0x66:
			// Call ToggleFullscreen()
			ToggleFullscreen();

			break;

		case VK_RIGHT:
			gftX += HWALK_SPEED;
			break;

		case VK_LEFT:
			gftX -= HWALK_SPEED;
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
	case WM_CHAR:

		switch (LOWORD(wParam)) {

			// humanoid moment
		case 'r':

			if (!(gfAngleHead <= -360)) {
				gfAngleHead = gfAngleHead - 1.4f;
			}
			break;

		case 'R':

			if (!(gfAngleHead >= 360)) {
				gfAngleHead = gfAngleHead + 1.4f;
			}
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
			gbPolyMode = gbPolyMode ? false : true;
			if (gbPolyMode == true) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;
		case 'Y':
			gftY += HWALK_SPEED;
			break;
		case 'y':
			gftY -= HWALK_SPEED;
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

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDeffuse);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

	glEnable(GL_LIGHT1);
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

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	fprintf(gpFile, "EXIT resize()\n");
}

GLfloat gfAngle = 0.0f;


void display(void) {

	//Function Prototype
	void update(void);
	void OwlHead(void);


	// code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			
	glTranslatef(gftX, gftY, gftZ);

	glRotatef(gfAngleHead, 0.0f, 1.0f, 0.0f);
	glPushMatrix();

	OwlHead();
	

	glPopMatrix();

	update();

	SwapBuffers(ghdc);
}

void OwlHead(void) {

	//Function Prototype

	//Code

	glTranslatef(0.0f, fabs(sin(gfWingsAngle)), 0.0f);
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
			glColor4f(1.0f, 1.0f, 1.0f,0.7f);
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
			glColor4f(1.0f, 1.0f, 1.0f,0.7f);
			quadric = gluNewQuadric();
			gluDisk(quadric, 0.0f, 0.02f, 30, 30);

			//Ears
			glTranslatef(-0.04f, 0.24f, -0.6f);
			glColor4f(1.0f, 0.76f, 0.85f, 1.0f);
			quadric = gluNewQuadric();
			gluPartialDisk(quadric, 0.0f, 0.04f, 30, 30, -90, 220.0f);
		glPopMatrix();

		glColor4f(1.0f,0.76f,0.85f,1.0f);
		//LEGS
		glPushMatrix();

			glTranslatef(-0.1f, -0.39f, 0.0f);
			glRotatef((GLfloat)90, 1.0f, 0.0f, 0.0f);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.05f, 0.01f, 0.2f, 30,30);

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
			glRotatef((GLfloat)sin(gfWingsAngle)*60, 0.0f, 1.0f, 1.0f);


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

void update(void) {

	// Function Prototype
	gfWingsAngle += 0.01;
	//code
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
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}

}
