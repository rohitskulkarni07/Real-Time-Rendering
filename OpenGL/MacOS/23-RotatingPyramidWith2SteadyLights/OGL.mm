//  23-RotatingPyramidWith2SteadyLights
//  OGL.m
//  
//
//  Created by Rohit Kulkarni on 09/01/22.
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

//	flags
bool 		gbPolygonMode	= true;
bool 		gbLights		= false;
bool 		gbAnimate		= false;

GLfloat gfAngle = 0.0f;

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


	// *** Shader Related Variables***/
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
		"#version 410 core																							\n" \
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
	"uniform vec3 u_la[2];																							\n" \
	"uniform vec3 u_ld[2];																							\n" \
	"uniform vec3 u_ls[2];																							\n" \
	"																												\n" \
	"uniform vec4 u_light_position[2];																				\n" \
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
	"		for(i = 0; i < 2; i++) {																				\n" \
	"																												\n" \
	"			light_direction		= normalize(vec3(u_light_position[i] - eye_coordinates));						\n" \
	"			reflection_vector 	= reflect(-light_direction,transformed_normal);									\n" \
	"			ambient 			= u_la[i] * u_ka;																\n" \
	"			diffuse 			= u_ld[i] * u_kd * max(dot(light_direction,transformed_normal),0.0);			\n" \
	"			specular			= u_ls[i] * u_ks * pow(max(dot(reflection_vector,view_vector),0.0),u_shininess);\n" \
	"			phong_ads_light		= phong_ads_light + ambient + diffuse + specular;								\n" \
	"		}																										\n" \
	"																												\n" \
	"	} else {																									\n" \
	"		phong_ads_light			= vec3(1.0,1.0,1.0);															\n" \
	"	}																											\n" \
	"																												\n" \
	"	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;								\n" \
	"}																												\n";


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
		"#version 410 core							\n" \
		"\n 										\n" \
		"in vec3	phong_ads_light;				\n" \
		"out vec4	FragColor;						\n" \
		"											\n" \
		"void main(void) {							\n" \
		"	FragColor = vec4(phong_ads_light,1.0);	\n" \
		"}											\n";
	

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
	// *** vertices, colors, shader attributes, vbo, vao_pyramid initialization ***
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
	//___________________________________________________________________________
	
	//Mandatory Function To Initialize Depth Buffer With All Ones
	glClearDepth(1.0f);

	//Mandatory Function To Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	//Mandatory Function To Checking Depth Bits With Reference Value
	glDepthFunc(GL_LEQUAL);

	gfAngle = 0.0f;
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
		modelMatrix 	 = vmath::translate(0.0f, 0.0f, -4.0f) * vmath::rotate(gfAngle, 0.0f,1.0f,0.0f);
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


	[self updateFunc];

    // update function call should be here
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

}

-(void) updateFunc {
	gfAngle += 0.7f;
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
}

-(void)dealloc {
    
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

