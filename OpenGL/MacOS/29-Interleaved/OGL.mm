//  29 - Interleaved
//  OGL.m
//  
//
//  Created by Rohit Kulkarni on 13/01/22.
//
#define GL_SILENCE_DEPRECATION
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

	GLuint  guiTexture_marble;


	// *** Shader Releted Variables***/
	
	GLuint vertexShaderObject;
	GLuint fragmentShaderObject;
	GLuint shaderProgramObject;

	// for cube
	GLuint vao_cube;
	GLuint vbo_position_cube;

	// variable for storing location of uniforms
	GLuint uniform_modelViewMatrixUniform;
	GLuint uniform_perspectiveProjection;
	GLuint lKeyPressed_uniform;
	GLuint mvpMatrixUniform;


	GLuint ld_uniform;	//light diffuse
	GLuint kd_uniform;	//material diffuse

	GLuint lightPosition_uniform; //position of light
	GLuint textureSamplerUniform;	//for sampler

	bool bLight;
	bool bAnimate;

	GLfloat gfAngle;
	// projection matirx
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


-(GLuint) loadTextureFromBMPFile: (const char*)fileName {

	// code
	NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appBuildPath = [appBundle bundlePath];
    NSString *parentDirPath = [appBuildPath stringByDeletingLastPathComponent];
    NSString *imageFileNameWithPath = [NSString stringWithFormat: @"%@/%s", parentDirPath, fileName];

	// Texture related code 
	NSImage *bmpImage = [[NSImage alloc] initWithContentsOfFile: imageFileNameWithPath]; // Get NSImage Representation of our image file
	
	// error checking
	if(!bmpImage) {
		fprintf(gpFile, "\nFailed to get NSImage format for BMP file, exiting now! : %d\n",__LINE__);
		return -1;
	}

	CGImageRef cgImage = [bmpImage CGImageForProposedRect: nil context:nil hints:nil];	// Get CG Image Representation of NSImage
	
	// Get Width and Height of Image
	int imageWidth = (int) CGImageGetWidth(cgImage);
	int imageHeight = (int) CGImageGetHeight(cgImage);

	// Get Core Foundation Data Representation
	CFDataRef imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));

	// Convert CF Data Ref Format Data Into Generic Data
	void* pixel = (void*) CFDataGetBytePtr(imageData);

	GLuint texture;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	glGenerateMipmap(GL_TEXTURE_2D);

	CFRelease(imageData);

	return texture;
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
		"#version 410 core																\n" \
		"																				\n" \
		"in vec4 vPosition;																\n" \
		"in vec3 vColor;																\n" \
		"in vec3 vNormal;																\n" \
		"in vec2 vTexCoord;																\n" \
		"																				\n" \
		"out vec2 out_texcoord;															\n" \
		"out vec3 out_color;															\n" \
		"																				\n" \
		"uniform mat4 u_model_view_matrix;												\n" \
		"uniform mat4 u_projection_matrix;												\n" \
		"uniform int u_lkeypressed;														\n" \
		"uniform vec3 u_ld;																\n" \
		"uniform vec3 u_kd;																\n" \
		"uniform vec4 u_light_position;													\n" \
		"																				\n" \
		"out vec3 diffuse_light;														\n" \
		"																				\n" \
		"void main(void)																\n" \
		"{																				\n" \
		"																				\n" \
		"	if(u_lkeypressed == 1) {													\n" \
		"		vec4 eye_coordinates = u_model_view_matrix * vPosition;					\n" \
		"		mat3 normal_matrix = mat3(transpose(inverse(u_model_view_matrix)));		\n" \
		"		vec3 t_norm = normalize(normal_matrix*vNormal);							\n" \
		"		vec3 s = normalize(vec3(u_light_position-eye_coordinates));				\n" \
		"		diffuse_light = u_ld * u_kd * max(dot(s, t_norm),0.0);					\n" \
		"	}																			\n" \
		"																				\n" \
		"	gl_Position = u_projection_matrix * u_model_view_matrix * vPosition;		\n" \
		"	out_texcoord = vTexCoord;													\n" \
		"	out_color = vColor;															\n" \
		"}																				\n";//end of code


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
		"#version 410 core									\n" \
		"													\n" \
		"in vec3 out_color;"\
		"vec4 color;"\
		"in vec2 out_texcoord;" \
		""\
		"uniform sampler2D u_texture_sampler;" \
		"in vec3 diffuse_light;"\
		"uniform int u_lkeypressed;"\
		""\
		"out vec4 FragColor;"\
		""\
		"void main(void)"\
		"{" \
		"if(u_lkeypressed == 1) {"\
			"color = vec4(out_color, 1.0) * vec4(diffuse_light, 1.0) * texture( u_texture_sampler, out_texcoord); "\
		"} else {"\
			"color = vec4(out_color, 1.0) * texture( u_texture_sampler, out_texcoord);"\
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
	glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_COLOR, "vColor");

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
	uniform_modelViewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_model_view_matrix");
	uniform_perspectiveProjection = glGetUniformLocation(shaderProgramObject, "u_projection_matrix");
	
	lKeyPressed_uniform = glGetUniformLocation(shaderProgramObject, "u_lkeypressed");

	ld_uniform = glGetUniformLocation(shaderProgramObject, "u_ld"); // light diffuse component
	kd_uniform = glGetUniformLocation(shaderProgramObject, "u_kd"); // material diffuse component
	lightPosition_uniform = glGetUniformLocation(shaderProgramObject, "u_light_position");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_texture_sampler");

	// *** vertices, colors, shader attributes, vbo, vao_cube initialization ***
	// *** vertices, colors, shader attribs, vbo, vao initializationa ***

	const GLfloat interleavedArray[] =
	{	// vertice				//color				//normals			//tex-coords
		 1.0f,	1.0f, 1.0f,		1.0f, 0.89f, 0.89f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-1.0f,	1.0f, 1.0f,		0.89f, 0.94f, 1.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.90, 0.89f, 0.94f,	   0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,		0.98f, 0.8f, 1.0f,	   0.0f, 0.0f, 1.0f,	0.0f, 1.0f,

		1.0f,  1.0f, -1.0f,		1.0f, 0.89f, 0.89f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		1.0f,  1.0f,  1.0f,		0.89f, 0.94f, 1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		1.0f, -1.0f,  1.0f,		0.90, 0.89f, 0.94f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		1.0f, -1.0f, -1.0f,		0.98f, 0.8f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,

		 1.0f,  1.0f, -1.0f,	1.0f, 0.89f, 0.89f,		0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.89f, 0.94f, 1.0f,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,	0.90, 0.89f, 0.94f,		0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,	0.98f, 0.8f, 1.0f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f,
																	
		-1.0f,  1.0f, -1.0f,	1.0f, 0.89f, 0.89f,		-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,	0.89f, 0.94f, 1.0f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,	0.90, 0.89f, 0.94f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,	0.98f, 0.8f, 1.0f,		-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
																	
		 1.0f, 1.0f,  1.0f,		1.0f, 0.89f, 0.89f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-1.0f, 1.0f,  1.0f,		0.89f, 0.94f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, 1.0f, -1.0f,		0.90, 0.89f, 0.94f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 1.0f, 1.0f, -1.0f,		0.98f, 0.8f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
																		
		 1.0f, -1.0f,  1.0f,	1.0f, 0.89f, 0.89f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,	0.89f, 0.94f, 1.0f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,	0.90, 0.89f, 0.94f,		0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,	0.98f, 0.8f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f
	};																

	//__________________________________________________________________________________________

	//For Square
	glGenVertexArrays(1, &vao_cube);
	glBindVertexArray(vao_cube);

		//for position of cube
		glGenBuffers(1, &vbo_position_cube);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(interleavedArray), interleavedArray, GL_STATIC_DRAW);
		
		glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);

		glVertexAttribPointer(RSK_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(RSK_ATTRIBUTE_COLOR);

		glVertexAttribPointer(RSK_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*) (6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(RSK_ATTRIBUTE_NORMAL);

		glVertexAttribPointer(RSK_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*) (9 * sizeof(GLfloat)));
		glEnableVertexAttribArray(RSK_ATTRIBUTE_TEXTURE0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	//___________________________________________________________________________
		
	guiTexture_marble = [self loadTextureFromBMPFile: "marble.bmp"];
	if(guiTexture_marble == -1) {
		[self release];
        [NSApp terminate: self];
	}
	bLight = false;
	bAnimate = false;

    //Enable Texture;
	glEnable(GL_TEXTURE_2D);

	//Mandatory Function To Initialize Depth Buffer With All Ones
	glClearDepth(1.0f);

	//Mandatory Function To Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	//Mandatory Function To Checking Depth Bits With Reference Value
	glDepthFunc(GL_LEQUAL);

	gfAngle = 0.0f;
	bLight = false;
	bAnimate = false;

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

		rotationMatrixX = vmath::rotate(gfAngle, 1.0f, 0.0f, 0.0f);
		rotationMatrixY = vmath::rotate(gfAngle, 0.0f, 1.0f, 0.0f);
		rotationMatrixZ = vmath::rotate(gfAngle, 0.0f, 0.0f, 1.0f);

		modelViewMatrix = translateMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ;
		glUniformMatrix4fv(uniform_modelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);

		projectionMatrix = perspectiveProjectionMatrix;
		glUniformMatrix4fv(uniform_perspectiveProjection, 1, GL_FALSE, projectionMatrix);

		//OpenGL Drawing

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, guiTexture_marble);
		glUniform1i(textureSamplerUniform, 0);

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
	glUseProgram(0);	//update

	if(bAnimate) {
		[self updateFunc];
	}

    // update function call should be here
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

}

-(void) updateFunc {
	gfAngle += 0.6f;
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
}

-(void)dealloc {
    
    if (vao_cube) {
		glDeleteVertexArrays(1, &vao_cube);
		vao_cube = 0;
	}
	
	if (vbo_position_cube) {
		glDeleteBuffers(1, &vbo_position_cube);
		vbo_position_cube = 0;
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

