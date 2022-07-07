
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

var gbPV = true;
var gbPF = false;

var light_ambient  = new Float32Array([0.1, 0.1, 0.1 ]);
var light_diffuse  = new Float32Array([1.0, 1.0, 1.0]);
var light_specular = new Float32Array([1.0, 1.0, 1.0]);
var light_position = new Float32Array([100.0, 100.0, 100.0, 1.0]); 

var material_ambient    =  new Float32Array([0.1, 0.1, 0.1]);
var material_diffuse    =  new Float32Array([0.5, 0.2, 0.7]);
var material_specular   =  new Float32Array([0.7, 0.7, 0.7]);
var material_shininess  = 128.0;

var vertexShaderObject_PF;
var fragmentShaderObject_PF;
var shaderProgramObject_PF;

var vao_cube;
var vbo_position_cube;
var vbo_normal_cube;

var la_uniform_PF;
var ld_uniform_PF;
var ls_uniform_PF;
var lightPosition_uniform_PF;

var ka_uniform_PF;
var kd_uniform_PF;
var ks_uniform_PF;
var shininess_uniform_PF;

var vertexShaderObject_PV;
var fragmentShaderObject_PV;
var shaderProgramObject_PV;

var la_uniform_PV;
var ld_uniform_PV;
var ls_uniform_PV;
var lightPosition_uniform_PV;

var ka_uniform_PV;
var kd_uniform_PV;
var ks_uniform_PV;
var shininess_uniform_PV;


var modelMatrixUniform_PV;
var viewMatrixUniform_PV;
var projectionMatrixUniform_PV;

var lkeypressed_uniform_PV;

var bLight;
var bAnimate;

//sphere
var sphere = null;

var modelMatrixUniform_PF;
var viewMatrixUniform_PF;
var projectionMatrixUniform_PF;

var lkeypressed_uniform_PF;

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
        case 81:    // Q
            uninitialize();
            window.close();
            break;
        
        case 66:    //ESC
            console.log(event);
            toggleFullscreen();
            break;

        case 76:    //L
            if (bLight) {
                bLight = false;
                gbPV = true;
                gbPF = false;
            }
            else {
                bLight = true;
                gbPV = true;
                gbPF = false;
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

        case 70:    //f
            gbPF = true;
            gbPV = false;
            break;

        case 86:    //v
            gbPV = true;
            gbPF = false;
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
    var vertexShaderSourceCode_PF = 
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
    "uniform vec4 u_light_position;                                                                                             \n"+
    "                                                                                                                           \n"+
    "vec4 eye_coordinates;                                                                                                      \n"+
    "out vec3 transformed_normal;                                                                                               \n"+
    "out vec3 light_direction;                                                                                                  \n"+
    "out vec3 view_vector;                                                                                                      \n"+
    "                                                                                                                           \n"+
    "void main(void) {                                                                                                          \n"+
    "                                                                                                                           \n"+
    "   if(u_lkeypressed == 1) {                                                                                                \n"+
    "                                                                                                                           \n"+
    "       eye_coordinates    = u_v_matrix * u_m_matrix * vPosition;                                                           \n"+
    "       transformed_normal = mat3(u_v_matrix * u_m_matrix) * vNormal;                                                       \n"+
    "       light_direction    = vec3(u_light_position-eye_coordinates);                                                        \n"+
    "       view_vector        = (-eye_coordinates.xyz);                                                                        \n"+
    "                                                                                                                           \n"+
    "   }                                                                                                                       \n"+
    "                                                                                                                           \n"+
    "   gl_Position = u_p_matrix * u_v_matrix * u_m_matrix * vPosition;                                                         \n"+
    "                                                                                                                           \n"+
    "}";

    vertexShaderObject_PF = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertexShaderObject_PF, vertexShaderSourceCode_PF);
    gl.compileShader(vertexShaderObject_PF);

    if(!gl.getShaderParameter(vertexShaderObject_PF, gl.COMPILE_STATUS)){
        var error = gl.getShaderInfoLog(vertexShaderObject_PF);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }
    }


      //Vertex Shader
      var vertexShaderSourceCode_PV = 
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
  
      vertexShaderObject_PV = gl.createShader(gl.VERTEX_SHADER);
      gl.shaderSource(vertexShaderObject_PV, vertexShaderSourceCode_PV);
      gl.compileShader(vertexShaderObject_PV);
  
      if(!gl.getShaderParameter(vertexShaderObject_PV, gl.COMPILE_STATUS)){
          var error = gl.getShaderInfoLog(vertexShaderObject_PV);
          if(error.length > 0) {
              alert(error);
              uninitialize();
          }
      }
  



    var fragmentShaderSourceCode_PF =
    "#version 300 es                                                                                                                    \n"+
    "                                                                                                                                   \n"+
    "precision highp float;                                                                                                             \n"+
    "precision mediump int;                                                                                                             \n"+
    "                                                                                                                                   \n"+
    "vec3 phong_ads_light;                                                                                                              \n"+
    "                                                                                                                                   \n"+
    "in vec3 transformed_normal;                                                                                                        \n"+
    "in vec3 light_direction;                                                                                                           \n"+
    "in vec3 view_vector;                                                                                                               \n"+
    "                                                                                                                                   \n"+
    "uniform int u_lkeypressed;                                                                                                         \n"+
    "                                                                                                                                   \n"+
    "uniform vec3 u_la;                                                                                                                 \n"+
    "uniform vec3 u_ld;                                                                                                                 \n"+
    "uniform vec3 u_ls;                                                                                                                 \n"+
    "                                                                                                                                   \n"+
    "uniform vec3 u_ka;                                                                                                                 \n"+
    "uniform vec3 u_kd;                                                                                                                 \n"+
    "uniform vec3 u_ks;                                                                                                                 \n"+
    "uniform float u_shininess;                                                                                                         \n"+
    "                                                                   	                                                            \n"+
    "out vec4 FragColor;                                                	                                                            \n"+
    "                                                                   	                                                            \n"+
    "void main(void) {                                                  	                                                            \n"+
    "                                                                   	                                                            \n"+
    "    if(u_lkeypressed == 1) {                                                                                                       \n"+
    "                                                                                                                                   \n"+
    "       vec3 normalized_transformed_normal   = normalize(transformed_normal);                                                       \n"+
    "       vec3 normalized_light_direction      = normalize(light_direction);                                                          \n"+
    "       vec3 normalized_view_vector          = normalize(view_vector);                                                              \n"+
    "       vec3 reflection_vector          = reflect(-normalized_light_direction, normalized_transformed_normal);                      \n"+
    "       vec3 ambient                    = u_la * u_ka;                                                                              \n"+
    "       vec3 diffuse                    = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transformed_normal),0.0);    \n"+
    "       vec3 specular                   = u_ls * u_ks * pow(max(dot(reflection_vector, normalized_view_vector),0.0), u_shininess);  \n"+
    "       phong_ads_light                 = ambient + diffuse + specular;                                                             \n"+
    "                                                                                                                                   \n"+
    "   } else {                                                                                                                        \n"+
    "                                                                                                                                   \n"+
    "       phong_ads_light = vec3(1.0,1.0,1.0);                                                                                        \n"+
    "                                                                                                                                   \n"+
    "   }                                                                                                                               \n"+
    "   FragColor = vec4(phong_ads_light, 1.0);                                                                                         \n"+
    "                                                                                                                                   \n"+
    "}";

    fragmentShaderObject_PF = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragmentShaderObject_PF, fragmentShaderSourceCode_PF);
    gl.compileShader(fragmentShaderObject_PF);

    if(!gl.getShaderParameter(fragmentShaderObject_PF, gl.COMPILE_STATUS)) {

        var error = gl.getShaderInfoLog(fragmentShaderObject_PF);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }
    }

    var fragmentShaderSourceCode_PV =
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

    fragmentShaderObject_PV = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragmentShaderObject_PV, fragmentShaderSourceCode_PV);
    gl.compileShader(fragmentShaderObject_PV);

    if(!gl.getShaderParameter(fragmentShaderObject_PV, gl.COMPILE_STATUS)) {

        var error = gl.getShaderInfoLog(fragmentShaderObject_PV);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }
    }

    
    shaderProgramObject_PF =  gl.createProgram();
    gl.attachShader(shaderProgramObject_PF, vertexShaderObject_PF);
    gl.attachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

    gl.bindAttribLocation(shaderProgramObject_PF, WebGLMacros.RSK_ATTRIBUTE_VERTEX, "vPosition");
    gl.bindAttribLocation(shaderProgramObject_PF, WebGLMacros.RSK_ATTRIBUTE_NORMAL, "vNormal");

    gl.linkProgram(shaderProgramObject_PF);
    if(!gl.getProgramParameter(shaderProgramObject_PF, gl.LINK_STATUS)) {
        
        var error = gl.getProgramInfoLog(shaderProgramObject_PF);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }

    }

    shaderProgramObject_PV =  gl.createProgram();
    gl.attachShader(shaderProgramObject_PV, vertexShaderObject_PV);
    gl.attachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

    gl.bindAttribLocation(shaderProgramObject_PV, WebGLMacros.RSK_ATTRIBUTE_VERTEX, "vPosition");
    gl.bindAttribLocation(shaderProgramObject_PV, WebGLMacros.RSK_ATTRIBUTE_NORMAL, "vNormal");

    gl.linkProgram(shaderProgramObject_PV);
    if(!gl.getProgramParameter(shaderProgramObject_PV, gl.LINK_STATUS)) {
        
        var error = gl.getProgramInfoLog(shaderProgramObject_PV);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }

    }

    modelMatrixUniform_PF          = gl.getUniformLocation(shaderProgramObject_PF, "u_m_matrix");
    viewMatrixUniform_PF           = gl.getUniformLocation(shaderProgramObject_PF, "u_v_matrix");
    projectionMatrixUniform_PF     = gl.getUniformLocation(shaderProgramObject_PF, "u_p_matrix");
    lkeypressed_uniform_PF         = gl.getUniformLocation(shaderProgramObject_PF, "u_lkeypressed");
    la_uniform_PF                  = gl.getUniformLocation(shaderProgramObject_PF, "u_la"); 
    ld_uniform_PF                  = gl.getUniformLocation(shaderProgramObject_PF, "u_ld"); 
    ls_uniform_PF                  = gl.getUniformLocation(shaderProgramObject_PF, "u_ls");    
    lightPosition_uniform_PF       = gl.getUniformLocation(shaderProgramObject_PF, "u_light_position");
    ka_uniform_PF                  = gl.getUniformLocation(shaderProgramObject_PF, "u_ka"); 
    kd_uniform_PF                  = gl.getUniformLocation(shaderProgramObject_PF, "u_kd"); 
    ks_uniform_PF                  = gl.getUniformLocation(shaderProgramObject_PF, "u_ks");    
    shininess_uniform_PF           = gl.getUniformLocation(shaderProgramObject_PF, "u_shininess");

    modelMatrixUniform_PV          = gl.getUniformLocation(shaderProgramObject_PV, "u_m_matrix");
    viewMatrixUniform_PV           = gl.getUniformLocation(shaderProgramObject_PV, "u_v_matrix");
    projectionMatrixUniform_PV     = gl.getUniformLocation(shaderProgramObject_PV, "u_p_matrix");
    lkeypressed_uniform_PV         = gl.getUniformLocation(shaderProgramObject_PV, "u_lkeypressed");
    la_uniform_PV                  = gl.getUniformLocation(shaderProgramObject_PV, "u_la"); 
    ld_uniform_PV                  = gl.getUniformLocation(shaderProgramObject_PV, "u_ld"); 
    ls_uniform_PV                  = gl.getUniformLocation(shaderProgramObject_PV, "u_ls");    
    lightPosition_uniform_PV       = gl.getUniformLocation(shaderProgramObject_PV, "u_light_position");
    ka_uniform_PV                  = gl.getUniformLocation(shaderProgramObject_PV, "u_ka"); 
    kd_uniform_PV                  = gl.getUniformLocation(shaderProgramObject_PV, "u_kd"); 
    ks_uniform_PV                  = gl.getUniformLocation(shaderProgramObject_PV, "u_ks");    
    shininess_uniform_PV           = gl.getUniformLocation(shaderProgramObject_PV, "u_shininess");
    
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
    
    var modelMatrix         =  mat4.create();
    var viewMatrix          =  mat4.create();
    var projectionMatrix    =  perspectiveProjectionMatrix

    mat4.translate(modelMatrix, modelMatrix,[0.0,0.0,-3.5]);

    if(gbPV) {
        
        gl.useProgram(shaderProgramObject_PV);
        gl.uniformMatrix4fv(modelMatrixUniform_PV, false, modelMatrix);
        gl.uniformMatrix4fv(viewMatrixUniform_PV, false, viewMatrix);
        gl.uniformMatrix4fv(projectionMatrixUniform_PV, false, projectionMatrix);
   
    } else {
      
        gl.useProgram(shaderProgramObject_PF);
        gl.uniformMatrix4fv(modelMatrixUniform_PF, false, modelMatrix);
        gl.uniformMatrix4fv(viewMatrixUniform_PF, false, viewMatrix);
        gl.uniformMatrix4fv(projectionMatrixUniform_PF, false, projectionMatrix);
   
    }

    if(bLight) {

        if (gbPV && bLight) {

            gl.uniform1i(lkeypressed_uniform_PV, 1);

            gl.uniform3fv(la_uniform_PV, light_ambient);
            gl.uniform3fv(ld_uniform_PV, light_diffuse);
            gl.uniform3fv(ls_uniform_PV, light_specular);
            gl.uniform4fv(lightPosition_uniform_PV, light_position);

            gl.uniform3fv(ka_uniform_PV, material_ambient);
            gl.uniform3fv(kd_uniform_PV, material_diffuse);
            gl.uniform3fv(ks_uniform_PV, material_specular);
            gl.uniform1f(shininess_uniform_PV, material_shininess);
        
            gl.uniformMatrix4fv(modelMatrixUniform_PV, false, modelMatrix);
            gl.uniformMatrix4fv(viewMatrixUniform_PV, false, viewMatrix);
            gl.uniformMatrix4fv(projectionMatrixUniform_PV, false, projectionMatrix);
       
        } else if(gbPF && bLight) {

            gl.uniform1i(lkeypressed_uniform_PF, 1);

            gl.uniform3fv(la_uniform_PF, light_ambient);
            gl.uniform3fv(ld_uniform_PF, light_diffuse);
            gl.uniform3fv(ls_uniform_PF, light_specular);
            gl.uniform4fv(lightPosition_uniform_PF, light_position);

            gl.uniform3fv(ka_uniform_PF, material_ambient);
            gl.uniform3fv(kd_uniform_PF, material_diffuse);
            gl.uniform3fv(ks_uniform_PF, material_specular);
            gl.uniform1f(shininess_uniform_PF, material_shininess);

            gl.uniformMatrix4fv(modelMatrixUniform_PF, false, modelMatrix);
            gl.uniformMatrix4fv(viewMatrixUniform_PF, false, viewMatrix);
            gl.uniformMatrix4fv(projectionMatrixUniform_PF, false, projectionMatrix);
        }
    } else {
        if (gbPV && !bLight) {
            gl.useProgram(shaderProgramObject_PV);
            gl.uniform1i(lkeypressed_uniform_PV, 0);
        } 
        if(gbPF && !bLight) {
            gl.uniform1i(lkeypressed_uniform_PF, 0);
        }
    }

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

    if(shaderProgramObject_PF) {
        
        if(fragmentShaderObject_PF) {
            gl.detachShader(shaderProgramObject_PF, fragmentShaderObject_PF);
            gl.deleteShader(fragmentShaderObject_PF);
            fragmentShaderObject_PF = null;
        }

        if(vertexShaderObject_PF) {
            gl.detachShader(shaderProgramObject_PF, vertexShaderObject_PF);
            gl.deleteShader(vertexShaderObject_PF);
            vertexShaderObject_PF = null;
        }

        gl.deleteProgram(shaderProgramObject_PF);
        shaderProgramObject_PF = null;
    }
    if(shaderProgramObject_PV) {
        
        if(fragmentShaderObject_PV) {
            gl.detachShader(shaderProgramObject_PV, fragmentShaderObject_PV);
            gl.deleteShader(fragmentShaderObject_PV);
            fragmentShaderObject_PV = null;
        }

        if(vertexShaderObject_PV) {
            gl.detachShader(shaderProgramObject_PV, vertexShaderObject_PV);
            gl.deleteShader(vertexShaderObject_PV);
            vertexShaderObject_PV = null;
        }

        gl.deleteProgram(shaderProgramObject_PV);
        shaderProgramObject_PV = null;
    }
}