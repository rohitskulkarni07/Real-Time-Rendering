
var canvas = null;
var gl = null;
var bFullscreen = false;
var canvas_orignal_width;
var canvas_orignal_height;

const WebGLMacros = {
    RSK_ATTRIBUTE_VERTEX:0,
    RSK_ATTRIBUTE_COLOR:1,
    RSK_ATTRIBUTE_NORMAL:2,
    RSK_ATTRIBUTE_TEXTURE0:3,
};

var angle = 0.0;

var vertexShaderObject;
var fragmentShaderObject;
var shaderProgramObject;

var vao_cube;
var vbo_position_cube;
var vbo_normal_cube;

var ld_uniform;
var kd_uniform;

var light_position_uniform;

var bLight;
var bAnimate;

var model_view_matrix_uniform;
var perspective_projection_uniform;
var lkeypressed_uniform;

var perspectiveProjectMatrix;

var requestAnimationFrame = window.requestAnimationFrame        || 
                            window.webkitRequestAnimationFrame  ||
                            window.mozRequestAnimationFrame     ||
                            window.oRequestAnimationFrame       ||
                            window.msRequestAnimatonFrame;

var cancelAnimationFrame =  window.cancelAnimationFrame ||
                            window.webkitCancelRequestAnimationFrame    || window.webkitCancelAnimationFrameme ||
                            window.mozCancelRequestAnimationFrame       || window.mozCancelAnimationFrameme ||
                            window.oCancelRequestAnimationFrame         || window.mozCancelAnimationFrame ||
                            window.msCancelRequestAnimationFrame        || window.msCancelAnimationFrame

function main() {

    //code

    //get canvas from DOM
    canvas = document.getElementById("RSK_CANVAS");

    if(!canvas) {
        console.log("FAILED TO GET THE CANVAS");
    } else {
        console.log("SUCCESFULLY GOT THE CANVAS");
    }

    canvas_orignal_width = canvas.width;
    canvas_orignal_height = canvas.height; 

    //Event handling for keyboard
    window.addEventListener("keydown",keyDown,false);

    //Event handling for mouse
    window.addEventListener("click",mouseDown,false);

    //Event handling for resize
    window.addEventListener("resize",resize,false);

    init();     // Initialize WebGL
    resize();   // warm-up resize
    draw();     // warm-up draw

}


function keyDown(event) {
    switch(event.keyCode) {
        case 27:    // escape
            uninitialize();
            window.close();
            break;

        case 70:    //'F' or 'f'
            toggleFullscreen();
            break;

            case 76:
                if (bLight) {
                    bLight = false;
                }
                else {
                    bLight = true;
                }
                break;
        case 65:
            if (bAnimate == true) {
				bAnimate = false;
			}
			else {
				bAnimate = true;
			}
            break;
    }
}


function mouseDown() {

}


function toggleFullscreen() {
    
    var fullscreen_element = document.fullscreenElement       ||    // chrome-default
                             document.webkitFullscreenElement ||    // apple safari
                             document.mozFullScreenElement    ||    // mozilla firefox
                             document.msFullscreenElement     ||    // MS-IE
                             null;                                  // other
    
    //if no fullscreen element set i.e no fullscreen
    
    if(fullscreen_element == null) {

        if(canvas.requestFullscreen) {
            canvas.requestFullscreen();
        } else if(canvas.webkitRequestFullscreen) {
            canvas.webkitRequestFullscreen();
        } else if(canvas.mozRequestFullScreen) {
            canvas.mozRequestFullScreen();
        } else if(canvas.msRequestFullscreen) {
            canvas.msRequestFullscreen();
        }

        bFullscreen = true;

    } else {

        if(document.exitFullscreen) {
            document.exitFullscreen();
        } else if(document.webkitExitFullscreen) {
            document.webkitExitFullscreen();
        } else if(document.msExitFullscreen) {
            document.msExitFullscreen();
        }

        bFullscreen = false;
    }

}


function init() {

    // Get WebGL-2 Context
    gl = canvas.getContext("webgl2");
    if(!gl) {
        console.log("FAILED TO GET THE WEBGL-2 CONTEXT");
    } else {
        console.log("SUCCESFULLY GOT THE WEBGL-2 CONTEXT");
    }

    gl.viewportWidth  = canvas.width;
    gl.viewportHeight = canvas.height;
 
    //Vertex Shader
    var vertexShaderSourceCode = 
    "#version 300 es                                                                            \n"+
    "                                                                                           \n"+
    "precision mediump int;                                                                     \n"+
    "                                                                                           \n"+
    "in vec4 vPosition;                                                                         \n"+
    "in vec3 vNormal;                                                                           \n"+
    "                                                                                           \n"+
    "uniform mat4 u_mv_matrix;                                                                  \n"+
    "uniform mat4 u_p_matrix;                                                                   \n"+
    "uniform int u_lkeypressed;                                                                 \n"+
    "uniform vec3 u_ld;                                                                         \n"+
    "uniform vec3 u_kd;                                                                         \n"+
    "uniform vec4 u_light_position;                                                             \n"+
    "                                                                                           \n"+
    "out vec3 diffuse_light;                                                                    \n"+
    "                                                                                           \n"+
    "void main(void) {                                                                          \n"+
    "                                                                                           \n"+
    "   if(u_lkeypressed == 1) {                                                                \n"+
    "       vec4 eye_coordinates    = u_mv_matrix * vPosition;                                  \n"+
    "       mat3 normal_matrix      = mat3(transpose(inverse(u_mv_matrix)));                    \n"+
    "       vec3 t_norm             = normalize(normal_matrix*vNormal);                         \n"+
    "       vec3 s                  = normalize(vec3(u_light_position-eye_coordinates));        \n"+
    "       diffuse_light           = u_ld * u_kd * max(dot(s, t_norm),0.0);                    \n"+
    "   }                                                                                       \n"+
    "                                                                                           \n"+
    "   gl_Position = u_p_matrix * u_mv_matrix * vPosition;                                     \n"+
    "                                                                                           \n"+
    "}";

    vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);
    gl.compileShader(vertexShaderObject);

    if(!gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS)){
        var error = gl.getShaderInfoLog(vertexShaderObject);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }
    }


    var fragmentShaderSourceCode =
    "#version 300 es                                                  \n"+
    "                                                                 \n"+
    "precision highp float;                                           \n"+
    "precision mediump int;                                           \n"+
    "                                                                 \n"+
    "vec4 color;                                                      \n"+
    "in vec3 diffuse_light;                                           \n"+
    "uniform int u_lkeypressed;                                       \n"+
    "                                                                 \n"+
    "out vec4 FragColor;                                              \n"+
    "                                                                 \n"+
    "void main(void) {                                                \n"+
    "                                                                 \n"+
    "   if(u_lkeypressed == 1) {                                      \n"+
    "       color = vec4(diffuse_light, 1.0);                         \n"+
    "   } else {                                                      \n"+
    "       color =  vec4(1.0,1.0,1.0,1.0);                           \n"+
    "   }                                                             \n"+
    "                                                                 \n"+
    "   FragColor = color;                                            \n"+
    "                                                                 \n"+
    "}";

    fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);
    gl.compileShader(fragmentShaderObject);

    if(!gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS)) {

        var error = gl.getShaderInfoLog(fragmentShaderObject);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }
    }

    shaderProgramObject =  gl.createProgram();
    gl.attachShader(shaderProgramObject, vertexShaderObject);
    gl.attachShader(shaderProgramObject, fragmentShaderObject);

    gl.bindAttribLocation(shaderProgramObject, WebGLMacros.RSK_ATTRIBUTE_VERTEX, "vPosition");
    gl.bindAttribLocation(shaderProgramObject, WebGLMacros.RSK_ATTRIBUTE_NORMAL, "vNormal");

    gl.linkProgram(shaderProgramObject);
    if(!gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS)) {
        
        var error = gl.getProgramInfoLog(shaderProgramObject);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }

    }

    model_view_matrix_uniform       = gl.getUniformLocation(shaderProgramObject, "u_mv_matrix");
    perspective_projection_uniform  = gl.getUniformLocation(shaderProgramObject, "u_p_matrix");
    lkeypressed_uniform             = gl.getUniformLocation(shaderProgramObject, "u_lkeypressed");
    light_position_uniform          = gl.getUniformLocation(shaderProgramObject, "u_light_position");
    ld_uniform                      = gl.getUniformLocation(shaderProgramObject, "u_ld");
    kd_uniform                      = gl.getUniformLocation(shaderProgramObject, "u_kd");

    var cubeVertices = new Float32Array([
        //front face
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,

        //right face
        1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        1.0, -1.0, 1.0,
        1.0, -1.0, -1.0,

        //back face
        1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,

        //left face
        -1.0, 1.0, -1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0,

        //top face
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0,
        1.0, 1.0, -1.0,

        //bottom face
        1.0, -1.0, 1.0,
        -1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0
    ]);

    var cubeNormals = new Float32Array([
        0.0, 0.0, 1.0,	//Front Face
		0.0, 0.0, 1.0,	//Front Face
		0.0, 0.0, 1.0,	//Front Face
		0.0, 0.0, 1.0,	//Front Face

		1.0, 0.0, 0.0,	//Right Face
		1.0, 0.0, 0.0,	//Right Face
		1.0, 0.0, 0.0,	//Right Face
		1.0, 0.0, 0.0,	//Right Face

		0.0, 0.0, -1.0,	//Back Face
		0.0, 0.0, -1.0,	//Back Face
		0.0, 0.0, -1.0,	//Back Face
		0.0, 0.0, -1.0,	//Back Face
		
		-1.0, 0.0, 0.0,	//Left Face
		-1.0, 0.0, 0.0,	//Left Face
		-1.0, 0.0, 0.0,	//Left Face
		-1.0, 0.0, 0.0,	//Left Face
		
		0.0, 1.0, 0.0,	//Top Face
		0.0, 1.0, 0.0,	//Top Face
		0.0, 1.0, 0.0,	//Top Face
		0.0, 1.0, 0.0,	//Top Face
		
		0.0, -1.0, 0.0,	//Bottom Face
		0.0, -1.0, 0.0,	//Bottom Face
		0.0, -1.0, 0.0,	//Bottom Face
		0.0, -1.0, 0.0	//Bottom Face  
    ]);

    vao_cube = gl.createVertexArray();
    gl.bindVertexArray(vao_cube);

    vbo_position_cube = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position_cube);
    gl.bufferData(gl.ARRAY_BUFFER, cubeVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    vbo_normal_cube = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_normal_cube);
    gl.bufferData(gl.ARRAY_BUFFER, cubeNormals, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_NORMAL);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    bLight = false;
	bAnimate = false;

    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST)
    gl.depthFunc(gl.LEQUAL);

    gl.clearColor(0.0,0.0,0.0,1.0);

    perspectiveProjectMatrix = mat4.create();

}


function resize() {
    
    if(bFullscreen == true) {
        canvas.width  = window.innerWidth; 
        canvas.height = window.innerHeight;
    } else {
        canvas.width  = canvas_orignal_width; 
        canvas.height = canvas_orignal_height;
    }

    gl.viewport(0,0,canvas.width,canvas.height);

    mat4.perspective(perspectiveProjectMatrix, 45.0,(parseFloat(canvas.width)/parseFloat(canvas.height)), 0.1, 100.0);
}


function draw() {

    gl.clear(gl.COLOR_BUFFER_BIT);
    
    gl.useProgram(shaderProgramObject);

    if (bLight) {
		
        gl.uniform1i(lkeypressed_uniform, 1);
        gl.uniform3f(ld_uniform, 1.0, 1.0, 1.0); // light diffuse
        gl.uniform3f(kd_uniform, 0.7, 0.7, 0.7); // material

        //var lightPosition = new Float32Array([0.0, 0.0, 2.0, 1.0]); //light position
        gl.uniform4f(light_position_uniform, 0.0, 0.0, 2.0, 1.0); // material

    }
    else {
        gl.uniform1i(lkeypressed_uniform, 0);
    }

    var modelViewMatrix =  mat4.create();
    //var modelViewProjectionMatrix =  mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix,[0.0,0.0,-3.5]);
    
    mat4.scale(modelViewMatrix, modelViewMatrix,[0.75,0.75,0.75]);
    
    mat4.rotateX(modelViewMatrix, modelViewMatrix,degToRad(angle));
    mat4.rotateY(modelViewMatrix, modelViewMatrix,degToRad(angle));
    mat4.rotateZ(modelViewMatrix, modelViewMatrix,degToRad(angle));

    gl.uniformMatrix4fv(model_view_matrix_uniform, false, modelViewMatrix);
    gl.uniformMatrix4fv(perspective_projection_uniform, false, perspectiveProjectMatrix);

    gl.bindVertexArray(vao_cube);
    
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);

    gl.bindVertexArray(null);

    gl.useProgram(null);

    update();
    requestAnimationFrame(draw,canvas);

}

function update() {
    if(bAnimate) {
        angle+=1.0;
    }
}

function degToRad(degree) {
    return(degree * Math.PI/180.0);
}

function uninitialize() {

    if(vao_cube) {
        gl.deleteVertexArray(vao_cube);
        vao_cube = null;
    }

    if(vbo_position_cube) {
        gl.deleteBuffer(vbo_position_cube);
        vbo_position_cube = null;
    }

    if(shaderProgramObject) {
        
        if(fragmentShaderObject) {
            gl.detachShader(shaderProgramObject, fragmentShaderObject);
            gl.deleteShader(fragmentShaderObject);
            fragmentShaderObject = null;
        }

        if(vertexShaderObject) {
            gl.detachShader(shaderProgramObject, vertexShaderObject);
            gl.deleteShader(vertexShaderObject);
            vertexShaderObject = null;
        }

        gl.deleteProgram(shaderProgramObject);
        shaderProgramObject = null;
    }
}