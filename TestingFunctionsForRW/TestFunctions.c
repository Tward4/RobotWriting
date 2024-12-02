#include <stdio.h>
#include <stdlib.h>

int main()
{

    char *FileFontTxt;

    FILE *FileT;

    FileT = fopen("FileTest.txt","r");
 
    if (FileT == NULL) 
    {
        printf("The file is not opened. The program will now exit.");

        exit(0);
    }    
    else
    {
        size_t FileSize;
        fseek(FileT,0,SEEK_END);

        FileSize = ftell(FileT);
        rewind(FileT);

        

        printf("%lld\n", FileSize);

        FileFontTxt = (char *)calloc(FileSize, sizeof(char)); //allocating memory

        if (FileFontTxt == NULL)
            {
                printf("Memory allocation failed!");
                fclose(FileT);
                return -1;
            }

        size_t BytesRead = 0;
        BytesRead = fread(FileFontTxt, sizeof(char),FileSize, FileT);
        if (BytesRead != FileSize)
        {
            printf("Error reading file");
            free(FileFontTxt);
            fclose(FileT);
            return -1;
        }

        FileFontTxt[FileSize] = '\0';

        printf("File Content: \n%s\n", FileFontTxt);
        free(FileFontTxt);
        fclose(FileT);

        return 0;

    }
 


}