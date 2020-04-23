/*Group 2 Program 3
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu
Rudolph Hanzes - han7739@calu.edu

CSC 460
Language Translations
*/

#include "file_util.h"

#define FILENAME_MAXSZ 100

typedef enum { none, overwrite, tryagain, userquit } overwriteoption;




//***Extension Manipulation Functions***//

void appendExtension(char* FileName, char* DesiredExtension) {
    //If no '.' (extension) was given:
    if (strrchr(FileName, '.') == NULL) {
        //Append extension
        strcat_s(FileName, FILENAME_MAXSZ, DesiredExtension);
    }
}

void stripExtension(char* FileName) {
    //find extension position
    char* extpos = strrchr(FileName, '.');
    if (extpos != NULL) {
        extpos[0] = '\0';
    }
}

void setExtension(char* FileName, char* DesiredExtension) {
    stripExtension(FileName);
    appendExtension(FileName, DesiredExtension);
}






//***Support Functions***//

logical getSameNameSelection()
{
    char choice[100] = { 0 };
    overwriteoption selection = none;
    printf("The file you selected already exists. \n");


    do {
        printf("Enter one of the following characters \n\t'o': Overwrite\n\t'n': New Filename (Try again)\n\t'q': Quit\n Your selection: ");
        gets(choice);
        if (strlen(choice) == 1)
        {
            switch (choice[0]) {
            case 'o':
                selection = overwrite;
                break;
            case 'n':
                selection = tryagain;
                break;
            case 'q':
                selection = userquit;
                break;
            default:
                printf("**Please enter a valid input.** \n");
            }
        }
        else {
            printf("**Please enter a valid input.** \n");
        }
    } while (selection == none);

    return selection;
}




//***File Management Functions***//

void copyToFile(FILE* DstFile, FILE* SrcFile)
{
    printf("In CopyToFile\n");
    rewind(SrcFile);
    char currentChar;
    while ((currentChar = fgetc(SrcFile)) != EOF)
    {
        fputc(currentChar, DstFile);
        printf("Looping\n");
    }

    rewind(SrcFile);
}

logical createBackup(char* OutFileName)
{
    logical success = lfalse;
    char bckfilename[FILENAME_MAXSZ] = { 0 };

    strcpy_s(bckfilename, FILENAME_MAXSZ, OutFileName);
    setExtension(bckfilename, ".bak");

    fopen_s(&BckFile, bckfilename, "w");
    copyToFile(BckFile, OutFile);
    if (BckFile != NULL) {
        fclose(BckFile); //close backfile
        success = ltrue;
        printf("Successfully created backup %s\n", bckfilename);
    }
    else {
        printf("Failure creating backup %s. ", bckfilename);
        success = lfalse;
    }

    return success;
}

logical getinpfile(char* InpFileName, logical PromptFile) {
    logical askagain = lfalse;
    logical aborted = lfalse;

    do {
        if (PromptFile) {
            printf("Please enter an input file name, or 'Q'/'' to quit: ");
            gets(InpFileName);
        }

        char firstchar = tolower(InpFileName[0]);
        int len = strlen(InpFileName);

        //if input is 'q'
        if (PromptFile && ((firstchar == 'q' && len == 1) || len == 0)) {
            printf("You have elected to exit. ");
            aborted = ltrue;
        }
        else {
            appendExtension(InpFileName, INPFILE_EXTENSION);

            //Open the file
            fopen_s(&InpFile, InpFileName, "r");

            if (InpFile == NULL) {
                printf("Could not find %s! Please try again. \n", InpFileName);
                askagain = lfalse;
            }
            else {
                askagain = ltrue;
            }
        }

        //Default the flag that prompts the user rather than uses cmdline args
        PromptFile = ltrue;

    } while (askagain == lfalse && aborted == lfalse);

    //Close all file handles.
    if (InpFile != NULL) {
        fclose(InpFile);
    }

    return aborted; //indicate whether the user quit or not
}



logical getOutfile(char* OutFileName, char* InpFileName, logical PromptFile) {
    logical askagain = lfalse;
    logical aborted = lfalse;
    char Bad_Out_File[] = "<>\":/\\|?*";

    do {
        if (PromptFile)
        {
            printf("Please enter an output file name, enter to use your input file, or q to quit: ");
            gets(OutFileName);
        }

        char firstchar = tolower(OutFileName[0]);
        int len = strlen(OutFileName);

        //if input is 'q'
        if (firstchar == 'q' && len == 1) {
            aborted = ltrue;
        }
        else {

            if (len == 0)
            {
                //if provided name is '', use input filename w/ output extension

                strcpy_s(OutFileName, FILENAME_MAXSZ, InpFileName);
                setExtension(OutFileName, OUTFILE_EXTENSION);

                printf("Using input file name for output\n");
            }
            else {
                //if anything else was provided, slap an extension on it.
                appendExtension(OutFileName, OUTFILE_EXTENSION);
            }

            printf("Prospective output file name: %s\n", OutFileName);


            //File name is fully composed by now. Check if invalid, then process
            if (strcmp(InpFileName, OutFileName) == 0) {
                printf("I don't think you understand what Read/Write access means. Try again, using different names.\n");
                askagain = ltrue;
            }
            else {
                if (strpbrk(Bad_Out_File, OutFileName) == NULL)
                {
                    fopen_s(&OutFile, OutFileName, "r");

                    //---------------------Check to see if file exists or not-----------

                    if (OutFile != NULL)
                    {
                        //Trying to overwrite existing file? Check options
                        overwriteoption selection = getSameNameSelection();
                        logical backupsuccessful;

                        switch (selection) {
                        case overwrite:
                            backupsuccessful = createBackup(OutFileName);
                            if (backupsuccessful)
                                askagain = lfalse;
                            else
                                aborted = ltrue;

                            break;
                        case tryagain:
                            askagain = ltrue;
                            break;
                        case userquit:
                            aborted = ltrue;
                            break;
                        default:
                            printf("Something has gone horribly wrong\n");
                            aborted = ltrue;
                            break;
                        }

                    }
                    else
                    {
                        askagain = lfalse;
                    }
                }
                else {
                    PromptFile = ltrue;
                    askagain = ltrue;
                    printf("File name invalid\n");
                }
            }
        }

        //Default the flag that prompts the user rather than uses cmdline args
        PromptFile = ltrue;
    } while (askagain == ltrue && aborted == lfalse);

    if (OutFile != NULL)
    {
        fclose(OutFile);
    }

    return aborted; //indicate whether the user quit or not
}








//*** Exported functions ***//

file_outcome openFiles(int argc, char* argv[]) {
    //data declarations for main
    logical promptinpfile = ltrue;
    logical promptoutfile = ltrue;
    logical abort;
    file_outcome fileCheck = quit;

    char inpfilename[FILENAME_MAXSZ] = { 0 };
    char outfilename[FILENAME_MAXSZ] = { 0 };
    char lisfilename[FILENAME_MAXSZ] = { 0 };
    char tmpfilename[FILENAME_MAXSZ] = "unlimited.power";

    //Checks for arg 1
    if (argc > 1) {
        promptinpfile = lfalse;
        strcpy_s(inpfilename, FILENAME_MAXSZ, argv[1]);
    }

    //Checkes for arg 2
    if (argc > 2) {
        promptoutfile = lfalse;
        strcpy_s(outfilename, FILENAME_MAXSZ, argv[2]);
    }


    abort = getinpfile(inpfilename, promptinpfile);

    if (abort == lfalse) {
        printf("Using input file %s\n", inpfilename);
        abort = getOutfile(outfilename, inpfilename, promptoutfile);

        if (abort == lfalse) {

            //Fix up the listing file name to back the outfile name
            strcpy_s(lisfilename, FILENAME_MAXSZ, outfilename);
            setExtension(lisfilename, LISFILE_EXTENSION);

            if (strcmp(outfilename, lisfilename) == 0) {
                printf("Listing filename same as outfilename. You're being mean, I don't want to talk to you anymore.");
                abort = ltrue;
            }
            else {
                //Prints for file names
                printf("\n\nCongratulations on getting this far. Your files:\n");
                printf("Input file     %s\n", inpfilename);
                printf("Output file    %s\n", outfilename);
                printf("Listing file   %s\n", lisfilename);
                printf("Temp file      %s\n", tmpfilename);

                //Opens Files
                fopen_s(&InpFile, inpfilename, "r");
                fopen_s(&OutFile, outfilename, "w");
                fopen_s(&LisFile, lisfilename, "w");
                fopen_s(&TmpFile, tmpfilename, "w");

                fileCheck = cont;
            }
        }
    }

    if (abort) {
        printf("Aborting program.\n");
    }


    return fileCheck;
}

void closeFiles() {
    if (InpFile != NULL) {
        fclose(InpFile);
    }
    if (TmpFile != NULL) {
        fclose(TmpFile);
    }
    if (OutFile != NULL) {
        fclose(OutFile);
    }
    if (LisFile != NULL) {
        fclose(LisFile);
    }
}

