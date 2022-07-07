
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

var vao_triangle;
var vbo_position_triangle;
var vbo_color_triangle;

var vao_quad;
var vbo_position_quad;
var vbo_color_quad;

var mvpUniform;

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
    "#version 300 es                                                  \n"+
    "                                                                 \n"+
    "in vec4 vPosition;                                               \n"+
    "in vec4 vColor;                                                  \n"+
    "uniform mat4 u_mvp_matrix;                                       \n"+
    "                                                                 \n"+
    "out vec4 out_color;                                              \n"+
    "                                                                 \n"+
    "void main(void) {                                                \n"+
    "                                                                 \n"+
    "   gl_Position = u_mvp_matrix * vPosition;                       \n"+
    "   out_color = vColor;                                           \n"+
    "                                                                 \n"+
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
    "                                                                 \n"+
    "in vec4 out_color;                                                    \n"+
    "out vec4 FragColor;                                              \n"+
    "uniform mat4 u_mvp_matrix;                                       \n"+
    "                                                                 \n"+
    "void main(void) {                                                \n"+
    "                                                                 \n"+
    "   FragColor = out_color;                                        \n"+
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
    gl.bindAttribLocation(shaderProgramObject, WebGLMacros.RSK_ATTRIBUTE_COLOR, "vColor");

    gl.linkProgram(shaderProgramObject);
    if(!gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS)) {
        
        var error = gl.getProgramInfoLog(shaderProgramObject);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }

    }

    mvpUniform = gl.getUniformLocation(shaderProgramObject, "u_mvp_matrix");

    var triangleVertices = new Float32Array([
        0.0,    1.0,   0.0,
        -1.0,  -1.0,  0.0,
        1.0,   -1.0,  0.0
    ]);

    var triangleColor = new Float32Array([
        1.0,    0.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    0.0,    1.0
    ]);

    var quadVertices = new Float32Array([
        1.0,    1.0,    0.0,
        -1.0,   1.0,    0.0,
        -1.0,   -1.0,   0.0,
        1.0,    -1.0,   0.0
    ]);

    var quadColor = new Float32Array([
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0
    ]);

    vao_triangle = gl.createVertexArray();
    gl.bindVertexArray(vao_triangle);

    vbo_position_triangle = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position_triangle);
    gl.bufferData(gl.ARRAY_BUFFER, triangleVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    vbo_color_triangle = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_color_triangle);
    gl.bufferData(gl.ARRAY_BUFFER, triangleColor, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_COLOR);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    
    gl.bindVertexArray(null);

    vao_quad = gl.createVertexArray();
    gl.bindVertexArray(vao_quad);

    vbo_position_quad = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position_quad);
    gl.bufferData(gl.ARRAY_BUFFER, quadVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    vbo_color_quad = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_color_quad);
    gl.bufferData(gl.ARRAY_BUFFER, quadColor, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_COLOR);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    
    gl.bindVertexArray(null);

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

    var modelViewMatrix =  mat4.create();
    var modelViewProjectionMatrix =  mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix,[-1.5,0.0,-3.5]);
    mat4.rotateY(modelViewMatrix, modelViewMatrix,degToRad(angle));
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectMatrix, modelViewMatrix);
    gl.uniformMatrix4fv(mvpUniform, false, modelViewProjectionMatrix);

    gl.bindVertexArray(vao_triangle);
    gl.drawArrays(gl.TRIANGLES, 0, 3);
    gl.bindVertexArray(null);

    modelViewMatrix =  mat4.create();
    modelViewProjectionMatrix =  mat4.create(); 

    mat4.translate(modelViewMatrix, modelViewMatrix,[1.5,0.0,-3.5]);
    mat4.scale(modelViewMatrix, modelViewMatrix, [0.75,0.75,0.75]);
    mat4.rotateX(modelViewMatrix, modelViewMatrix,degToRad(angle));
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectMatrix, modelViewMatrix);
    gl.uniformMatrix4fv(mvpUniform, false, modelViewProjectionMatrix);

    gl.bindVertexArray(vao_quad);
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.bindVertexArray(null);

    gl.useProgram(null);

    update();
    requestAnimationFrame(draw,canvas);

}

function update() {
    angle+=1.0;
}

function degToRad(degree) {
    return(degree * Math.PI/180.0);
}

function uninitialize() {

    if(vao_triangle) {
        gl.deleteVertexArray(vao_triangle);
        vao_triangle = null;
    }

    if(vbo_position_triangle) {
        gl.deleteBuffer(vbo_position_triangle);
        vbo_position_triangle = null;
    }

    if(vbo_color_triangle) {
        gl.deleteBuffer(vbo_color_triangle);
        vbo_color_triangle = null;
    }

    if(vao_quad) {
        gl.deleteVertexArray(vao_quad);
        vao_quad = null;
    }

    if(vbo_position_quad) {
        gl.deleteBuffer(vbo_position_quad);
        vbo_position_quad = null;
    }

    if(vbo_color_quad) {
        gl.deleteBuffer(vbo_color_quad);
        vbo_color_quad = null;
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