#ifndef symbolTableDef
#define symbolTableDef
#include <stdio.h>
#include <stdlib.h>
#include "typeChecker.h"

#define RELATIONALOP  3
#define BOOLEAN  3
#define LOGICALOP  2
#define ERROR  5
#define REAL  2
#define MOD  1009
#define ARITHMETICOP  1
#define EXPR  325
#define INTEGER  1
#define epsilon  101
#define ASSIGNOP  4
#define ARRAY  4

//funcs[0] = declaration
//funcs[1] = definition
int funcs[2][1125];
int redeclaration;
int serialNumber;


typedef struct manage
{
    fields *lowerBound;
    fields *higherBound;
    int isDynamic;

    char lowerBoundLexeme[100];
    char higheBoundLexeme[100];

}manage;

//similar to fields
typedef struct Sequence
{
	int sc;
	int type;
	char seqChar[50];
	char seqchar2[50];
	int subtype;
	int lb;
	int ub;
	int hash_val;
	int mark;
	int marker2;
	struct Sequence* Next;
}Sequence;



Sequence *outputList[1025];

typedef struct util
{

	node* in;
	node* out;
	int row_num;
	int checkForCondition;
	int valueScopeNested;
	int scope;
	int x;// hash value with whom to do paramaeter testing
}util;

typedef struct util2
{

	node* index1;
	int row_num;
	int scopesNestedInside;
	int lower_bound;
	int upper_bound;
	int checkForDynamicity;
}util2;

char errorList[1050][150];
int errorCount;

util things[120];
int utilptr;
fields emptynode;
int globalScopeControl;// init to 0
int currentScope;


char hash[1050][52];
int used[1050];

int parent[1110];// parent scope

int moduleName;
char findScopeName[50];

int scopedOrNot;

int lines[2][1110]; //0->starts, 1->ends
int realVariables[1125];

//2D array of fields
//scope is basically an array of ALL_POSSIBLE_SCOPE_VALUES(INTS) X ALL_POSSIBLE_HASH_VALUES (for different vars in that scope)
fields scope[110][1150];// 110 scopes hashed over 1050 hash space
int globalTesting;

Sequence *inputList[1025];

//locates the scope of individual IDs based on their hash
int scopeMap[1110];
char ret[50];

//maintains the current offset of a module - index of this array is the unique hash val of that module name
int offsetCurr[1110];

//maintains start rowNum (line num) of scopes
int fooOffset;


#endif
