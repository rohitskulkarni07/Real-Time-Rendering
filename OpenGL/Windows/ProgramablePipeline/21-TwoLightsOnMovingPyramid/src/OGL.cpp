#include <Windows.h>
#include <stdio.h>
#include "OGL.h"

#include <gl/glew.h> // graphics library extension wrangler //For GLSL Extensions This Library Must Be Before #include <gl\gl.h>
#include <gl/gl.h>	// graphic library

#include "vmath.h"	/*	This File Comes With RedBook 8th Edition
						header for vector mathematics */
#include "Sphere.h"

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"Sphere.lib")

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

bool gbFullscreen	= false;
bool gbActiveWindow = false;
bool gbPolygonMode	= true;
bool bLight;
bool bAnimate;

GLfloat pyramidAngle = 0.0f;

struct Light {
	vec3 light_ambient;
	vec3 light_diffuse;
	vec3 light_specular;
	vec4 light_position;
};

struct Light light[2];

// Material Arrays
GLfloat material_ambient[3] = { 0.0f, 0.0f, 0.0f };
GLfloat material_diffuse[3] = { 1.0f, 1.0f, 1.0f };
GLfloat material_specular[3] = { 1.0f, 1.0f, 1.0f };
GLfloat material_shininess = 128.0f;

//--------------GLSL RELETED------------------------------
GLuint vertexShaderObject;
GLuint fragmentShaderObject;
GLuint shaderProgramObject;

//for sphere
GLuint vao_pyramid;
GLuint vbo_pyramid_position;
GLuint vbo_pyramid_normals;

//to get uniform location from shader
GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint projectionMatrixUniform;

GLuint lKeyPressed_uniform; //keypress

// Lights Uniforms
GLuint la_uniform[2];	
GLuint ld_uniform[2];
GLuint ls_uniform[2];
GLuint lightPosition_uniform[2];

// Material Uniforms
GLuint ka_uniform;
GLuint kd_uniform;
GLuint ks_uniform;
GLuint shininess_uniform;

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

		switch (LOWORD(wParam)) {
		case 'P':
		case 'p':
			if (gbPolygonMode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				gbPolygonMode = false;
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				gbPolygonMode = true;
			}
			break;
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
	fprintf(gpFile, "\n");
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
		"" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		""\
		"uniform int u_lkeypressed;"\
		""\
		"uniform vec3 u_la[2];" \
		"uniform vec3 u_ld[2];" \
		"uniform vec3 u_ls[2];" \
		"uniform vec4 u_light_position[2];" \
		""\
		"uniform vec3 u_ka;" \
		"uniform vec3 u_kd;" \
		"uniform vec3 u_ks;" \
		"uniform float u_shininess;"\
		""\
		"vec3 ambient;"\
		"vec3 diffuse;"\
		"vec3 specular;"\
		"vec3 light_direction;"\
		"vec3 reflection_vector;"\
		""\
		"out vec3 phong_ads_light;" \
		"void main(void)" \
		"{" \
		"int i = 0;"\
		"phong_ads_light = vec3(0.0,0.0,0.0);"
		"if(u_lkeypressed == 1)"\
		"{"\
		"vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;"\
		"vec3 tranformed_normal = normalize(mat3(u_view_matrix*u_model_matrix) * vNormal);"\
		"vec3 view_vector = normalize(-eye_coordinates.xyz);"\
		""\
		"for(i = 0; i < 2; i++)"\
		"{"\
		"light_direction = normalize(vec3(u_light_position[i]-eye_coordinates));"\
		"reflection_vector = reflect(-light_direction,tranformed_normal);"\
		"ambient = u_la[i] * u_ka;"\
		"diffuse = u_ld[i] * u_kd * max(dot(light_direction,tranformed_normal),0.0);"\
		"specular = u_ls[i] * u_ks * pow(max(dot(reflection_vector,view_vector),0.0),u_shininess);"\
		"phong_ads_light = phong_ads_light + ambient + diffuse + specular;"\
		"}"\
		"}"\
		"else{"\
		"phong_ads_light = vec3(1.0,1.0,1.0);"\
		"}"\
		"gl_Position =  u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		""\
		"}";//end of code

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
				fprintf(gpFile, "*** Vertext Shader Compilation Status***\n%s\n", szInfoLog);
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
		"in vec3 phong_ads_light;"\
		"out vec4 FragColor;"\
		"void main(void)"\
		"{" \
		"FragColor = vec4(phong_ads_light, 1.0);" \
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
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_model_matrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_view_matrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projection_matrix");
	lKeyPressed_uniform = glGetUniformLocation(shaderProgramObject, "u_lkeypressed");

	la_uniform[0] = glGetUniformLocation(shaderProgramObject, "u_la[0]"); // light ambient component
	ld_uniform[0] = glGetUniformLocation(shaderProgramObject, "u_ld[0]"); // light diffuse component
	ls_uniform[0] = glGetUniformLocation(shaderProgramObject, "u_ls[0]"); // light specular component
	
	la_uniform[1] = glGetUniformLocation(shaderProgramObject, "u_la[1]"); // light ambient component
	ld_uniform[1] = glGetUniformLocation(shaderProgramObject, "u_ld[1]"); // light diffuse component
	ls_uniform[1] = glGetUniformLocation(shaderProgramObject, "u_ls[1]"); // light specular component

	lightPosition_uniform[0] = glGetUniformLocation(shaderProgramObject, "u_light_position[0]"); // light position
	lightPosition_uniform[1] = glGetUniformLocation(shaderProgramObject, "u_light_position[1]"); // light position

	ka_uniform = glGetUniformLocation(shaderProgramObject, "u_ka"); // material ambient component
	kd_uniform = glGetUniformLocation(shaderProgramObject, "u_kd"); // material diffuse component
	ks_uniform = glGetUniformLocation(shaderProgramObject, "u_ks"); // material specular component
	shininess_uniform = glGetUniformLocation(shaderProgramObject, "u_shininess"); // material shininess Component
	
	fprintf(gpFile, "la_uniform[0] :%u\n", la_uniform[0]);
	fprintf(gpFile, "ld_uniform[0] :%u\n", ld_uniform[0]);
	fprintf(gpFile, "ls_uniform[0] :%u\n", ls_uniform[0]);
	fprintf(gpFile, "la_uniform[1] :%u\n", la_uniform[1]);
	fprintf(gpFile, "ld_uniform[1] :%u\n", ld_uniform[1]);
	fprintf(gpFile, "ls_uniform[1] :%u\n", ls_uniform[1]);

	fprintf(gpFile, "lightPosition_uniform[0] :%u\n", lightPosition_uniform[0]);
	fprintf(gpFile, "lightPosition_uniform[1] :%u\n", lightPosition_uniform[1]);

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
	const GLfloat pyramidNormals[] = {
		// front face
		0.0f, 0.447214f, 0.894427f,
		0.0f, 0.447214f, 0.894427f,
		0.0f, 0.447214f, 0.894427f,
		// right face
		0.894427f, 0.447214f, 0.0f,
		0.894427f, 0.447214f, 0.0f,
		0.894427f, 0.447214f, 0.0f,
		// back face
		0.0f, 0.447214f, -0.894427f,
		0.0f, 0.447214f, -0.894427f,
		0.0f, 0.447214f, -0.894427f,
		// left face
		-0.894427f, 0.447214f, 0.0f,
		-0.894427f, 0.447214f, 0.0f,
		-0.894427f, 0.447214f, 0.0f
	};

	//For Geometry
	glGenVertexArrays(1, &vao_pyramid);
	glBindVertexArray(vao_pyramid);

		//for Geometry Position Properties
		glGenBuffers(1, &vbo_pyramid_position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid_position);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//for Geometry Normals Properties
		glGenBuffers(1, &vbo_pyramid_normals);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidNormals), pyramidNormals, GL_STATIC_DRAW);
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


	light[0].light_ambient  = vec3(0.0f, 0.0f, 0.0f);
	light[0].light_diffuse  = vec3(1.0f, 0.0f, 0.0f);
	light[0].light_specular = vec3(1.0f, 0.0f, 0.0f);
	light[0].light_position = vec4(-2.0f, 0.0f, 0.0f, 1.0f);

	light[1].light_ambient  = vec3(0.0f, 0.0f, 0.0f);
	light[1].light_diffuse  = vec3(0.0f, 0.0f, 1.0f);
	light[1].light_specular = vec3(0.0f, 0.0f, 1.0f);
	light[1].light_position = vec4(2.0f, 0.0f, 0.0f, 1.0f);

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

	// Function Prototype
	void update(void);
	//code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion
	

	//start using OpenGL program Object
	glUseProgram(shaderProgramObject);

	//OpenGL Drawing

	if (bLight) {

		glUniform1i(lKeyPressed_uniform, 1);
		glUniform3fv(la_uniform[0], 1, light[0].light_ambient);  // light ambient
		glUniform3fv(ld_uniform[0], 1, light[0].light_diffuse);  // light diffuse
		glUniform3fv(ls_uniform[0], 1, light[0].light_specular); // light specular
		glUniform4fv(lightPosition_uniform[0], 1, light[0].light_position); // light position

		glUniform3fv(la_uniform[1], 1, light[1].light_ambient);  // light ambient
		glUniform3fv(ld_uniform[1], 1, light[1].light_diffuse);  // light diffuse
		glUniform3fv(ls_uniform[1], 1, light[1].light_specular); // light specular
		glUniform4fv(lightPosition_uniform[1], 1, light[1].light_position); // light position


		glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
		glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
		glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
		glUniform1f(shininess_uniform , (GLfloat)material_shininess ); // material shininess

	}
	else {
		glUniform1i(lKeyPressed_uniform, 0);
	}

	//set model-view & model-view-projection matrices to identity
	mat4 modelMatrix		= mat4::identity();
	mat4 viewMatrix			= mat4::identity();
	mat4 translationMatrix	= mat4::identity();
	mat4 rotationMatrix_Y   = mat4::identity();

	mat4 projectionMatrix	= perspectiveProjectionMatrix;
	
	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);
	rotationMatrix_Y  = vmath::rotate(pyramidAngle, 0.0f, 1.0f, 0.0f);

	modelMatrix = translationMatrix * rotationMatrix_Y;

	//Sending Unifomrs to Shader
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE,projectionMatrix);

	/*** bind vao_square ***/
	glBindVertexArray(vao_pyramid);
	glDrawArrays(GL_TRIANGLES, 0, 12);	// why 12 is last parameter because vertices * triangle faces
	glBindVertexArray(0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//stop using OpenGL program Object
	glUseProgram(0);

	update();
	SwapBuffers(ghdc);
}

void update(void) {

	pyramidAngle = pyramidAngle + 0.02f;
	if (pyramidAngle >= 360.0f) {
		pyramidAngle = 0.0f;
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


	if (vao_pyramid) {
		glDeleteVertexArrays(1, &vao_pyramid);
		vao_pyramid= 0;
	}

	if (vbo_pyramid_position) {
		glDeleteBuffers(1, &vbo_pyramid_position);
		vbo_pyramid_position = 0;
	}
	if (vbo_pyramid_normals) {
		glDeleteBuffers(1, &vbo_pyramid_normals);
		vbo_pyramid_normals = 0;
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