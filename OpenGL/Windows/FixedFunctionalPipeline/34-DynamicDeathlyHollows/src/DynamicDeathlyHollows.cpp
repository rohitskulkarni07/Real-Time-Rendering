#include <Windows.h>
#include <stdio.h>
#include "MyWindowWithIcon.h"
#include <math.h>

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

#define PI 3.141592653589793238


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
BOOL gbRotateLine = TRUE;
BOOL gbRotateTriangle = TRUE;
BOOL gbRotateCircle = TRUE;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	// Function Prototype
	void initialize(void);  // init
	void display(void);		// draw
	void update(void);

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
	wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));


	RegisterClassEx(&wndclass);
	fprintf(gpFile, "Class Registered Successfully\n");

	//Create Window In Memory

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,	//newly added parametr Beacuse CreateWindowEx
		szAppName,
		TEXT("Deathly Hollows : Dynamic"),
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
				// Here You Should Call display() For OpenGL Rendering 
				display();
				update();
			}
		}

	}
	fprintf(gpFile, "Closing File ...Program Succesfully Executed!\n");
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

	// Code

	switch (iMsg) {

	case WM_SETFOCUS:			// This Message Comes When Application Window Get Activated
		gbActiveWindow = true;

		break;

	case WM_KILLFOCUS:			// // This Message Comes When Application Window Is Get Unfocus
		gbActiveWindow = false;

		break;

	case WM_ERASEBKGND:
		return(0); // SOMETHING NEW

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));

		break;

	case WM_KEYDOWN:

		switch (wParam) {

		case VK_ESCAPE:

			DestroyWindow(hwnd);

			break;

		case 0x46:
		case 0x66:
			// Call ToggleFullscreen()
			ToggleFullscreen();

			break;

		default:
			break;
		}

		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
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

	//Play Sound
	//PlaySound(TEXT("harry_potter.wav"),GetModuleHandle(NULL), SND_ASYNC | SND_FILENAME);

	// Warm-up Call To Resize
	resize(WIN_WIDTH, WIN_HEIGHT);

	//fprintf(gpFile, "%lf\n", distAB);
	//fprintf(gpFile, "%lf\n", distBC);
	//fprintf(gpFile, "%lf\n", distAC);
	//fprintf(gpFile, "%lf\n", s);
	//fprintf(gpFile, "%lf\n", K);
	//fprintf(gpFile, "%lf\n", rad);
	//fprintf(gpFile, "%lf\n", ix);
	//fprintf(gpFile, "%lf\n", iy);

	fprintf(gpFile, "EXIT initialize()\n");
}

void resize(int width, int height) {

	// code
	if (height == 0) {
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}


void display(void) {

	//Function prototype
	void DrawGraph(void);
	void DrawTriangle(void);
	void DrawLine(void);
	void DrawCircle(void);
	// code

	glClear(GL_COLOR_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor()
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0f,0.0f, -1.8f);
	
	//DrawGraph();

	if (gbRotateLine == FALSE) {
		gfTranslateLineY = 0.0f;
	}
	glTranslatef(0.0f, gfTranslateLineY, -3.0f);
	glRotatef(gfAngleForLine,0.0f, 0.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawLine();


	if (gbRotateLine == FALSE) {
		gfTranslateTriangleX = 0.0f;
		gfTranslateTriangleY = 0.0f;
	}
	glLoadIdentity();
	glTranslatef(gfTranslateTriangleX,gfTranslateTriangleY , -3.0f);
	glRotatef(gfAngleForTriangle, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawTriangle();

	if (gbRotateCircle == FALSE) {
		gfTranslateCircleX = ix;
		gfTranslateCircleY = iy;
	}
	glLoadIdentity();
	glTranslatef(gfTranslateCircleX, gfTranslateCircleY,-3.0f);
	glRotatef(gfAngleForCircle, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawCircle();


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

}

void DrawGraph(void) {

	static GLfloat y = 0.0;
	static GLfloat x = 0.0;
	//code

	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 1.0f);
	for (y = 0.0f; y <= 1.0f; y = y + 0.05f) {
		glVertex3f(-1.0f, y, 0.0f);
		glVertex3f(1.0f, y, 0.0f);
		glVertex3f(-1.0f, -y, 0.0f);
		glVertex3f(1.0f, -y, 0.0f);
	}

	glColor3f(0.0f, 0.0f, 1.0f);
	for (x = 0.0f; x <= 1.0f; x = x + 0.05f) {
		glVertex3f(x, -1.0f, 0.0f);
		glVertex3f(x, 1.0f, 0.0f);
		glVertex3f(-x, -1.0f, 0.0f);
		glVertex3f(-x, 1.0f, 0.0f);
	}

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);


	glEnd();
}


void DrawTriangle(void) {
	
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

void DrawLine(void) {

	glBegin(GL_LINES);

	glVertex3f(0.0f, 0.5f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);

	glEnd();
}

void DrawCircle() {


	glBegin(GL_POINTS);

	for (GLfloat angle = 0.0f; angle <= 2 * PI; angle = angle + 0.001f) {
	
		glVertex3f(rad * cos(angle), rad * sin(angle), 0.0f);
	}

	glEnd();
}


void update(void) {

	gfTranslateLineY = gfTranslateLineY - 0.0001f;
	if (gfTranslateLineY <= 0.0f) {
		//gfAngleForLine = 360.0f;
		gbRotateLine = FALSE;
	}

	gfTranslateTriangleX = gfTranslateTriangleX + 0.0001f;
	gfTranslateTriangleY = gfTranslateTriangleY + 0.0001f;
	if (gfTranslateTriangleX >= 0.0f && gfTranslateTriangleY>=0.0f) {
		/*gfAngleForTriangle = 360.0f;*/
		gbRotateTriangle = FALSE;
	}

	gfTranslateCircleX = gfTranslateCircleX - 0.0001f;

	if (gfTranslateCircleX <= ix ) {
		gfTranslateCircleX = ix;
	/*	gfTranslateCircleY = iy;
		gfAngleForCircle = 360.0f;*/
		//gbRotateCircle = FALSE;

	}

	gfTranslateCircleY = gfTranslateCircleY + 0.0001f;
	if (gfTranslateCircleY >= iy) {
		gfTranslateCircleY = iy;
		//gbRotateCircle = FALSE;
	}

	if (gfTranslateCircleX <= ix && gfTranslateCircleY >= iy) {
		gbRotateCircle = FALSE;
	}


	gfAngleForLine = gfAngleForLine + 0.012f;
	if (gfAngleForLine >= 360.0f) {
		gfAngleForLine = 360.0f;
	}
	
	gfAngleForTriangle = gfAngleForTriangle + 0.012f;
	if (gfAngleForTriangle >= 360.0f) {
		gfAngleForTriangle = 360.0f;
	}

	gfAngleForCircle = gfAngleForCircle + 0.012f;
	if (gfAngleForCircle >= 360.0f) {
		gfAngleForCircle = 360.0f;
	}
}