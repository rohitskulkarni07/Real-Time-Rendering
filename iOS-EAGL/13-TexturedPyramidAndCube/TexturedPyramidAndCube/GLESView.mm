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
    
    // for pyramid
    GLuint vao_pyramid;
    GLuint vbo_position_pyramid;
    GLuint vbo_textured_pyramid;

    // for cube
    GLuint vao_cube;
    GLuint vbo_position_cube;
    GLuint vbo_textured_cube;

    // uniforms
    GLuint mvpMatrixUniform;        // for model-view-projection
    GLuint textureSamplerUniform;    // for sampler
    
    GLfloat gfAngle;
      
    mat4 perspectiveProjectionMatrix;
    
    //Texture Related Variables GLuint  = OpenGL unsigned integer
    GLuint guiStone_texture;
    GLuint guiKundali_texture;
    
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
            "#version 300 es                                    \n" \
            "                                                   \n" \
            "in vec2 vTexCoord;                                 \n" \
            "in vec4 vPosition;                                 \n" \
            "                                                   \n" \
            "uniform mat4 u_mvp_matrix;                         \n" \
            "                                                   \n" \
            "out vec2 out_texcoord;                             \n" \
            "                                                   \n" \
            "void main(void)                                    \n" \
            "{                                                  \n" \
            "                                                   \n" \
            "   gl_Position = u_mvp_matrix * vPosition;         \n" \
            "   out_texcoord =  vTexCoord;                      \n" \
            "                                                   \n" \
            "}                                                  \n ";//endOfCode

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
            "#version 300 es                                                \n" \
            "                                                               \n" \
            "precision highp float;                                         \n" \
            "                                                               \n" \
            "in vec2 out_texcoord;                                          \n" \
            "                                                               \n" \
            "uniform sampler2D u_texture_sampler;                           \n" \
            "                                                               \n" \
            "out vec4 FragColor;                                            \n" \
            "                                                               \n" \
            "void main(void)                                                \n" \
            "{                                                              \n" \
            "                                                               \n" \
            "   FragColor = texture( u_texture_sampler, out_texcoord);      \n" \
            "                                                               \n" \
            "}                                                              \n"; //endOfCode

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
        glBindAttribLocation(shaderProgramObject, RSK_ATTRIBUTE_TEXTURE0, "vTexCoord");


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

        // uniform location
        mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");
        textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_texture_sampler");
        
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

        const GLfloat pyramidTexCoord2f[] =
            {
                // front face
                0.5f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,


                //right face
                0.5f, 1.0f,
                1.0f, 0.0f,
                0.0f, 0.0f,

                //back face
                0.5f, 1.0f,
                1.0f, 0.0f,
                0.0f, 0.0f,

                //left face
                0.5f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f

            };

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

        const GLfloat cubeTexCoord2f[] =
            {
                //front face
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,
                //right face
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,

                //back face

                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,

                //left face
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,

                //top face
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,

                //bottom face
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f
            };
            //__________________________________________________________________________________________

        // For Pyramid
        glGenVertexArrays(1, &vao_pyramid); // create named buffer object for vertex buffer object
        glBindVertexArray(vao_pyramid);        // bind vao with buffer object

            //for position of triangle
            glGenBuffers(1, &vbo_position_pyramid);    // creating named buffer object to map VRAM Buffer with Pointer
            glBindBuffer(GL_ARRAY_BUFFER, vbo_position_pyramid); // bind named object with VRAM Buffer
            glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW); //feed the data to VRAM buffer
            glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);    // how to read data from buffer
            glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
            glBindBuffer(GL_ARRAY_BUFFER, 0);    //Un-Bind
            
            //for texture of triangle
            glGenBuffers(1, &vbo_textured_pyramid);    //
            glBindBuffer(GL_ARRAY_BUFFER, vbo_textured_pyramid);
            glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidTexCoord2f), pyramidTexCoord2f, GL_STATIC_DRAW);
            glVertexAttribPointer(RSK_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(RSK_ATTRIBUTE_TEXTURE0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
            
        //___________________________________________________________________________________________

        // For Cube
        glGenVertexArrays(1, &vao_cube);
        glBindVertexArray(vao_cube);

            //for position of cube
            glGenBuffers(1, &vbo_position_cube);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(RSK_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(RSK_ATTRIBUTE_POSITION);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            //for texture of cube
            glGenBuffers(1, &vbo_textured_cube);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_textured_cube);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexCoord2f), cubeTexCoord2f, GL_STATIC_DRAW);
            glVertexAttribPointer(RSK_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(RSK_ATTRIBUTE_TEXTURE0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
        //___________________________________________________________________________
    
        
        //Enable Texture;
        glEnable(GL_TEXTURE_2D);
        
        // Mandatory Function To Initialize Depth Buffer With All Ones
        glClearDepthf(1.0f);

        // Mandatory Function To Enable Depth Test
        glEnable(GL_DEPTH_TEST);

        // Mandatory Function To Checking Depth Bits With Reference Value
        glDepthFunc(GL_LEQUAL);
        
        glClearColor(0.045f, 0.045f, 0.045f, 1.0f);
        //____________________________________________________________________________________________________
        
        // Variables Initialisation
        gfAngle = 0.0f;
        
        guiStone_texture = [self loadTextureFromBMPFile: @"Stone" : @"bmp"];
        if(guiStone_texture == -1) {
            [self release];
            exit(0);
        }
            
        guiKundali_texture = [self loadTextureFromBMPFile: @"Vijay_Kundali": @"bmp"];
        if(guiKundali_texture == -1) {
            [self release];
            exit(0);
        }
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
    
    // draw here...
    //start using OpenGL program Object
    glUseProgram(shaderProgramObject);

    //TRIANGLE TRANSFORMATION

        //set model-view & model-view-projection matrices to identity
        mat4 modelViewMatrix = mat4::identity();
        mat4 modelViewProjectionMatrix = mat4::identity();
        //translate matrix
        mat4 translateMatrix = mat4::identity();
        mat4 scaleMatrix  = mat4::identity();
        mat4 rotationMatrix = mat4::identity();

        mat4 rotationMatrixX = mat4::identity();
        mat4 rotationMatrixY = mat4::identity();
        mat4 rotationMatrixZ = mat4::identity();

        
        translateMatrix = vmath::translate(-1.5f, 0.0f, -6.0f);
        rotationMatrix = vmath::rotate(gfAngle, 0.0f, 1.0f, 0.0f);

        modelViewMatrix = translateMatrix * rotationMatrix;

        //ORDER OF MULTIPLICATION IMPORTANT BECAUSE MATRIX MULTIPLICATION IS NON_COMMUTATIVE
        modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

        //pass above modelViewProjection matrix to the vertex shader in u_mvp_matrix shader variable ...This is Pre-Multiplied Matrix
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, guiStone_texture);        // change state to texture for next upcoming geometry
        glUniform1i(textureSamplerUniform, 0);
        glBindVertexArray(vao_pyramid);

        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()

        glDrawArrays(GL_TRIANGLES, 0, 12);    // why 12 is last parameter because vertices * triangle faces

        glBindVertexArray(0);

        modelViewMatrix = mat4::identity();
        modelViewProjectionMatrix = mat4::identity();
        translateMatrix = vmath::translate(1.5f, 0.0f, -6.0f);
        scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
        rotationMatrixX = vmath::rotate(gfAngle, 1.0f, 0.0f, 0.0f);
        rotationMatrixY = vmath::rotate(gfAngle, 0.0f, 1.0f, 0.0f);
        rotationMatrixZ = vmath::rotate(gfAngle, 0.0f, 0.0f, 1.0f);

        modelViewMatrix = translateMatrix * scaleMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ;
        modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, guiKundali_texture);    //change state to texture for next upcoming geometry
        glUniform1i(textureSamplerUniform, 0);

        // *** bind vao_square ***
        glBindVertexArray(vao_cube);

        // *** draw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

        // *** unbind vao ***
        glBindVertexArray(0);

    //stop using OpenGL program Object
    glUseProgram(0);

    [self updateFunc];
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}


-(void) updateFunc {
    gfAngle += 1.0f;
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
}


- (void)onDoubleTap:(UITapGestureRecognizer *)gr {
    
    // code
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
    
    if (vao_pyramid) {
        glDeleteVertexArrays(1, &vao_pyramid);
        vao_pyramid = 0;
    }
    
    if (vao_cube) {
        glDeleteVertexArrays(1, &vao_cube);
        vao_cube = 0;
    }
    
    if (vbo_position_pyramid) {
        glDeleteBuffers(1, &vbo_position_pyramid);
        vbo_position_pyramid = 0;
    }

    if (vbo_textured_pyramid) {
        glDeleteBuffers(1, &vbo_textured_pyramid);
        vbo_textured_pyramid = 0;
    }

    if (vbo_position_cube) {
        glDeleteBuffers(1, &vbo_position_cube);
        vbo_position_cube = 0;
    }

    if (vbo_textured_cube) {
        glDeleteBuffers(1, &vbo_textured_cube);
        vbo_textured_cube = 0;
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
