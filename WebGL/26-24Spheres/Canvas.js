
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
var light_position = new Float32Array(4); 

var material_ambient    =  new Float32Array(3);
var material_diffuse    =  new Float32Array(3);
var material_specular   =  new Float32Array(3);
var material_shininess  =  0;

var vertexShaderObject_PF;
var fragmentShaderObject_PF;
var shaderProgramObject_PF;

var keypressed = 0;
var r = 10.0;
var gfAngleX = 0.0;
var gfAngleY = 0.0;
var gfAngleZ = 0.0;

var tx = 0.0, ty = 0.0, tz = -2.1;
var sx = 1.0, sy = 1.0, sz = 1.0;

let giWidth = 800;
let giHeight = 600;

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
        case 88:  // x
            keypressed = 1;
            gfAngleX = 0.0;
            break;
        
        case 89:  // y
            keypressed = 2;
            gfAngleY = 0.0;
            break;

        case 90:   // z
            keypressed = 3;
            gfAngleY = 0.0;
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

    sphere = new Mesh();
    makeSphere(sphere, 0.8, 30, 30);  

    bLight = false;
	bAnimate = false;

    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST)
    gl.depthFunc(gl.LEQUAL);

    gl.clearColor(0.11,0.11,0.11,1.0);

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

    giWidth = canvas.width;
    giHeight = canvas.height;

    gl.viewport(0,0,canvas.width,canvas.height);

    mat4.perspective(perspectiveProjectionMatrix, 45.0,(parseFloat(canvas.width)/parseFloat(canvas.height)), 0.1, 1000.0);
}


function draw() {

    gl.clear(gl.COLOR_BUFFER_BIT);
    
    gl.useProgram(shaderProgramObject_PF);

    if(bLight) {

        if (keypressed == 1) {
			light_position[0] = 0.0;
			light_position[1] = r * Math.sin(gfAngleX);
			light_position[2] = r * Math.cos(gfAngleX);
			light_position[3] = 1.0;

		}
		else if (keypressed == 2) {
			light_position[0] = r * Math.sin(gfAngleY);
			light_position[1] = 0.0;
			light_position[2] = r * Math.cos(gfAngleY);
			light_position[3] = 1.0;

		}
		else if (keypressed == 3) {
			light_position[0] = r * Math.sin(gfAngleZ);
			light_position[1] = r * Math.cos(gfAngleZ);
			light_position[2] = 0.0;
			light_position[3] = 1.0;

		}

        gl.uniform1i(lkeypressed_uniform_PF, 1);

        gl.uniform3fv(la_uniform_PF, light_ambient);
        gl.uniform3fv(ld_uniform_PF, light_diffuse);
        gl.uniform3fv(ls_uniform_PF, light_specular);
        gl.uniform4fv(lightPosition_uniform_PF, light_position);

    } else {
        gl.uniform1i(lkeypressed_uniform_PF, 0);
    }
    var modelMatrix         =  mat4.create();
    var viewMatrix          =  mat4.create();
    var projectionMatrix    =  perspectiveProjectionMatrix

    mat4.translate(modelMatrix, modelMatrix,[tx, ty, tz]);
    mat4.scale(modelMatrix, modelMatrix, [sx, sy, sx]);

    gl.uniformMatrix4fv(modelMatrixUniform_PF, false, modelMatrix);
    gl.uniformMatrix4fv(viewMatrixUniform_PF, false, viewMatrix);
    gl.uniformMatrix4fv(projectionMatrixUniform_PF, false, projectionMatrix);

    draw24Speheres();

    gl.useProgram(null);

    update();
    requestAnimationFrame(draw,canvas);

}

var viewX;
var viewY;

function draw24Speheres() {

	//_______________________________________________________________________________________

    //_________________________ Emerald 1st Sphere On First Coloumn _________________________
    material_ambient[0] = 0.0215;	//r
	material_ambient[1] = 0.1745;	//g
	material_ambient[2] = 0.0215;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.07568;	//r
	material_diffuse[1] = 0.61424;	//g
	material_diffuse[2] = 0.07568;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.633;	//r
	material_specular[1] = 0.727811;	//g
	material_specular[2] = 0.633;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.6 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 0; viewY = 5;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();
    
	//____________________________________________________________________________________
	//_________________________ Jade 2nd Sphere On First Coloumn _________________________
	material_ambient[0] = 0.135;	//r
	material_ambient[1] = 0.2255;	//g
	material_ambient[2] = 0.1575;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.54;	//r
	material_diffuse[1] = 0.89;	//g
	material_diffuse[2] = 0.63;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.316228;	//r
	material_specular[1] = 0.316228;	//g
	material_specular[2] = 0.316228;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.1 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 0; viewY = 4;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    	//____________________________________________________________________________________
	//___________________ Obsidian 3rd Sphere On First Coloumn _______________________________
	//Set Material

	material_ambient[0] = 0.05375;	//r
	material_ambient[1] = 0.05;	//g
	material_ambient[2] = 0.06625;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.18275;	//r
	material_diffuse[1] = 0.17;	//g
	material_diffuse[2] = 0.22525;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.332741;	//r
	material_specular[1] = 0.328634;	//g
	material_specular[2] = 0.346434;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.3 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 0; viewY = 3;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    //____________________________________________________________________________________
	//___________________ Pearl 4TH Sphere On First Coloumn _______________________________
	//Set Material
	material_ambient[0] = 0.25;	//r
	material_ambient[1] = 0.20725;		//g
	material_ambient[2] = 0.20725;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 1.0;	//r
	material_diffuse[1] = 0.829;	//g
	material_diffuse[2] = 0.829;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.296648	//r
	material_specular[1] = 0.296648;	//g
	material_specular[2] = 0.296648;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.088 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 0; viewY = 2;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    //____________________________________________________________________________________
	//___________________ Ruby 5th Sphere On First Coloumn _______________________________
	//Set Material
	material_ambient[0] = 0.1745;	//r
	material_ambient[1] = 0.01175;		//g
	material_ambient[2] = 0.01175;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.61424;	//r
	material_diffuse[1] = 0.04136;	//g
	material_diffuse[2] = 0.04136;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.727811;	//r
	material_specular[1] = 0.626959;	//g
	material_specular[2] = 0.626959;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.6 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 0; viewY = 1;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    //____________________________________________________________________________________
	//___________________ Turquoise 6th Sphere On First Coloumn _______________________________
	//Set Material
	material_ambient[0] = 0.1;	//r
	material_ambient[1] = 0.18725;		//g
	material_ambient[2] = 0.1745;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.396;	//r
	material_diffuse[1] = 0.74151;	//g
	material_diffuse[2] = 0.69102;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.297254;	//r
	material_specular[1] = 0.30829;	//g
	material_specular[2] = 0.306678;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.1 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 0; viewY = 0;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    //____________________________________________________________________________________
	//___________________________ Brass 1st Sphere On Second Coloumn ___________________________
	//Set Material

	material_ambient[0] = 0.329412;	//r
	material_ambient[1] = 0.223529;	//g
	material_ambient[2] = 0.027451;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.780392;	//r
	material_diffuse[1] = 0.568627;	//g
	material_diffuse[2] = 0.113725;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.992157;	//r
	material_specular[1] = 0.941176;	//g
	material_specular[2] = 0.807843;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.21794872 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 1; viewY = 5;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();
	//____________________________________________________________________________________
	//___________________________ Bronze 2nd Sphere On Second Coloumn ___________________________
	//Set Material

	material_ambient[0] = 0.2125;	//r
	material_ambient[1] = 0.1275;	//g
	material_ambient[2] = 0.054;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.714;	//r
	material_diffuse[1] = 0.4284;	//g
	material_diffuse[2] = 0.18144;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.39354;	//r
	material_specular[1] = 0.271906;	//g
	material_specular[2] = 0.166721;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.2 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 1; viewY = 4;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();
    //____________________________________________________________________________________
	//___________________________ Chrome 3rd Sphere On Second Coloumn ___________________________

	//Set Material

	material_ambient[0] = 0.25;	//r
	material_ambient[1] = 0.25;	//g
	material_ambient[2] = 0.25;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.4;	//r
	material_diffuse[1] = 0.4;	//g
	material_diffuse[2] = 0.4;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.774597;	//r
	material_specular[1] = 0.774597;	//g
	material_specular[2] = 0.774597;	//b
	material_specular[3] = 1.0;	//a

	material_shininess = 0.6 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 1; viewY = 3;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();
    	//____________________________________________________________________________________

	//___________________________ Copper 4th Sphere On Second Coloumn ___________________________

	material_ambient[0] = 0.19125;	//r
	material_ambient[1] = 0.0735;	//g
	material_ambient[2] = 0.0225;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.7038;	//r
	material_diffuse[1] = 0.27048;	//g
	material_diffuse[2] = 0.0828;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.256777;	//r
	material_specular[1] = 0.137622;	//g
	material_specular[2] = 0.086014;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.1 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 1; viewY = 2;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();
	//____________________________________________________________________________________
	//___________________________ Gold 5th Sphere On Second Coloumn ___________________________

	//Set Material

	material_ambient[0] = 0.24725;	//r
	material_ambient[1] = 0.1995;	//g
	material_ambient[2] = 0.0745;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.75164;	//r
	material_diffuse[1] = 0.60648;	//g
	material_diffuse[2] = 0.2264;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.628281;	//r
	material_specular[1] = 0.555802;	//g
	material_specular[2] = 0.366065;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.4 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 1; viewY = 1;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();
    	//____________________________________________________________________________________
	//___________________________ Silver 6th Sphere On Second Coloumn ___________________________

	//Set Material

	material_ambient[0] = 0.19225;	//r
	material_ambient[1] = 0.19225;	//g
	material_ambient[2] = 0.19225;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.50754;	//r
	material_diffuse[1] = 0.50754;	//g
	material_diffuse[2] = 0.50754;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.508273;	//r
	material_specular[1] = 0.508273;	//g
	material_specular[2] = 0.508273;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.4 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 1; viewY = 0;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    //____________________________________________________________________________________

	//___________________________ Black 1st Sphere On Third Coloumn ___________________________
	//Set Material


	material_ambient[0] = 0.0;	//r
	material_ambient[1] = 0.0;	//g
	material_ambient[2] = 0.0;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.01;	//r
	material_diffuse[1] = 0.01;	//g
	material_diffuse[2] = 0.01;	//b
	material_diffuse[3] = 1.0;	//a

	material_specular[0] = 0.50;	//r
	material_specular[1] = 0.50;	//g
	material_specular[2] = 0.50;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.25 * 128;
    
    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 2; viewY = 5;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    //____________________________________________________________________________________
	//___________________________ Cyan 2nd Sphere On Third Coloumn ___________________________

	//Set Material
	material_ambient[0] = 0.0;	//r
	material_ambient[1] = 0.1;	//g
	material_ambient[2] = 0.06;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.0;	//r
	material_diffuse[1] = 0.50980392;	//g
	material_diffuse[2] = 0.50980392;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.50196078;	//r
	material_specular[1] = 0.50196078;	//g
	material_specular[2] = 0.50196078;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.25 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 2; viewY = 4;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    //____________________________________________________________________________________

	//___________________________ Green 3rd Sphere On Third Coloumn ___________________________
	//Set Material

	material_ambient[0] = 0.0;	//r
	material_ambient[1] = 0.0;	//g
	material_ambient[2] = 0.0;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.1;	//r
	material_diffuse[1] = 0.35;	//g
	material_diffuse[2] = 0.1;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.45;	//r
	material_specular[1] = 0.55;	//g
	material_specular[2] = 0.45;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.25 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 2; viewY = 3;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    
	//____________________________________________________________________________________

	//___________________________ Red 4th Sphere On Third Coloumn ___________________________
	//Set Material


	material_ambient[0] = 0.0;	//r
	material_ambient[1] = 0.0;	//g
	material_ambient[2] = 0.0;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.5;	//r
	material_diffuse[1] = 0.0;	//g
	material_diffuse[2] = 0.0;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.7;	//r
	material_specular[1] = 0.6;	//g
	material_specular[2] = 0.6;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.25 * 128;

    
    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 2; viewY = 2;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();


    //____________________________________________________________________________________

	//___________________________ White 5th Sphere On Third Coloumn ___________________________
	//Set Material

	material_ambient[0] = 0.0;	//r
	material_ambient[1] = 0.0;	//g
	material_ambient[2] = 0.0;	//b
	material_ambient[3] = 1.0;		//a

    material_diffuse[0] = 0.55;	//r
	material_diffuse[1] = 0.55;	//g
	material_diffuse[2] = 0.55;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.70;	//r
	material_specular[1] = 0.70;	//g
	material_specular[2] = 0.70;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.25 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 2; viewY = 1;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    //____________________________________________________________________________________
	//___________________________ Yellow Plastic 6th Sphere On Third Coloumn ___________________________

	//Set Material

	material_ambient[0] = 0.0;	//r
	material_ambient[1] = 0.0;	//g
	material_ambient[2] = 0.0;	//b
	material_ambient[3] = 1.0;	//a

	material_diffuse[0] = 0.5;	//r
	material_diffuse[1] = 0.5;	//g
	material_diffuse[2] = 0.0;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.60;	//r
	material_specular[1] = 0.60;	//g
	material_specular[2] = 0.50;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.25 * 128;

    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 2; viewY = 0;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

    	//____________________________________________________________________________________

	//___________________________ Black 1st Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.02;	//r
	material_ambient[1] = 0.02;	//g
	material_ambient[2] = 0.02;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.01;	//r
	material_diffuse[1] = 0.01;	//g
	material_diffuse[2] = 0.01;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.4;	//r
	material_specular[1] = 0.4;	//g
	material_specular[2] = 0.4;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.078125 * 128;


    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 3; viewY = 5;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();
	//____________________________________________________________________________________
		//___________________________ Cyan 2nd Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.0;	//r
	material_ambient[1] = 0.05;	//g
	material_ambient[2] = 0.05;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.4;	//r
	material_diffuse[1] = 0.5;	//g
	material_diffuse[2] = 0.5;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.04;	//r
	material_specular[1] = 0.7;	//g
	material_specular[2] = 0.7;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.078125 * 128;

    
    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 3; viewY = 4;

	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

	//____________________________________________________________________________________


	//___________________________ Green 3rd Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.0;	//r
	material_ambient[1] = 0.05;	//g
	material_ambient[2] = 0.0;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.4;	//r
	material_diffuse[1] = 0.5;	//g
	material_diffuse[2] = 0.4;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.04;	//r
	material_specular[1] = 0.7;	//g
	material_specular[2] = 0.04;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.078125 * 128;
    
    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 3; viewY = 3;

	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

	//____________________________________________________________________________________


	//___________________________ Red 4th Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.05;	//r
	material_ambient[1] = 0.0;	//g
	material_ambient[2] = 0.0;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.5;	//r
	material_diffuse[1] = 0.4;	//g
	material_diffuse[2] = 0.4;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.7;	//r
	material_specular[1] = 0.04;	//g
	material_specular[2] = 0.04;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.078125 * 128;

    
    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 3; viewY = 2;

	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();


	//____________________________________________________________________________________

	//___________________________ White 5th Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.05;	//r
	material_ambient[1] = 0.05;	//g
	material_ambient[2] = 0.05;	//b
	material_ambient[3] = 1.0;		//a

	material_diffuse[0] = 0.5;	//r
	material_diffuse[1] = 0.5;	//g
	material_diffuse[2] = 0.5;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.7;	//r
	material_specular[1] = 0.7;	//g
	material_specular[2] = 0.7;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.078125 * 128;

    
    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 3; viewY = 1;

	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();

	//____________________________________________________________________________________

	//___________________________ Yellow Rubber 6th Sphere On Fourth Coloumn ___________________________

	material_ambient[0] = 0.05;	//r
	material_ambient[1] = 0.05;	//g
	material_ambient[2] = 0.0; //b
	material_ambient[3] = 1.0; //a

	material_diffuse[0] = 0.5;	//r
	material_diffuse[1] = 0.5;	//g
	material_diffuse[2] = 0.4;	//b
	material_diffuse[3] = 1.0;		//a

	material_specular[0] = 0.7;	//r
	material_specular[1] = 0.7;	//g
	material_specular[2] = 0.04;	//b
	material_specular[3] = 1.0;		//a

	material_shininess = 0.078125 * 128;

    
    gl.uniform3fv(ka_uniform_PF, material_ambient);
    gl.uniform3fv(kd_uniform_PF, material_diffuse);
    gl.uniform3fv(ks_uniform_PF, material_specular);
    gl.uniform1f(shininess_uniform_PF, material_shininess);

    viewX = 3; viewY = 0;
	gl.viewport(viewX * (giWidth / 4), viewY * (giHeight / 6), giWidth / 6,giHeight / 6);
    sphere.draw();
	
	//_____________________________
}


function update() {
    if (bAnimate) {
		if (keypressed == 1) {
			gfAngleX = gfAngleX + 0.02;
		}
		else if (keypressed == 2) {
			gfAngleY = gfAngleY + 0.02;
		}
		else if (keypressed == 3) {
			gfAngleZ = gfAngleZ + 0.02;
		}
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
}