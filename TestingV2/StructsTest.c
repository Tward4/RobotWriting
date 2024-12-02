#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point
{ 
    int x;
    int y;
    int p;
};



//Structure converting the Letters to Ascii.
int LettersToAscii(struct point *pArray,int CharacterSize,char WordBuffer); 




int main()
{

    char WordBuffer[10];

    FILE *fFontTxt;

    fFontTxt = fopen("FontTextTest.txt","r");
 
    if (fFontTxt == NULL) 
    {
        printf("The file is not opened. The program will now exit.");

        return -1;
    }   


    
    struct point *pArray = (struct point *)malloc(sizeof(struct point));
    
    int i;
    for (i=0; i<1028;i++)
    {
        fscanf(fFontTxt,"%d%d%d",&pArray[i].x,&pArray[i].y,&pArray[i].p); //This now reads the File data
        int test = (pArray[i].x);
        printf("\n%d",test);

        if (pArray[i].x == 999)
        {
            printf("ok");
        }
    }
    
    //Reading the Text Word File

    char FileName[50];

    FileSelection:

    printf("\nPlease enter file name: ");
    scanf("%s",FileName);
    strcat(FileName,".txt");

    FILE *fWordFile;
    fWordFile = fopen(FileName,"r");

     if (fWordFile == NULL) 
    {
        printf("The file has not been found. Please try again.");

        goto FileSelection;

        return -1;
    }   

    printf("Successfully opened %s",FileName);

    fscanf(fWordFile,"%s",WordBuffer);
    printf("\n%s",WordBuffer);

// Font size question.

    int CharacterSize;
    FontSize:
    printf("\nPlease enter font size between 4 and 10: ");
    scanf("%d",&CharacterSize);

// Check if font size is within limit

    if (CharacterSize >= 11 || CharacterSize <=3)
    {
        printf("Charasize of %d is not possible, please try again.",CharacterSize);
        goto FontSize;
    }
    printf("Font Size %d selected",CharacterSize);

  
    fclose(fWordFile);
    fclose(fFontTxt);
}
            
            
        
            
int LettersToAscii(struct point *pArray,int CharacterSize,char WordBuffer)
{
        int WordLength = 0;
        int AsciiBuffer = 0;
        scanf("%s%n",&WordBuffer,&WordLength);

        int ii;
        int iii;
        int CharacterSizeBuffer = 0;
        int LetterStrokes = 0;
        for (ii=0; ii<WordLength;ii++)
        {
            printf("%d",WordLength);

            AsciiBuffer=(int)(&WordBuffer)[ii]; //converting letters into the corresponding Ascii numbers.

            scanf("%n%n", &pArray[AsciiBuffer].p,&LetterStrokes); //finds how many letter strokes are needed for Ascii number.

            for (iii=AsciiBuffer; iii<iii+LetterStrokes; iii++)
            {
                CharacterSizeBuffer = pArray[iii].p;     //
                
            }
            
        }
        return iii;
}





