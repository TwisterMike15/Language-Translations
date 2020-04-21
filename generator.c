





#include "generator.h"

//Design discussed with Anthony Carrola



IdString SymbolTable[100];
int SymbolCount = 0;

logical isInSymbolTable(IdString Id) {
	logical success = lfalse;
	int i = 0;

	//Loop through all symbols; if one matches Id parameter, end loop & return
	while (i < SymbolCount && !success) {
		if ( strcmp(SymbolTable[i],Id) == 0 ) {
			success = ltrue;
		} else {
			i++;
		}
	}

	return success;
}

logical registerSymbol(IdString Id) {
	logical canregister = isInSymbolTable(Id);
	if (canregister) {
		//Copy new id into the latest symbol slot
		strcpy(SymbolTable[SymbolCount], Id);
		SymbolCount++;
	}

	return canregister;
}

char* registerTemp() {
	static int tempcount = 1;
	static IdString tempid;

	sprintf(tempid, "Temp%d", tempcount);

	tempcount++;
	return tempid;
}


void generate(const char* s1, const char* s2, const char* s3, const char* s4, const char* s5, const char* s6, const char* s7, const char* s8) {
	fprintf(TmpFile, "%s%s%s%s%s%s%s%s", s1, s2, s3, s4, s5, s6, s7, s8);
}


char* prefixString(char* buff, char prefix) {
	int length = strlen(buff), i = 0;
	IdString adjusted = { 0 };

	strn[0] = prefix;
	for (i = 0; i < length; i++)
	{
		strn[i + 1] = buff[i];
	}
	strcpy(buff, strn);

	return buff;
}

ExprRec generateCondition(ExprRec LeftExpr, OpRec Operator, ExprRec RightExpr) {
	ExprRec tempexpr;
	tempexpr.type = TEMPEXPR;
	strcpy(tempexpr.data, registerTemp());

	generate(tempexpr.data," = ",LeftExpr.data," ",Operator.op," ",RightExpr.data,";\n");

	return tempexpr;
}

void generateIfStatement(ExprRec Condition)
{
	generate("if( ", Condition.data, " )\n{\n", "", "", "", "", "");
}

void generateWhileStatement(ExprRec Condition)
{
	generate("while( ", Condition.data, " )\n{\n", "", "", "", "", "");
}

void generateAssignment(ExprRec Target, ExprRec Source) {
	generate(Target.data, " = ", Source.data, ";\n", "", "", "", "");
}

void generateWriteStatement(ExprRec WriteExpression) {

}















