package com.example.ogl24;

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

    float[] light_ambient = new float[] { 0.1f, 0.1f, 0.1f };
    float[] light_diffuse = new float[] { 1.0f, 1.0f, 1.0f };
    float[] light_specular = new float[] { 1.0f, 1.0f, 1.0f };
    float[] light_position = new float[4];

    float[] material_ambient = new float[4];
    float[] material_diffuse = new float[4];
    float[] material_specular = new float[4];
    float material_shininess = 0.0f;

    private float key_pressed = 0;
    private float r = 10.0f;
    private float angleX = 0.0f;
    private float angleY = 0.0f;
    private float angleZ = 0.0f;

    private float tx = 0.0f, ty = -0.425f, tz = -2.89f;
    private float sx = 0.99f, sy = 0.99f, sz = 0.99f;

    private int giWidth = 0;
    private int giHeight = 0;

    private int viewX = 0;
    private int viewY = 0;

    float[] sphere_vertices = new float[1146];
    float[] sphere_normals = new float[1146];
    float[] sphere_textures = new float[764];
    short[] sphere_elements = new short[2280];

    int numVertices;
    int numElements;

    private int vertexShaderObject_PF;
    private int fragmentShaderObject_PF;
    private int shaderProgramObject_PF;

    private int[] vao_sphere = new int[1];
    private int[] vbo_position_sphere = new int[1];
    private int[] vbo_normal_sphere = new int[1];
    private int[] vbo_element_sphere = new int[1];
    private int[] vbo_texture_sphere = new int[1];

    private int la_uniform_PF;
    private int ld_uniform_PF;
    private int ls_uniform_PF;
    private int lightPosition_uniform_PF;

    private int ka_uniform_PF;
    private int kd_uniform_PF;
    private int ks_uniform_PF;
    private int shininess_uniform_PF;

    private boolean bLight;
    private boolean bAnimate;

    private int modelMatrix_uniform_PF;
    private int viewMatrix_uniform_PF;
    private int perspectiveProjectionMatrix_uniform_PF;

    private int lkeypressed_uniform_PF;

    private float[] perspectiveProjectionMatrix = new float[16];

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
        if (bAnimate == true) {
            bAnimate = false;
        } else {
            bAnimate = true;
        }

        return (true);
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return (true);
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {

        key_pressed++;

        angleX = 0.0f;
        angleY = 0.0f;
        angleZ = 0.0f;

        if (key_pressed >= 4) {
            key_pressed = 1;
        }

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
        if (bLight == true) {
            bLight = false;
        } else {
            bLight = true;
        }
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

        Sphere sphere = new Sphere();

        // *******************************************************************
        // *** VERTEX SHADER ***
        // *******************************************************************

        // create vertex shader
        vertexShaderObject_PF = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

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
            "uniform vec3 u_la;                                                                                                 \n"+
            "uniform vec3 u_ld;                                                                                                 \n"+
            "uniform vec3 u_ls;                                                                                                 \n"+
            "uniform vec4 u_light_position;                                                                                     \n"+
            "                                                                                                                   \n"+
            "uniform vec3 u_ka;                                                                                                 \n"+
            "uniform vec3 u_kd;                                                                                                 \n"+
            "uniform vec3 u_ks;                                                                                                 \n"+
            "uniform float u_shininess;                                                                                         \n"+
            "                                                                                                                   \n"+
            "out vec3 phong_ads_light;                                                                                          \n"+
            "                                                                                                                   \n"+
            "vec4 eye_coordinates;                                                                                              \n"+
            "out vec3 transformed_normal;                                                                                       \n"+
            "out vec3 light_direction;                                                                                          \n"+
            "out vec3 view_vector;                                                                                              \n"+
            "                                                                                                                   \n"+
            "void main() {                                                                                                      \n"+
            "                                                                                                                   \n"+
            "   if(u_lkeypressed == 1) {                                                                                        \n"+
            "                                                                                                                   \n"+
            "       eye_coordinates     = u_v_matrix * u_m_matrix * vPosition;                                                  \n"+
            "       transformed_normal  = mat3(u_v_matrix * u_m_matrix) * vNormal;                                              \n"+
            "       light_direction     = vec3(u_light_position - eye_coordinates);                                             \n"+
            "       view_vector         = -eye_coordinates.xyz;                                                                 \n"+
            "                                                                                                                   \n"+
            "   }                                                                                                               \n"+
            "                                                                                                                   \n"+
            "   gl_Position = u_p_matrix * u_v_matrix * u_m_matrix * vPosition;                                                 \n"+
            "                                                                                                                   \n"+
            "}                                                                                                                  \n");

        // provide source code to Vertex Shader
        GLES32.glShaderSource(vertexShaderObject_PF, vertexShaderSourceCode);

        // compile shader
        GLES32.glCompileShader(vertexShaderObject_PF);

        // error checking
        int[] iShaderCompileStatus = new int[1];
        int[] iInfoLofLength = new int[1];
        String szInfoLog = null;

        GLES32.glGetShaderiv(vertexShaderObject_PF, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
        if (iShaderCompileStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetShaderiv(vertexShaderObject_PF, GLES32.GL_INFO_LOG_LENGTH, iInfoLofLength, 0);
            if (iInfoLofLength[0] > 0) {

                szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject_PF);
                System.out.println("RSK: Vertex Shader Compilation Log = " + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        //*******************************************************************
        //                      *** FRAGMENT SHADER ***
        //*******************************************************************

        // create shader
        fragmentShaderObject_PF = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        // write shader
        final String fragmentShaderSourceCode = String.format(
            "#version 320 es                                                                                                                        \n"+
            "                                                                                                                                       \n"+
            "precision highp float;                                                                                                                 \n"+
            "precision mediump int;                                                                                                                 \n"+
            "                                                                                                                                       \n"+
            "vec3 phong_ads_light;                                                                                                                  \n"+
            "                                                                                                                                       \n"+
            "in vec3 transformed_normal;                                                                                                            \n"+
            "in vec3 light_direction;                                                                                                               \n"+
            "in vec3 view_vector;                                                                                                                   \n"+
            "                                                                                                                                       \n"+
            "vec3 reflection_vector;                                                                                                                \n"+
            "                                                                                                                                       \n"+
            "vec3 ambient;                                                                                                                          \n"+
            "vec3 diffuse;                                                                                                                          \n"+
            "vec3 specular;                                                                                                                         \n"+
            "                                                                                                                                       \n"+
            "uniform int  u_lkeypressed;                                                                                                            \n"+
            "                                                                                                                                       \n"+
            "uniform vec3 u_la;                                                                                                                     \n"+
            "uniform vec3 u_ld;                                                                                                                     \n"+
            "uniform vec3 u_ls;                                                                                                                     \n"+
            "uniform vec4 u_light_position;                                                                                                         \n"+
            "                                                                                                                                       \n"+
            "uniform vec3 u_ka;                                                                                                                     \n"+
            "uniform vec3 u_kd;                                                                                                                     \n"+
            "uniform vec3 u_ks;                                                                                                                     \n"+
            "uniform float u_shininess;                                                                                                             \n"+
            "                                                                                                                                       \n"+
            "out vec4 FragColor;                                                                                                                    \n"+
            "                                                                                                                                       \n"+
            "void main() {                                                                                                                          \n"+
            "                                                                                                                                       \n"+
            "   if(u_lkeypressed == 1) {                                                                                                            \n"+
            "                                                                                                                                       \n"+
            "       vec3 normalized_transformed_normal = normalize(transformed_normal);                                                             \n"+
            "       vec3 normalized_light_direction    = normalize(light_direction);                                                                \n"+
            "       vec3 normalized_view_vector        = normalize(view_vector);                                                                    \n"+        
            "       reflection_vector                  = reflect(-normalized_light_direction, normalized_transformed_normal);                       \n"+
            "       ambient                            = u_la * u_ka;                                                                               \n"+
            "       diffuse                            = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transformed_normal), 0.0);    \n"+
            "       specular                           = u_ls * u_ks * pow(max(dot(reflection_vector, normalized_view_vector),0.0), u_shininess);   \n"+
            "       phong_ads_light                    = ambient + diffuse + specular;                                                              \n"+
            "                                                                                                                                       \n"+
            "   } else {                                                                                                                            \n"+
            "                                                                                                                                       \n"+
            "      phong_ads_light = vec3(1.0, 1.0, 1.0);                                                                                           \n"+
            "                                                                                                                                       \n"+
            "   }                                                                                                                                   \n"+
            "                                                                                                                                       \n"+
            "   FragColor = vec4(phong_ads_light, 1.0);                                                                                             \n"+
            "                                                                                                                                       \n"+
            "}                                                                                                                                      \n");

        // provide source code to shader
        GLES32.glShaderSource(fragmentShaderObject_PF, fragmentShaderSourceCode);

        // compile shader
        GLES32.glCompileShader(fragmentShaderObject_PF);

        // error checking
        iShaderCompileStatus[0] = 0;
        iInfoLofLength[0] = 0;
        szInfoLog = null;

        GLES32.glGetShaderiv(fragmentShaderObject_PF, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
        if (iShaderCompileStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetShaderiv(fragmentShaderObject_PF, GLES32.GL_INFO_LOG_LENGTH, iInfoLofLength, 0);
            if (iInfoLofLength[0] > 0) {

                szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject_PF);
                System.out.println("RSK: Fragment Shader Compilation Log = " + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        // *******************************************************************
        // *** SHADER PROGRAM ***
        // *******************************************************************

        // create shader program
        shaderProgramObject_PF = GLES32.glCreateProgram();

        // attach vertex shader to shader program
        GLES32.glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);

        // attach fragment shader to shader program
        GLES32.glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

        // pre-linking step binding attributes to shader program
        GLES32.glBindAttribLocation(shaderProgramObject_PF, GLESMacros.RSK_ATTRIBUTE_VERTEX, "vPosition");
        GLES32.glBindAttribLocation(shaderProgramObject_PF, GLESMacros.RSK_ATTRIBUTE_NORMAL, "vNormal");

        // linking
        GLES32.glLinkProgram(shaderProgramObject_PF);

        // linking error checking
        int[] iShaderProgramLinkStatus = new int[1];
        iInfoLofLength[0] = 0;
        szInfoLog = null;

        GLES32.glGetProgramiv(shaderProgramObject_PF, GLES32.GL_LINK_STATUS, iShaderProgramLinkStatus, 0);

        if (iShaderProgramLinkStatus[0] == GLES32.GL_FALSE) {

            GLES32.glGetProgramiv(shaderProgramObject_PF, GLES32.GL_INFO_LOG_LENGTH, iInfoLofLength, 0);
            if (iInfoLofLength[0] > 0) {
                szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject_PF);
                System.out.println("RSK: Shader Program Link LOg =" + szInfoLog);
                uninitialized();
                System.exit(0);
            }
        }

        // get uniforms location here
        modelMatrix_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_m_matrix");
        viewMatrix_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_v_matrix");
        perspectiveProjectionMatrix_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_p_matrix");
        lkeypressed_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_lkeypressed");
        la_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_la");
        ld_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_ld");
        ls_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_ls");
        lightPosition_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_light_position");
        ka_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_ka");
        kd_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_kd");
        ks_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_ks");
        shininess_uniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_shininess");

        // *** vertices, colors, shader attributes, vbo, vao initializations ***

        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);

        numVertices = sphere.getNumberOfSphereVertices();
        numElements = sphere.getNumberOfSphereElements();

        // ______________________________________________________________________________
        // VAO_SPHERE
        // ______________________________________________________________________________

        GLES32.glGenVertexArrays(1, vao_sphere, 0);
        GLES32.glBindVertexArray(vao_sphere[0]);

        // ______________________________________________________________________________
        // POSITION
        // ______________________________________________________________________________

        GLES32.glGenBuffers(1, vbo_position_sphere, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_sphere[0]);

        // _______________________________________________________________________
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(sphere_vertices.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_vertices);
        verticesBuffer.position(0);
        // _______________________________________________________________________

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sphere_vertices.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // ______________________________________________________________________________
        // NORMAL
        // ______________________________________________________________________________

        GLES32.glGenBuffers(1, vbo_normal_sphere, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_normal_sphere[0]);

        // _______________________________________________________________________
        byteBuffer = ByteBuffer.allocateDirect(sphere_normals.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer = byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_normals);
        verticesBuffer.position(0);
        // _______________________________________________________________________

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sphere_normals.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_NORMAL);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // ______________________________________________________________________________
        // TEXTURE
        // ______________________________________________________________________________
        GLES32.glGenBuffers(1, vbo_texture_sphere, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texture_sphere[0]);

        // _______________________________________________________________________
        byteBuffer = ByteBuffer.allocateDirect(sphere_textures.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer = byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_textures);
        verticesBuffer.position(0);
        // _______________________________________________________________________

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sphere_textures.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_TEXTURE0, 2, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_TEXTURE0);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // ______________________________________________________________________________
        // ELEMENTS
        // ______________________________________________________________________________

        GLES32.glGenBuffers(1, vbo_element_sphere, 0);
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_element_sphere[0]);

        // _______________________________________________________________________
        byteBuffer = ByteBuffer.allocateDirect(sphere_elements.length * 2);
        byteBuffer.order(ByteOrder.nativeOrder());
        ShortBuffer elementsBuffer = byteBuffer.asShortBuffer();
        elementsBuffer.put(sphere_elements);
        elementsBuffer.position(0);
        // _______________________________________________________________________

        GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, sphere_elements.length * 2, elementsBuffer,
                GLES32.GL_STATIC_DRAW);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);

        bLight = false;
        bAnimate = false;

        // enable depth testing
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);

        // depth test
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // set the background color
        GLES32.glClearColor(0.11f, 0.11f, 0.11f, 1.0f);

        // set projection matrix to identity
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
    }

    private void resize(int width, int height) {

        if (height == 0) {
            height = 1;
        }

        giHeight = height;
        giWidth = width;

        // adjust the viewport based on geometry changes
        GLES32.glViewport(0, 0, width, height);
        Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float) width / (float) height, 0.1f, 1000.0f);
    }

    private void display() {

        // set matrix for transformation and projection
        float[] modelMatrix = new float[16];
        float[] viewMatrix = new float[16];

        // draw background color
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        // use shader program
        GLES32.glUseProgram(shaderProgramObject_PF);

        if (bLight == true) {

            if (key_pressed == 1) {

                light_position[0] = 0.0f;
                light_position[1] = (float) (r * Math.sin(angleX));
                light_position[2] = (float) (r * Math.cos(angleX));
                light_position[3] = 1.0f;

            } else if (key_pressed == 2) {

                light_position[0] = (float) (r * Math.sin(angleY));
                light_position[1] = 0.0f;
                light_position[2] = (float) (r * Math.cos(angleY));
                light_position[3] = 1.0f;

            } else if (key_pressed == 3) {

                light_position[0] = (float) (r * Math.sin(angleZ));
                light_position[1] = (float) (r * Math.cos(angleZ));
                light_position[2] = 0.0f;
                light_position[3] = 1.0f;
            }

            GLES32.glUniform1i(lkeypressed_uniform_PF, 1);

            GLES32.glUniform3fv(la_uniform_PF, 1, light_ambient, 0);
            GLES32.glUniform3fv(ld_uniform_PF, 1, light_diffuse, 0);
            GLES32.glUniform3fv(ls_uniform_PF, 1, light_specular, 0);
            GLES32.glUniform4fv(lightPosition_uniform_PF, 1, light_position, 0);

        } else {

            GLES32.glUniform1i(lkeypressed_uniform_PF, 0);

        }

        // set matrix to identity
        Matrix.setIdentityM(modelMatrix, 0);
        Matrix.setIdentityM(viewMatrix, 0);

        Matrix.translateM(modelMatrix, 0, modelMatrix, 0, tx, ty, tz);
        Matrix.scaleM(modelMatrix, 0, modelMatrix, 0, sx, sy, sz);

        // pass mvp matrix to shader
        GLES32.glUniformMatrix4fv(modelMatrix_uniform_PF, 1, false, modelMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrix_uniform_PF, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(perspectiveProjectionMatrix_uniform_PF, 1, false, perspectiveProjectionMatrix, 0);

        draw24Sphere();

        // stop using shader program
        GLES32.glUseProgram(0);

        update();

        // render
        requestRender();

    }

    private void draw24Sphere() {

        /**
         * Emerald 1st Sphere On First Column
         */
        material_ambient[0] = 0.0215f; // r
        material_ambient[1] = 0.1745f; // g
        material_ambient[2] = 0.0215f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.07568f; // r
        material_diffuse[1] = 0.61424f; // g
        material_diffuse[2] = 0.07568f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.633f; // r
        material_specular[1] = 0.727811f; // g
        material_specular[2] = 0.633f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.6f * 128f;
        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 0;
        viewY = 5;
        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Jade 2nd Sphere On First Column
         */
        material_ambient[0] = 0.135f; // r
        material_ambient[1] = 0.2255f; // g
        material_ambient[2] = 0.1575f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.54f; // r
        material_diffuse[1] = 0.89f; // g
        material_diffuse[2] = 0.63f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.316228f; // r
        material_specular[1] = 0.316228f; // g
        material_specular[2] = 0.316228f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.1f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 0;
        viewY = 4;
        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Obsidian 3rd Sphere On First Column
         */
        material_ambient[0] = 0.05375f; // r
        material_ambient[1] = 0.05f; // g
        material_ambient[2] = 0.06625f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.18275f; // r
        material_diffuse[1] = 0.17f; // g
        material_diffuse[2] = 0.22525f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.332741f; // r
        material_specular[1] = 0.328634f; // g
        material_specular[2] = 0.346434f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.3f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 0;
        viewY = 3;
        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Pearl 4TH Sphere On First Column
         */
        material_ambient[0] = 0.25f; // r
        material_ambient[1] = 0.20725f; // g
        material_ambient[2] = 0.20725f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 1.0f; // r
        material_diffuse[1] = 0.829f; // g
        material_diffuse[2] = 0.829f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.296648f; // r
        material_specular[1] = 0.296648f; // g
        material_specular[2] = 0.296648f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.088f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 0;
        viewY = 2;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Ruby 5th Sphere On First Column
         */
        material_ambient[0] = 0.1745f; // r
        material_ambient[1] = 0.01175f; // g
        material_ambient[2] = 0.01175f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.61424f; // r
        material_diffuse[1] = 0.04136f; // g
        material_diffuse[2] = 0.04136f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.727811f; // r
        material_specular[1] = 0.626959f; // g
        material_specular[2] = 0.626959f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.6f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 0;
        viewY = 1;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Turquoise 6th Sphere On First Column
         */

        material_ambient[0] = 0.1f; // r
        material_ambient[1] = 0.18725f; // g
        material_ambient[2] = 0.1745f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.396f; // r
        material_diffuse[1] = 0.74151f; // g
        material_diffuse[2] = 0.69102f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.297254f; // r
        material_specular[1] = 0.30829f; // g
        material_specular[2] = 0.306678f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.1f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 0;
        viewY = 0;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Brass 1st Sphere On Second Column
         */
        material_ambient[0] = 0.329412f; // r
        material_ambient[1] = 0.223529f; // g
        material_ambient[2] = 0.027451f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.780392f; // r
        material_diffuse[1] = 0.568627f; // g
        material_diffuse[2] = 0.113725f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.992157f; // r
        material_specular[1] = 0.941176f; // g
        material_specular[2] = 0.807843f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.21794872f * 128.0f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 1;
        viewY = 5;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Bronze 2nd Sphere On Second Column
         */
        material_ambient[0] = 0.2125f; // r
        material_ambient[1] = 0.1275f; // g
        material_ambient[2] = 0.054f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.714f; // r
        material_diffuse[1] = 0.4284f; // g
        material_diffuse[2] = 0.18144f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.39354f; // r
        material_specular[1] = 0.271906f; // g
        material_specular[2] = 0.166721f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.2f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 1;
        viewY = 4;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Chrome 3rd Sphere On Second Column
         */

        material_ambient[0] = 0.25f; // r
        material_ambient[1] = 0.25f; // g
        material_ambient[2] = 0.25f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.4f; // r
        material_diffuse[1] = 0.4f; // g
        material_diffuse[2] = 0.4f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.774597f; // r
        material_specular[1] = 0.774597f; // g
        material_specular[2] = 0.774597f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.6f * 128;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 1;
        viewY = 3;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Copper 4th Sphere On Second Column
         */

        material_ambient[0] = 0.19125f; // r
        material_ambient[1] = 0.0735f; // g
        material_ambient[2] = 0.0225f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.7038f; // r
        material_diffuse[1] = 0.27048f; // g
        material_diffuse[2] = 0.0828f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.256777f; // r
        material_specular[1] = 0.137622f; // g
        material_specular[2] = 0.086014f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.1f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 1;
        viewY = 2;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Gold 5th Sphere On Second Column
         */

        material_ambient[0] = 0.24725f; // r
        material_ambient[1] = 0.1995f; // g
        material_ambient[2] = 0.0745f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.75164f; // r
        material_diffuse[1] = 0.60648f; // g
        material_diffuse[2] = 0.2264f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.628281f; // r
        material_specular[1] = 0.555802f; // g
        material_specular[2] = 0.366065f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.4f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 1;
        viewY = 1;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Silver 6th Sphere On Second Column
         */

        material_ambient[0] = 0.19225f; // r
        material_ambient[1] = 0.19225f; // g
        material_ambient[2] = 0.19225f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.50754f; // r
        material_diffuse[1] = 0.50754f; // g
        material_diffuse[2] = 0.50754f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.508273f; // r
        material_specular[1] = 0.508273f; // g
        material_specular[2] = 0.508273f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.4f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 1;
        viewY = 0;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Black 1st Sphere On Third Column
         */

        material_ambient[0] = 0.0f; // r
        material_ambient[1] = 0.0f; // g
        material_ambient[2] = 0.0f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.01f; // r
        material_diffuse[1] = 0.01f; // g
        material_diffuse[2] = 0.01f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.50f; // r
        material_specular[1] = 0.50f; // g
        material_specular[2] = 0.50f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.25f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 2;
        viewY = 5;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Cyan 2nd Sphere On Third Column
         */
        material_ambient[0] = 0.0f; // r
        material_ambient[1] = 0.1f; // g
        material_ambient[2] = 0.06f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.0f; // r
        material_diffuse[1] = 0.50980392f; // g
        material_diffuse[2] = 0.50980392f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.50196078f; // r
        material_specular[1] = 0.50196078f; // g
        material_specular[2] = 0.50196078f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.25f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 2;
        viewY = 4;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Green 3rd Sphere On Third Column
         */

        material_ambient[0] = 0.0f; // r
        material_ambient[1] = 0.0f; // g
        material_ambient[2] = 0.0f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.1f; // r
        material_diffuse[1] = 0.35f; // g
        material_diffuse[2] = 0.1f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.45f; // r
        material_specular[1] = 0.55f; // g
        material_specular[2] = 0.45f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.25f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 2;
        viewY = 3;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Red 4th Sphere On Third Column
         */

        material_ambient[0] = 0.0f; // r
        material_ambient[1] = 0.0f; // g
        material_ambient[2] = 0.0f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.5f; // r
        material_diffuse[1] = 0.0f; // g
        material_diffuse[2] = 0.0f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.7f; // r
        material_specular[1] = 0.6f; // g
        material_specular[2] = 0.6f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.25f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 2;
        viewY = 2;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * White 5th Sphere On Third Column
         */

        material_ambient[0] = 0.0f; // r
        material_ambient[1] = 0.0f; // g
        material_ambient[2] = 0.0f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.55f; // r
        material_diffuse[1] = 0.55f; // g
        material_diffuse[2] = 0.55f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.70f; // r
        material_specular[1] = 0.70f; // g
        material_specular[2] = 0.70f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.25f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 2;
        viewY = 1;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Yellow Plastic 6th Sphere On Third Column
         */

        material_ambient[0] = 0.0f; // r
        material_ambient[1] = 0.0f; // g
        material_ambient[2] = 0.0f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.5f; // r
        material_diffuse[1] = 0.5f; // g
        material_diffuse[2] = 0.0f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.60f; // r
        material_specular[1] = 0.60f; // g
        material_specular[2] = 0.50f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.25f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 2;
        viewY = 0;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Black 1st Sphere On Fourth Column
         */

        material_ambient[0] = 0.02f; // r
        material_ambient[1] = 0.02f; // g
        material_ambient[2] = 0.02f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.01f; // r
        material_diffuse[1] = 0.01f; // g
        material_diffuse[2] = 0.01f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.4f; // r
        material_specular[1] = 0.4f; // g
        material_specular[2] = 0.4f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.078125f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 3;
        viewY = 5;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Cyan 2nd Sphere On Fourth Column
         */

        material_ambient[0] = 0.0f; // r
        material_ambient[1] = 0.05f; // g
        material_ambient[2] = 0.05f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.4f; // r
        material_diffuse[1] = 0.5f; // g
        material_diffuse[2] = 0.5f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.04f; // r
        material_specular[1] = 0.7f; // g
        material_specular[2] = 0.7f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.078125f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 3;
        viewY = 4;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Green 3rd Sphere On Fourth Column
         */

        material_ambient[0] = 0.0f; // r
        material_ambient[1] = 0.05f; // g
        material_ambient[2] = 0.0f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.4f; // r
        material_diffuse[1] = 0.5f; // g
        material_diffuse[2] = 0.4f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.04f; // r
        material_specular[1] = 0.7f; // g
        material_specular[2] = 0.04f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.078125f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 3;
        viewY = 3;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Red 4th Sphere On Fourth Column
         */
        material_ambient[0] = 0.05f; // r
        material_ambient[1] = 0.0f; // g
        material_ambient[2] = 0.0f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.5f; // r
        material_diffuse[1] = 0.4f; // g
        material_diffuse[2] = 0.4f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.7f; // r
        material_specular[1] = 0.04f; // g
        material_specular[2] = 0.04f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.078125f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 3;
        viewY = 2;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * White 5th Sphere On Fourth Column
         */

        material_ambient[0] = 0.05f; // r
        material_ambient[1] = 0.05f; // g
        material_ambient[2] = 0.05f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.5f; // r
        material_diffuse[1] = 0.5f; // g
        material_diffuse[2] = 0.5f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.7f; // r
        material_specular[1] = 0.7f; // g
        material_specular[2] = 0.7f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.078125f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 3;
        viewY = 1;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();

        /**
         * Yellow Rubber 6th Sphere On Fourth Column
         */

        material_ambient[0] = 0.05f; // r
        material_ambient[1] = 0.05f; // g
        material_ambient[2] = 0.0f; // b
        material_ambient[3] = 1.0f; // a

        material_diffuse[0] = 0.5f; // r
        material_diffuse[1] = 0.5f; // g
        material_diffuse[2] = 0.4f; // b
        material_diffuse[3] = 1.0f; // a

        material_specular[0] = 0.7f; // r
        material_specular[1] = 0.7f; // g
        material_specular[2] = 0.04f; // b
        material_specular[3] = 1.0f; // a

        material_shininess = 0.078125f * 128f;

        GLES32.glUniform3fv(ka_uniform_PF, 1, material_ambient, 0);
        GLES32.glUniform3fv(kd_uniform_PF, 1, material_diffuse, 0);
        GLES32.glUniform3fv(ks_uniform_PF, 1, material_specular, 0);
        GLES32.glUniform1f(shininess_uniform_PF, material_shininess);

        viewX = 3;
        viewY = 0;

        GLES32.glViewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 4, giHeight / 4);
        drawSphere();
    }

    private void drawSphere() {
        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_element_sphere[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
        // unbind vao
        GLES32.glBindVertexArray(0);
    }

    private void update() {
        if (bAnimate == true) {
            if (key_pressed == 1) {
                angleX = angleX + 0.02f;
            } else if (key_pressed == 2) {
                angleY = angleY + 0.02f;
            } else if (key_pressed == 3) {
                angleZ = angleZ + 0.02f;
            }
        }

    }

    private void uninitialized() {

        if (vao_sphere[0] != 0) {
            GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
            vao_sphere[0] = 0;
        }

        if (vbo_position_sphere[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_position_sphere, 0);
            vbo_position_sphere[0] = 0;
        }

        if (vbo_normal_sphere[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_normal_sphere, 0);
            vbo_normal_sphere[0] = 0;
        }

        if (vbo_texture_sphere[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_texture_sphere, 0);
            vbo_texture_sphere[0] = 0;
        }

        if (vbo_element_sphere[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_element_sphere, 0);
            vbo_element_sphere[0] = 0;
        }

        if (shaderProgramObject_PF != 0) {
            if (fragmentShaderObject_PF != 0) {
                GLES32.glDetachShader(shaderProgramObject_PF, fragmentShaderObject_PF);
                GLES32.glDeleteShader(fragmentShaderObject_PF);
                fragmentShaderObject_PF = 0;
            }
            if (vertexShaderObject_PF != 0) {
                GLES32.glDetachShader(shaderProgramObject_PF, fragmentShaderObject_PF);
                GLES32.glDeleteShader(vertexShaderObject_PF);
                fragmentShaderObject_PF = 0;
            }
        }

        if (shaderProgramObject_PF != 0) {
            GLES32.glDeleteProgram(shaderProgramObject_PF);
            shaderProgramObject_PF = 0;
        }
    }
}
