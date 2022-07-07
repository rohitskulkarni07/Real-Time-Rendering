package com.example.ortho;

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
    private int shaderProgramObject;

    private int[] vao = new int[1];
    private int[] vbo = new int[1];
    private int mvpUniform;

    private float orthographicProjectionMatrix[] = new float[16];

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
        return(true);
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return(true);
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
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
                "#version 320 es"+
                "\n"+
                 "in vec4 vPosition;\n"+
                 "uniform mat4 u_mvp_matrix;\n"+
                 "void main() {\n"+
                    "gl_Position = u_mvp_matrix * vPosition;\n"+
                 "}\n"
        );

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
        //                      *** FRAGMENT SHADER ***
        //*******************************************************************
        // create shader
        fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        // write shader
        final String fragmentShaderSourceCode = String.format(
                "#version 320 es"+
                "\n"+
                "precision highp float;"+
                "out vec4 FragColor;\n"+
                "void main() {\n"+
                    "FragColor = vec4(1.0,1.0,1.0,1.0);\n"+
                "}\n"
        );

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
                System.out.println("RSK: Shader Program Link LOg =" + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        //get uniforms location here
        mvpUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_mvp_matrix");

        // *** vertices, colors, shader attributes, vbo, vao initializations ***

        final float triangleVertices[] = new float[] {
                 0.0f,   50.0f,  0.0f,
                -50.0f, -50.0f,  0.0f,
                 50.0f, -50.0f,  0.0f
        };

        GLES32.glGenVertexArrays(1, vao,0);
        GLES32.glBindVertexArray(vao[0]);

        GLES32.glGenBuffers(1,vbo,0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER ,vbo[0]);

        //_______________________________________________________________________
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(triangleVertices.length*4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
        verticesBuffer.put(triangleVertices);
        verticesBuffer.position(0);
        //_______________________________________________________________________

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
                triangleVertices.length * 4,
                verticesBuffer,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false,0,0);
        GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
        GLES32.glBindVertexArray(0);

        // enable depth testing
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);

        // depth test
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        //set the background color
        GLES32.glClearColor(0.0f,0.0f,1.0f,1.0f);

        // set projection matrix to identity
        Matrix.setIdentityM(orthographicProjectionMatrix,0);
    }

    private void resize(int width, int height) {
        //adjust the viewport based on geometry changes
        GLES32.glViewport(0,0,width,height);

        if(width <= height) {
            Matrix.orthoM(orthographicProjectionMatrix,0,-100.0f,100.0f,(-100.0f*(height/width)),(100.0f*(height/width)),-100.0f,100.0f);
        } else {
            Matrix.orthoM(orthographicProjectionMatrix,0,(-100.0f*(width/height)),(100.0f*(width/height)),-100.0f,100.0f,-100.0f,100.0f);
        }
    }

    private void display() {

        // draw background color
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        // use shader program
        GLES32.glUseProgram(shaderProgramObject);

        // set matrix for transformation and projection
        float modelViewMatrix[] = new float[16];
        float modelViewProjectionMatrix[] = new float[16];

        // set matrix to identity
        Matrix.setIdentityM(modelViewMatrix,0);
        Matrix.setIdentityM(modelViewProjectionMatrix,0);

        // multiply the model view and projection matrix to get mvp matrix
        Matrix.multiplyMM(modelViewProjectionMatrix,0,orthographicProjectionMatrix,0,modelViewMatrix,0);

        // pass mvp matrix to shader
        GLES32.glUniformMatrix4fv(mvpUniform,1,false, modelViewProjectionMatrix,0);

        // bind vao
        GLES32.glBindVertexArray(vao[0]);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 3);
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

        if(vbo[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo,0);
        }

        if(shaderProgramObject != 0) {
            if(fragmentShaderObject != 0) {
                GLES32.glDetachShader(shaderProgramObject, fragmentShaderObject);
                GLES32.glDeleteShader(fragmentShaderObject);
                fragmentShaderObject = 0;
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
