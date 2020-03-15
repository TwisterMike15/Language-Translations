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
void statementList();
void statement();
void ifTail();
void idList();
void exprList();
void expression();
void term();
void factor();
void addOp();
void multOp();
void condition();
void addition();
void multiplication();
void unary();
void lprimary();
void relOp();



//Checks for **optional** productions
logical isStatementList() {
	logical success = lfalse;
	switch (CurrToken.Id) {
		case ID:
		case READ:
		case WRITE:
		case IF:
			success = ltrue;
			break;
	}
	return success;
}

logical isAddOp() {
	return (CurrToken.Id == PLUSOP || CurrToken.Id == MINUSOP);
}

logical isMultOp() {
	return (CurrToken.Id == MULTOP || CurrToken.Id == DIVOP);
}

logical isRelOp() {
	logical success = lfalse;
	switch (CurrToken.Id) {
		case LESSOP:
		case LESSEQUALOP:
		case GREATEROP:
		case GREATEREQUALOP:
		case EQUALOP:
		case NOTEQUALOP:
			success = ltrue;
			break;
	}
	return success;
}




//Productions
void systemgoal() {
	program();
	match(SCANEOF);
}

void program() {
	match(BEGIN);
	statementList();
	match(END);
}

void statementList() {
	do {
		statement();
	} while ( isStatementList() );
}

void statement() {
	switch (CurrToken.Id) {
	case ID:
		match(ID);
		match(ASSIGNOP);
		expression();
		match(SEMICOLON);

		break;
	case READ:
		match(READ);
		match(LPAREN);
		idList();
		match(RPAREN);
		match(SEMICOLON);

		break;
	case WRITE:
		match(WRITE);
		match(LPAREN);
		exprList();
		match(RPAREN);
		match(SEMICOLON);

		break;
	case IF:
		match(IF);
		match(LPAREN);
		condition();
		match(THEN);
		statementList();
		ifTail();

		break;
	case WHILE:
		match(WHILE);
		match(LPAREN);
		condition();
		match(RPAREN);
			
		if (isStatementList()) {
			statementList();
		} else {
			match(ENDWHILE);
		}
			
		break;
	default:

		break;
	}
}

void ifTail() {
	switch (CurrToken.Id) {
	case ELSE:
		match(ELSE);
		statementList();
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
	switch (CurrToken.Id) {
		case COMMA:
			match(COMMA);
			match(ID);
		
			break;
		default:
			
			break;
	}
}

void exprList() {
	expression();
	
	while ( CurrToken.Id == COMMA ) {
		match(COMMA);
		exprList();
	}
}

void expression() {
	term();
	
	if ( isAddOp() ) {
		addOp();
		term();
	}
}

void term() {
	factor();
	if ( isMultOp() ) {
		multOp();
		factor();
	}	
}

void factor() {
	switch (CurrToken.Id) {
	case LPAREN:		//'('
		match(LPAREN);
		expression();
		match(RPAREN);

		break;
	case MINUSOP:
		match(MINUSOP);
		factor();

		break;
	case ID:
		match(ID);
			
		break;
	case INTLITERAL:
		match(INTLITERAL);
					
		break;
	default:

		break;
	}
}

void addOp() {
	switch(CurrToken.Id) {
	case PLUSOP:		
		match(PLUSOP);
			
		break;
	case MINUSOP:
		match(MINUSOP);
	
		break;
	default:
		
		break;
	}
}

void multOp() {
	switch(CurrToken.Id) {
	case MULTOP:
		match(MULTOP);
			
		break;
	case DIVOP:
		match(DIVOP);
			
		break;
	default:
		
		break;
	}
}
void condition() {
	addition();
	if ( isRelOp() ) {
		relOp();
		addition();
	}
}

void addition() {
	multiplication();

	if ( isAddOp() ) {
		addOp();
		multiplication();
	}
}

void multiplication() {
	unary();
	
	if ( isMultOp() ) {
		multOp();
		unary();
	}
}

void unary() {
	switch(CurrToken.Id) {
	case NOTOP:
		match(NOTOP);
		unary();
			
		break;
	case MINUSOP:
		match(MINUSOP);
		unary();
			
		break;
	default:
		lprimary();
			
		break;
	}
}

void lprimary() {
	switch(CurrToken.Id) {
	case INTLITERAL:
		match(INTLITERAL);
			
		break;
	case ID:
		match(ID);
			
		break;
	case LPAREN:
		match(LPAREN);
		condition();
		match(RPAREN);
			
		break;
	case FALSEOP:
		match(FALSEOP);
			
		break;
	case TRUEOP:
		match(TRUEOP);
			
		break;
	case NULLOP:
		match(NULLOP);
		
		break;
	default:
			
		break;
	}
}

void relOp() {
	switch(CurrToken.Id) {
	case LESSEQUALOP:
		match(LESSOP);
		match(LESSEQUALOP);
			
		break;
	case NOTEQUALOP:
		match(LESSOP);
		match(GREATEROP);
			
		break;
	case LESSOP:
		match(LESSOP);
			
		break;
	case GREATEREQUALOP:
		match(GREATEROP);
		match(GREATEREQUALOP);
			
		break;
	case GREATEROP:
		match(GREATEROP);
			
		break;
	case EQUALOP:
		match(EQUALOP);
			
		break;
	default:
		
		break;
	}			
}

