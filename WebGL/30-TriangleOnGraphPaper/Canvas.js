
var canvas = null;
var gl = null;
var bFullscreen = false;
var canvas_original_width;
var canvas_original_height;

const WebGLMacros = {
    RSK_ATTRIBUTE_VERTEX:0,
    RSK_ATTRIBUTE_COLOR:1,
    RSK_ATTRIBUTE_NORMAL:2,
    RSK_ATTRIBUTE_TEXTURE0:3,
};

var vertexShaderObject;
var fragmentShaderObject;
var shaderProgramObject;

var vao;
var vbo_position;
var vbo_color;

var vao_triangle;
var vbo_position_triangle;

var mvpUniform;
var colorUniform;

var perspectiveProjectMatrix;

var requestAnimationFrame = window.requestAnimationFrame        || 
                            window.webkitRequestAnimationFrame  ||
                            window.mozRequestAnimationFrame     ||
                            window.oRequestAnimationFrame       ||
                            window.msRequestAnimationFrame;

var cancelAnimationFrame =  window.cancelAnimationFrame ||
                            window.webkitCancelRequestAnimationFrame    || window.webkitCancelAnimationFrame ||
                            window.mozCancelRequestAnimationFrame       || window.mozCancelAnimationFrame ||
                            window.oCancelRequestAnimationFrame         || window.mozCancelAnimationFrame ||
                            window.msCancelRequestAnimationFrame        || window.msCancelAnimationFrame

function main() {

    //code

    //get canvas from DOM
    canvas = document.getElementById("RSK_CANVAS");

    if(!canvas) {
        console.log("FAILED TO GET THE CANVAS");
    } else {
        console.log("SUCCESSFULLY GOT THE CANVAS");
    }

    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height; 

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
        console.log("SUCCESSFULLY GOT THE WEBGL-2 CONTEXT");
    }

    gl.viewportWidth  = canvas.width;
    gl.viewportHeight = canvas.height;
 
    //Vertex Shader
    var vertexShaderSourceCode = 
    "#version 300 es                                                  \n"+
    "                                                                 \n"+
    "in vec4 vPosition;                                               \n"+
    "uniform mat4 u_mvp_matrix;                                       \n"+
    "                                                                 \n"+
    "                                                                 \n"+
    "void main(void) {                                                \n"+
    "                                                                 \n"+
    "   gl_Position = u_mvp_matrix * vPosition;                       \n"+
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
    "uniform vec4 out_color;                                          \n"+
    "out vec4 FragColor;                                              \n"+
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

    mvpUniform   = gl.getUniformLocation(shaderProgramObject, "u_mvp_matrix");
    colorUniform = gl.getUniformLocation(shaderProgramObject, "out_color");

    var triangleVertices = new Float32Array([
        0.0,  1.0,	0.0,
		-1.0, -1.0,	0.0,
		1.0, -1.0,	0.0
    ]);

    var lineVertices = new Float32Array([
        0.0,  1.0,	0.0,
		0.0, -1.0,	0.0
    ]);

    
    vao_triangle = gl.createVertexArray();
    gl.bindVertexArray(vao_triangle);

    vbo_position_triangle = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position_triangle);
    gl.bufferData(gl.ARRAY_BUFFER,triangleVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    
    gl.bindVertexArray(null);


    vao = gl.createVertexArray();
    gl.bindVertexArray(vao);

    vbo_position = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position);
    gl.bufferData(gl.ARRAY_BUFFER, lineVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    
    gl.bindVertexArray(null);


    gl.clearDepth(1.0);
	gl.enable(gl.DEPTH_TEST);
	gl.depthFunc(gl.LEQUAL);

    gl.clearColor(0.0,0.0,0.0,1.0);

    perspectiveProjectMatrix = mat4.create();

}


function resize() {
    
    if(bFullscreen == true) {
        canvas.width  = window.innerWidth; 
        canvas.height = window.innerHeight;
    } else {
        canvas.width  = canvas_original_width; 
        canvas.height = canvas_original_height;
    }

    gl.viewport(0,0,canvas.width,canvas.height);

    mat4.perspective(perspectiveProjectMatrix, 45.0,(parseFloat(canvas.width)/parseFloat(canvas.height)), 0.1, 100.0);
}


function draw() {

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    
    drawGraph();
    drawTriangle();

    requestAnimationFrame(draw,canvas);

}

function drawGraph() {
    
    var modelViewMatrix =  mat4.create();
    var modelViewProjectionMatrix =  mat4.create();

    var color_red   = [1.0, 0.0, 0.0, 1.0];
    var color_green = [0.0, 1.0, 0.0, 1.0];
    var color_blue  = [0.0, 0.0, 1.0, 1.0];

    for (var y = 0.0; y <= 1.0; y += 0.05) {
        mat4.translate(modelViewMatrix, modelViewMatrix, [y, 0.0, -2.5]);
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectMatrix, modelViewMatrix);

        drawLines(modelViewProjectionMatrix, color_blue);
        modelViewMatrix =  mat4.create();
        modelViewProjectionMatrix =  mat4.create();
        
        mat4.translate(modelViewMatrix, modelViewMatrix, [-y, 0.0, -2.5]);
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectMatrix, modelViewMatrix);

        drawLines(modelViewProjectionMatrix, color_blue);
        modelViewMatrix =  mat4.create();
        modelViewProjectionMatrix =  mat4.create();

        mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, y, -2.5]);
        mat4.rotateZ(modelViewMatrix, modelViewMatrix,degToRad(90));
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectMatrix, modelViewMatrix);

        drawLines(modelViewProjectionMatrix, color_blue);
        modelViewMatrix =  mat4.create();
        modelViewProjectionMatrix =  mat4.create();

        mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, -y, -2.5]);
        mat4.rotateZ(modelViewMatrix, modelViewMatrix,degToRad(90));
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectMatrix, modelViewMatrix);

        drawLines(modelViewProjectionMatrix, color_blue);
        modelViewMatrix =  mat4.create();
        modelViewProjectionMatrix =  mat4.create();
    }

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -2.5]);
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectMatrix, modelViewMatrix);

    drawLines(modelViewProjectionMatrix, color_green);
    modelViewMatrix =  mat4.create();
    modelViewProjectionMatrix =  mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -2.5]);
    mat4.rotateZ(modelViewMatrix, modelViewMatrix,degToRad(90));
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectMatrix, modelViewMatrix);

    drawLines(modelViewProjectionMatrix, color_red);
    modelViewMatrix =  mat4.create();
    modelViewProjectionMatrix =  mat4.create();

}

function drawLines(modelViewProjectionMatrix, color) {

    gl.useProgram(shaderProgramObject);

    gl.uniformMatrix4fv(mvpUniform, false, modelViewProjectionMatrix);
    gl.uniform4fv(colorUniform, color);

    gl.bindVertexArray(vao);
        gl.drawArrays(gl.LINES, 0, 2);
    gl.bindVertexArray(null);

    gl.useProgram(null);
}

function drawTriangle() {

     
    var modelViewMatrix =  mat4.create();
    var modelViewProjectionMatrix =  mat4.create();

    var color_yellow   = [1.0, 1.0, 0.0, 1.0];

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -2.5]);
    mat4.scale(modelViewMatrix, modelViewMatrix, [0.50 ,0.50, 0.50]);
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectMatrix, modelViewMatrix);

    gl.useProgram(shaderProgramObject);
        gl.uniformMatrix4fv(mvpUniform, false, modelViewProjectionMatrix);
        gl.uniform4fv(colorUniform, color_yellow);

        gl.bindVertexArray(vao_triangle);
            gl.drawArrays(gl.LINE_LOOP  , 0, 3);
        gl.bindVertexArray(null);
    gl.useProgram(null);

}

function degToRad(degree) {
    return(degree * Math.PI/180.0);
}

function uninitialize() {

    if(vao) {
        gl.deleteVertexArray(vao);
        vao = null;
    }

    if(vbo_position) {
        gl.deleteBuffer(vbo_position);
        vbo_position = null;
    }

    if(vbo_color) {
        gl.deleteBuffer(vbo_color);
        vbo_color = null;
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