#ifndef symbolTable
#define symbolTable
#include <stdio.h>
#include <stdlib.h>
#include "symbolTableDef.h"

extern int max(int a,int b);
extern int getWidthOfType(int x);
extern int getType(char* str);
extern char* getNameOfType(int x);
extern void helper1(node *head, node *_node, int hash_call);
extern void helper2(node *head, node *_node, int hash_call);
extern void printArrays();

extern void stmtManip1(node *head);
extern void stmtManip2(node *head);
extern void stmtManip3(node *head);
extern void stmtManip4(node *head);
extern void stmtManip5(node *head);
extern void stmtManip6(node *head);
extern void stmtManip7(node *head);
extern void stmtManip8(node *head);
extern void stmtManip9(node *head);
extern void stmtManip10(node *head);
extern void stmtManip11(node *head);

extern int putHashId(node* head,int typ);
extern void checkErrorFor();
extern int makeHashInputList(node* head,int hash_val);
extern int makeHashOutputList(node* head,int hash_val);

extern void matcherMan(node *head, node *to_call, node *l, int h);
extern fields *getScopeFor(int x);
extern fields *getScopeWhile(int x);

extern int put_hash_sym( char* in);

extern fields *getScopeSwitch(int x);
extern int makeHashArray(node* head,int typ,int lo, int hi, int isDynamic, manage *manNode);// returns -1 if error (redeclartion) else 0;
extern void fixList(fields *position, int hash_val, int x, int o);
extern int matchInputParameters(int x, node* head,int c_scope);
extern void fixAtEndOfList(fields *position, int hash_val, int x, int o);
extern void remainingErrors();
extern int matchReturnedParameters(int x, node* head,int c_scope); // note that relist cannot have arrays in decleration
extern manage *arrayDimensionsCheck(node *var, int hash_val, node *rng_ptr, int *f1, int *f2);
extern void checkVarsAssigned(int idx, node* head);
extern void printErrors();
extern void postOrder(node* head);
extern void makeSymbolTable(node* pt_head);
extern void printSymbolTable(node* head);


#endif
