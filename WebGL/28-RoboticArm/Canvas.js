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

var ld_uniform;
var kd_uniform;

var light_position_uniform;

var bLight;
var bAnimate;

let my_stack = new Stack();

let elbow       = 0.0;
let shoulder    = 0.0;

//sphere
var sphere = null;

var sphere_color_uniform;
var model_matrix_uniform;
var view_matrix_uniform;
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


        case 69:    // elbow S
            if(event.shiftKey) {
                elbow+=1.5;
            }
            else {
                elbow-=1.5;
            }
            break;

        case 83:    //shoulder S
            if(event.shiftKey) {
                shoulder+=1.5;
            }
            else {
                shoulder-=1.5;
            }
            break;

            // case:
            // break;
            // case:
            // break;
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
    "uniform mat4 u_m_matrix;                                                                   \n"+
    "uniform mat4 u_v_matrix;                                                                   \n"+
    "uniform mat4 u_p_matrix;                                                                   \n"+
    "                                                                                           \n"+
    "void main(void) {                                                                          \n"+
    "                                                                                           \n"+
    "   gl_Position = u_p_matrix * u_v_matrix * u_m_matrix * vPosition;                         \n"+
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
    "out vec4 FragColor;                                              \n"+
    "                                                                 \n"+
    "void main(void) {                                                \n"+
    "                                                                 \n"+
    "   FragColor = vec4(0.5,0.35,0.05,1.0);                          \n"+
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

    model_matrix_uniform            = gl.getUniformLocation(shaderProgramObject, "u_m_matrix");
    view_matrix_uniform             = gl.getUniformLocation(shaderProgramObject, "u_v_matrix");
    perspective_projection_uniform  = gl.getUniformLocation(shaderProgramObject, "u_p_matrix");

    sphere = new Mesh();
    makeSphere(sphere, 1.0, 60, 60);  

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

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    

    let modelMatrix      =  mat4.create();
    let viewMatrix       =  mat4.create();
    let projectionMatrix =  mat4.create();

    mat4.translate(modelMatrix, modelMatrix,[0.0,0.0,-12.0]);
    mat4.lookAt(viewMatrix, [0.0, 0.0, 1.0], [0.0, 0.0, 0.0], [0.0, 1.0, 0.0]);
    projectionMatrix = perspectiveProjectMatrix;

    my_stack.pushMatrix(modelMatrix);
    
        mat4.rotateZ(modelMatrix,modelMatrix,degToRad(shoulder));
        mat4.translate(modelMatrix, modelMatrix, [1.0,0.0,0.0]);
        
        my_stack.pushMatrix(modelMatrix);

            mat4.scale(modelMatrix,modelMatrix,[2.0, 0.5, 1.0]);
            drawArm(modelMatrix, viewMatrix, projectionMatrix);

        modelMatrix = my_stack.popMatrix();

        mat4.translate(modelMatrix, modelMatrix, [1.8, 0.0, 0.0])
        mat4.rotateZ(modelMatrix, modelMatrix, degToRad(elbow));
        mat4.translate(modelMatrix, modelMatrix, [1.0, 0.0, 0.0]);

        my_stack.pushMatrix(modelMatrix);

        mat4.scale(modelMatrix, modelMatrix, [2.0, 0.5, 1.0]);
        drawArm(modelMatrix, viewMatrix, projectionMatrix);

        modelMatrix = my_stack.popMatrix();

    modelMatrix = my_stack.popMatrix();
	//_________________________________________________


    //update();
    requestAnimationFrame(draw,canvas);

}

function drawArm( modelMatrix, viewMatrix, projectionMatrix ) {
    gl.useProgram(shaderProgramObject);
        
        gl.uniformMatrix4fv(model_matrix_uniform, false, modelMatrix);
        gl.uniformMatrix4fv(view_matrix_uniform, false, viewMatrix);
        gl.uniformMatrix4fv(perspective_projection_uniform, false, projectionMatrix);
        
        sphere.draw();

    gl.useProgram(null);

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
    
    if(sphere) {
        sphere.deallocate();
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