//  24 - ThreeAnimatingLightsOnSteadySphere
//  OGL.m
//  
//
//  Created by Rohit Kulkarni on 14/01/22.
//
#define GL_SILENCE_DEPRECATION
#include <Foundation/Foundation.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CVDisplayLink.h>
#include <OpenGL/gl3.h>
#include "vmath.h"
#include "sphere.h"


using namespace vmath;


CVReturn myDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);

FILE *gpFile = NULL;

//Sphere related variable
SphereData sphere_data; 

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

	bool gbPolygonMode;
	bool bLight;
	bool bAnimate;
	bool gbPV;
	bool gbPF;

	struct Light {
		vec3 light_ambient;
		vec3 light_diffuse;
		vec3 light_specular;
		vec4 light_position;
	};

	struct Light light[3];

	GLfloat light0_Angle;
	GLfloat light1_Angle;
	GLfloat light2_Angle;
	GLfloat r;

    GLuint vao_sphere;
    GLuint vbo_sphere_position;
	GLuint vbo_sphere_normals;
	GLuint vbo_sphere_textures;
	GLuint vbo_sphere_elements;

	GLfloat gfAngle;

	// Material Arrays
	GLfloat material_ambient[3];
	GLfloat material_diffuse[3];
	GLfloat material_specular[3];
	GLfloat material_shininess;


	//--------------PER VERTEX------------------------------
	GLuint		vertexShaderObject_PV;
	GLuint 		fragmentShaderObject_PV;
	GLuint		shaderProgramObject_PV;

	// variable for storing location of uniforms
	GLuint		modelMatrixUniform_PV;
	GLuint 		viewMatrixUniform_PV;
	GLuint 		projectionMatrixUniform_PV;
	GLuint		lKeyPressed_uniform_PV; //keypress

	// Lights Uniforms
	GLuint 		la_uniform_PV[3];	
	GLuint 		ld_uniform_PV[3];
	GLuint 		ls_uniform_PV[3];
	GLuint 		lightPosition_uniform_PV[3];	

	// Material Uniforms
	GLuint		ka_uniform_PV;
	GLuint		kd_uniform_PV;
	GLuint 		ks_uniform_PV;
	GLuint 		shininess_uniform_PV;

	//--------------PER FRAGMENT------------------------------
	GLuint 		vertexShaderObject_PF;
	GLuint 		fragmentShaderObject_PF;
	GLuint 		shaderProgramObject_PF;

	//to get uniform location from shader
	GLuint 		modelMatrixUniform_PF;
	GLuint 		viewMatrixUniform_PF;
	GLuint 		projectionMatrixUniform_PF;

	GLuint 		lKeyPressed_uniform_PF; // key press


	// Lights Uniforms
	GLuint 		la_uniform_PF[3];
	GLuint 		ld_uniform_PF[3];
	GLuint 		ls_uniform_PF[3];
	GLuint 		lightPosition_uniform_PF[3];
		

	// Material Uniforms
	GLuint 		ka_uniform_PF;
	GLuint 		kd_uniform_PF;
	GLuint 		ks_uniform_PF;
	GLuint 		shininess_uniform_PF;

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

	//*************************************************************************************
	//									GLSL
	//*************************************************************************************
	GLint infoLogLenght				= 0;
	GLint shaderCompilationStatus 	= 0;
	GLint shaderProgramLinkStatus	= 0;
	char* szInfoLog					= NULL;
		
	//*************************************************************************************
	//							 *** VERTEX SHADER FOR PER VERTEX ***
	//*************************************************************************************

	// create vertex shader
	vertexShaderObject_PV = glCreateShader(GL_VERTEX_SHADER);

	// write the shader source code
	const GLchar* vertexShaderSourceCode_PV = 
	"#version 410 core																								\n" \
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
	"uniform vec3 u_la[3];																							\n" \
	"uniform vec3 u_ld[3];																							\n" \
	"uniform vec3 u_ls[3];																							\n" \
	"																												\n" \
	"uniform vec4 u_light_position[3];																				\n" \
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
	"	phong_ads_light = vec3(0.0,0.0,0.0);																		\n" \
	"																												\n" \
	"	if(u_lkeypressed == 1) {																					\n" \
	"																												\n" \
	"		vec4 eye_coordinates 	= u_view_matrix * u_model_matrix * vPosition;									\n" \
	"		vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);					\n" \
	"		vec3 view_vector 		= normalize(-eye_coordinates.xyz);												\n" \
	"																												\n" \
	"		for(i = 0; i < 3; i++) {																				\n" \
	"																												\n" \
	"			light_direction		= normalize(vec3(u_light_position[i] - eye_coordinates));						\n" \
	"			reflection_vector 	= reflect(-light_direction,transformed_normal);									\n" \
	"			ambient 			= u_la[i] * u_ka;																\n" \
	"			diffuse 			= u_ld[i] * u_kd * max(dot(light_direction,transformed_normal),0.0);			\n" \
	"			specular			= u_ls[i] * u_ks * pow(max(dot(reflection_vector,view_vector),0.0),u_shininess);\n" \
	"			phong_ads_light		= phong_ads_light + ambient + diffuse + specular;								\n" \
	"																												\n" \
	"		}																										\n" \
	"																												\n" \
	"	} else {																									\n" \
	"		phong_ads_light			= vec3(1.0,1.0,1.0);															\n" \
	"	}																											\n" \
	"																												\n" \
	"	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;								\n" \
	"}																												\n";

	// provide source code to shader 
	glShaderSource(vertexShaderObject_PV, 1, (const GLchar**) &vertexShaderSourceCode_PV, NULL);

	// compile shader source code
	glCompileShader(vertexShaderObject_PV);

	// check compilation errors
	
	glGetShaderiv(vertexShaderObject_PV, GL_COMPILE_STATUS, &shaderCompilationStatus);
	if(shaderCompilationStatus == GL_FALSE) {

		glGetShaderiv(vertexShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLenght);
		if(infoLogLenght > 0) {

			szInfoLog = (char*) malloc(infoLogLenght);
			if(szInfoLog != NULL) {

				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject_PV, infoLogLenght, &written, szInfoLog);
				fprintf(gpFile, "\n*** Vertex Shader Compilation Status ***\n");
				fprintf(gpFile, "%s\n", szInfoLog);
				free(szInfoLog);
				 [self release];
            	[NSApp terminate: self];
			}
		}
	}

	//*************************************************************************************
	//							 *** VERTEX SHADER FOR PER FRAGMENT ***
	//*************************************************************************************

	// create vertex shader
	vertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);

	// write the shader source code
	const GLchar* vertexShaderSourceCode_PF = 
	"#version 410 core																								\n" \
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
	"uniform vec4 u_light_position[3];																				\n" \
	"																												\n" \
	"out vec3 transformed_normal;																					\n" \
	"out vec3 light_direction[3];																					\n" \
	"out vec3 view_vector;																							\n" \
	"																												\n" \
	"void main(void) {																								\n" \
	"																												\n" \
	"	int i = 0;																									\n" \
	"																												\n" \
	"	if(u_lkeypressed == 1) {																					\n" \
	"																												\n" \
	"		vec4 eye_coordinates 	= u_view_matrix * u_model_matrix * vPosition;									\n" \
	"		transformed_normal 		= mat3(u_view_matrix * u_model_matrix) * vNormal;								\n" \
	"		view_vector 			= (-eye_coordinates.xyz);														\n" \
	"																												\n" \
	"		for(i = 0; i < 3; i++){																					\n" \
	"																												\n" \
	"			light_direction[i]		= vec3(u_light_position[i] - eye_coordinates);								\n" \
	"		}																										\n" \
	"	}																											\n" \
	"																												\n" \
	"	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;								\n" \
	"}																												\n";

	// provide source code to shader 
	glShaderSource(vertexShaderObject_PF, 1, (const GLchar**) &vertexShaderSourceCode_PF, NULL);

	// compile shader source code
	glCompileShader(vertexShaderObject_PF);

	// check compilation errors
	
	glGetShaderiv(vertexShaderObject_PF, GL_COMPILE_STATUS, &shaderCompilationStatus);
	if(shaderCompilationStatus == GL_FALSE) {

		glGetShaderiv(vertexShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLenght);
		if(infoLogLenght > 0) {

			szInfoLog = (char*) malloc(infoLogLenght);
			if(szInfoLog != NULL) {

				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject_PF, infoLogLenght, &written, szInfoLog);
				fprintf(gpFile, "\n*** Vertex Shader Compilation Status ***\n");
				fprintf(gpFile, "%s\n", szInfoLog);
				free(szInfoLog);
				 [self release];
            	[NSApp terminate: self];
			}
		}
	}

	//*************************************************************************************
	// 								*** FRAGMENT SHADER FOR PER VERTX ***
	//*************************************************************************************

	// create fragment shader
	fragmentShaderObject_PV = glCreateShader(GL_FRAGMENT_SHADER);

	// write the shader source code
	const GLchar* fragmentShaderSourceCode_PV = 
	"#version 410 core																									\n" \
	"\n 																												\n" \
	"in vec3	phong_ads_light;																						\n" \
	"out vec4	FragColor;																								\n" \
	"																													\n" \
	"void main(void) {																									\n" \
	"	FragColor = vec4(phong_ads_light,1.0);																			\n" \
	"}																													\n";
	
	// provide source code to shader
	glShaderSource(fragmentShaderObject_PV, 1, (const GLchar**) &fragmentShaderSourceCode_PV, NULL);

	// compile shader source code
	glCompileShader(fragmentShaderObject_PV);

	// check compilation errors
	infoLogLenght				= 0;
	shaderCompilationStatus 	= 0;
	szInfoLog					= NULL;

	glGetShaderiv(fragmentShaderObject_PV, GL_COMPILE_STATUS, &shaderCompilationStatus);
	if(shaderCompilationStatus == GL_FALSE) {

		glGetShaderiv(fragmentShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLenght);
		if(infoLogLenght > 0) {

			szInfoLog = (char*) malloc(infoLogLenght);
			if(szInfoLog != NULL) {

				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject_PV, infoLogLenght, &written, szInfoLog);
				fprintf(gpFile, "\n*** Fragment Shader Compilation Status ***\n");
				fprintf(gpFile, "%s\n", szInfoLog);
				free(szInfoLog);
				 [self release];
            	[NSApp terminate: self];
			}
		}
	}


	//*************************************************************************************
	// 								*** FRAGMENT SHADER FOR PER FRAGMENT ***
	//*************************************************************************************

	// create fragment shader
	fragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

	// write the shader source code
	const GLchar* fragmentShaderSourceCode_PF = 
	"#version 410 core																													\n" \
	"\n 																																\n" \
	"vec3 phong_ads_light;																												\n" \
	"																																	\n" \
	"in vec3 transformed_normal;																										\n" \
	"in vec3 light_direction[3];																										\n" \
	"in vec3 view_vector;																												\n" \
	"																																	\n" \
	"uniform int u_lkeypressed;																											\n" \
	"																																	\n" \
	"uniform vec3 u_la[3];																												\n" \
	"uniform vec3 u_ld[3];																												\n" \
	"uniform vec3 u_ls[3];																												\n" \
	"																																	\n" \
	"uniform vec3 u_ka;																													\n" \
	"uniform vec3 u_kd;																													\n" \
	"uniform vec3 u_ks;																													\n" \
	"																																	\n" \
	"uniform float u_shininess;																											\n" \
	"																																	\n" \
	"out vec4 FragColor;																												\n" \
	"																																	\n" \
	"void main(void) {																													\n" \
	"																																	\n" \
	"	int i = 0;																														\n" \
	"	phong_ads_light = vec3(0.0, 0.0, 0.0);																							\n" \
	"																																	\n" \
	"	if( u_lkeypressed == 1) {																										\n" \
	"																																	\n" \
	"		vec3 normalized_transformed_normal 	= normalize(transformed_normal);														\n" \
	"		vec3 normalized_view_vector 		= normalize(view_vector);																\n" \
	"																																	\n" \
	"		for(i = 0; i < 3; i++) {																									\n" \
	"																																	\n" \
	"			vec3 normalized_light_direction 	= normalize(light_direction[i]);													\n" \
	"			vec3 reflection_vector 	= reflect(-normalized_light_direction,normalized_transformed_normal);							\n" \
	"			vec3 ambient 			= u_la[i] * u_ka;																				\n" \
	"			vec3 diffuse 			= u_ld[i] * u_kd * max(dot(normalized_light_direction,normalized_transformed_normal),0.0);		\n" \
	"			vec3 specular			= u_ls[i] * u_ks * pow(max(dot(reflection_vector,normalized_view_vector),0.0),u_shininess);		\n" \
	"			phong_ads_light			= phong_ads_light + ambient + diffuse + specular;												\n" \
	"		}																															\n" \
	"																																	\n" \
	"	} else {																														\n" \
	"		phong_ads_light			= vec3(1.0,1.0,1.0);																				\n" \
	"	}																																\n" \
	"	FragColor = vec4(phong_ads_light,1.0);																							\n" \
	"}																																	\n";
	
	// provide source code to shader
	glShaderSource(fragmentShaderObject_PF, 1, (const GLchar**) &fragmentShaderSourceCode_PF, NULL);

	// compile shader source code
	glCompileShader(fragmentShaderObject_PF);

	// check compilation errors
	infoLogLenght				= 0;
	shaderCompilationStatus 	= 0;
	szInfoLog					= NULL;

	glGetShaderiv(fragmentShaderObject_PF, GL_COMPILE_STATUS, &shaderCompilationStatus);
	if(shaderCompilationStatus == GL_FALSE) {

		glGetShaderiv(fragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLenght);
		if(infoLogLenght > 0) {

			szInfoLog = (char*) malloc(infoLogLenght);
			if(szInfoLog != NULL) {

				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject_PF, infoLogLenght, &written, szInfoLog);
				fprintf(gpFile,"\n*** Fragment Shader Compilation Status ***\n");
				fprintf(gpFile,"%s\n", szInfoLog);
				free(szInfoLog);
				[self release];
            	[NSApp terminate: self];
			}
		}
	}

//*************************************************************************************
	//							*** SHADER PROGRAM FOR PER VERTEX***
	//*************************************************************************************

	// create shader program object
	shaderProgramObject_PV = glCreateProgram();

	// attach vertex shader object to shader program object
	glAttachShader(shaderProgramObject_PV, vertexShaderObject_PV);

	// attach fragment shader object to shader program object
	glAttachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

	// before linking, bind shader program with attributes
	glBindAttribLocation(shaderProgramObject_PV, RSK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(shaderProgramObject_PV, RSK_ATTRIBUTE_NORMAL, "vNormal");

	// link shader program
	glLinkProgram(shaderProgramObject_PV);

	// check linking errors
	infoLogLenght				= 0;
	shaderCompilationStatus 	= 0;
	shaderProgramLinkStatus		= 0;
	szInfoLog					= NULL;

	glGetProgramiv(shaderProgramObject_PV, GL_LINK_STATUS, &shaderProgramLinkStatus);
	if(shaderProgramLinkStatus == GL_FALSE) {

		glGetProgramiv(shaderProgramObject_PV, GL_INFO_LOG_LENGTH, &infoLogLenght);
		if(infoLogLenght > 0) {

			szInfoLog = (char*) malloc(infoLogLenght);
			if(szInfoLog != NULL) {
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject_PV, infoLogLenght,&written, szInfoLog);
				fprintf(gpFile, "\n*** Shader Program Link Log ***\n");
				fprintf(gpFile, "%s\n", szInfoLog);
				[self release];
            	[NSApp terminate: self];

			}
		}
	}
	//*************************************************************************************
	//							*** SHADER PROGRAM FOR PER FRAGMENT***
	//*************************************************************************************

	// create shader program object
	shaderProgramObject_PF = glCreateProgram();

	// attach vertex shader object to shader program object
	glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);

	// attach fragment shader object to shader program object
	glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

	// before linking, bind shader program with attributes
	glBindAttribLocation(shaderProgramObject_PF, RSK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(shaderProgramObject_PF, RSK_ATTRIBUTE_NORMAL, "vNormal");

	// link shader program
	glLinkProgram(shaderProgramObject_PF);

	// check linking errors
	infoLogLenght				= 0;
	shaderCompilationStatus 	= 0;
	shaderProgramLinkStatus		= 0;
	szInfoLog					= NULL;

	glGetProgramiv(shaderProgramObject_PF, GL_LINK_STATUS, &shaderProgramLinkStatus);
	if(shaderProgramLinkStatus == GL_FALSE) {

		glGetProgramiv(shaderProgramObject_PF, GL_INFO_LOG_LENGTH, &infoLogLenght);
		if(infoLogLenght > 0) {

			szInfoLog = (char*) malloc(infoLogLenght);
			if(szInfoLog != NULL) {
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject_PF, infoLogLenght,&written, szInfoLog);
				fprintf(gpFile, "\n*** Shader Program Link Log ***\n");
				fprintf(gpFile, "%s\n", szInfoLog);
				[self release];
            	[NSApp terminate: self];
			}
		}
	}
		//	get uniform locations from shader program FOR PER VERTEX
	modelMatrixUniform_PV 		= glGetUniformLocation(shaderProgramObject_PV, "u_model_matrix");
	viewMatrixUniform_PV 		= glGetUniformLocation(shaderProgramObject_PV, "u_view_matrix");
	projectionMatrixUniform_PV 	= glGetUniformLocation(shaderProgramObject_PV, "u_projection_matrix");
	lKeyPressed_uniform_PV 		= glGetUniformLocation(shaderProgramObject_PV, "u_lkeypressed");

	la_uniform_PV[0] 				= glGetUniformLocation(shaderProgramObject_PV, "u_la[0]"); // light ambient component
	ld_uniform_PV[0] 				= glGetUniformLocation(shaderProgramObject_PV, "u_ld[0]"); // light diffuse component
	ls_uniform_PV[0] 				= glGetUniformLocation(shaderProgramObject_PV, "u_ls[0]"); // light specular component
	lightPosition_uniform_PV[0] 	= glGetUniformLocation(shaderProgramObject_PV, "u_light_position[0]"); // light position

	la_uniform_PV[1] 				= glGetUniformLocation(shaderProgramObject_PV, "u_la[1]"); // light ambient component
	ld_uniform_PV[1] 				= glGetUniformLocation(shaderProgramObject_PV, "u_ld[1]"); // light diffuse component
	ls_uniform_PV[1] 				= glGetUniformLocation(shaderProgramObject_PV, "u_ls[1]"); // light specular component
	lightPosition_uniform_PV[1] 	= glGetUniformLocation(shaderProgramObject_PV, "u_light_position[1]"); // light position

	la_uniform_PV[2] 				= glGetUniformLocation(shaderProgramObject_PV, "u_la[2]"); // light ambient component
	ld_uniform_PV[2] 				= glGetUniformLocation(shaderProgramObject_PV, "u_ld[2]"); // light diffuse component
	ls_uniform_PV[2] 				= glGetUniformLocation(shaderProgramObject_PV, "u_ls[2]"); // light specular component
	lightPosition_uniform_PV[2] 	= glGetUniformLocation(shaderProgramObject_PV, "u_light_position[2]"); // light position

	ka_uniform_PV 				= glGetUniformLocation(shaderProgramObject_PV, "u_ka"); // material ambient component
	kd_uniform_PV 				= glGetUniformLocation(shaderProgramObject_PV, "u_kd"); // material diffuse component
	ks_uniform_PV 				= glGetUniformLocation(shaderProgramObject_PV, "u_ks"); // material specular component
	shininess_uniform_PV 		= glGetUniformLocation(shaderProgramObject_PV, "u_shininess"); // material shininess Component

	//	get uniform locations from shader program FOR PER FRAGMENT
	modelMatrixUniform_PF 		= glGetUniformLocation(shaderProgramObject_PF, "u_model_matrix");
	viewMatrixUniform_PF 		= glGetUniformLocation(shaderProgramObject_PF, "u_view_matrix");
	projectionMatrixUniform_PF 	= glGetUniformLocation(shaderProgramObject_PF, "u_projection_matrix");
	lKeyPressed_uniform_PF 		= glGetUniformLocation(shaderProgramObject_PF, "u_lkeypressed");

	la_uniform_PF[0] 				= glGetUniformLocation(shaderProgramObject_PF, "u_la[0]"); // light ambient component
	ld_uniform_PF[0] 				= glGetUniformLocation(shaderProgramObject_PF, "u_ld[0]"); // light diffuse component
	ls_uniform_PF[0] 				= glGetUniformLocation(shaderProgramObject_PF, "u_ls[0]"); // light specular component
	lightPosition_uniform_PF[0] 	= glGetUniformLocation(shaderProgramObject_PF, "u_light_position[0]"); // light position

	la_uniform_PF[1] 				= glGetUniformLocation(shaderProgramObject_PF, "u_la[1]"); // light ambient component
	ld_uniform_PF[1] 				= glGetUniformLocation(shaderProgramObject_PF, "u_ld[1]"); // light diffuse component
	ls_uniform_PF[1] 				= glGetUniformLocation(shaderProgramObject_PF, "u_ls[1]"); // light specular component
	lightPosition_uniform_PF[1] 	= glGetUniformLocation(shaderProgramObject_PF, "u_light_position[1]"); // light position

	la_uniform_PF[2] 				= glGetUniformLocation(shaderProgramObject_PF, "u_la[2]"); // light ambient component
	ld_uniform_PF[2] 				= glGetUniformLocation(shaderProgramObject_PF, "u_ld[2]"); // light diffuse component
	ls_uniform_PF[2] 				= glGetUniformLocation(shaderProgramObject_PF, "u_ls[2]"); // light specular component
	lightPosition_uniform_PF[2] 	= glGetUniformLocation(shaderProgramObject_PF, "u_light_position[2]"); // light position

	ka_uniform_PF 				= glGetUniformLocation(shaderProgramObject_PF, "u_ka"); // material ambient component
	kd_uniform_PF 				= glGetUniformLocation(shaderProgramObject_PF, "u_kd"); // material diffuse component
	ks_uniform_PF 				= glGetUniformLocation(shaderProgramObject_PF, "u_ks"); // material specular component
	shininess_uniform_PF 		= glGetUniformLocation(shaderProgramObject_PF, "u_shininess"); // material shininess Component

	Sphere(1.0, 60, 60, &sphere_data);

	//__________________________________________________________________________________________

	//For Geometry
	glGenVertexArrays(1, &vao_sphere);
	glBindVertexArray(vao_sphere);

		//for Geometry Position Properties
		glGenBuffers(1, &vbo_sphere_position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_position);
		glBufferData(GL_ARRAY_BUFFER, sphere_data.numberOfVertices * 3 * sizeof(GLfloat), sphere_data.sphere_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//for Geometry Normals Properties
		glGenBuffers(1, &vbo_sphere_normals);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_normals);
		glBufferData(GL_ARRAY_BUFFER, sphere_data.numberOfVertices * 3 * sizeof(GLfloat), sphere_data.sphere_normals, GL_STATIC_DRAW);
		glVertexAttribPointer(RSK_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(RSK_ATTRIBUTE_NORMAL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// //for Geometry Texture Properties
		// glGenBuffers(1, &vbo_sphere_textures);
		// glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_textures);
		// glBufferData(GL_ARRAY_BUFFER, sphere_data.numberOfVertices * 2 * sizeof(GLfloat), sphere_data.sphere_texcoords, GL_STATIC_DRAW);
		// glVertexAttribPointer(RSK_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		// glEnableVertexAttribArray(RSK_ATTRIBUTE_TEXTURE0);
		// glBindBuffer(GL_ARRAY_BUFFER, 0);

		//for Geometry Elements Properties
		glGenBuffers(1, &vbo_sphere_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere_data.numberOfIndices * sizeof(GLuint), sphere_data.sphere_indices, GL_STATIC_DRAW);
		//no neeed of VertexAttribPointer
		//no need to EnableVertexAttribArray
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glBindVertexArray(0);
	//___________________________________________________________________________
	
	//glEnable(GL_TEXTURE_2D);
	
	//Mandatory Function To Initialize Depth Buffer With All Ones
	glClearDepth(1.0f);

	//Mandatory Function To Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	//Mandatory Function To Checking Depth Bits With Reference Value
	glDepthFunc(GL_LEQUAL);

	gbPolygonMode = true;
	bLight = false;
	bAnimate = false;
	gbPV = true;
	gbPF = false;
	gfAngle = 0.0f;
	
	light0_Angle = 0.0f;
	light1_Angle = 0.0f;
	light2_Angle = 0.0f;
	r = 5.0f;

	
	light[0].light_ambient	= vec3(0.0f, 0.0f, 0.0f);
	light[0].light_diffuse	= vec3(1.0f, 0.0f, 0.0f);
	light[0].light_specular	= vec3(1.0f, 0.0f, 0.0f);
	light[0].light_position	= vec4(0.0f, 0.0f, 0.0f, 1.0f);

	light[1].light_ambient	= vec3(0.0f, 0.0f, 0.0f);
	light[1].light_diffuse	= vec3(0.0f, 1.0f, 0.0f);
	light[1].light_specular	= vec3(0.0f, 1.0f, 0.0f);
	light[1].light_position	= vec4(0.0f, 0.0f, 0.0f, 1.0f);

	light[2].light_ambient	= vec3(0.0f, 0.0f, 0.0f);
	light[2].light_diffuse	= vec3(0.0f, 0.0f, 1.0f);
	light[2].light_specular	= vec3(0.0f, 0.0f, 1.0f);
	light[2].light_position	= vec4(0.0f, 0.0f, .0f, 1.0f);

	// Material Arrays
	material_ambient[0] = 0.1f;
	material_ambient[1] = 0.1f;
	material_ambient[2] = 0.1f;
	
	material_diffuse[0] = 0.5f;
	material_diffuse[1] = 0.2f;
	material_diffuse[2] = 0.7f;
	
	material_specular[0] = 0.7f;
	material_specular[1] = 0.7f;
	material_specular[2] = 0.7f;
	
	material_shininess = 128.0f;

    glClearColor(0.1f, 0.1f, 0.1f, 0.15f);

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

	// Matrices
	modelMatrix 	 = vmath::translate(0.0f, 0.0f, -4.5f);
	viewMatrix		 = mat4::identity();
	projectionMatrix = perspectiveProjectionMatrix;		

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

	// start using shader program

	if(bLight) {
			
		light[0].light_position[0] = 0.0f;
		light[0].light_position[1] = r * sin(light0_Angle);
		light[0].light_position[2] = r * cos(light0_Angle);
		light[0].light_position[3] = 1.0f;

		light[1].light_position[0] = r * sin(light1_Angle);
		light[1].light_position[1] = 0.0f;
		light[1].light_position[2] = r * cos(light1_Angle);
		light[1].light_position[3] = 1.0f;

		light[2].light_position[0] = r * sin(light2_Angle);
		light[2].light_position[1] = r * cos(light2_Angle);
		light[2].light_position[2] = 0.0f;
		light[2].light_position[3] = 1.0f;
		
		if (gbPV) {

			glUniform1i(lKeyPressed_uniform_PV, 1);

			glUniform3fv(la_uniform_PV[0], 1, light[0].light_ambient);  // light ambient
			glUniform3fv(ld_uniform_PV[0], 1, light[0].light_diffuse);  // light diffuse
			glUniform3fv(ls_uniform_PV[0], 1, light[0].light_specular); // light specular
			glUniform4fv(lightPosition_uniform_PV[0], 1, light[0].light_position); // light position
	
			glUniform3fv(la_uniform_PV[1], 1, light[1].light_ambient);  // light ambient
			glUniform3fv(ld_uniform_PV[1], 1, light[1].light_diffuse);  // light diffuse
			glUniform3fv(ls_uniform_PV[1], 1, light[1].light_specular); // light specular
			glUniform4fv(lightPosition_uniform_PV[1], 1, light[1].light_position); // light position
	
			glUniform3fv(la_uniform_PV[2], 1, light[2].light_ambient);  // light ambient
			glUniform3fv(ld_uniform_PV[2], 1, light[2].light_diffuse);  // light diffuse
			glUniform3fv(ls_uniform_PV[2], 1, light[2].light_specular); // light specular
			glUniform4fv(lightPosition_uniform_PV[2], 1, light[2].light_position); // light position
	
			glUniform3fv(ka_uniform_PV, 1, (GLfloat*)material_ambient);  // material ambient
			glUniform3fv(kd_uniform_PV, 1, (GLfloat*)material_diffuse);  // material diffuse
			glUniform3fv(ks_uniform_PV, 1, (GLfloat*)material_specular); // material specular
			glUniform1f(shininess_uniform_PV, (GLfloat)material_shininess); // material shininess

			//Sending Unifomrs to Shader
			glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, projectionMatrix);

		} else {
			
			glUniform1i(lKeyPressed_uniform_PF, 1);
			
			glUniform3fv(la_uniform_PF[0], 1, light[0].light_ambient);  // light ambient
			glUniform3fv(ld_uniform_PF[0], 1, light[0].light_diffuse);  // light diffuse
			glUniform3fv(ls_uniform_PF[0], 1, light[0].light_specular); // light specular
			glUniform4fv(lightPosition_uniform_PF[0], 1, light[0].light_position); // light position
			
			glUniform3fv(la_uniform_PF[1], 1, light[1].light_ambient);  // light ambient
			glUniform3fv(ld_uniform_PF[1], 1, light[1].light_diffuse);  // light diffuse
			glUniform3fv(ls_uniform_PF[1], 1, light[1].light_specular); // light specular
			glUniform4fv(lightPosition_uniform_PF[1], 1, light[1].light_position); // light position
			
			glUniform3fv(la_uniform_PF[2], 1, light[2].light_ambient);  // light ambient
			glUniform3fv(ld_uniform_PF[2], 1, light[2].light_diffuse);  // light diffuse
			glUniform3fv(ls_uniform_PF[2], 1, light[2].light_specular); // light specular
			glUniform4fv(lightPosition_uniform_PF[2], 1, light[2].light_position); // light position
			
			glUniform3fv(ka_uniform_PF, 1, (GLfloat*)material_ambient);  // material ambient
			glUniform3fv(kd_uniform_PF, 1, (GLfloat*)material_diffuse);  // material diffuse
			glUniform3fv(ks_uniform_PF, 1, (GLfloat*)material_specular); // material specular
			glUniform1f(shininess_uniform_PF, (GLfloat)material_shininess); // material shininess

			//Sending Uniforms to Shader
			glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, projectionMatrix);
		}
	} else {
		if (gbPV) {
			glUseProgram(shaderProgramObject_PV);
			glUniform1i(lKeyPressed_uniform_PV, 0);
		}
		else {
			glUseProgram(shaderProgramObject_PF);
			glUniform1i(lKeyPressed_uniform_PF, 0);
		}
	}
		
	glBindVertexArray(vao_sphere);
	
	/*** Bind Array Elements ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
	
	// *** draw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawElements(GL_TRIANGLES, sphere_data.numberOfIndices, GL_UNSIGNED_INT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	//stop using OpenGL program Object
	glUseProgram(0);

	if(bAnimate) {
		[self updateFunc];
	}

    // update function call should be here
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

}

-(void) updateFunc {
	light0_Angle += 0.01f;
	if (light0_Angle >= 360.0f) {
		light0_Angle = 0.0f;
	}

	light1_Angle += 0.01f;
	if (light1_Angle >= 360.0f) {
		light1_Angle = 0.0f;
	}
	light2_Angle += 0.01f;
	
	if (light2_Angle >= 360.0f) {
		light2_Angle = 0.0f;
	}
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
			case 'p':
			case 'P':
				gbPF = true;
				gbPV = false;
				break;

			//per vertex toggle
			case 'V':
			case 'v':
				gbPV = true;
				gbPF = false;
				break;

        default:
            break;
    }
}

-(void)dealloc {
    
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

	if (sphere_data.sphere_indices) {
		free(sphere_data.sphere_indices);
		sphere_data.sphere_indices = NULL;
	}

	if (sphere_data.sphere_texcoords) {
		free(sphere_data.sphere_texcoords);
		sphere_data.sphere_texcoords = NULL;
	}
	if (sphere_data.sphere_normals) {
		free(sphere_data.sphere_normals);
		sphere_data.sphere_normals = NULL;
	}
	if (sphere_data.sphere_vertices) {
		free(sphere_data.sphere_vertices);
		sphere_data.sphere_vertices = NULL;
	}
	if(shaderProgramObject_PV) {

		glUseProgram(shaderProgramObject_PV);

			GLsizei shaderCount;
			glGetProgramiv(shaderProgramObject_PV, GL_ATTACHED_SHADERS, &shaderCount);
			
			GLuint* p_shader = NULL;
			p_shader = (GLuint*) malloc(sizeof(GLint) * shaderCount);
			
			glGetAttachedShaders(shaderProgramObject_PV, shaderCount, &shaderCount, p_shader);

			for(GLsizei i = 0; i < shaderCount; i++) {

				glDetachShader(shaderProgramObject_PV, p_shader[i]);
				glDeleteShader(p_shader[i]);
				p_shader[i] = 0;
			}

			free(p_shader);
			glDeleteProgram(shaderProgramObject_PV);
			shaderProgramObject_PV = 0;

		glUseProgram(0);

	}
	if(shaderProgramObject_PF) {

		glUseProgram(shaderProgramObject_PF);

			GLsizei shaderCount;
			glGetProgramiv(shaderProgramObject_PF, GL_ATTACHED_SHADERS, &shaderCount);
			
			GLuint* p_shader = NULL;
			p_shader = (GLuint*) malloc(sizeof(GLint) * shaderCount);
			
			glGetAttachedShaders(shaderProgramObject_PF, shaderCount, &shaderCount, p_shader);

			for(GLsizei i = 0; i < shaderCount; i++) {

				glDetachShader(shaderProgramObject_PF, p_shader[i]);
				glDeleteShader(p_shader[i]);
				p_shader[i] = 0;
			}

			free(p_shader);
			glDeleteProgram(shaderProgramObject_PF);
			shaderProgramObject_PF = 0;

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

