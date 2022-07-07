#include <iostream>		// Input-Output Stream
#include <stdio.h>		// For C Standard Input-Output Function
#include <stdlib.h>		// For C Standard library Functions ex. exit()
#include <memory.h> 	// memset
#include <X11/Xlib.h>	// XLib Releted API
#include <X11/Xutil.h>	// XVisualInfo Releted Function
#include <X11/XKBlib.h> // Releted To Keyboard symbol conversion
#include <X11/keysym.h> // KeySym Releted API
#include <GL/glew.h>	// For GLSL Extensions This Library Must Be Before #include <gl\gl.h>
#include <GL/glx.h>		// For glx API ding API Library
#include <GL/gl.h>		// For OpenGL API	// Path : /usr/include/GL
#include "../vmath.h"	// This File Comes With RedBook 8th Edition Header for vector mathematics
// namespaces
using namespace std;
using namespace vmath;

bool 		 bFullscreen 	= false;	
Display 	*gpDisplay 		= NULL;		// Pointer To Display Struct
XVisualInfo *gpXVisualInfo	= NULL;		// Pointer To XVisualInfo Struct, Information Releted to Visual Information screen, depth, color
Colormap 	 gColormap;
Window 		 gWindow;
GLXContext	 gGlxContext;
int 		 giWindowWidth  = 800;
int 	 	 giWindowHeight = 600;

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);
glXCreateContextAttribsARBProc 	glXCreateContextAttribsARB = NULL;
GLXFBConfig  					gGlxfbConfig;

//	flags
bool 		gbPolygonMode	= true;
bool 		gbLights		= false;
bool 		gbAnimate		= false;

enum {
	RSK_ATTRIBUTE_POSITION = 0,
	RSK_ATTRIBUTE_COLOR,
	RSK_ATTRIBUTE_NORMAL,
	RSK_ATTRIBUTE_TEXTURE0,
};


GLfloat pyramidAngle = 0.0f;

struct Light {
	vec3 light_ambient;
	vec3 light_diffuse;
	vec3 light_specular;
	vec4 light_position;
};

struct Light light[2];

// Material Arrays
GLfloat 	material_ambient[3] = { 0.0f, 0.0f, 0.0f };
GLfloat 	material_diffuse[3] = { 1.0f, 1.0f, 1.0f };
GLfloat 	material_specular[3] = { 1.0f, 1.0f, 1.0f };
GLfloat 	material_shininess = 128.0f;


// *** Shader Releted Variables***/
GLuint		vertexShaderObject;
GLuint 		fragmentShaderObject;
GLuint		shaderProgramObject;

// vertex array object and vertex buffer object
GLuint vao_pyramid;
GLuint vbo_pyramid_position;
GLuint vbo_pyramid_normals;

// variable for storing location of uniforms
GLuint 		modelMatrixUniform;
GLuint 		viewMatrixUniform;
GLuint 		projectionMatrixUniform;
GLuint 		lKeyPressed_uniform; //keypress

// Lights Uniforms
GLuint 		la_uniform[2];	
GLuint 		ld_uniform[2];
GLuint 		ls_uniform[2];
GLuint 		lightPosition_uniform[2];	

// Material Uniforms
GLuint 		ka_uniform;
GLuint 		kd_uniform;
GLuint 		ks_uniform;
GLuint 		shininess_uniform;

// projection matirx
vmath::mat4 perspectiveProjectionMatrix;	// 4x4 matrix

// Entry-point Function
int main(void) {

	// Function Prototype
	void CreateWindow(void);		
	void ToggleFullscreen(void);
	
	
	//bluescreen changes
	void initialize(void);
	void resize(int, int);
	void display(void);
	void uninitialize(void);
	
	// Variable Declarations
	int winWidth  = giWindowWidth;
	int winHeight = giWindowHeight;
	
	//bluescreen changes
	bool bDone = false;
	

	// Code
	CreateWindow();

	initialize();
	

	// Game Loop
	
	XEvent event;	// XEvent is Union Having Many Members out of them 33 are Struct Members 
	KeySym keysym;	// To Store The Key-symbol
	
	while(bDone == false) {
			
		while(XPending(gpDisplay)) {
			
			XNextEvent(gpDisplay, &event);
			
			switch(event.type) {
				
				case MapNotify:			// similar to WM_CREATE
					break;
					
				case KeyPress:
					
					keysym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
					
					switch(keysym) {
						
						case XK_Escape: // Key symbol for Escape Key 
							//bluescreen changes
							bDone = true;
							
							
						case XK_F:		// Key-Symbol for 'F' Key
						case XK_f:		// Key-Symbol for 'f' Key
							
							if(bFullscreen == false) {
								ToggleFullscreen();
								bFullscreen = true;
							}
							
							else {
								ToggleFullscreen();
								bFullscreen = false;
							}
							
							break;
						case 'P':	
						case 'p':
							
							if(gbPolygonMode){
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
								gbPolygonMode = false;
							}else{
								glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
								gbPolygonMode = true;
							}
							break;	
							
						case 'l':
						case 'L':
							if (gbLights == true) {
								gbLights = false;
							}
							else {
								gbLights = true;
							}
							break;

						case 'a':
						case 'A':
							if (gbAnimate == true) {
								gbAnimate = false;
							}
							else {
								gbAnimate = true;
							}
							break;
						default:
							break;
					}
					
					break;

				case ButtonPress:
					switch(event.xbutton.button) {
						case 1:				// left mouse button
							break;
						case 2:				// middle mouse button
							break;
						case 3:				// right mouse button
							break;
						case 4:				// middle wheel up mouse button
							break;
						case 5:				// middle wheel down mouse button
							break;
						default:
							break;
					}
					break;

				case MotionNotify: 			// pointer move / curosor move
					break;
					
				case ConfigureNotify: 		// similar to WM_SIZE releted StrucutreNotifyMask
					
					winWidth  = event.xconfigure.width;
					winHeight = event.xconfigure.height;
					
					// bluescreen changes
					resize(winWidth, winHeight);
					
					break;
					
				case Expose:				// similar to WM_PAINT
					break;
					
				case DestroyNotify: 		// similar to WM_DESTROY
					break;
					
				case 33: 					// similar to DestroyNotify For dependent on Window Manager
					bDone = true;
					
				default:
					break;
			}
		}
		
		// Here Call Update Function
		display();
	}

	uninitialize();

	printf("\nExecuted Successfully\n");
	return(0);
}

void CreateWindow(void) {
	
	// Function Prototypes
	void uninitialize(void);

	// Variable Declarations
	XSetWindowAttributes  winAttribs;			 // Object To struct which Holds Information about Window 
	
	GLXFBConfig			 *pGlxfbConfig 		= NULL;	
	GLXFBConfig 		  bestGlxfbConfig;
	XVisualInfo 		 *pTempXvisualInfo;

	int 				  numOfFBConfig 	= 0;

	int defaultScreen;
	int defaultDepth;
	int styleMask;

	static int frameBufferAttribute[] = {	
						GLX_DOUBLEBUFFER,	True,
						GLX_X_RENDERABLE, 	True,
						GLX_DRAWABLE_TYPE, 	GLX_WINDOW_BIT,
						GLX_RENDER_TYPE, 	GLX_RGBA_BIT,
						GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
						GLX_RED_SIZE,		8,
						GLX_GREEN_SIZE,		8,
						GLX_BLUE_SIZE,		8,
						GLX_ALPHA_SIZE,		8,
						GLX_STENCIL_SIZE,	8,
						GLX_DEPTH_SIZE, 	24,	
						None 	};

	/*	V4L(Video For Linux) recommends bit-size of depth around 24 or below, V4L Works Under Device Driver: 
	Which further Classified into 2 Parts VA-API(Video Accelarartion) And VDPAU(Video Driver Presentation Unix
	 API) Components Above This Layer MESA cames Into Picture */					
	
	// Code
	gpDisplay = XOpenDisplay(NULL);
	
	// Error-Checking
	if(gpDisplay == NULL) {
		printf("Error : Unable To Open X Display.\nExitting Now...\n");
		uninitialize();
		exit(1);	// Exit Status:	0 successfull, 1 abortive,  -1 errornous
	}
	
	// To Get Default Screen/Current Screen Connected To Current Graphic Card
	defaultScreen = XDefaultScreen(gpDisplay);
	
	// To Get Current Color Depth for Default Screen Connected to Current GPU
	pGlxfbConfig = glXChooseFBConfig(gpDisplay, defaultScreen, frameBufferAttribute, &numOfFBConfig);
	printf("NumberOfFBConfig = %d\n",numOfFBConfig);

	int bestFrameBufferConfig	=  -1;
	int worstFrameBufferConfig 	=  -1;
	int bestNumberOfSamples		=  -1;
	int worstNumberOfSamples	= 999;

	printf("\nSrNo\t\tsamples\t\tsampleBuff\tvisID\n");
	for(int i = 0; i < numOfFBConfig; i++) {

		pTempXvisualInfo = glXGetVisualFromFBConfig(gpDisplay, pGlxfbConfig[i]);
		

		if(pTempXvisualInfo) {
			
			int samplesBuffers, samples;

			glXGetFBConfigAttrib(gpDisplay,pGlxfbConfig[i],GLX_SAMPLE_BUFFERS,&samplesBuffers);
			glXGetFBConfigAttrib(gpDisplay,pGlxfbConfig[i],GLX_SAMPLES,&samples);
			
			if(bestFrameBufferConfig<0 || samplesBuffers && samples>bestNumberOfSamples) {
				
				bestFrameBufferConfig	 = i;
				bestNumberOfSamples		 = samples;			
			}

			if(worstFrameBufferConfig<0 || (!samplesBuffers) || samples<worstFrameBufferConfig) {

				worstFrameBufferConfig	 = i;
				worstNumberOfSamples 	 = samples;
			}
			printf("%d\t\t%d\t\t%d\t\t%lu\n",i,samples,samplesBuffers,pTempXvisualInfo->visualid);
		}
	}

	bestGlxfbConfig = pGlxfbConfig[bestFrameBufferConfig];
	gGlxfbConfig 	= bestGlxfbConfig;

	XFree(pGlxfbConfig);

	gpXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, gGlxfbConfig);

	
	winAttribs.border_pixel 		= 0;										// Defualt Border Color
	winAttribs.background_pixmap 	= 0;										// No background Image
	winAttribs.background_pixel 	= BlackPixel(gpDisplay, defaultScreen);		// Background Color : Black
	winAttribs.colormap 			= XCreateColormap(gpDisplay, RootWindow(gpDisplay, gpXVisualInfo->screen), gpXVisualInfo->visual, AllocNone);
	gColormap = winAttribs.colormap;
	
	winAttribs.event_mask = ExposureMask 			|
							VisibilityChangeMask 	|
							ButtonPressMask 		|
							KeyPressMask 			|
							PointerMotionMask		|
							StructureNotifyMask;

	
	styleMask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;
	
	gWindow = XCreateWindow(gpDisplay,
				RootWindow(gpDisplay, gpXVisualInfo->screen),
				0,
				0,
				giWindowWidth,
				giWindowHeight,
				0,
				gpXVisualInfo->depth,
				InputOutput,
				gpXVisualInfo->visual,
				styleMask,
				&winAttribs);
	
	if(!gWindow) {
		printf("Error : Failed To Create Main Window.\nExiting Now...\n");
		uninitialize();
		exit(1);
	}

	// Caption To Window
	XStoreName(gpDisplay,gWindow, "RTR2020-004 Rohit Sudhir Kulkarni");
	Atom windowManagerDelete = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(gpDisplay, gWindow, &windowManagerDelete,1);

	//Show Window On Display
	XMapWindow(gpDisplay, gWindow);

}

void initialize(void) {

	// Function Prototype
	void resize(int, int);
	void uninitialize(void);
	// Local Variable	
	
	// Code
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddress((GLubyte*)"glXCreateContextAttribsARB");

	const int attribs[] = {
							GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
							GLX_CONTEXT_MINOR_VERSION_ARB, 5,
							GLX_CONTEXT_PROFILE_MASK_ARB,  GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
							None};

	gGlxContext = glXCreateContextAttribsARB(gpDisplay, gGlxfbConfig, 0, GL_TRUE, attribs);

	if(!gGlxContext){
		const int attribs[] = {
							GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
							GLX_CONTEXT_MINOR_VERSION_ARB, 0,
							GLX_CONTEXT_PROFILE_MASK_ARB,  GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
							None};
		gGlxContext = glXCreateContextAttribsARB(gpDisplay, gGlxfbConfig, 0, GL_TRUE, attribs);
		printf("\nContext 1.0\n");

	} else {
		printf("\nContext 4.5\n");
	}

	Bool isDirectContext = glXIsDirect(gpDisplay,gGlxContext);
	
	if(isDirectContext) {
		printf("\nHardware Context Achieved\n");
	}else {
		printf("\nSoftware Context Achieved\n");
	}

	glXMakeCurrent(gpDisplay, gWindow, gGlxContext);
	
	//*** Enable Extention For GLSL	***
	GLenum glew_error = glewInit();
	if(glew_error != GLEW_OK) {
		printf("GLEW Initialization Failed\n");
		uninitialize();
		exit(1);
	}

	// OpenGL Releted Log
	printf("\n\n");
	printf("\nOpenGL Releted Log\n");
	printf("OpenGL Vendor 	:%s\n", glGetString(GL_VENDOR));
	printf("OpenGL Renderer	:%s\n", glGetString(GL_RENDERER));
	printf("OpenGL Version 	:%s\n", glGetString(GL_VERSION));
	printf("GLSL Version 	:%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("\n\n");

	// Log For Extentions Enabled By GLEW
	GLint numOfExtentions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numOfExtentions);
	printf("\nLog For Extentions Enabled By GLEW\n\n");
	for (int i = 0; i < numOfExtentions; i++) {
		printf("%s\n", glGetStringi(GL_EXTENSIONS, i));
	}
	printf("\n\n");

	//*************************************************************************************
	//									GLSL
	//*************************************************************************************
	GLint infoLogLenght				= 0;
	GLint shaderCompilationStatus 	= 0;
	GLint shaderProgramLinkStatus	= 0;
	char* szInfoLog					= NULL;
	//*************************************************************************************
	//							 *** VERTEX SHADER ***
	//*************************************************************************************

	// create vertex shader
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// write the shader source code
	const GLchar* vertexShaderSourceCode = 
	"#version 450 core																								\n" \
	"\n 																											\n" \
	"in vec4 vPosition;																								\n" \
	"in vec3 vNormal;																								\n" \
	"																												\n" \
	"uniform mat4 u_model_matrix;																					\n" \
	"uniform mat4 u_view_matrix;																					\n" \
	"uniform mat4 u_projection_matrix;																				\n" \
	"																												\n" \
	"uniform int u_lkeypressed;																						\n" \
	"																												\n" \
	"uniform vec3 u_la[2];																								\n" \
	"uniform vec3 u_ld[2];																								\n" \
	"uniform vec3 u_ls[2];																								\n" \
	"																												\n" \
	"uniform vec4 u_light_position[2];																					\n" \
	"																												\n" \
	"uniform vec3 u_ka;																								\n" \
	"uniform vec3 u_kd;																								\n" \
	"uniform vec3 u_ks;																								\n" \
	"																												\n" \
	"uniform float u_shininess;																						\n" \
	"																												\n" \
	"vec3 ambient;																									\n" \
	"vec3 diffuse;																									\n" \
	"vec3 specular;																									\n" \
	"vec3 light_direction;																							\n" \
	"vec3 reflection_vector;																						\n" \
	"																												\n" \
	"out vec3 phong_ads_light;																						\n" \
	"																												\n" \
	"void main(void) {																								\n" \
	"																												\n" \
	"	int i = 0;																									\n" \
	"	phong_ads_light	= vec3(0.0,0.0,0.0);																		\n" \
	"																												\n" \
	"	if(u_lkeypressed == 1) {																					\n" \
	"		vec4 eye_coordinates 	= u_view_matrix * u_model_matrix * vPosition;									\n" \
	"		vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);					\n" \
	"		vec3 view_vector 		= normalize(-eye_coordinates.xyz);												\n" \
	"																												\n" \
	"		for(i = 0; i < 2; i++) {																					\n" \
	"																												\n" \
	"			light_direction		= normalize(vec3(u_light_position[i] - eye_coordinates));							\n" \
	"			reflection_vector 	= reflect(-light_direction,transformed_normal);									\n" \
	"			ambient 			= u_la[i] * u_ka;																	\n" \
	"			diffuse 			= u_ld[i] * u_kd * max(dot(light_direction,transformed_normal),0.0);				\n" \
	"			specular			= u_ls[i] * u_ks * pow(max(dot(reflection_vector,view_vector),0.0),u_shininess);	\n" \
	"			phong_ads_light		= phong_ads_light + ambient + diffuse + specular;													\n" \
	"		}																										\n" \
	"																												\n" \
	"	} else {																									\n" \
	"		phong_ads_light			= vec3(1.0,1.0,1.0);															\n" \
	"	}																											\n" \
	"																												\n" \
	"	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;								\n" \
	"}																												\n";

	// provide source code to shader 
	glShaderSource(vertexShaderObject, 1, (const GLchar**) &vertexShaderSourceCode, NULL);

	// compile shader source code
	glCompileShader(vertexShaderObject);

	// check compilation errors
	
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &shaderCompilationStatus);
	if(shaderCompilationStatus == GL_FALSE) {

		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLenght);
		if(infoLogLenght > 0) {

			szInfoLog = (char*) malloc(infoLogLenght);
			if(szInfoLog != NULL) {

				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLenght, &written, szInfoLog);
				printf("\n*** Vertex Shader Compilation Status ***\n");
				printf("%s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	//*************************************************************************************
	// 								*** FRAGMENT SHADER ***
	//*************************************************************************************

	// create fragment shader
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// write the shader source code
	const GLchar* fragmentShaderSourceCode = 
	"#version 450 core																									\n" \
	"\n 																												\n" \
	"in vec3	phong_ads_light;																						\n" \
	"out vec4	FragColor;																								\n" \
	"																													\n" \
	"void main(void) {																									\n" \
	"	FragColor = vec4(phong_ads_light,1.0);																			\n" \
	"}																													\n";
	
	// provide source code to shader
	glShaderSource(fragmentShaderObject, 1, (const GLchar**) &fragmentShaderSourceCode, NULL);

	// compile shader source code
	glCompileShader(fragmentShaderObject);

	// check compilation errors
	infoLogLenght				= 0;
	shaderCompilationStatus 	= 0;
	szInfoLog					= NULL;

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &shaderCompilationStatus);
	if(shaderCompilationStatus == GL_FALSE) {

		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLenght);
		if(infoLogLenght > 0) {

			szInfoLog = (char*) malloc(infoLogLenght);
			if(szInfoLog != NULL) {

				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLenght, &written, szInfoLog);
				printf("\n*** Fragment Shader Compilation Status ***\n");
				printf("%s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}


	//*************************************************************************************
	//							*** SHADER PROGRAM ***
	//*************************************************************************************

	// create shader program object
	shaderProgramObject = glCreateProgram();

	// attach vertex shader object to shader program object
	glAttachShader(shaderProgramObject, vertexShaderObject);

	// attach fragment shader object to shader program object
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// before linking, bind shader program with attributes
	glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_POSITION, "vPosition");
	//glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_COLOR, "vColor");
	glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_NORMAL, "vNormal");

	// link shader program
	glLinkProgram(shaderProgramObject);

	// check linking errors
	infoLogLenght				= 0;
	shaderCompilationStatus 	= 0;
	shaderProgramLinkStatus		= 0;
	szInfoLog					= NULL;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);
	if(shaderProgramLinkStatus == GL_FALSE) {

		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLenght);
		if(infoLogLenght > 0) {

			szInfoLog = (char*) malloc(infoLogLenght);
			if(szInfoLog != NULL) {
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject, infoLogLenght,&written, szInfoLog);
				printf("\n*** Shader Program Link Log ***\n");
				printf("%s\n", szInfoLog);
				uninitialize();
				exit(0);

			}
		}
	}

	//	get uniform locations from shader program
	modelMatrixUniform 		= glGetUniformLocation(shaderProgramObject, "u_model_matrix");
	viewMatrixUniform 		= glGetUniformLocation(shaderProgramObject, "u_view_matrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projection_matrix");
	lKeyPressed_uniform 	= glGetUniformLocation(shaderProgramObject, "u_lkeypressed");

	la_uniform[0] 			= glGetUniformLocation(shaderProgramObject, "u_la[0]"); // light ambient component
	ld_uniform[0] 			= glGetUniformLocation(shaderProgramObject, "u_ld[0]"); // light diffuse component
	ls_uniform[0] 			= glGetUniformLocation(shaderProgramObject, "u_ls[0]"); // light specular component

	la_uniform[1] 			= glGetUniformLocation(shaderProgramObject, "u_la[1]"); // light ambient component
	ld_uniform[1] 			= glGetUniformLocation(shaderProgramObject, "u_ld[1]"); // light diffuse component
	ls_uniform[1] 			= glGetUniformLocation(shaderProgramObject, "u_ls[1]"); // light specular component

	lightPosition_uniform[0] 	= glGetUniformLocation(shaderProgramObject, "u_light_position[0]"); // light position
	lightPosition_uniform[1] 	= glGetUniformLocation(shaderProgramObject, "u_light_position[1]"); // light position
	
	ka_uniform 				= glGetUniformLocation(shaderProgramObject, "u_ka"); // material ambient component
	kd_uniform 				= glGetUniformLocation(shaderProgramObject, "u_kd"); // material diffuse component
	ks_uniform 				= glGetUniformLocation(shaderProgramObject, "u_ks"); // material specular component
	shininess_uniform 		= glGetUniformLocation(shaderProgramObject, "u_shininess"); // material shininess Component
	//__________________________________________________________________________________
	//	 *** Vertices, Colors, Shaders Attributes, VBO, VAO initializations ***
	//__________________________________________________________________________________
		const GLfloat pyramidVertices[] =
	{
		//front face
		0.0f,	1.0f,	0.0f,
		-1.0f,	-1.0f,	1.0f,
		1.0f,	-1.0f,	1.0f,

		//right face
		0.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,	1.0f,
		1.0f,	-1.0f,	-1.0f,

		//back face
		0.0f,	1.0f,	0.0f,
		-1.0f,	-1.0f,	-1.0f,
		1.0f,	-1.0f,	-1.0f,

		//left face
		0.0f,	1.0f,	0.0f,
		-1.0f,	-1.0f,	-1.0f,
		-1.0f,	-1.0f,	1.0f

	};
	const GLfloat pyramidNormals[] = {
		// front face
		0.0f,	0.447214f,	0.894427f,
		0.0f,	0.447214f,	0.894427f,
		0.0f,	0.447214f,	0.894427f,

		// right face
		0.894427f,	0.447214f,	0.0f,
		0.894427f,	0.447214f,	0.0f,
		0.894427f,	0.447214f,	0.0f,
		// back face
		0.0f,	0.447214f,	-0.894427f,
		0.0f,	0.447214f,	-0.894427f,
		0.0f,	0.447214f,	-0.894427f,
		// left face
		-0.894427f,	0.447214f,	0.0f,
		-0.894427f,	0.447214f,	0.0f,
		-0.894427f,	0.447214f,	0.0f
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


	light[0].light_ambient  = vec3(0.0f, 0.0f, 0.0f);
	light[0].light_diffuse  = vec3(1.0f, 0.0f, 0.0f);
	light[0].light_specular = vec3(1.0f, 0.0f, 0.0f);
	light[0].light_position = vec4(-2.0f, 0.0f, 0.0f, 1.0f);

	light[1].light_ambient  = vec3(0.0f, 0.0f, 0.0f);
	light[1].light_diffuse  = vec3(0.0f, 0.0f, 1.0f);
	light[1].light_specular = vec3(0.0f, 0.0f, 1.0f);
	light[1].light_position = vec4(2.0f, 0.0f, 0.0f, 1.0f);

	gbLights  = false;
	gbAnimate = false;
	//glShadeModel(GL_SMOOTH);
	
	glClearDepth(1.0f);
	
	glEnable(GL_DEPTH_TEST);
	
	glDepthFunc(GL_LEQUAL);
	
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	// set projection matrix to identity
	perspectiveProjectionMatrix = mat4::identity();

	resize(giWindowWidth, giWindowHeight);
}


void resize(int width, int height) {

	// code
	if (height <= 0) {
		height = 1;
	}
	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width/(GLfloat)height,0.1f,100.0f);

}

void display(void) {
	
	// function prototype
	void update (void);

	// code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 modelMatrix 		= mat4::identity();
	mat4 viewMatrix 		= mat4::identity();
	mat4 projectionMatrix 	= mat4::identity();

	// start using shader program
	glUseProgram(shaderProgramObject);

		if(gbLights) {

		
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

		} else {
			glUniform1i(lKeyPressed_uniform, 0);
		}
		// Matrices
		modelMatrix 	 = vmath::translate(0.0f, 0.0f, -4.0f) * vmath::rotate(pyramidAngle, 0.0f,1.0f,0.0f);
		viewMatrix		 = mat4::identity();
		projectionMatrix = perspectiveProjectionMatrix;		

		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE,projectionMatrix);

		glBindVertexArray(vao_pyramid);
		glDrawArrays(GL_TRIANGLES, 0, 12);	// why 12 is last parameter because vertices * triangle faces
		glBindVertexArray(0);

	// stop using shader program
	glUseProgram(0);

	//update
	update();

	//swap buffers
	glXSwapBuffers(gpDisplay, gWindow);
	
}

void update (void) {
	
	pyramidAngle = pyramidAngle + 0.02f;
	if (pyramidAngle >= 360.0f) {
		pyramidAngle = 0.0f;
	}
}

void uninitialize(void) {


	if (vao_pyramid) {
		glDeleteVertexArrays(1, &vao_pyramid);
		vao_pyramid = 0;
	}

	if (vbo_pyramid_position) {
		glDeleteBuffers(1, &vbo_pyramid_position);
		vbo_pyramid_position = 0;
	}
	if (vbo_pyramid_normals) {
		glDeleteBuffers(1, &vbo_pyramid_normals);
		vbo_pyramid_normals = 0;
	}

	if(shaderProgramObject) {

		glUseProgram(shaderProgramObject);

			GLsizei shaderCount;
			glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);
			
			GLuint* p_shader = NULL;
			p_shader = (GLuint*) malloc(sizeof(GLint) * shaderCount);
			
			glGetAttachedShaders(shaderProgramObject, shaderCount, &shaderCount, p_shader);

			for(GLsizei i = 0; i < shaderCount; i++) {

				glDetachShader(shaderProgramObject, p_shader[i]);
				glDeleteShader(p_shader[i]);
				p_shader[i] = 0;
			}

			free(p_shader);
			glDeleteProgram(shaderProgramObject);
			shaderProgramObject = 0;

		glUseProgram(0);

	}

	GLXContext currentGLXContext;
	
	currentGLXContext = glXGetCurrentContext();
	
	if(currentGLXContext == gGlxContext) {
	
		glXMakeCurrent(gpDisplay, 0, 0);
	}
	
	if(gGlxContext) {
		
		glXDestroyContext(gpDisplay, gGlxContext);
	}
	
	if(gWindow) {
		// unallocate memory given to window
		XDestroyWindow(gpDisplay, gWindow);
	
	}
	
	if(gColormap) {
		// Free Color-Map
		XFreeColormap(gpDisplay,gColormap);
	}
	
	if(gpXVisualInfo) {
		// Free gpXVisualInfo
		free(gpXVisualInfo);
		gpXVisualInfo = NULL;
	}
	
	if(gpDisplay) {
		// Dissconect the Display To Client App
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}
	
	
}


void ToggleFullscreen(void) {

	// Variable Declarations
	Atom wm_state; 		// Window's Current State
	Atom fullscreen;	// fullscreen state's atom
	XEvent xev={0};		// Create Object Of Event And Make All Of Members Zero

	// Code
	
	wm_state = XInternAtom(gpDisplay, "_NET_WM_STATE", False); 	// _NET_WM_STATE Currnet Window State Accros The Network

	memset(&xev, 0, sizeof(xev)); 							// use memset to zero out or initalise all to zero use either one 

	xev.type = ClientMessage;								// Type Of Message of Client (Send To Client)
	xev.xclient.window = gWindow;							// To Whcih Window to send message
	xev.xclient.message_type = wm_state;					// What Order to send
	xev.xclient.format = 32;								// Order Byte 32 bit message
	xev.xclient.data.l[0] = bFullscreen ? 0 : 1;			// Toggle The State fullscreen to normal or normal to fullscreen
	
	fullscreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
	xev.xclient.data.l[1] = fullscreen;

	XSendEvent(gpDisplay, RootWindow(gpDisplay, gpXVisualInfo->screen),False,StructureNotifyMask,&xev);

}

/*
	COMPILE: 	g++ -o OGL OGL.cpp -lX11 -lGL -lGLEW -L"./" -lSphere
				
	RUN: 		./OGL

 */
	
