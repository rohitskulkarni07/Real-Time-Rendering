class Stack {
    constructor() {
        this.items = [];
    }

    pushMatrix(t) {

        let a = mat4.create();
        
        a[0] = t[0];
        a[1] = t[1];
        a[2] = t[2];
        a[3] = t[3];
        a[4] = t[4];
        a[5] = t[5];
        a[6] = t[6];
        a[7] = t[7];
        a[8] = t[8];
        a[9] = t[9];
        a[10] = t[10];
        a[11] = t[11];
        a[12] = t[12];
        a[13] = t[13];
        a[14] = t[14];
        a[15] = t[15];
        this.items.push(a);
    }

    popMatrix() {
        if(this.items.length == 0) {
            return "uderflow";
        }
        return this.items.pop();
    }
}

