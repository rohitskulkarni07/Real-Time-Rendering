#include <Windows.h>
#include "MyWindowWithIcon.h"
#include <stdio.h>


/*____________________________________________________________________________________________________

All OpenGL Related Function's Prototypes Are Resdes Here in gl.h

And opengl32.lib Is A Import Library Which Knows Where The Function's Code resides in opengl32.dll
And Where To Use Tha Code In Running Instance Of Program...
To Link This Library Without Using Command-line We Have Used #pragma Preprocessor To Ask The Linker
That Please Link This Library While Linking My .obj File At The Time Of Excecution
______________________________________________________________________________________________________*/

#include <gl/gl.h>

#pragma comment(lib,"opengl32.lib")

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

// Global Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable
FILE* gpFile = NULL;

HWND ghwnd = NULL;
/*__________________________________________________________________________________________________________
HDC ghdc Is A Specailist Who knows How To Draw On Window ....It Holds All The Details About Current
Device Context(Here Device Means Graphics Device/Card) Its Like Interior Decorator For Our Applications
Window. This Hdc is Works With Single Thread Mode And Only Can Be Use For Retain Mode Graphics And Cannot
Be Used With OpenGl Application Which Is Real Time Graphics(Rendered Mode Graphics), HDC Has All Supported
Pixel Formate Are Arranged in Indexed Wise Postion...
____________________________________________________________________________________________________________*/
HDC	ghdc = NULL;


/*________________________________________________________________________________________________________
Hdc is  Not Powerfull as OpenGL's Deivce Context Handle But The Most Important Thing is This Hdc Can Be
Converted Into OpenGl's Device Context Handler Who's Type is HGLRC and Identifier name is ghrc
Described As Handle to an OpenGL Rendering Context
__________________________________________________________________________________________________________*/
HGLRC ghrc = NULL;

DWORD dwStyle = NULL;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbFullscreen = false;
bool gbActiveWindow = false;



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
		TEXT("OpenGL Blue Colured Triangle"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		100,
		100,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Assign global File Handle With Current Window Handle
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

		fprintf(gpFile, "PostQuitMessage() Succesfull\n");

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

/*_________________________________________________________________________________________________________________________________

PIXELFORMATDESCRIPTOR A 27 Member Structure
	As We Have To Use Powerfull Hdc i.e HGLRC  We Have To Use High Valued Colors... For That We Must Change Pixel's Color Bits, To
	Archieve So We Have Structure Which Describes All Information To Used For Pixel Or We Can Say That For Rendering.
	We Did Not Have To Initialize All The Values Of This Strucutre We Just Need To Initialize Those Values Which We Needed...

	Memebers And What To Initialize With???

	nSize		This value should be set to sizeof(PIXELFORMATDESCRIPTOR).

	nVersion	Specifies the version of this data structure.Microsoft Has Stopped Its Support For
				OpenGL At Version 1, This value should be set to 1.

	dwFlags		A set of bit flags that specify properties of the pixel buffer.
				The properties are generally not mutually exclusive; you can set any combination of bit flags,
				with the exceptions noted. The following bit flag constants are defined.

					PFD_DRAW_TO_WINDOW		This Flag Specify That The buffer can draw to a window or device surface
					PFD_SUPPORT_OPENGL		This Flag Specify That The buffer supports OpenGL drawing.

	iPixelType	Specifies the type of pixel data. The following types are defined.

					PFD_TYPE_RGBA	RGBA pixels. Each pixel has four components in this order: red, green, blue, and alpha.

	cColorBits	Specifies the number of color bitplanes in each color buffer.
				For RGBA pixel types, it is the size of the color buffer, excluding the alpha bitplanes.
				For color-index pixels, it is the size of the color-index buffer Here We Are Using RGBA Flag Hence Its Assign
				To 32bits

	cRedBits	Specifies the number of red bitplanes in each RGBA color buffer.
	cGreenBits	Specifies the number of green bitplanes in each RGBA color buffer.
	cBlueBits	Specifies the number of blue bitplanes in each RGBA color buffer.
	cAlphaBits	Specifies the number of alpha bitplanes in each RGBA color buffer. Alpha bitplanes are not supported.
				here 8 bits are used for each value

	cDepthBits	Specifies the depth of the depth (z-axis) buffer.... For 3D Effect.
_________________________________________________________________________________________________________________________________*/


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

	//pfd.cDepthBits = ? for 3D

	//The ChoosePixelFormat function attempts to match an appropriate pixel format supported by a device context to a given pixel format specification.
	//What Pixel Format We Have Given Is To Be Searched In Standard Format list and Return The Index At Which its found
	//1st Parameter Where To be Searched -> ghdc
	//2nd What To Search->&pfd
	//Return Values Is Index.

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	//Error Checking
	if (iPixelFormatIndex == 0) {
		fprintf(gpFile, "ChoosePixelFormat() Failed\n");
		DestroyWindow(ghwnd);
	}

	//The SetPixelFormat function sets the pixel format of the specified device context to the format specified by the iPixelFormat index.
	//If Pixel FormAt Is Found Then Make This Format As My Current Pixel Format For This Application
	//1st Parameter From Where You Choosed The Pixel Formate->ghdc
	//2nd Parameter At What Index It Resides -> iPixelFormateIndex
	//3rd Parameter  pointer to logical pixel format specification
	//Return VAlue Is BOOL If Succeded Return True And False if Failed

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE) {
		fprintf(gpFile, "SetPixelFormat() Failed\n");
		DestroyWindow(ghwnd);
	}


	//The wglCreateContext function creates a new OpenGL rendering context, 
	//which is suitable for drawing on the device referenced by hdc.
	//The rendering context has the same pixel format as the device context.
	//!st parameter device context of device that the rendering context will be suitable for.
	//If the function succeeds, the return value is a valid handle to an OpenGL rendering context.
	//If the function fails, the return value is NULL

	ghrc = wglCreateContext(ghdc); // Bridging API or Windowing System Integration (WSI)

	if (ghrc == NULL) {
		fprintf(gpFile, "wglCreateContext() Failed\n");
		DestroyWindow(ghwnd);
	}


	//The wglMakeCurrent function makes a specified OpenGL rendering context the calling thread's current rendering context.
	//All subsequent OpenGL calls made by the thread are drawn on the device identified by hdc.
	//You can also use wglMakeCurrent to change the calling thread's current rendering context so it's no longer current.
	//1st Parameter device context of device that OpenGL calls are to be drawn on
	//2nd Parameter OpenGL rendering context to be made the calling thread's current rendering context
	//Return Value When the wglMakeCurrent function succeeds, the return value is TRUE; otherwise the return value is FALSE;

	if (wglMakeCurrent(ghdc, ghrc) == FALSE) {
		fprintf(gpFile, "wglMakeCurrent() Failed\n");
		DestroyWindow(ghwnd);
	}

	// Color To Be Set For Window Backgound
	//The glClearColor function specifies clear values for the color buffers.
	//red, green, blue, alpha		The red, green, blue, and alpha values used when the color buffers are cleared.The default values are all zero.

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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


	//The glViewport function sets the viewport.
	//x, y				The lower - left corner of the viewport rectangle, in pixels.The default is(0, 0).
	//width, height		The widthand height, respectively, of the viewport.When an OpenGL context is first attached to a window,
	//					widthand height are set to the dimensions of that window.
	//GLsizei is typedef for int

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	fprintf(gpFile, "EXIT resize()\n");
}

void display(void) {

	// code

	//The glClear function clears buffers to preset values.
	//GL_COLOR_BUFFER_BIT The buffers currently enabled for color writing.
	//The glClear function sets the bitplane area of the window to values previously selected by glClearColor
	//You can clear multiple color buffers simultaneously by selecting more than one buffer at a time using glDrawBuffer.

	glClear(GL_COLOR_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor()
	
	glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();

	//The glFlush function forces execution of OpenGL functions in finite time.
	//The glFlush function empties all these buffers...All command are in pipe are flushed to screen
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

	//The wglGetCurrentContext function obtains a handle to the current OpenGL rendering context of the calling thread.
	if (wglGetCurrentContext() == ghrc) {
		wglMakeCurrent(NULL, NULL);
	}

	//The wglDeleteContext function deletes a specified OpenGL rendering context
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

/*
cl.exe /c /EHsc OpenGLPreparatoryCode.cpp
link.exe OpenGLPreparatoryCode.obj rico.res kernel32.lib user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

or

cl.exe ColouredTriangle.cpp /link rico.res kernel32.lib gdi32.lib user32.lib
*/