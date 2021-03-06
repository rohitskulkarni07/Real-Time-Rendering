#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "MyWindowWithIcon.h"

#include <gl/gl.h>	// graphic library
#include <gl/glu.h> // graphic library utility

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
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

bool gbFullscreen = false;
bool gbActiveWindow = false;

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
		TEXT("Dynamic India"),
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
				if (gbFullscreen == true) {
					display();
				}
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
		case VK_SPACE:
				PlaySound(MAKEINTRESOURCE(INDIASOUND), NULL, SND_RESOURCE | SND_ASYNC);
				break;
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
		if (gbFullscreen) {
			PlaySound(MAKEINTRESOURCE(INDIASOUND), NULL, SND_RESOURCE | SND_ASYNC);
		}
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

	//Accessory Funtion To Remove Jagged Edges While Kepping Depth Effect Intact
	glShadeModel(GL_SMOOTH);

	//Mandatory Function To Initialise Depth Buffer With All Ones
	glClearDepth(1.0f);

	//Mandatory Function To Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	//Mandatory Function To Cheaking Depth Bits With Reference Value
	glDepthFunc(GL_LEQUAL);

	//Accessory Funtion To Smoothing Corner Shapes
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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

void display(void) {

	//Function Prototype
	void update(void);
	void DrawI(void);
	void DrawN(void);
	void DrawD(void);
	void DrawA(void);
	void DrawFlag(void);
	void DrawFighter(void);
	void DrawIAFF(void);
	void DrawBackground(void);


	// code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(0.0, 0.0f, -3.0f);
	DrawBackground();

	//_______Left 'I'__________
	
	glLoadIdentity();
	glTranslatef(gftLeftIX, 0.0f, -2.5f);	//X,Y,Z For Left I Position(-1.2f, 0.0f, -2.5f)
	glLineWidth(20.0f);
	DrawI();

	//_______'N'__________

	glLoadIdentity();
	glTranslatef(-0.627f, gftNY, -2.5f);	//X, Y, Z Positin For N (-0.627f, 0.0f, -2.5f);
	glLineWidth(20.0f);
	DrawN();
	
	//_______Left 'I'__________

	glLoadIdentity();
	glTranslatef(0.55f, gftRightIY, -2.5f);		//X,Y,Z For Right I Position(0.55f, 0.0f, -2.5f)
	glLineWidth(20.0f);
	DrawI();
	
	//_______'D'__________

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -2.5f);			//X, Y, Z For D Position (0.0f, 0.0f, -2.5f);
	glLineWidth(20.0f);
	glScalef(gfsDZ, gfsDZ, 0.0f);
	DrawD();

	//_______'A'__________

	glLoadIdentity();
	glTranslatef(gftAX, 0.0f, -2.5f);		//X, Y, Z For Position Of A (1.12f, 0.0f, -2.5f);
	glLineWidth(20.0f);
	DrawA();

	glLoadIdentity();
	glTranslatef(gftFlagX, 0.0f, -2.5f);
	glScalef(0.375f, 0.05f, 1.0f);
	DrawFlag();

	//_________UPPER FIGHTER_______________________________
	
	glLoadIdentity();
	glTranslatef(gftFireX, cos(gftUpFireY) + 0.985, -2.5f);
	glScalef(0.3f, 0.3f, 1.0f);
	glRotatef(gfrUpFireAngle, 0.0f, 0.0f, 1.0f);
	DrawFighter();


	//_________MIDDLE FIGHTER_______________________________

	glLoadIdentity();
	glTranslatef(gftFireX, 0.0f, -2.5f);
	glScalef(0.3f, 0.3f, 1.0f);
	DrawFighter();

	//_________LOWER FIGHTER_______________________________

	glLoadIdentity();
	glTranslatef(gftFireX, -cos(gftLoFireY)-0.985f, -2.5f);
	glScalef(0.3f, 0.3f, 1.0f);
	glRotatef(gfrLoFireAngle, 0.0f, 0.0f, 1.0f);
	DrawFighter();

	//_______________________________________________

	/*glLoadIdentity();
	glTranslatef(0, 0, -2.5f);
	DrawFighter();*/



	update();

	SwapBuffers(ghdc);
}

void update(void) {

	//_____________Update Frames For Left 'I'__________

	if (gbAnimateLeftI == true) {
		gftLeftIX += 0.001f;
	}
	if (gftLeftIX >= -1.2f) {
		gftLeftIX = -1.2f;
		gbAnimateLeftI = false;
		gbAnimateA = true;
	}
	//_____________Update Frames For  'A'______________

	if (gbAnimateA == true) {
		gftAX -= 0.001f;
	}
	if (gftAX <= 1.12f) {
		gftAX = 1.12f;
		gbAnimateA = false;
		gbAnimateN = true;
	}
	
	//_____________Update Frames For  'N'_______________

	if (gbAnimateN == true) {
		gftNY -= 0.001f;
	}
	if (gftNY <= 0.0f) {
		gftNY = 0.0f;
		gbAnimateN = false;
		gbAnimateRightI = true;
	}

	//_____________Update Frames For Right 'I'__________

	if (gbAnimateRightI == true) {
		gftRightIY += 0.001f;
	}
	if (gftRightIY >= 0.0f) {
		gftRightIY = 0.0f;
		gbAnimateRightI = false;
		gbAnimateD = true;
	}

	//_____________Update Frames For 'D'________________

	if (gbAnimateD == true) {
		gfsDZ += 0.001f;
	}
	if (gfsDZ >= 1.0f) {
		gfsDZ = 1.0f;
		gbAnimateD = false;
		gbAnimateFighter = true;
		gbAnimateFlag = true;
	}
	//__________Update Frame For Background and Flag_________________

	if (gbAnimateFlag == true) {
		gftFlagX += 0.001f;
	}
	if (gftFlagX >= 1.12f) {
		gftFlagX = 1.12;
		gbAnimateFlag = false;
	}

	if (gbAnimateBg == true) {
		if (!(gfcWhite >= 1.0f)) {
			gfcWhite += 0.001f;
		}
	}

	//_____________Update Frames For FireFighters_______________
	
	if (gbAnimateFighter == true) {
		gftFireX += 0.001f;

		if (gftFireX >= -0.015f && gftFireX <= 1.4f) {

			gfrUpFireAngle = 0;
			gfrLoFireAngle = 0;
			gftUpFireY = -2.969;
			gftLoFireY = 2.969;
		}
		else {

			gfrUpFireAngle += 0.0225;
			gfrLoFireAngle -= 0.0225;
			gftUpFireY += 0.001f;
			gftLoFireY -= 0.001f;
		}
	}

	//when to animate bg
	if (gftFireX >= 1.4) {
		gbAnimateBg = true;
	}
//_________________________________________________________________
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

void DrawI(void) {


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

void DrawN(void) {



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

void DrawD(void) {


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

void DrawA(void) {


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

void DrawFlag(void) {

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
void DrawFighter(void) {

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


void DrawBackground(void) {


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