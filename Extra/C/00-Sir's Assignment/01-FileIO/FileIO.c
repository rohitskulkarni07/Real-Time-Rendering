#include <stdio.h>
#include <stdlib.h>

int main(void) {


    FILE *pFile = NULL;

    if(fopen_s(&pFile, "Log.txt", "w") != 0) {

        printf("Cannot Open Desired File EXITING NOW...\n\n");
        exit(0);
    }

    fprintf(pFile,"India Is My Country.\n");

    fclose(pFile);

    return(0);
}

