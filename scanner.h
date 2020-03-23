/*Group 2 Program 3
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu
Rudolph Hanzes - han7739@calu.edu

CSC 460
Language Translations
*/

#ifndef SCANNER_H
#define SCANNER_H

#include <string.h>

//*****************************Constants*****************************//
#define LINE_BUFF_SIZE 1026
#define OUTFILE_BUFF_SIZE 2000
#define TOKEN_BUFF_SIZE 100
#define TOKEN_TYPE_SIZE 20

static const char* TOKEN_NAMES[34] = {
	"BEGIN",         //0
	"END",           //1
	"READ",          //2
	"WRITE",         //3
	"IF",            //4
	"THEN",          //5
	"ELSE",          //6
	"ENDIF",         //7
	"WHILE",         //8
	"ENDWHILE",      //9
	"FALSE",         //10
	"TRUE",          //11
	"NULL",          //12
	"LPAREN",        //13
	"RPAREN",        //14
	"SEMICOLON",     //15
	"COMMA",         //16
	"ASSIGNOP",      //17
	"PLUSOP",        //18
	"MINUSOP",       //19
	"MULTOP",        //20
	"DIVOP",         //21
	"NOTOP",         //22
	"LESSOP",        //23
	"LESSEQUALOP",   //24
	"GREATEROP",     //25
	"GREATEREQUALOP",//26
	"EQUALOP",       //27
	"NOTEQUALOP",    //28
	"EOF",           //29
	"ID",            //30
	"INTLITERAL",    //31
	"LEXERR",        //32
	"COMMENT"		 //33
};

typedef enum {
	BEGIN,           //0
	END,             //1
	READ,            //2
	WRITE,           //3
	IF,              //4
	THEN,            //5
	ELSE,            //6
	ENDIF,           //7
	WHILE,           //8
	ENDWHILE,        //9
	FALSEOP,         //10
	TRUEOP,          //11
	NULLOP,          //12
	LPAREN,          //13
	RPAREN,          //14
	SEMICOLON,       //15
	COMMA,           //16
	ASSIGNOP,        //17
	PLUSOP,          //18
	MINUSOP,         //19
	MULTOP,          //20
	DIVOP,           //21
	NOTOP,           //22
	LESSOP,          //23
	LESSEQUALOP,     //24
	GREATEROP,       //25
	GREATEREQUALOP,  //26
	EQUALOP,         //27
	NOTEQUALOP,      //28
	SCANEOF,         //29
	ID,              //30
	INTLITERAL,      //31
	LEXERR,          //32
	COMMENT          //33
} TokenId;

typedef struct {
	TokenId Id;
	char Buff[TOKEN_BUFF_SIZE];  //What was in the input file (">=")
	char* Name;                  //What name ("GREATEREQUALOP")
	int BuffIndex;
} tokendata;




//*****************************Attributes****************************//
tokendata CurrToken;
tokendata NextToken;



//*****************************Functions*******************************//
logical match(TokenId);
TokenId peekNextToken();
void initScanner();
void printLexErrors();
void printNumOfErrors();
void clearBuffer(char*, int);

#endif
