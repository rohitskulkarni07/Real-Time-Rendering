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

//Sphere related variable
SphereData sphere_data; 

@implementation GLESView {
@private
    EAGLContext *eaglContext;
    
    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    
    bool gbPolygonMode;
    bool bLight;
    bool bAnimate;
    bool gbPV;
    bool gbPF;
    
    
    GLuint vertexShaderObject;
    GLuint fragmentShaderObject;
    GLuint shaderProgramObject;
    
    GLuint vao_sphere;
    
    GLuint vbo_sphere_position;
    GLuint vbo_sphere_normals;
    GLuint vbo_sphere_textures;
    GLuint vbo_sphere_elements;
    
    GLfloat gfAngle;
    GLuint  keyPressed;
    GLfloat r;
    GLfloat gfAngleX;
    GLfloat gfAngleY;
    GLfloat gfAngleZ;
    GLfloat tx, ty, tz;
    GLfloat sx, sy, sz;
    
    GLint    giWidth;
    GLint    giHeight;
    
    // Lights Arrays
    GLfloat light_ambient[3];
    GLfloat light_diffuse[3];
    GLfloat light_specular[3];
    GLfloat light_position[4]; //light position
    
    // Material Arrays
    GLfloat material_ambient[4];
    GLfloat material_diffuse[4];
    GLfloat material_specular[4];
    GLfloat material_shininess;
    
    //--------------PER VERTEX------------------------------
    GLuint        vertexShaderObject_PV;
    GLuint        fragmentShaderObject_PV;
    GLuint        shaderProgramObject_PV;
    
    // variable for storing location of uniforms
    GLuint        modelMatrixUniform_PV;
    GLuint        viewMatrixUniform_PV;
    GLuint        projectionMatrixUniform_PV;
    GLuint        lKeyPressed_uniform_PV; //keypress
    
    // Lights Uniforms
    GLuint         la_uniform_PV;
    GLuint         ld_uniform_PV;
    GLuint         ls_uniform_PV;
    GLuint         lightPosition_uniform_PV;
    
    // Material Uniforms
    GLuint        ka_uniform_PV;
    GLuint        kd_uniform_PV;
    GLuint        ks_uniform_PV;
    GLuint        shininess_uniform_PV;
    
    
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
    GLuint         la_uniform_PF;
    GLuint         ld_uniform_PF;
    GLuint         ls_uniform_PF;
    GLuint         lightPosition_uniform_PF;
    
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
        "precision highp float;                                                                                                       \n" \
        "precision highp int;                                                                                                         \n" \
        "in vec4 vPosition;                                                                                                \n" \
        "in vec3 vNormal;                                                                                                \n" \
        "                                                                                                                \n" \
        "uniform mat4 u_model_matrix;                                                                                    \n" \
        "uniform mat4 u_view_matrix;                                                                                    \n" \
        "uniform mat4 u_projection_matrix;                                                                                \n" \
        "                                                                                                                \n" \
        "uniform int u_lkeypressed;                                                                                        \n" \
        "                                                                                                                \n" \
        "uniform vec3 u_la;                                                                                                \n" \
        "uniform vec3 u_ld;                                                                                                \n" \
        "uniform vec3 u_ls;                                                                                                \n" \
        "                                                                                                                \n" \
        "uniform vec4 u_light_position;                                                                                    \n" \
        "                                                                                                                \n" \
        "uniform vec3 u_ka;                                                                                                \n" \
        "uniform vec3 u_kd;                                                                                                \n" \
        "uniform vec3 u_ks;                                                                                                \n" \
        "                                                                                                                \n" \
        "uniform float u_shininess;                                                                                        \n" \
        "                                                                                                                \n" \
        "out vec3 phong_ads_light;                                                                                        \n" \
        "                                                                                                                \n" \
        "void main(void) {                                                                                                \n" \
        "                                                                                                                \n" \
        "    if(u_lkeypressed == 1) {                                                                                    \n" \
        "                                                                                                                \n" \
        "        vec4 eye_coordinates     = u_view_matrix * u_model_matrix * vPosition;                                    \n" \
        "        vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);                    \n" \
        "        vec3 light_direction    = normalize(vec3(u_light_position - eye_coordinates));                            \n" \
        "        vec3 reflection_vector     = reflect(-light_direction,transformed_normal);                                    \n" \
        "        vec3 view_vector         = normalize(-eye_coordinates.xyz);                                                \n" \
        "        vec3 ambient             = u_la * u_ka;                                                                    \n" \
        "        vec3 diffuse             = u_ld * u_kd * max(dot(light_direction,transformed_normal),0.0);                \n" \
        "        vec3 specular            = u_ls * u_ks * pow(max(dot(reflection_vector,view_vector),0.0),u_shininess);    \n" \
        "        phong_ads_light            = ambient + diffuse + specular;                                                    \n" \
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
        "precision highp float;                                                                                                       \n" \
        "precision highp int;                                                                                                         \n" \
        "in vec4 vPosition;                                                                                                \n" \
        "in vec3 vNormal;                                                                                                \n" \
        "                                                                                                                \n" \
        "uniform mat4 u_model_matrix;                                                                                    \n" \
        "uniform mat4 u_view_matrix;                                                                                    \n" \
        "uniform mat4 u_projection_matrix;                                                                                \n" \
        "                                                                                                                \n" \
        "uniform int u_lkeypressed;                                                                                        \n" \
        "                                                                                                                \n" \
        "uniform vec4 u_light_position;                                                                                    \n" \
        "                                                                                                                \n" \
        "out vec3 transformed_normal;                                                                                    \n" \
        "out vec3 light_direction;                                                                                        \n" \
        "out vec3 view_vector;                                                                                            \n" \
        "                                                                                                                \n" \
        "void main(void) {                                                                                                \n" \
        "                                                                                                                \n" \
        "    if(u_lkeypressed == 1) {                                                                                    \n" \
        "                                                                                                                \n" \
        "        vec4 eye_coordinates     = u_view_matrix * u_model_matrix * vPosition;                                    \n" \
        "        transformed_normal         = mat3(u_view_matrix * u_model_matrix) * vNormal;                                \n" \
        "        light_direction            = vec3(u_light_position - eye_coordinates);                                        \n" \
        "        view_vector             = (-eye_coordinates.xyz);                                                        \n" \
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
        "\n                                                                                                                 \n" \
        "precision highp float;                                                                                                       \n" \
        "precision highp int;                                                                                                         \n" \
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
        "#version 300 es                                                                                                              \n" \
        "                                                                                                                             \n" \
        "precision highp float;                                                                                                       \n" \
        "precision highp int;                                                                                                         \n" \
        "                                                                                                                             \n" \
        "vec3 phong_ads_light;                                                                                                        \n" \
        "                                                                                                                             \n" \
        "in vec3 transformed_normal;                                                                                                  \n" \
        "in vec3 light_direction;                                                                                                     \n" \
        "in vec3 view_vector;                                                                                                         \n" \
        "                                                                                                                             \n" \
        "uniform int u_lkeypressed;                                                                                                   \n" \
        "                                                                                                                             \n" \
        "uniform vec3 u_la;                                                                                                           \n" \
        "uniform vec3 u_ld;                                                                                                           \n" \
        "uniform vec3 u_ls;                                                                                                           \n" \
        "                                                                                                                             \n" \
        "uniform vec3 u_ka;                                                                                                           \n" \
        "uniform vec3 u_kd;                                                                                                           \n" \
        "uniform vec3 u_ks;                                                                                                           \n" \
        "                                                                                                                             \n" \
        "uniform float u_shininess;                                                                                                   \n" \
        "                                                                                                                             \n" \
        "out vec4 FragColor;                                                                                                          \n" \
        "                                                                                                                             \n" \
        "void main(void) {                                                                                                            \n" \
        "                                                                                                                             \n" \
        "    if( u_lkeypressed == 1) {                                                                                                \n" \
        "                                                                                                                             \n" \
        "        vec3 normalized_transformed_normal     = normalize(transformed_normal);                                              \n" \
        "        vec3 normalized_light_direction     = normalize(light_direction);                                                    \n" \
        "        vec3 normalized_view_vector         = normalize(view_vector);                                                        \n" \
        "                                                                                                                             \n" \
        "        vec3 reflection_vector     = reflect(-normalized_light_direction,normalized_transformed_normal);                     \n" \
        "        vec3 ambient             = u_la * u_ka;                                                                              \n" \
        "        vec3 diffuse             = u_ld * u_kd * max(dot(normalized_light_direction,normalized_transformed_normal),0.0);     \n" \
        "        vec3 specular            = u_ls * u_ks * pow(max(dot(reflection_vector,normalized_view_vector),0.0),u_shininess);    \n" \
        "        phong_ads_light            = ambient + diffuse + specular;                                                           \n" \
        "    } else {                                                                                                                 \n" \
        "        phong_ads_light            = vec3(1.0,1.0,1.0);                                                                      \n" \
        "    }                                                                                                                        \n" \
        "    FragColor = vec4(phong_ads_light,1.0);                                                                                   \n" \
        "}                                                                                                                            \n";
        
        // provide source code to shader
        glShaderSource(fragmentShaderObject_PF, 1, (const GLchar**) &fragmentShaderSourceCode_PF, NULL);
        
        // compile shader source code
        glCompileShader(fragmentShaderObject_PF);
        
        // check compilation errors
        infoLogLenght                = 0;
        shaderCompilationStatus     = 0;
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
        //glBindAttribLocation(shaderProgramObject_PV, RSK_ATTRIBUTE_COLOR, "vColor");
        glBindAttribLocation(shaderProgramObject_PV, RSK_ATTRIBUTE_NORMAL, "vNormal");
        
        // link shader program
        glLinkProgram(shaderProgramObject_PV);
        
        // check linking errors
        infoLogLenght                = 0;
        shaderCompilationStatus     = 0;
        shaderProgramLinkStatus        = 0;
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
        
        la_uniform_PV                 = glGetUniformLocation(shaderProgramObject_PV, "u_la"); // light ambient component
        ld_uniform_PV                 = glGetUniformLocation(shaderProgramObject_PV, "u_ld"); // light diffuse component
        ls_uniform_PV                 = glGetUniformLocation(shaderProgramObject_PV, "u_ls"); // light specular component
        lightPosition_uniform_PV     = glGetUniformLocation(shaderProgramObject_PV, "u_light_position"); // light position
        
        ka_uniform_PV                 = glGetUniformLocation(shaderProgramObject_PV, "u_ka"); // material ambient component
        kd_uniform_PV                 = glGetUniformLocation(shaderProgramObject_PV, "u_kd"); // material diffuse component
        ks_uniform_PV                 = glGetUniformLocation(shaderProgramObject_PV, "u_ks"); // material specular component
        shininess_uniform_PV         = glGetUniformLocation(shaderProgramObject_PV, "u_shininess"); // material shininess Component
        
        //    get uniform locations from shader program FOR PER FRAGMENT
        modelMatrixUniform_PF         = glGetUniformLocation(shaderProgramObject_PF, "u_model_matrix");
        viewMatrixUniform_PF         = glGetUniformLocation(shaderProgramObject_PF, "u_view_matrix");
        projectionMatrixUniform_PF     = glGetUniformLocation(shaderProgramObject_PF, "u_projection_matrix");
        lKeyPressed_uniform_PF         = glGetUniformLocation(shaderProgramObject_PF, "u_lkeypressed");
        
        la_uniform_PF                 = glGetUniformLocation(shaderProgramObject_PF, "u_la"); // light ambient component
        ld_uniform_PF                 = glGetUniformLocation(shaderProgramObject_PF, "u_ld"); // light diffuse component
        ls_uniform_PF                 = glGetUniformLocation(shaderProgramObject_PF, "u_ls"); // light specular component
        lightPosition_uniform_PF     = glGetUniformLocation(shaderProgramObject_PF, "u_light_position"); // light position
        
        ka_uniform_PF                 = glGetUniformLocation(shaderProgramObject_PF, "u_ka"); // material ambient component
        kd_uniform_PF                 = glGetUniformLocation(shaderProgramObject_PF, "u_kd"); // material diffuse component
        ks_uniform_PF                 = glGetUniformLocation(shaderProgramObject_PF, "u_ks"); // material specular component
        shininess_uniform_PF         = glGetUniformLocation(shaderProgramObject_PF, "u_shininess"); // material shininess Component
        
        
        Sphere(1.0, 30, 30, &sphere_data);
        
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
        
        // //for Geometry Normals Properties
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
        gbPolygonMode = true;
        bLight = false;
        bAnimate = true;
        gbPV = true;
        gbPF = false;
        gfAngle = 0.0f;
        
        keyPressed = 0;
        r = 100.0f;
        
        // angles vars
        gfAngleX = 0.0f;
        gfAngleY = 0.0f;
        gfAngleZ = 0.0f;
        
        // translation vars
        tx = 0.0f;
        ty = 0.0f;
        tz = -2.75f;
        
        // scaling vars
        sx = 0.4f;
        sy = 0.4f;
        sz = 0.4f;
        
        giWidth = 800;
        giHeight = 600;
        
        // Lights Arrays
        light_ambient[0] = 0.1f;
        light_ambient[1] = 0.1f;
        light_ambient[2] = 0.1f;
        
        light_diffuse[0] = 1.0f;
        light_diffuse[1] = 1.0f;
        light_diffuse[2] = 1.0f;
        
        light_specular[0] = 1.0f;
        light_specular[1] = 1.0f;
        light_specular[2] = 1.0f;
        
        light_position[0] = 100.0f;
        light_position[1] = 100.0f;
        light_position[2] = 100.0f;
        light_position[3] = 1.0f;
        
        // Mandatory Function To Initialize Depth Buffer With All Ones
        glClearDepthf(1.0f);
        
        // Mandatory Function To Enable Depth Test
        glEnable(GL_DEPTH_TEST);
        
        // Mandatory Function To Checking Depth Bits With Reference Value
        glDepthFunc(GL_LEQUAL);
        
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
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
    
    giWidth = width;
    giHeight = height;
    printf("%d | %d ", width, height);
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
    
    mat4 modelMatrix        = mat4::identity();
    mat4 viewMatrix         = mat4::identity();
    mat4 projectionMatrix   = mat4::identity();
    
    modelMatrix             = vmath::translate(0.0f, 0.0f, -1.5f);
    viewMatrix              = mat4::identity();
    projectionMatrix        = perspectiveProjectionMatrix;
    
    mat4 translationMatrix  = vmath::translate(tx, ty, tz);
    
    mat4 scalingMatrix      = vmath::scale(sx, sy, sz);
    
    mat4 rotationMatrix_X   = vmath::rotate(0.0f,0.0f,0.0f);
    mat4 rotationMatrix_Y   = vmath::rotate(0.0f,0.0f,0.0f);
    mat4 rotationMatrix_Z   = vmath::rotate(0.0f,0.0f,0.0f);
    
    modelMatrix = translationMatrix * scalingMatrix * rotationMatrix_X * rotationMatrix_Y * rotationMatrix_Z;
    
    // draw here...
    
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
        
        if (keyPressed == 1) {
            
            light_position[0] = 0.0f;
            light_position[1] = r * sin(gfAngleX);
            light_position[2] = r * cos(gfAngleX);
            light_position[3] = 1.0f;
            
        } else if (keyPressed == 2) {
            
            light_position[0] = r * sin(gfAngleY);
            light_position[1] = 0.0f;
            light_position[2] = r * cos(gfAngleY);
            light_position[3] = 1.0f;
            
        }    else if (keyPressed == 3) {
            
            light_position[0] = r * sin(gfAngleZ);
            light_position[1] = r * cos(gfAngleZ);
            light_position[2] = 0.0f;
            light_position[3] = 1.0f;
        }
        
        if (gbPV) {
            glUniform1i(lKeyPressed_uniform_PV, 1);
            glUniform3fv(la_uniform_PV, 1, (GLfloat*)light_ambient);  // light ambient
            glUniform3fv(ld_uniform_PV, 1, (GLfloat*)light_diffuse);  // light diffuse
            glUniform3fv(ls_uniform_PV, 1, (GLfloat*)light_specular); // light specular
            glUniform4fv(lightPosition_uniform_PV, 1, (GLfloat*)light_position); // light position
            
            glUniform3fv(ka_uniform_PV, 1, (GLfloat*)material_ambient);  // material ambient
            glUniform3fv(kd_uniform_PV, 1, (GLfloat*)material_diffuse);  // material diffuse
            glUniform3fv(ks_uniform_PV, 1, (GLfloat*)material_specular); // material specular
            glUniform1f(shininess_uniform_PV, (GLfloat)material_shininess); // material shininess
            //Sending Uniforms to Shader
            glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, projectionMatrix);
            
        } else {
            glUniform1i(lKeyPressed_uniform_PF, 1);
            
            glUniform3fv(la_uniform_PF, 1, (GLfloat*)light_ambient);  // light ambient
            glUniform3fv(ld_uniform_PF, 1, (GLfloat*)light_diffuse);  // light diffuse
            glUniform3fv(ls_uniform_PF, 1, (GLfloat*)light_specular); // light specular
            glUniform4fv(lightPosition_uniform_PF, 1, (GLfloat*)light_position); // light position
            
            glUniform3fv(ka_uniform_PF, 1, (GLfloat*)material_ambient);  // material ambient
            glUniform3fv(kd_uniform_PF, 1, (GLfloat*)material_diffuse);  // material diffuse
            glUniform3fv(ks_uniform_PF, 1, (GLfloat*)material_specular); // material specular
            glUniform1f(shininess_uniform_PF , (GLfloat)material_shininess ); // material shininess
            
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
        if (gbPF) {
            glUniform1i(lKeyPressed_uniform_PF, 0);
        }
    }
    
    // draw 24 spheres
    [self draw24Spheres];
    
    //stop using OpenGL program Object
    glUseProgram(0);
    
    [self updateFunc];
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

-(void) draw24Spheres {
    int viewX = 0; int viewY = 0;
    
    //_________________________ Emerald 1st Sphere On First Column _________________________
    material_ambient[0] = 0.0215f;    //r
    material_ambient[1] = 0.1745f;    //g
    material_ambient[2] = 0.0215f;    //b
    material_ambient[3] = 1.0f;        //a
    
    material_diffuse[0] = 0.07568f;    //r
    material_diffuse[1] = 0.61424f;    //g
    material_diffuse[2] = 0.07568f;    //b
    material_diffuse[3] = 1.0f;        //a
    
    material_specular[0] = 0.633f;        //r
    material_specular[1] = 0.727811f;    //g
    material_specular[2] = 0.633f;        //b
    material_specular[3] = 1.0f;        //a
    
    material_shininess = 0.6 * 128;
    viewX = 0; viewY = 5;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];
    
    //_________________________ Jade 2nd Sphere On First Column _________________________
    material_ambient[0] = 0.135f;    //r
    material_ambient[1] = 0.2255f;    //g
    material_ambient[2] = 0.1575f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.54;    //r
    material_diffuse[1] = 0.89;    //g
    material_diffuse[2] = 0.63f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.316228;    //r
    material_specular[1] = 0.316228;    //g
    material_specular[2] = 0.316228;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.1f * 128;

    viewX = 0; viewY = 4;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //___________________ Obsidian 3rd Sphere On First Column _______________________________
    material_ambient[0] = 0.05375f;    //r
    material_ambient[1] = 0.05f;    //g
    material_ambient[2] = 0.06625f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.18275f;    //r
    material_diffuse[1] = 0.17f;    //g
    material_diffuse[2] = 0.22525f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.332741f;    //r
    material_specular[1] = 0.328634f;    //g
    material_specular[2] = 0.346434f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.3f * 128;

    viewX = 0; viewY = 3;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //___________________ Pearl 4TH Sphere On First Column _______________________________
    material_ambient[0] = 0.25f;    //r
    material_ambient[1] = 0.20725f;        //g
    material_ambient[2] = 0.20725f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 1.0f;    //r
    material_diffuse[1] = 0.829f;    //g
    material_diffuse[2] = 0.829f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.296648f;    //r
    material_specular[1] = 0.296648f;    //g
    material_specular[2] = 0.296648f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.088f * 128;

    viewX = 0; viewY = 2;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //___________________ Ruby 5th Sphere On First Column _______________________________

    material_ambient[0] = 0.1745f;    //r
    material_ambient[1] = 0.01175f;        //g
    material_ambient[2] = 0.01175f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.61424f;    //r
    material_diffuse[1] = 0.04136f;    //g
    material_diffuse[2] = 0.04136f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.727811f;    //r
    material_specular[1] = 0.626959f;    //g
    material_specular[2] = 0.626959f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.6f * 128;

    viewX = 0; viewY = 1;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //___________________ Turquoise 6th Sphere On First Column _______________________________
    //Set Material
    material_ambient[0] = 0.1f;    //r
    material_ambient[1] = 0.18725f;        //g
    material_ambient[2] = 0.1745f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.396f;    //r
    material_diffuse[1] = 0.74151f;    //g
    material_diffuse[2] = 0.69102f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.297254f;    //r
    material_specular[1] = 0.30829f;    //g
    material_specular[2] = 0.306678f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.1f * 128;

    viewX = 0; viewY = 0;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //___________________________ Brass 1st Sphere On Second Column ___________________________
    //Set Material

    material_ambient[0] = 0.329412f;    //r
    material_ambient[1] = 0.223529f;    //g
    material_ambient[2] = 0.027451f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.780392f;    //r
    material_diffuse[1] = 0.568627f;    //g
    material_diffuse[2] = 0.113725f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.992157f;    //r
    material_specular[1] = 0.941176f;    //g
    material_specular[2] = 0.807843f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.21794872f * 128;

    viewX = 1; viewY = 5;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________
    //___________________________ Bronze 2nd Sphere On Second Column ___________________________
    //Set Material

    material_ambient[0] = 0.2125f;    //r
    material_ambient[1] = 0.1275f;    //g
    material_ambient[2] = 0.054f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.714f;    //r
    material_diffuse[1] = 0.4284f;    //g
    material_diffuse[2] = 0.18144f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.39354f;    //r
    material_specular[1] = 0.271906f;    //g
    material_specular[2] = 0.166721f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.2f * 128;

    viewX = 1; viewY = 4;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];
    //____________________________________________________________________________________
    //___________________________ Chrome 3rd Sphere On Second Column ___________________________

    material_ambient[0] = 0.25f;    //r
    material_ambient[1] = 0.25f;    //g
    material_ambient[2] = 0.25f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.4f;    //r
    material_diffuse[1] = 0.4f;    //g
    material_diffuse[2] = 0.4f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.774597f;    //r
    material_specular[1] = 0.774597f;    //g
    material_specular[2] = 0.774597f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.6f * 128;

    viewX = 1; viewY = 3;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];
    //____________________________________________________________________________________
    //___________________________ Copper 4th Sphere On Second Column ___________________________

    material_ambient[0] = 0.19125f;    //r
    material_ambient[1] = 0.0735f;    //g
    material_ambient[2] = 0.0225f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.7038f;    //r
    material_diffuse[1] = 0.27048f;    //g
    material_diffuse[2] = 0.0828f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.256777f;    //r
    material_specular[1] = 0.137622f;    //g
    material_specular[2] = 0.086014f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.1 * 128;

    viewX = 1; viewY = 2;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________
    //___________________________ Gold 5th Sphere On Second Column ___________________________

    //Set Material

    material_ambient[0] = 0.24725f;    //r
    material_ambient[1] = 0.1995f;    //g
    material_ambient[2] = 0.0745f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.75164f;    //r
    material_diffuse[1] = 0.60648f;    //g
    material_diffuse[2] = 0.2264f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.628281f;    //r
    material_specular[1] = 0.555802f;    //g
    material_specular[2] = 0.366065f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.4f * 128;

    viewX = 1; viewY = 1;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];
    //____________________________________________________________________________________
    //___________________________ Silver 6th Sphere On Second Column ___________________________

    material_ambient[0] = 0.19225f;    //r
    material_ambient[1] = 0.19225f;    //g
    material_ambient[2] = 0.19225f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.50754f;    //r
    material_diffuse[1] = 0.50754f;    //g
    material_diffuse[2] = 0.50754f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.508273f;    //r
    material_specular[1] = 0.508273f;    //g
    material_specular[2] = 0.508273f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.4f * 128;

    viewX = 1; viewY = 0;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________

    //___________________________ Black 1st Sphere On Third Column ___________________________
    material_ambient[0] = 0.0f;    //r
    material_ambient[1] = 0.0f;    //g
    material_ambient[2] = 0.0f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.01f;    //r
    material_diffuse[1] = 0.01f;    //g
    material_diffuse[2] = 0.01f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.50f;    //r
    material_specular[1] = 0.50f;    //g
    material_specular[2] = 0.50f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.25 * 128;
    viewX = 2; viewY = 5;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________
    //___________________________ Cyan 2nd Sphere On Third Column ___________________________

    material_ambient[0] = 0.0f;    //r
    material_ambient[1] = 0.1f;    //g
    material_ambient[2] = 0.06f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.0f;    //r
    material_diffuse[1] = 0.50980392f;    //g
    material_diffuse[2] = 0.50980392f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.50196078f;    //r
    material_specular[1] = 0.50196078f;    //g
    material_specular[2] = 0.50196078f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.25f * 128;
    viewX = 2; viewY = 4;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];
    //____________________________________________________________________________________
    //___________________________ Green 3rd Sphere On Third Column ___________________________

    material_ambient[0] = 0.0f;    //r
    material_ambient[1] = 0.0f;    //g
    material_ambient[2] = 0.0f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.1f;    //r
    material_diffuse[1] = 0.35f;    //g
    material_diffuse[2] = 0.1f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.45f;    //r
    material_specular[1] = 0.55f;    //g
    material_specular[2] = 0.45f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.25f * 128;

    viewX = 2; viewY = 3;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];
    //____________________________________________________________________________________
    //___________________________ Red 4th Sphere On Third Column ___________________________
    material_ambient[0] = 0.0f;    //r
    material_ambient[1] = 0.0f;    //g
    material_ambient[2] = 0.0f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.5f;    //r
    material_diffuse[1] = 0.0f;    //g
    material_diffuse[2] = 0.0f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.7f;    //r
    material_specular[1] = 0.6f;    //g
    material_specular[2] = 0.6f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.25 * 128;

    viewX = 2; viewY = 2;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________
    //___________________________ White 5th Sphere On Third Column ___________________________
    material_ambient[0] = 0.0f;    //r
    material_ambient[1] = 0.0f;    //g
    material_ambient[2] = 0.0f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.55f;    //r
    material_diffuse[1] = 0.55f;    //g
    material_diffuse[2] = 0.55f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.70f;    //r
    material_specular[1] = 0.70f;    //g
    material_specular[2] = 0.70f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.25f * 128;

    viewX = 2; viewY = 1;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________
    //___________________________ Yellow Plastic 6th Sphere On Third Column ___________________________

    material_ambient[0] = 0.0f;    //r
    material_ambient[1] = 0.0f;    //g
    material_ambient[2] = 0.0f;    //b
    material_ambient[3] = 1.0f;    //a

    material_diffuse[0] = 0.5f;    //r
    material_diffuse[1] = 0.5f;    //g
    material_diffuse[2] = 0.0f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.60f;    //r
    material_specular[1] = 0.60f;    //g
    material_specular[2] = 0.50f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.25f * 128;

    viewX = 2; viewY = 0;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________
    //___________________________ Black 1st Sphere On Fourth Column ___________________________

    material_ambient[0] = 0.02f;    //r
    material_ambient[1] = 0.02f;    //g
    material_ambient[2] = 0.02f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.01f;    //r
    material_diffuse[1] = 0.01f;    //g
    material_diffuse[2] = 0.01f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.4f;    //r
    material_specular[1] = 0.4f;    //g
    material_specular[2] = 0.4f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.078125f * 128;

    viewX = 3; viewY = 5;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________
    //___________________________ Cyan 2nd Sphere On Fourth Column ___________________________

    material_ambient[0] = 0.0f;    //r
    material_ambient[1] = 0.05f;    //g
    material_ambient[2] = 0.05f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.4f;    //r
    material_diffuse[1] = 0.5f;    //g
    material_diffuse[2] = 0.5f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.04f;    //r
    material_specular[1] = 0.7f;    //g
    material_specular[2] = 0.7f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.078125f * 128;

    viewX = 3; viewY = 4;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________


    //___________________________ Green 3rd Sphere On Fourth Column ___________________________

    material_ambient[0] = 0.0f;    //r
    material_ambient[1] = 0.05f;    //g
    material_ambient[2] = 0.0f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.4f;    //r
    material_diffuse[1] = 0.5f;    //g
    material_diffuse[2] = 0.4f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.04f;    //r
    material_specular[1] = 0.7f;    //g
    material_specular[2] = 0.04f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.078125f * 128;

    viewX = 3; viewY = 3;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________


    //___________________________ Red 4th Sphere On Fourth Column ___________________________

    material_ambient[0] = 0.05f;    //r
    material_ambient[1] = 0.0f;    //g
    material_ambient[2] = 0.0f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.5f;    //r
    material_diffuse[1] = 0.4f;    //g
    material_diffuse[2] = 0.4f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.7f;    //r
    material_specular[1] = 0.04f;    //g
    material_specular[2] = 0.04f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.078125f * 128;

    viewX = 3; viewY = 2;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];


    //____________________________________________________________________________________

    //___________________________ White 5th Sphere On Fourth Column ___________________________

    material_ambient[0] = 0.05f;    //r
    material_ambient[1] = 0.05f;    //g
    material_ambient[2] = 0.05f;    //b
    material_ambient[3] = 1.0f;        //a

    material_diffuse[0] = 0.5f;    //r
    material_diffuse[1] = 0.5f;    //g
    material_diffuse[2] = 0.5f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.7f;    //r
    material_specular[1] = 0.7f;    //g
    material_specular[2] = 0.7f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.078125f * 128;

    viewX = 3; viewY = 1;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];

    //____________________________________________________________________________________

    //___________________________ Yellow Rubber 6th Sphere On Fourth Column ___________________________

    material_ambient[0] = 0.05f;    //r
    material_ambient[1] = 0.05f;    //g
    material_ambient[2] = 0.0f;    //b
    material_ambient[3] = 1.0f;    //a

    material_diffuse[0] = 0.5f;    //r
    material_diffuse[1] = 0.5f;    //g
    material_diffuse[2] = 0.4f;    //b
    material_diffuse[3] = 1.0f;        //a

    material_specular[0] = 0.7f;    //r
    material_specular[1] = 0.7f;    //g
    material_specular[2] = 0.04f;    //b
    material_specular[3] = 1.0f;        //a

    material_shininess = 0.078125f * 128;

    viewX = 3; viewY = 0;
    [self setViewportXYAndDrawSphere:viewX secondParam:viewY];
    
    //____________________________________________________________________________________
    
    
}

-(void) setViewportXYAndDrawSphere: (int) viewX secondParam:(int) viewY {
    
    // Set Materials Uniforms
    if(gbPV) {
        glUniform3fv(ka_uniform_PV, 1, (GLfloat*)material_ambient);  // material ambient
        glUniform3fv(kd_uniform_PV, 1, (GLfloat*)material_diffuse);  // material diffuse
        glUniform3fv(ks_uniform_PV, 1, (GLfloat*)material_specular); // material specular
        glUniform1f(shininess_uniform_PV, (GLfloat)material_shininess); // material shininess
    } else {
        glUniform3fv(ka_uniform_PF, 1, (GLfloat*)material_ambient);  // material ambient
        glUniform3fv(kd_uniform_PF, 1, (GLfloat*)material_diffuse);  // material diffuse
        glUniform3fv(ks_uniform_PF, 1, (GLfloat*)material_specular); // material specular
        glUniform1f(shininess_uniform_PF, (GLfloat)material_shininess); // material shininess
    }
    
    //fprintf(gpFile," %d %d\n",giWidth, giHeight);
    // Set Viewport
    glViewport(viewX * ((GLsizei)giWidth / 4), viewY * ((GLsizei)giHeight / 6), (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
    
    // Draw sphere
    glBindVertexArray(vao_sphere);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);
    glDrawElements(GL_TRIANGLES, sphere_data.numberOfIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


-(void) updateFunc {
    if (bAnimate) {
        if (keyPressed == 1) {
            gfAngleX = gfAngleX + 0.2f;
        }
        else if (keyPressed == 2) {
            gfAngleY = gfAngleY + 0.2f;
        }
        else if (keyPressed == 3) {
            gfAngleZ = gfAngleZ + 0.2f;
        }
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
    if(keyPressed == 1) {
        gfAngleX = 0.0f;
    } else if (keyPressed == 2) {
        gfAngleY = 0.0f;
        
    } else if (keyPressed == 3) {
        gfAngleZ = 0.0f;
        keyPressed = 0;
    }
    keyPressed++;
}


- (void)onDoubleTap:(UITapGestureRecognizer *)gr {
    
    // code
    if(bLight) {
        bLight = false;
    } else {
        bLight = true;
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
    if(gbPV == true) {
        gbPV = false;
        gbPF = true;
    } else {
        gbPV = true;
        gbPF = false;
    }
}

- (void)uninitialize {
    
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
