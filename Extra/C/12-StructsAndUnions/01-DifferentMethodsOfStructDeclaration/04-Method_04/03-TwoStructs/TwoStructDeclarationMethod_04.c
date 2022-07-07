#include <stdio.h>

struct MyPoint {

    int x;
    int y;

};

struct MyPointProperties {

    int quadrant;
    char axis_location[10];

};

int main(void) {

    struct MyPoint point;
    struct MyPointProperties point_proprties;
    

    printf("\n\n");

    printf("ENter X-Cordinate For A point : ");
    scanf("%d", &point.x);
    printf("ENter Y-Cordinate For A point : ");
    scanf("%d", &point.y);

    printf("\n\n");
    printf("POint Co-ordinate (x, y) Are : (%d, %d) !!!\n",point.x, point.y);

    if((point.x == 0) && (point.y == 0)) {
        printf("Point Co-ordinates (x, y) Are  Origin: (%d, %d) !!!\n",point.x, point.y);

    } else {

        if (point.x == 0) {
         
            if(point.y < 0) {
                strcpy(point_proprties.axis_location,"Negative Y");
            }
            
            if(point.y > 0) {
                strcpy(point_proprties.axis_location,"Positive Y");
            }
            
            point_proprties.quadrant = 0;
            printf("The Point Lies On THe %s Axis !!!\n", point_proprties.axis_location);
        } else if(point.y == 0) {

             if(point.x < 0) {
                strcpy(point_proprties.axis_location,"Negative X");
            }
            
            if(point.x > 0) {
                strcpy(point_proprties.axis_location,"Positive X");
            }
            
            point_proprties.quadrant = 0;
            printf("The Point Lies On THe %s Axis !!!\n", point_proprties.axis_location);
        } else {

            point_proprties.axis_location[0] = '\0';

            if(point.x > 0 && point.y > 0) {
                point_proprties.quadrant =1;
            } else if(point.x < 0 && point.y > 0) {
                point_proprties.quadrant = 2;
            } else if(point.x < 0 && point.y < 0) {
                point_proprties.quadrant = 3;
            } else {
                point_proprties.quadrant = 4; 
            }
            
            printf("The Point Lies In Quadrant Number %d !!!\n", point_proprties.quadrant);
    
        }

    }

    return(0);
}