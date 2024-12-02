#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point
{ 
    int x;
    int y;
    int p;
};

struct Strokes
{
    int x;
    int y;
    int p;
};





//Structure converting the Letters to Ascii.
void LettersToAscii(struct point *pArray, struct Strokes *pStroke, int CharacterSize,char WordBuffer[10]); 




int main()
{

    
    FILE *fFontTxt;

    fFontTxt = fopen("FontTextTest.txt","r");
 
    if (fFontTxt == NULL) 
    {
        printf("The file is not opened. The program will now exit.");

        
        return -1;
    }   


    
    struct point *pArray = (struct point *)calloc((1027*3), sizeof(struct point));

    if (pArray == NULL) 
    {
        printf("Memory allocation for Array failed!\n");
        
        free(pArray);
        return -1;
    }


    int i;
    for (i=0; i<1027;i++)
    {
        fscanf(fFontTxt,"%d%d%d",&pArray[i].x,&pArray[i].y,&pArray[i].p); //This now reads the File data

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

    char *WordBuffer = calloc(20, sizeof(char));

    if (WordBuffer == NULL) 
    {
        printf("The file is not opened. The program will now exit.");

        
    return -1;
    }


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

    struct Strokes *pStrokes = (struct Strokes *)calloc((1027*3), sizeof(struct Strokes));
    
    if (pStrokes == NULL) 
    {
        printf("Memory allocation for Array failed!\n");
        
        free(pStrokes);
        return -1;
    }

                      

    LettersToAscii(pArray, pStrokes, CharacterSize, WordBuffer);



    free(pArray);
    free(pStrokes);

    fclose(fWordFile);
    fclose(fFontTxt);


}
            
            
        
            
void LettersToAscii(struct point *pArray, struct Strokes *pStrokes, int CharacterSize,char WordBuffer[10])
{
        int WordLength = 0;
        int AsciiBuffer = 0;
        
        while (WordBuffer[WordLength] != '\0') //counts the letters
        {
            WordLength ++;
        }

        int i;
        int ii;
        int iii = 0;
        int LetterStrokes = 0;
        
            

        for (i=0; i<WordLength;i++)
        {
            int LineinFontFile = 0;            
            AsciiBuffer=(int)(WordBuffer[i]); //converting letters into the corresponding Ascii numbers.

            for (ii=0; ii<1027; ii++)
            {
                if (pArray[ii].x==999 && pArray[ii].y==(AsciiBuffer))
                LineinFontFile = ii;
                
            }

            

            LetterStrokes= pArray[LineinFontFile].p; //finds how many letter strokes are needed for Ascii number.

            

            for (iii=0; iii<LetterStrokes; iii++)  
            {
                
               pStrokes[iii].x = (pArray[LineinFontFile+iii+1].x)/CharacterSize;     // amending stroke coordinates, scaling them and adding to a new struct to store data for the word.
                pStrokes[iii].y = (pArray[LineinFontFile+iii+1].y)/CharacterSize;
                pStrokes[iii].p = (pArray[LineinFontFile+iii+1].p);

                printf("%d",pStrokes[iii].x);
                printf("%d",pStrokes[iii].y);
                printf("%d",pStrokes[iii].p);

                      
                
            }
            
            
            
        }
 
}





