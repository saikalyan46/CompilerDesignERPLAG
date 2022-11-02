#ifndef parserDef
#define parserDef
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#define MAXSZ 10010
#define MAXRULES 200


int hasepsilon[1025];
int parse_table[1025][64];
int nodes_par;
int nodes_ast;

typedef struct construct
{
	int check;
	int hash_val;
	struct construct * Next;
}construct;

typedef struct rule
{
	construct *first;
	construct *last;
}rule;

//used for function scoping
typedef struct fields
{ int sc;       //gives the scope of that node
  int type;
  int subtype;
  int lb;
  int ub;
  int offset;
  int width;
  int dec;
  int inInputList;  //0 if hashed in input list
                    //1 if hashed in output list
                    //2 if hashed inside the function
  char module_name[10]; //i.e. name of the function

                //these 2 fields are used for the bounds of a dynamic array
  int arrayHash;
  int isBound;
                //for dynamic arrays
  int isDynamic;
  struct fields *lowerBound;
  struct fields *higherBound;
  int lowerOffset;
  int higherOffset;

    char lowerBoundLexeme[100];
  char higheBoundLexeme[100];

  int isunop;   //1 if it is;
  //pointing to the indexes in the case of a dynamic array
}fields;

typedef struct node
{
	struct node* par;
	struct node* child;
	struct node* Next;
	int check;
	struct Token* ptr;
	fields* sym_ptr;
	int hash_val;
}node;

node* mystack[MAXSZ];
int stktop;
extern node* parseTree_head;

rule *G[MAXRULES];

//0 is terminal
//1 is non terminal
char hashed_string[2][1025][40];

int rule_no;
int num_term;
int check_epsilon;

int final_term_hash[1025];
int vapas_hash[64];
unsigned long long int ff[2][1025];


#endif
