#include <stdio.h>
#include <stdlib.h>

//This function creates an array for the Font Data

int main()
{
        char WordBuffer[10]="heelo";
        int WordLength = 0;
        int AsciiBuffer = 0;
        scanf("%s%n",WordBuffer,WordLength);

        int ii;
        for (ii=0; ii<WordLength;ii++)
        {
            printf("%d",WordLength);

            AsciiBuffer=(int)(WordBuffer)[ii];
        }
        return 0;
}

