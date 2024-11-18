#include <stdio.h>
#include <stdlib.h>

int main()
{

    FILE *FileT;

    FileT = fopen("FileTest.txt","r");
 
    if (FileT == NULL) 
    {
        printf("The file is not opened. The program will now exit.");

        exit(0);
    }    
    else
    {
        printf("ok");

        char character = fgetc(FileT);
        printf("%s",character);

        fclose(FileT);
    }
 


}