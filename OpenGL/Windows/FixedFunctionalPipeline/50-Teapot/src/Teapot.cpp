#include <Windows.h>
#include <stdio.h>


#include "MyWindowWithIcon.h"

#include <gl/gl.h>	// graphic library
#include <gl/glu.h> // graphic library utility

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#include "Teapot.h"

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

//angle for rotating shapes
GLfloat gfAngle = 0.0f;

bool gbAnimate = false;

bool gbTexture = false; // By Default Texture Is Off

bool gbLight = false; // By Default Lighting Is Off

bool gbPolygonMode = false;
//Texture Releted Varaibles GLuint  = OpenGL unsinged integer
GLuint guiTeapot_texture; ;

//Lights Changes
GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };	
GLfloat lightDeffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };	
GLfloat lightPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f };	
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDeffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat materialShininess = 50.0f;

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
		TEXT("Teapot"),
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

		case 'T':
		case 't':
			gbTexture = gbTexture ? false : true;
			if (gbTexture == true) {
				glEnable(GL_TEXTURE_2D);
			}
			else {
				glDisable(GL_TEXTURE_2D);
			}
			break;
		case 'P':
		case 'p':

			gbPolygonMode = gbPolygonMode ? false : true;
			if (gbPolygonMode == true) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

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

	//Loading Texture (userdefine function)
	LoadGLTexture(&guiTeapot_texture, MAKEINTRESOURCE(MARBLE_BITMAP));

	//Enable Texture;
	//glEnable(GL_TEXTURE_2D);

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

	// Lighting Changes
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDeffuse);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDeffuse);

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glEnable(GL_LIGHT1);

	// Warm-up Call To Resize
	resize(WIN_WIDTH, WIN_HEIGHT);

	fprintf(gpFile, "EXIT initialize()\n");
}

/*______________________________________________________________________________________________________________________________________

	Function	:LoadGLTexture
	prameter	:GLuint*	Which Texture to be loaded
	prameter	:TCHAR[]	Resouce id of Texture To be Loaded

	return type :bool		Wheather Function Successfull or not
______________________________________________________________________________________________________________________________________*/

bool LoadGLTexture(GLuint* texture, TCHAR resourceid[]) {
	
	// Local Variable
	bool bResult = false;		// Return value
	HBITMAP hBitmap = NULL;		// Handle to Bitmap
	BITMAP bmp;					// bmp Strucutre's Variable To Store All The Necessary Informormation About Loaded Bitmap

	//code
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), resourceid, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	// Function
	// LoadImage	: win32 api The LoadImage function loads an icon, cursor, or bitmap.
	// Parameter	
	// HINSTANCE	: handle of the instance containing the image GetModuleHandle retrives Current Processe's handle 
	// LPCTSTR		: name or identifier of image here, resource id of loaded texture(bitmap)
	// UINT uType   : type of image	here, file is type of Bitmap hence flag IMAGE_BITMAP
	// int			: desired width	here, 0 for complete loading 
	// int			: desired height	, 0 for complete loading 
	// UINT			: load flags		,LR_CREATEDIBSECTION This flag is useful for loading a bitmap without mapping it to the colors of the display device
	//									When the uType parameter specifies IMAGE_BITMAP, causes the function to return a DIB section bitmap rather than a compatible bitmap


	if (hBitmap) {

		bResult = true;

		// The GetObject function obtains information about a specified graphics object. 
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		// int cbBuffer,      size of buffer for object information
		// HGDIOBJ hgdiobj,   handle to graphics object of interest
		// LPVOID lpvObject   pointer to buffer for object information	

		// The glPixelStoref and glPixelStorei functions set pixel storage modes.
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		// How To Arrange Data In Memory i.e here, UPPACKED But Aligned
		// use 4 Rows For arrenging data in memory

		//The glGenTextures function generates texture names
		glGenTextures(1, texture);
		// 1 denotes total number of texture to be generate
		// here texture is filled with proper texture handle from gpu memory

		//Bind 2d Texture to the texture variable
		glBindTexture(GL_TEXTURE_2D, *texture);


		//Setting Texture Paramters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//Build Mipmaps and push the data to GPU with help of DeviceDriver
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

		//Delete loaded bitmap from memory after it converted into mipmaps	
		DeleteObject(hBitmap);

	}

	return(bResult);

}
//______________________________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________________________

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

	// Function Prototype
	void update(void);

	// code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -1.0f);
	glRotatef(gfAngle, 0.0f, 1.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, guiTeapot_texture);		//change state to texture for next upcoming geometry

	glBegin(GL_TRIANGLES);
	
	for (int i = 0; i < (sizeof(face_indicies)/sizeof(face_indicies[0])); i++) {
		
		for (int j = 0; j < 3; j++) {
		
			int vi = face_indicies[i][j];
			int ni = face_indicies[i][j + 3];
			int ti = face_indicies[i][j + 6];

			glNormal3f(normals[ni][0], normals[ni][1], normals[ni][2]);
			glTexCoord2f(textures[ti][0], textures[ti][1]);
			glVertex3f(vertices[vi][0], vertices[vi][1], vertices[vi][2]);

		}
	}

	glEnd();
	
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

	// remove texture from gpu memory
	glDeleteTextures(1, &guiTeapot_texture);
}

void update(void) {

	if (gbAnimate == true) {

		gfAngle = gfAngle + 0.7f;

		if (gfAngle >= 360) {
			gfAngle = 0;
		}
	}
}

