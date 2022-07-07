#include <Windows.h>
#include <stdio.h>
#include "OGL.h"

#include <gl/glew.h> // graphics library extension wrangler //For GLSL Extensions This Library Must Be Before #include <gl\gl.h>
#include <gl/gl.h>	// graphic library

#include "vmath.h"	/*	This File Comes With RedBook 8th Edition
						header for vector mathematics */

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

using namespace vmath;
 
enum {
	RSK_ATTRIBUTE_POSITION = 0,
	RSK_ATTRIBUTE_COLOR,
	RSK_ATTRIBUTE_NORMAL,
	RSK_ATTRIBUTE_TEXTURE0,
};


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

GLfloat pyramidAngle = 0.0f;
GLfloat cubeAngle = 0.0f;

//Texture Releted Varaibles GLuint  = OpenGL unsinged integer
GLuint guiStone_texture;
GLuint guiKundali_texture;

//--------------GLSL RELETED------------------------------

GLuint vertexShaderObjet;
GLuint fragmentShaderObjet;
GLuint shaderProgramObjet;

//for triangle
GLuint vao_pyramid;
GLuint vbo_position_pyramid;
GLuint vbo_textured_pyramid;

//for quad
GLuint vao_cube;
GLuint vbo_position_cube;
GLuint vbo_textured_cube;

//uniforms
GLuint mvpMatrixUniform;	//for model-view-projection
GLuint textureSamplerUniform;	//for sampler

mat4 perspectiveProjectionMatrix; // from vmath 4x4 Matrix

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
		TEXT("OGL: PP"),
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

	fprintf(gpFile, "Program Executed Succesfully!\n");

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

	case WM_CHAR:
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

	//________________Enable Extenion For GLSL__________________________

	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK) {
		fprintf(gpFile, "glewInit() Failed\n");
		DestroyWindow(ghwnd);
	}
	//________________________________________________________________

	//_________________OpenGL Releted Log_____________________________

	fprintf(gpFile, "\nOpenGL Vendor : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version : %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL Version   : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//________________________________________________________________
	//___________________OpenGL Enable Extentions_____________________
	GLint numOfExtentions = 0;

	glGetIntegerv(GL_NUM_EXTENSIONS, &numOfExtentions);

	for (int i = 0; i < numOfExtentions; i++) {
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	}
	fprintf(gpFile,"\n");
	//________________________________________________________________
	//		
	//			-------   GLSL   -------
	//________________________________________________________________

	/**** VERTEXT SHADER SOURCE CODE****/

	//create shader
	vertexShaderObjet = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	// core profile is used #version 450 core

	const GLchar* vertexShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec2 vTexCoord;" \
		"uniform mat4 u_mvp_matrix;" \
		"out vec2 out_texcoord;" \
		"void main(void)" \
		"{" \
		"gl_Position = u_mvp_matrix * vPosition;" \
		"out_texcoord =  vTexCoord;"
		"}";//end of code

	//	void glShaderSource(GLuint shader,GLsizei count,const GLchar** string, const GLint* length);

	/*	glShaderSource sets the source code in shader to the source code in the array of strings specified
	*	by string.Any source code previously stored in the shader object is completely replaced.The number
		of strings in the array is specified by count.If length is NULL, each string is assumed to be null
		terminated.If length is a value other than NULL, it points to an array containing a string length
		for each of the corresponding elements of string.Each element in the length array may contain the
		length of the corresponding string(the null character is not counted as part of the string length)
		or a value less than 0 to indicate that the string is null terminated.The source code strings are
		not scanned or parsed at this time; they are simply copied into the specified shader object. */
	glShaderSource(vertexShaderObjet, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	glCompileShader(vertexShaderObjet);

	// Error checking

	GLint infoLogLength = 0;
	GLint shaderCompileStatus = 0;
	char* szInfoLog = NULL;

	glGetShaderiv(vertexShaderObjet, GL_COMPILE_STATUS, &shaderCompileStatus);

	if (shaderCompileStatus == GL_FALSE) {
		
		glGetShaderiv(vertexShaderObjet, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {
			
			szInfoLog = (char*)malloc(infoLogLength);
			
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObjet, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "*** Vertext Shader Compilation Status***\n%s\n",szInfoLog);
				free(szInfoLog);
				DestroyWindow(ghwnd);
			}	
		}
	}
	//_______________________________________________________________________________________________________________________

	/**** FRAGMENT SHADER SOURCE CODE****/

	//create shader
	fragmentShaderObjet = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar* fragmentShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec2 out_texcoord;" \
		"uniform sampler2D u_texture_sampler;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = texture( u_texture_sampler, out_texcoord);" \
		"}"; //endofcode

	glShaderSource(fragmentShaderObjet, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(fragmentShaderObjet);

	// Error checking

	glGetShaderiv(fragmentShaderObjet, GL_COMPILE_STATUS, &shaderCompileStatus);

	if (shaderCompileStatus == GL_FALSE) {

		glGetShaderiv(fragmentShaderObjet, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {

			szInfoLog = (char*)malloc(infoLogLength);

			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObjet, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "*** Fragment Shader Compilation Status***\n%s\n", szInfoLog);
				free(szInfoLog);
				DestroyWindow(ghwnd);
			}
		}
	}

	// *** SHADER PROGRAM ***

	// create
	shaderProgramObjet = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(shaderProgramObjet, vertexShaderObjet);

	//attach fragment shader to shader program
	glAttachShader(shaderProgramObjet, fragmentShaderObjet);

	//pre-link binding of shader program object with vertex shader position attribute
	glBindAttribLocation(shaderProgramObjet, RSK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(shaderProgramObjet, RSK_ATTRIBUTE_TEXTURE0, "vTexCoord");

	//link shader
	glLinkProgram(shaderProgramObjet);

	GLint shaderProgramLinkStatus = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	//Error checking
	glGetProgramiv(shaderProgramObjet, GL_LINK_STATUS, &shaderProgramLinkStatus);
	if (shaderProgramLinkStatus == GL_FALSE) {
		
		glGetProgramiv(shaderProgramObjet, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {
			
			szInfoLog = (char*)malloc(infoLogLength);
			
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObjet, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log :\n%s\n", szInfoLog);

				free(szInfoLog);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get MVP uniform location
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObjet, "u_mvp_matrix");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObjet, "u_texture_sampler");
	// *** vertices, colors, shader attribs, vbo, vao initializationa ***
	const GLfloat pyramidVertices[] =
	{	
		//front face
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		//right face
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		//back face
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		//left face
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f

	};
	
	const GLfloat pyramidTexCoord2f[] =
	{	
		// front face
		0.5f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,


		//right face
		0.5f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		//back face
		0.5f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		//left face
		0.5f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f

	};

	const GLfloat cubeVertices[] =
	{	
		//front face
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		//right face
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		//back face
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		//left face
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,

		//top face
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,

		//bottom face
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f
	};

	const GLfloat cubeTexCoord2f[] =
	{
		//front face
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//right face
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		//back face

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		//left face
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		//top face
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		//bottom face
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};
	//__________________________________________________________________________________________

	//For Triangle
	glGenVertexArrays(1, &vao_pyramid); // create named buffer object for vertex buffer object
	glBindVertexArray(vao_pyramid);		// bind vao with buffer object

		//for position of triangle
		glGenBuffers(1, &vbo_position_pyramid);	// creating named buffer object to map VRAM Buffer with Pointer
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position_pyramid); // bind named object with VRAM Buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW); //feed the data to VRAM buffer
		glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);	// how to read data from buffer
		glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);	//Un-Bind

		//for texture of triangle
		glGenBuffers(1, &vbo_textured_pyramid);	//
		glBindBuffer(GL_ARRAY_BUFFER, vbo_textured_pyramid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidTexCoord2f), pyramidTexCoord2f, GL_STATIC_DRAW);
		glVertexAttribPointer(RSK_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	//___________________________________________________________________________________________

	//For Square
	glGenVertexArrays(1, &vao_cube);
	glBindVertexArray(vao_cube);

		//for position of quad
		glGenBuffers(1, &vbo_position_cube);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//for trexture of quad
		glGenBuffers(1, &vbo_textured_cube);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_textured_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexCoord2f), cubeTexCoord2f, GL_STATIC_DRAW);
		glVertexAttribPointer(RSK_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	//___________________________________________________________________________
	
	//Loading Texture (userdefine function)
	LoadGLTexture(&guiStone_texture, MAKEINTRESOURCE(STONE_BITMAP));
	LoadGLTexture(&guiKundali_texture, MAKEINTRESOURCE(KUNDALI_BITMAP));

	//Enable Texture;
	glEnable(GL_TEXTURE_2D);

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

	//glEnable(GL_CULL_FACE);

	//Clear Color Buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//set perspectivecMatrix to identity matrix
	perspectiveProjectionMatrix = mat4::identity();

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
	//										When the uType parameter specifies IMAGE_BITMAP, causes the function to return a DIB section bitmap rather than a compatible bitmap


	if (hBitmap) {

		bResult = true;

		// The GetObject function obtains information about a specified graphics object. 
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		// int cbBuffer,      size of buffer for object information
		// HGDIOBJ hgdiobj,   handle to graphics object of interest
		// LPVOID lpvObject   pointer to buffer for object information	

		// The glPixelStoref and glPixelStorei functions set pixel storage modes.
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		// How To Arrange Data In Memory i.e here, UPPACKED But Aligned
		// use 1x Rows For arrenging data in memory

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
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Delete loaded bitmap from memory after it converted into mipmaps	
		DeleteObject(hBitmap);

	}

	return(bResult);

}
//______________________________________________________________________________________________________________________________________


void resize(int width, int height) {
	fprintf(gpFile, "ENRTY resize()\n");

	// code
	if (height == 0) {
		height = 1;
	}
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f,100.0f);

	fprintf(gpFile, "EXIT resize()\n");
}

void display(void) {

	//
	void update(void);

	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion

	//start using OpenGL program Object
	glUseProgram(shaderProgramObjet);

	//OpenGL Drawing

	//TRIANGLE TRANSFORMATION

	//set model-view & model-view-projection matrices to identity
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	//translate matrix
	mat4 translateMatrix = mat4::identity();
	mat4 scaleMatrix  = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	mat4 rotationMatrixX = mat4::identity();
	mat4 rotationMatrixY = mat4::identity();
	mat4 rotationMatrixZ = mat4::identity();

	
	translateMatrix = vmath::translate(-1.5f, 0.0f, -5.0f);
	rotationMatrix = vmath::rotate(pyramidAngle, 0.0f, 1.0f, 0.0f);

	modelViewMatrix = translateMatrix * rotationMatrix;

	//ORDER OF MULTIPLICATION IMPORTAN BECAUSE MATRIX MULTIPLICATION IS NON_COMUTTATIVE
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	//pass above modelviewprojection matrix to the vertex shader in u_mvp_matrix shader variable ...This is Pre-Multiplied Matrix
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, guiStone_texture);		//change state to texture for next upcoming geometry
	glUniform1i(textureSamplerUniform, 0);
	// *** bind vao_traingle***
	glBindVertexArray(vao_pyramid);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()

	glDrawArrays(GL_TRIANGLES, 0, 12);	// why 12 is last parameter because vertices * triangle faces

	// *** unbind vao ***
	glBindVertexArray(0);

	//SQUARE TRANSFORMATION
	modelViewMatrix = mat4::identity();
	modelViewProjectionMatrix = mat4::identity();
	translateMatrix = vmath::translate(1.5f, 0.0f, -5.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	rotationMatrixX = vmath::rotate(cubeAngle, 1.0f, 0.0f, 0.0f);
	rotationMatrixY = vmath::rotate(cubeAngle, 0.0f, 1.0f, 0.0f);
	rotationMatrixZ = vmath::rotate(cubeAngle, 0.0f, 0.0f, 1.0f);

	modelViewMatrix = translateMatrix * scaleMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ;
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, guiKundali_texture);	//change state to texture for next upcoming geometry
	glUniform1i(textureSamplerUniform, 0);

	// *** bind vao_square ***
	glBindVertexArray(vao_cube);

	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	// *** unbind vao ***
	glBindVertexArray(0);

	//stop using OpenGL program Object
	glUseProgram(0);

	update();
	SwapBuffers(ghdc);
}

void update(void) {

	pyramidAngle = pyramidAngle + 0.1f;
	if (pyramidAngle >= 360) {
		pyramidAngle = 0;
	}
	cubeAngle = cubeAngle + 0.1f;
	if (cubeAngle >= 360) {
		cubeAngle = 0;
	}
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

	// remove texture from gpu memory
	glDeleteTextures(1, &guiStone_texture);
	glDeleteTextures(1, &guiKundali_texture);

	if (vao_pyramid) {
		glDeleteVertexArrays(1, &vao_pyramid);
		vao_pyramid = 0;
	}

	if (vao_cube) {
		glDeleteVertexArrays(1, &vao_cube);
		vao_cube = 0;
	}

	if (vbo_position_pyramid) {
		glDeleteBuffers(1, &vbo_position_pyramid);
		vbo_position_pyramid = 0;
	}

	if (vbo_position_cube) {
		glDeleteBuffers(1, &vbo_position_cube);
		vbo_position_cube = 0;
	}

	if (vbo_textured_pyramid) {
		glDeleteBuffers(1, &vbo_textured_pyramid);
		vbo_textured_pyramid = 0;
	}

	if (vbo_textured_cube) {
		glDeleteBuffers(1, &vbo_textured_cube);
		vbo_textured_cube = 0;
	}
	if (shaderProgramObjet) {
		
		glUseProgram(shaderProgramObjet);

		GLsizei shaderCount;

		glGetProgramiv(shaderProgramObjet, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* p_shader = NULL;

		p_shader = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(shaderProgramObjet, shaderCount, &shaderCount, p_shader);

		for (GLsizei i = 0; i < shaderCount; i++) {

			//detach shader from ith shader
			glDetachShader(shaderProgramObjet, p_shader[i]);

			//delete vertex shader
			glDeleteShader(p_shader[i]);

			p_shader[i] = 0;
		}

		free(p_shader);
		glDeleteProgram(shaderProgramObjet);
		shaderProgramObjet = 0;
		glUseProgram(0);

	}

	//deselect the rendering context
	if (wglGetCurrentContext() == ghrc) {
		wglMakeCurrent(NULL, NULL);
	}

	//delete rendering context
	if (ghrc) {
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	//delete device context
	if (ghdc) {
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

}


//Compile
// cl.exe /c /EHsc /I C:\glew-2.1.0\include OGL.cpp

//Link
//link.exe OGL.obj /LIBPATH:C:\glew - 2.1.0\lib\Release\Win32 glew32.lib user32.lib gdi32.lib kernel32.lib rico.res

//Compile + Link in Single Command
//cl.exe OGL.cpp /I C:\glew-2.1.0\include /link user32.lib gdi32.lib kernel32.lib rico.res /LIBPATH:C:\glew-2.1.0\lib\Release\Win32 glew32.lib