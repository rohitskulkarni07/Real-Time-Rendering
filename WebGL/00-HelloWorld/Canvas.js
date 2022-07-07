function main() {

    //code
    //get canvas from DOM
    var canvas = document.getElementById("RSK_CANVAS")
    if(!canvas) {
        console.log("FAILED TO GET THE CANVAS");
    } else {
        console.log("SUCCESFULLY GOT THE CANVAS");
    }

    //retrive the width and height of Canvas For Information
    console.log("Canvas Width = "+ canvas.width + " | Canvas Height = "+ canvas.height);

    // get drawing context From Canvas
    var context = canvas.getContext("2d");
    if(!context) {
        console.log("FAILED TO GET THE CONTEXT");
    } else {
        console.log("SUCCESFULLY GOT THE CONTEXT");
    }

    //Paint Background by Black Color
    context.fillStyle = "black";

    //Fill The Canvas
    context.fillRect(0, 0, canvas.width, canvas.height);

    //Center the following text
    context.textAlign = "center";       //horizentally center
    context.textBaselign = "middle";    // vertically center
    
    //Font Familiy
    context.font = "48px sans-serif";

    // String To Be Display
    var str = "Hello, World!"

    // Foregrounf color
    context.fillStyle = "white";
    // fill the text
    context.fillText(str, canvas.width/2, canvas.height/2);

}