#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H


#include "file_util.h"


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


void generateIfStatement(ExprRec Condition);




#endif
