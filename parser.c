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
#include "generator.h"

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
ExprRec condition();
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
	case WHILE:
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
	startMain();
	statementList();
	match(END);
	endMain();
}

void statementList() {
	do {
		statement();
	} while (isStatementList());
}

void statement() {
	ExprRec target, source;

	switch (CurrToken.Id) {
	case ID:
		target = processId(CurrToken.Buff);
		match(ID);
		match(ASSIGNOP);
		expression(&source);
		generateAssignment(target, source);
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
		generateIfStatement(condition());
		match(RPAREN);
		match(THEN);
		statementList();
		generate("}\n");
		ifTail();

		break;
	case WHILE:
		match(WHILE);
		match(LPAREN);
		generateWhileStatement(condition());
		match(RPAREN);

		statementList();

		match(ENDWHILE);
		generate("}\n");
		break;
	default:

		break;
	}
}

void ifTail() {
	switch (CurrToken.Id) {
	case ELSE:
		match(ELSE);
		generate("else\n{\n");
		statementList();
		match(ENDIF);
		generate("}\n");
		break;
	case ENDIF:
		match(ENDIF);

		break;
	default:
		parserError("iftail (ELSE or ENDIF)");
		getNextToken();
		break;
	}
}

void idList() {
	generateRead(CurrToken.Buff);
	match(ID);
		
	while (CurrToken.Id == COMMA) {
		match(COMMA);
		generateRead(CurrToken.Buff);
		match(ID);
		
	}
}

void exprList() {
	ExprRec exprlist_root;

	expression(&exprlist_root);
	generateWriteStatement(exprlist_root);

	while (CurrToken.Id == COMMA) {
		match(COMMA);
		exprList();
	}
}

void expression(ExprRec* ResultExpression) {
	ExprRec operand_left, operand_right;
	OpRec operator;

	term(&operand_left);

	while (isAddOp()) {
		addOp(&operator);
		term(&operand_right);
		operand_left = generateInfix(operand_left, operator,operand_right);
	}
	*ResultExpression = operand_left;
}

void term(ExprRec* ResultExpression) {
	ExprRec operand_left, operand_right;
	OpRec operator;

	factor(&operand_left);
	while (isMultOp()) {
		multOp(&operator);
		factor(&operand_right);
		operand_left = generateInfix(operand_left, operator,operand_right);
	}
	*ResultExpression = operand_left;
}

void factor(ExprRec* ResultExpression) {
	ExprRec tempresult;

	switch (CurrToken.Id) {
	case LPAREN:		//'('
		match(LPAREN);
		expression(&tempresult);
		*ResultExpression = tempresult;

		match(RPAREN);

		break;
	case MINUSOP:
		match(MINUSOP);
		factor(&tempresult);
		prefixString(tempresult, '-');
		*ResultExpression = tempresult;

		break;
	case ID:
		*ResultExpression = processId(CurrToken.Buff);
		match(ID);

		break;
	case INTLITERAL:
		*ResultExpression = processLiteral(CurrToken.Buff);
		match(INTLITERAL);

		break;
	default:
		parserError("factor (ID, INTLITERAL, LPAREN, or MINUSOP)");
		getNextToken();
		break;
	}
}

void addOp(OpRec* ResultOp) {
	switch (CurrToken.Id) {
	case PLUSOP:
		match(PLUSOP);
		*ResultOp = processOp(PLUSOP);

		break;
	case MINUSOP:
		match(MINUSOP);
		*ResultOp = processOp(MINUSOP);

		break;
	default:

		break;
	}
}

void multOp(OpRec* ResultOperator) {
	switch (CurrToken.Id) {
	case MULTOP:
		match(MULTOP);
		*ResultOperator = processOp(MULTOP);

		break;
	case DIVOP:
		match(DIVOP);
		*ResultOperator = processOp(DIVOP);

		break;
	default:

		break;
	}
}

ExprRec condition() {
	ExprRec condition_left, condition_right;
	OpRec condition_op;

	//Get left-side of condition
	addition(&condition_left);

	while (isRelOp()) {
		//Get operator
		relOp(&condition_op);

		//Get right-side of condition
		addition(&condition_right);

		//make the left, op, and right parts of condition into a new ExprRec, for the next iteration
		condition_left = generateCondition(condition_left, condition_op, condition_right);
	}

	return condition_left;
}

void addition(ExprRec* Condition) {
	ExprRec add_left, add_right;
	OpRec add_op;

	multiplication(&add_left);

	while (isAddOp()) {
		addOp(&add_op);
		multiplication(&add_right);
		add_left = generateCondition(add_left, add_op, add_right);
	}

	*Condition = add_left;
}

void multiplication(ExprRec* Condition) {
	ExprRec mult_left, mult_right;
	OpRec mult_op;

	unary(&mult_left);

	while (isMultOp()) {
		multOp(&mult_op);
		unary(&mult_right);
		mult_left = generateCondition(mult_left, mult_op, mult_right);
	}

	*Condition = mult_left;
}

void unary(ExprRec* Condition) {
	ExprRec expression;


	switch (CurrToken.Id) {
	case NOTOP:
		match(NOTOP);
		unary(&expression);
		prefixString(expression.data, '!');
		*Condition = expression;

		break;
	case MINUSOP:
		match(MINUSOP);
		unary(&expression);
		prefixString(expression.data, '!');
		*Condition = expression;

		break;
	default:
		lprimary(&expression);
		*Condition = expression;

		break;
	}

}

void lprimary(ExprRec* Condition) {
	OpRec temp;

	switch (CurrToken.Id) {
	case FALSEOP:
	case TRUEOP:
	case NULLOP:
		//match any of the above ones as themself
		temp = processOp(CurrToken.Id);
		strcpy(Condition->data, temp.op);
		match(CurrToken.Id);

		break;
	case ID:
		*Condition = processId(CurrToken.Buff);
		match(ID);

		break;
	case INTLITERAL:
		*Condition = processLiteral(CurrToken.Buff);
		match(INTLITERAL); //match any of the above ones as themself

		break;
	case LPAREN:
		match(LPAREN);
		*Condition = condition();
		match(RPAREN);
		break;
	default:
		//Because lprimary is never firstly optional
		parserError("lprimary (INTLITERAL, ID, LPAREN, FALSEOF, TRUEOP, or NULLOP)");
		getNextToken();


		break;
	}
}

void relOp(OpRec* Operator) {
	switch (CurrToken.Id) {
	case LESSEQUALOP:
	case NOTEQUALOP:
	case LESSOP:
	case GREATEREQUALOP:
	case GREATEROP:
	case EQUALOP:
		*Operator = processOp(CurrToken.Id);
		match(CurrToken.Id);
		break;

	default:
		break;
	}
}
