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

bool gbPV = true;
bool gbPF = false;
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
GLfloat light_position[4] = { 100.0f, 100.0f, 100.0f, 1.0f }; //light position

// Material Arrays
GLfloat material_ambient[3] = { 0.1f, 0.1f, 0.1f };
GLfloat material_diffuse[3] = { 0.5f, 0.2f, 0.7f };
GLfloat material_specular[3] = { 0.7f, 0.7f, 0.7f };
GLfloat material_shininess = 128.0f;

//for sphere
GLuint vao_sphere;
GLuint vbo_sphere_position;
GLuint vbo_sphere_normals;
GLuint vbo_sphere_elements;

//--------------GLSL RELETED------------------------------
//--------------PER VERTEX------------------------------
GLuint vertexShaderObject_PV;
GLuint fragmentShaderObject_PV;
GLuint shaderProgramObject_PV;

//to get uniform location from shader
GLuint modelMatrixUniform_PV;
GLuint viewMatrixUniform_PV;
GLuint projectionMatrixUniform_PV;
GLuint lKeyPressed_uniform_PV; //keypress

// Lights Uniforms
GLuint la_uniform_PV;	
GLuint ld_uniform_PV;
GLuint ls_uniform_PV;
GLuint lightPosition_uniform_PV;	

// Material Uniforms
GLuint ka_uniform_PV;
GLuint kd_uniform_PV;
GLuint ks_uniform_PV;
GLuint shininess_uniform_PV;

//--------------PER FRAGMENT------------------------------
GLuint vertexShaderObject_PF;
GLuint fragmentShaderObject_PF;
GLuint shaderProgramObject_PF;

//to get uniform location from shader
GLuint modelMatrixUniform_PF;
GLuint viewMatrixUniform_PF;
GLuint projectionMatrixUniform_PF;

GLuint lKeyPressed_uniform_PF; //keypress

// Lights Uniforms
GLuint la_uniform_PF;
GLuint ld_uniform_PF;
GLuint ls_uniform_PF;
GLuint lightPosition_uniform_PF;

// Material Uniforms
GLuint ka_uniform_PF;
GLuint kd_uniform_PF;
GLuint ks_uniform_PF;
GLuint shininess_uniform_PF;

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
			ToggleFullscreen();
			break;
		default:
			break;
		}

		break;

	case WM_CHAR:

		switch (LOWORD(wParam)) {
		case 'Q':
		case 'q':
			DestroyWindow(hwnd);
			break;

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
				gbPV = true;
				gbPF = false;

			}
			else {
				bLight = true;
				gbPV = true;
				gbPF = false;
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
		//per fragment toggle
		case 'F':
		case 'f':
			gbPF = true;
			gbPV = false;
			break;
		//per vertex toggle
		case 'V':
		case 'v':
			gbPV = true;
			gbPF = false;
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
	vertexShaderObject_PV = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	// core profile is used #version 450 core

	const GLchar* vertexShaderSourceCode_PV =
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
		"uniform vec3 u_la;" \
		"uniform vec3 u_ld;" \
		"uniform vec3 u_ls;" \
		"uniform vec4 u_light_position;" \
		""\
		"uniform vec3 u_ka;" \
		"uniform vec3 u_kd;" \
		"uniform vec3 u_ks;" \
		"uniform float u_shininess;"\
		""\
		"out vec3 phong_ads_light;" \
		"void main(void)" \
		"{" \
		"if(u_lkeypressed == 1)"\
		"{"\
		"vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;"\
		"vec3 tranformed_normal = normalize(mat3(u_view_matrix*u_model_matrix) * vNormal);"\
		"vec3 light_direction = normalize(vec3(u_light_position-eye_coordinates));"\
		"vec3 reflection_vector = reflect(-light_direction,tranformed_normal);"\
		"vec3 view_vector = normalize(-eye_coordinates.xyz);"\
		"vec3 ambient = u_la * u_ka;"\
		"vec3 diffuse = u_ld * u_kd * max(dot(light_direction,tranformed_normal),0.0);"\
		"vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector,view_vector),0.0),u_shininess);"\
		"phong_ads_light = ambient + diffuse + specular;"\
		"}"\
		"else{"\
		"phong_ads_light = vec3(1.0,1.0,1.0);"\
		"}"\
		"gl_Position =  u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		""\
		"}";//end of code

	
	glShaderSource(vertexShaderObject_PV, 1, (const GLchar**)&vertexShaderSourceCode_PV, NULL);

	glCompileShader(vertexShaderObject_PV);

	// Error checking

	GLint infoLogLength = 0;
	GLint shaderCompileStatus = 0;
	char* szInfoLog = NULL;

	glGetShaderiv(vertexShaderObject_PV, GL_COMPILE_STATUS, &shaderCompileStatus);

	if (shaderCompileStatus == GL_FALSE) {
		
		glGetShaderiv(vertexShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {
			
			szInfoLog = (char*)malloc(infoLogLength);
			
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject_PV, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "*** Vertext Shader Compilation Status***\n%s\n",szInfoLog);
				free(szInfoLog);
				DestroyWindow(ghwnd);
			}	
		}
	}
	//_______________________________________________________________________________________________________________________

	/**** FRAGMENT SHADER SOURCE CODE****/

	//create shader
	fragmentShaderObject_PV = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar* fragmentShaderSourceCode_PV =
		"#version 450 core"\
		"\n"\
		"in vec3 phong_ads_light;"\
		"out vec4 FragColor;"\
		"void main(void)"\
		"{" \
		"FragColor = vec4(phong_ads_light, 1.0);" \
		"}"; //endofcode

	glShaderSource(fragmentShaderObject_PV, 1, (const GLchar**)&fragmentShaderSourceCode_PV, NULL);

	//compile shader
	glCompileShader(fragmentShaderObject_PV);

	// Error checking

	glGetShaderiv(fragmentShaderObject_PV, GL_COMPILE_STATUS, &shaderCompileStatus);

	if (shaderCompileStatus == GL_FALSE) {

		glGetShaderiv(fragmentShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {

			szInfoLog = (char*)malloc(infoLogLength);

			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject_PV, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "*** Fragment Shader Compilation Status***\n%s\n", szInfoLog);
				free(szInfoLog);
				DestroyWindow(ghwnd);
			}
		}
	}

	// *** SHADER PROGRAM ***

	// create
	shaderProgramObject_PV = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(shaderProgramObject_PV, vertexShaderObject_PV);

	//attach fragment shader to shader program
	glAttachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

	//pre-link binding of shader program object with vertex shader position attribute
	glBindAttribLocation(shaderProgramObject_PV, RSK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(shaderProgramObject_PV, RSK_ATTRIBUTE_NORMAL, "vNormal");

	//link shader
	glLinkProgram(shaderProgramObject_PV);

	GLint shaderProgramLinkStatus = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	//Error checking
	glGetProgramiv(shaderProgramObject_PV, GL_LINK_STATUS, &shaderProgramLinkStatus);
	if (shaderProgramLinkStatus == GL_FALSE) {
		
		glGetProgramiv(shaderProgramObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {
			
			szInfoLog = (char*)malloc(infoLogLength);
			
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject_PV, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log :\n%s\n", szInfoLog);

				free(szInfoLog);
				DestroyWindow(ghwnd);
			}
		}
	}
	/**** VERTEXT SHADER SOURCE CODE****/

//create shader
	vertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	// core profile is used #version 450 core

	const GLchar* vertexShaderSourceCode_PF =
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

	glShaderSource(vertexShaderObject_PF, 1, (const GLchar**)&vertexShaderSourceCode_PF, NULL);

	glCompileShader(vertexShaderObject_PF);

	// Error checking

	infoLogLength = 0;
	shaderCompileStatus = 0;
	szInfoLog = NULL;

	glGetShaderiv(vertexShaderObject_PF, GL_COMPILE_STATUS, &shaderCompileStatus);

	if (shaderCompileStatus == GL_FALSE) {

		glGetShaderiv(vertexShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {

			szInfoLog = (char*)malloc(infoLogLength);

			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject_PF, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "*** Vertext Shader Compilation Status***\n%s\n", szInfoLog);
				free(szInfoLog);
				DestroyWindow(ghwnd);
			}
		}
	}
	//_______________________________________________________________________________________________________________________

	/**** FRAGMENT SHADER SOURCE CODE****/

	//create shader
	fragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar* fragmentShaderSourceCode_PF =
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

	glShaderSource(fragmentShaderObject_PF, 1, (const GLchar**)&fragmentShaderSourceCode_PF, NULL);

	//compile shader
	glCompileShader(fragmentShaderObject_PF);

	// Error checking

	glGetShaderiv(fragmentShaderObject_PF, GL_COMPILE_STATUS, &shaderCompileStatus);

	if (shaderCompileStatus == GL_FALSE) {

		glGetShaderiv(fragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {

			szInfoLog = (char*)malloc(infoLogLength);

			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject_PF, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "*** Fragment Shader Compilation Status***\n%s\n", szInfoLog);
				free(szInfoLog);
				DestroyWindow(ghwnd);
			}
		}
	}

	// *** SHADER PROGRAM ***

	// create
	shaderProgramObject_PF = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);

	//attach fragment shader to shader program
	glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

	//pre-link binding of shader program object with vertex shader position attribute
	glBindAttribLocation(shaderProgramObject_PF, RSK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(shaderProgramObject_PF, RSK_ATTRIBUTE_NORMAL, "vNormal");

	//link shader
	glLinkProgram(shaderProgramObject_PF);

	shaderProgramLinkStatus = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	//Error checking
	glGetProgramiv(shaderProgramObject_PF, GL_LINK_STATUS, &shaderProgramLinkStatus);
	if (shaderProgramLinkStatus == GL_FALSE) {

		glGetProgramiv(shaderProgramObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {

			szInfoLog = (char*)malloc(infoLogLength);
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject_PF, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log :\n%s\n", szInfoLog);

				free(szInfoLog);
				DestroyWindow(ghwnd);
			}
		}
	}
	//get MVP uniform location OF PER VERTEX
	modelMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_model_matrix");
	viewMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_view_matrix");
	projectionMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_projection_matrix");
	lKeyPressed_uniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_lkeypressed");

	la_uniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_la"); // light ambient component
	ld_uniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ld"); // light diffuse component
	ls_uniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ls"); // light specular component
	lightPosition_uniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_light_position"); // light position

	ka_uniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ka"); // material ambient component
	kd_uniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_kd"); // material diffuse component
	ks_uniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ks"); // material specular component
	shininess_uniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_shininess"); // material shininess Component



	//get MVP uniform location of PER FRAGMENT
	modelMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_model_matrix");
	viewMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_view_matrix");
	projectionMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_projection_matrix");
	lKeyPressed_uniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_lkeypressed");

	la_uniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_la"); // light ambient component
	ld_uniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ld"); // light diffuse component
	ls_uniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ls"); // light specular component
	lightPosition_uniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_light_position"); // light position

	ka_uniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ka"); // material ambient component
	kd_uniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_kd"); // material diffuse component
	ks_uniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ks"); // material specular component
	shininess_uniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_shininess"); // material shininess Component

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
	


	//set model-view & model-view-projection matrices to identity
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 projectionMatrix = perspectiveProjectionMatrix;

	modelMatrix = vmath::translate(0.0f, 0.0f, -1.5f);

	if (gbPV) {
		glUseProgram(shaderProgramObject_PV);
		glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, projectionMatrix);
	}
	else {
		glUseProgram(shaderProgramObject_PF);
		glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, projectionMatrix);
	}


	if (bLight) {
		

		if (gbPV && bLight) {
			//start using OpenGL program Objecc

			glUniform1i(lKeyPressed_uniform_PV, 1);
			glUniform3fv(la_uniform_PV, 1, (GLfloat*)light_ambient);  // light ambient
			glUniform3fv(ld_uniform_PV, 1, (GLfloat*)light_diffuse);  // light diffuse
			glUniform3fv(ls_uniform_PV, 1, (GLfloat*)light_specular); // light specular
			glUniform4fv(lightPosition_uniform_PV, 1, (GLfloat*)light_position); // light position

			glUniform3fv(ka_uniform_PV, 1, (GLfloat*)material_ambient);  // material ambient
			glUniform3fv(kd_uniform_PV, 1, (GLfloat*)material_diffuse);  // material diffuse
			glUniform3fv(ks_uniform_PV, 1, (GLfloat*)material_specular); // material specular
			glUniform1f(shininess_uniform_PV, (GLfloat)material_shininess); // material shininess

			//Sending Unifomrs to Shader
			glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, projectionMatrix);

		}
		else if (gbPF && bLight) {

			//start using OpenGL program Object


			glUniform1i(lKeyPressed_uniform_PF, 1);
			glUniform3fv(la_uniform_PF, 1, (GLfloat*)light_ambient);  // light ambient
			glUniform3fv(ld_uniform_PF, 1, (GLfloat*)light_diffuse);  // light diffuse
			glUniform3fv(ls_uniform_PF, 1, (GLfloat*)light_specular); // light specular
			glUniform4fv(lightPosition_uniform_PF, 1, (GLfloat*)light_position); // light position

			glUniform3fv(ka_uniform_PF, 1, (GLfloat*)material_ambient);  // material ambient
			glUniform3fv(kd_uniform_PF, 1, (GLfloat*)material_diffuse);  // material diffuse
			glUniform3fv(ks_uniform_PF, 1, (GLfloat*)material_specular); // material specular
			glUniform1f(shininess_uniform_PF, (GLfloat)material_shininess); // material shininess

			//Sending Unifomrs to Shader
			glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, projectionMatrix);

		}
	}
	else {

		if (gbPV && !bLight) {
			glUseProgram(shaderProgramObject_PV);

			glUniform1i(lKeyPressed_uniform_PV, 0);
		}

		if (gbPF && !bLight) {
			glUniform1i(lKeyPressed_uniform_PF, 0);
		}
	}


	/*** bind vao_square ***/
	glBindVertexArray(vao_sphere);

	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	
	// *** darw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, num_of_sphere_elements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	//stop using OpenGL program Object
	glUseProgram(0);

	update();
	SwapBuffers(ghdc);
}

void update(void) {

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
	if (shaderProgramObject_PV) {
		
		glUseProgram(shaderProgramObject_PV);

		GLsizei shaderCount;

		glGetProgramiv(shaderProgramObject_PV, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* p_shader = NULL;

		p_shader = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(shaderProgramObject_PV, shaderCount, &shaderCount, p_shader);

		for (GLsizei i = 0; i < shaderCount; i++) {

			//detach shader from ith shader
			glDetachShader(shaderProgramObject_PV, p_shader[i]);

			//delete vertex shader
			glDeleteShader(p_shader[i]);

			p_shader[i] = 0;
		}


		free(p_shader);
		glDeleteProgram(shaderProgramObject_PV);
		shaderProgramObject_PV = 0;
		glUseProgram(0);

	}

	if (shaderProgramObject_PF) {

		glUseProgram(shaderProgramObject_PF);

		GLsizei shaderCount;

		glGetProgramiv(shaderProgramObject_PF, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* p_shader = NULL;

		p_shader = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(shaderProgramObject_PF, shaderCount, &shaderCount, p_shader);

		for (GLsizei i = 0; i < shaderCount; i++) {

			//detach shader from ith shader
			glDetachShader(shaderProgramObject_PF, p_shader[i]);

			//delete vertex shader
			glDeleteShader(p_shader[i]);

			p_shader[i] = 0;
		}

		free(p_shader);
		glDeleteProgram(shaderProgramObject_PF);
		shaderProgramObject_PF = 0;
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