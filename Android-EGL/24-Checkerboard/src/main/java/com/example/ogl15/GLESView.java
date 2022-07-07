package com.example.ogl15;

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

    private int CHECKIMAGEWIDHT = 64;
    private int CHECKIMAGEHEIGHT = 64;

    byte[] checkImageArr =  new byte[CHECKIMAGEWIDHT * CHECKIMAGEHEIGHT * 4];

    float quadVertices[] = new float[12];

    private int vertexShaderObject;
    private int fragmentShaderObject;
    private int shaderProgramObject;

    private int[] vao_quad = new int[1];
    private int[] vbo_position_quad = new int[1];
    private int[] vbo_texture_quad = new int[1];

    private int[] checkTexture = new int[1];

    private int mvpUniform;
    private int texture_sampler_uniform;

    ByteBuffer byteBuffer;
    FloatBuffer verticesBuffer_quad;

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

        final float quadTexCoord2f[] = new float[] {
            //front face
		    0.0f, 0.0f,
		    1.0f, 0.0f,
		    1.0f, 1.0f,
		    0.0f, 1.0f
        };


        // ______________________________QUAD____________________________________________________________

        GLES32.glGenVertexArrays(1, vao_quad, 0);
        GLES32.glBindVertexArray(vao_quad[0]);

        // _______________________________________________________________________
        byteBuffer = ByteBuffer.allocateDirect(quadVertices.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer_quad = byteBuffer.asFloatBuffer();
        verticesBuffer_quad.put(quadVertices);
        verticesBuffer_quad.position(0);
        // _______________________________________________________________________

        GLES32.glGenBuffers(1, vbo_position_quad, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_quad[0]);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, quadVertices.length * 4, verticesBuffer_quad,
                GLES32.GL_DYNAMIC_DRAW);
        GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
        
        // _______________________________________________________________________
        byteBuffer = ByteBuffer.allocateDirect(quadTexCoord2f.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer_quad = byteBuffer.asFloatBuffer();
        verticesBuffer_quad.put(quadTexCoord2f);
        verticesBuffer_quad.position(0);
        // _______________________________________________________________________
        
        GLES32.glGenBuffers(1, vbo_texture_quad, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texture_quad[0]);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, quadTexCoord2f.length * 4, verticesBuffer_quad,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_TEXTURE0, 2, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_TEXTURE0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
        GLES32.glBindVertexArray(0);

        loadGlTexture();

        // enable texture 2d
        GLES32.glEnable(GLES32.GL_TEXTURE_2D);

        // enable depth testing
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);

        // depth test
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // set the background color
        GLES32.glClearColor(0.098f, 0.098f, 0.098f, 1.0f);

        // set projection matrix to identity
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
    }

    private void loadGlTexture() {

        makeCheckImage();

        Bitmap bitmap = Bitmap.createBitmap(CHECKIMAGEHEIGHT, CHECKIMAGEWIDHT, Bitmap.Config.ARGB_8888);
        
        byteBuffer = ByteBuffer.allocateDirect(64 * 64 * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        byteBuffer.put(checkImageArr);
        byteBuffer.position(0);

        bitmap.copyPixelsFromBuffer(byteBuffer);

        GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 1);
        GLES32.glGenTextures(1, checkTexture, 0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, checkTexture[0]);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR_MIPMAP_LINEAR);
        GLUtils.texImage2D(GLES32.GL_TEXTURE_2D, 0, bitmap, 0);
		GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D);

    }

    private void makeCheckImage() {

        int i, j, c;
    
        for (i = 0; i < CHECKIMAGEHEIGHT; i++) {
    
            for (j = 0; j < CHECKIMAGEWIDHT; j++) {
    
                    c = (int) ((i & 8) ^ (j & 8)) * 255;
        
                checkImageArr[4*(i*64+j)+0] = (byte)c;
                checkImageArr[4*(i*64+j)+1] = (byte)c;
                checkImageArr[4*(i*64+j)+2] = (byte)c;
                checkImageArr[4*(i*64+j)+3] = (byte)255;
    
            }
        }
    }

    private void resize(int width, int height) {
        if (height == 0) {
            height = 1;
        }
        // adjust the viewport based on geometry changes
        GLES32.glViewport(0, 0, width, height);
        Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 60.0f, (float) width / (float) height, 0.1f, 30.0f);
    }

    private void display() {
        
        // draw background color
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        // set matrix for transformation and projection
        float modelViewMatrix[] = new float[16];
        float modelViewProjectionMatrix[] = new float[16];

        Matrix.setIdentityM(modelViewMatrix, 0);
        Matrix.setIdentityM(modelViewProjectionMatrix, 0);
        Matrix.translateM(modelViewMatrix, 0, modelViewMatrix, 0, 0.0f, 0.0f, -3.5f);
        Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);
        
        // stop using shader program
        GLES32.glUseProgram(shaderProgramObject);

            // pass mvp matrix to shader
            GLES32.glUniformMatrix4fv(mvpUniform, 1, false, modelViewProjectionMatrix, 0);
        
            GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
		    GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, checkTexture[0]);		//change state to texture for next upcoming geometry
		    GLES32.glUniform1i(texture_sampler_uniform, 0);

            GLES32.glBindVertexArray(vao_quad[0]);

            quadVertices[0] = -2.0f; quadVertices[1] = -1.0f; quadVertices[2] = 0.0f;
			quadVertices[3] = -2.0f; quadVertices[4] = 1.0f; quadVertices[5] = 0.0f;
			quadVertices[6] = 0.0f; quadVertices[7] = 1.0f; quadVertices[8] = 0.0f;
			quadVertices[9] = 0.0f; quadVertices[10] = -1.0f; quadVertices[11] = 0.0f;

            
            // _______________________________________________________________________
            byteBuffer = ByteBuffer.allocateDirect(quadVertices.length * 4);
            byteBuffer.order(ByteOrder.nativeOrder());
            verticesBuffer_quad = byteBuffer.asFloatBuffer();
            verticesBuffer_quad.put(quadVertices);
            verticesBuffer_quad.position(0);
            // _______________________________________________________________________

            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_quad[0]);
            GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, quadVertices.length * 4, verticesBuffer_quad,
                    GLES32.GL_DYNAMIC_DRAW);
            GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false, 0, 0);
            GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
            
            GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
        
            GLES32.glBindVertexArray(0);
        
            GLES32.glBindVertexArray(vao_quad[0]);

            quadVertices[0] = 1.0f;	 quadVertices[1] = -1.0f; quadVertices[2] = 0.0f;
			quadVertices[3] = 1.0f;	 quadVertices[4] = 1.0f; quadVertices[5] = 0.0f;
			quadVertices[6] = 2.41421f; quadVertices[7] = 1.0f; quadVertices[8] = -1.41421f;
			quadVertices[9] = 2.41421f; quadVertices[10] = -1.0f; quadVertices[11] = -1.41421f;

            
            // _______________________________________________________________________
            byteBuffer = ByteBuffer.allocateDirect(quadVertices.length * 4);
            byteBuffer.order(ByteOrder.nativeOrder());
            verticesBuffer_quad = byteBuffer.asFloatBuffer();
            verticesBuffer_quad.put(quadVertices);
            verticesBuffer_quad.position(0);
            // _______________________________________________________________________

            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_quad[0]);
            GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, quadVertices.length * 4, verticesBuffer_quad,
                    GLES32.GL_DYNAMIC_DRAW);
            GLES32.glVertexAttribPointer(GLESMacros.RSK_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false, 0, 0);
            GLES32.glEnableVertexAttribArray(GLESMacros.RSK_ATTRIBUTE_VERTEX);
            GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
            
            GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
        
            GLES32.glBindVertexArray(0);
        // stop using shader program
        GLES32.glUseProgram(0);

        // render
        requestRender();

    }

    private void uninitialized() {

        if (vao_quad[0] != 0) {
            GLES32.glDeleteVertexArrays(1, vao_quad, 0);
            vao_quad[0] = 0;
        }

        if (vbo_position_quad[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_position_quad, 0);
            vbo_position_quad[0] = 0;
        }

        if (vbo_texture_quad[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_texture_quad, 0);
            vbo_texture_quad[0] = 0;
        }

        if(checkTexture[0] != 0) {
            GLES32.glDeleteTextures(1, checkTexture, 0);
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
