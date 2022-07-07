#include <stdio.h>

int main(void) {

    int rsk_length, rsk_breadth, rsk_area;

    struct MyPoint {
 
        int rsk_x;
        int rsk_y;

    };

    struct Rectangle {

        struct MyPoint point_01;
        struct MyPoint point_02;
    
    };
    struct Rectangle rect = {{2, 3}, {5, 6}};

    rsk_length = rect.point_02.rsk_y - rect.point_01.rsk_y;
    
    if(rsk_length > 0) {
        rsk_length = rsk_length* -1;
    }

    rsk_breadth = rect.point_02.rsk_x - rect.point_01.rsk_x;

    rsk_area = rsk_length * rsk_breadth;

    printf("\n\n");
    printf("Length  Of Rectangle = %d\n\n", rsk_length);
    printf("Breadth Of Rectangle = %d\n\n", rsk_breadth);
    printf("Area    Of Rectangle = %d\n\n", rsk_area);

    return(0);
}
