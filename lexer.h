#ifndef lexer
#define lexer

#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include "lexerDef.h"


extern void print_program_withoutcomment();
extern void removeComments(char * filename1);
extern nodeHM *newNode(char *value, char *token, int hash);
extern int hashFunction(char *value);
extern void insertHM(char *value, char *token);
extern int setUpStream(char *filename);
extern void initLexor();
extern Token *makeTokenMan(char *value, char *token,struct Symbol_Table* tokenVal);
extern char readBuffer();
extern void modifyBufferForError();
extern void modifybufferPointer();
extern Token *getNextToken();
extern char *search(char *value,char *temp);
extern void modifyLexeme();
extern Token *retraceToPrevState(int state);
extern void printhmot();
extern struct Symbol_Table* getSymbolTableNode(char* tokenName,char* tokenVal);
extern void insertSymbolTable(char* tokenName,char* tokenVal);
extern Token* searchSymbolTable(char *str,char *str1);
extern void read_symbolTable();
extern void getTokens();

#endif
