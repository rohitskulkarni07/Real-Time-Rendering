#include <stdio.h>

struct MyPoint {
    int x;
    int y;
};

int main() {

    struct MyPoint point_A, point_B, point_C, point_D, point_E;

    printf("\n\n");
    
    printf("Enter X-Co-ordinate For Point 'A' : ");
    scanf("%d",&point_A.x);
    
    printf("Enter Y-Co-ordinate For Point 'A' : ");
    scanf("%d",&point_A.y);
    
    printf("\n\n");

    printf("\n\n");
    
    printf("Enter X-Co-ordinate For Point 'B' : ");
    scanf("%d",&point_B.x);
    
    printf("Enter Y-Co-ordinate For Point 'B' : ");
    scanf("%d",&point_B.y);
    
    printf("\n\n");

    printf("\n\n");
    
    printf("Enter X-Co-ordinate For Point 'C' : ");
    scanf("%d",&point_C.x);
    
    printf("Enter Y-Co-ordinate For Point 'C' : ");
    scanf("%d",&point_C.y);
    
    printf("\n\n");

    printf("\n\n");
    
    printf("Enter X-Co-ordinate For Point 'D' : ");
    scanf("%d",&point_D.x);
    
    printf("Enter Y-Co-ordinate For Point 'D' : ");
    scanf("%d",&point_D.y);
    
    printf("\n\n");

    printf("\n\n");
    
    printf("Enter X-Co-ordinate For Point 'E' : ");
    scanf("%d",&point_E.x);

    printf("Enter Y-Co-ordinate For Point 'E' : ");
    scanf("%d",&point_E.y);
    
    printf("\n\n");

    printf("Co-ordinates (x, y) Of Point 'A' Are : (%d, %d)\n\n",point_A.x, point_A.y);
    printf("Co-ordinates (x, y) Of Point 'B' Are : (%d, %d)\n\n",point_B.x, point_B.y);
    printf("Co-ordinates (x, y) Of Point 'C' Are : (%d, %d)\n\n",point_C.x, point_C.y);
    printf("Co-ordinates (x, y) Of Point 'D' Are : (%d, %d)\n\n",point_D.x, point_D.y);
    printf("Co-ordinates (x, y) Of Point 'E' Are : (%d, %d)\n\n",point_E.x, point_E.y);

    printf("\n\n");


    return(0);
}