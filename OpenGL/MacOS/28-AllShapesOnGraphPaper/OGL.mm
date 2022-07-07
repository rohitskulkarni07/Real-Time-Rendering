//  28-AllShapesOnGraphPaper
//  OGL.m
//  
//
//  Created by Rohit Kulkarni on 08/01/22.
//
#define GL_SILENCE_DEPRECATION
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>
#include "vmath.h"

using namespace vmath;


CVReturn myDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);
	
// DISTANCE FORMULA Length Of Sides Of Triangle
GLfloat distAB = sqrt(pow(-0.5f - 0.0f, 2) + pow(-0.5f - 0.5f, 2));
GLfloat distBC = sqrt(pow(0.5f - (-0.5f), 2) + pow(-0.5f - (-0.5f), 2));
GLfloat distAC = sqrt(pow(0.5f - 0.0f, 2) + pow(-0.5f - 0.5f, 2));

GLfloat s = (distAB + distBC + distAC) / (2.0f);

// RADIUS OF CIRCLE or Inner-radius
GLfloat rad = (sqrt(s * (s - distAB) * (s - distBC) * (s - distAC))) / (s);

// x,y of in-circle
GLfloat K = (distAB + distBC + distAC);
GLfloat ix = ((distBC * 0.0f) + (distAC * (-0.5f)) + (distAB * (0.5f))) / K;
GLfloat iy = ((distBC * (0.5f)) + (distAC * (-0.5f)) + (distAB * (-0.5f))) / K;

// Distance Formula For Outer Circle radius
GLfloat gfRadOuter = sqrt(pow(0.5f - 0.0f, 2) + pow(0.5f - 0.0f, 2));

FILE *gpFile = NULL;

@interface AppDelegate: NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

int main(int argc, char* argv[]) {
    
    // code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSApp = [NSApplication sharedApplication];
    [NSApp setDelegate: [[AppDelegate alloc] init]];
    
    // run loop
    [NSApp run];
    
    [pool release];
    
    return 0;
}

@interface MyOpenGLView: NSOpenGLView
@end

@implementation AppDelegate {
    @private
    NSWindow *window;
    MyOpenGLView *myOpenGLView;
}

-(void) applicationDidFinishLaunching: (NSNotification *)aNotification {
    
    // code
    
    // Log File Related
    NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appBuildPath = [appBundle bundlePath];
    NSString *parentDirPath = [appBuildPath stringByDeletingLastPathComponent];
    NSString *logFileNameWithPath = [NSString stringWithFormat: @"%@/log.txt", parentDirPath];

    const char* pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding: NSASCIIStringEncoding];

    gpFile = fopen(pszLogFileNameWithPath, "w");

    if(gpFile == NULL) {
        [self release];
        [NSApp terminate: self];
    }

    fprintf(gpFile, "\nProgram Started Successfully: %d\n",__LINE__);
    NSRect win_rect = NSMakeRect(0.0, 0.0, 800, 600); // internally c library function (Core Graphics)
    window = [
        [NSWindow alloc] initWithContentRect: win_rect
                                         styleMask: NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                           backing: NSBackingStoreBuffered
                                            defer: NO
    ];
    [window setTitle: @"MacOS | OpenGL - Window"];
    [window center];
    
    myOpenGLView = [[MyOpenGLView alloc] initWithFrame: win_rect];
    [window setContentView: myOpenGLView];
    [window setDelegate: self];
    [window makeKeyAndOrderFront: self];
}

-(void) applicationWillTerminate: (NSNotification *)aNotification {
    // code
    if(gpFile) {
        fprintf(gpFile, "\nProgram Completed Successfully: %d\n",__LINE__);
    }
}

-(void) windowWillClose: (NSNotification *)aNotification {
    
    // code
    [NSApp terminate: self];
    
}

-(void) dealloc {
    
    // code
    [myOpenGLView release];
    [window release];
    [super dealloc];
}
@end

@implementation MyOpenGLView {
    
    @private
    CVDisplayLinkRef displayLink;

    // OpenGL & GL-SL Related Declarations
    enum {
	    RSK_ATTRIBUTE_POSITION = 0,
	    RSK_ATTRIBUTE_COLOR,
	    RSK_ATTRIBUTE_NORMAL,
	    RSK_ATTRIBUTE_TEXTURE0,
    };



    GLuint vertexShaderObject;
    GLuint fragmentShaderObject;
    GLuint shaderProgramObject;

 
	GLuint vao;
	GLuint vbo_position;
	GLuint vbo_color;

	GLuint vao_triangle;
	GLuint vbo_position_triangle;

	GLuint vao_circle;
	GLuint vbo_position_circle;

	GLuint vao_outer_circle;
	GLuint vbo_position_outer_circle;

	GLuint vao_rect;
	GLuint vbo_position_rect;

	GLuint mvpMatrixUniform;
	GLuint colorUniform;
    
    mat4 perspectiveProjectionMatrix; // from vmath 4x4 Matrix

}

-(id) initWithFrame: (NSRect)frame {
    
    // code
    self = [super initWithFrame: frame];
    
    if(self) {

        NSOpenGLPixelFormatAttribute attributes[] = 
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
            NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
            NSOpenGLPFANoRecovery,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFADoubleBuffer,
            0
        };

        NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes: attributes] autorelease];
        if(pixelFormat == nil) {
            fprintf(gpFile, "\nFailed to get pixel format, exiting now! : %d\n",__LINE__);
            [self release];
            [NSApp terminate: self];
        }
        NSOpenGLContext *glContext = [[[NSOpenGLContext alloc] initWithFormat: pixelFormat shareContext: nil] autorelease];
        [self setPixelFormat: pixelFormat];
        [self setOpenGLContext: glContext];
    }
    
    return self;
}

-(CVReturn) getFrameForTime: (const CVTimeStamp*)outputTime {
    
    // code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    [self drawView];
    [pool release];

    return (kCVReturnSuccess);
}

-(void) prepareOpenGL {
    
    // code
    [super prepareOpenGL];

    [[self openGLContext] makeCurrentContext];

    // swap interval for synchronization between horizontal and vertical sync
    GLint swapInt = 1;
    [[self openGLContext] setValues: &swapInt forParameter: NSOpenGLCPSwapInterval];
    
    //GL-SL related
	/**** VERTEX SHADER SOURCE CODE ****/
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
    const GLchar* vertexShaderSourceCode =
		"#version 410 core                                  \n" \
		"                                                   \n" \
		"in vec4 vPosition;                                 \n" \
		"                                                   \n" \
		"uniform mat4 u_mvp_matrix;                         \n" \
		"                                                   \n" \
		"                                                   \n" \
		"void main(void)                                    \n" \
		"{                                                  \n" \
		"                                                   \n" \
		"   gl_Position = u_mvp_matrix * vPosition;         \n" \
		"                                                   \n" \
		"}";//endOfCode

	// glShaderSource sets the source code in shader to the source code in the array of strings specified by string
    glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

    // compile shader source code
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
				fprintf(gpFile, "*** Vertex Shader Compilation Status ***\n%s\n",szInfoLog);
				free(szInfoLog);

                [self release];
                [NSApp terminate: self];
			}	
		}
	}

	/**** FRAGMENT SHADER SOURCE CODE****/
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    // provide source code to shader
	const GLchar* fragmentShaderSourceCode =
		"#version 410 core                              \n" \
		"                                               \n" \
		"out vec4 FragColor;                            \n" \
		"uniform vec4 out_color;                        \n" \
		"void main(void)                                \n" \
		"{                                              \n" \
		"                                               \n" \
		"   FragColor = out_color;						\n" \
		"                                               \n" \
		"}"; //endOfCode

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
				fprintf(gpFile, "*** Fragment Shader Compilation Status ***\n%s\n", szInfoLog);
				free(szInfoLog);
				
                [self release];
                [NSApp terminate: self];
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

                [self release];
                [NSApp terminate: self];
			}
		}
	}

	//get MVP uniform location
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");
	colorUniform = glGetUniformLocation(shaderProgramObject, "out_color");
	// *** vertices, colors, shader attributes, vbo, vao initialization ***
	const GLfloat triangle_vertices[] =
	{
		0.0f,	0.5f,	0.0f,
		-0.5f,	-0.5f,	0.0f,
		0.5f,	-0.5f,	0.0f,
	};

	const GLfloat rect_vertices[] =
	{
		1.0f,	1.0f,	0.0f,
		-1.0f,	1.0f,	0.0f,
		-1.0f,	-1.0f,	0.0f,
		1.0f,	-1.0f,	0.0f
	};


	GLfloat circle_vertices[1080];
	GLfloat angle = 0.0f;
	GLfloat circleRadius = rad;

	for (int i = 0; i < 360; i++) {

		circle_vertices[i * 3]		= circleRadius * cos(angle);
		circle_vertices[i * 3 + 1]	= circleRadius * sin(angle);
		circle_vertices[i * 3 + 2]	= 0.0f;

		angle += 2 * M_PI / 360.0f;
	}

	GLfloat outer_circle_vertices[1080];
	angle = 0.0f;
	circleRadius = gfRadOuter;
	for (int i = 0; i < 360; i++) {

		outer_circle_vertices[i * 3] = circleRadius * cos(angle);
		outer_circle_vertices[i * 3 + 1] = circleRadius * sin(angle);
		outer_circle_vertices[i * 3 + 2] = 0.0f;

		angle += 2 * M_PI / 360.0f;
	}

	const GLfloat lineVertices[] =
	{
		0.0f,	1.0f,	0.0f,
		0.0f,	-1.0f,	0.0f
	};
	

	glGenVertexArrays(1, &vao); // create named buffer object for vertex buffer object
	glBindVertexArray(vao);		// bind vao with buffer object

		//for position
		glGenBuffers(1, &vbo_position);	// creating named buffer object to map VRAM Buffer with Pointer
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position); // bind named object with VRAM Buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW); //feed the data to VRAM buffer
		glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);	// how to read data from buffer
		glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);	//Un-Bind

	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_circle); // create named buffer object for vertex buffer object
	glBindVertexArray(vao_circle);		// bind vao with buffer object

		//for position
	glGenBuffers(1, &vbo_position_circle);	// creating named buffer object to map VRAM Buffer with Pointer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_circle); // bind named object with VRAM Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(circle_vertices), circle_vertices, GL_STATIC_DRAW); //feed the data to VRAM buffer
	glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);	// how to read data from buffer
	glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	//Un-Bind

	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_outer_circle); // create named buffer object for vertex buffer object
	glBindVertexArray(vao_outer_circle);		// bind vao with buffer object

		//for position
	glGenBuffers(1, &vbo_position_outer_circle);	// creating named buffer object to map VRAM Buffer with Pointer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_outer_circle); // bind named object with VRAM Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(outer_circle_vertices), outer_circle_vertices, GL_STATIC_DRAW); //feed the data to VRAM buffer
	glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);	// how to read data from buffer
	glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	//Un-Bind

	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_triangle); // create named buffer object for vertex buffer object
	glBindVertexArray(vao_triangle);		// bind vao with buffer object

		//for position
	glGenBuffers(1, &vbo_position_triangle);	// creating named buffer object to map VRAM Buffer with Pointer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_triangle); // bind named object with VRAM Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW); //feed the data to VRAM buffer
	glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);	// how to read data from buffer
	glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	//Un-Bind

	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_rect); // create named buffer object for vertex buffer object
	glBindVertexArray(vao_rect);		// bind vao with buffer object

		//for position
	glGenBuffers(1, &vbo_position_rect);	// creating named buffer object to map VRAM Buffer with Pointer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_rect); // bind named object with VRAM Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW); //feed the data to VRAM buffer
	glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);	// how to read data from buffer
	glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	//Un-Bind

	glBindVertexArray(0);
	//___________________________________________________________________________
	
	
	//Mandatory Function To Initialize Depth Buffer With All Ones
	glClearDepth(1.0f);

	//Mandatory Function To Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	//Mandatory Function To Checking Depth Bits With Reference Value
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    //set orthographicMatrix to identity matrix
	perspectiveProjectionMatrix = mat4::identity();

    // core graphics related
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputCallback(displayLink, &myDisplayLinkCallback, self);
    CGLContextObj cglContext = (CGLContextObj) [[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj) [[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    CVDisplayLinkStart(displayLink);

	fprintf(gpFile, "prepareOpenGL succeeded \n");

}

-(void) reshape {

    // code
    [super reshape];
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

    NSRect rect = [self bounds];

    if(rect.size.height <= 0) {
        rect.size.height = 1;
    }
    glViewport(0, 0, (GLsizei)rect.size.width, (GLsizei)rect.size.height);

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)rect.size.width/(GLfloat)rect.size.height, 0.1f,100.0f);

    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
	fprintf(gpFile, "reshape succeeded\n");

}

-(void) drawRect: (NSRect)dirtyRect {
    
    // code
    [self drawView];

}

-(void) drawView {

    // code
    [[self openGLContext] makeCurrentContext];
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  	[self drawGraph];
	[self drawCircle];
	[self drawTriangle];
	[self myDrawRect];
	[self drawOuterCircle];

	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 translateMatrix = mat4::identity();

	float color_yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	modelViewMatrix = vmath::translate(0.0f, 0.0f, -2.5f)*vmath::scale(1.0f,0.5f,0.0f);
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;
	[self drawLine: modelViewProjectionMatrix secondParam:color_yellow];

    // update function call should be here
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

}


-(void) myDrawRect {

	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 translateMatrix = mat4::identity();

	float color_yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	modelViewMatrix = vmath::translate(0.0f, 0.0f, -2.5f) * vmath::scale(0.50f, 0.50f, 0.50f);
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUseProgram(shaderProgramObject);

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
	glUniform4fv(colorUniform, 1, (GLfloat*)color_yellow);
	glBindVertexArray(vao_rect);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glBindVertexArray(0);

	glUseProgram(0);

}

-(void) drawTriangle {
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 translateMatrix = mat4::identity();

	float color_yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	modelViewMatrix = vmath::translate(0.0f, 0.0f, -2.5f);
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUseProgram(shaderProgramObject);

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
	glUniform4fv(colorUniform, 1, (GLfloat*)color_yellow);
	glBindVertexArray(vao_triangle);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
	glBindVertexArray(0);

	glUseProgram(0);

}

-(void) drawCircle {
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 translateMatrix = mat4::identity();

	float color_yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	modelViewMatrix = vmath::translate(ix, iy, -2.5f);
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUseProgram(shaderProgramObject);

		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
		glUniform4fv(colorUniform, 1, (GLfloat*)color_yellow);
		glBindVertexArray(vao_circle);
		glDrawArrays(GL_LINE_LOOP, 0, 360);
		glBindVertexArray(0);

	glUseProgram(0);

}

-(void) drawOuterCircle {
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 translateMatrix = mat4::identity();

	float color_yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	modelViewMatrix = vmath::translate(0.0f,0.0f, -2.5f);
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUseProgram(shaderProgramObject);

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
	glUniform4fv(colorUniform, 1, (GLfloat*)color_yellow);
	glBindVertexArray(vao_outer_circle);
	glDrawArrays(GL_LINE_LOOP, 0, 360);
	glBindVertexArray(0);

	glUseProgram(0);
}

-(void) drawGraph{

	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 translateMatrix = mat4::identity();

	float color_red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float color_green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float color_blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	for (float y = 0.0f; y <= 1.0f; y = y += 0.05f) {

		modelViewMatrix = vmath::translate(y, 0.0f, -2.5f);
		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		[self drawLine: modelViewProjectionMatrix secondParam:color_blue];

		modelViewMatrix = vmath::translate(-y, 0.0f, -2.5f);
		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;
		[self drawLine: modelViewProjectionMatrix secondParam:color_blue];

		modelViewMatrix = vmath::translate(0.0f, y, -2.5f) * vmath::rotate(90.0f, 0.0f, 0.0f, 1.0f);
		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;
		[self drawLine: modelViewProjectionMatrix secondParam:color_blue];

		modelViewMatrix = vmath::translate(0.0f, -y, -2.5f) * vmath::rotate(90.0f, 0.0f, 0.0f, 1.0f);
		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;
		[self drawLine: modelViewProjectionMatrix secondParam:color_blue];

	}

	modelViewMatrix = vmath::translate(0.0f, 0.0f, -2.5f);
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;
	[self drawLine: modelViewProjectionMatrix secondParam:color_red];

	modelViewMatrix = vmath::translate(0.0f, 0.0f, -2.5f) * vmath::rotate(90.0f, 0.0f, 0.0f, 1.0f);
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;
	[self drawLine: modelViewProjectionMatrix secondParam:color_red];

}

-(void) drawLine: (mat4)modelViewProjectionMatrix secondParam: (float[]) color {

	glUseProgram(shaderProgramObject);

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
	glUniform4fv(colorUniform, 1, (GLfloat*)color);
	glBindVertexArray(vao);
		glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);

	glUseProgram(0);

}

-(BOOL) acceptsFirstResponder{
    
    // code
    [[self window] makeFirstResponder: self];
    return YES;
}

-(void) keyDown: (NSEvent*) theEvent {
    // code
    int key = [[theEvent characters] characterAtIndex: 0];
    
    switch(key) {
        case 27:
            [self release];
            [NSApp terminate: self];
            break;
            
        case 'f':
        case 'F':
            [[self window] toggleFullScreen: self];
            break;
        default:
            break;
    }
}

-(void)dealloc {
    
    if (vao) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	
	if (vbo_position) {
		glDeleteBuffers(1, &vbo_position);
		vbo_position = 0;
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

    [super dealloc];

    CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);

}
@end

CVReturn myDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext) {
    
    // code
    CVReturn result = [(MyOpenGLView*)displayLinkContext getFrameForTime: outputTime];
    return result;
}
