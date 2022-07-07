
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


var light_ambient  = new Float32Array([0.1, 0.1, 0.1 ]);
var light_diffuse  = new Float32Array([1.0, 1.0, 1.0]);
var light_specular = new Float32Array([1.0, 1.0, 1.0]);
var light_position = new Float32Array([100.0, 100.0, 100.0, 1.0]); 

var material_ambient    =  new Float32Array([0.1, 0.1, 0.1]);
var material_diffuse    =  new Float32Array([0.5, 0.2, 0.7]);
var material_specular   =  new Float32Array([0.7, 0.7, 0.7]);
var material_shininess  = 128.0;

var vertexShaderObject;
var fragmentShaderObject;
var shaderProgramObject;

var vao_cube;
var vbo_position_cube;
var vbo_normal_cube;

var la_uniform;
var ld_uniform;
var ls_uniform;
var lightPosition_uniform;

var ka_uniform;
var kd_uniform;
var kd_uniform;
var shininess_uniform;


var bLight;
var bAnimate;

//sphere
var sphere = null;

var modelMatrixUniform;
var viewMatrixUniform;
var projectionMatrixUniform;

var lkeypressed_uniform;

var perspectiveProjectionMatrix;

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
        
        case 80:
            if (gbPolygonMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                gbPolygonMode = false;
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                gbPolygonMode = true;
            }
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
    "#version 300 es                                                                                                            \n"+
    "                                                                                                                           \n"+
    "                                                                                                                           \n"+
    "precision highp float;                                                                                                     \n"+
    "precision mediump int;                                                                                                     \n"+
    "                                                                                                                           \n"+
    "in vec4 vPosition;                                                                                                         \n"+
    "in vec3 vNormal;                                                                                                           \n"+
    "                                                                                                                           \n"+
    "uniform mat4 u_m_matrix;                                                                                                   \n"+
    "uniform mat4 u_v_matrix;                                                                                                   \n"+
    "uniform mat4 u_p_matrix;                                                                                                   \n"+
    "                                                                                                                           \n"+
    "uniform int u_lkeypressed;                                                                                                 \n"+
    "                                                                                                                           \n"+
    "uniform vec3 u_la;                                                                                                         \n"+
    "uniform vec3 u_ld;                                                                                                         \n"+
    "uniform vec3 u_ls;                                                                                                         \n"+
    "uniform vec4 u_light_position;                                                                                            \n"+
    "                                                                                                                           \n"+
    "uniform vec3 u_ka;                                                                                                         \n"+
    "uniform vec3 u_kd;                                                                                                         \n"+
    "uniform vec3 u_ks;                                                                                                         \n"+
    "uniform float u_shininess;                                                                                                 \n"+
    "                                                                                                                           \n"+
    "out vec3 phong_ads_light;                                                                                                  \n"+
    "                                                                                                                           \n"+
    "void main(void) {                                                                                                          \n"+
    "                                                                                                                           \n"+
    "   if(u_lkeypressed == 1) {                                                                                                \n"+
    "                                                                                                                           \n"+
    "       vec4 eye_coordinates    = u_v_matrix * u_m_matrix * vPosition;                                                      \n"+
    "       vec3 transformed_normal = normalize(mat3(u_v_matrix * u_m_matrix) * vNormal);                                       \n"+
    "       vec3 light_direction    = normalize(vec3(u_light_position-eye_coordinates));                                        \n"+
    "       vec3 reflection_vector  = reflect(-light_direction, transformed_normal);                                            \n"+
    "       vec3 view_vector        = normalize(-eye_coordinates.xyz);                                                          \n"+
    "       vec3 ambient            = u_la * u_ka;                                                                              \n"+
    "       vec3 diffuse            = u_ld * u_kd * max(dot(light_direction, transformed_normal),0.0);                          \n"+
    "       vec3 specular           = u_ls * u_ks * pow(max(dot(reflection_vector, view_vector),0.0), u_shininess);             \n"+
    "       phong_ads_light         = ambient + diffuse + specular;                                                             \n"+
    "                                                                                                                           \n"+
    "   }                                                                                                                       \n"+
    "   else {                                                                                                                  \n"+
    "                                                                                                                           \n"+
    "       phong_ads_light         = vec3(1.0, 1.0, 1.0);                                                                      \n"+
    "                                                                                                                           \n"+
    "   }                                                                                                                       \n"+
    "                                                                                                                           \n"+
    "   gl_Position = u_p_matrix * u_v_matrix * u_m_matrix * vPosition;                                                         \n"+
    "                                                                                                                           \n"+
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
    "in vec3 phong_ads_light;                                         \n"+
    "                                                                 \n"+
    "out vec4 FragColor;                                              \n"+
    "                                                                 \n"+
    "void main(void) {                                                \n"+
    "                                                                 \n"+
    "   FragColor = vec4(phong_ads_light, 1.0);                       \n"+
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

    modelMatrixUniform          = gl.getUniformLocation(shaderProgramObject, "u_m_matrix");
    viewMatrixUniform           = gl.getUniformLocation(shaderProgramObject, "u_v_matrix");
    projectionMatrixUniform     = gl.getUniformLocation(shaderProgramObject, "u_p_matrix");
    lkeypressed_uniform         = gl.getUniformLocation(shaderProgramObject, "u_lkeypressed");
    la_uniform                  = gl.getUniformLocation(shaderProgramObject, "u_la"); 
    ld_uniform                  = gl.getUniformLocation(shaderProgramObject, "u_ld"); 
    ls_uniform                  = gl.getUniformLocation(shaderProgramObject, "u_ls");    
    lightPosition_uniform       = gl.getUniformLocation(shaderProgramObject, "u_light_position");
    ka_uniform                  = gl.getUniformLocation(shaderProgramObject, "u_ka"); 
    kd_uniform                  = gl.getUniformLocation(shaderProgramObject, "u_kd"); 
    ks_uniform                  = gl.getUniformLocation(shaderProgramObject, "u_ks");    
    shininess_uniform           = gl.getUniformLocation(shaderProgramObject, "u_shininess");

    sphere = new Mesh();
    makeSphere(sphere, 1.0, 30, 30);  

    bLight = false;
	bAnimate = false;

    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST)
    gl.depthFunc(gl.LEQUAL);

    gl.clearColor(0.0,0.0,0.0,1.0);

    perspectiveProjectionMatrix = mat4.create();

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

    mat4.perspective(perspectiveProjectionMatrix, 45.0,(parseFloat(canvas.width)/parseFloat(canvas.height)), 0.1, 1000.0);
}


function draw() {

    gl.clear(gl.COLOR_BUFFER_BIT);
    
    gl.useProgram(shaderProgramObject);

    if(bLight) {

        gl.uniform1i(lkeypressed_uniform, 1);

        gl.uniform3fv(la_uniform, light_ambient);
        gl.uniform3fv(ld_uniform, light_diffuse);
        gl.uniform3fv(ls_uniform, light_specular);
        gl.uniform4fv(lightPosition_uniform, light_position);

        gl.uniform3fv(ka_uniform, material_ambient);
        gl.uniform3fv(kd_uniform, material_diffuse);
        gl.uniform3fv(ks_uniform, material_specular);
        gl.uniform1f(shininess_uniform, material_shininess);

    } else {
        gl.uniform1i(lkeypressed_uniform, 0);
    }
    var modelMatrix         =  mat4.create();
    var viewMatrix          =  mat4.create();
    var projectionMatrix    =  perspectiveProjectionMatrix

    mat4.translate(modelMatrix, modelMatrix,[0.0,0.0,-3.5]);

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
    gl.uniformMatrix4fv(projectionMatrixUniform, false, projectionMatrix);


    sphere.draw();

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