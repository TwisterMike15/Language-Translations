#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H


#include "file_util.h"


typedef char IdString[100];
enum Expr{ IDEXPR, LITERALEXPR, TEMPEXPR } typedef Expr;

typedef struct Op_Rec {
	IdString op;
} Op_Rec;

typedef struct Expr_Rec {
	IdString data;
	Expr expr;
} Expr_Rec;





logical isInSymbolTable(IdString Id);
logical registerSymbol(IdString Id);







#endif
