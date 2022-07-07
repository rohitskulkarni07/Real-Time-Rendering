#include <stdio.h>

int main(void) {

    int rsk_length, rsk_breadth, rsk_area;

    struct Reactangle {

        struct MyPoint {
        
            int rsk_x;
            int rsk_y;

        }point_01,point_02;
    
    }rect;

    printf("\n\n");
    printf("Enter Leftmost X-Cordinate Of Reactangle : ");
    scanf("%d", &rect.point_01.rsk_x);

    printf("\n\n");
    printf("Enter Bottommost Y-Cordinate Of Reactangle : ");
    scanf("%d", &rect.point_01.rsk_y);

    printf("\n\n");
    printf("Enter Rightmost X-Cordinate Of Reactangle : ");
    scanf("%d", &rect.point_02.rsk_x);

    printf("\n\n");
    printf("Enter Topmost Y-Cordinate Of Reactangle : ");
    scanf("%d", &rect.point_02.rsk_y);

    rsk_length = rect.point_02.rsk_y - rect.point_01.rsk_y;
    if(rsk_length < 0) {
        rsk_length = rsk_length* -1;
    }   

    rsk_breadth = rect.point_02.rsk_x - rect.point_01.rsk_y;
    if (rsk_breadth < 0) {
        rsk_breadth = rsk_breadth* -1;
    }

    rsk_area = rsk_length * rsk_breadth;

    printf("\n\n");
    printf("Length  Of Reactangle = %d\n\n", rsk_length);
    printf("Breadth Of Reactangle = %d\n\n", rsk_breadth);
    printf("Area    Of Reactangle = %d\n\n", rsk_area);

    return(0);
    
}
