/*Group 2 Program 3
Paul Maclean- mac7537@calu.edu
Mike Gorse- gor9632@calu.edu
Robert Breckenridge- bre6896@calu.edu
Chase Smith- smi8808@calu.edu
Rudolph Hanzes - han7739@calu.edu

CSC 460
Language Translations
*/

//Design discussed with Anthony Carrola

#include "generator.h"
#include "scanner.h"


#define OUTFILE_BUFF_SIZE 2000
char OutPutBuffer[OUTFILE_BUFF_SIZE] = { '\0' };


IdString SymbolTable[100];
int SymbolCount = 0;

void printSymbols(void);


//didn't think about this when implementing the scanner. don't like switch cases and other methods so this works
//sorry it's ugly
static const char* OPERATOR_LOOKUP[34] = {
	NULL,    //0
	NULL,    //1
	NULL,    //2
	NULL,    //3
	NULL,    //4
	NULL,    //5
	NULL,    //6
	NULL,    //7
	NULL,    //8
	NULL,    //9
	"0",     //10
	"1",     //11
	"NULL",  //12
	NULL,    //13
	NULL,    //14
	NULL,    //15
	NULL,    //16
	NULL,    //17
	"+",     //18
	"-",     //19
	"*",     //20
	"/",     //21
	NULL,    //22
	"<",     //23
	"<=",    //24
	">",     //25
	">=",    //26
	"==",    //27
	"<>",    //28
	NULL,    //29
	NULL,    //30
	NULL,    //31
	NULL,    //32
	NULL	 //33
};


OpRec processOp(TokenId OpEnum);
ExprRec processLiteral(char* TokenContent);
ExprRec processId(char* TokenContent);



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


void generate(const char* output) {
	fprintf(TmpFile, "%s", output);
}











void prefixString(char* buff, char prefix) {
	int length = strlen(buff)+1;
	int i = 0;

	//Begin from end, copy every char from previous index
	for (i = length+1; i > 0; i--) {
		if (i == length)
		{
			buff[i] = '\0';
		}
		else
		{
			buff[i] = buff[i - 1];
		}
	}
	buff[0] = prefix;
}

OpRec processOp(TokenId OpEnum)							//Processes operands
{
	OpRec oper;
	strcpy(oper.op, OPERATOR_LOOKUP[OpEnum]);
	return oper;
}

ExprRec processLiteral(char* TokenContent)			//processes literals
{
	ExprRec lit;
	lit.type = LITERALEXPR;
	strcpy(lit.data, TokenContent);
	return lit;
}

ExprRec processId(char* TokenContent)				//processes IDs that are the current token
{
	ExprRec idvar;
	registerSymbol(TokenContent);
	idvar.type = IDEXPR;							//sets the kind
	strcpy(idvar.data, TokenContent);					//sets the data
	return idvar;									//returns our expression now
}



ExprRec generateCondition(ExprRec LeftExpr, OpRec Operator, ExprRec RightExpr) {
	ExprRec tempexpr;
	tempexpr.type = TEMPEXPR;
	strcpy(tempexpr.data, registerTemp());

	generate(tempexpr.data);
	generate(" = ");
	generate(LeftExpr.data);
	generate(" ");
	generate(Operator.op);
	generate(" ");
	generate(RightExpr.data);
	generate(";\n");

	return tempexpr;
}

void generateRead(char* TokenContent) {
	ExprRec idrec = processId(TokenContent);
	clearBuffer(OutPutBuffer, OUTFILE_BUFF_SIZE);
	//**************************************************
	sprintf(OutPutBuffer, "scanf(\"%%d\", &%s);\n", TokenContent);
	fputs(OutPutBuffer, TmpFile);
}

ExprRec generateInfix(ExprRec LeftOp, OpRec OpInf, ExprRec RightOp)		//generates expressions
{
	ExprRec tempexpr;
	tempexpr.type = TEMPEXPR;
	strcpy(tempexpr.data, registerTemp());
	generate(tempexpr.data);
	generate("=");
	generate(LeftOp.data);
	generate(OpInf.op);
	generate(RightOp.data);
	generate(";\n");
	return tempexpr;
}


void generateIfStatement(ExprRec Condition) {
	generate("if( ");
	generate(Condition.data);
	generate(" )\n{\n");
}

void generateWhileStatement(ExprRec Condition) {
	generate("while( ");
	generate(Condition.data);
	generate(" )\n{\n");
}

void generateAssignment(ExprRec Target, ExprRec Source) {
	generate(Target.data);
	generate(" = ");
	generate(Source.data);
	generate(";\n");
}

void generateWriteStatement(ExprRec WriteExpression) {
	generate("printf(\"%d\", ");
	generate(WriteExpression.data);
	generate(" );\n");
}

void startMain()
{
	char printMain[1000] = { '\0' };
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(printMain, "//Date and Time of Compilation: %s\n", asctime(timeinfo));
	fputs(printMain, OutFile);
	fputs("main()\n{\n", OutFile);
}

void endMain()
{
	printf("Inside endMain\n");
	printSymbols();
	copyToFile(OutFile, TmpFile);
	fputs("}", OutFile);
}

void printSymbols()
{
	int i = 0;
	for (i = 0; i < SymbolCount; i++)
	{
		fputc(SymbolTable[i], OutFile);
	}
}












