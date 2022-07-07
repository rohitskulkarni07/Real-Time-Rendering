
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

var light_ambient_red  = new Float32Array([0.0, 0.0, 0.0 ]);
var light_diffuse_red  = new Float32Array([1.0, 0.0, 0.0]);
var light_specular_red = new Float32Array([1.0, 0.0, 0.0]);
var light_position_red = new Float32Array([-2.0, 0.0, 0.0, 1.0]); 

var light_ambient_blue  = new Float32Array([0.0, 0.0, 0.0 ]);
var light_diffuse_blue  = new Float32Array([0.0, 0.0, 1.0]);
var light_specular_blue = new Float32Array([0.0, 0.0, 1.0]);
var light_position_blue = new Float32Array([2.0, 0.0, 0.0, 1.0]); 

var material_ambient    =  new Float32Array([0.0, 0.0, 0.0]);
var material_diffuse    =  new Float32Array([1.0, 1.0, 1.0]);
var material_specular   =  new Float32Array([1.0, 1.0, 1.0]);
var material_shininess  = 128.0;

var vao_pyramid;
var vbo_position_pyramid;
var vbo_normal_pyramid;

var bLight;
var bAnimate;

var vertexShaderObject_PV;
var fragmentShaderObject_PV;
var shaderProgramObject_PV;

var la_uniform_PV = new Array(2);
var ld_uniform_PV = new Array(2);
var ls_uniform_PV = new Array(2);
var lightPosition_uniform_PV = new Array(2);

var ka_uniform_PV;
var kd_uniform_PV;
var ks_uniform_PV;
var shininess_uniform_PV;

var modelMatrixUniform_PV;
var viewMatrixUniform_PV;
var projectionMatrixUniform_PV;

var lkeypressed_uniform_PV;

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
    "uniform vec3 u_la[2];                                                                                                         \n"+
    "uniform vec3 u_ld[2];                                                                                                         \n"+
    "uniform vec3 u_ls[2];                                                                                                         \n"+
    "uniform vec4 u_light_position[2];                                                                                            \n"+
    "                                                                                                                           \n"+
    "uniform vec3 u_ka;                                                                                                         \n"+
    "uniform vec3 u_kd;                                                                                                         \n"+
    "uniform vec3 u_ks;                                                                                                         \n"+
    "uniform float u_shininess;                                                                                                 \n"+
    "                                                                                                                           \n"+
    "vec3 light_direction;                                                                                                                                   \n"+
    "vec3 reflection_vector;                                                                                                                           \n"+
    "vec3 ambient;                                                                                                                           \n"+
    "vec3 diffuse;                                                                                                                           \n"+
    "vec3 specular;                                                                                                                           \n"+
    "                                                                                                                           \n"+
    "out vec3 phong_ads_light;                                                                                                  \n"+
    "                                                                                                                           \n"+
    "void main(void) {                                                                                                          \n"+
    "                                                                                                                           \n"+
    "   int i = 0;                                                                                                                        \n"+
    "                                                                                                                           \n"+
    "   if(u_lkeypressed == 1) {                                                                                                \n"+
    "                                                                                                                           \n"+
    "       vec4 eye_coordinates    = u_v_matrix * u_m_matrix * vPosition;                                                      \n"+
    "       vec3 transformed_normal = normalize(mat3(u_v_matrix * u_m_matrix) * vNormal);                                       \n"+
    "       vec3 view_vector        = normalize(-eye_coordinates.xyz);                                                          \n"+
    "                                                                                                                           \n"+
    "       for(i = 0; i < 2; i++) {                                                                                                                    \n"+
    "                                                                                                                           \n"+
    "           light_direction         = normalize(vec3(u_light_position[i]-eye_coordinates));                                        \n"+
    "           reflection_vector       = reflect(-light_direction, transformed_normal);                                            \n"+
    "           ambient                 = u_la[i] * u_ka;                                                                              \n"+
    "           diffuse                 = u_ld[i] * u_kd * max(dot(light_direction, transformed_normal),0.0);                          \n"+
    "           specular                = u_ls[i] * u_ks * pow(max(dot(reflection_vector, view_vector),0.0), u_shininess);             \n"+
    "           phong_ads_light         = phong_ads_light + ambient + diffuse + specular;                                                             \n"+
    "       }                                                                                                                    \n"+
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

    modelMatrixUniform_PV          = gl.getUniformLocation(shaderProgramObject_PV, "u_m_matrix");
    viewMatrixUniform_PV           = gl.getUniformLocation(shaderProgramObject_PV, "u_v_matrix");
    projectionMatrixUniform_PV     = gl.getUniformLocation(shaderProgramObject_PV, "u_p_matrix");
    lkeypressed_uniform_PV         = gl.getUniformLocation(shaderProgramObject_PV, "u_lkeypressed");
    
    la_uniform_PV[0]               = gl.getUniformLocation(shaderProgramObject_PV, "u_la[0]"); 
    ld_uniform_PV[0]               = gl.getUniformLocation(shaderProgramObject_PV, "u_ld[0]"); 
    ls_uniform_PV[0]               = gl.getUniformLocation(shaderProgramObject_PV, "u_ls[0]");

    la_uniform_PV[1]               = gl.getUniformLocation(shaderProgramObject_PV, "u_la[1]"); 
    ld_uniform_PV[1]               = gl.getUniformLocation(shaderProgramObject_PV, "u_ld[1]"); 
    ls_uniform_PV[1]               = gl.getUniformLocation(shaderProgramObject_PV, "u_ls[1]");        
    
    lightPosition_uniform_PV[0]    = gl.getUniformLocation(shaderProgramObject_PV, "u_light_position[0]");
    lightPosition_uniform_PV[1]    = gl.getUniformLocation(shaderProgramObject_PV, "u_light_position[1]");
    
    ka_uniform_PV                  = gl.getUniformLocation(shaderProgramObject_PV, "u_ka"); 
    kd_uniform_PV                  = gl.getUniformLocation(shaderProgramObject_PV, "u_kd"); 
    ks_uniform_PV                  = gl.getUniformLocation(shaderProgramObject_PV, "u_ks");    
    shininess_uniform_PV           = gl.getUniformLocation(shaderProgramObject_PV, "u_shininess");

    var pyramidVertices = new Float32Array([
      //front face
		0.0, 1.0, 0.0,
		-1.0, -1.0, 1.0,
		1.0, -1.0, 1.0,

		//right face
		0.0, 1.0, 0.0,
		1.0, -1.0, 1.0,
		1.0, -1.0, -1.0,

		//back face
		0.0, 1.0, 0.0,
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,

		//left face
		0.0, 1.0, 0.0,
		-1.0, -1.0, -1.0,
		-1.0, -1.0, 1.0
    ]);

    var pyramidNormals = new Float32Array([
        // front face
		0.0, 0.447214, 0.894427,
		0.0, 0.447214, 0.894427,
		0.0, 0.447214, 0.894427,
		
        // right face
		0.894427, 0.447214, 0.0,
		0.894427, 0.447214, 0.0,
		0.894427, 0.447214, 0.0,
		
        // back face
		0.0, 0.447214, -0.894427,
		0.0, 0.447214, -0.894427,
		0.0, 0.447214, -0.894427,
		
        // left face
		-0.894427, 0.447214, 0.0,
		-0.894427, 0.447214, 0.0,
		-0.894427, 0.447214, 0.0
    ]);

    vao_pyramid = gl.createVertexArray();
    gl.bindVertexArray(vao_pyramid);

        vbo_position_pyramid = gl.createBuffer();
            gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position_pyramid);
            gl.bufferData(gl.ARRAY_BUFFER, pyramidVertices, gl.STATIC_DRAW);
            gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
            gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_VERTEX);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        vbo_normal_pyramid = gl.createBuffer();
            gl.bindBuffer(gl.ARRAY_BUFFER, vbo_normal_pyramid);
            gl.bufferData(gl.ARRAY_BUFFER, pyramidNormals, gl.STATIC_DRAW);
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
    
    gl.useProgram(shaderProgramObject_PV);

    if(bLight) {

        gl.uniform1i(lkeypressed_uniform_PV, 1);

        gl.uniform3fv(la_uniform_PV[0], light_ambient_red);
        gl.uniform3fv(ld_uniform_PV[0], light_diffuse_red);
        gl.uniform3fv(ls_uniform_PV[0], light_specular_red);
        gl.uniform4fv(lightPosition_uniform_PV[0], light_position_red);

        gl.uniform3fv(la_uniform_PV[1], light_ambient_blue);
        gl.uniform3fv(ld_uniform_PV[1], light_diffuse_blue);
        gl.uniform3fv(ls_uniform_PV[1], light_specular_blue);
        gl.uniform4fv(lightPosition_uniform_PV[1], light_position_blue);


        gl.uniform3fv(ka_uniform_PV, material_ambient);
        gl.uniform3fv(kd_uniform_PV, material_diffuse);
        gl.uniform3fv(ks_uniform_PV, material_specular);
        gl.uniform1f(shininess_uniform_PV, material_shininess);

    } else {
        gl.uniform1i(lkeypressed_uniform_PV, 0);
    }
    var modelMatrix         =  mat4.create();
    var viewMatrix          =  mat4.create();
    var projectionMatrix    =  perspectiveProjectionMatrix

    mat4.translate(modelMatrix, modelMatrix,[0.0,0.0,-3.5]);
    mat4.rotateY(modelMatrix, modelMatrix, angle);

    gl.uniformMatrix4fv(modelMatrixUniform_PV, false, modelMatrix);
    gl.uniformMatrix4fv(viewMatrixUniform_PV, false, viewMatrix);
    gl.uniformMatrix4fv(projectionMatrixUniform_PV, false, projectionMatrix);

    gl.bindVertexArray(vao_pyramid);
	gl.drawArrays(gl.TRIANGLES, 0, 12);	// why 12 is last parameter because vertices * triangle faces
	gl.bindVertexArray(null);

    gl.useProgram(null);

    update();
    requestAnimationFrame(draw,canvas);

}

function update() {
    if(true) {
        angle+=0.01;
    }
}

function degToRad(degree) {
    return(degree * Math.PI/180.0);
}

function uninitialize() {

    if(vao_pyramid) {
        gl.deleteVertexArray(vao_pyramid);
        vao_pyramid = null;
    }

    if(vbo_position_pyramid) {
        gl.deleteBuffer(vbo_position_pyramid);
        vbo_position_pyramid = null;
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