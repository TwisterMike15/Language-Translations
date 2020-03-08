/*Group 2 Program 3
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu
Rudolph Hanzes - han7739@calu.edu

CSC 460
Language Translations
*/

#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <stdio.h>


//*****************************Constants*****************************//
typedef enum { lfalse, ltrue } logical;
typedef enum { cont, quit } file_outcome;

static const char INPFILE_EXTENSION[] = ".in";
static const char OUTFILE_EXTENSION[] = ".out";
static const char LISFILE_EXTENSION[] = ".lis";


//*****************************Attributes****************************//


FILE* InpFile, * OutFile, * LisFile, * BckFile, * TmpFile;


//*****************************Methods*******************************//
file_outcome openFiles(int, char* []);
void copyToFile(FILE*, FILE*);
void closeFiles(void);

#endif
