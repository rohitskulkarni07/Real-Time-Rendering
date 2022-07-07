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
#include <SOIL/SOIL.h> 	// Simple OpenGL Image Library

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
bool gbPolygonMode		= true;

GLfloat pyramidAngle	= 0.0f;
GLfloat cubeAngle		= 0.0f;

enum {
	RSK_ATTRIBUTE_POSITION = 0,
	RSK_ATTRIBUTE_COLOR,
	RSK_ATTRIBUTE_NORMAL,
	RSK_ATTRIBUTE_TEXTURE0,
};

//Texture Releted Varaibles GLuint  = OpenGL unsinged integer
GLuint guiStone_texture;
GLuint guiKundali_texture;
GLuint guiPressedDigit;

// *** Shader Releted Variables***/
GLuint		vertexShaderObject;
GLuint 		fragmentShaderObject;
GLuint		shaderProgramObject;

// vertex array object and vertex buffer object

// for pyramid
GLuint 		vao_pyramid;
GLuint 		vbo_position_pyramid;
GLuint 		vbo_textured_pyramid;

// for Cube
GLuint 		vao_cube;
GLuint 		vbo_position_cube;
GLuint 		vbo_textured_cube;

// variable for storing location of uniforms
GLuint		mvpMatrixUniform;
GLuint 		textureSamplerUniform;	//for sampler


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
	GLuint LoadBitmapAsTexture(const char*);
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
		"#version 450 core" \
		"\n" \
		"in vec2 out_texcoord;" \
		"uniform sampler2D u_texture_sampler;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = texture( u_texture_sampler, out_texcoord);" \
		"}"; //endofcode

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
	glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_TEXTURE0, "vTexCoord");

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
	mvpMatrixUniform 		= glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");
	textureSamplerUniform 	= glGetUniformLocation(shaderProgramObject, "u_texture_sampler");
	//__________________________________________________________________________________
	//	 *** Vertices, Colors, Shaders Attributes, VBO, VAO initializations ***
	//__________________________________________________________________________________
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
	guiStone_texture = LoadBitmapAsTexture("Stone.bmp");
	guiKundali_texture = LoadBitmapAsTexture("Vijay_Kundali.bmp");

	//Enable Texture;
	glEnable(GL_TEXTURE_2D);
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


GLuint LoadBitmapAsTexture(const char* path) {
	
	// Variable Declarations
	int width, height;
	unsigned char* imageData = NULL;
	GLuint textureID;
	
	// code

	/* Function:
	 * SOIL_load_image(const char* path, int *width, int *height, channel, colormode);
	 * 
	*/
	imageData = SOIL_load_image(path, &width, &height, NULL, SOIL_LOAD_RGB);
	
	// The glPixelStoref and glPixelStorei functions set pixel storage modes.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	//The glGenTextures function generates texture names
	// 1 denotes total number of texture to be generate
	// here texture is filled with proper texture handle from gpu memory
	glGenTextures(1, &textureID);

	//Bind 2d Texture to the texture variable
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Setting Texture Paramters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(imageData);

	return(textureID);

}

void resize(int width, int height) {

	// code
	if (height <= 0) {
		height = 1;
	}
	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width/(GLfloat)height,0.1,100.0f);

}

void display(void) {
	
	// function prototype
	void update (void);

	// code
// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear My Window With Color Set by glClearColor() and Set The Buffer Bit With Given Value In Initialize Funtion

	//start using OpenGL program Object
	glUseProgram(shaderProgramObject);

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
	//update
	update();

	//swap buffers
	glXSwapBuffers(gpDisplay, gWindow);
	
}

void update (void) {
	
	
	pyramidAngle	+=	0.01f;
	if(pyramidAngle>=360.0f) {
		pyramidAngle = 0.0f;
	}
		
	
	cubeAngle		+=	0.01f;
	if(cubeAngle>=360.0f) {
		cubeAngle = 0.0f;
	}
	

}

void uninitialize(void) {


	if(vbo_position_cube) {
		glDeleteBuffers(1, &vbo_position_cube);
		vbo_position_cube = 0;
	}

	if(vbo_textured_cube) {
		glDeleteBuffers(1, &vbo_textured_cube);
		vbo_textured_cube = 0;
	}

	if(vao_cube) {
		glDeleteVertexArrays(1, &vao_cube);
		vao_cube = 0;
	}

	if(vbo_position_pyramid) {
		glDeleteBuffers(1, &vbo_position_pyramid);
		vbo_position_pyramid = 0;
	}

	if(vbo_textured_pyramid) {
		glDeleteBuffers(1, &vbo_textured_pyramid);
		vbo_textured_pyramid = 0;
	}

	if(vao_pyramid) {
		glDeleteVertexArrays(1, &vao_pyramid);
		vao_pyramid = 0;
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
	COMPILE: 	g++ -o OGL OGL.cpp -lX11 -lGL -lGLEW
	RUN: 		./OGL

 */
	
