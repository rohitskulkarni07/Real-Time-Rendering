
var canvas = null;
var context = null;

function main() {

    //code
    //get canvas from DOM
    canvas = document.getElementById("RSK_CANVAS");

    if(!canvas) {
        console.log("FAILED TO GET THE CANVAS");
    } else {
        console.log("SUCCESFULLY GOT THE CANVAS");
    }

    //retrive the width and height of Canvas For Information
    console.log("Canvas Width = "+ canvas.width + " | Canvas Height = "+ canvas.height);

    // get drawing context From Canvas
    context = canvas.getContext("2d");
    if(!context) {
        console.log("FAILED TO GET THE CONTEXT");
    } else {
        console.log("SUCCESFULLY GOT THE CONTEXT");
    }

    //Paint Background by Black Color
    context.fillStyle = "black";

    //Fill The Canvas
    context.fillRect(0, 0, canvas.width, canvas.height);

    drawText("Hello, World !!!");
    //Event handling for keyboard
    window.addEventListener("keydown",keyDown,false);

    //Event handling for mouse
    window.addEventListener("click",mouseDown,false);

}


function keyDown(event) {
    switch(event.keyCode) {
        case 70:
            toggleFullscreen();
            //drawText("Hello, World !!! Hello, World !!!");
            break;

    }
}


function mouseDown() {
}

function drawText(text) {
  //Center the following text
  context.textAlign = "center";       // horizentally center
  context.textBaselign = "middle";    // vertically center
  //Font Familiy
  context.font = "48px sans-serif";
  // Foregrounf color
  context.fillStyle = "white";
  
  // fill the text
  context.fillText(text, canvas.width/2, canvas.height/2);
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

    } else {

        if(document.exitFullscreen) {
            document.exitFullscreen();
        } else if(document.webkitExitFullscreen) {
            document.webkitExitFullscreen();
        } else if(document.msExitFullscreen) {
            document.msExitFullscreen();
        }
    }

}
