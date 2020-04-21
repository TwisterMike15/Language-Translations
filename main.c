/*Group 2 Program 2
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu
Rudolph Hanzes - han7739@calu.edu

CSC 460
Language Translations
*/

#include "file_util.h"
#include "scanner.h"


file_outcome init(int argc, char* argv[]) {
    file_outcome fileCheck = openFiles(argc, argv);

    if (fileCheck == cont)
    {
        
    }

    return fileCheck;
}

void cleanup() {
    //closes files
    closeFiles();
}

int main(int argc, char* argv[]) {

    if (init(argc, argv) == cont) {
        
        initScanner(InpFile);
        printf("Scanner initialized. Parsing...\n");
        systemgoal();
        printf("Parsing complete.\n");
        cleanup();
    }
    else
    {
        printf("Init has failed. No scanner for you.");
    }

    return 0;
}
