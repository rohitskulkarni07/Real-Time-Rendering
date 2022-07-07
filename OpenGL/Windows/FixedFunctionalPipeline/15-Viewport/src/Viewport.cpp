#include <Windows.h>	// All Win32 API
#include <stdio.h>		// For File IO  and exit()
#include "MyWindowWithIcon.h"


#include <gl/gl.h>		// For All OpenGL Releted Functions
#include <gl/glu.h>		// For All Wrapper(Utility) Funtions In OpenGL

#pragma comment(lib,"opengl32.lib") // Import Library For OpenGL Functions
#pragma comment(lib,"glu32.lib")	// Import Library For Utility Functions

#define WIN_WIDTH	800		// Initial Windows Width
#define WIN_HEIGHT	600		// Initial Windows Height

GLsizei vWidth;
GLsizei vHeight;

// Global Function Declaration
// Callback

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable Declaration

FILE*	gpFile	= NULL;	// Log File
HWND	ghwnd	= NULL;	// Global Window Handle
HDC		ghdc	= NULL; // Global Device Context
HGLRC	ghrc	= NULL;	// Rendering Context
DWORD	dwStyle = NULL; // Style Holder For Previous Style

WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; // Window's Previous Placement Holder

bool gbFullscreen   = false; // To Toggle Fullscreen
bool gbActiveWindow = false; // For Active Window (Focused Window)

// GUI ENTRY POINT
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	// Function Prototype
	void initialize(void);	// init()
	void display(void);		// draw()

	// Local Variable

	WNDCLASSEX	wndclass = { sizeof(WNDCLASSEX) };
	MSG			msg = {};
	HWND		hwnd = NULL;
	TCHAR		szAppName[255] = TEXT("CLASS_MYOPENGL_WINDOW");

	bool		bDone = false; // For Game Loop

	if (fopen_s(&gpFile, "Log.txt", "w") != 0) {
		MessageBox(NULL, TEXT("LOG FILE CREATION FAILED... PROGRAM WILL EXIT NOW!!!"), TEXT("PROGRAM ERROR"), MB_OK | MB_ICONEXCLAMATION);
		exit(0);
	}
	else {
		fprintf(gpFile, "File \"log.txt\" Created Succesfully... Program Started Successfully\n");
	}

	// Initialize WNDCLASSEX struc

	wndclass.cbSize			= sizeof(WNDCLASSEX);
	wndclass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= szAppName;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	RegisterClassEx(&wndclass);

	fprintf(gpFile, "Class Registered Succesfully\n");

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Viewport"),
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

	fprintf(gpFile, "CreateWindow() Successfull\n");

	// Initializig OpenGL Compatibility
	initialize();

	// Show Window From Memory

	ShowWindow(hwnd, iCmdShow);
	fprintf(gpFile, "ShowWindow() Successfull\n");

	// Replacement Of UpdateWindow
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// The Game Loop
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
				// display()
				display();
				// update()
				// update();
			}
		}
	}


	return((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	// Function Prototype
	void ToggleFullscreen(void);
	void display(void);
	void resize(int, int);
	void uninitialize(void);

	// Local Variable
	
	// Code

	switch (iMsg) {

	case WM_SETFOCUS:
		gbActiveWindow = true;

		break;
	case WM_KILLFOCUS:
		gbActiveWindow = false;

		break;
		
	case WM_ERASEBKGND:
		return(0);

	case WM_SIZE:
		vWidth  = LOWORD(lParam);
		vHeight = HIWORD(lParam);
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		
		switch (wParam) {
		case VK_ESCAPE:
			DestroyWindow(hwnd);

			break;
		case 0x46:
			ToggleFullscreen();
			break;

		case 48:
		case VK_NUMPAD0:
			glViewport(0, 0, (GLfloat)vWidth, (GLfloat)vHeight); // Fullsize
			break;

		case 49:
		case VK_NUMPAD1:
			glViewport(0, vHeight/2, (GLfloat)vWidth/2, (GLfloat)vHeight/2);	// Left-top

			break;

		case 50:
		case VK_NUMPAD2:
			glViewport(vWidth/2, vHeight/2, (GLfloat)vWidth/2, (GLfloat)vHeight/2);		// Right-top

			break;

		case 51:
		case VK_NUMPAD3:
			glViewport(vWidth / 2, 0, (GLfloat)vWidth / 2, (GLfloat)vHeight / 2);	// Right-bottom

			break;

		case 52:
		case VK_NUMPAD4:
			glViewport(0, 0, (GLfloat)vWidth / 2, (GLfloat)vHeight / 2);	// Left-bottom

			break;

		case 53:
		case VK_NUMPAD5:
			glViewport(0, 0, (GLfloat)vWidth / 2, (GLfloat)vHeight);	// Left-Whole

			break;

		case 54:
		case VK_NUMPAD6:
			glViewport(vWidth/2, 0, (GLfloat)vWidth / 2, (GLfloat)vHeight);		// Right-Whole

			break;

		case 55:
		case VK_NUMPAD7:
			glViewport(0, vHeight/2, (GLfloat)vWidth , (GLfloat)vHeight/2);		// Top-Whole

			break;

		case 56:
		case VK_NUMPAD8:
			glViewport(0,0, (GLfloat)vWidth, (GLfloat)vHeight / 2);		// Bottom8-Whole

			break;

		case 57:
		case VK_NUMPAD9:
			glViewport(vWidth/4, vHeight/4, (GLfloat)vWidth/2, (GLfloat)vHeight / 2);	// Center 

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

	// Local Variables
	MONITORINFO mi = { sizeof(MONITORINFO) };	// The MONITORINFO structure contains information about a display monitor.

	// Code

	if (gbFullscreen == false) {

		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);	// Retrieves information about the specified window , Here Retrives Current Styel Of Given Window


		if (dwStyle & WS_OVERLAPPEDWINDOW) {

			if ((GetWindowPlacement(ghwnd, &wpPrev)) && (GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))) {
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

				// The SetWindowPos function changes the size, position, and Z order of a
				// child, pop-up, or top-level window. Child, pop-up, and top-level windows
				// are ordered according to their appearance on the screen. The topmost window
				// receives the highest rank and is the first window in the Z order. 
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
		SetWindowPlacement(ghwnd, &wpPrev);		// Store the Window's Prevous Placement
		SetWindowPos(ghwnd,
			HWND_TOP,
			0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		gbFullscreen = false;
		
	}

}

void initialize(void) {

	fprintf(gpFile, "Entry In initialize()\n");

	// Function Prototype
	void resize(int,int);


	// Local Variable
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	// Code
	ghdc = GetDC(ghwnd);

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR)); // Similar to memset((void*)&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));


	// initialize Struct PIXELFORMATEDESCRIPTOR For OpenGL Compatibility

	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType	= PFD_TYPE_RGBA;
	pfd.cColorBits	= 32;
	pfd.cRedBits	= 8;
	pfd.cGreenBits	= 8;
	pfd.cBlueBits	= 8;
	pfd.cAlphaBits	= 8;

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

	ghrc = wglCreateContext(ghdc);

	if (ghrc == NULL) {
		fprintf(gpFile, "wglCreateContext() Failed\n");
		DestroyWindow(ghwnd);
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE) {
		fprintf(gpFile, "wglMakeCurrent() Failed\n");
		DestroyWindow(ghwnd);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	resize(WIN_WIDTH, WIN_HEIGHT);

	fprintf(gpFile, "Exit From initialize()\n");

}

void resize(int width, int height) {
	fprintf(gpFile, "Entry In resize()\n");

	if (height == 0) {
		height = 1;
	}

	glViewport(0, 0, (GLfloat)width, (GLfloat)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	fprintf(gpFile, "Exit From resize()\n");

}


void display(void) {

	//code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();

	SwapBuffers(ghdc);	
}

void uninitialize(void) {
	fprintf(gpFile, "Entry In uninitialize()\n");

	//code

	if (gbFullscreen == false) {
		
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

	if (gpFile) {
		fclose(gpFile);
		gpFile = NULL;
	}
}


// /a