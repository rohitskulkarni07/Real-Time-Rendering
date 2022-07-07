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
            "                                                   \n" \
            "in vec4 vPosition;                                 \n" \
            "uniform mat4 u_mvp_matrix;                         \n" \
            "                                                   \n" \
            "void main(void)                                    \n" \
            "{                                                  \n" \
            "                                                   \n" \
            "   gl_Position = u_mvp_matrix * vPosition;         \n" \
            "                                                   \n" \
            "}";//endOfCode

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
            "out vec4 FragColor;                            \n" \
            "                                               \n" \
            "void main(void)                                \n" \
            "{                                              \n" \
            "                                               \n" \
            "   FragColor = vec4(1.0, 1.0, 1.0, 1.0);       \n" \
            "                                               \n" \
            "}"; //endOfCode

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
        mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");
        
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

            // //for Geometry Normals Properties
            // glGenBuffers(1, &vbo_sphere_normals);
            // glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_normals);
            // glBufferData(GL_ARRAY_BUFFER, sphere_data.numberOfVertices * 3 * sizeof(GLfloat), sphere_data.sphere_normals, GL_STATIC_DRAW);
            // glVertexAttribPointer(RSK_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            // glEnableVertexAttribArray(RSK_ATTRIBUTE_NORMAL);
            // glBindBuffer(GL_ARRAY_BUFFER, 0);
        
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
    
    //set model-view & model-view-projection matrices to identity
    mat4 modelViewMatrix = mat4::identity();
    mat4 modelViewProjectionMatrix = mat4::identity();
    
    // calculate model matrix
    modelViewMatrix = vmath::translate(0.0f, 0.0f, -6.0f) * vmath::rotate(gfAngle, 1.0f, 0.0f, 0.0f) * vmath::rotate(gfAngle, 0.0f, 1.0f, 0.0f) * vmath::rotate(gfAngle, 0.0f, 0.0f, 1.0f);

    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    // pass above modelViewProjection matrix to the vertex shader in shader variable
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    glBindVertexArray(vao_sphere);

    /*** Bind Array Elements ***/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_elements);

    // *** draw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glDrawElements(GL_TRIANGLES, sphere_data.numberOfIndices, GL_UNSIGNED_INT, 0);

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
