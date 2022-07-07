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

//--------------GLSL RELETED VARIABLES------------------------------

GLuint vertexShaderObject;
GLuint fragmentShaderObject;
GLuint shaderProgramObject;

//vao and vbo For Cube
GLuint vao_cube;
GLuint vbo_position_cube;
GLuint vbo_normal_cube;

//Uniforms

GLuint uniform_modelViewMatrixUniform;
GLuint uniform_perspectiveProjection;
GLuint lKeyPressed_uniform;

GLuint ld_uniform;
GLuint kd_uniform;
GLuint lightPosition_uniform;

bool bLight;
bool bAnimate;

// perspective matrix
mat4 perspectiveProjectionMatrix; // from vmath 4x4 Matrix

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
				if (bAnimate) {
					update();
				}

				// Here You Must Call display() For OpenGL Rendering 
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
		switch (LOWORD(wParam))
		{
		case 'l':
		case 'L':
			if (bLight == true) {
				bLight = false;
			}
			else {
				bLight = true;
			}
			break;

		case 'a':
		case 'A':
			if (bAnimate == true) {
				bAnimate = false;
			}
			else {
				bAnimate = true;
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
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	// core profile is used #version 450 core

	const GLchar* vertexShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform int u_lkeypressed;" \
		"uniform vec3 u_ld;" \
		"uniform vec3 u_kd;" \
		"uniform vec4 u_light_position;" \
		"out vec3 diffuse_light;"\
		"void main(void)" \
		"{" \
		"\n"\
		"if(u_lkeypressed == 1) {" 
		"vec4 eye_coordinates = u_model_view_matrix * vPosition;" 				// calculate eye-coordinates |eye calculated by multiplying by position coordinates and model_view_matrix
		"mat3 normal_matrix = mat3(transpose(inverse(u_model_view_matrix)));" 	// calsulate normal-matrix | Normal Matrix is inverse of transpose of upper 3x3 of model-view matrix
		"vec3 t_norm = normalize(normal_matrix*vNormal);" 						// calculate tranformed normals | normalised product of normal_matrix and actual normals
		"vec3 s = normalize(vec3(u_light_position-eye_coordinates));" 		// calculate source (s) source coord = light_source pistion subtracted by eye coordinate
		"diffuse_light = u_ld * u_kd * max(dot(s, t_norm),0.0);" 				// calculate diffused light using light equation
		"}"
		"gl_Position = u_projection_matrix * u_model_view_matrix * vPosition;" \
		"}";//end of code

	//Normal Matrix is inverse of transpose of upper 3x3 of model-view matrix

	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	glCompileShader(vertexShaderObject);

	// Error checking

	GLint infoLogLength = 0;
	GLint shaderCompileStatus = 0;
	char* szInfoLog = NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &shaderCompileStatus);

	if (shaderCompileStatus == GL_FALSE) {
		
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {
			
			szInfoLog = (char*)malloc(infoLogLength);
			
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "*** Vertext Shader Compilation Status***\n%s\n",szInfoLog);
				free(szInfoLog);
				DestroyWindow(ghwnd);
			}	
		}
	}
	//_______________________________________________________________________________________________________________________

	/**** FRAGMENT SHADER SOURCE CODE****/

	//create shader
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar* fragmentShaderSourceCode =
		"#version 450 core"\
		"\n"\
		"vec4 color;"\
		""\
		"in vec3 diffuse_light;"\
		"uniform int u_lkeypressed;"\
		""\
		"out vec4 FragColor;"\
		""\
		"void main(void)"\
		"{" \
			"if(u_lkeypressed == 1) {"\
				"color = vec4(diffuse_light, 1.0); "\
			"} else {"\
				"color =  vec4(1.0,1.0,1.0,1.0);"\
			"}"\
			"FragColor = color;" \
		"}"; //endofcode

	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(fragmentShaderObject);

	// Error checking

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &shaderCompileStatus);

	if (shaderCompileStatus == GL_FALSE) {

		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {

			szInfoLog = (char*)malloc(infoLogLength);

			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "*** Fragment Shader Compilation Status***\n%s\n", szInfoLog);
				free(szInfoLog);
				DestroyWindow(ghwnd);
			}
		}
	}

	// *** SHADER PROGRAM ***

	// create
	shaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(shaderProgramObject, vertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	//pre-link binding of shader program object with vertex shader position attribute
	glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_NORMAL, "vNormal");

	//link shader
	glLinkProgram(shaderProgramObject);

	GLint shaderProgramLinkStatus = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	//Error checking
	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);
	if (shaderProgramLinkStatus == GL_FALSE) {
		
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {
			
			szInfoLog = (char*)malloc(infoLogLength);
			
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log :\n%s\n", szInfoLog);

				free(szInfoLog);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get MVP uniform location
	uniform_modelViewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_model_view_matrix");
	uniform_perspectiveProjection = glGetUniformLocation(shaderProgramObject, "u_projection_matrix");
	
	lKeyPressed_uniform = glGetUniformLocation(shaderProgramObject, "u_lkeypressed");

	ld_uniform = glGetUniformLocation(shaderProgramObject, "u_ld");
	kd_uniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	lightPosition_uniform = glGetUniformLocation(shaderProgramObject, "u_light_position");

	// *** vertices, colors, shader attribs, vbo, vao initializationa ***

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
		1.0f, -1.0f, -1.0f,
	};

	const GLfloat cubeNormals[] = {
		0.0f, 0.0f, 1.0f,	//Front Face
		0.0f, 0.0f, 1.0f,	//Front Face
		0.0f, 0.0f, 1.0f,	//Front Face
		0.0f, 0.0f, 1.0f,	//Front Face

		1.0f, 0.0f, 0.0f,	//Right Face
		1.0f, 0.0f, 0.0f,	//Right Face
		1.0f, 0.0f, 0.0f,	//Right Face
		1.0f, 0.0f, 0.0f,	//Right Face

		0.0f, 0.0f, -1.0f,	//Back Face
		0.0f, 0.0f, -1.0f,	//Back Face
		0.0f, 0.0f, -1.0f,	//Back Face
		0.0f, 0.0f, -1.0f,	//Back Face
		
		-1.0f, 0.0f, 0.0f,	//Left Face
		-1.0f, 0.0f, 0.0f,	//Left Face
		-1.0f, 0.0f, 0.0f,	//Left Face
		-1.0f, 0.0f, 0.0f,	//Left Face
		
		0.0f, 1.0f, 0.0f,	//Top Face
		0.0f, 1.0f, 0.0f,	//Top Face
		0.0f, 1.0f, 0.0f,	//Top Face
		0.0f, 1.0f, 0.0f,	//Top Face
		
		0.0f, -1.0f, 0.0f,	//Bottom Face
		0.0f, -1.0f, 0.0f,	//Bottom Face
		0.0f, -1.0f, 0.0f,	//Bottom Face
		0.0f, -1.0f, 0.0f,	//Bottom Face
	};

	//__________________________________________________________________________________________

	//For Square
	glGenVertexArrays(1, &vao_cube);
	glBindVertexArray(vao_cube);

		//for position of cube
		glGenBuffers(1, &vbo_position_cube);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//for Normals of cube
		glGenBuffers(1, &vbo_normal_cube);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_normal_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);
		glVertexAttribPointer(RSK_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_NORMAL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	//___________________________________________________________________________
	
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

	bLight = false;
	bAnimate = false;

	//Clear Color Buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//set perspectivecMatrix to identity matrix
	perspectiveProjectionMatrix = mat4::identity();

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

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f,100.0f);

	fprintf(gpFile, "EXIT resize()\n");
}

void display(void) {

	//
	//void update(void);

	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion

	//start using OpenGL program Object
	glUseProgram(shaderProgramObject);
		
		if (bLight) {
		
			glUniform1i(lKeyPressed_uniform, 1);
			glUniform3f(ld_uniform, 1.0f, 1.0f, 1.0f); // light diffuse
			glUniform3f(kd_uniform, 0.5f, 0.5f, 0.5f); // material

			GLfloat lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f }; //light position
			glUniform4fv(lightPosition_uniform, 1,(GLfloat*)lightPosition); // material

		}
		else {
			glUniform1i(lKeyPressed_uniform, 0);
		}
		
		//set model-view & projection matrices to identity
		mat4 modelViewMatrix  = mat4::identity();
		mat4 projectionMatrix = mat4::identity();
		
		//translation matrix
		mat4 translateMatrix = mat4::identity();

		//rotation matrix
		mat4 rotationMatrixX = mat4::identity();
		mat4 rotationMatrixY = mat4::identity();
		mat4 rotationMatrixZ = mat4::identity();

		//SQUARE TRANSFORMATION
		modelViewMatrix  = mat4::identity();
		projectionMatrix = mat4::identity();

		translateMatrix = vmath::translate(0.0f, 0.0f, -5.0f);

		rotationMatrixX = vmath::rotate(cubeAngle, 1.0f, 0.0f, 0.0f);
		rotationMatrixY = vmath::rotate(cubeAngle, 0.0f, 1.0f, 0.0f);
		rotationMatrixZ = vmath::rotate(cubeAngle, 0.0f, 0.0f, 1.0f);

		modelViewMatrix = translateMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ;
		glUniformMatrix4fv(uniform_modelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);

		projectionMatrix = perspectiveProjectionMatrix;
		glUniformMatrix4fv(uniform_perspectiveProjection, 1, GL_FALSE, projectionMatrix);

		//OpenGL Drawing

		// *** bind vao_square
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

	SwapBuffers(ghdc);
}

void update(void) {

	cubeAngle = cubeAngle + 0.06f;
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


	if (vao_cube) {
		glDeleteVertexArrays(1, &vao_cube);
		vao_cube = 0;
	}

	if (vbo_position_cube) {
		glDeleteBuffers(1, &vbo_position_cube);
		vbo_position_cube = 0;
	}

	if (vbo_normal_cube) {
		glDeleteBuffers(1, &vbo_normal_cube);
		vbo_normal_cube = 0;
	}

	if (shaderProgramObject) {
		
		glUseProgram(shaderProgramObject);

		GLsizei shaderCount;

		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* p_shader = NULL;

		p_shader = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(shaderProgramObject, shaderCount, &shaderCount, p_shader);

		for (GLsizei i = 0; i < shaderCount; i++) {

			//detach shader from ith shader
			glDetachShader(shaderProgramObject, p_shader[i]);

			//delete vertex shader
			glDeleteShader(p_shader[i]);

			p_shader[i] = 0;
		}

		free(p_shader);
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
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