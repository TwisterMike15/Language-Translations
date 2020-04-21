#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H


typedef char String[100];
enum Expr{ IDEXPR, LITERALEXPR, TEMPEXPR } typedef Expr;

typedef struct Op_Rec {
	String op;
} Op_Rec;

typedef struct Expr_Rec {
	String data;
	Expr;
} Expr_Rec;




#endif
