//  30-TessellationControlShader_AND_TessellationEvaluationShader
//  OGL.m
//  
//
//  Created by Rohit Kulkarni on 07/01/22.
//
//#define GL_SILENCE_DEPRECATION
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>
#include "vmath.h"

using namespace vmath;


CVReturn myDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);

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

	GLfloat gfAngle;
	// *** Shader Related Variables***/
	GLuint		vertexShaderObject;
	GLuint 		fragmentShaderObject;

	GLuint		tesselationControlShaderObject;
	GLuint		tesselationEvaluationShaderObject;

	GLuint		shaderProgramObject;

	// vertex array object and vertex buffer object
	GLuint 		vao;
	GLuint 		vbo_position;

	// variable for storing location of uniforms
	GLuint		mvpMatrixUniform;

	GLuint		numberOfSegmentsUniform;
	GLuint		numberOfStripsUniform;
	GLuint		lineColorUniform;

	unsigned int uiNumberOfSegment;

	// projection matrix
	vmath::mat4 perspectiveProjectionMatrix;	// 4x4 matrix

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
		"in vec2 vPosition;                                 \n" \
		"                                                   \n" \
		"void main(void)                                    \n" \
		"{                                                  \n" \
		"                                                   \n" \
		"   gl_Position = vec4(vPosition, 0.0, 1.0);        \n" \
		"                                                   \n" \
		"}";//endOfCode

	// glShaderSource sets the source code in shader to the source code in the array of strings specified by string
    glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

    // compile shader source code
	glCompileShader(vertexShaderObject);

    // Error checking
	GLint infoLogLength				= 0;
	GLint shaderCompilationStatus 	= 0;
	GLint shaderProgramLinkStatus	= 0;
	char* szInfoLog					= NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &shaderCompilationStatus);

	if (shaderCompilationStatus == GL_FALSE) {
		
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

	//*************************************************************************************
	//							 *** TESSELATION CONTROL SHADER ***
	//*************************************************************************************

	// create TC shader
	tesselationControlShaderObject = glCreateShader(GL_TESS_CONTROL_SHADER);

	// write the shader source code
	const GLchar* tesselationControlShaderSourceCode = 
	"#version 410 core																						\n" \
	"layout(vertices = 4) out;																				\n" \
	"uniform int numberOfSegment;																			\n" \
	"uniform int numberOfStrips;																			\n" \
	"																										\n" \
	"void main(void) {																						\n" \
	"																										\n" \
	"gl_out[gl_InvocationID].gl_Position 	= gl_in[gl_InvocationID].gl_Position;							\n" \
	"gl_TessLevelOuter[0] 					= float(numberOfStrips);										\n" \
	"gl_TessLevelOuter[1] 					= float(numberOfSegment);										\n" \
	"																										\n" \
	"}																										\n";



	// provide source code to shader 
	glShaderSource(tesselationControlShaderObject, 1, (const GLchar**) &tesselationControlShaderSourceCode, NULL);

	// compile shader source code
	glCompileShader(tesselationControlShaderObject);

	// check compilation errors
	infoLogLength				= 0;
	shaderCompilationStatus 	= 0;
	shaderProgramLinkStatus		= 0;
	szInfoLog					= NULL;
	
	glGetShaderiv(tesselationControlShaderObject, GL_COMPILE_STATUS, &shaderCompilationStatus);
	if(shaderCompilationStatus == GL_FALSE) {

		glGetShaderiv(tesselationControlShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0) {

			szInfoLog = (char*) malloc(infoLogLength);
			if(szInfoLog != NULL) {

				GLsizei written;
				glGetShaderInfoLog(tesselationControlShaderObject, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "\n*** Tesselation Control Shader Compilation Status ***\n");
				fprintf(gpFile, "%s\n", szInfoLog);
				free(szInfoLog);
				[self release];
                [NSApp terminate: self];
			}
		}
	}

	//*************************************************************************************
	//							 *** TESSELATION EVALUATION SHADER ***
	//*************************************************************************************

	// create TEC shader
	tesselationEvaluationShaderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);

	// write the shader source code
	const GLchar* tesselationEvaluationShaderSourceCode = 
	"#version 410 core																						\n" \
	"\n 																									\n" \
	"layout(isolines) in;																					\n" \
	"uniform mat4 u_mvp_matrix;																				\n" \
	"																										\n" \
	"void main(void) {																						\n" \
	"																										\n" \
	"	float tessCoord = gl_TessCoord.x;																	\n" \
	"	vec3 p0 		= gl_in[0].gl_Position.xyz;															\n" \
	"	vec3 p1 		= gl_in[1].gl_Position.xyz;															\n" \
	"	vec3 p2 		= gl_in[2].gl_Position.xyz;															\n" \
	"	vec3 p3 		= gl_in[3].gl_Position.xyz;															\n" \
	"																										\n" \
	"	vec3 p			= p0 * (1.0 - tessCoord) * (1.0 - tessCoord) * (1.0 - tessCoord) + \
						  p1 * 3.0 * tessCoord * (1.0-tessCoord) * (1.0-tessCoord) + \
						  p2 * 3.0 * tessCoord * tessCoord * (1.0-tessCoord) + \
						  p3 * tessCoord * tessCoord * tessCoord;\n" \
	"	gl_Position 	= u_mvp_matrix * vec4(p, 1.0); 														\n" \
	"}																										\n";


	// provide source code to shader 
	glShaderSource(tesselationEvaluationShaderObject, 1, (const GLchar**) &tesselationEvaluationShaderSourceCode, NULL);

	// compile shader source code
	glCompileShader(tesselationEvaluationShaderObject);

	// check compilation errors
	infoLogLength				= 0;
	shaderCompilationStatus 	= 0;
	shaderProgramLinkStatus		= 0;
	szInfoLog					= NULL;
	
	glGetShaderiv(tesselationEvaluationShaderObject, GL_COMPILE_STATUS, &shaderCompilationStatus);
	if(shaderCompilationStatus == GL_FALSE) {

		glGetShaderiv(tesselationEvaluationShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0) {

			szInfoLog = (char*) malloc(infoLogLength);
			if(szInfoLog != NULL) {

				GLsizei written;
				glGetShaderInfoLog(tesselationEvaluationShaderObject, infoLogLength, &written, szInfoLog);
				fprintf(gpFile, "\n*** Tesselation Evaluation Shader Compilation Status ***\n");
				fprintf(gpFile, "%s\n", szInfoLog);
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
		"#version 410 core																\n" \
		"out vec4 FragColor;															\n" \
		"uniform vec4 lineColor;														\n" \
		"																				\n" \
		"void main(void) {																\n" \
		"																				\n" \
		"	FragColor = lineColor;														\n" \
		"																				\n" \
		"}																				\n";
	

    glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(fragmentShaderObject);

	// Error checking
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &shaderCompilationStatus);

	if (shaderCompilationStatus == GL_FALSE) {

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

   	// create shader program object
	shaderProgramObject = glCreateProgram();

	// attach vertex shader object to shader program object
	glAttachShader(shaderProgramObject, vertexShaderObject);

	// attach TCS to shader program object
	glAttachShader(shaderProgramObject, tesselationControlShaderObject);

	// attach TES to shader program object
	glAttachShader(shaderProgramObject, tesselationEvaluationShaderObject);

	// attach fragment shader object to shader program object
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// before linking, bind shader program with attributes
	glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_POSITION, "vPosition");

	// link shader program
	glLinkProgram(shaderProgramObject);
		// check linking errors
	infoLogLength				= 0;
	shaderCompilationStatus 	= 0;
	shaderProgramLinkStatus		= 0;
	szInfoLog					= NULL;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);
	if(shaderProgramLinkStatus == GL_FALSE) {

		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0) {

			szInfoLog = (char*) malloc(infoLogLength);
			if(szInfoLog != NULL) {
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject, infoLogLength,&written, szInfoLog);
				fprintf(gpFile, "\n*** Shader Program Link Log ***\n");
				fprintf(gpFile, "%s\n", szInfoLog);
				[self release];
                [NSApp terminate: self];

			}
		}
	}

	//	get uniform locations from shader program
	mvpMatrixUniform		 = glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");
	numberOfSegmentsUniform	 = glGetUniformLocation(shaderProgramObject, "numberOfSegment");
	numberOfStripsUniform	 = glGetUniformLocation(shaderProgramObject, "numberOfStrips");
	lineColorUniform		 = glGetUniformLocation(shaderProgramObject, "lineColor");



	const GLfloat lineVertices[] = 
	{
		-1.0f, -1.0f,
		-0.5f,  1.0f,
		 0.5f, -1.0f,
		 1.0f,  1.0f,
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

		glGenBuffers(1, &vbo_position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	
			glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);	
	
	//Mandatory Function To Initialize Depth Buffer With All Ones
	glClearDepth(1.0f);

	//Mandatory Function To Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	//Mandatory Function To Checking Depth Bits With Reference Value
	glDepthFunc(GL_LEQUAL);

	gfAngle = 0.0f;
	uiNumberOfSegment = 1;
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
	glLineWidth(5.0f);

    	// start using shader program
	glUseProgram(shaderProgramObject);

		// Matrices
		mat4 modelViewMatrix 			= mat4::identity();
		mat4 modelViewProjectionMatrix 	= mat4::identity();
		mat4 translationMatrix			= mat4::identity();

		translationMatrix = translate(0.0f,0.0f,-3.0f);
		modelViewMatrix = translationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		float color[] = {sin(gfAngle), cos(gfAngle), 0.9, 0.0};
		// pass matrix to shader
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
		glUniform1i(numberOfSegmentsUniform, uiNumberOfSegment);
		glUniform1i(numberOfStripsUniform, 1);
		glUniform4fv(lineColorUniform, 1, (float*)color);

		//OpenGL drawing

		//	*** bind vao ***
		glBindVertexArray(vao);

		//draw
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawArrays(GL_PATCHES, 0, 4);

		// *** unbind vao ***
		glBindVertexArray(0);

	// stop using shader program
	glUseProgram(0);

	[self updateFunc];

    // update function call should be here
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

}

-(void) updateFunc {
	gfAngle += 0.01f;
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
		
		case 'u': //up
		case 'U': //up
			uiNumberOfSegment++;
			if(uiNumberOfSegment>=30) {
				uiNumberOfSegment = 30;
			}
			break;

		case 'd': //down
		case 'D': //down
			uiNumberOfSegment--;
			if(uiNumberOfSegment<=0) {
				uiNumberOfSegment = 1;
			}
			break;

        default:
            break;
    }
}

-(void)dealloc {
    
   if(vbo_position) {
		glDeleteBuffers(1, &vbo_position);
		vbo_position = 0;
	}

	if(vao) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
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

