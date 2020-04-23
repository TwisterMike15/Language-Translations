#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H


#include "file_util.h"
#include "scanner.h"


typedef char IdString[100];
enum ExprType{ IDEXPR, LITERALEXPR, TEMPEXPR } typedef ExprType;

typedef struct OpRec {
	IdString op;
} OpRec;

typedef struct ExprRec {
	IdString data;
	ExprType type;
} ExprRec;




logical isInSymbolTable(IdString Id);
logical registerSymbol(IdString Id);


OpRec processOp(TokenId OpEnum);
ExprRec processLiteral(char* TokenContent);
ExprRec processId(char* TokenContent);

void generateIfStatement(ExprRec Condition);
ExprRec generateCondition(ExprRec LeftExpr, OpRec Operator, ExprRec RightExpr);
ExprRec generateInfix(ExprRec LeftOp, OpRec OpInf, ExprRec RightOp);

void generate(const char* output);
void startMain(void);
void endMain(void);


#endif
