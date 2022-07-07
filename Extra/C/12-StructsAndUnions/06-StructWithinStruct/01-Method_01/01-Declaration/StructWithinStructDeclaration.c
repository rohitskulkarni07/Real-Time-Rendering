#include <stdio.h>

struct Reactangle {

    struct MyPoint {
        int x;
        int y;

    }point_01,point_02;
    
}rect;

int main(void) {

    int length, breadth, area;

    printf("\n\n");
    printf("Enter Leftmost X-Cordinate Of Reactangle : ");
    scanf("%d", &rect.point_01.x);

    printf("\n\n");
    printf("Enter Bottommost Y-Cordinate Of Reactangle : ");
    scanf("%d", &rect.point_01.y);

    printf("\n\n");
    printf("Enter Rightmost X-Cordinate Of Reactangle : ");
    scanf("%d", &rect.point_02.x);

    printf("\n\n");
    printf("Enter Topmost Y-Cordinate Of Reactangle : ");
    scanf("%d", &rect.point_02.y);

    length = rect.point_02.y - rect.point_01.y;
    if(length < 0) {
        length = length* -1;
    }   

    breadth = rect.point_02.x - rect.point_01.y;
    if (breadth < 0) {
        breadth = breadth* -1;
    }

    area = length * breadth;

    printf("\n\n");
    printf("Length  Of Reactangle = %d\n\n", length);
    printf("Breadth Of Reactangle = %d\n\n", breadth);
    printf("Area    Of Reactangle = %d\n\n", area);

    return(0);
    
}
