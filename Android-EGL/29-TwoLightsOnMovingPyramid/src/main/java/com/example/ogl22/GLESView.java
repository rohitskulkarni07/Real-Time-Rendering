package com.example.ogl22;

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

    float[] light_ambient_red  = new float[] { 0.0f, 0.0f, 0.0f};
    float[] light_diffuse_red  = new float[] { 1.0f, 0.0f, 0.0f};
    float[] light_specular_red = new float[] { 1.0f, 0.0f, 0.0f};
    float[] light_position_red = new float[] { -2.0f, 0.0f, 0.0f, 1.0f};

    float[] light_ambient_blue  = new float[] { 0.0f, 0.0f, 0.0f};
    float[] light_diffuse_blue  = new float[] { 0.0f, 0.0f, 1.0f};
    float[] light_specular_blue = new float[] { 0.0f, 0.0f, 1.0f};
    float[] light_position_blue = new float[] { 2.0f, 0.0f, 0.0f, 1.0f};


    float[] material_ambient  = new float[] {0.1f, 0.1f, 0.1f}; 
    float[] material_diffuse  = new float[] {0.5f, 0.2f, 0.7f};
    float[] material_specular = new float[] {0.7f, 0.7f, 0.7f};
    float material_shininess  = 128.0f;
    
    private int vertexShaderObject_PV;
    private int fragmentShaderObject_PV;
    private int shaderProgramObject_PV;

    private int[] vao_pyramid           = new int[1];
    private int[] vbo_position_pyramid  = new int[1];
    private int[] vbo_normals_pyramid     = new int[1];

    private int[] la_uniform_PV = new int[2];
    private int[] ld_uniform_PV = new int[2];
    private int[] ls_uniform_PV = new int[2];
    private int[] lightPosition_uniform_PV = new int[2];

    private int ka_uniform_PV;
    private int kd_uniform_PV;
    private int ks_uniform_PV;
    private int shininess_uniform_PV;


    private boolean bLight;
    private boolean bAnimate;

    private int modelMatrix_uniform_PV;
    private int viewMatrix_uniform_PV;
    private int perspectiveProjectionMatrix_uniform_PV;

    private int lkeypressed_uniform_PV;
    
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

        Sphere sphere = new Sphere();

        // *******************************************************************
        // *** VERTEX SHADER ***
        // *******************************************************************

        // create vertex shader
        vertexShaderObject_PV = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        // write vertex shader source code
        final String vertexShaderSourceCode = String.format(
            "#version 320 es                                                                                                    \n"+
            "                                                                                                                   \n"+
            "precision highp float;                                                                                             \n"+
            "precision mediump int;                                                                                             \n"+
            "                                                                                                                   \n"+
            "in vec4 vPosition;                                                                                                 \n"+
            "in vec3 vNormal;                                                                                                   \n"+
            "                                                                                                                   \n"+
            "uniform mat4 u_m_matrix;                                                                                           \n"+
            "uniform mat4 u_v_matrix;                                                                                           \n"+
            "uniform mat4 u_p_matrix;                                                                                           \n"+
            "                                                                                                                   \n"+
            "uniform int  u_lkeypressed;                                                                                        \n"+
            "                                                                                                                   \n"+
            "uniform vec3 u_la[2];                                                                                              \n"+
            "uniform vec3 u_ld[2];                                                                                              \n"+
            "uniform vec3 u_ls[2];                                                                                              \n"+
            "uniform vec4 u_light_position[2];                                                                                  \n"+
            "                                                                                                                   \n"+
            "uniform vec3 u_ka;                                                                                                 \n"+
            "uniform vec3 u_kd;                                                                                                 \n"+
            "uniform vec3 u_ks;                                                                                                 \n"+
            "uniform float u_shininess;                                                                                         \n"+
            "                                                                                                                   \n"+
            "out vec3 phong_ads_light;                                                                                          \n"+
            "                                                                                                                   \n"+
            "vec4 eye_coordinates;                                                                                              \n"+
            "vec3 transformed_normal;                                                                                           \n"+
            "vec3 light_direction;                                                                                              \n"+
            "vec3 reflection_vector;                                                                                            \n"+
            "vec3 view_vector;                                                                                                  \n"+
            "vec3 ambient;                                                                                                      \n"+
            "vec3 diffuse;                                                                                                      \n"+
            "vec3 specular;                                                                                                     \n"+
            "                                                                                                                   \n"+
            "void main() {                                                                                                      \n"+
            "                                                                                                                   \n"+
            "   int i = 0;                                                                                                      \n"+
            "                                                                                                                   \n"+
            "   if(u_lkeypressed == 1) {                                                                                        \n"+
            "                                                                                                                   \n"+
            "       eye_coordinates     = u_v_matrix * u_m_matrix * vPosition;                                                  \n"+
            "       transformed_normal  = normalize(mat3(u_v_matrix * u_m_matrix) * vNormal);                                   \n"+
            "       view_vector         = normalize(-eye_coordinates.xyz);                                                      \n"+
            "                                                                                                                   \n"+
            "       for(i = 0; i < 2; i++) {                                                                                    \n"+
            "                                                                                                                   \n"+
            "           light_direction     = normalize(vec3(u_light_position[i] - eye_coordinates));                           \n"+
            "           reflection_vector   = reflect(-light_direction, transformed_normal);                                    \n"+
            "           ambient             = u_la[i] * u_ka;                                                                   \n"+
            "           diffuse             = u_ld[i] * u_kd * max(dot(light_direction, transformed_normal), 0.0);              \n"+
            "           specular            = u_ls[i] * u_ks * pow(max(dot(reflection_vector, view_vector), 0.0), u_shininess); \n"+
            "           phong_ads_light     = phong_ads_light + ambient + diffuse + specular;                                   \n"+
            "                                                                                                                   \n"+
            "       }                                                                                                           \n"+
            "                                                                                                                   \n"+
            "   } else {                                                                                                        \n"+
            "       phong_ads_light     = vec3(1.0, 1.0, 1.0);                                                                  \n"+
            "   }                                                                                                               \n"+
            "   gl_Position = u_p_matrix * u_v_matrix * u_m_matrix * vPosition;                                                 \n"+
            "}                                                                                                                  \n");

        // provide source code to Vertex Shader
        GLES32.glShaderSource(vertexShaderObject_PV, vertexShaderSourceCode);

        // compile shader
        GLES32.glCompileShader(vertexShaderObject_PV);

        // error checking
        int[] iShaderCompileStatus = new int[1];
        int[] iInfoLofLength = new int[1];
        String szInfoLog = null;

        GLES32.glGetShaderiv(vertexShaderObject_PV, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
        if (iShaderCompileStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetShaderiv(vertexShaderObject_PV, GLES32.GL_INFO_LOG_LENGTH, iInfoLofLength, 0);
            if (iInfoLofLength[0] > 0) {

                szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject_PV);
                System.out.println("RSK: Vertex Shader Compilation Log = " + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }


        //*******************************************************************
        //                      *** FRAGMENT SHADER ***
        //*******************************************************************

        // create shader
        fragmentShaderObject_PV = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        // write shader
        final String fragmentShaderSourceCode = String.format(
            "#version 320 es                                                \n"+
            "                                                               \n"+
            "precision highp float;                                         \n"+
            "precision mediump int;                                         \n"+
            "                                                               \n"+
            "in vec3 phong_ads_light;                                       \n"+
            "out vec4 FragColor;                                            \n"+
            "                                                               \n"+
            "void main() {                                                  \n"+
            "                                                               \n"+
            "   FragColor = vec4(phong_ads_light, 1.0);                     \n"+
            "                                                               \n"+
            "}                                                              \n");

        // provide source code to shader
        GLES32.glShaderSource(fragmentShaderObject_PV, fragmentShaderSourceCode);

        // compile shader
        GLES32.glCompileShader(fragmentShaderObject_PV);

        // error checking
        iShaderCompileStatus[0] = 0;
        iInfoLofLength[0] = 0;
        szInfoLog = null;

        GLES32.glGetShaderiv(fragmentShaderObject_PV, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
        if (iShaderCompileStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetShaderiv(fragmentShaderObject_PV, GLES32.GL_INFO_LOG_LENGTH, iInfoLofLength, 0);
            if (iInfoLofLength[0] > 0) {

                szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject_PV);
                System.out.println("RSK: Fragment Shader Compilation Log = " + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        // *******************************************************************
        // *** SHADER PROGRAM ***
        // *******************************************************************

        // create shader program
        shaderProgramObject_PV = GLES32.glCreateProgram();

        // attach vertex shader to shader program
        GLES32.glAttachShader(shaderProgramObject_PV, vertexShaderObject_PV);

        // attach fragment shader to shader program
        GLES32.glAttachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

        // pre-linking step binding attributes to shader program
        GLES32.glBindAttribLocation(shaderProgramObject_PV, GLESMacros.RSK_ATTRIBUTE_VERTEX, "vPosition");
        GLES32.glBindAttribLocation(shaderProgramObject_PV, GLESMacros.RSK_ATTRIBUTE_NORMAL, "vNormal");

        // linking
        GLES32.glLinkProgram(shaderProgramObject_PV);

        // linking error checking
        int[] iShaderProgramLinkStatus = new int[1];
        iInfoLofLength[0] = 0;
        szInfoLog = null;

        GLES32.glGetProgramiv(shaderProgramObject_PV, GLES32.GL_LINK_STATUS, iShaderProgramLinkStatus, 0);

        if (iShaderProgramLinkStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetProgramiv(shaderProgramObject_PV, GLES32.GL_INFO_LOG_LENGTH, iInfoLofLength, 0);
            if (iInfoLofLength[0] > 0) {
                szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject_PV);
                System.out.println("RSK: Shader Program Link LOg =" + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        //get uniforms location here
        modelMatrix_uniform_PV                 = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_m_matrix");
        viewMatrix_uniform_PV                  = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_v_matrix");
        perspectiveProjectionMatrix_uniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_p_matrix");
        lkeypressed_uniform_PV                 = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_lkeypressed");
        
        la_uniform_PV[0]                          = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_la[0]");
        ld_uniform_PV[0]                          = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_ld[0]");
        ls_uniform_PV[0]                          = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_ls[0]");
        lightPosition_uniform_PV[0]               = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_light_position[0]");

        la_uniform_PV[1]                          = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_la[1]");
        ld_uniform_PV[1]                          = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_ld[1]");
        ls_uniform_PV[1]                          = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_ls[1]");
        lightPosition_uniform_PV[1]               = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_light_position[1]");

        ka_uniform_PV                          = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_ka");
        kd_uniform_PV                          = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_kd");
        ks_uniform_PV                          = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_ks");
        shininess_uniform_PV                   = GLES32.glGetUniformLocation(shaderProgramObject_PV,"u_shininess");
       
        // *** vertices, colors, shader attributes, vbo, vao initializations ***

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

          final float[] pyramidNormals = new float[] {
            // front face
            0.0f, 0.447214f, 0.894427f,
            0.0f, 0.447214f, 0.894427f,
            0.0f, 0.447214f, 0.894427f,
            
            // right face
            0.894427f, 0.447214f, 0.0f,
            0.894427f, 0.447214f, 0.0f,
            0.894427f, 0.447214f, 0.0f,
            
            // back face
            0.0f, 0.447214f, -0.894427f,
            0.0f, 0.447214f, -0.894427f,
            0.0f, 0.447214f, -0.894427f,
            
            // left face
            -0.894427f, 0.447214f, 0.0f,
            -0.894427f, 0.447214f, 0.0f,
            -0.894427f, 0.447214f, 0.0f
          };
    

        GLES32.glGenVertexArrays(1, vao_pyramid,0);
        GLES32.glBindVertexArray(vao_pyramid[0]);

            GLES32.glGenBuffers(1,vbo_position_pyramid,0);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER ,vbo_position_pyramid[0]);

            //_______________________________________________________________________
            ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pyramidVertices.length*4);
            byteBuffer.order(ByteOrder.nativeOrder());
            FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
            verticesBuffer.put(pyramidVertices);
            verticesBuffer.position(0);
            //_______________________________________________________________________

            GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramidVertices.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
            GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false,0,0);
            GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

            GLES32.glGenBuffers(1,vbo_normals_pyramid,0);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER ,vbo_normals_pyramid[0]);

            //_______________________________________________________________________
            byteBuffer = ByteBuffer.allocateDirect(pyramidNormals.length*4);
            byteBuffer.order(ByteOrder.nativeOrder());
            verticesBuffer = byteBuffer.asFloatBuffer();
            verticesBuffer.put(pyramidNormals);
            verticesBuffer.position(0);
            //_______________________________________________________________________

            GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramidNormals.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
            GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false,0,0);
            GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_NORMAL);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
            
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
        float[] modelMatrix = new float[16];
        float[] viewMatrix = new float[16];
        
        // set matrix to identity
        Matrix.setIdentityM(modelMatrix, 0);
        Matrix.setIdentityM(viewMatrix, 0);

        Matrix.translateM(modelMatrix, 0, modelMatrix, 0, 0.0f, 0.0f, -4.5f);
        Matrix.rotateM(modelMatrix, 0, angle, 0.0f, 1.0f, 0.0f);

        // draw background color
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
        
        // use shader program
        GLES32.glUseProgram(shaderProgramObject_PV);

        if(bLight == true) {

            GLES32.glUniform1i(lkeypressed_uniform_PV, 1);

            GLES32.glUniform3fv(la_uniform_PV[0], 1, light_ambient_red, 0);  
            GLES32.glUniform3fv(ld_uniform_PV[0], 1, light_diffuse_red, 0);  
            GLES32.glUniform3fv(ls_uniform_PV[0], 1, light_specular_red, 0); 

            GLES32.glUniform4fv(lightPosition_uniform_PV[0], 1, light_position_red, 0);
            
            
            GLES32.glUniform3fv(la_uniform_PV[1], 1, light_ambient_blue, 0);  
            GLES32.glUniform3fv(ld_uniform_PV[1], 1, light_diffuse_blue, 0);  
            GLES32.glUniform3fv(ls_uniform_PV[1], 1, light_specular_blue, 0); 

            GLES32.glUniform4fv(lightPosition_uniform_PV[1], 1, light_position_blue, 0);
            
            GLES32.glUniform3fv(ka_uniform_PV, 1, material_ambient, 0);   
            GLES32.glUniform3fv(kd_uniform_PV, 1, material_diffuse, 0);   
            GLES32.glUniform3fv(ks_uniform_PV, 1, material_specular, 0);  
            GLES32.glUniform1f(shininess_uniform_PV, material_shininess);

        
        } else {

            GLES32.glUniform1i(lkeypressed_uniform_PV, 0);

        }

        // pass mvp matrix to shader
        GLES32.glUniformMatrix4fv(modelMatrix_uniform_PV, 1, false, modelMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrix_uniform_PV, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(perspectiveProjectionMatrix_uniform_PV, 1, false, perspectiveProjectionMatrix, 0);

        // bind vao
        GLES32.glBindVertexArray(vao_pyramid[0]);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 12);
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

       
        if(vao_pyramid[0] != 0) {
            GLES32.glDeleteVertexArrays(1, vao_pyramid,0);
            vao_pyramid[0] = 0;
        }

        if(vbo_position_pyramid[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_position_pyramid, 0);
            vbo_position_pyramid[0] = 0;
        }

        if(vbo_normals_pyramid[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_normals_pyramid, 0);
            vbo_normals_pyramid[0] = 1;
        }

        if (shaderProgramObject_PV != 0) {
            if (fragmentShaderObject_PV != 0) {
                GLES32.glDetachShader(shaderProgramObject_PV, fragmentShaderObject_PV);
                GLES32.glDeleteShader(fragmentShaderObject_PV);
                fragmentShaderObject_PV = 0;
            }
            if (vertexShaderObject_PV != 0) {
                GLES32.glDetachShader(shaderProgramObject_PV, fragmentShaderObject_PV);
                GLES32.glDeleteShader(vertexShaderObject_PV);
                fragmentShaderObject_PV = 0;
            }
        }

        if (shaderProgramObject_PV != 0) {
            GLES32.glDeleteProgram(shaderProgramObject_PV);
            shaderProgramObject_PV = 0;
        }
    }
}
