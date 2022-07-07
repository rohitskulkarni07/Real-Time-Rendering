
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
var vbo_color_cube;
var vbo_texture_cube;
var vbo_normal_cube;

var guiMarble_texture;

var ld_uniform;
var kd_uniform;

var light_position_uniform;

var bLight;
var bAnimate;

var model_view_matrix_uniform;
var perspective_projection_uniform;

var lkeypressed_uniform;
var texture_sample_uniform;

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
    "#version 300 es                                                                \n"+
    "                                                                               \n"+
    "precision mediump int;                                                         \n"+
    "                                                                               \n"+
    "in vec4 vPosition;                                                             \n"+
    "in vec3 vColor;                                                                \n"+
    "in vec3 vNormal;                                                               \n"+
    "in vec2 vTexCoord;                                                             \n"+
    "                                                                               \n"+
    "out vec2 out_texcoord;                                                         \n"+
    "out vec3 out_color;                                                            \n"+
    "                                                                               \n"+
    "uniform mat4 u_model_view_matrix;                                              \n"+
    "uniform mat4 u_projection_matrix;                                              \n"+
    "                                                                               \n"+
    "uniform int u_lkeypressed;                                                     \n"+
    "                                                                               \n"+
    "uniform vec3 u_ld;                                                             \n"+
    "uniform vec3 u_kd;                                                             \n"+
    "uniform vec4 u_light_position;                                                 \n"+
    "                                                                               \n"+
    "out vec3 diffuse_light;                                                        \n"+
    "                                                                               \n"+
    "void main(void)                                                                \n"+
    "{                                                                              \n"+
    "                                                                               \n"+
    "      if(u_lkeypressed == 1) {                                                 \n"+ 
    "          vec4 eye_coordinates = u_model_view_matrix * vPosition;              \n"+ 				
    "          mat3 normal_matrix = mat3(transpose(inverse(u_model_view_matrix)));  \n"+ 	
    "          vec3 t_norm = normalize(normal_matrix*vNormal);                      \n"+ 						
    "          vec3 s = normalize(vec3(u_light_position-eye_coordinates));          \n"+
    "          diffuse_light = u_ld * u_kd * max(dot(s, t_norm),0.0);               \n"+ 				
    "      }                                                                        \n"+
    "   gl_Position = u_projection_matrix * u_model_view_matrix * vPosition;        \n"+
    "   out_texcoord = vTexCoord;                                                   \n"+
    "   out_color = vColor;                                                         \n"+
    "}                                                                                           ";//end of code


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
    "#version 300 es                                                                                                    \n"+
    "                                                                                                                   \n"+
    "precision highp float;                                                                                             \n"+
    "precision mediump int;                                                                                             \n"+
    "                                                                                                                   \n"+
    "in vec3 out_color;                                                                                                 \n"+
    "in vec2 out_texcoord;                                                                                              \n"+
    "                                                                                                                   \n"+
    "uniform sampler2D u_texture_sampler;                                                                               \n"+
    "in vec3 diffuse_light;                                                                                             \n"+
    "uniform int u_lkeypressed;                                                                                         \n"+
    "                                                                                                                   \n"+
    "out vec4 FragColor;                                                                                                \n"+
    "vec4 color = vec4(0);                                                                                              \n"+
    "                                                                                                                   \n"+
    "void main(void)                                                                                                    \n"+
    "{                                                                                                                  \n"+
        "if(u_lkeypressed == 1) {                                                                                       \n"+
            "color = vec4(out_color, 1.0) * vec4(diffuse_light, 1.0) * texture( u_texture_sampler, out_texcoord);       \n"+
        "} else {                                                                                                       \n"+
            "color = vec4(out_color, 1.0) * texture( u_texture_sampler, out_texcoord);                                  \n"+
        "}                                                                                                              \n"+
        "FragColor = color;                                                                                         \n"+
    "}                                                                                                                   ";

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
	gl.bindAttribLocation(shaderProgramObject, WebGLMacros.RSK_ATTRIBUTE_COLOR, "vColor");
	gl.bindAttribLocation(shaderProgramObject, WebGLMacros.RSK_ATTRIBUTE_TEXTURE0, "vTexCoord");

    gl.linkProgram(shaderProgramObject);
    if(!gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS)) {
        
        var error = gl.getProgramInfoLog(shaderProgramObject);
        if(error.length > 0) {
            alert(error);
            uninitialize();
        }

    }

    console.log( model_view_matrix_uniform       = gl.getUniformLocation(shaderProgramObject, "u_model_view_matrix"));
    console.log( perspective_projection_uniform  = gl.getUniformLocation(shaderProgramObject, "u_projection_matrix"));
    console.log( texture_sample_uniform          = gl.getUniformLocation(shaderProgramObject, "u_texture_sampler"));
    console.log( lkeypressed_uniform             = gl.getUniformLocation(shaderProgramObject, "u_lkeypressed"));
    console.log( light_position_uniform          = gl.getUniformLocation(shaderProgramObject, "u_light_position"));
    console.log( ld_uniform                      = gl.getUniformLocation(shaderProgramObject, "u_ld"));
    console.log( kd_uniform                      = gl.getUniformLocation(shaderProgramObject, "u_kd"));

  	// *** vertices, colors, shader attribs, vbo, vao initializationa ***

	var interleavedArray = new Float32Array([
    	// vertice			//color			    //normals			//tex-coords
		 1.0,	1.0, 1.0,	1.0, 0.89, 0.89,	0.0, 0.0, 1.0,	    0.0, 0.0,
		-1.0,	1.0, 1.0,	0.89, 0.94, 1.0,	0.0, 0.0, 1.0,	    1.0, 0.0,
		-1.0, -1.0, 1.0,	0.90, 0.89, 0.94,   0.0, 0.0, 1.0,	    1.0, 1.0,
		 1.0, -1.0, 1.0,	0.98, 0.8, 1.0,	    0.0, 0.0, 1.0,	    0.0, 1.0,

		1.0,  1.0, -1.0,    1.0, 0.89, 0.89,	1.0, 0.0, 0.0,	    1.0, 0.0,
		1.0,  1.0,  1.0,    0.89, 0.94, 1.0,	1.0, 0.0, 0.0,	    1.0, 1.0,
		1.0, -1.0,  1.0,    0.90, 0.89, 0.94,	1.0, 0.0, 0.0,	    0.0, 1.0,
		1.0, -1.0, -1.0,    0.98, 0.8, 1.0,		1.0, 0.0, 0.0,	    0.0, 0.0,

		 1.0,  1.0, -1.0,   1.0, 0.89, 0.89,	0.0, 0.0, -1.0,	    1.0, 0.0,
		-1.0,  1.0, -1.0,   0.89, 0.94, 1.0,	0.0, 0.0, -1.0,	    1.0, 1.0,
		-1.0, -1.0, -1.0,   0.90, 0.89, 0.94,	0.0, 0.0, -1.0,	    0.0, 1.0,
		 1.0, -1.0, -1.0,   0.98, 0.8, 1.0,		0.0, 0.0, -1.0,	    0.0, 0.0,
																	
		-1.0,  1.0, -1.0,   1.0, 0.89, 0.89,	-1.0, 0.0, 0.0,	    0.0, 0.0,
		-1.0,  1.0,  1.0,   0.89, 0.94, 1.0,	-1.0, 0.0, 0.0,	    1.0, 0.0,
		-1.0, -1.0,  1.0,   0.90, 0.89, 0.94,	-1.0, 0.0, 0.0,	    1.0, 1.0,
		-1.0, -1.0, -1.0,   0.98, 0.8, 1.0,		-1.0, 0.0, 0.0,	    0.0, 1.0,
																	
		 1.0, 1.0,  1.0,	1.0, 0.89, 0.89,	0.0, 1.0, 0.0,	    0.0, 1.0,
		-1.0, 1.0,  1.0,	0.89, 0.94, 1.0,	0.0, 1.0, 0.0,	    0.0, 0.0,
		-1.0, 1.0, -1.0,	0.90, 0.89, 0.94,	0.0, 1.0, 0.0,	    1.0, 0.0,
		 1.0, 1.0, -1.0,	0.98, 0.8, 1.0,		0.0, 1.0, 0.0,	    1.0, 1.0,
																		
		 1.0, -1.0,  1.0,   1.0, 0.89, 0.89,	0.0, -1.0, 0.0,	    1.0, 1.0,
		-1.0, -1.0,  1.0,   0.89, 0.94, 1.0,	0.0, -1.0, 0.0,	    0.0, 1.0,
		-1.0, -1.0, -1.0,   0.90, 0.89, 0.94,	0.0, -1.0, 0.0,	    0.0, 0.0,
		 1.0, -1.0, -1.0,   0.98, 0.8, 1.0,		0.0, -1.0, 0.0,	    1.0, 0.0
    ]);																

	//__________________________________________________________________________________________
    var bytes_per_float = 4;//interleavedArray[0].BYTES_PER_ELEMENT;
    //console.log( interleavedArray[0].BYTES_PER_ELEMENT);
    console.log( bytes_per_float);

    vao_cube = gl.createVertexArray();
    gl.bindVertexArray(vao_cube);

    vbo_position_cube = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position_cube);
    gl.bufferData(gl.ARRAY_BUFFER, interleavedArray, gl.STATIC_DRAW);
    
    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 11 * bytes_per_float , 0);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_VERTEX);

    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 11 * bytes_per_float , 3 * bytes_per_float);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_COLOR);

    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 11 * bytes_per_float, 6 * bytes_per_float);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_NORMAL);

    gl.vertexAttribPointer(WebGLMacros.RSK_ATTRIBUTE_TEXTURE0, 2, gl.FLOAT, false, 11 * bytes_per_float, 9 *bytes_per_float);
    gl.enableVertexAttribArray(WebGLMacros.RSK_ATTRIBUTE_TEXTURE0);
    
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    guiMarble_texture           = gl.createTexture();
    guiMarble_texture.image     = new Image();
    guiMarble_texture.image.src = "marble.png";

    guiMarble_texture.image.onload = function () {
        gl.bindTexture(gl.TEXTURE_2D, guiMarble_texture);
        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,guiMarble_texture.image);
        gl.bindTexture(gl.TEXTURE_2D, null);
    }

    bLight = false;
	bAnimate = false;

    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST)
    gl.depthFunc(gl.LEQUAL);

    gl.clearColor(0.055,0.055,0.055,1.0);

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

    
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, guiMarble_texture);
	gl.uniform1i(texture_sample_uniform, 0);

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