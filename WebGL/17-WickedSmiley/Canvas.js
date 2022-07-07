
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

var vao_pyramid;
var vbo_position_pyramid;
var vbo_textured_pyramid;

var vao_quad;
var vbo_position_quad;
var vbo_textured_quad;

var quadTexCoord2f = new Float32Array(8);


var smiley_texture;
var pressed_digit;
var texture_enable_uniform;
var texture_sample_uniform;
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
            case 49:	//1 ascii
			pressed_digit = 1;
			break;

		case 50:	//2
            pressed_digit = 2;
			break;

		case 51:	//3
            pressed_digit = 3;
			break;

		case 52:	//4
			pressed_digit = 4;
			break;

		default:
			pressed_digit = 0;
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
    "precision mediump int;                                           \n"+
    "                                                                 \n"+
    "in vec4 vPosition;                                               \n"+
    "in vec2 vTexCoord;                                               \n"+
    "uniform mat4 u_mvp_matrix;                                       \n"+
    "                                                                 \n"+
    "out vec2 out_texcoord;                                           \n"+
    "                                                                 \n"+
    "void main(void) {                                                \n"+
    "                                                                 \n"+
    "   gl_Position = u_mvp_matrix * vPosition;                       \n"+
    "   out_texcoord = vTexCoord;                                    \n"+
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
    "precision mediump int;                                           \n"+
    "                                                                 \n"+
    "in vec2 out_texcoord;                                            \n"+
    "uniform sampler2D u_texture_sampler;                             \n"+
    "uniform int u_texture_enable;                                    \n"+
    "out vec4 FragColor;                                              \n"+
    "uniform mat4 u_mvp_matrix;                                       \n"+
    "                                                                 \n"+
    "void main(void) {                                                \n"+
    "                                                                 \n"+
    "   if(u_texture_enable == 1) {                                   \n"+
    "       FragColor = texture(u_texture_sampler, out_texcoord);     \n"+
    "   } else {                                                      \n"+
    "       FragColor = vec4(1.0,1.0,1.0,1.0);                        \n"+
    "    }                                                            \n"+
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
    gl.bindAttribLocation(shaderProgramObject, WebGLMacros.RSK_ATTRIBUTE_TEXTURE0, "vTexCoord");

    gl.linkProgram(shaderProgramObject);
    if(!gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS)) {
        
        var error = gl.getProgramInfoLog(shaderProgramObject);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }

    }

    mvpUniform              = gl.getUniformLocation(shaderProgramObject, "u_mvp_matrix");
    texture_sample_uniform  = gl.getUniformLocation(shaderProgramObject, "u_texture_sampler");
    texture_enable_uniform  = gl.getUniformLocation(shaderProgramObject, "u_texture_enable");
 
    var quadVertices = new Float32Array([
        //front face
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,
    ]);

    vao_quad = gl.createVertexArray();
    gl.bindVertexArray(vao_quad);

    vbo_position_quad = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position_quad);
    gl.bufferData(gl.ARRAY_BUFFER, quadVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    vbo_textured_quad = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_textured_quad);
    gl.bufferData(gl.ARRAY_BUFFER,  quadTexCoord2f,gl.DYNAMIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_TEXTURE0, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_TEXTURE0);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    smiley_texture           = gl.createTexture();
    smiley_texture.image     = new Image();
    smiley_texture.image.src = "smiley.png";

    smiley_texture.image.onload = function () {
        gl.bindTexture(gl.TEXTURE_2D, smiley_texture);
        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,smiley_texture.image);
        gl.bindTexture(gl.TEXTURE_2D, null);
    }

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

    modelViewMatrix =  mat4.create();
    modelViewProjectionMatrix =  mat4.create(); 

    mat4.translate(modelViewMatrix, modelViewMatrix,[0.0,0.0,-3.5]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectMatrix, modelViewMatrix);
    gl.uniformMatrix4fv(mvpUniform, false, modelViewProjectionMatrix);

    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, smiley_texture);
	gl.uniform1i(texture_sample_uniform, 0);

    if (pressed_digit == 1) {
        quadTexCoord2f[0] = 1.0; quadTexCoord2f[1] = 1.0;
        quadTexCoord2f[2] = 0.0; quadTexCoord2f[3] = 1.0;
        quadTexCoord2f[4] = 0.0; quadTexCoord2f[5] = 0.0;
        quadTexCoord2f[6] = 1.0; quadTexCoord2f[7] = 0.0;
        gl.uniform1i(texture_enable_uniform, 1);

    }
    else if (pressed_digit == 2) {
        quadTexCoord2f[0] = 0.5; quadTexCoord2f[1] = 0.5;
        quadTexCoord2f[2] = 0.0; quadTexCoord2f[3] = 0.5;
        quadTexCoord2f[4] = 0.0; quadTexCoord2f[5] = 0.0;
        quadTexCoord2f[6] = 0.5; quadTexCoord2f[7] = 0.0;
        gl.uniform1i(texture_enable_uniform, 1);

    }
    else if (pressed_digit == 3) {
        quadTexCoord2f[0] = 2.0; quadTexCoord2f[1] = 2.0;
        quadTexCoord2f[2] = 0.0; quadTexCoord2f[3] = 2.0;
        quadTexCoord2f[4] = 0.0; quadTexCoord2f[5] = 0.0;
        quadTexCoord2f[6] = 2.0; quadTexCoord2f[7] = 0.0;
        gl.uniform1i(texture_enable_uniform, 1);

    }
    else if (pressed_digit == 4) {
        quadTexCoord2f[0] = 0.5; quadTexCoord2f[1] = 0.5;
        quadTexCoord2f[2] = 0.5; quadTexCoord2f[3] = 0.5;
        quadTexCoord2f[4] = 0.5; quadTexCoord2f[5] = 0.5;
        quadTexCoord2f[6] = 0.5; quadTexCoord2f[7] = 0.5;
        gl.uniform1i(texture_enable_uniform, 1);

    }	
    else {
        gl.uniform1i(texture_enable_uniform, 0);
    }
    
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_textured_quad);
    gl.bufferData(gl.ARRAY_BUFFER,  quadTexCoord2f,gl.DYNAMIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_TEXTURE0, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_TEXTURE0);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
  

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


    if(vao_quad) {
        gl.deleteVertexArray(vao_quad);
        vao_quad = null;
    }

    if(vbo_position_quad) {
        gl.deleteBuffer(vbo_position_quad);
        vbo_position_quad = null;
    }

    if(vbo_textured_quad) {
        gl.deleteBuffer(vbo_textured_quad);
        vbo_textured_quad = null;
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

//try this
//start chrome --allow-file-access-from-file --user-data-dir="D:\TEXTURE GROUP\temp"

//____OR____

//use this if above command not worked
//start chrome --disable-web-security --user-data-dir="D:\TEXTURE GROUP\temp"
//path for .html file

//file:///D:/TEXTURE%20GROUP/04_WebGL_Code/15-TexturedPyramidAndCube/Canvas.html

//OR USING LOCALHOST (***IF above all not worked at all***)

//python -m http.server
//give above command from source folder and then start chrome browser and give following url
//localhost:8000
//select .html (source file)
//ta-da !!! enjoy the output
