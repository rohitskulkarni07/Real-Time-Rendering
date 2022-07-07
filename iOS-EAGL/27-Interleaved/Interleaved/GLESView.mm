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
    
    GLuint  guiTexture_marble;
    
    GLuint vertexShaderObject;
    GLuint fragmentShaderObject;
    GLuint shaderProgramObject;
    
    GLuint vao_cube;
    GLuint vbo_position_cube;
    
    // variable for storing location of uniforms
    GLuint uniform_modelViewMatrixUniform;
    GLuint uniform_perspectiveProjection;
    GLuint lKeyPressed_uniform;
    GLuint mvpMatrixUniform;
    
    GLuint ld_uniform;    //light diffuse
    GLuint kd_uniform;    //material diffuse
    
    GLuint lightPosition_uniform; //position of light
    GLuint textureSamplerUniform;    //for sampler
    
    bool bLight;
    bool bAnimate;
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
        "#version 300 es                                  \n" \
        "precision highp float;                         \n" \
        "precision highp int;                         \n" \
        "in vec4 vPosition;                                                                \n" \
        "in vec3 vColor;                                                                \n" \
        "in vec3 vNormal;                                                                \n" \
        "in vec2 vTexCoord;                                                                \n" \
        "                                                                                \n" \
        "out vec2 out_texcoord;                                                            \n" \
        "out vec3 out_color;                                                            \n" \
        "                                                                                \n" \
        "uniform mat4 u_model_view_matrix;                                                \n" \
        "uniform mat4 u_projection_matrix;                                                \n" \
        "uniform int u_lkeypressed;                                                        \n" \
        "uniform vec3 u_ld;                                                                \n" \
        "uniform vec3 u_kd;                                                                \n" \
        "uniform vec4 u_light_position;                                                    \n" \
        "                                                                                \n" \
        "out vec3 diffuse_light;                                                        \n" \
        "                                                                                \n" \
        "void main(void)                                                                \n" \
        "{                                                                                \n" \
        "                                                                                \n" \
        "    if(u_lkeypressed == 1) {                                                    \n" \
        "        vec4 eye_coordinates = u_model_view_matrix * vPosition;                    \n" \
        "        mat3 normal_matrix = mat3(transpose(inverse(u_model_view_matrix)));        \n" \
        "        vec3 t_norm = normalize(normal_matrix*vNormal);                            \n" \
        "        vec3 s = normalize(vec3(u_light_position-eye_coordinates));                \n" \
        "        diffuse_light = u_ld * u_kd * max(dot(s, t_norm),0.0);                    \n" \
        "    }                                                                            \n" \
        "                                                                                \n" \
        "    gl_Position = u_projection_matrix * u_model_view_matrix * vPosition;        \n" \
        "    out_texcoord = vTexCoord;                                                    \n" \
        "    out_color = vColor;                                                            \n" \
        "}                                                                                \n";//end of code
        
        
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
        "precision highp int;                         \n" \
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
                    printf("Shader Program Link Log :\n%s\n", szInfoLog);
                    free(szInfoLog);
                    
                    [self release];
                    exit(0);
                }
            }
        }
        
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
        {    // vertice                //color                //normals            //tex-coords
            1.0f,    1.0f, 1.0f,        1.0f, 0.89f, 0.89f,        0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
            -1.0f,    1.0f, 1.0f,        0.89f, 0.94f, 1.0f,        0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
            -1.0f, -1.0f, 1.0f,        0.90, 0.89f, 0.94f,       0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,        0.98f, 0.8f, 1.0f,       0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
            
            1.0f,  1.0f, -1.0f,        1.0f, 0.89f, 0.89f,        1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
            1.0f,  1.0f,  1.0f,        0.89f, 0.94f, 1.0f,        1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
            1.0f, -1.0f,  1.0f,        0.90, 0.89f, 0.94f,        1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
            1.0f, -1.0f, -1.0f,        0.98f, 0.8f, 1.0f,        1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
            
            1.0f,  1.0f, -1.0f,    1.0f, 0.89f, 0.89f,        0.0f, 0.0f, -1.0f,    1.0f, 0.0f,
            -1.0f,  1.0f, -1.0f,    0.89f, 0.94f, 1.0f,        0.0f, 0.0f, -1.0f,    1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,    0.90, 0.89f, 0.94f,        0.0f, 0.0f, -1.0f,    0.0f, 1.0f,
            1.0f, -1.0f, -1.0f,    0.98f, 0.8f, 1.0f,        0.0f, 0.0f, -1.0f,    0.0f, 0.0f,
            
            -1.0f,  1.0f, -1.0f,    1.0f, 0.89f, 0.89f,        -1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
            -1.0f,  1.0f,  1.0f,    0.89f, 0.94f, 1.0f,        -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
            -1.0f, -1.0f,  1.0f,    0.90, 0.89f, 0.94f,        -1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,    0.98f, 0.8f, 1.0f,        -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
            
            1.0f, 1.0f,  1.0f,        1.0f, 0.89f, 0.89f,        0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
            -1.0f, 1.0f,  1.0f,        0.89f, 0.94f, 1.0f,        0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
            -1.0f, 1.0f, -1.0f,        0.90, 0.89f, 0.94f,        0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
            1.0f, 1.0f, -1.0f,        0.98f, 0.8f, 1.0f,        0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
            
            1.0f, -1.0f,  1.0f,    1.0f, 0.89f, 0.89f,        0.0f, -1.0f, 0.0f,    1.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,    0.89f, 0.94f, 1.0f,        0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,    0.90, 0.89f, 0.94f,        0.0f, -1.0f, 0.0f,    0.0f, 0.0f,
            1.0f, -1.0f, -1.0f,    0.98f, 0.8f, 1.0f,        0.0f, -1.0f, 0.0f,    1.0f, 0.0f
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
        
        //______________________________________________________________________________________
        guiTexture_marble = [self loadTextureFromBMPFile: @"marble" : @"bmp"];
        if(guiTexture_marble == -1) {
            [self release];
            exit(0);
        }
        //
        
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

-(GLuint) loadTextureFromBMPFile:(NSString *)imageFileName :(NSString*)extension {
    
    // code
    NSString *imageFileNameWithPath = [[NSBundle mainBundle]pathForResource:imageFileName ofType:extension];
    
    UIImage *bmpImage = [[UIImage alloc] initWithContentsOfFile: imageFileNameWithPath]; // Get NSImage Representation of our image file
    // error checking
    if(!bmpImage) {
        printf("\nFailed to get NSImage format for BMP file, exiting now! : %d\n",__LINE__);
        return (-1);
    }
    CGImageRef cgImage = [bmpImage CGImage];    // Get CG Image Representation of NSImage
    
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
    
    return (texture);
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
    glUseProgram(0);    //upd
    
    [self updateFunc];
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}


-(void) updateFunc {
    
    if(bAnimate) {
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
