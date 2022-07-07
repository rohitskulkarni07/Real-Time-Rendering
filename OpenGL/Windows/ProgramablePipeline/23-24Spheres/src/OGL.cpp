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

//Sphere releted variable

GLfloat sphere_vertices[1146];
GLfloat sphere_normals[1146];
GLfloat sphere_textures[746];

unsigned short sphere_elements[2280];

GLuint num_of_sphere_vertices = 0;
GLuint num_of_sphere_elements = 0;

// Lights Arrays
GLfloat light_ambient[3] = { 0.1f, 0.1f, 0.1f };
GLfloat light_diffuse[3] = { 1.0f, 1.0f, 1.0f };
GLfloat light_specular[3] = { 1.0f, 1.0f, 1.0f };
GLfloat light_position[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //light position

// Material Arrays
GLfloat material_ambient[3];
GLfloat material_diffuse[3];
GLfloat material_specular[3];
GLfloat material_shininess = 0;


GLuint	keypressed = 0;
GLfloat r = 100.0f;
GLfloat gfAngleX = 0.0f;
GLfloat gfAngleY = 0.0f;
GLfloat gfAngleZ = 0.0f;
GLfloat tx = 0.0f, ty = 0.0f, tz = -1.5f;
GLfloat sx = 1.0f, sy = 1.0f, sz = 1.0f;

GLint	giWidht = 800;
GLint	giHeight = 600;

//GLfloat tx = 2.0f, ty = 14.0f , tz = -1.5f;
//GLfloat sx = 2.0f, sy = 2.0f, sz = 2.0f;

//--------------GLSL RELETED------------------------------
GLuint vertexShaderObject;
GLuint fragmentShaderObject;
GLuint shaderProgramObject;

//for sphere
GLuint vao_sphere;
GLuint vbo_sphere_position;
GLuint vbo_sphere_normals;
GLuint vbo_sphere_elements;

//to get uniform location from shader
GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint projectionMatrixUniform;

GLuint lKeyPressed_uniform; //keypress

// Lights Uniforms
GLuint la_uniform;	
GLuint ld_uniform;
GLuint ls_uniform;
GLuint lightPosition_uniform;	

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
		case 'x':
		case 'X':
			keypressed = 1;
			gfAngleX = 0.0f;
			break;
		case 'y':
		case 'Ý':
			keypressed = 2;
			gfAngleY = 0.0f;

			break;
		case 'z':
		case 'Z':
			keypressed = 3;
			gfAngleZ = 0.0f;

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
		"uniform vec4 u_light_position;" \
		""\
		"out vec3 tranformed_normal;"\
		"out vec3 light_direction;"\
		"out vec3 view_vector;"\
		""\
		"out vec3 phong_ads_light;" \
		"void main(void)" \
		"{" \
		"if(u_lkeypressed == 1)"\
		"{"\
		"vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;"\
		"tranformed_normal = mat3(u_view_matrix*u_model_matrix) * vNormal;"\
		"light_direction = vec3(u_light_position-eye_coordinates);"\
		"view_vector = -eye_coordinates.xyz;"\
		"}"\
		"gl_Position =  u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
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
		""\
		"in vec3 tranformed_normal;"\
		"in vec3 light_direction;"\
		"in vec3 view_vector;"\
		""\
		"uniform int u_lkeypressed;"\
		""\
		"uniform vec3 u_la;" \
		"uniform vec3 u_ld;" \
		"uniform vec3 u_ls;" \
		""\
		"uniform vec3 u_ka;" \
		"uniform vec3 u_kd;" \
		"uniform vec3 u_ks;" \
		"uniform float u_shininess;"\
		""\
		"out vec4 FragColor;"\
		"void main(void)"\
		"{" \
		"vec3 phong_ads_light;"\
		"if(u_lkeypressed == 1)"\
		"{"\
		"vec3 normalized_tranformed_normal = normalize(tranformed_normal);"\
		"vec3 normalized_light_direction = normalize(light_direction);"\
		"vec3 normalized_view_vector = normalize(view_vector);"\
		"vec3 reflection_vector = reflect(-normalized_light_direction,normalized_tranformed_normal);"\
		"vec3 ambient = u_la * u_ka;"\
		"vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction,normalized_tranformed_normal),0.0);"\
		"vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector,normalized_view_vector),0.0),u_shininess);"\
		"phong_ads_light = ambient + diffuse + specular;"\
		"}"\
		"else"\
		"{"\
		"phong_ads_light = vec3(1.0,1.0,1.0);"\
		"}"\
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

	la_uniform = glGetUniformLocation(shaderProgramObject, "u_la"); // light ambient component
	ld_uniform = glGetUniformLocation(shaderProgramObject, "u_ld"); // light diffuse component
	ls_uniform = glGetUniformLocation(shaderProgramObject, "u_ls"); // light specular component
	lightPosition_uniform = glGetUniformLocation(shaderProgramObject, "u_light_position"); // light position

	ka_uniform = glGetUniformLocation(shaderProgramObject, "u_ka"); // material ambient component
	kd_uniform = glGetUniformLocation(shaderProgramObject, "u_kd"); // material diffuse component
	ks_uniform = glGetUniformLocation(shaderProgramObject, "u_ks"); // material specular component
	shininess_uniform = glGetUniformLocation(shaderProgramObject, "u_shininess"); // material shininess Component

	// *** vertices, colors, shader attribs, vbo, vao initializationa ***
	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);

	num_of_sphere_vertices = getNumberOfSphereVertices();
	num_of_sphere_elements = getNumberOfSphereElements();
	//__________________________________________________________________________________________

	//For Geometry
	glGenVertexArrays(1, &vao_sphere);
	glBindVertexArray(vao_sphere);

		//for Geometry Position Properties
		glGenBuffers(1, &vbo_sphere_position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_position);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//for Geometry Normals Properties
		glGenBuffers(1, &vbo_sphere_normals);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
		glVertexAttribPointer(RSK_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_NORMAL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//for Geometry Elements Properties
		glGenBuffers(1, &vbo_sphere_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
		//no neeed of VertexAttribPointer
		//no need to enable vertex array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

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
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

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
	//glViewport(0, 0, (GLsizei)width/6, (GLsizei)height/6);

	giWidht = width;
	giHeight = height;

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f,100.0f);

	/*
	if (width <= height) {

		perspectiveProjectionMatrix = vmath::ortho(0.0f,
			15.5f,
			0.0f,
			15.5 * ((GLfloat)height / (GLfloat)width),
			-10.0f,
			10.0f);
	}
	else {

		perspectiveProjectionMatrix = vmath::ortho(0.0f,
			15.5f * ((GLfloat)width / (GLfloat)height),
			0.0f,
			15.5,
			-10.0f,
			10.0f);
	}
	*/
	fprintf(gpFile, "EXIT resize()\n");
}

void display(void) {

	// Function Prototype
	void update(void);
	void draw24Spheres(void);
	//code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion
	

	//start using OpenGL program Object
	glUseProgram(shaderProgramObject);

	//OpenGL Drawing

	if (bLight) {

		if (keypressed == 1) {
			light_position[0] = 0.0f;
			light_position[1] = r * sin(gfAngleX);
			light_position[2] = r * cos(gfAngleX);
			light_position[3] = 1.0f;

		}
		else if (keypressed == 2) {
			light_position[0] = r * sin(gfAngleY);
			light_position[1] = 0.0f;
			light_position[2] = r * cos(gfAngleY);
			light_position[3] = 1.0f;

		}
		else if (keypressed == 3) {
			light_position[0] = r * sin(gfAngleZ);
			light_position[1] = r * cos(gfAngleZ);
			light_position[2] = 0.0f;
			light_position[3] = 1.0f;

		}
			
		glUniform1i(lKeyPressed_uniform, 1);
		glUniform3fv(la_uniform, 1, (GLfloat*)light_ambient);  // light ambient
		glUniform3fv(ld_uniform, 1, (GLfloat*)light_diffuse);  // light diffuse
		glUniform3fv(ls_uniform, 1, (GLfloat*)light_specular); // light specular
		glUniform4fv(lightPosition_uniform, 1, (GLfloat*)light_position); // light position

		
	}
	else {
		glUniform1i(lKeyPressed_uniform, 0);
	}

	//set model-view & model-view-projection matrices to identity
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 projectionMatrix = perspectiveProjectionMatrix;
	mat4 translationMatrix = vmath::translate(tx, ty, tz);
	mat4 scalingMatrix = vmath::scale(sx, sy, sz);
	mat4 rotationMatrix_X = vmath::rotate(0.0f, 0.0f, 0.0f, 0.0f);
	mat4 rotationMatrix_Y = vmath::rotate(0.0f, 0.0f, 0.0f, 0.0f);
	mat4 rotationMatrix_Z = vmath::rotate(0.0f, 0.0f, 0.0f, 0.0f);

	modelMatrix = translationMatrix * scalingMatrix * rotationMatrix_X * rotationMatrix_Y * rotationMatrix_Z;

	//Sending Unifomrs to Shader
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, projectionMatrix);

	draw24Spheres();

	//stop using OpenGL program Object
	glUseProgram(0);

	update();
	SwapBuffers(ghdc);
}

void update(void) {
	if (bAnimate) {
		if (keypressed == 1) {
			gfAngleX = gfAngleX + 0.002f;
		}
		else if (keypressed == 2) {
			gfAngleY = gfAngleY + 0.002f;
		}
		else if (keypressed == 3) {
			gfAngleZ = gfAngleZ + 0.002f;
		}
	}
}


int viewX = 0;
int viewY = 0;


void draw24Spheres(void) {

	//____________________________________________________________________________________
	//_________________________ Emerald 1st Sphere On First Coloumn _________________________
	material_ambient[0] = 0.0215f;	//r
	material_ambient[1] = 0.1745f;	//g
	material_ambient[2] = 0.0215f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.07568f;	//r
	material_diffuse[1] = 0.61424f;	//g
	material_diffuse[2] = 0.07568f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.633f;	//r
	material_specular[1] = 0.727811f;	//g
	material_specular[2] = 0.633f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.6 * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 0; viewY = 5;
	glViewport(viewX * ((GLsizei)giWidht / 4), viewY * ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//_________________________ Jade 2nd Sphere On First Coloumn _________________________
	material_ambient[0] = 0.135f;	//r
	material_ambient[1] = 0.2255f;	//g
	material_ambient[2] = 0.1575f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.54;	//r
	material_diffuse[1] = 0.89;	//g
	material_diffuse[2] = 0.63f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.316228;	//r
	material_specular[1] = 0.316228;	//g
	material_specular[2] = 0.316228;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.1f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 0; viewY = 4;
	glViewport(viewX * ((GLsizei)giWidht / 4), viewY * ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________ Obsidian 3rd Sphere On First Coloumn _______________________________
	//Set Material

	material_ambient[0] = 0.05375f;	//r
	material_ambient[1] = 0.05f;	//g
	material_ambient[2] = 0.06625f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.18275f;	//r
	material_diffuse[1] = 0.17f;	//g
	material_diffuse[2] = 0.22525f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.332741f;	//r
	material_specular[1] = 0.328634f;	//g
	material_specular[2] = 0.346434f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.3f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 0; viewY = 3;
	glViewport(viewX * ((GLsizei)giWidht / 4), viewY * ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________ Pearl 4TH Sphere On First Coloumn _______________________________
	//Set Material
	material_ambient[0] = 0.25f;	//r
	material_ambient[1] = 0.20725f;		//g
	material_ambient[2] = 0.20725f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 1.0f;	//r
	material_diffuse[1] = 0.829f;	//g
	material_diffuse[2] = 0.829f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.296648f;	//r
	material_specular[1] = 0.296648f;	//g
	material_specular[2] = 0.296648f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.088f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 0; viewY = 2;
	glViewport(viewX * ((GLsizei)giWidht / 4), viewY * ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________ Ruby 5th Sphere On First Coloumn _______________________________
	//Set Material
	material_ambient[0] = 0.1745f;	//r
	material_ambient[1] = 0.01175f;		//g
	material_ambient[2] = 0.01175f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.61424f;	//r
	material_diffuse[1] = 0.04136f;	//g
	material_diffuse[2] = 0.04136f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.727811f;	//r
	material_specular[1] = 0.626959f;	//g
	material_specular[2] = 0.626959f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.6f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 0; viewY = 1;
	glViewport(viewX * ((GLsizei)giWidht /4), viewY * ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________ Turquoise 6th Sphere On First Coloumn _______________________________
	//Set Material
	material_ambient[0] = 0.1f;	//r
	material_ambient[1] = 0.18725f;		//g
	material_ambient[2] = 0.1745f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.396f;	//r
	material_diffuse[1] = 0.74151f;	//g
	material_diffuse[2] = 0.69102f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.297254f;	//r
	material_specular[1] = 0.30829f;	//g
	material_specular[2] = 0.306678f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.1f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 0; viewY = 0;
	glViewport(viewX * ((GLsizei)giWidht / 4), viewY * ((GLsizei)giHeight / 6), (GLsizei)giWidht/6, (GLsizei)giHeight/6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________________ Brass 1st Sphere On Second Coloumn ___________________________
	//Set Material

	material_ambient[0] = 0.329412f;	//r
	material_ambient[1] = 0.223529f;	//g
	material_ambient[2] = 0.027451f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.780392f;	//r
	material_diffuse[1] = 0.568627f;	//g
	material_diffuse[2] = 0.113725f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.992157f;	//r
	material_specular[1] = 0.941176f;	//g
	material_specular[2] = 0.807843f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.21794872f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 1; viewY = 5;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________________ Bronze 2nd Sphere On Second Coloumn ___________________________
	//Set Material

	material_ambient[0] = 0.2125f;	//r
	material_ambient[1] = 0.1275f;	//g
	material_ambient[2] = 0.054f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.714f;	//r
	material_diffuse[1] = 0.4284f;	//g
	material_diffuse[2] = 0.18144f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.39354f;	//r
	material_specular[1] = 0.271906f;	//g
	material_specular[2] = 0.166721f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.2f * 128;


	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 1; viewY = 4;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________________ Chrome 3rd Sphere On Second Coloumn ___________________________

	//Set Material

	material_ambient[0] = 0.25f;	//r
	material_ambient[1] = 0.25f;	//g
	material_ambient[2] = 0.25f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.4f;	//r
	material_diffuse[1] = 0.4f;	//g
	material_diffuse[2] = 0.4f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.774597f;	//r
	material_specular[1] = 0.774597f;	//g
	material_specular[2] = 0.774597f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.6f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 1; viewY = 3;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________

	//___________________________ Copper 4th Sphere On Second Coloumn ___________________________

	material_ambient[0] = 0.19125f;	//r
	material_ambient[1] = 0.0735f;	//g
	material_ambient[2] = 0.0225f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.7038f;	//r
	material_diffuse[1] = 0.27048f;	//g
	material_diffuse[2] = 0.0828f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.256777f;	//r
	material_specular[1] = 0.137622f;	//g
	material_specular[2] = 0.086014f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.1 * 128;


	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 1; viewY = 2;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________________ Gold 5th Sphere On Second Coloumn ___________________________

	//Set Material

	material_ambient[0] = 0.24725f;	//r
	material_ambient[1] = 0.1995f;	//g
	material_ambient[2] = 0.0745f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.75164f;	//r
	material_diffuse[1] = 0.60648f;	//g
	material_diffuse[2] = 0.2264f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.628281f;	//r
	material_specular[1] = 0.555802f;	//g
	material_specular[2] = 0.366065f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.4f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 1; viewY = 1;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________________ Silver 6th Sphere On Second Coloumn ___________________________

	//Set Material

	material_ambient[0] = 0.19225f;	//r
	material_ambient[1] = 0.19225f;	//g
	material_ambient[2] = 0.19225f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.50754f;	//r
	material_diffuse[1] = 0.50754f;	//g
	material_diffuse[2] = 0.50754f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.508273f;	//r
	material_specular[1] = 0.508273f;	//g
	material_specular[2] = 0.508273f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.4f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 1; viewY = 0;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);
	//____________________________________________________________________________________

	//___________________________ Black 1st Sphere On Third Coloumn ___________________________
	//Set Material


	material_ambient[0] = 0.0f;	//r
	material_ambient[1] = 0.0f;	//g
	material_ambient[2] = 0.0f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.01f;	//r
	material_diffuse[1] = 0.01f;	//g
	material_diffuse[2] = 0.01f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.50f;	//r
	material_specular[1] = 0.50f;	//g
	material_specular[2] = 0.50f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.25 * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 2; viewY = 5;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________________ Cyan 2nd Sphere On Third Coloumn ___________________________

	//Set Material
	material_ambient[0] = 0.0f;	//r
	material_ambient[1] = 0.1f;	//g
	material_ambient[2] = 0.06f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.0f;	//r
	material_diffuse[1] = 0.50980392f;	//g
	material_diffuse[2] = 0.50980392f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.50196078f;	//r
	material_specular[1] = 0.50196078f;	//g
	material_specular[2] = 0.50196078f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.25f * 128;


	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 2; viewY = 4;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________

	//___________________________ Green 3rd Sphere On Third Coloumn ___________________________
	//Set Material

	material_ambient[0] = 0.0f;	//r
	material_ambient[1] = 0.0f;	//g
	material_ambient[2] = 0.0f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.1f;	//r
	material_diffuse[1] = 0.35f;	//g
	material_diffuse[2] = 0.1f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.45f;	//r
	material_specular[1] = 0.55f;	//g
	material_specular[2] = 0.45f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.25f * 128;


	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 2; viewY = 3;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________

	//___________________________ Red 4th Sphere On Third Coloumn ___________________________
	//Set Material


	material_ambient[0] = 0.0f;	//r
	material_ambient[1] = 0.0f;	//g
	material_ambient[2] = 0.0f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.5f;	//r
	material_diffuse[1] = 0.0f;	//g
	material_diffuse[2] = 0.0f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.7f;	//r
	material_specular[1] = 0.6f;	//g
	material_specular[2] = 0.6f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.25 * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 2; viewY = 2;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________

	//___________________________ White 5th Sphere On Third Coloumn ___________________________
	//Set Material

	material_ambient[0] = 0.0f;	//r
	material_ambient[1] = 0.0f;	//g
	material_ambient[2] = 0.0f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.55f;	//r
	material_diffuse[1] = 0.55f;	//g
	material_diffuse[2] = 0.55f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.70f;	//r
	material_specular[1] = 0.70f;	//g
	material_specular[2] = 0.70f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.25f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 2; viewY = 1;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
	//___________________________ Yellow Plastic 6th Sphere On Third Coloumn ___________________________

	//Set Material

	material_ambient[0] = 0.0f;	//r
	material_ambient[1] = 0.0f;	//g
	material_ambient[2] = 0.0f;	//b
	material_ambient[3] = 1.0f;	//a

	material_diffuse[0] = 0.5f;	//r
	material_diffuse[1] = 0.5f;	//g
	material_diffuse[2] = 0.0f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.60f;	//r
	material_specular[1] = 0.60f;	//g
	material_specular[2] = 0.50f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.25f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 2; viewY = 0;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);
	//____________________________________________________________________________________
		//____________________________________________________________________________________

	//___________________________ Black 1st Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.02f;	//r
	material_ambient[1] = 0.02f;	//g
	material_ambient[2] = 0.02f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.01f;	//r
	material_diffuse[1] = 0.01f;	//g
	material_diffuse[2] = 0.01f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.4f;	//r
	material_specular[1] = 0.4f;	//g
	material_specular[2] = 0.4f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.078125f * 128;


	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 3; viewY = 5;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________
		//___________________________ Cyan 2nd Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.0f;	//r
	material_ambient[1] = 0.05f;	//g
	material_ambient[2] = 0.05f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.4f;	//r
	material_diffuse[1] = 0.5f;	//g
	material_diffuse[2] = 0.5f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.04f;	//r
	material_specular[1] = 0.7f;	//g
	material_specular[2] = 0.7f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.078125f * 128;


	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 3; viewY = 4;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________


	//___________________________ Green 3rd Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.0f;	//r
	material_ambient[1] = 0.05f;	//g
	material_ambient[2] = 0.0f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.4f;	//r
	material_diffuse[1] = 0.5f;	//g
	material_diffuse[2] = 0.4f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.04f;	//r
	material_specular[1] = 0.7f;	//g
	material_specular[2] = 0.04f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.078125f * 128;



	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 3; viewY = 3;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________


	//___________________________ Red 4th Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.05f;	//r
	material_ambient[1] = 0.0f;	//g
	material_ambient[2] = 0.0f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.5f;	//r
	material_diffuse[1] = 0.4f;	//g
	material_diffuse[2] = 0.4f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.7f;	//r
	material_specular[1] = 0.04f;	//g
	material_specular[2] = 0.04f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.078125f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 3; viewY = 2;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________

	//___________________________ White 5th Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.05f;	//r
	material_ambient[1] = 0.05f;	//g
	material_ambient[2] = 0.05f;	//b
	material_ambient[3] = 1.0f;		//a

	material_diffuse[0] = 0.5f;	//r
	material_diffuse[1] = 0.5f;	//g
	material_diffuse[2] = 0.5f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.7f;	//r
	material_specular[1] = 0.7f;	//g
	material_specular[2] = 0.7f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.078125f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 3; viewY = 1;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);

	//____________________________________________________________________________________

	//___________________________ Yellow Rubber 6th Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.05f;	//r
	material_ambient[1] = 0.05f;	//g
	material_ambient[2] = 0.0f;	//b
	material_ambient[3] = 1.0f;	//a

	material_diffuse[0] = 0.5f;	//r
	material_diffuse[1] = 0.5f;	//g
	material_diffuse[2] = 0.4f;	//b
	material_diffuse[3] = 1.0f;		//a

	material_specular[0] = 0.7f;	//r
	material_specular[1] = 0.7f;	//g
	material_specular[2] = 0.04f;	//b
	material_specular[3] = 1.0f;		//a

	material_shininess = 0.078125f * 128;

	glUniform3fv(ka_uniform, 1, (GLfloat*)material_ambient);  // material ambient
	glUniform3fv(kd_uniform, 1, (GLfloat*)material_diffuse);  // material diffuse
	glUniform3fv(ks_uniform, 1, (GLfloat*)material_specular); // material specular
	glUniform1f(shininess_uniform, (GLfloat)material_shininess); // material shininess

	//Set Viewport
	viewX = 3; viewY = 0;
	glViewport(viewX* ((GLsizei)giWidht / 4), viewY* ((GLsizei)giHeight / 6), (GLsizei)giWidht / 6, (GLsizei)giHeight / 6);

	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);
	// *** unbind vao ***
	glBindVertexArray(0);
	//____________________________________________________________________________________

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


	if (vao_sphere) {
		glDeleteVertexArrays(1, &vao_sphere);
		vao_sphere = 0;
	}

	if (vbo_sphere_position) {
		glDeleteBuffers(1, &vbo_sphere_position);
		vbo_sphere_position = 0;
	}

	if (vbo_sphere_normals) {
		glDeleteBuffers(1, &vbo_sphere_normals);
		vbo_sphere_normals = 0;
	}

	if (vbo_sphere_elements) {
		glDeleteBuffers(1, &vbo_sphere_elements);
		vbo_sphere_elements = 0;
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