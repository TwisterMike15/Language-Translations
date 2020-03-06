/*Group 2 Program 3
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu
Rudolf Hanzes - han7739@calu.edu

CSC 460
Language Translations
*/

#include "file_util.h"
#include "scanner.h"
#include "parser.h"



file_outcome init(int argc, char* argv[]) {
    file_outcome fileCheck = openFiles(argc, argv);

    if (fileCheck == cont)
    {
        fputs("The temp", TmpFile);
    }

    return fileCheck;
}

void cleanup() {
    //closes files
    printLexErrors();
    closeFiles();
}

int main(int argc, char* argv[]) {

    if (init(argc, argv) == cont) {
        printf("\nScanning File\n");
        initScanner();
        systemgoal(); //Enter parser here
        
        cleanup();
        printf("Scan Complete\n");
    }
    else
    {
        printf("Init has failed. No scanner for you.");
    }

    return 0;
}