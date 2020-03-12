/*Group 2 Program 2
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu

CSC 460
Language Translations
*/

#ifndef FILE_UTIL_H
#define FILE_UTIL_H




#include <stdio.h>
#include <string.h>


//***Enums***//
typedef enum { lfalse, ltrue } logical;
typedef enum { cont, quit } file_outcome;


//***Character Arrays***//

static char INPFILE_EXTENSION[] = ".in";
static char OUTFILE_EXTENSION[] = ".out";
static char LISFILE_EXTENSION[] = ".lis";


//***Files***//
FILE* InpFile, * OutFile, * LisFile, * BckFile, * TmpFile;

//***Exported Functions***//
file_outcome openFiles(int, char* []);
void copyToFile(FILE*, FILE*);
void closeFiles(void);

#endif