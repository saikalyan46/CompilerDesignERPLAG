#ifndef lexerDef
#define lexerDef

#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#define NUM_TOKENS 200
#define BUFF_SIZE 500

static int lnm,state,lexemeptr;
static int bufferPointer,flag,bufferUsed;
FILE *fp;

typedef struct nodeHM
{
    int hash;
	char value[50];
	char tokenName[50];
    struct nodeHM* Next;

}nodeHM;


struct Symbol_Table{
    char tokenName[50]; //INTEGER
    char tokenVal[100]; //integer
    struct Symbol_Table *Next;
};

typedef struct Token
{
    int rowNum;
    char value[50];
    char tokenName[50];
    struct Symbol_Table *tokenVal;
    struct Token* Next;

}Token;


nodeHM *hmot[NUM_TOKENS];

char buf1[BUFF_SIZE];
char buf2[BUFF_SIZE];
char lexeme[200];
extern int cntToken;

extern struct Symbol_Table *symbolTable_head;
extern struct Symbol_Table *symbolTable_tail;

//our symbol table is a LINKED LIST of struct Symbol_Table type

extern struct Token *tokenList_head;
extern struct Token *tokenList_tail;
extern int error;

#endif
