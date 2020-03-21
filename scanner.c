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
#include "scanner.h"

char LineBuff[LINE_BUFF_SIZE]           = { '\0' };
char LisFileBuffer[OUTFILE_BUFF_SIZE]   = { '\0' };  //for formatting numbered lines in listing file
char LexErrBuff[LINE_BUFF_SIZE]         = { '\0' };
char ErrorBuffer[OUTFILE_BUFF_SIZE]     = { '\0' };  //for formatting lexical errors in listing file
char TokenOutputBuffer[OUTFILE_BUFF_SIZE] = { '\0' };

int LexErrTotal = 0;
int LexErrIndex = 0;
int numOfLineTokens = 0;

int LineCount = 1;
int LinePos = 0;







//*** Scanner Support Functions ***//

//Sets thistoken to the given token and places a tokenval into the tokenBuff. Ex: (GREATEREQUALOP, ">=")
void setToken(TokenId newid, char* newbuff) {
    NextToken.Id = newid;

    //use hardcoded tokenvalue
    if (newbuff != NULL)
        strcpy_s(NextToken.Buff, TOKEN_TYPE_SIZE, newbuff);

    //if NULL is provided, do nothing
}

//Adds a character to the NextToken buffer
void addToTokenBuff(char newchar) {
    if (NextToken.BuffIndex < TOKEN_BUFF_SIZE) {
        NextToken.Buff[NextToken.BuffIndex] = newchar;
        NextToken.BuffIndex++;
    }
}

//Adds a character to the LexErrIndex
void addLexErr(char currchar) {
    LexErrBuff[LexErrIndex] = currchar;
    LexErrIndex++;
    LexErrTotal++;
}

void clearBuffer(char* buffer, int bufferSize)
{
    int i = 0;
    for (i; i < bufferSize; i++)
    {
        buffer[i] = '\0';
    }
}

void printLexErrors() {
    clearBuffer(LisFileBuffer, OUTFILE_BUFF_SIZE);
    sprintf(LisFileBuffer, "Number of total errors: %d\n", LexErrTotal);
    fputs(LisFileBuffer, LisFile);
}

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
    if (filechar == '\n' || (filechar == EOF && LinePos > 0)) {
        int i = 0;

        //Format the collected line. No need to clear buffer.
        sprintf(LisFileBuffer, "%2d   %s\n", LineCount, LineBuff);
        fputs(LisFileBuffer, LisFile);

        //Print each lex error
        for (i = 0; i < LexErrIndex; i++)
        {
            char currErr = LexErrBuff[i];
            sprintf(ErrorBuffer, "Lexical Error on line %2d-   %c not recognized\n", LineCount, currErr);
            fputs(ErrorBuffer, LisFile);
        }
        LexErrIndex = 0; //Reset errors for next line

        //Clear error and line buffers
        memset(LexErrBuff, '\0', LINE_BUFF_SIZE);
        memset(LineBuff, '\0', LINE_BUFF_SIZE);

        //Reset line index
        LinePos = 0;
        LineCount++;
    }
    else {
        //Append character to lineBuffer.
        LineBuff[LinePos] = filechar;
        LinePos++;
    }

    return toupper(filechar);
}




//Consumes a digit. Also consumes the character given to the function (guaranteed to be a "-" or a digit)
void gatherDigit(char currchar) {
    logical stillDigit = ltrue;

    do {
        addToTokenBuff(currchar);
        currchar = peekChar();

        if (isdigit(currchar) && !isDelimiter(currchar) && NextToken.BuffIndex < TOKEN_BUFF_SIZE) {
            consumeChar();
        }
        else
            stillDigit = lfalse;

    } while (stillDigit);
}



TokenId getReservedId() {
    TokenId foundtoken = ID; //assume it's an ID, because 
    int i = 0;
    //only go up to 13 because that's the last enterable reserved word
    for (i = 0; i < 13; i++) {
        if (strcmp(NextToken.Buff, TOKEN_NAMES[i]) == 0) {
            foundtoken = i;
        }
    }

    return foundtoken;
}

void getNextToken() {
    memcpy(&CurrToken, &NextToken, sizeof(tokendata)); //Move NextToken into CurrToken


    //Initialize NextToken
    memset(&NextToken, 0, sizeof(tokendata));    //Salt the NextToken fields (clear all)
    NextToken.Id = -1;                           //Claim no token found

    char currchar;

    do {
        currchar = consumeChar();

        if (isspace(currchar)) {
            //do nothing
        }
        else if (isalpha(currchar)) { //Compose ID/Reserved Word
            logical stillId = ltrue;

            do {
                addToTokenBuff(currchar);
                currchar = peekChar();
                if (isalnum(currchar) && !isDelimiter(currchar) && NextToken.BuffIndex < TOKEN_BUFF_SIZE)
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
                } while (currchar != '\n' && currchar != EOF && NextToken.BuffIndex < TOKEN_BUFF_SIZE);

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
                addToTokenBuff(currchar);
                setToken(LEXERR, NULL);
                addLexErr(currchar);
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
            addToTokenBuff(currchar);
            setToken(LEXERR, NULL);
            addLexErr(currchar);

        }
    } while (NextToken.Id == -1 && currchar != EOF);

    NextToken.Name = TOKEN_NAMES[NextToken.Id];

    numOfLineTokens++;

    
    //printf("%s", printBuffer);
}

void parserError(const char *expected) {
    printf("Line %d: Expected %s, got '%s'\n", LineCount, expected, CurrToken.Buff);
}

logical match(TokenId desiredid) {
    logical success = lfalse;
    clearBuffer(LisFileBuffer, OUTFILE_BUFF_SIZE);
    sprintf(LisFileBuffer, "Expected Token: %-12s Actual Token: %-15s\n", TOKEN_NAMES[desiredid] , CurrToken.Buff);
    fputs(LisFileBuffer, OutFile);
    if (CurrToken.Id == desiredid) {
        success = ltrue;
    } else if(!singleDigitLog){     //Checks singleDigitLog, so that we don't print out errors that we should not
        parserError(TOKEN_NAMES[desiredid]);
    }
    getNextToken();
    return success;
}



TokenId peekNextToken() {
    return NextToken.Id;
}




//*** Primary Scanner Function ***//

void initScanner()
{
    getNextToken(); //put first token into NextToken
    getNextToken(); //put first token into CurrToken, and second token into NextToken
}

