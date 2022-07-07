//
//  GLESView.m
//  window
//
//  Created by Rohit Kulkarni on 15/02/22.
//

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#import "GLESView.h"
#import "vmath.h"
#import "sphere.h"

using namespace vmath;

// OpenGL & GL-SL Related Declarations
enum {
    RSK_ATTRIBUTE_POSITION = 0,
    RSK_ATTRIBUTE_COLOR,
    RSK_ATTRIBUTE_NORMAL,
    RSK_ATTRIBUTE_TEXTURE0,
};


struct Light {
    vec3 light_ambient;
    vec3 light_diffuse;
    vec3 light_specular;
    vec4 light_position;
};

struct Light light[3];

//Sphere related variable
SphereData sphere_data; 

@implementation GLESView {
@private
    EAGLContext *eaglContext;
    
    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    
    GLuint vertexShaderObject;
    GLuint fragmentShaderObject;
    GLuint shaderProgramObject;
    
    GLuint vao_sphere;
    
    GLuint vbo_sphere_position;
    GLuint vbo_sphere_normals;
    GLuint vbo_sphere_textures;
    GLuint vbo_sphere_elements;
    
    GLuint mvpMatrixUniform;
    
    GLfloat gfAngle;
    
    GLfloat light0_Angle;
    GLfloat light1_Angle;
    GLfloat light2_Angle;
    GLfloat r;
    
    // Material Arrays
    GLfloat material_ambient[3];
    GLfloat material_diffuse[3];
    GLfloat material_specular[3];
    GLfloat material_shininess;
    
    bool bLight;
    bool bAnimate;
    bool gbPV;
    bool gbPF;
    
    //--------------PER VERTEX------------------------------
    GLuint        vertexShaderObject_PV;
    GLuint         fragmentShaderObject_PV;
    GLuint        shaderProgramObject_PV;
    
    // variable for storing location of uniforms
    GLuint        modelMatrixUniform_PV;
    GLuint         viewMatrixUniform_PV;
    GLuint         projectionMatrixUniform_PV;
    GLuint        lKeyPressed_uniform_PV; //keypress
    
    // Lights Uniforms
    GLuint         la_uniform_PV[3];
    GLuint         ld_uniform_PV[3];
    GLuint         ls_uniform_PV[3];
    GLuint         lightPosition_uniform_PV[3];
    
    // Material Uniforms
    GLuint         ka_uniform_PV;
    GLuint         kd_uniform_PV;
    GLuint         ks_uniform_PV;
    GLuint         shininess_uniform_PV;
    
    //--------------PER FRAGMENT------------------------------
    GLuint         vertexShaderObject_PF;
    GLuint         fragmentShaderObject_PF;
    GLuint         shaderProgramObject_PF;
    
    //to get uniform location from shader
    GLuint         modelMatrixUniform_PF;
    GLuint         viewMatrixUniform_PF;
    GLuint         projectionMatrixUniform_PF;
    
    GLuint         lKeyPressed_uniform_PF; // key press
    
    
    // Lights Uniforms
    GLuint         la_uniform_PF[3];
    GLuint         ld_uniform_PF[3];
    GLuint         ls_uniform_PF[3];
    GLuint         lightPosition_uniform_PF[3];
    
    
    // Material Uniforms
    GLuint         ka_uniform_PF;
    GLuint         kd_uniform_PF;
    GLuint         ks_uniform_PF;
    GLuint         shininess_uniform_PF;
    
    mat4 perspectiveProjectionMatrix; // from vmath 4x4 Matrix
    
    id displayLink;
    NSInteger animationFrameInterval;
    BOOL isAnimating;
    
}

- (id)initWithFrame:(CGRect)frame {
    
    // code
    self = [super initWithFrame:frame];
    
    if(self) {
        
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *) [super layer];
        [eaglLayer setOpaque: YES];
        [eaglLayer setDrawableProperties:[NSDictionary dictionaryWithObjectsAndKeys:
                                          [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                          kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                          nil]];
        
        eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if(eaglContext == nil) {
            
            printf("OpenGLES context creation failed!\n");
            return (nil);
        }
        [EAGLContext setCurrentContext:eaglContext];
        
        // Framebuffer
        glGenFramebuffers(1, &defaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        // Color render buffer
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        
        // Give storage to render buffer
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        GLint backingwidth;
        GLint backingHeight;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingwidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
        
        // Depth Buffer
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        // Give storage to render buffer
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingwidth, backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER != GL_FRAMEBUFFER_COMPLETE)) {
            printf("Framebuffer is not complete!\n");
            [self uninitialize];
            return (nil);
        }
        
        animationFrameInterval = 60;
        isAnimating = NO;
        //_________________________________Initalize-OpenGL_____________________________________________________________
        
        //*************************************************************************************
        //                                    GLSL
        //*************************************************************************************
        GLint infoLogLenght                = 0;
        GLint shaderCompilationStatus     = 0;
        GLint shaderProgramLinkStatus    = 0;
        char* szInfoLog                    = NULL;
        
        //*************************************************************************************
        //                             *** VERTEX SHADER FOR PER VERTEX ***
        //*************************************************************************************
        
        // create vertex shader
        vertexShaderObject_PV = glCreateShader(GL_VERTEX_SHADER);
        
        // write the shader source code
        const GLchar* vertexShaderSourceCode_PV =
        "#version 300 es                                                                                                \n" \
        "\n                                                                                                             \n" \
        "                                                                                                                 \n" \
        "precision highp float;                                                                                                           \n" \
        "precision highp int;                                                                                                             \n" \
        "                                                                                                                 \n" \
        "in vec4 vPosition;                                                                                                \n" \
        "in vec3 vNormal;                                                                                                \n" \
        "                                                                                                                \n" \
        "uniform mat4 u_model_matrix;                                                                                    \n" \
        "uniform mat4 u_view_matrix;                                                                                    \n" \
        "uniform mat4 u_projection_matrix;                                                                                \n" \
        "                                                                                                                \n" \
        "uniform int u_lkeypressed;                                                                                        \n" \
        "                                                                                                                \n" \
        "uniform vec3 u_la[3];                                                                                            \n" \
        "uniform vec3 u_ld[3];                                                                                            \n" \
        "uniform vec3 u_ls[3];                                                                                            \n" \
        "                                                                                                                \n" \
        "uniform vec4 u_light_position[3];                                                                                \n" \
        "                                                                                                                \n" \
        "uniform vec3 u_ka;                                                                                                \n" \
        "uniform vec3 u_kd;                                                                                                \n" \
        "uniform vec3 u_ks;                                                                                                \n" \
        "                                                                                                                \n" \
        "uniform float u_shininess;                                                                                        \n" \
        "                                                                                                                \n" \
        "vec3 ambient;                                                                                                    \n" \
        "vec3 diffuse;                                                                                                    \n" \
        "vec3 specular;                                                                                                    \n" \
        "vec3 light_direction;                                                                                            \n" \
        "vec3 reflection_vector;                                                                                        \n" \
        "                                                                                                                \n" \
        "out vec3 phong_ads_light;                                                                                        \n" \
        "                                                                                                                \n" \
        "void main(void) {                                                                                                \n" \
        "                                                                                                                \n" \
        "    int i = 0;                                                                                                    \n" \
        "    phong_ads_light = vec3(0.0,0.0,0.0);                                                                        \n" \
        "                                                                                                                \n" \
        "    if(u_lkeypressed == 1) {                                                                                    \n" \
        "                                                                                                                \n" \
        "        vec4 eye_coordinates     = u_view_matrix * u_model_matrix * vPosition;                                    \n" \
        "        vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);                    \n" \
        "        vec3 view_vector         = normalize(-eye_coordinates.xyz);                                                \n" \
        "                                                                                                                \n" \
        "        for(i = 0; i < 3; i++) {                                                                                \n" \
        "                                                                                                                \n" \
        "            light_direction        = normalize(vec3(u_light_position[i] - eye_coordinates));                        \n" \
        "            reflection_vector     = reflect(-light_direction,transformed_normal);                                    \n" \
        "            ambient             = u_la[i] * u_ka;                                                                \n" \
        "            diffuse             = u_ld[i] * u_kd * max(dot(light_direction,transformed_normal),0.0);            \n" \
        "            specular            = u_ls[i] * u_ks * pow(max(dot(reflection_vector,view_vector),0.0),u_shininess);\n" \
        "            phong_ads_light        = phong_ads_light + ambient + diffuse + specular;                                \n" \
        "                                                                                                                \n" \
        "        }                                                                                                        \n" \
        "                                                                                                                \n" \
        "    } else {                                                                                                    \n" \
        "        phong_ads_light            = vec3(1.0,1.0,1.0);                                                            \n" \
        "    }                                                                                                            \n" \
        "                                                                                                                \n" \
        "    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;                                \n" \
        "}                                                                                                                \n";
        
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
                    printf("\n*** Vertex Shader Compilation Status ***\n");
                    printf("%s\n", szInfoLog);
                    free(szInfoLog);
                    [self release];
                    exit(0);
                }
            }
        }
        
        //*************************************************************************************
        //                             *** VERTEX SHADER FOR PER FRAGMENT ***
        //*************************************************************************************
        
        // create vertex shader
        vertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);
        
        // write the shader source code
        const GLchar* vertexShaderSourceCode_PF =
        "#version 300 es                                                                                                \n" \
        "\n                                                                                                             \n" \
        "                                                                                                                 \n" \
        "precision highp float;                                                                                                           \n" \
        "precision highp int;                                                                                                             \n" \
        "                                                                                                                 \n" \
        "in vec4 vPosition;                                                                                                \n" \
        "in vec3 vNormal;                                                                                                \n" \
        "                                                                                                                \n" \
        "uniform mat4 u_model_matrix;                                                                                    \n" \
        "uniform mat4 u_view_matrix;                                                                                    \n" \
        "uniform mat4 u_projection_matrix;                                                                                \n" \
        "                                                                                                                \n" \
        "uniform int u_lkeypressed;                                                                                        \n" \
        "                                                                                                                \n" \
        "uniform vec4 u_light_position[3];                                                                                \n" \
        "                                                                                                                \n" \
        "out vec3 transformed_normal;                                                                                    \n" \
        "out vec3 light_direction[3];                                                                                    \n" \
        "out vec3 view_vector;                                                                                            \n" \
        "                                                                                                                \n" \
        "void main(void) {                                                                                                \n" \
        "                                                                                                                \n" \
        "    int i = 0;                                                                                                    \n" \
        "                                                                                                                \n" \
        "    if(u_lkeypressed == 1) {                                                                                    \n" \
        "                                                                                                                \n" \
        "        vec4 eye_coordinates     = u_view_matrix * u_model_matrix * vPosition;                                    \n" \
        "        transformed_normal         = mat3(u_view_matrix * u_model_matrix) * vNormal;                                \n" \
        "        view_vector             = (-eye_coordinates.xyz);                                                        \n" \
        "                                                                                                                \n" \
        "        for(i = 0; i < 3; i++){                                                                                    \n" \
        "                                                                                                                \n" \
        "            light_direction[i]        = vec3(u_light_position[i] - eye_coordinates);                                \n" \
        "        }                                                                                                        \n" \
        "    }                                                                                                            \n" \
        "                                                                                                                \n" \
        "    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;                                \n" \
        "}                                                                                                                \n";
        
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
                    printf("\n*** Vertex Shader Compilation Status ***\n");
                    printf("%s\n", szInfoLog);
                    free(szInfoLog);
                    [self release];
                    exit(0);
                }
            }
        }
        
        //*************************************************************************************
        //                                 *** FRAGMENT SHADER FOR PER VERTX ***
        //*************************************************************************************
        
        // create fragment shader
        fragmentShaderObject_PV = glCreateShader(GL_FRAGMENT_SHADER);
        
        // write the shader source code
        const GLchar* fragmentShaderSourceCode_PV =
        "#version 300 es                                                                                                    \n" \
        "                                                                                                                 \n" \
        "precision highp float;                                                                                                           \n" \
        "precision highp int;                                                                                                             \n" \
        "                                                                                                                 \n" \
        "in vec3    phong_ads_light;                                                                                        \n" \
        "out vec4    FragColor;                                                                                                \n" \
        "                                                                                                                    \n" \
        "void main(void) {                                                                                                    \n" \
        "    FragColor = vec4(phong_ads_light,1.0);                                                                            \n" \
        "}                                                                                                                    \n";
        
        // provide source code to shader
        glShaderSource(fragmentShaderObject_PV, 1, (const GLchar**) &fragmentShaderSourceCode_PV, NULL);
        
        // compile shader source code
        glCompileShader(fragmentShaderObject_PV);
        
        // check compilation errors
        infoLogLenght                = 0;
        shaderCompilationStatus     = 0;
        szInfoLog                    = NULL;
        
        glGetShaderiv(fragmentShaderObject_PV, GL_COMPILE_STATUS, &shaderCompilationStatus);
        if(shaderCompilationStatus == GL_FALSE) {
            
            glGetShaderiv(fragmentShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLenght);
            if(infoLogLenght > 0) {
                
                szInfoLog = (char*) malloc(infoLogLenght);
                if(szInfoLog != NULL) {
                    
                    GLsizei written;
                    glGetShaderInfoLog(fragmentShaderObject_PV, infoLogLenght, &written, szInfoLog);
                    printf("\n*** Fragment Shader Compilation Status ***\n");
                    printf("%s\n", szInfoLog);
                    free(szInfoLog);
                    [self release];
                    exit(0);
                }
            }
        }
        
        
        //*************************************************************************************
        //                                 *** FRAGMENT SHADER FOR PER FRAGMENT ***
        //*************************************************************************************
        
        // create fragment shader
        fragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);
        
        // write the shader source code
        const GLchar* fragmentShaderSourceCode_PF =
        "#version 300 es                                                                                                                  \n" \
        "                                                                                                                                 \n" \
        "precision highp float;                                                                                                           \n" \
        "precision highp int;                                                                                                             \n" \
        "                                                                                                                                 \n" \
        "vec3 phong_ads_light;                                                                                                            \n" \
        "                                                                                                                                 \n" \
        "in vec3 transformed_normal;                                                                                                      \n" \
        "in vec3 light_direction[3];                                                                                                      \n" \
        "in vec3 view_vector;                                                                                                             \n" \
        "                                                                                                                                 \n" \
        "uniform int u_lkeypressed;                                                                                                       \n" \
        "                                                                                                                                 \n" \
        "uniform vec3 u_la[3];                                                                                                            \n" \
        "uniform vec3 u_ld[3];                                                                                                            \n" \
        "uniform vec3 u_ls[3];                                                                                                            \n" \
        "                                                                                                                                 \n" \
        "uniform vec3 u_ka;                                                                                                               \n" \
        "uniform vec3 u_kd;                                                                                                               \n" \
        "uniform vec3 u_ks;                                                                                                               \n" \
        "                                                                                                                                 \n" \
        "uniform float u_shininess;                                                                                                       \n" \
        "                                                                                                                                 \n" \
        "out vec4 FragColor;                                                                                                              \n" \
        "                                                                                                                                 \n" \
        "void main(void) {                                                                                                                \n" \
        "                                                                                                                                 \n" \
        "    int i = 0;                                                                                                                   \n" \
        "    phong_ads_light = vec3(0.0, 0.0, 0.0);                                                                                       \n" \
        "                                                                                                                                 \n" \
        "    if( u_lkeypressed == 1) {                                                                                                    \n" \
        "                                                                                                                                 \n" \
        "        vec3 normalized_transformed_normal     = normalize(transformed_normal);                                                  \n" \
        "        vec3 normalized_view_vector         = normalize(view_vector);                                                            \n" \
        "                                                                                                                                 \n" \
        "        for(i = 0; i < 3; i++) {                                                                                                 \n" \
        "                                                                                                                                 \n" \
        "            vec3 normalized_light_direction     = normalize(light_direction[i]);                                                 \n" \
        "            vec3 reflection_vector   = reflect(-normalized_light_direction,normalized_transformed_normal);                       \n" \
        "            vec3 ambient             = u_la[i] * u_ka;                                                                           \n" \
        "            vec3 diffuse             = u_ld[i] * u_kd * max(dot(normalized_light_direction,normalized_transformed_normal),0.0);  \n" \
        "            vec3 specular            = u_ls[i] * u_ks * pow(max(dot(reflection_vector,normalized_view_vector),0.0),u_shininess); \n" \
        "            phong_ads_light          = phong_ads_light + ambient + diffuse + specular;                                           \n" \
        "        }                                                                                                                        \n" \
        "                                                                                                                                 \n" \
        "    } else {                                                                                                                     \n" \
        "        phong_ads_light            = vec3(1.0,1.0,1.0);                                                                          \n" \
        "    }                                                                                                                            \n" \
        "    FragColor = vec4(phong_ads_light,1.0);                                                                                       \n" \
        "}                                                                                                                                \n";
        
        // provide source code to shader
        glShaderSource(fragmentShaderObject_PF, 1, (const GLchar**) &fragmentShaderSourceCode_PF, NULL);
        
        // compile shader source code
        glCompileShader(fragmentShaderObject_PF);
        
        // check compilation errors
        infoLogLenght                = 0;
        shaderCompilationStatus      = 0;
        szInfoLog                    = NULL;
        
        glGetShaderiv(fragmentShaderObject_PF, GL_COMPILE_STATUS, &shaderCompilationStatus);
        if(shaderCompilationStatus == GL_FALSE) {
            
            glGetShaderiv(fragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLenght);
            if(infoLogLenght > 0) {
                
                szInfoLog = (char*) malloc(infoLogLenght);
                if(szInfoLog != NULL) {
                    
                    GLsizei written;
                    glGetShaderInfoLog(fragmentShaderObject_PF, infoLogLenght, &written, szInfoLog);
                    printf("\n*** Fragment Shader Compilation Status ***\n");
                    printf("%s\n", szInfoLog);
                    free(szInfoLog);
                    [self release];
                    exit(0);
                }
            }
        }
        
        //*************************************************************************************
        //                            *** SHADER PROGRAM FOR PER VERTEX***
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
        infoLogLenght                = 0;
        shaderCompilationStatus      = 0;
        shaderProgramLinkStatus      = 0;
        szInfoLog                    = NULL;
        
        glGetProgramiv(shaderProgramObject_PV, GL_LINK_STATUS, &shaderProgramLinkStatus);
        if(shaderProgramLinkStatus == GL_FALSE) {
            
            glGetProgramiv(shaderProgramObject_PV, GL_INFO_LOG_LENGTH, &infoLogLenght);
            if(infoLogLenght > 0) {
                
                szInfoLog = (char*) malloc(infoLogLenght);
                if(szInfoLog != NULL) {
                    GLsizei written;
                    glGetProgramInfoLog(shaderProgramObject_PV, infoLogLenght,&written, szInfoLog);
                    printf("\n*** Shader Program Link Log ***\n");
                    printf("%s\n", szInfoLog);
                    [self release];
                    exit(0);
                    
                }
            }
        }
        //*************************************************************************************
        //                            *** SHADER PROGRAM FOR PER FRAGMENT***
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
        infoLogLenght                = 0;
        shaderCompilationStatus     = 0;
        shaderProgramLinkStatus        = 0;
        szInfoLog                    = NULL;
        
        glGetProgramiv(shaderProgramObject_PF, GL_LINK_STATUS, &shaderProgramLinkStatus);
        if(shaderProgramLinkStatus == GL_FALSE) {
            
            glGetProgramiv(shaderProgramObject_PF, GL_INFO_LOG_LENGTH, &infoLogLenght);
            if(infoLogLenght > 0) {
                
                szInfoLog = (char*) malloc(infoLogLenght);
                if(szInfoLog != NULL) {
                    GLsizei written;
                    glGetProgramInfoLog(shaderProgramObject_PF, infoLogLenght,&written, szInfoLog);
                    printf("\n*** Shader Program Link Log ***\n");
                    printf("%s\n", szInfoLog);
                    [self release];
                    exit(0);
                }
            }
        }
        //    get uniform locations from shader program FOR PER VERTEX
        modelMatrixUniform_PV         = glGetUniformLocation(shaderProgramObject_PV, "u_model_matrix");
        viewMatrixUniform_PV         = glGetUniformLocation(shaderProgramObject_PV, "u_view_matrix");
        projectionMatrixUniform_PV     = glGetUniformLocation(shaderProgramObject_PV, "u_projection_matrix");
        lKeyPressed_uniform_PV         = glGetUniformLocation(shaderProgramObject_PV, "u_lkeypressed");
        
        la_uniform_PV[0]                 = glGetUniformLocation(shaderProgramObject_PV, "u_la[0]"); // light ambient component
        ld_uniform_PV[0]                 = glGetUniformLocation(shaderProgramObject_PV, "u_ld[0]"); // light diffuse component
        ls_uniform_PV[0]                 = glGetUniformLocation(shaderProgramObject_PV, "u_ls[0]"); // light specular component
        lightPosition_uniform_PV[0]     = glGetUniformLocation(shaderProgramObject_PV, "u_light_position[0]"); // light position
        
        la_uniform_PV[1]                 = glGetUniformLocation(shaderProgramObject_PV, "u_la[1]"); // light ambient component
        ld_uniform_PV[1]                 = glGetUniformLocation(shaderProgramObject_PV, "u_ld[1]"); // light diffuse component
        ls_uniform_PV[1]                 = glGetUniformLocation(shaderProgramObject_PV, "u_ls[1]"); // light specular component
        lightPosition_uniform_PV[1]     = glGetUniformLocation(shaderProgramObject_PV, "u_light_position[1]"); // light position
        
        la_uniform_PV[2]                 = glGetUniformLocation(shaderProgramObject_PV, "u_la[2]"); // light ambient component
        ld_uniform_PV[2]                 = glGetUniformLocation(shaderProgramObject_PV, "u_ld[2]"); // light diffuse component
        ls_uniform_PV[2]                 = glGetUniformLocation(shaderProgramObject_PV, "u_ls[2]"); // light specular component
        lightPosition_uniform_PV[2]     = glGetUniformLocation(shaderProgramObject_PV, "u_light_position[2]"); // light position
        
        ka_uniform_PV                 = glGetUniformLocation(shaderProgramObject_PV, "u_ka"); // material ambient component
        kd_uniform_PV                 = glGetUniformLocation(shaderProgramObject_PV, "u_kd"); // material diffuse component
        ks_uniform_PV                 = glGetUniformLocation(shaderProgramObject_PV, "u_ks"); // material specular component
        shininess_uniform_PV         = glGetUniformLocation(shaderProgramObject_PV, "u_shininess"); // material shininess Component
        
        //    get uniform locations from shader program FOR PER FRAGMENT
        modelMatrixUniform_PF         = glGetUniformLocation(shaderProgramObject_PF, "u_model_matrix");
        viewMatrixUniform_PF         = glGetUniformLocation(shaderProgramObject_PF, "u_view_matrix");
        projectionMatrixUniform_PF     = glGetUniformLocation(shaderProgramObject_PF, "u_projection_matrix");
        lKeyPressed_uniform_PF         = glGetUniformLocation(shaderProgramObject_PF, "u_lkeypressed");
        
        la_uniform_PF[0]                 = glGetUniformLocation(shaderProgramObject_PF, "u_la[0]"); // light ambient component
        ld_uniform_PF[0]                 = glGetUniformLocation(shaderProgramObject_PF, "u_ld[0]"); // light diffuse component
        ls_uniform_PF[0]                 = glGetUniformLocation(shaderProgramObject_PF, "u_ls[0]"); // light specular component
        lightPosition_uniform_PF[0]     = glGetUniformLocation(shaderProgramObject_PF, "u_light_position[0]"); // light position
        
        la_uniform_PF[1]                 = glGetUniformLocation(shaderProgramObject_PF, "u_la[1]"); // light ambient component
        ld_uniform_PF[1]                 = glGetUniformLocation(shaderProgramObject_PF, "u_ld[1]"); // light diffuse component
        ls_uniform_PF[1]                 = glGetUniformLocation(shaderProgramObject_PF, "u_ls[1]"); // light specular component
        lightPosition_uniform_PF[1]     = glGetUniformLocation(shaderProgramObject_PF, "u_light_position[1]"); // light position
        
        la_uniform_PF[2]                 = glGetUniformLocation(shaderProgramObject_PF, "u_la[2]"); // light ambient component
        ld_uniform_PF[2]                 = glGetUniformLocation(shaderProgramObject_PF, "u_ld[2]"); // light diffuse component
        ls_uniform_PF[2]                 = glGetUniformLocation(shaderProgramObject_PF, "u_ls[2]"); // light specular component
        lightPosition_uniform_PF[2]     = glGetUniformLocation(shaderProgramObject_PF, "u_light_position[2]"); // light position
        
        ka_uniform_PF                 = glGetUniformLocation(shaderProgramObject_PF, "u_ka"); // material ambient component
        kd_uniform_PF                 = glGetUniformLocation(shaderProgramObject_PF, "u_kd"); // material diffuse component
        ks_uniform_PF                 = glGetUniformLocation(shaderProgramObject_PF, "u_ks"); // material specular component
        shininess_uniform_PF         = glGetUniformLocation(shaderProgramObject_PF, "u_shininess"); // material shininess Component
        
        
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
        
        // Variable Initialization
        gfAngle = 0.0f;
        bLight = false;
        bAnimate = false;
        gbPV = true;
        gbPF = false;
        gfAngle = 0.0f;
        
        light0_Angle = 0.0f;
        light1_Angle = 0.0f;
        light2_Angle = 0.0f;
        r = 5.0f;
        
        
        light[0].light_ambient    = vec3(0.0f, 0.0f, 0.0f);
        light[0].light_diffuse    = vec3(1.0f, 0.0f, 0.0f);
        light[0].light_specular    = vec3(1.0f, 0.0f, 0.0f);
        light[0].light_position    = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        
        light[1].light_ambient    = vec3(0.0f, 0.0f, 0.0f);
        light[1].light_diffuse    = vec3(0.0f, 1.0f, 0.0f);
        light[1].light_specular    = vec3(0.0f, 1.0f, 0.0f);
        light[1].light_position    = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        
        light[2].light_ambient    = vec3(0.0f, 0.0f, 0.0f);
        light[2].light_diffuse    = vec3(0.0f, 0.0f, 1.0f);
        light[2].light_specular    = vec3(0.0f, 0.0f, 1.0f);
        light[2].light_position    = vec4(0.0f, 0.0f, .0f, 1.0f);
        
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
        
        // Mandatory Function To Initialize Depth Buffer With All Ones
        glClearDepthf(1.0f);
        
        // Mandatory Function To Enable Depth Test
        glEnable(GL_DEPTH_TEST);
        
        // Mandatory Function To Checking Depth Bits With Reference Value
        glDepthFunc(GL_LEQUAL);
        
        glClearColor(0.045f, 0.045f, 0.045f, 1.0f);
        //____________________________________________________________________________________________________
        
        perspectiveProjectionMatrix = mat4::identity();
        printf("prepareOpenGL succeeded \n");
        // Gesture related code
        
        // Single tap
        UITapGestureRecognizer *singleTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        // double tap
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        // fail double tap if single tap
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        UISwipeGestureRecognizer *swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(onSwipe:)];
        [swipeGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(onLongPress:)];
        [longPressGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:longPressGestureRecognizer];
        
    }
    
    return (self);
}

+(Class) layerClass {
    
    // code
    return ([CAEAGLLayer class]);
}
/*
 - (void)drawRect:(CGRect)rect {
 
 // Drawing code
 }
 */

- (void)layoutSubviews {
    
    // code
    
    // Color render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    // Give storage to render buffer
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable: (CAEAGLLayer*) [self layer]];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    GLint width;
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
    // Depth Buffer
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    // Give storage to render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER != GL_FRAMEBUFFER_COMPLETE)) {
        printf("Framebuffer is not complete!\n");
    }
    
    if(height<0) {
        height = 1;
    }
    // Ortho or Perspective here...
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f,100.0f);
    
    [self drawView:nil];
}

- (void) drawView:(id) sender {
    
    // code
    
    [EAGLContext setCurrentContext:eaglContext];
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mat4 modelMatrix         = mat4::identity();
    mat4 viewMatrix         = mat4::identity();
    mat4 projectionMatrix     = mat4::identity();
    
    // Matrices
    modelMatrix      = vmath::translate(0.0f, 0.0f, -4.5f);
    viewMatrix         = mat4::identity();
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
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}


-(void) updateFunc {
    gfAngle += 1.0f;
    
    light0_Angle += 0.8f;
    if (light0_Angle >= 360.0f) {
        light0_Angle = 0.0f;
    }
    
    light1_Angle += 0.8f;
    if (light1_Angle >= 360.0f) {
        light1_Angle = 0.0f;
    }
    light2_Angle += 0.8f;
    
    if (light2_Angle >= 360.0f) {
        light2_Angle = 0.0f;
    }
}

- (void)startAnimation {
    
    // code
    
    if(isAnimating == NO) {
        displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector: @selector(drawView:)];
        [displayLink setPreferredFramesPerSecond:animationFrameInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        isAnimating = YES;
    }
}

- (void)stopAnimation {
    
    // code
    if(isAnimating == YES) {
        [displayLink invalidate];
        displayLink = nil;
    }
}

- (void)onSingleTap:(UITapGestureRecognizer *)gr {
    
    // code
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
}


- (void)onDoubleTap:(UITapGestureRecognizer *)gr {
    
    // code
    if(gbPV) {
        gbPV = false;
        gbPF = true;
    } else {
        gbPV = true;
        gbPF = false;
    }
}

- (void)onSwipe:(UISwipeGestureRecognizer *)gr {
    
    // code
    [self uninitialize];
    [self release];
    
    exit(0);
}

- (void)onLongPress:(UILongPressGestureRecognizer *)gr {
    
    // code
    if (bAnimate == true) {
        bAnimate = false;
    }
    else {
        bAnimate = true;
    }
}

- (void)uninitialize {
    
    // code
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
    
    if(depthRenderbuffer) {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    
    if(colorRenderbuffer) {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
    
    if(defaultFramebuffer) {
        glDeleteFramebuffers(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }
    
    if(eaglContext) {
        if([EAGLContext currentContext] == eaglContext) {
            [EAGLContext setCurrentContext:nil];
            [eaglContext release];
            eaglContext = nil;
        }
    }
}

- (void)dealloc {
    // code
    [self uninitialize];
    [super dealloc];
}

@end
