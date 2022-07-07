package com.example.ogl28;

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

import java.nio.ShortBuffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener {

    private final Context context;
    private GestureDetector gestureDetector;

    private float angle = 0.0f;

    private boolean negativeAnimate = false;
    private boolean positiveAnimate = true;

    private int vertexShaderObject;
    private int fragmentShaderObject;
    private int shaderProgramObject;

    MyStack myStack =  new MyStack(32);

    float[] sphere_vertices =  new float[1146];  
    float[] sphere_normals  =  new float[1146]; 
    float[] sphere_textures =  new float[764];
    short[] sphere_elements =  new short[2280];

    int numVertices;
    int numElements;

    private int[] vao_sphere           = new int[1];
    private int[] vbo_position_sphere  = new int[1];
    private int[] vbo_normal_sphere    = new int[1];
    private int[] vbo_element_sphere   = new int[1];
    private int[] vbo_texture_sphere   = new int[1];

    float elbow    = 0.0f;
    float shoulder = 0.0f;
  
    private int mUniform;
    private int vUniform;
    private int pUniform;
    private int colorUniform;

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
        return(true);
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return(true);
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        if(!negativeAnimate){
            negativeAnimate = true;
        }else {
            negativeAnimate = false;
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

        

        Sphere sphere = new Sphere();
        
        //*******************************************************************
        //                   *** VERTEX SHADER ***
        //*******************************************************************

        // create vertex shader
        vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        // write vertex shader source code
        final String vertexShaderSourceCode = String.format(
            "#version 320 es                                                                    \n"+
            "                                                                                   \n"+
            "in vec4 vPosition;                                                                 \n"+
            "uniform mat4 u_m_matrix;                                                           \n"+
            "uniform mat4 u_v_matrix;                                                           \n"+
            "uniform mat4 u_p_matrix;                                                           \n"+
            "                                                                                   \n"+
            "void main() {                                                                      \n"+
            "   gl_Position = u_p_matrix * u_v_matrix * u_m_matrix * vPosition;                 \n"+
            "}                                                                                  \n");

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
            "#version 320 es                                                \n"+
            "                                                               \n"+
            "precision highp float;                                         \n"+
            "uniform vec3 u_color;                                          \n"+
            "out vec4 FragColor;                                            \n"+
            "                                                               \n"+
            "void main() {                                                  \n"+
            "   FragColor =  vec4(u_color, 1.0);                            \n"+
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

        // attach fragment shader to shader program
        GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

        // pre-linking step binding attributes to shader program
        GLES32.glBindAttribLocation(shaderProgramObject,GLESMacros.RSK_ATTRIBUTE_VERTEX,"vPosition");
        GLES32.glBindAttribLocation(shaderProgramObject,GLESMacros.RSK_ATTRIBUTE_COLOR,"vColor");

        //linking
        GLES32.glLinkProgram(shaderProgramObject);

        //linking error checking
        int[] iShaderProgramLinkStatus  = new int[1];
        iInfoLofLength[0]               = 0;
        szInfoLog                       = null;

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
        mUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_m_matrix");
        vUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_v_matrix");
        pUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_p_matrix");

        colorUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_color");
        
        // *** vertices, colors, shader attributes, vbo, vao initializations ***

        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);

        numVertices = sphere.getNumberOfSphereVertices();
        numElements = sphere.getNumberOfSphereElements();

    
    //______________________________________________________________________________
    //                  VAO_SPHERE
    //______________________________________________________________________________

        GLES32.glGenVertexArrays(1, vao_sphere,0);
        GLES32.glBindVertexArray(vao_sphere[0]);

            //______________________________________________________________________________
            //                          POSITION
            //______________________________________________________________________________

            GLES32.glGenBuffers(1,vbo_position_sphere,0);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER ,vbo_position_sphere[0]);

                //_______________________________________________________________________
                ByteBuffer byteBuffer = ByteBuffer.allocateDirect(sphere_vertices.length*4);
                byteBuffer.order(ByteOrder.nativeOrder());
                FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
                verticesBuffer.put(sphere_vertices);
                verticesBuffer.position(0);
                //_______________________________________________________________________

                GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sphere_vertices.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
                GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false, 0, 0);
                GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);

            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

            //______________________________________________________________________________
            //                          NORMAL
            //______________________________________________________________________________

            GLES32.glGenBuffers(1,vbo_normal_sphere,0);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER ,vbo_normal_sphere[0]);
 
                //_______________________________________________________________________
                byteBuffer = ByteBuffer.allocateDirect(sphere_normals.length*4);
                byteBuffer.order(ByteOrder.nativeOrder());
                verticesBuffer = byteBuffer.asFloatBuffer();
                verticesBuffer.put(sphere_normals);
                verticesBuffer.position(0);
                //_______________________________________________________________________

                GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sphere_normals.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
                GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
                GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_NORMAL);

            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
            
            //______________________________________________________________________________
            //                          TEXTURE
            //______________________________________________________________________________
            GLES32.glGenBuffers(1,vbo_texture_sphere,0);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER ,vbo_texture_sphere[0]);
 
                //_______________________________________________________________________
                byteBuffer = ByteBuffer.allocateDirect(sphere_textures.length*4);
                byteBuffer.order(ByteOrder.nativeOrder());
                verticesBuffer = byteBuffer.asFloatBuffer();
                verticesBuffer.put(sphere_textures);
                verticesBuffer.position(0);
                //_______________________________________________________________________

                GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sphere_textures.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
                GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_TEXTURE0, 2, GLES32.GL_FLOAT, false, 0, 0);
                GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_TEXTURE0);

            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
            
            //______________________________________________________________________________
            //                          ELEMENTS
            //______________________________________________________________________________

            GLES32.glGenBuffers(1, vbo_element_sphere,0);
            GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER ,vbo_element_sphere[0]);
 
                //_______________________________________________________________________
                byteBuffer = ByteBuffer.allocateDirect(sphere_elements.length * 2);
                byteBuffer.order(ByteOrder.nativeOrder());
                ShortBuffer elementsBuffer = byteBuffer.asShortBuffer();
                elementsBuffer.put(sphere_elements);
                elementsBuffer.position(0);
                //_______________________________________________________________________

                GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, sphere_elements.length * 2, elementsBuffer, GLES32.GL_STATIC_DRAW);

            GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);

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

        // use shader program
        GLES32.glUseProgram(shaderProgramObject);

        float[] arm_color   = new float[] {0.5f, 0.35f, 0.05f};

        // set matrix for transformation and projection
        float[] modelMatrix = new float[16];
        float[] viewMatrix = new float[16];

        // set matrix to identity
        Matrix.setIdentityM(modelMatrix,0);
        Matrix.setIdentityM(viewMatrix,0);

        Matrix.translateM(modelMatrix, 0, modelMatrix, 0, 0.0f, 0.0f, -2.5f);
        Matrix.setLookAtM(viewMatrix, 0, 0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        // pass mvp matrix to shader
        GLES32.glUniformMatrix4fv(mUniform, 1, false, modelMatrix,0);
        GLES32.glUniformMatrix4fv(vUniform, 1, false, viewMatrix,0);
        GLES32.glUniformMatrix4fv(pUniform, 1, false, perspectiveProjectionMatrix,0);
        GLES32.glUniform3fv(colorUniform, 0, arm_color, 0);

        myStack.pushMatrix(modelMatrix);

            Matrix.rotateM(modelMatrix, 0, modelMatrix, 0, shoulder, 0.0f, 0.0f, 1.0f);
            Matrix.translateM(modelMatrix, 0, modelMatrix, 0, 1.0f, 0.0f, 0.0f);
            GLES32.glUniformMatrix4fv(mUniform, 1, false, modelMatrix, 0);

            myStack.pushMatrix(modelMatrix);
                
                Matrix.scaleM(modelMatrix, 0, modelMatrix, 0, 2.0f, 0.5f, 1.0f);
                GLES32.glUniformMatrix4fv(mUniform, 1, false, modelMatrix, 0);
                drawSphere();

            myStack.popMatrix(modelMatrix);

            Matrix.translateM(modelMatrix, 0, modelMatrix, 0, 1.0f, 0.0f, 0.0f);
            Matrix.rotateM(modelMatrix, 0, modelMatrix, 0, elbow, 0.0f, 0.0f, 1.0f);
            Matrix.translateM(modelMatrix, 0, modelMatrix, 0, 1.0f, 0.0f, 0.0f);
            GLES32.glUniformMatrix4fv(mUniform, 1, false, modelMatrix, 0);

            myStack.pushMatrix(modelMatrix);

                Matrix.scaleM(modelMatrix, 0, modelMatrix, 0, 2.0f, 0.5f, 1.0f);
                GLES32.glUniformMatrix4fv(mUniform, 1, false, modelMatrix, 0);
                drawSphere();

            myStack.popMatrix(modelMatrix);

        myStack.popMatrix(modelMatrix);

            // stop using shader program
        GLES32.glUseProgram(0);

        update();

        //render
        requestRender();

    }

    private void drawSphere() {
        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);
        
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER ,vbo_element_sphere[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
        // unbind vao
        GLES32.glBindVertexArray(0);
    }

    private void update() {

		angle-=1.0f;
        if(negativeAnimate) {
            elbow -=1.0f;
            shoulder -=1.0f;
        } else {
            elbow +=1.0f;
            shoulder +=1.0f;
        }
    }
    
    private void uninitialized() {

        if(vao_sphere[0] != 0) {
            GLES32.glDeleteVertexArrays(1, vao_sphere,0);
            vao_sphere[0] = 0;
        }

        if(vbo_position_sphere[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_position_sphere, 0);
            vbo_position_sphere[0] = 0;
        }

        if(vbo_normal_sphere[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_normal_sphere, 0);
            vbo_normal_sphere[0] = 0;
        }
        
        if(vbo_texture_sphere[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_texture_sphere, 0);
            vbo_texture_sphere[0] = 0;
        }

           
        if(vbo_element_sphere[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_element_sphere, 0);
            vbo_element_sphere[0] = 0;
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
