#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct point
{ 
    int x;
    int y;
    int p;
};

struct Strokes
{
    float x;
    float y;
    float p;
};






//FUnction converting the Letters to Ascii to Font code.
void LettersToAscii(struct point *pArray, struct Strokes *pStrokes, float CharacterFraction, char WordBuffer[10], float *XCoord, float *YCoord); 

//Function converting Font code to G-Code.
void FontCodeToGCode(struct Strokes *pStroke);





int main()
{
    float XCoord = 0;
    float YCoord = -5;


    
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

    int CharacterSize;      // Selecting Character Size
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
    float CharacterFraction = 0;
    CharacterFraction = (float)CharacterSize/18;



    while (fscanf(fWordFile,"%s",WordBuffer) == 1)
    {
            printf("\n%s",WordBuffer);

            // Font size question.

        struct Strokes *pStrokes = (struct Strokes *)calloc((1027*3), sizeof(struct Strokes));

        if (pStrokes == NULL) 
        {
            printf("Memory allocation for Array failed!\n");

            free(pStrokes);
            return -1;
        }

 
           LettersToAscii(pArray, pStrokes, CharacterFraction, WordBuffer, &XCoord, &YCoord);
           free(pStrokes);
    }


    printf("\ncount: %f",XCoord);
    printf("\ncount: %f",YCoord);





    free(pArray);

    fclose(fWordFile);
    fclose(fFontTxt);


}
            
            
        
            
void LettersToAscii(struct point *pArray, struct Strokes *pStrokes, float CharacterFraction,char WordBuffer[10], float *XCoord, float *YCoord)
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
        float MaxLetterLength = 0;   
        float MaxLetterHeight = 0;     
        AsciiBuffer=(int)(WordBuffer[i]); //converting letters into the corresponding Ascii numbers.

        for (ii=0; ii<1027; ii++)
        {
            if (pArray[ii].x==999 && pArray[ii].y==(AsciiBuffer))
            LineinFontFile = ii;
             
                
        }

            

        LetterStrokes= pArray[LineinFontFile].p; //finds how many letter strokes are needed for Ascii number.

            

        for (iii=0; iii<LetterStrokes; iii++)  
        {

            pStrokes[iii].x = ((CharacterFraction)*((float)pArray[LineinFontFile+iii+1].x))+*XCoord;   //Dividing Scale to get true font size.
            pStrokes[iii].y = ((CharacterFraction)*((float)pArray[LineinFontFile+iii+1].y))+(*YCoord-CharacterFraction);   //Dividing Scale to get true font size.
            pStrokes[iii].p = ((float)pArray[LineinFontFile+iii+1].p);







            
            
                if (MaxLetterLength <= CharacterFraction*(float)pArray[LineinFontFile+iii+1].x)        //Max Letter Length 
                {
                    MaxLetterLength += CharacterFraction*(float)pArray[LineinFontFile+iii+1].x;
                }
            
            
            
                if (MaxLetterHeight <= CharacterFraction*(float)pArray[LineinFontFile+iii+1].y)        //Max Letter Height
                {
                    MaxLetterHeight += CharacterFraction*(float)pArray[LineinFontFile+iii+1].y;
            
                
                }
            
               
            if (iii == LetterStrokes-1)
            {
                (*XCoord) += (float)MaxLetterLength;
                
                if (*XCoord >= 100)          //Test if X Coordinates go over 100.
                {        

                    *XCoord = 0;
                    (*YCoord) -= (float)(MaxLetterHeight)+5;
                }

                if (*YCoord <= -50)
                {
                    printf("\nRobot has run out of space on page");
                    exit(1);
                }

            }
                
                



            //printf("\n%f",pStrokes[iii].x);
            //printf("%f",pStrokes[iii].y);
            //printf("%f",pStrokes[iii].p);

                      
                
        }
            
            
            
    }
 
}

//void FontCodeToGCode(struct Strokes *pStroke)

