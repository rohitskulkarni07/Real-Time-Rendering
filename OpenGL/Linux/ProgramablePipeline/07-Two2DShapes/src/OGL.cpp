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

bool gbPolygonMode = true;


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


enum {
	RSK_ATTRIBUTE_POSITION = 0,
	RSK_ATTRIBUTE_COLOR,
	RSK_ATTRIBUTE_NORMAL,
	RSK_ATTRIBUTE_TEXTURE0,
};

// *** Shader Releted Variables***/
GLuint		vertexShaderObject;
GLuint 		fragmentShaderObject;
GLuint		shaderProgramObject;

// vertex array object and vertex buffer object

// for triangle
GLuint 		vao_triangle;
GLuint 		vbo_position_triangle;
GLuint 		vbo_color_triangle;

// for square
GLuint 		vao_square;
GLuint 		vbo_position_sqaure;
GLuint 		vbo_color_sqaure;

// variable for storing location of uniforms
GLuint		mvpMatrixUniform;

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
	"#version 450 core																\n" \
	"\n 																			\n" \
	"in vec4 vPosition;																\n" \
	"in vec4 vColor;																\n" \
	"uniform mat4 u_mvp_matrix;														\n" \
	"																				\n" \
	"out vec4 out_color;															\n" \
	"																				\n" \
	"void main(void) {																\n" \
	"	gl_Position = u_mvp_matrix * vPosition;										\n" \
	"	out_color = vColor;															\n" \
	"}																				\n";

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
	"#version 450 core																\n" \
	"\n 																			\n" \
	"in vec4 out_color;																\n" \
	"out vec4 FragColor;															\n" \
	"																				\n" \
	"void main(void) {																\n" \
	"	FragColor = out_color;														\n" \
	"}																				\n";
	
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
	glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_COLOR, "vColor");

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
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");
	//__________________________________________________________________________________
	//	 *** Vertices, Colors, Shaders Attributes, VBO, VAO initializations ***
	//__________________________________________________________________________________

	const GLfloat triangleVertices[] = {
		 0.0f,	 1.0f,	0.0f,	// apex
		-1.0f,	-1.0f,	0.0f,	// left-bottom
		 1.0f,	-1.0f,	0.0f	// right-bottom
	};

	const GLfloat triangleColor[] = {
		1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	1.0f
	};

	const GLfloat squareVertices[] = {
		 1.0f,	 1.0f,	0.0f,	// right-top
		-1.0f,	 1.0f,	0.0f,	// left-top
		-1.0f,	-1.0f,	0.0f,	// left-bottom
		 1.0f,	-1.0f,	0.0f	// right-bottom
	};

	const GLfloat squareColor[] = {
		0.0f,	0.0f,	1.0f,
		0.0f,	0.0f,	1.0f,
		0.0f,	0.0f,	1.0f,
		0.0f,	0.0f,	1.0f
	};

	//				*** For Triangle ***
	glGenVertexArrays(1, &vao_triangle);
	glBindVertexArray(vao_triangle);

		glGenBuffers(1, &vbo_position_triangle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position_triangle);
	
			glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &vbo_color_triangle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_color_triangle);
	
			glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);
			glVertexAttribPointer(RSK_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(RSK_ATTRIBUTE_COLOR);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	//				*** For Square ***

	glGenVertexArrays(1, &vao_square);
	glBindVertexArray(vao_square);

		glGenBuffers(1, &vbo_position_sqaure);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position_sqaure);
	
			glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &vbo_color_sqaure);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_color_sqaure);
	
			glBufferData(GL_ARRAY_BUFFER, sizeof(squareColor), squareColor, GL_STATIC_DRAW);
			glVertexAttribPointer(RSK_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(RSK_ATTRIBUTE_COLOR);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

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
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width/(GLfloat)height,0.1,100.0f);

}

void display(void) {
	
	// function prototype
	void update (void);

	// code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// start using shader program
	glUseProgram(shaderProgramObject);

		//____________________________________________________________________________________________________
		//					*** triangle ***
		//____________________________________________________________________________________________________
		// Matrices
		mat4 modelViewMatrix 			= mat4::identity();
		mat4 modelViewProjectionMatrix 	= mat4::identity();
		mat4 translationMatrix			= mat4::identity();

		translationMatrix = translate(-1.5f,0.0f,-4.0f);
		modelViewMatrix = translationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		// pass matrix to shader
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		//OpenGL drawing

		//	*** bind vao ***
		glBindVertexArray(vao_triangle);

		//draw
		glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

		// *** unbind vao ***
		glBindVertexArray(0);

		//____________________________________________________________________________________________________
		//							*** square ***
		//____________________________________________________________________________________________________

		// Matrices
		modelViewMatrix 			= mat4::identity();
		modelViewProjectionMatrix 	= mat4::identity();
		translationMatrix			= mat4::identity();

		translationMatrix = translate(1.5f,0.0f,-4.0f);
		modelViewMatrix = translationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		// pass matrix to shader
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		//OpenGL drawing

		//	*** bind vao ***
		glBindVertexArray(vao_square);

		//draw
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		// *** unbind vao ***
		glBindVertexArray(0);
		//____________________________________________________________________________________________________


	// stop using shader program
	glUseProgram(0);

	//update
	update();

	//swap buffers
	glXSwapBuffers(gpDisplay, gWindow);
	
}

void update (void) {

}

void uninitialize(void) {

	if(vbo_position_sqaure) {
		glDeleteBuffers(1, &vbo_position_sqaure);
		vbo_position_sqaure = 0;
	}

	if(vbo_color_sqaure) {
		glDeleteBuffers(1, &vbo_color_sqaure);
		vbo_color_sqaure = 0;
	}

	if(vao_square) {
		glDeleteVertexArrays(1, &vao_square);
		vao_square = 0;
	}

	if(vbo_position_triangle) {
		glDeleteBuffers(1, &vbo_position_triangle);
		vbo_position_triangle = 0;
	}

	if(vbo_color_triangle) {
		glDeleteBuffers(1, &vbo_color_triangle);
		vbo_color_triangle = 0;
	}

	if(vao_triangle) {
		glDeleteVertexArrays(1, &vao_triangle);
		vao_triangle = 0;
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
	
