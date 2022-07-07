package com.example.ogl18;

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

    private float angle = 0.0f;

    private int vertexShaderObject;
    private int fragmentShaderObject;
    private int shaderProgramObject;

    private int[] vao_cube           = new int[1];
    private int[] vbo_position_cube  = new int[1];
    private int[] vbo_normal_cube    = new int[1];

    private int ld_uniform;
    private int kd_uniform;

    private int light_position_uniform;

    private boolean bLight;
    private boolean bAnimate;

    private int model_view_matrix_uniform;
    private int perspective_projection_uniform;

    private int lkeypressed_uniform;
    
    private float[] perspectiveProjectionMatrix = new float[16];

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
        if (bLight==true) {
            bLight = false;
        }
        else {
            bLight = true;
        }
        return(true);
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return(true);
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        if (bAnimate == true) {
            bAnimate = false;
        }
        else {
            bAnimate = true;
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

        // *******************************************************************
        // *** VERTEX SHADER ***
        // *******************************************************************

        // create vertex shader
        vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        // write vertex shader source code
        final String vertexShaderSourceCode = String.format(
            "#version 320 es                                                                        \n"+
            "                                                                                       \n"+
            "precision mediump int;                                                                 \n"+
            "                                                                                       \n"+
            "in vec4 vPosition;                                                                     \n"+
            "in vec3 vNormal;                                                                       \n"+
            "                                                                                       \n"+
            "uniform mat4 u_mv_matrix;                                                              \n"+
            "uniform mat4 u_p_matrix;                                                               \n"+
            "uniform int  u_lkeypressed;                                                            \n"+
            "uniform vec3 u_ld;                                                                     \n"+
            "uniform vec3 u_kd;                                                                     \n"+
            "uniform vec4 u_light_position;                                                         \n"+
            "                                                                                       \n"+
            "vec4 eye_coordinates;                                                                  \n"+
            "mat3 normal_matrix;                                                                    \n"+
            "vec3 t_norm;                                                                           \n"+
            "vec3 s;                                                                                \n"+
            "                                                                                       \n"+
            "out vec3 diffuse_light;                                                                \n"+
            "                                                                                       \n"+
            "void main() {                                                                          \n"+
            "                                                                                       \n"+
            "   if(u_lkeypressed == 1) {                                                            \n"+
            "                                                                                       \n"+
            "       eye_coordinates     = u_mv_matrix * vPosition;                                  \n"+
            "       normal_matrix       = mat3(transpose(inverse(u_mv_matrix)));                    \n"+
            "       t_norm              = normalize(normal_matrix * vNormal);                       \n"+
            "       s                   = normalize(vec3(u_light_position - eye_coordinates));      \n"+
            "       diffuse_light       = u_ld * u_kd * max(dot(s, t_norm), 0.0);                   \n"+
            "                                                                                       \n"+
            "   }                                                                                   \n"+
            "                                                                                       \n"+
            "   gl_Position = u_p_matrix * u_mv_matrix * vPosition;                                             \n"+
            "}                                                                                      \n");

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
            "precision mediump int;                                         \n"+
            "                                                               \n"+
            "vec4 color;                                                    \n"+
            "in vec3 diffuse_light;                                         \n"+
            "uniform int u_lkeypressed;                                     \n"+
            "                                                               \n"+
            "out vec4 FragColor;                                            \n"+
            "                                                               \n"+
            "void main() {                                                  \n"+
            "                                                               \n"+
            "   if(u_lkeypressed == 1) {                                    \n"+
            "       color = vec4(diffuse_light, 1.0);                       \n"+
            "   } else {                                                    \n"+
            "       color = vec4(1.0, 1.0, 1.0, 1.0);                       \n"+
            "   }                                                           \n"+
            "                                                               \n"+
            "   FragColor =  color;                                         \n"+
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
        GLES32.glBindAttribLocation(shaderProgramObject, GLESMacros.RSK_ATTRIBUTE_NORMAL, "vNormal");

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

        //get uniforms location here
        model_view_matrix_uniform       = GLES32.glGetUniformLocation(shaderProgramObject,"u_mv_matrix");
        perspective_projection_uniform  = GLES32.glGetUniformLocation(shaderProgramObject,"u_p_matrix");
        lkeypressed_uniform             = GLES32.glGetUniformLocation(shaderProgramObject,"u_lkeypressed");
        light_position_uniform          = GLES32.glGetUniformLocation(shaderProgramObject,"u_light_position");
        ld_uniform                      = GLES32.glGetUniformLocation(shaderProgramObject,"u_ld");
        kd_uniform                      = GLES32.glGetUniformLocation(shaderProgramObject,"u_kd");
       
        // *** vertices, colors, shader attributes, vbo, vao initializations ***

        final float[] cubeVertices = new float[] {
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

        final float[] cubeNormals = new float[] {
            
            //Front Face
            0.0f, 0.0f, 1.0f,	
            0.0f, 0.0f, 1.0f,	
            0.0f, 0.0f, 1.0f,	
            0.0f, 0.0f, 1.0f,	
    
            //Right Face
            1.0f, 0.0f, 0.0f,	
            1.0f, 0.0f, 0.0f,	
            1.0f, 0.0f, 0.0f,	
            1.0f, 0.0f, 0.0f,	
    
            //Back Face
            0.0f, 0.0f, -1.0f,	
            0.0f, 0.0f, -1.0f,	
            0.0f, 0.0f, -1.0f,	
            0.0f, 0.0f, -1.0f,	
            
            //Left Face
            -1.0f, 0.0f, 0.0f,	
            -1.0f, 0.0f, 0.0f,	
            -1.0f, 0.0f, 0.0f,	
            -1.0f, 0.0f, 0.0f,	
            
            //Top Face
            0.0f, 1.0f, 0.0f,	
            0.0f, 1.0f, 0.0f,	
            0.0f, 1.0f, 0.0f,	
            0.0f, 1.0f, 0.0f,	
            
            //Bottom Face
            0.0f, -1.0f, 0.0f,	
            0.0f, -1.0f, 0.0f,	
            0.0f, -1.0f, 0.0f,	
            0.0f, -1.0f, 0.0f	
        };

        GLES32.glGenVertexArrays(1, vao_cube, 0);
        GLES32.glBindVertexArray(vao_cube[0]);

            GLES32.glGenBuffers(1, vbo_position_cube, 0);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_cube[0]);

                // _______________________________________________________________________
                ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cubeVertices.length * 4);
                byteBuffer.order(ByteOrder.nativeOrder());
                FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
                verticesBuffer.put(cubeVertices);
                verticesBuffer.position(0);
                // _______________________________________________________________________

                GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubeVertices.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
                GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false, 0, 0);
                GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);

            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

            GLES32.glGenBuffers(1, vbo_normal_cube, 0);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_normal_cube[0]);

                // _______________________________________________________________________
                byteBuffer = ByteBuffer.allocateDirect(cubeNormals.length * 4);
                byteBuffer.order(ByteOrder.nativeOrder());
                verticesBuffer = byteBuffer.asFloatBuffer();
                verticesBuffer.put(cubeNormals);
                verticesBuffer.position(0);
                // _______________________________________________________________________

                GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubeNormals.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
                GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
                GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_NORMAL);

            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);


        bLight = false;
	    bAnimate = false;

        // enable depth testing
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);

        // depth test
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // set the background color
        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // set projection matrix to identity
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
    }

    private void resize(int width, int height) {
        if (height == 0) {
            height = 1;
        }
        // adjust the viewport based on geometry changes
        GLES32.glViewport(0, 0, width, height);
        Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float) width / (float) height, 0.1f, 100.0f);
    }

    private void display() {

        
        // set matrix for transformation and projection
        float[] modelViewMatrix = new float[16];
        
        // set matrix to identity
        Matrix.setIdentityM(modelViewMatrix, 0);

        Matrix.translateM(modelViewMatrix, 0, modelViewMatrix, 0, 0.0f, 0.0f, -4.5f);
        Matrix.scaleM(modelViewMatrix, 0, modelViewMatrix, 0, 0.75f, 0.75f, 0.75f);
        Matrix.rotateM(modelViewMatrix, 0, angle, 1.0f, 0.0f, 0.0f);
        Matrix.rotateM(modelViewMatrix, 0, angle, 0.0f, 1.0f, 0.0f);
        Matrix.rotateM(modelViewMatrix, 0, angle, 0.0f, 0.0f, 1.0f);

        // draw background color
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
        
        // use shader program
        GLES32.glUseProgram(shaderProgramObject);

        if(bLight == true) {

            GLES32.glUniform1i(lkeypressed_uniform, 1);
            GLES32.glUniform3f(ld_uniform, 1.0f, 1.0f, 1.0f);   // light diffuse component 
            GLES32.glUniform3f(kd_uniform, 0.65f, 0.65f, 0.65f);   // material component

            GLES32.glUniform4f(light_position_uniform, 2.0f, 1.0f, 2.0f, 1.0f);   //light position component
        
        } else {

            GLES32.glUniform1i(lkeypressed_uniform, 0);

        }

        // pass mvp matrix to shader
        GLES32.glUniformMatrix4fv(model_view_matrix_uniform, 1, false, modelViewMatrix, 0);
        GLES32.glUniformMatrix4fv(perspective_projection_uniform, 1, false, perspectiveProjectionMatrix, 0);

        // bind vao
        GLES32.glBindVertexArray(vao_cube[0]);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // stop using shader program
        GLES32.glUseProgram(0);

        update();

        // render
        requestRender();

    }

    private void update() {
        if(bAnimate ==  true) {
            angle += 0.50f;
        }

    }

    private void uninitialized() {

        if (vao_cube[0] != 0) {
            GLES32.glDeleteVertexArrays(1, vao_cube, 0);
            vao_cube[0] = 0;
        }

        if (vbo_position_cube[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_position_cube, 0);
        }

        if (vbo_normal_cube[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_normal_cube, 0);
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
