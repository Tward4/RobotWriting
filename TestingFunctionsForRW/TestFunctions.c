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

        char myString[100];

// Read the content and print it
        while(fgets(myString, 100, FileT)) 
        {
        printf("%s", myString);
        }


        fclose(FileT);
    }
 


}