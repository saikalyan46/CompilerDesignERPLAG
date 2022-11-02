#ifndef typeCheckerDef
#define typeCheckerDef
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "symbolTableDef.h"

typedef struct arraysInfoDynamic
{
    int isdynamic;
    int sc;
	int type;
	char arrayId[100];
	char lowerBoundLexeme[100];
	int subtype;
	char higherBoundLexeme[100];
}arraysInfoDynamic;


extern int tchinter;

node* MainStack[1000];
extern int semantic_error;
extern int condition5;


char storeStack[100];



#endif
