#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void ASTnodes(node* head)
{
	if(head==NULL)
		return;

	++nodes_ast;
	ASTnodes(head->child);
	//printing the node just like printing the ast
	//lexemeCurrentNode     lineno   token   valueIfNumber   parentNodeSymbol   isLeafNode(yes/no)    NodeSymbol

	//printing the siblings
	if(head->child!=NULL)
	{
		node* siblings = (head->child)->Next;
		while(siblings!=NULL)
		{
			ASTnodes(siblings);
			siblings = siblings->Next;
		}
	}
}

int importantNode(char str[50])
{
    if(strcmp(str,"BO")==0)
		return 0;
	if(strcmp(str,"BC")==0)
		return 0;
	if(strcmp(str,"START")==0)
		return 0;
	if(strcmp(str,"END")==0)
		return 0;
    if(strcmp(str,"DRIVERDEF")==0)
		return 0;
	if(strcmp(str,"DRIVER")==0)
        return 0;
	if(strcmp(str,"IN")==0)
		return 0;
	if(strcmp(str,"COLON")==0)
		return 0;
	if(strcmp(str,"SEMICOL")==0)
		return 0;
	if(strcmp(str,"BREAK")==0)
		return 0;
	if(strcmp(str,"RANGEOP")==0)
		return 0;
	if(strcmp(str,"COMMA")==0)
		return 0;
	if(strcmp(str,"USE")==0)
		return 0;
    if(strcmp(str,"DEF")==0)
		return 0;
	if(strcmp(str,"ENDDEF")==0)
		return 0;
	if(strcmp(str,"DRIVERENDDEF")==0)
		return 0;
	if(strcmp(str,"MODULE")==0)
		return 0;
	if(strcmp(str,"WITH")==0)
		return 0;
	if(strcmp(str,"TAKES")==0)
		return 0;
	if(strcmp(str,"INPUT")==0)
		return 0;
    if(strcmp(str,"PARAMETERS")==0)
		return 0;
	if(strcmp(str,"PROGRAM")==0)
		return 0;
    if(strcmp(str,"OF")==0)
		return 0;
	return 1;
}

void ParseTreenodes(node* head)
{
	if(head==NULL)
		return;
	++nodes_par;// parse

	ParseTreenodes(head->child);
	//printing the node
	//lexemeCurrentNode     lineno   token   valueIfNumber   parentNodeSymbol   isLeafNode(yes/no)    NodeSymbol

	//printing the siblings
	if(head->child!=NULL)
	{
		node* siblings = (head->child)->Next;
		while(siblings!=NULL)
		{
			ParseTreenodes(siblings);
			siblings = siblings->Next;
		}
	}
}
void printAST(node* head)
{
	if(head==NULL)
		return;

	printAST(head->child);
	//PRINTING THE NODE
	//lexemeCurrentNode     lineno   token   valueIfNumber   parentNodeSymbol   isLeafNode(yes/no)    NodeSymbol
		if(head->check==0)
		{
			if(head->hash_val==epsilon)
			{
				//LEXEME
				printf("----\t");

				//LINENO
				printf("----\t");

				//TOKEN
				printf("----\t");

				//valueIfNumber
				printf("----\t");

				//parentNodeSymbol
				if(head->par==NULL)
					printf("ROOT\t");
				else
					printf("%s\t",hashed_string[1][(head->par)->hash_val]+1);

				//isLeafNode
				printf("yes\t");

				//NodeSymbol
				printf("%s\t",hashed_string[0][head->hash_val]);
			}
			else
			{
				//LEXEME
				if((head->ptr)->tokenVal==NULL)
					print_lexeme((head->ptr)->tokenName);
				else
					printf("%s\t",((head->ptr)->tokenVal)->tokenVal);

				//LINENO
				printf("%d\t",(head->ptr)->rowNum);

				//TOKEN
				printf("%s\t",(head->ptr)->tokenName);

				//valueIfNumber
				if(strcmp((head->ptr)->tokenName,"NUM")==0 || strcmp((head->ptr)->tokenName,"RNUM")==0)
					printf("%s\t",((head->ptr)->tokenVal)->tokenVal);
				else
					printf("----\t");

				//parentNodeSymbol
				if(head->par==NULL)
					printf("ROOT\t");
				else
					printf("%s\t",hashed_string[1][(head->par)->hash_val]+1);

				//isLeafNode
				printf("yes\t");

				//NodeSymbol
				printf("----\t");
			}
		}
		else
		{
			//LEXEME
			printf("----\t");

			//LINENO
			printf("----\t");

			//TOKEN
			printf("----\t");

			//valueIfNumber
			printf("----\t");

			//parentNodeSymbol
			if(head->par==NULL)
				printf("ROOT\t");
			else
				printf("%s\t",hashed_string[1][(head->par)->hash_val]+1);

			//isLeafNode
			printf("no\t");

			//NodeSymbol
			printf("%s\t",hashed_string[1][head->hash_val]+1);
		}
		printf("\n");

	//printing the siblings
	if(head->child!=NULL)
	{
		node* siblings = (head->child)->Next;
		while(siblings!=NULL)
		{
			printAST(siblings);
			siblings = siblings->Next;
		}
	}
}

void deleteNode(node* head)
{
	if(head==NULL)
		return;

	if(head==head->par->child)
	{
		head->par->child = head->Next;
		free(head);
		return;
	}

	node* temp = head->par->child;
	while(temp->Next!=head)
		temp = temp->Next;
	temp->Next = head->Next;
	free(head);
	return;
}
void node_trav(node* head,int ch)
{
if(ch)
{
head->par->child = head->child;

}
else
{
node* temp = head->par->child;
			while(temp->Next!=head)
				temp = temp->Next;
			temp->Next = head->child;
			//head->child->par = head->par;
			//head->child->Next = head->Next;
			///free(head);
}
head->child->Next = head->Next;
			head->child->par = head->par;
			free(head);
}
node* generateAST(node* head)
{
	if(head==NULL)
		return head;

	node* node_;

    //check is  0 => terminal
    //ans it is not an epsilon
    //and it is not an important node...delete it

    if(!head->check && operationNode(head->ptr->tokenName)) //else if it is a terminal and an operation
	{
		node_ = head->Next;
		node* temp = head->par;
		temp->ptr = head->ptr;
		temp->check = 0;
		temp->sym_ptr = head->sym_ptr;
		temp->hash_val = head->hash_val;
		deleteNode(head);
		return node_;
	}
	else if(head->child!=NULL && (head->child)->Next==NULL) //non terminals with only one child
	{
        node* node_ = head->child;
		if(head==head->par->child)
		{
		    //if it is <unary_op>...then it needs to a different treatment in typechecking
		    if(strcmp(hashed_string[head->check][head->hash_val], "<unary_op") == 0)
            {
                //printf("hello bro\n");
                if(head->child)
                {
                    //printf("%s\n", head->child->ptr->tokenName);
                    head->child->sym_ptr = &emptynode;
                    head->child->sym_ptr->isunop = 1;
                    //that op if a unary op
                }
                //printf("bye\n");
            }

node_trav(head,1);
		}
		else    //<whichStmt> for assignment always goes into this else condition
		{
		    node_trav(head,0);
		}
		return generateAST(node_);
	}
	 else if(!head->check  && head->hash_val!=epsilon && !importantNode(head->ptr->tokenName))
	{
	    //if(strcmp(hashed_string[head->][head->hash_val]))
		node_ = head->Next;
		deleteNode(head);
		return node_;
	}

	node* temp =  head->child;
	while(temp!=NULL)
		temp = generateAST(temp);
	return head->Next;
}

/***

void collect_node(node* head)
{
    int i;
for( i=0;i<MAXSZ;i++)
{
if(delstack[i]==NULL)
    break;
}
delstack[i]=head;
}
void deleteNode(node* head)
{
	if(head==NULL)
		return;
	if(head==head->par->child)
	{
		head->par->child = head->Next;
		free(head);
		return;
	}
	node* temp = head->par->child;
}



void deleteNodestack()
{
  int i=0;

    while(delstack[i]!=NULL)
    {
    deleteNode(delstack[i]);;
//        printf("ho--%d",delstack[i]->hash_val);
    i++;
    }
}
node* gen_ast(node* head)
{
}

node* generateAST(node* head)
{
    gen_ast(head);
    deleteNodestack();

}

**/
