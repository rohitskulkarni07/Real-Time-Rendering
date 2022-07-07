#include <stdio.h>

typedef int MY_INT;

int main(void) {

    MY_INT Add(MY_INT, MY_INT);

    typedef int MY_INT;

    typedef float PVG_FLOAT;

    typedef char CHARACHTER;

    typedef double MY_DOUBLE;

    typedef unsigned int UINT;

    typedef UINT HANDLE;

    typedef HANDLE HWND;

    typedef HANDLE HINSTANCE;
    
    MY_INT a = 10, i;
    MY_INT iArray[] = { 9, 18, 27, 36, 45, 54, 63, 72, 81, 90};

    PVG_FLOAT f_pvg = 30.9f;
    const PVG_FLOAT f_pvg_pi = 3.14f;

    CHARACHTER ch = '*';
    CHARACHTER chArray_01[] = "Hello";
    CHARACHTER chArray_02[][10] = {"RTR", "BATCH", "2020-2021" };

    MY_DOUBLE d = 8.041997;

    UINT uint = 2456;
    HANDLE handle = 987;
    HWND hwnd = 9876;
    HINSTANCE hInstance = 14466;

    printf("\n\n");
    
    printf("Type MY_INT Variable a = %d\n", a);

    printf("\n\n");
    
    for(i=0; i < (sizeof(iArray) / sizeof(int)); i++) {
    
        printf("Type MY_INT Array Variable iArray[%d] = %d\n", i, iArray[i]);
    
    }
    
    printf("\n\n");
    printf("\n\n");

    printf("Type PVG_FLOAT Variable f = %f\n", f_pvg);
    printf("Type PVG_FLOAT constant  f_pvg_pi = %f\n", f_pvg_pi);

    printf("\n\n");
    printf("Type MY_DOUBLE Variable d = %lf\n", d);

    printf("\n\n");
    printf("Type CHARACTER Variable ch = %c\n", ch);

    printf("\n\n");
    printf("Type CHARACTER Array Variable chArray01 = %s\n", chArray_01);

    printf("\n\n");
    for(i=0; i < (sizeof(chArray_02) / sizeof(chArray_02[0])); i++) {
        
        printf("%s\t", chArray_02[i]);
    }
    printf("\n\n");

    printf("\n\n");
    printf("Type UINT Variable uint  = %u\n\n", uint);
    printf("Type HANDLE Variable handle = %u\n\n",handle);
    printf("Type HWND Variable hwnd = %u\n\n",hwnd);
    printf("Type HINSTANCE Variable hInstance = %u\n\n", hInstance);
    printf("\n\n");

    MY_INT x = 90;
    MY_INT y = 90;
    MY_INT ret;

    ret = Add(x, y);
    printf("ret = %d \n\n",ret);

    return(0);
}

MY_INT Add(MY_INT a, MY_INT b) {

    MY_INT c;
    c = a +b;
    return(c);

}
