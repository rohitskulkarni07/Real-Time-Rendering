package com.example.framebuffer;

import android.content.Context;
import android.opengl.GLSurfaceView; /*For OpenGL Surface view and all Related type GL10*/
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import android.opengl.GLES32;

import android.opengl.Matrix;
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

//for texture
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;


public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener {

    private final Context context;
    private GestureDetector gestureDetector;

    private int vertexShaderObject;
    private int fragmentShaderObject;
    private int shaderProgramObject;

    private int[] framebufferObject = new int[1];
    private int[] colorTextureAttachment = new int[1];
    private int[] depthTextureAttachment = new int[1];

    private int gWidth = 0;
    private int gHeight = 0;

    private float angle;

    private int[] vao_cube = new int[1];
    private int[] vbo_position_quad = new int[1];
    private int[] vbo_texture_quad = new int[1];

    private int[] vao_pyramid = new int[1];
    private int[] vbo_position_pyramid = new int[1];
    private int[] vbo_texture_pyramid = new int[1];

    private int[] stone_texture = new int[1];
    private int[] kundali_texture = new int[1];

    private int mvpUniform;
    private int texture_sampler_uniform;
    
    private float perspectiveProjectionMatrix[] = new float[16];

    GLESView(Context drawingContext) {

        super(drawingContext);
        context = drawingContext;

        // accordingly set EGLContext to current supported version of OpenGL-ES
        setEGLContextClientVersion(3);

        // set Renderer for drawing on the surface view
        setRenderer(this);

        // Render the view only when there is change in the drawing data
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        gestureDetector = new GestureDetector(context, this, null, false);
        gestureDetector.setOnDoubleTapListener(this);
    }

    // overridden method of GLSurfaceView.Renderer(Init code)
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

        // OpenGL-ES version check
        String glesVersion = gl.glGetString(GL10.GL_VERSION);
        System.out.println("RSK: OpenGL-ES Version = " + glesVersion);
        // OpenGL-GLSL version
        String glslVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
        System.out.println("RSK: GLSL Version = " + glslVersion);

        initialize(gl);
    }

    // overridden method of GLSurfaceView.Renderer ( change size)
    @Override
    public void onSurfaceChanged(GL10 unused, int width, int height) {
        resize(width, height);
    }

    // overridden method of GLSurfaceView.Renderer
    @Override
    public void onDrawFrame(GL10 unused) {
        display();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        // code
        //
        // int eventaction = event.getAction();
        if (!gestureDetector.onTouchEvent(event)) {
            super.onTouchEvent(event);
        }
        return (true);
    }

    @Override
    public boolean onDoubleTap(MotionEvent e) {
        return (true);
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return (true);
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        return (true);
    }

    @Override
    public boolean onDown(MotionEvent e) {
        return (true);
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        return (true);
    }

    @Override
    public void onLongPress(MotionEvent e) {
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        uninitialized();
        System.exit(0);
        return (true);
    }

    @Override
    public void onShowPress(MotionEvent e) {
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return (true);
    }

    private void initialize(GL10 gl) {

        // *******************************************************************
        // *** VERTEX SHADER ***
        // *******************************************************************

        // create vertex shader
        vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        // write vertex shader source code
        final String vertexShaderSourceCode = String.format(
            "#version 320 es                                                \n"+
            "                                                               \n"+
            "in vec4 vPosition;                                             \n"+
            "in vec2 vTexCoord;                                             \n"+
            "uniform mat4 u_mvp_matrix;                                     \n"+
            "                                                               \n"+
            "out vec2 out_texcoord;                                         \n"+
            "                                                               \n"+
            "void main() {                                                  \n"+
            "                                                               \n"+
            "   gl_Position = u_mvp_matrix * vPosition;                     \n"+
            "   out_texcoord = vTexCoord;                                   \n"+
            "                                                               \n"+
            "}                                                              \n");

        // provide source code to Vertex Shader
        GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);

        // compile shader
        GLES32.glCompileShader(vertexShaderObject);

        // error checking
        int[] iShaderCompileStatus = new int[1];
        int[] iInfoLofLength = new int[1];
        String szInfoLog = null;

        GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
        if (iShaderCompileStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLofLength, 0);
            if (iInfoLofLength[0] > 0) {

                szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
                System.out.println("RSK: Vertex Shader Compilation Log = " + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        // *******************************************************************
        // *** FRAGMENT SHADER ***
        // *******************************************************************

        // create shader
        fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        // write shader
        final String fragmentShaderSourceCode = String.format(
            "#version 320 es                                                \n"+
            "                                                               \n"+
            "precision highp float;                                         \n"+
            "                                                               \n"+
            "in vec2 out_texcoord;                                          \n"+
            "uniform highp sampler2D u_texture_sampler;                     \n"+
            "out vec4 FragColor;                                            \n"+
            "                                                               \n"+
            "void main() {                                                  \n"+
            "                                                               \n"+
            "   FragColor = texture(u_texture_sampler, out_texcoord);       \n"+
            "                                                               \n"+
            "}                                                              \n");

        // provide source code to shader
        GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);

        // compile shader
        GLES32.glCompileShader(fragmentShaderObject);

        // error checking
        iShaderCompileStatus[0] = 0;
        iInfoLofLength[0] = 0;
        szInfoLog = null;

        GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
        if (iShaderCompileStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLofLength, 0);
            if (iInfoLofLength[0] > 0) {

                szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
                System.out.println("RSK: Fragment Shader Compilation Log = " + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        // *******************************************************************
        // *** SHADER PROGRAM ***
        // *******************************************************************

        // create shader program
        shaderProgramObject = GLES32.glCreateProgram();

        // attach vertex shader to shader program
        GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);

        // attach fragment shader to shader program
        GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

        // pre-linking step binding attributes to shader program
        GLES32.glBindAttribLocation(shaderProgramObject, GLESMacros.RSK_ATTRIBUTE_VERTEX, "vPosition");
        GLES32.glBindAttribLocation(shaderProgramObject, GLESMacros.RSK_ATTRIBUTE_TEXTURE0, "vTexCoord");

        // linking
        GLES32.glLinkProgram(shaderProgramObject);

        // linking error checking
        int[] iShaderProgramLinkStatus = new int[1];
        iInfoLofLength[0] = 0;
        szInfoLog = null;

        GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, iShaderProgramLinkStatus, 0);

        if (iShaderProgramLinkStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLofLength, 0);
            if (iInfoLofLength[0] > 0) {
                szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);
                System.out.println("RSK: Shader Program Link LOg =" + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        // get uniforms location here
        mvpUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");
        texture_sampler_uniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_texture_sampler");
        // *** vertices, colors, shader attributes, vbo, vao initializations ***

        final float cubeVertices[] = new float[] {
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

        final float cubeTexCoord2f[] = new float[] {
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

        
        final float[] pyramidVertices = new float[] {
           
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
 
        final float[] pyramidTexCoord2f =  new float[] {
            //  front face
            0.5f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            
            //  right face
            0.5f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            
            //  back face
            0.5f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            
            //  left face
            0.5f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f
        };


        // ______________________________CUBE____________________________________________________________

        GLES32.glGenVertexArrays(1, vao_cube, 0);
        GLES32.glBindVertexArray(vao_cube[0]);

        // _______________________________________________________________________
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cubeVertices.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer verticesBuffer_quad = byteBuffer.asFloatBuffer();
        verticesBuffer_quad.put(cubeVertices);
        verticesBuffer_quad.position(0);
        // _______________________________________________________________________

        GLES32.glGenBuffers(1, vbo_position_quad, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_quad[0]);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubeVertices.length * 4, verticesBuffer_quad,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
        
        // _______________________________________________________________________
        byteBuffer = ByteBuffer.allocateDirect(cubeTexCoord2f.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer_quad = byteBuffer.asFloatBuffer();
        verticesBuffer_quad.put(cubeTexCoord2f);
        verticesBuffer_quad.position(0);
        // _______________________________________________________________________
        
        GLES32.glGenBuffers(1, vbo_texture_quad, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texture_quad[0]);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubeTexCoord2f.length * 4, verticesBuffer_quad,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_TEXTURE0, 2, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_TEXTURE0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
        GLES32.glBindVertexArray(0);

        // ______________________________PYRAMID____________________________________________________________

        GLES32.glGenVertexArrays(1, vao_pyramid, 0);
        GLES32.glBindVertexArray(vao_pyramid[0]);

        // _______________________________________________________________________
        byteBuffer = ByteBuffer.allocateDirect(pyramidVertices.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer_quad = byteBuffer.asFloatBuffer();
        verticesBuffer_quad.put(pyramidVertices);
        verticesBuffer_quad.position(0);
        // _______________________________________________________________________

        GLES32.glGenBuffers(1, vbo_position_pyramid, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_pyramid[0]);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramidVertices.length * 4, verticesBuffer_quad,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
        
        // _______________________________________________________________________
        byteBuffer = ByteBuffer.allocateDirect(pyramidTexCoord2f.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer_quad = byteBuffer.asFloatBuffer();
        verticesBuffer_quad.put(pyramidTexCoord2f);
        verticesBuffer_quad.position(0);
        // _______________________________________________________________________
        
        GLES32.glGenBuffers(1, vbo_texture_pyramid, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texture_pyramid[0]);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramidTexCoord2f.length * 4, verticesBuffer_quad,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_TEXTURE0, 2, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_TEXTURE0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
        GLES32.glBindVertexArray(0);


        stone_texture[0]   = loadGlTexture(R.raw.stone);
        kundali_texture[0] = loadGlTexture(R.raw.kundali);

        //______________________________________________________________________________________
    //                                  FRAMEBUFFER
    //______________________________________________________________________________________

	GLES32.glGenFramebuffers(1, framebufferObject, 0);
	GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, framebufferObject[0]);
	
		// Color buffer
		GLES32.glGenTextures(1, colorTextureAttachment, 0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, colorTextureAttachment[0]);

		GLES32.glTexImage2D(GLES32.GL_TEXTURE_2D, 0, GLES32.GL_RGB, 800, 600, 0, GLES32.GL_RGB, GLES32.GL_UNSIGNED_BYTE, null);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);  

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

		// Depth buffer
		GLES32.glGenRenderbuffers(1, depthTextureAttachment, 0);
		GLES32.glBindRenderbuffer(GLES32.GL_RENDERBUFFER, depthTextureAttachment[0]);
		GLES32.glRenderbufferStorage(GLES32.GL_RENDERBUFFER, GLES32.GL_DEPTH_COMPONENT, 800, 600);
		GLES32.glBindRenderbuffer(GLES32.GL_RENDERBUFFER, 0);

		// Attachment
		GLES32.glFramebufferTexture2D(GLES32.GL_FRAMEBUFFER, GLES32.GL_COLOR_ATTACHMENT0, GLES32.GL_TEXTURE_2D, colorTextureAttachment[0], 0);
		GLES32.glFramebufferRenderbuffer(GLES32.GL_FRAMEBUFFER, GLES32.GL_DEPTH_ATTACHMENT, GLES32.GL_RENDERBUFFER, depthTextureAttachment[0]); 

		if(GLES32.glCheckFramebufferStatus(GLES32.GL_FRAMEBUFFER) != GLES32.GL_FRAMEBUFFER_COMPLETE) {
            System.out.println("RSK: Shader Program Link LOg =" + szInfoLog);
            uninitialized();
            System.exit(0);
		}
	
	GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, 0); 

        // enable texture 2d
        GLES32.glEnable(GLES32.GL_TEXTURE_2D);

        // enable depth testing
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);

        // depth test
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // set the background color
        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // set projection matrix to identity
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
    }

    private int loadGlTexture(int imageFileResourceID) {

        BitmapFactory.Options options = new BitmapFactory.Options();
        
        //dont scale
        options.inScaled = false;

        Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(), imageFileResourceID, options);
        
        int[] texture = new int[1];

        GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 1);
        GLES32.glGenTextures(1, texture, 0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture[0]);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR_MIPMAP_LINEAR);
        GLUtils.texImage2D(GLES32.GL_TEXTURE_2D, 0, bitmap, 0);
		GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D);

        return(texture[0]);
    }

    private void resize(int width, int height) {
        if (height == 0) {
            height = 1;
        }

        gWidth = width;
        gHeight = height;
        // adjust the viewport based on geometry changes
        Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float) width / (float) height, 0.1f, 100.0f);
    }

    private void display() {
        

        // Declarations
        // set matrix for transformation and projection
       float modelViewMatrix[] = new float[16];
       float modelViewProjectionMatrix[] = new float[16];
   
        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, framebufferObject[0]);

            GLES32.glViewport(0,0, 800, 600);
            GLES32.glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
            GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

            GLES32.glUseProgram(shaderProgramObject);
            
                Matrix.setIdentityM(modelViewMatrix, 0);
                Matrix.setIdentityM(modelViewProjectionMatrix, 0);
                Matrix.translateM(modelViewMatrix, 0, modelViewMatrix, 0, 0.0f, 0.0f, -4.5f);
                Matrix.rotateM(modelViewMatrix, 0, modelViewMatrix, 0, angle, 0.0f, 1.0f, 0.0f);
                Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);
                
                // pass mvp matrix to shader
                GLES32.glUniformMatrix4fv(mvpUniform, 1, false, modelViewProjectionMatrix, 0);
            
                GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
                GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, stone_texture[0]);		//change state to texture for next upcoming geometry
                GLES32.glUniform1i(texture_sampler_uniform, 0);
    
                GLES32.glBindVertexArray(vao_pyramid[0]);
                    GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 12);      
                GLES32.glBindVertexArray(0);
            
            GLES32.glUseProgram(0);

        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, 0);
        
        GLES32.glViewport(0,0, gWidth, gHeight);
        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
        
        GLES32.glUseProgram(shaderProgramObject);
        
            Matrix.setIdentityM(modelViewMatrix, 0);
            Matrix.setIdentityM(modelViewProjectionMatrix, 0);
            Matrix.translateM(modelViewMatrix, 0, modelViewMatrix, 0, 0.0f, 0.0f, -4.5f);
            Matrix.scaleM(modelViewMatrix, 0, modelViewMatrix, 0, 0.75f, 0.75f, 0.75f);
            Matrix.rotateM(modelViewMatrix, 0, modelViewMatrix, 0, angle, 1.0f, 0.0f, 0.0f);
            Matrix.rotateM(modelViewMatrix, 0, modelViewMatrix, 0, angle, 0.0f, 1.0f, 0.0f);
            Matrix.rotateM(modelViewMatrix, 0, modelViewMatrix, 0, angle, 0.0f, 0.0f, 1.0f);
            Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);
            
            // pass mvp matrix to shader
            GLES32.glUniformMatrix4fv(mvpUniform, 1, false, modelViewProjectionMatrix, 0);
        
            GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
		    GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, colorTextureAttachment[0]);		//change state to texture for next upcoming geometry
		    GLES32.glUniform1i(texture_sampler_uniform, 0);

            GLES32.glBindVertexArray(vao_cube[0]);
        
            GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
            GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);
            GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);
            GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);
            GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);
            GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);
        
            GLES32.glBindVertexArray(0);
            update();

        // stop using shader program
        GLES32.glUseProgram(0);

        // render
        requestRender();

    }

    private void update() {
        angle+=0.5f;
    }

    private void uninitialized() {

        if (vao_cube[0] != 0) {
            GLES32.glDeleteVertexArrays(1, vao_cube, 0);
            vao_cube[0] = 0;
        }

        if (vbo_position_quad[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_position_quad, 0);
            vbo_position_quad[0] = 0;
        }

        if (vbo_texture_quad[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_texture_quad, 0);
            vbo_texture_quad[0] = 0;
        }

        if (vao_pyramid[0] != 0) {
            GLES32.glDeleteVertexArrays(1, vao_pyramid, 0);
            vao_cube[0] = 0;
        }

        if (vbo_position_pyramid[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_position_pyramid, 0);
            vbo_position_pyramid[0] = 0;
        }

        if (vbo_texture_pyramid[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_texture_pyramid, 0);
            vbo_texture_pyramid[0] = 0;
        }

        if(stone_texture[0] != 0) {
            GLES32.glDeleteTextures(1, stone_texture, 0);
            stone_texture[0] = 0;
            
        }

        if(kundali_texture[0] != 0) {
            GLES32.glDeleteTextures(1, kundali_texture, 0);
            kundali_texture[0] = 0;
            
        }

        if(framebufferObject[0] != 0) {
            GLES32.glDeleteFramebuffers(1, framebufferObject, 0);
            framebufferObject[0] = 0;
            
        }

        if (shaderProgramObject != 0) {
            if (fragmentShaderObject != 0) {
                GLES32.glDetachShader(shaderProgramObject, fragmentShaderObject);
                GLES32.glDeleteShader(fragmentShaderObject);
                fragmentShaderObject = 0;
            }
            if (vertexShaderObject != 0) {
                GLES32.glDetachShader(shaderProgramObject, fragmentShaderObject);
                GLES32.glDeleteShader(vertexShaderObject);
                fragmentShaderObject = 0;
            }
        }

        if (shaderProgramObject != 0) {
            GLES32.glDeleteProgram(shaderProgramObject);
            shaderProgramObject = 0;
        }
    }

}
