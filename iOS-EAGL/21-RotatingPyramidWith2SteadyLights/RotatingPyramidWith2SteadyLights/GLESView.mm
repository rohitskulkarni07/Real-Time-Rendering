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

struct Light light[2];


// Material Arrays
GLfloat     material_ambient[3] = { 0.0f, 0.0f, 0.0f };
GLfloat     material_diffuse[3] = { 1.0f, 1.0f, 1.0f };
GLfloat     material_specular[3] = { 1.0f, 1.0f, 1.0f };
GLfloat     material_shininess = 128.0f;

bool        gbLights        = false;
bool        gbAnimate       = false;


@implementation GLESView {
@private
    EAGLContext *eaglContext;
    
    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    
    // *** Shader Related Variables***/
    GLuint        vertexShaderObject;
    GLuint        fragmentShaderObject;
    GLuint        shaderProgramObject;
    
    // vertex array object and vertex buffer object
    GLuint         vao_pyramid;
    GLuint         vbo_pyramid_position;
    GLuint         vbo_pyramid_normals;
    
    // variable for storing location of uniforms
    GLuint         modelMatrixUniform;
    GLuint         viewMatrixUniform;
    GLuint         projectionMatrixUniform;
    GLuint         lKeyPressed_uniform; //keypress
    
    // Lights Uniforms
    GLuint         la_uniform[2];
    GLuint         ld_uniform[2];
    GLuint         ls_uniform[2];
    GLuint         lightPosition_uniform[2];
    
    // Material Uniforms
    GLuint         ka_uniform;
    GLuint         kd_uniform;
    GLuint         ks_uniform;
    GLuint         shininess_uniform;
    
    GLfloat gfAngle;
    
    // projection matirx
    vmath::mat4 perspectiveProjectionMatrix;    // 4x4 matrix
    
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
        
        /* VERTEX SHADER SOURCE CODE */
        vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
        
        // provide source code to shader
        const GLchar* vertexShaderSourceCode =
        "#version 300 es                                                                                                 \n" \
        "precision highp float;                                                                                          \n" \
        "precision highp int;                                                                                            \n" \
        "                                                                                                                \n" \
        "in vec4 vPosition;                                                                                              \n" \
        "in vec3 vNormal;                                                                                                \n" \
        "                                                                                                                \n" \
        "uniform mat4 u_model_matrix;                                                                                    \n" \
        "uniform mat4 u_view_matrix;                                                                                     \n" \
        "uniform mat4 u_projection_matrix;                                                                               \n" \
        "                                                                                                                \n" \
        "uniform int u_lkeypressed;                                                                                      \n" \
        "                                                                                                                \n" \
        "uniform vec3 u_la[2];                                                                                           \n" \
        "uniform vec3 u_ld[2];                                                                                           \n" \
        "uniform vec3 u_ls[2];                                                                                           \n" \
        "                                                                                                                \n" \
        "uniform vec4 u_light_position[2];                                                                               \n" \
        "                                                                                                                \n" \
        "uniform vec3 u_ka;                                                                                              \n" \
        "uniform vec3 u_kd;                                                                                              \n" \
        "uniform vec3 u_ks;                                                                                              \n" \
        "                                                                                                                \n" \
        "uniform float u_shininess;                                                                                      \n" \
        "                                                                                                                \n" \
        "vec3 ambient;                                                                                                   \n" \
        "vec3 diffuse;                                                                                                   \n" \
        "vec3 specular;                                                                                                  \n" \
        "vec3 light_direction;                                                                                           \n" \
        "vec3 reflection_vector;                                                                                         \n" \
        "                                                                                                                \n" \
        "out vec3 phong_ads_light;                                                                                       \n" \
        "                                                                                                                \n" \
        "void main(void) {                                                                                               \n" \
        "                                                                                                                \n" \
        "    int i = 0;                                                                                                  \n" \
        "    phong_ads_light    = vec3(0.0,0.0,0.0);                                                                     \n" \
        "                                                                                                                \n" \
        "    if(u_lkeypressed == 1) {                                                                                    \n" \
        "        vec4 eye_coordinates     = u_view_matrix * u_model_matrix * vPosition;                                  \n" \
        "        vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);                    \n" \
        "        vec3 view_vector         = normalize(-eye_coordinates.xyz);                                             \n" \
        "                                                                                                                \n" \
        "        for(i = 0; i < 2; i++) {                                                                                \n" \
        "                                                                                                                \n" \
        "            light_direction        = normalize(vec3(u_light_position[i] - eye_coordinates));                    \n" \
        "            reflection_vector     = reflect(-light_direction,transformed_normal);                               \n" \
        "            ambient             = u_la[i] * u_ka;                                                               \n" \
        "            diffuse             = u_ld[i] * u_kd * max(dot(light_direction,transformed_normal),0.0);            \n" \
        "            specular            = u_ls[i] * u_ks * pow(max(dot(reflection_vector,view_vector),0.0),u_shininess);\n" \
        "            phong_ads_light        = phong_ads_light + ambient + diffuse + specular;                            \n" \
        "        }                                                                                                       \n" \
        "                                                                                                                \n" \
        "    } else {                                                                                                    \n" \
        "        phong_ads_light            = vec3(1.0,1.0,1.0);                                                         \n" \
        "    }                                                                                                           \n" \
        "                                                                                                                \n" \
        "    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;                             \n" \
        "}                                                                                                               \n";
        
        
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
                    printf("*** Vertex Shader Compilation Status ***\n%s\n",szInfoLog);
                    free(szInfoLog);
                    [self release];
                    exit(0);
                }
            }
        }
        
        /* FRAGMENT SHADER SOURCE CODE */
        fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
        
        // provide source code to shader
        const GLchar* fragmentShaderSourceCode =
        "#version 300 es                                \n" \
        "precision highp float;                         \n" \
        "precision highp int;                           \n" \
        "in vec3    phong_ads_light;                    \n" \
        "out vec4    FragColor;                         \n" \
        "                                               \n" \
        "void main(void) {                              \n" \
        "    FragColor = vec4(phong_ads_light,1.0);     \n" \
        "}                                              \n";
        
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
                    printf("*** Fragment Shader Compilation Status ***\n%s\n", szInfoLog);
                    free(szInfoLog);
                    
                    [self release];
                    exit(0);
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
                    printf("Shader Program Link Log :\n%s\n", szInfoLog);
                    free(szInfoLog);
                    
                    [self release];
                    exit(0);
                }
            }
        }
        
        modelMatrixUniform         = glGetUniformLocation(shaderProgramObject, "u_model_matrix");
        viewMatrixUniform         = glGetUniformLocation(shaderProgramObject, "u_view_matrix");
        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projection_matrix");
        lKeyPressed_uniform     = glGetUniformLocation(shaderProgramObject, "u_lkeypressed");
        
        la_uniform[0]             = glGetUniformLocation(shaderProgramObject, "u_la[0]"); // light ambient component
        ld_uniform[0]             = glGetUniformLocation(shaderProgramObject, "u_ld[0]"); // light diffuse component
        ls_uniform[0]             = glGetUniformLocation(shaderProgramObject, "u_ls[0]"); // light specular component
        
        la_uniform[1]             = glGetUniformLocation(shaderProgramObject, "u_la[1]"); // light ambient component
        ld_uniform[1]             = glGetUniformLocation(shaderProgramObject, "u_ld[1]"); // light diffuse component
        ls_uniform[1]             = glGetUniformLocation(shaderProgramObject, "u_ls[1]"); // light specular component
        
        lightPosition_uniform[0]     = glGetUniformLocation(shaderProgramObject, "u_light_position[0]"); // light position
        lightPosition_uniform[1]     = glGetUniformLocation(shaderProgramObject, "u_light_position[1]"); // light position
        
        ka_uniform                 = glGetUniformLocation(shaderProgramObject, "u_ka"); // material ambient component
        kd_uniform                 = glGetUniformLocation(shaderProgramObject, "u_kd"); // material diffuse component
        ks_uniform                 = glGetUniformLocation(shaderProgramObject, "u_ks"); // material specular component
        shininess_uniform         = glGetUniformLocation(shaderProgramObject, "u_shininess"); // material shininess Component
        
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
            0.0f,    0.447214f,    0.894427f,
            0.0f,    0.447214f,    0.894427f,
            0.0f,    0.447214f,    0.894427f,
            
            // right face
            0.894427f,    0.447214f,    0.0f,
            0.894427f,    0.447214f,    0.0f,
            0.894427f,    0.447214f,    0.0f,
            
            // back face
            0.0f,    0.447214f,    -0.894427f,
            0.0f,    0.447214f,    -0.894427f,
            0.0f,    0.447214f,    -0.894427f,
            
            // left face
            -0.894427f,    0.447214f,    0.0f,
            -0.894427f,    0.447214f,    0.0f,
            -0.894427f,    0.447214f,    0.0f
        };
        
        // *** vertices, colors, shader attributes, vbo, vao_pyramid initialization ***
        //____________________________________________________________________________________________________
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
        
        //____________________________________________________________________________________________________
        
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
        
        //____________________________________________________________________________________________________
        
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
    
    // draw here...
    mat4 modelMatrix        = mat4::identity();
    mat4 viewMatrix         = mat4::identity();
    mat4 projectionMatrix   = mat4::identity();
    
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
    modelMatrix      = vmath::translate(0.0f, 0.0f, -4.0f) * vmath::rotate(gfAngle, 0.0f,1.0f,0.0f);
    viewMatrix       = mat4::identity();
    projectionMatrix = perspectiveProjectionMatrix;
    
    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE,projectionMatrix);
    
    glBindVertexArray(vao_pyramid);
    glDrawArrays(GL_TRIANGLES, 0, 12);    // why 12 is last parameter because vertices * triangle faces
    glBindVertexArray(0);
    
    // stop using shader program
    glUseProgram(0);
    
    [self updateFunc];
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}


-(void) updateFunc {
    if(gbAnimate) {
        gfAngle += 1.0f;
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
    if (gbLights == true) {
        gbLights = false;
    } else {
        gbLights = true;
    }
}


- (void)onDoubleTap:(UITapGestureRecognizer *)gr {
    
    // code
    if (gbAnimate == true) {
        gbAnimate = false;
    }
    else {
        gbAnimate = true;
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
}

- (void)uninitialize {
    
    // code
    if (vao_pyramid) {
        glDeleteVertexArrays(1, &vao_pyramid);
        vao_pyramid = 0;
    }
    
    if (vbo_pyramid_position) {
        glDeleteBuffers(1, &vbo_pyramid_position);
        vbo_pyramid_position = 0;
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
