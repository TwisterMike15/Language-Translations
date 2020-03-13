/*Group 2 Program 2
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu

CSC 460
Language Translations
*/

#ifndef SCANNER_H
#define SCANNER_H


static const char* TOKEN_TYPES[33] = {
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
	"LEXERR"         //32
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
} TokenId;


void scanner(FILE*);

#endif