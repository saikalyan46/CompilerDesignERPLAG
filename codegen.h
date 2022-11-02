#ifndef codegen
#define codegen
#include <stdio.h>
#include <stdlib.h>
#include "typeChecker.h"

extern void codeGeneration(node* head);
extern void resetCodeGen();
extern void inputCode(node* head);
extern void printCode(node *head);
extern void assignopCode(node* head);
extern void loopOpCode(node* head);
extern void conditionalopCode(node* head);
FILE *fp_asm;
#endif
