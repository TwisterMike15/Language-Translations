





#include "generator.h"




IdString SymbolTable[127];
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




















