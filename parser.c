/*Group 2 Program 3
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu
Rudolf Hanzes - han7739@calu.edu

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
			//expression();

			break;
		case READ:
			match(READ);
			match(LPAREN);
			//idlist();

			break;
		case WRITE:
			//Blah

			break;
		case IF:

			break;
		default:

			break;
	}
}
