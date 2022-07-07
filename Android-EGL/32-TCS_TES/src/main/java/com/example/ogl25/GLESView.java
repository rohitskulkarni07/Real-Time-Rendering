package com.example.ogl25;

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

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener {

    private final Context context;
    private GestureDetector gestureDetector;

    private int vertexShaderObject;
    private int fragmentShaderObject;
    private int tesselationControlShaderObject;
    private int tesselationEvaluationShaderObject;
    private int shaderProgramObject;

    private int numberOfSegmentUniform;
    private int numberOfStripsUniform;
    private int lineColorUniform;

    private int uiNumberOfSegment;

    private int[] vao           = new int[1];
    private int[] vbo_position  = new int[1];
    private int[] vbo_color     = new int[1];

    private float x = 0.1f;

    private int mvpUniform;

    private float perspectiveProjectionMatrix[] = new float[16];

    GLESView(Context drawingContext) {
        
        super(drawingContext);
        context = drawingContext;

        //accordingly set EGLContext to current supported version of OpenGL-ES
        setEGLContextClientVersion(3);

        //set Renderer for drawing on the surface view
        setRenderer(this);

        //Render the view only when there is change in the drawing data
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        gestureDetector = new GestureDetector(context, this, null, false);
        gestureDetector.setOnDoubleTapListener(this);
    }

    //overridden method of GLSurfaceView.Renderer(Init code)
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

        //OpenGL-ES version check
        String glesVersion = gl.glGetString(GL10.GL_VERSION);
        System.out.println("RSK: OpenGL-ES Version = "+glesVersion);
        //OpenGL-GLSL version
        String glslVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
        System.out.println("RSK: GLSL Version = "+glslVersion);

        initialize(gl);
    }

    //overridden method of GLSurfaceView.Renderer ( change size)
    @Override
    public void onSurfaceChanged(GL10 unused, int width, int height) {
        resize(width, height);
    }

    //overridden method of GLSurfaceView.Renderer
    @Override
    public void onDrawFrame(GL10 unused) {
        display();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        //code
        //
        // int eventaction = event.getAction();
        if(!gestureDetector.onTouchEvent(event)) {
            super.onTouchEvent(event);
        }
        return(true);
    }

    @Override
    public boolean onDoubleTap(MotionEvent e) {
        uiNumberOfSegment--;
		if (uiNumberOfSegment <= 0) { 
		    uiNumberOfSegment = 1;
		}
        return(true);
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {

        return(true);
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        uiNumberOfSegment++; 
        if(uiNumberOfSegment >= 30) {
            uiNumberOfSegment = 30;
        }
        return(true);
    }

    @Override
    public boolean onDown(MotionEvent e) {
        return(true);
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        return(true);
    }

    @Override
    public void onLongPress(MotionEvent e) {
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        uninitialized();
        System.exit(0);
        return(true);
    }

    @Override
    public void onShowPress(MotionEvent e) { }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return(true);
    }

    private void initialize(GL10 gl) {

        //*******************************************************************
        //                   *** VERTEX SHADER ***
        //*******************************************************************

        // create vertex shader
        vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        // write vertex shader source code
        final String vertexShaderSourceCode = String.format(
            "#version 320 es                                                \n"+
            "precision mediump int;                                         \n"+
            "precision highp float;                                         \n"+
            "                                                               \n"+
            "in vec2 vPosition;                                             \n"+
            "                                                               \n"+
            "void main() {                                                  \n"+
            "   gl_Position = vec4(vPosition, 0.0, 0.0);                    \n"+
            "}                                                              \n");

        // provide source code to Vertex Shader
        GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);

        // compile shader
        GLES32.glCompileShader(vertexShaderObject);

        // error checking
        int [] iShaderCompileStatus = new int[1];
        int [] iInfoLofLength = new int[1];
        String szInfoLog = null;

        GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
        if(iShaderCompileStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH,iInfoLofLength,0);
            if(iInfoLofLength[0] > 0) {

                szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
                System.out.println("RSK: Vertex Shader Compilation Log = "+ szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        //*******************************************************************
        //                   *** TESSELATION CONTROL SHADER ***
        //*******************************************************************
        
        // create tess control shader
        tesselationControlShaderObject = GLES32.glCreateShader(GLES32.GL_TESS_CONTROL_SHADER);

        // write tess control shader source code
        final String tesselationControlShaderSourceCode = String.format(
            "#version 320 es                                                                            \n"+
            "precision mediump int;                                                                     \n"+
            "precision highp float;                                                                     \n"+
            "                                                                                           \n"+
            "layout(vertices = 4)out;                                                                   \n"+
            "uniform int u_numberOfStrips;                                                              \n"+
            "uniform int u_numberOfSegment;                                                             \n"+
            "                                                                                           \n"+
            "void main() {                                                                              \n"+
            "                                                                                           \n"+
            "   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;               \n"+
            "   gl_TessLevelOuter[0] = float(u_numberOfStrips);                                         \n"+
            "   gl_TessLevelOuter[1] = float(u_numberOfSegment);                                        \n"+
            "}                                                                                          \n"
        );
        
        // provide source code to Vertex Shader
        GLES32.glShaderSource(tesselationControlShaderObject, tesselationControlShaderSourceCode);

        // compile shader
        GLES32.glCompileShader(tesselationControlShaderObject);

        // error checking
        iShaderCompileStatus[0] = 0;
        iInfoLofLength[0]       = 0;
        szInfoLog               = null;

        
        GLES32.glGetShaderiv(tesselationControlShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
        if(iShaderCompileStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetShaderiv(tesselationControlShaderObject, GLES32.GL_INFO_LOG_LENGTH,iInfoLofLength,0);
            if(iInfoLofLength[0] > 0) {

                szInfoLog = GLES32.glGetShaderInfoLog(tesselationControlShaderObject);
                System.out.println("RSK: Tesselation Control Shader Compilation Log = "+ szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        //*******************************************************************
        //                   *** TESSELATION EVALUATION SHADER ***
        //*******************************************************************

        // create tess eval shader
        tesselationEvaluationShaderObject = GLES32.glCreateShader(GLES32.GL_TESS_EVALUATION_SHADER);

        // write tess eval shader source code
        final String tesselationEvaluationShaderSourceCode = String.format(
            "#version 320 es                                                                            \n"+
            "precision mediump int;                                                                     \n"+
            "precision highp float;                                                                     \n"+
            "                                                                                           \n"+
            "layout(isolines)in;                                                                        \n"+
            "uniform mat4 u_mvp_matrix;                                                                 \n"+
            "                                                                                           \n"+
            "void main() {                                                                              \n"+
            "                                                                                           \n"+
            "   float tessCoord		= gl_TessCoord.x;                                                   \n"+
            "   vec3 p0				= gl_in[0].gl_Position.xyz;                                         \n"+
            "   vec3 p1				= gl_in[1].gl_Position.xyz;                                         \n"+
            "   vec3 p2				= gl_in[2].gl_Position.xyz;                                         \n"+
            "   vec3 p3				= gl_in[3].gl_Position.xyz;                                         \n"+
            "   vec3 p				= p0 * (1.0 - tessCoord) * (1.0 - tessCoord) * (1.0 - tessCoord)    \n"+
            "                       + p1 * 3.0 * tessCoord * (1.0 - tessCoord) * (1.0 - tessCoord)      \n"+
            "                       + p2 * 3.0 * tessCoord * tessCoord * (1.0 - tessCoord)              \n"+
            "                       + p3 * tessCoord * tessCoord * tessCoord;                           \n"+
            "                                                                                           \n"+
            "   gl_Position			= u_mvp_matrix * vec4(p,1.0);                                       \n"+
            "                                                                                           \n"+
            "}                                                                                          \n"
        );
        
        // provide source code to TES
        GLES32.glShaderSource(tesselationEvaluationShaderObject, tesselationEvaluationShaderSourceCode);

        // compile shader
        GLES32.glCompileShader(tesselationEvaluationShaderObject);

        // error checking
        iShaderCompileStatus[0] = 0;
        iInfoLofLength[0]       = 0;
        szInfoLog               = null;

        
        GLES32.glGetShaderiv(tesselationEvaluationShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
        if(iShaderCompileStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetShaderiv(tesselationEvaluationShaderObject, GLES32.GL_INFO_LOG_LENGTH,iInfoLofLength,0);
            if(iInfoLofLength[0] > 0) {

                szInfoLog = GLES32.glGetShaderInfoLog(tesselationEvaluationShaderObject);
                System.out.println("RSK: Tesselation Evaluation Shader Compilation Log = "+ szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }


        //*******************************************************************
        //                      *** FRAGMENT SHADER ***
        //*******************************************************************

        // create shader
        fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        // write shader
        final String fragmentShaderSourceCode = String.format(
            "#version 320 es                                                \n"+
            "                                                               \n"+
            "precision highp float;                                         \n"+
            "out vec4 FragColor;                                            \n"+
            "uniform vec4 u_lineColor;                                      \n"+
            "void main() {                                                  \n"+
            "   FragColor = u_lineColor;                                    \n"+
            "}                                                              \n");

        // provide source code to shader
        GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);

        // compile shader
        GLES32.glCompileShader(fragmentShaderObject);

        // error checking
        iShaderCompileStatus[0] = 0;
        iInfoLofLength[0]       = 0;
        szInfoLog               = null;

        GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
        if(iShaderCompileStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH,iInfoLofLength,0);
            if(iInfoLofLength[0] > 0) {

                szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
                System.out.println("RSK: Fragment Shader Compilation Log = "+ szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        //*******************************************************************
        //                     *** SHADER PROGRAM ***
        //*******************************************************************

        // create shader program
        shaderProgramObject = GLES32.glCreateProgram();

        // attach vertex   shader to shader program
        GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);

        // attach tesselation Control Shader to shader program
        GLES32.glAttachShader(shaderProgramObject, tesselationControlShaderObject);
        
        // attach tesselation evaluation Shader to shader program
        GLES32.glAttachShader(shaderProgramObject, tesselationEvaluationShaderObject);
        
        // attach fragment shader to shader program
        GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

        // pre-linking step binding attributes to shader program
        GLES32.glBindAttribLocation(shaderProgramObject,GLESMacros.RSK_ATTRIBUTE_VERTEX,"vPosition");

        //linking
        GLES32.glLinkProgram(shaderProgramObject);

        //linking error checking
        int[] iShaderProgramLinkStatus = new int[1];
        iInfoLofLength[0]       = 0;
        szInfoLog               = null;

        GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, iShaderProgramLinkStatus, 0);

        if(iShaderProgramLinkStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLofLength, 0);
            if(iInfoLofLength[0] > 0) {
                szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);
                System.out.println("RSK: Shader Program Link Log =" + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        //get uniforms location here
        mvpUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_mvp_matrix");
        lineColorUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_lineColor");
        numberOfStripsUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_numberOfStrips");
        numberOfSegmentUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_numberOfSegment");

        System.out.println("RSK: "+ mvpUniform + " " + numberOfSegmentUniform + " " + " " + numberOfStripsUniform + " " + lineColorUniform);
        // *** vertices, colors, shader attributes, vbo, vao initializations ***

        final float lineVertices[] = new float[] {
            -1.0f, -1.0f,
		    -0.5f,  1.0f,
	 	    0.5f, -1.0f,
		    1.0f,  1.0f
        };
   
        GLES32.glGenVertexArrays(1, vao,0);
        GLES32.glBindVertexArray(vao[0]);

            //_______________________________________________________________________
            ByteBuffer byteBuffer = ByteBuffer.allocateDirect(lineVertices.length*4);
            byteBuffer.order(ByteOrder.nativeOrder());
            FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
            verticesBuffer.put(lineVertices);
            verticesBuffer.position(0);
            //_______________________________________________________________________

            GLES32.glGenBuffers(1,vbo_position,0);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER ,vbo_position[0]);
            GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, lineVertices.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
            GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 2, GLES32.GL_FLOAT, false,0,0);
            GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
      
        GLES32.glBindVertexArray(0);

        uiNumberOfSegment = 1;

        // enable depth testing
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);

        // depth test
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        //set the background color
        GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);

        // set projection matrix to identity
        Matrix.setIdentityM(perspectiveProjectionMatrix,0);
    }

    private void resize(int width, int height) {
        if (height == 0) {
            height = 1;
        }
        //adjust the viewport based on geometry changes
        GLES32.glViewport(0,0,width,height);
        Matrix.perspectiveM(perspectiveProjectionMatrix,0,45.0f,(float)width/(float)height,0.1f,100.0f);
    }

    private void display() {

        // draw background color
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
        float[] color = new float[] { 1.0f, 1.0f, 0.0f, 1.0f};
        
        // use shader program
        GLES32.glUseProgram(shaderProgramObject);
        GLES32.glLineWidth(41.0f);
        
        // set matrix for transformation and projection
        float modelViewMatrix[] = new float[16];
        float modelViewProjectionMatrix[] = new float[16];
        
        // set matrix to identity
        Matrix.setIdentityM(modelViewMatrix,0);
        Matrix.setIdentityM(modelViewProjectionMatrix,0);
        
        Matrix.translateM(modelViewMatrix,0,modelViewMatrix,0,0.0f,0.0f,-3.0f);
        
        // multiply the model view and projection matrix to get mvp matrix
        Matrix.multiplyMM(modelViewProjectionMatrix,0,perspectiveProjectionMatrix,0,modelViewMatrix,0);
        
        // pass mvp matrix to shader
        GLES32.glUniformMatrix4fv(mvpUniform,1,false, modelViewProjectionMatrix,0);
        GLES32.glUniform1i(numberOfSegmentUniform, uiNumberOfSegment);
	    GLES32.glUniform1i(numberOfStripsUniform, 1);

	    GLES32.glUniform4f(lineColorUniform, (float)Math.sin(x),(float)Math.cos(x), x, 1.0f);
        
        // ByteBuffer byteBuffer = ByteBuffer.allocateDirect(color.length*4);
        // byteBuffer.order(ByteOrder.nativeOrder());
        // FloatBuffer fb_colorBuffer = byteBuffer.asFloatBuffer();
        // fb_colorBuffer.put(color);
        // fb_colorBuffer.position(0);
	    // GLES32.glUniform4fv(lineColorUniform, 1, fb_colorBuffer);

        x = x + 0.01f;

        // bind vao
        GLES32.glBindVertexArray(vao[0]);

        GLES32.glPatchParameteri(GLES32.GL_PATCH_VERTICES, 4);
        GLES32.glDrawArrays(GLES32.GL_PATCHES, 0, 4);
        // unbind vao
        GLES32.glBindVertexArray(0);

        // stop using shader program
        GLES32.glUseProgram(0);

        //render
        requestRender();

    }

    private void uninitialized() {

        if(vao[0] != 0) {
            GLES32.glDeleteVertexArrays(1, vao,0);
            vao[0] = 0;
        }

        if(vbo_position[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_position, 0);
        }
        
        if(shaderProgramObject != 0) {
            if(fragmentShaderObject != 0) {
                GLES32.glDetachShader(shaderProgramObject, fragmentShaderObject);
                GLES32.glDeleteShader(fragmentShaderObject);
                fragmentShaderObject = 0;
            }
            if(tesselationEvaluationShaderObject!= 0) {
                GLES32.glDetachShader(shaderProgramObject, tesselationEvaluationShaderObject);
                GLES32.glDeleteShader(tesselationEvaluationShaderObject);
                tesselationEvaluationShaderObject = 0;
            }
            if(tesselationControlShaderObject!= 0) {
                GLES32.glDetachShader(shaderProgramObject, tesselationControlShaderObject);
                GLES32.glDeleteShader(tesselationControlShaderObject);
                tesselationControlShaderObject = 0;
            }
            if(vertexShaderObject != 0) {
                GLES32.glDetachShader(shaderProgramObject, fragmentShaderObject);
                GLES32.glDeleteShader(vertexShaderObject);
                fragmentShaderObject = 0;
            }
        }

        if(shaderProgramObject != 0) {
            GLES32.glDeleteProgram(shaderProgramObject);
            shaderProgramObject = 0;
        }
    }

}
