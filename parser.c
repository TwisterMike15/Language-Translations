/*Group 2 Program 3
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu
Rudolph Hanzes - han7739@calu.edu

CSC 460
Language Translations
*/

#include "parser.h"
#include "file_util.h"
#include "scanner.h"

//Production Prototypes
void systemgoal();
void program();
void statementlist();
void statement();
void IFTail();
void idList();
void exprList();
void expression();
void term();
void factor();
void addop();		//do we need this here?
void multop();		//do we need this here?
void condition();
void addition();
void multiplication();
void unary();
void lprimary();
void RelOp();


//Productions
void systemgoal() {
	program();
	match(SCANEOF);
}

void program() {
	match(BEGIN);
	statementlist();
	match(END);
}

void statementlist() {
	statement();
}

void statement() {
	switch (CurrToken.Id) {
	case ID:
		match(ID);
		match(ASSIGNOP);
		expression();

		break;
	case READ:
		match(READ);
		match(LPAREN);
		idList();
		match(RPAREN);

		break;
	case WRITE:
		match(WRITE);
		match(LPAREN);
		exprList();
		match(RPAREN);

		break;
	case IF:
		match(IF);
		match(LPAREN);
		condition();
		match(THEN);
		statementlist();
		IFTail();

		break;
	case WHILE:
		match(WHILE);
		match(LPAREN);
		condition();
		match(RPAREN);
		statementlist();
		match(ENDWHILE);

		break;
	default:

		break;
	}
}

void IFTail() {
	switch (CurrToken.Id) {
	case ELSE:
		match(ELSE);
		statementlist();
		match(ENDIF);

		break;
	case ENDIF:
		match(ENDIF);

		break;
	default:

		break;
	}
}

void idList() {
		match(ID);
		match(COMMA);
		match(ID);
}

void exprList() {
	expression();
	match(COMMA);
	//expression();
}

void expression() {
	term();
	addop();
	//term();
}

void term() {
	factor();
	multop();
	//factor();
}

//	THIS NEEDS FURTHER WORK
void factor() {
	switch (CurrToken.Id) {
	case LPAREN:		//'('
		match(LPAREN);
		expression();
		match(RPAREN);

		break;
	case '-':

		break;
	default:

		break;
	}
}

//	PRODUCE THE ADDOP AND MULTOP HERE

void condition() {
	addition();
	RelOp();
	//addition();
}

void addition() {
	multiplication();
	addop();
	//multiplication();
}

void multiplication() {
	unary();
	multop();
	//unary();
}

// void unary()

// void lprimary()

// void RelOp()
