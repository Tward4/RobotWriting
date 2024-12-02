#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include <windows.h>
#include "rs232.h"
#include "serial.h"

#define bdrate 115200               /* 115200 baud */

void SendCommands (char *buffer );

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

    //char mode[]= {'8','N','1',0};
    char buffer[100];

    // If we cannot open the port then give up immediately
    if ( CanRS232PortBeOpened() == -1 )
    {
        printf ("\nUnable to open the COM port (specified in serial.h) ");
        exit (0);
    }

    // Time to wake up the robot
    printf ("\nAbout to wake up the robot\n");

    // We do this by sending a new-line
    sprintf (buffer, "\n");
     // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    Sleep(100);

    // This is a special case - we wait  until we see a dollar ($)
    WaitForDollar();

    printf ("\nThe robot is now ready to draw\n");

        //These commands get the robot into 'ready to draw mode' and need to be sent before any writing commands
    sprintf (buffer, "G1 X0 Y0 F1000\n");
    SendCommands(buffer);
    sprintf (buffer, "M3\n");
    SendCommands(buffer);
    sprintf (buffer, "S0\n");
    SendCommands(buffer);

    


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

            
            
        
            





    // These are sample commands to draw out some information - these are the ones you will be generating.
    sprintf (buffer, "G0 X-13.41849 Y0.000\n");
    SendCommands(buffer);
    sprintf (buffer, "S1000\n");
    SendCommands(buffer);
    sprintf (buffer, "G1 X-13.41849 Y-4.28041\n");
    SendCommands(buffer);
    sprintf (buffer, "G1 X-13.41849 Y0.0000\n");
    SendCommands(buffer);
    sprintf (buffer, "G1 X-13.41089 Y4.28041\n");
    SendCommands(buffer);
    sprintf (buffer, "S0\n");
    SendCommands(buffer);
    sprintf (buffer, "G0 X-7.17524 Y0\n");
    SendCommands(buffer);
    sprintf (buffer, "S1000\n");
    SendCommands(buffer);
    sprintf (buffer, "G0 X0 Y0\n");
    SendCommands(buffer);

    // Before we exit the program we need to close the COM port
    CloseRS232Port();
    printf("Com port now closed\n");

    return (0);
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

// Send the data to the robot - note in 'PC' mode you need to hit space twice
// as the dummy 'WaitForReply' has a getch() within the function.
void SendCommands (char *buffer )
{
    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    WaitForReply();
    Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
}
