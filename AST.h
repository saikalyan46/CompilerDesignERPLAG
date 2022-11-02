#ifndef AST
#define AST
#include <stdio.h>
#include <stdlib.h>

extern void printAST(node* head);
extern void ParseTreenodes(node* head);
extern void ASTnodes(node* head);

extern int importantNode(char str[50]);
extern void deleteNode(node* head);
extern node* generateAST(node* head);
#endif
