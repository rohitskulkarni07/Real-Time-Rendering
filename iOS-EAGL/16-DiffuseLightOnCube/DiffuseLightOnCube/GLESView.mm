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

@implementation GLESView {
    @private
    EAGLContext *eaglContext;
    
    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    
    GLuint vertexShaderObject;
    GLuint fragmentShaderObject;
    GLuint shaderProgramObject;
    
    GLuint vao_cube;
    GLuint vbo_position_cube;
    GLuint vbo_normal_cube;

    // Uniforms
    GLuint uniform_perspectiveProjection;
    GLuint uniform_modelViewMatrixUniform;
    
    GLuint lKeyPressed_uniform;

    GLuint ld_uniform;
    GLuint kd_uniform;

    GLuint lightPosition_uniform;

    bool bLight;
    bool bAnimate;
    
    GLuint mvpMatrixUniform;

    GLfloat gfAngle;
       
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
        
        /* VERTEX SHADER SOURCE CODE */
        vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

        // provide source code to shader
        const GLchar* vertexShaderSourceCode =
            "#version 300 es                                                             \n" \
            "                                                                            \n" \
            "precision highp float;                                                      \n" \
            "precision highp int;                                                        \n" \
            "                                                                            \n" \
            "in vec4 vPosition;                                                          \n" \
            "in vec3 vNormal;                                                            \n" \
            "                                                                            \n" \
            "uniform mat4 u_model_view_matrix;                                           \n" \
            "uniform mat4 u_projection_matrix;                                           \n" \
            "                                                                            \n" \
            "uniform int u_lkeypressed;                                                  \n" \
            "                                                                            \n" \
            "uniform vec3 u_ld;                                                          \n" \
            "uniform vec3 u_kd;                                                          \n" \
            "uniform vec4 u_light_position;                                              \n" \
            "                                                                            \n" \
            "out vec3 diffuse_light;                                                     \n" \
            "                                                                            \n" \
            "void main(void) {                                                           \n" \
            "                                                                            \n" \
            "    if(u_lkeypressed == 1) {                                                \n" \
            "                                                                            \n" \
            "        vec4 eye_coordinates = u_model_view_matrix * vPosition;             \n" \
            "        mat3 normal_matrix = mat3(transpose(inverse(u_model_view_matrix))); \n" \
            "        vec3 t_norm = normalize(normal_matrix*vNormal);                     \n" \
            "        vec3 s = normalize(vec3(u_light_position-eye_coordinates));         \n" \
            "                                                                            \n" \
            "        diffuse_light = u_ld * u_kd * max(dot(s, t_norm),0.0);              \n" \
            "                                                                            \n" \
            "    }                                                                       \n" \
            "                                                                            \n" \
            "    gl_Position = u_projection_matrix * u_model_view_matrix * vPosition;    \n" \
            "                                                                            \n" \
            "}";//end of code

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
            "#version 300 es                                      \n" \
            "                                                     \n" \
            "precision highp float;                               \n" \
            "precision highp int;                                 \n" \
            "                                                     \n" \
            "vec4 color;                                          \n" \
            "                                                     \n" \
            "in vec3 diffuse_light;                               \n" \
            "uniform int u_lkeypressed;                           \n" \
            "                                                     \n" \
            "out vec4 FragColor;                                  \n" \
            "                                                     \n" \
            "void main(void)                                      \n" \
            "{                                                    \n" \
            "    if(u_lkeypressed == 1) {                         \n" \
            "        color = vec4(diffuse_light, 1.0);            \n" \
            "    } else {                                         \n" \
            "        color =  vec4(1.0,1.0,1.0,1.0);              \n" \
            "    }                                                \n" \
            "    FragColor = color;                               \n" \
            "}                                                    \n"; //endOfCode


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

        // get MVP uniform location
        uniform_modelViewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_model_view_matrix");
        uniform_perspectiveProjection = glGetUniformLocation(shaderProgramObject, "u_projection_matrix");
            
        lKeyPressed_uniform = glGetUniformLocation(shaderProgramObject, "u_lkeypressed");

        ld_uniform = glGetUniformLocation(shaderProgramObject, "u_ld");
        kd_uniform = glGetUniformLocation(shaderProgramObject, "u_kd");
        lightPosition_uniform = glGetUniformLocation(shaderProgramObject, "u_light_position");
        
        // *** vertices, colors, shader attributes, vbo, vao_cube initialization ***
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
        const GLfloat cubeNormals[] = {
                0.0f, 0.0f, 1.0f,    //Front Face
                0.0f, 0.0f, 1.0f,    //Front Face
                0.0f, 0.0f, 1.0f,    //Front Face
                0.0f, 0.0f, 1.0f,    //Front Face

                1.0f, 0.0f, 0.0f,    //Right Face
                1.0f, 0.0f, 0.0f,    //Right Face
                1.0f, 0.0f, 0.0f,    //Right Face
                1.0f, 0.0f, 0.0f,    //Right Face

                0.0f, 0.0f, -1.0f,    //Back Face
                0.0f, 0.0f, -1.0f,    //Back Face
                0.0f, 0.0f, -1.0f,    //Back Face
                0.0f, 0.0f, -1.0f,    //Back Face
                
                -1.0f, 0.0f, 0.0f,    //Left Face
                -1.0f, 0.0f, 0.0f,    //Left Face
                -1.0f, 0.0f, 0.0f,    //Left Face
                -1.0f, 0.0f, 0.0f,    //Left Face
                
                0.0f, 1.0f, 0.0f,    //Top Face
                0.0f, 1.0f, 0.0f,    //Top Face
                0.0f, 1.0f, 0.0f,    //Top Face
                0.0f, 1.0f, 0.0f,    //Top Face
                
                0.0f, -1.0f, 0.0f,    //Bottom Face
                0.0f, -1.0f, 0.0f,    //Bottom Face
                0.0f, -1.0f, 0.0f,    //Bottom Face
                0.0f, -1.0f, 0.0f,    //Bottom Face
            };
        
        //___________________________________________________________________________
        
        glGenVertexArrays(1, &vao_cube); // create named buffer object for vertex buffer object
        glBindVertexArray(vao_cube);        // bind vao_cube with buffer object

            //for position of cube
            glGenBuffers(1, &vbo_position_cube);    // creating named buffer object to map VRAM Buffer with Pointer
            glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube); // bind named object with VRAM Buffer
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW); //feed the data to VRAM buffer
            glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);    // how to read data from buffer
            glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);

            //for Normals of cube
            glGenBuffers(1, &vbo_normal_cube);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_normal_cube);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);
            glVertexAttribPointer(RSK_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(RSK_ATTRIBUTE_NORMAL);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        //___________________________________________________________________________
        
        // Variables initialisation
        gfAngle = 0.0f;
        bLight = false;
        bAnimate = false;
        
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
    
    //start using OpenGL program Object
    glUseProgram(shaderProgramObject);
   
    if (bLight) {
        
        glUniform1i(lKeyPressed_uniform, 1);
        glUniform3f(ld_uniform, 1.0f, 1.0f, 1.0f); // light diffuse
        glUniform3f(kd_uniform, 0.5f, 0.5f, 0.5f); // material
        GLfloat lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f }; //light position
        glUniform4fv(lightPosition_uniform, 1,(GLfloat*)lightPosition); // material

    }else {

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

    // *** bind vao_cube ***
    glBindVertexArray(vao_cube);

    // *** draw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

    // *** unbind vao_cube ***
    glBindVertexArray(0);
    //stop using OpenGL program Object
    glUseProgram(0);

    [self updateFunc];
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}


-(void) updateFunc {
    if (bAnimate == true) {
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
    if (bLight == true) {
        bLight = false;
    }
    else {
        bLight = true;
    }
    
}


- (void)onDoubleTap:(UITapGestureRecognizer *)gr {
    
    // code
    if (bAnimate == true) {
                bAnimate = false;
    }
    else {
        bAnimate = true;
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
