package com.example.ogl27;

class MyStack {
   
    private float[][] stack;
    private int top;
    private int size; 

    private final int FAIL = -1;
    private final int SUCCESS = 0;

    public MyStack(int size) {
        this.size = size;
        this.top = -1;
        stack = new float[this.size][16];
    }

    public int pushMatrix(float[] arr) {
        
        if(top == size-1) {
            return FAIL;
        }
        top++;
        for(byte i = 0; i < 16; i++) {
            stack[top][i] = arr[i];
        }
        return SUCCESS;
    }

    public int popMatrix(float[] arr) {

        if(top == -1) {
            return FAIL;
        }

        for(byte i = 0; i < 16; i++) {
            arr[i] = stack[top][i];
        }
        top--;
        return SUCCESS;
    }
}