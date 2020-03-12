/*Group 2 Program 2
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu

CSC 460
Language Translations
*/

#include "file_util.h"
#include "scanner.h"


#define LINE_BUFF_SIZE 1024
#define TOKEN_BUFF_SIZE 1024
#define OUTFILE_BUFF_SIZE 1024
#define TOKEN_TYPE_SIZE 20

char lineBuff[LINE_BUFF_SIZE] = { '\0' };
char tokenBuff[TOKEN_BUFF_SIZE] = { '\0' };
char tokenType[TOKEN_TYPE_SIZE] = { '\0' };

char errorBuffer[OUTFILE_BUFF_SIZE] = { '\0' };    //for formatting lexical errors in listing file
char lisfileBuffer[OUTFILE_BUFF_SIZE] = { '\0' };  //for formatting numbered lines in listing file

char Lex_Err_Buff[LINE_BUFF_SIZE] = { '\0' };
int Lex_Err_Total = 0;
int Lex_Err_Index = 0;

int buffpos = 0;

int lineCount = 1;
int linePos = 0;

TokenId thistoken;





//*** Scanner Support Functions ***//

logical isDelimiter(char currchar) {
    return (currchar == ' ' || currchar == '\n' || currchar == '\t' || currchar == EOF);
}


//Non-destructive character lookahead. Returns capital, for easier use.
char peekChar() {
    char filechar = fgetc(InpFile);
    ungetc(filechar, InpFile);
    return toupper(filechar);
}


//Consumes a character from the InpFile's instream. Also prints lines & lex errors to LisFile
char consumeChar() {
    char filechar = fgetc(InpFile);

    //Check if a list needs to be printed to the lisfile
    if (filechar == '\n' || (filechar == EOF && linePos > 0)) {
        int i = 0;

        //Format the collected line. No need to clear buffer.
        sprintf(lisfileBuffer, "%2d   %s\n", lineCount, lineBuff);
        fputs(lisfileBuffer, LisFile);
        
        //Print each lex error
        for (i = 0; i < Lex_Err_Index; i++)
        {
            char currErr = Lex_Err_Buff[i];
            sprintf(errorBuffer, "Lexical Error on line %2d-   %c not recognized\n", lineCount, currErr);
            fputs(errorBuffer, LisFile);
        }
        Lex_Err_Index = 0; //Reset errors for next line

        //Clear error and line buffers
        memset(Lex_Err_Buff, '\0', LINE_BUFF_SIZE);
        memset(lineBuff, '\0', LINE_BUFF_SIZE);

        //Reset line index
        linePos = 0;
        lineCount++;
    }
    else {
        //Append character to lineBuffer.
        lineBuff[linePos] = filechar;
        linePos++;
    }

    return toupper(filechar);
}


//Consumes a digit. Also consumes the character given to the function (guaranteed to be a "-" or a digit)
void gatherDigit(char currchar) {
    logical stillDigit = ltrue;

    do {
        tokenBuff[buffpos] = currchar;
        buffpos++;
        currchar = peekChar();
        
        if (isdigit(currchar) && !isDelimiter(currchar) && buffpos < TOKEN_BUFF_SIZE) {
            consumeChar();
        }
        else
            stillDigit = lfalse;

    } while (stillDigit);
}

//Setts thistoken to the given token and places a tokenval into the tokenBuff. Ex: (GREATEREQUALOP, ">=")
void setToken(TokenId token, char* tokenvalue) {
    thistoken = token;

    //if NULL is provided, do nothing
    if (tokenvalue != NULL)
        strcpy_s(tokenBuff, TOKEN_BUFF_SIZE, tokenvalue);
}

void clearBuffer(char* buffer, int bufferSize)
{
    int i = 0;
    for (i; i < bufferSize; i++)
    {
        buffer[i] = '\0';
    }
}

char* decode(TokenId typeid)
{
    return (char*)TOKEN_TYPES[typeid];
}

TokenId getReservedId() {
    TokenId foundtoken = ID; //assume it's an ID, because 
    int i = 0;
    //only go up to 13 because that's the last enterable reserved word
    for (i = 0; i < 13; i++) {
        if (strcmp(tokenBuff, TOKEN_TYPES[i]) == 0) {
            foundtoken = i;
        }
    }

    return foundtoken;
}



TokenId getToken()
{
    thistoken = -1; //Claim no token found
    buffpos = 0;
    char currchar;

    do {
        currchar = consumeChar();

        if (isspace(currchar)) {
            //do nothing
        }
        else if (isalpha(currchar)) { //Compose ID/Reserved Word
            logical stillId = ltrue;

            do {
                tokenBuff[buffpos] = currchar;
                buffpos++;
                currchar = peekChar();
                if (isalnum(currchar) && !isDelimiter(currchar) && buffpos < TOKEN_BUFF_SIZE)
                    consumeChar();
                else
                    stillId = lfalse;

            } while (stillId);

            setToken(getReservedId(), NULL);
        }
        else if (isdigit(currchar)) { //Compose INTLITERAL
            gatherDigit(currchar);
            setToken(INTLITERAL, NULL);
        }
        else if (currchar == '-') { //check possible - paths (--COMMENT, -INTLITERAL, -)
            char lookahead = peekChar();

            if (lookahead == '-') { //compose a comment
                do {
                    currchar = consumeChar(); //consume chars until newline. consume the newline, too.
                } while (currchar != '\n' && currchar != EOF && buffpos < TOKEN_BUFF_SIZE);

            }
            else if (isdigit(lookahead)) {
                gatherDigit(currchar);
                setToken(INTLITERAL, NULL);
            }
            else {
                setToken(MINUSOP, "-");
            }

        }
        else if (currchar == ':') { //check possible : paths (:=, lex error)
            char lookahead = peekChar();

            if (lookahead == '=') {
                consumeChar();
                setToken(ASSIGNOP, ":=");
            }
            else {
                setToken(LEXERR, NULL);

                tokenBuff[buffpos] = currchar;
                Lex_Err_Buff[Lex_Err_Index] = currchar;
                Lex_Err_Index++;
                Lex_Err_Total++;
            }
        }
        else if (currchar == '<') { //check possible : paths (<=, <>, <)
            char lookahead = peekChar();

            if (lookahead == '=') {
                consumeChar();
                setToken(LESSEQUALOP, "<=");
            }
            else if (lookahead == '>') {
                consumeChar();
                setToken(NOTEQUALOP, "<>");
            }
            else {
                setToken(LESSOP, "<");
            }
        }
        else if (currchar == '>') { //check possible : paths (>=, >)
            char lookahead = peekChar();

            if (lookahead == '=') {
                consumeChar(); //eat the '='
                setToken(GREATEREQUALOP, ">=");
            }
            else {
                setToken(GREATEROP, ">");
            }
        }
        else if (currchar == '(') {
            setToken(LPAREN, "(");
        }
        else if (currchar == ')') {
            setToken(RPAREN, ")");
        }
        else if (currchar == ';') {
            setToken(SEMICOLON, ";");
        }
        else if (currchar == ',') {
            setToken(COMMA, ",");
        }
        else if (currchar == '+') {
            setToken(PLUSOP, "+");
        }
        else if (currchar == '*') {
            setToken(MULTOP, "*");
        }
        else if (currchar == '/') {
            setToken(DIVOP, "/");
        }
        else if (currchar == '!') {
            setToken(NOTOP, "!");
        }
        else if (currchar == '=') {
            setToken(EQUALOP, "=");
        }
        else if (currchar == EOF) {
            setToken(SCANEOF, "EOF");
        }
        else {
            tokenBuff[buffpos] = currchar;
            thistoken = LEXERR;

            Lex_Err_Buff[Lex_Err_Index] = currchar;
            Lex_Err_Index++;
            Lex_Err_Total++;
        }
    } while (thistoken == -1 && currchar != EOF);

    return thistoken;
}



//*** Primary Scanner Function ***//

void scanner(FILE* InpFile)
{

    char printBuffer[OUTFILE_BUFF_SIZE] = { '\0' };

    TokenId thistoken;

    do {
        thistoken = getToken();

        //parsely goes here

        char* tokentypeptr = decode(thistoken);

        clearBuffer(printBuffer, OUTFILE_BUFF_SIZE);
        sprintf(printBuffer, "Token Number: %-12dToken Type: %-15sActual Token: %-15s\n", thistoken, tokentypeptr, tokenBuff);
        fputs(printBuffer, OutFile);

        clearBuffer(tokenBuff, TOKEN_BUFF_SIZE);
    } while (thistoken != SCANEOF);


    clearBuffer(printBuffer, OUTFILE_BUFF_SIZE);
    sprintf(printBuffer, "Number of total errors: %d\n", Lex_Err_Total);
    fputs(printBuffer, LisFile);

}