#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include <windows.h>
#include "rs232.h"
#include "serial.h"
#include <string.h>
#include <stdbool.h>


#define bdrate 115200               /* 115200 baud */





//Structure Stores the Letters from Stroke Coords.
struct point
{ 
    int x;
    int y;
    int p;
};

//Structure Stores Coordinate data 
struct Strokes
{
    float x;
    float y;
    float p;
};






//FUnction converting the Letters to Ascii to Font code.
void LettersToAscii(struct point *pArray, struct Strokes *pStrokes, float CharacterFraction, char WordBuffer[10], float *XCoord, float *YCoord, char *buffer); 

//Function converting Font code to G-Code.
void FontCodeToGCode(struct Strokes *pStroke, char *buffer, int StrokeBuffer);

void SendCommands (char *buffer );

int main()
{


    char buffer[100];
    float XCoord = 0;   //XCoordinate Preset 
    float YCoord = -10;  //YCoordinate Preset

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









    //Opening Font File 
    FILE *fFontTxt;

    fFontTxt = fopen("FontTextTest.txt","r");   
 
    if (fFontTxt == NULL) 
    {
        printf("The file is not opened. The program will now exit.");

        
        return 1;
    }   


    //Allocating memory to struct
    struct point *pArray = (struct point *)calloc((1027*3), sizeof(struct point));  

    if (pArray == NULL) 
    {
        printf("Memory allocation for Array failed!\n");
        
        free(pArray);
        return 1;
    }

    //Storing Stroke Font data in a struct
    int i;
    for (i=0; i<1027;i++)
    {
        //This variable now reads the File data
        fscanf(fFontTxt,"%d%d%d",&pArray[i].x,&pArray[i].y,&pArray[i].p); 
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

        return 1;
    }   

    printf("Successfully opened %s",FileName);


    //Selecting Character Size
    int CharacterSize;      
    FontSize:
    printf("\nPlease enter font size between 4 and 10: ");
    scanf("%d",&CharacterSize);


    //Check if font size is within limit of 4 to 10
    if (CharacterSize >= 11 || CharacterSize <=3)
    {
        printf("Charasize of %d is not possible, please try again.",CharacterSize);
        goto FontSize;
    }
    printf("Font Size %d selected",CharacterSize);

    //Creating font fraction to multiply against font coordinates to get true size
    float CharacterFraction = 0;
    CharacterFraction = (float)CharacterSize/18;


    //Allocating Memory for WordBuffer
    char *WordBuffer = calloc(20, sizeof(char));    

    if (WordBuffer == NULL) 
    {
        printf("The file is not opened. The program will now exit.");

        
    return 1;
    }

    //Storing Read Word in Buffer
    while (fscanf(fWordFile,"%s",WordBuffer) == 1)
    {
           

            
        //Allocating memory to struct
        struct Strokes *pStrokes = (struct Strokes *)calloc((1027*3), sizeof(struct Strokes));

        if (pStrokes == NULL) 
        {
            printf("Memory allocation for Array failed!\n");

            free(pStrokes);
            return 1;
        }

            //Function called inside While loop, if there is a word remaining the function will be processed
            LettersToAscii(pArray, pStrokes, CharacterFraction, WordBuffer, &XCoord, &YCoord, buffer);
           
            //Freeing up space in pStroke Structure after every word has been sent
            free(pStrokes);
    }




    //Freeing up space in pArray Structure
    free(pArray);

    //Closing files after use
    fclose(fWordFile);
    fclose(fFontTxt);






    // Before we exit the program we need to close the COM port
    CloseRS232Port();
    printf("Com port now closed\n");

    return (0);
}


//Structure which reads the letters into Ascii units and exports them as coordinates from a font file
void LettersToAscii(struct point *pArray, struct Strokes *pStrokes, float CharacterFraction,char WordBuffer[10], float *XCoord, float *YCoord, char *buffer)
{
    int i;
    int ii;
    int iii;
    int LetterStrokes = 0;
    int StrokeBuffer = 0;
    int WordLength = 0;
    int AsciiBuffer = 0;
    int StrokeBufferSignal = 0;

    //Counts the letters in the word    
    while (WordBuffer[WordLength] != '\0') 
    {
        WordLength ++;
    }

              
    //For loop which has a range in the amount of letters in the word
    for (i=0; i<WordLength;i++)
    {
        int LineinFontFile = 0;    
        float MaxLetterLength = 0;   
        float MaxLetterHeight = 0;
             
        //Converting letters into the corresponding Ascii unit
        AsciiBuffer=(int)(WordBuffer[i]);

        //Finding the corresponding Ascii unit
        for (ii=0; ii<1027; ii++)
        {
            if (pArray[ii].x==999 && pArray[ii].y==(AsciiBuffer))
            LineinFontFile = ii;
             
                
        }
           
        //Stores how many letter stroke commands are needed for the Ascii unit
        LetterStrokes= pArray[LineinFontFile].p; 

        //Stores the font coordinates in a new struct    
        for (iii=0; iii<LetterStrokes; iii++)  
        {
            MaxLetterLength = 0;
            pStrokes[iii+StrokeBuffer].x = ((CharacterFraction)*((float)pArray[LineinFontFile+iii+1].x))+*XCoord;   //Multipling Scale to get true font size.
            pStrokes[iii+StrokeBuffer].y = ((CharacterFraction)*((float)pArray[LineinFontFile+iii+1].y))+(*YCoord-CharacterFraction);   //Multipling Scale to get true font size.
            pStrokes[iii+StrokeBuffer].p = ((float)pArray[LineinFontFile+iii+1].p);

                    
            //Finding max letter length
            if (MaxLetterLength <= CharacterFraction*((float)pArray[LineinFontFile+iii+1].x))         
            {
                MaxLetterLength += CharacterFraction*((float)pArray[LineinFontFile+iii+1].x);
            }
            
            //Finding max letter height
            if (MaxLetterHeight <= CharacterFraction*(float)pArray[LineinFontFile+iii+1].y)        
            {
                MaxLetterHeight += CharacterFraction*(float)pArray[LineinFontFile+iii+1].y;
            }
            
            //At the end of the letter, store the max letter width for ofset   
            if (iii == LetterStrokes-1)         
            {
                (*XCoord) += (float)MaxLetterLength;
                
                //Test if X Coordinates go over 100
                if (*XCoord >= 100)          
                {        
                    //Ofset in Y if X axis is full
                    (*YCoord) -= (float)(MaxLetterHeight)+2;
                    *XCoord = 0;
                    
                    StrokeBuffer = 0;
                    memset(pStrokes, 0, sizeof(struct Strokes));
                    AsciiBuffer = 0;
                    int r;

                    //Re-doing the start of the function with stroke Coordinates but now with the Y ofset
                    for (r=0; r<WordLength; r++)
                    {
                        
                        AsciiBuffer=(int)(WordBuffer[r]);

                        for (ii=0; ii<1027; ii++)
                        {           
                            if (pArray[ii].x==999 && pArray[ii].y==(AsciiBuffer))
                            LineinFontFile = ii;   
                        }

                        LetterStrokes= pArray[LineinFontFile].p; 

                        for (iii=0; iii<LetterStrokes; iii++)  
                        {
                            MaxLetterLength = 0;
                            pStrokes[iii+StrokeBuffer].x = ((CharacterFraction)*((float)pArray[LineinFontFile+iii+1].x))+*XCoord;   
                            pStrokes[iii+StrokeBuffer].y = ((CharacterFraction)*((float)pArray[LineinFontFile+iii+1].y))+(*YCoord-CharacterFraction); 
                            pStrokes[iii+StrokeBuffer].p = ((float)pArray[LineinFontFile+iii+1].p);  

                              
                            if (MaxLetterLength <= CharacterFraction*((float)pArray[LineinFontFile+iii+1].x))        
                            {
                                MaxLetterLength += CharacterFraction*((float)pArray[LineinFontFile+iii+1].x);
                            }
            
            
            
                            if (MaxLetterHeight <= CharacterFraction*(float)pArray[LineinFontFile+iii+1].y)        
                            {
                                MaxLetterHeight += CharacterFraction*(float)pArray[LineinFontFile+iii+1].y;
                            }

                            if (iii == LetterStrokes-1)         
                            {
                                (*XCoord) += (float)MaxLetterLength;
                            }
                        
                        }
                        //Adding to the Buffer to record the amount of strokes for the word
                        StrokeBuffer += iii;
                    
                    }
                    //Identifies if the sentence needed another line
                    StrokeBufferSignal = 1;
                    goto Gcode;
                    
                }
               
                //Checks if the page has ran out at the limit of -50 in the Y axis
                if (*YCoord <= -50)
                {
                    printf("\nRobot has run out of space on page");
                    exit(1);
                }

            }
                
                

        }
     
    
                
    //Check if the Word has gone onto the next Sentence, if not the Buffer adds. If statement makes sure it doesnt count twice            
    if (StrokeBufferSignal == 0)
    {
    StrokeBuffer += iii;
    }


            
    }
//Calls Function GCode which converts font coordinates to GCode values
Gcode:
FontCodeToGCode(pStrokes, buffer, StrokeBuffer);
 
}

void FontCodeToGCode(struct Strokes *pStrokes, char *buffer, int StrokeBuffer)
{
    int j=0;
    
    //For loop in range of the word
    for (j = 0; j<StrokeBuffer; j++)
    {

        //If Else statement allows GCode to know if its a pen up or down command.
        if (pStrokes[j].p == 0.000000)
        {
            if (pStrokes[j-1].p != 0.000000)
            {
                sprintf(buffer, "S%.0f\n",pStrokes[j].p);
                SendCommands(buffer);
            }
        
        }
        else
        {
            if (pStrokes[j-1].p !=1.000000)
            {
                sprintf(buffer, "S%.0f000\n",pStrokes[j].p);
                SendCommands(buffer);
            }
        }

        //Sending GCode commands through a buffer
        sprintf(buffer,"G%.0f X%.2f Y%.2f\n",pStrokes[j].p,pStrokes[j].x,pStrokes[j].y);

        //Calling GCode to Robot sending function
        SendCommands(buffer);
    }
}




// Send the data to the robot - note in 'PC' mode you need to hit space twice
// as the dummy 'WaitForReply' has a getch() within the function.
void SendCommands (char *buffer )
{
 
    PrintBuffer (&buffer[0]);
    WaitForReply();
    Sleep(100); 

}
