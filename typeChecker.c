#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typeChecker.h"

#define MOD  1009
#define ARITHMETICOP  1
#define EXPR  325
#define INTEGER  1
#define epsilon  101
#define ASSIGNOP  4
#define ARRAY  4
#define RELATIONALOP  3
#define BOOLEAN  3
#define LOGICALOP  2
#define ERROR  5
#define REAL  2


int tchinter = -1;
int condition5 = -1;
int semantic_error = 0;

void resetTypeCheck()
{
    int i;
    tchinter = -1;
    condition5 = -1;
    semantic_error = 0;

    for(i= 0;i<1000;i++)
        MainStack[i] = NULL;

    for(i=0;i<100;i++)
        storeStack[i] = '\0';
}


//called by checkTypes function
//called by checkTypes
int operationNode(char str[50])
{
    if(strcmp(str,"PLUS")==0 || strcmp(str,"MINUS")==0 || strcmp(str,"MUL")==0 || strcmp(str,"DIV")==0)
		return ARITHMETICOP;
	else if(strcmp(str,"LT")==0 || strcmp(str,"LE")==0 || strcmp(str,"GT")==0 || strcmp(str,"GE")==0 || strcmp(str,"EQ")==0 || strcmp(str,"NE")==0)
		return RELATIONALOP;
	else if(strcmp(str,"ASSIGNOP")==0)
		return ASSIGNOP;
	else if(strcmp(str,"AND")==0 || strcmp(str,"OR")==0)
		return LOGICALOP;
	else
		return 0;
}

//checks what kind of var is looked at now
int returnVarType(char str[50])
{
	if(strcmp(str,"ID")==0)
		return 1;
	if(strcmp(str,"NUM")==0)
		return 1;
	if(strcmp(str,"RNUM")==0)
		return 1;
	if(strcmp(str,"TRUE")==0)
		return 1;
	if(strcmp(str,"FALSE")==0)
		return 1;
	return 0;
}

int assignTypeToVar(char str[50])
{
	if(strcmp(str,"NUM")==0)
		return INTEGER;
	if(strcmp(str,"RNUM")==0)
		return REAL;
	if(strcmp(str,"TRUE")==0)
		return BOOLEAN;
	if(strcmp(str,"FALSE")==0)
		return BOOLEAN;
	return ERROR;
}

int checkActualArrayOrNot(node *ptr1)
{
    Token *tkn=tokenList_head;
    while(ptr1->ptr!=tkn) tkn=tkn->Next;
    tkn=tkn->Next;
    if(strcmp(tkn->tokenName,"SQBO")==0) return 0;
    return 1;
}


//called after making ast
//called on ast
void checkTypes(node* head)
{
	if(head==NULL)
		return;
    //1 means non terminal
	if(head->check && strcmp(hashed_string[1][head->hash_val],"<moduleReuseStmt")==0)
		return;

	node *temp = head->child;

	//call this function recursively on all its children
	while(temp!=NULL)
	{
		checkTypes(temp);
		temp = temp->Next;
		/*if(temp){
            if(temp->ptr)
            {
                if(temp->ptr->tokenVal)
                    printf("%s\n", temp->ptr->tokenVal->tokenVal);
            }
		}*/
	}
	int op;
	if(head->check==0)  //its on a terminal now
	{
        //if(head->ptr && head->ptr->tokenVal)
        //    printf("%s\n",head->ptr->tokenVal->tokenVal);

		if(strcmp(head->ptr->tokenName,"SQBO")==0)
        {
			storeStack[++condition5] = '[';
		}
		else if(strcmp(head->ptr->tokenName,"SQBC")==0)
			condition5--;
		else if(condition5!=-1)	// should not push id[index] in the stack
		{
			if(strcmp(head->ptr->tokenName,"ID")==0)
			{
				if(head->sym_ptr->type!=INTEGER && strcmp(head->ptr->tokenName,"ID")!=0)
				{
					semantic_error = 1;
					printf("ERROR (%d): DATATYPE of the indexing variable must be INTEGER or an ID\n",head->ptr->rowNum);
				}
			}
			else if(head->ptr && strcmp(head->ptr->tokenName, "NUM") == 0)    //bound checking for static arrays;
            {
                if(head->Next && strcmp(head->Next->ptr->tokenName, "SQBC")==0)
                {
                    int lb, hb, index;
                    node *idOfArray = MainStack[tchinter];

                    if(idOfArray && idOfArray->sym_ptr){
                        lb = idOfArray->sym_ptr->lb;
                        hb = idOfArray->sym_ptr->ub;
                    }
                    if(head->ptr && head->ptr->tokenVal)
                    {
                        //printf("GOLGUPPI\n\n\n");
                        index = atoi(head->ptr->tokenVal->tokenVal);
                        if(lb!=-1 && hb!=-1 && (index < lb || index > hb))
                        {
                            printf("ERROR (%d): Array '%s' index out of bounds error\n", head->ptr->rowNum, idOfArray->ptr->tokenVal->tokenVal);
                        }
                        /*else//no bound error nothing...an element like A[4] is called IN BOUNDS...put it on the stack.
                        {
                            //get its subtype
                            int type;
                            if(idOfArray->sym_ptr)
                                type = idOfArray->sym_ptr->subtype;
                            else
                                type = 5;
                            head->sym_ptr->type = type;
                            head->sym_ptr->arrayHash = idOfArray->hash_val;

                            if(type == INTEGER)
                            {
                                strcpy(head->ptr->tokenName, "NUM");
                            }
                            else if(type == REAL)
                            {
                                strcpy(head->ptr->tokenName, "RNUM");
                            }
                            else if(type == BOOLEAN)
                            {
                                strcpy(head->ptr->tokenName, "TRUE");
                            }
                            else
                            {
                                strcpy(head->ptr->tokenName, "ERROR");
                            }
                            head->sym_ptr->index = index;
                            MainStack[tchinter] = head;
                        }*/
                    }
                }
            }
			return;
		}
		else if(returnVarType(head->ptr->tokenName))    //if it is indeed one of the vars
		{
			MainStack[++tchinter] = head;   //its an array of nodes
		}
		else if(head->sym_ptr && head->sym_ptr->isunop == 1)     //is ARITHMETICOP
        {
            int t1, ck1, ck2, raon1=0;
			node *ptr1;
			op = operationNode(head->ptr->tokenName);

			//get type of t1
			if(MainStack[tchinter--]->sym_ptr==NULL)
			{
			    t1 = assignTypeToVar(MainStack[tchinter+1]->ptr->tokenName);
			    ptr1 = MainStack[tchinter+1];
                //if(MainStack[tchinter+1]->ptr->tokenVal)
                //    printf("in here 1: %s of type %d\n",MainStack[tchinter+1]->ptr->tokenVal->tokenVal, t1);
			}
			else
			{
			    t1 = MainStack[tchinter+1]->sym_ptr->type;
			    ptr1 = MainStack[tchinter+1];
                //if(MainStack[tchinter+1]->ptr->tokenVal)
                //    printf("in here 2: %s of type %d\n",MainStack[tchinter+1]->ptr->tokenVal->tokenVal, t1);

				if(t1==ARRAY){
                    ck1=ARRAY;
                    raon1=checkActualArrayOrNot(ptr1);
                    //if(b) printf("actual array id only at line %d\n",ptr1->ptr->rowNum);
                    //else printf("SQBO ALSO array id only at line %d\n",ptr1->ptr->rowNum);
					t1 = MainStack[tchinter+1]->sym_ptr->subtype;
                    ptr1 = MainStack[tchinter+1];
                }
			}

			node* temp = getParseTreeNode(0,0,NULL);
			temp->ptr = (struct Token*)malloc(sizeof(struct Token));
			temp->ptr->rowNum = MainStack[tchinter+1]->ptr->rowNum;
			temp->sym_ptr = (fields*)malloc(sizeof(fields));
                //printf("thala1\n");

			if(raon1 && op!=ASSIGNOP)    //both A, B array variables and op is something other than assignop
            {
                printf("ERROR (%d): Arrays do not support such operations with unary ops\n",head->ptr->rowNum);
            }
			else if(t1==INTEGER && op==ARITHMETICOP)
			{
				temp->sym_ptr->type = INTEGER;
				//if(strcmp(head->ptr->tokenName, "MINUS") == 0)
                MainStack[++tchinter] = temp;
			}
			else if(t1==REAL && op==ARITHMETICOP)
			{
				temp->sym_ptr->type = REAL;
				MainStack[++tchinter] = temp;
			}
			else
			{
				semantic_error = 1;
				if(t1 != -1 && t1 != 5)
                {
                    //printf("%d %d\n",t1, t2);
                    //printf("thala1\n");
                    if(ptr1->ptr && ptr1->ptr->tokenVal)
                        printf("ERROR (%d): Type mismatch error on '%s' by unary ops\n", head->ptr->rowNum, ptr1->ptr->tokenVal->tokenVal);
                    else
                        printf("ERROR (%d): Type mismatch error\n", head->ptr->rowNum);
                }
				temp->sym_ptr->type = ERROR;
				MainStack[++tchinter] = temp;
			}
        }
		else if(op=operationNode(head->ptr->tokenName))  //operationNode any other one than unary operations
		{
		    //printf("value of op is %d and it is %s\n", op, head->ptr->tokenName);

			int t1,t2,ck1,ck2,raon1=0,raon2=0;
			node *ptr1, *ptr2;
			//get type of t1
			if(MainStack[tchinter--]->sym_ptr==NULL)
			{
			    t1 = assignTypeToVar(MainStack[tchinter+1]->ptr->tokenName);
			    ptr1 = MainStack[tchinter+1];
                //if(MainStack[tchinter+1]->ptr->tokenVal)
                //    printf("in here 1: %s of type %d\n",MainStack[tchinter+1]->ptr->tokenVal->tokenVal, t1);

			}
			else
			{
			    t1 = MainStack[tchinter+1]->sym_ptr->type;
			    ptr1 = MainStack[tchinter+1];
                //if(MainStack[tchinter+1]->ptr->tokenVal)
                //    printf("in here 2: %s of type %d\n",MainStack[tchinter+1]->ptr->tokenVal->tokenVal, t1);

				if(t1==ARRAY){
                    ck1=ARRAY;
                    raon1=checkActualArrayOrNot(ptr1);
                    //if(b) printf("actual array id only at line %d\n",ptr1->ptr->rowNum);
                    //else printf("SQBO ALSO array id only at line %d\n",ptr1->ptr->rowNum);
					t1 = MainStack[tchinter+1]->sym_ptr->subtype;
                    ptr1 = MainStack[tchinter+1];
                }
			}
            //printf("thala\n");
            //get type of t2
			if(MainStack[tchinter--]->sym_ptr==NULL)
			{
			    //printf("thala\n");
			    t2 = assignTypeToVar(MainStack[tchinter+1]->ptr->tokenName);
				ptr2 = MainStack[tchinter+1];
                //if(MainStack[tchinter+1]->ptr->tokenVal)
                 //   printf("in here 3: %s of type %d\n",MainStack[tchinter+1]->ptr->tokenVal->tokenVal, t2);
			}
			else
			{
			    //printf("thala\n");
			    t2 = MainStack[tchinter+1]->sym_ptr->type;
			    //printf("thala\n");
			    ptr2 = MainStack[tchinter+1];
			    //if(MainStack[tchinter+1]->ptr->tokenVal)
                 //   printf("in here 4: %s of type %d\n",MainStack[tchinter+1]->ptr->tokenVal->tokenVal, t2);
                //printf("in here 4: of type %d\n", t2);
				//printf("%s\n",MainStack[tchinter+1]->ptr->tokenVal->tokenVal);
				if(t2==ARRAY){
                    ck2=ARRAY;
                    raon2=checkActualArrayOrNot(ptr2);
                    //if(b) printf("actual array id only at line %d\n",ptr2->ptr->rowNum);
                    //else printf("SQBO ALSO array id only at line %d\n",ptr2->ptr->rowNum);
					t2 = MainStack[tchinter+1]->sym_ptr->subtype;
					ptr2 = MainStack[tchinter+1];
                }
			}
			//printf("thala1\n");

			node* temp = getParseTreeNode(0,0,NULL);
			temp->ptr = (struct Token*)malloc(sizeof(struct Token));
			temp->ptr->rowNum = MainStack[tchinter+2]->ptr->rowNum;
			temp->sym_ptr = (fields*)malloc(sizeof(fields));
                //printf("thala1\n");
			if(op == ASSIGNOP && raon1 && raon2)
            {
                int subt1, subt2;
                subt1 = ptr1->sym_ptr->subtype;
                subt2 = ptr2->sym_ptr->subtype;

                int l1 = ptr1->sym_ptr->lb, l2 = ptr2->sym_ptr->lb, u1 = ptr1->sym_ptr->ub, u2 = ptr2->sym_ptr->ub;

                if(subt1 == subt2 && l1==l2 && u1==u2)
                {
                    //do nothing it works
                    //doubt when do we relplace the 2 aarrays...in codegen?
                }
                else
                {
                    printf("ERROR (%d): The 2 arrays '%s' and '%s' CANNOT be equated due to difference in types/bounds\n", head->ptr->rowNum, ptr1->ptr->tokenVal->tokenVal, ptr2->ptr->tokenVal->tokenVal);
                }
            }
            else if(raon1  && raon2 && op!=ASSIGNOP){
                printf("ERROR (%d): Arrays do not support such operations\n",head->ptr->rowNum);
            }
            else if(raon1 && !raon2 || !raon1 && raon2){
                printf("ERROR (%d): Arrays do not support operations with other datatypes\n",head->ptr->rowNum);
            }

			else if(op==ASSIGNOP && t1==t2)
			{
			    //do nothing
			}
			else if(op==ASSIGNOP && t1!=t2)
            {
                //printf("in here 5\n");
                //printf("%d  %d\n",t1, t2);
                if(t1 && -1 && t2 != -1 && t1!=5 && t2!=5)
                    printf("ERROR (%d): assignment operation mistake - type mismatch error\n",head->ptr->rowNum);
            }
			else if(t1==INTEGER && t2==INTEGER && op==ARITHMETICOP)
			{
			    //printf("yes it was\n");
				temp->sym_ptr->type = INTEGER;
				MainStack[++tchinter] = temp;
			}
			else if(t1==REAL && t2==REAL && op==ARITHMETICOP)
			{
				temp->sym_ptr->type = REAL;
				MainStack[++tchinter] = temp;
				//printf("inserting new made node of type REAL\n");
			}
			//else if(t1==INTEGER && t2==INTEGER && op==)
			else if(t1==INTEGER && t2==INTEGER && op==RELATIONALOP)
			{
				temp->sym_ptr->type = BOOLEAN;
				MainStack[++tchinter] = temp;
				//printf("inserting new made node of type BOOLEAN\n");
			}
			else if(t1==REAL && t2==REAL && op==RELATIONALOP)
			{
				temp->sym_ptr->type = BOOLEAN;
				MainStack[++tchinter] = temp;
				//printf("inserting new made node of type BOOLEAN\n");
			}
			else if(t1==BOOLEAN && t2==BOOLEAN && op==LOGICALOP)
			{
				temp->sym_ptr->type = BOOLEAN;
				MainStack[++tchinter] = temp;
				//printf("inserting new made node of type BOOLEAN\n");
			}
			else
			{

				semantic_error = 1;
				if(t1 != -1 && t2 != -1 && t1 != 5 && t2 != 5)
                {
                    //printf("%d %d\n",t1, t2);
                    //printf("thala1\n");
                    if(ptr1->ptr && ptr1->ptr->tokenVal && ptr2->ptr && ptr2->ptr->tokenVal)
                        printf("ERROR (%d): Type mismatch error between '%s' and '%s'\n", head->ptr->rowNum, ptr1->ptr->tokenVal->tokenVal, ptr2->ptr->tokenVal->tokenVal);
                    else
                        printf("ERROR (%d): Type mismatch error\n", head->ptr->rowNum);
                }
				temp->sym_ptr->type = ERROR;
				MainStack[++tchinter] = temp;
			}
		}
	}
	else if(head->par!=NULL && head->par->check==1 && strcmp(hashed_string[1][head->par->hash_val],"<iterativeStmt")==0 && strcmp(head->par->child->ptr->tokenName,"WHILE")==0)
	{
		if(head->par->child->Next==head)
		{
			int t1;
			if(MainStack[tchinter]->sym_ptr==NULL)
				t1 = assignTypeToVar(MainStack[tchinter]->ptr->tokenName);
			else
			{
				t1 = MainStack[tchinter]->sym_ptr->type;
				if(t1==ARRAY)
					t1 = MainStack[tchinter]->sym_ptr->subtype;

			}
			if(t1!=BOOLEAN)
			{
				semantic_error = 1;
				printf("ERROR (%d): Expression in while statement does not evaluate to boolean\n",head->par->child->ptr->rowNum);
			}
		}
	}
}

void s1(node *head, int rowNum)
{
    if(head->child->Next->Next->Next->check==0)
    {
        semantic_error = 1;
        printf("ERROR (%d): Switch with INTEGER datatype should have default statement\n",rowNum);
    }
    node* temp = head->child->Next->Next;
    while(!(temp->check==0 && strcmp(temp->ptr->tokenName,"epsilon")==0))
    {
        if(strcmp(temp->child->Next->ptr->tokenName,"NUM")!=0)
        {
            semantic_error = 1;
            printf("ERROR (%d): Switch with INTEGER datatype can only have NUM in CASE statement\n",temp->child->Next->ptr->rowNum);
        }
        temp = temp->child->Next->Next->Next;
    }
}

void s2(node *head, int rowNum)
{
    if(head->child->Next->Next->Next->check!=0)
    {
        semantic_error = 1;
        printf("ERROR (%d): Switch with BOOLEAN datatype can not have default statement\n",rowNum);
    }
    node* temp = head->child->Next->Next;
    while(!(temp->check==0 && strcmp(temp->ptr->tokenName,"epsilon")==0))
    {
        if(!(strcmp(temp->child->Next->ptr->tokenName,"TRUE")==0 || strcmp(temp->child->Next->ptr->tokenName,"FALSE")==0))
        {
            semantic_error = 1;
            printf("ERROR (%d): Switch with BOOLEAN datatype can only have TRUE or FALSE in CASE statement\n",temp->child->Next->ptr->rowNum);
        }
        temp = temp->child->Next->Next->Next;
    }
}

//INORDER TRAVERSAL
void switchStmtErrors(node *head)
{
	if(head==NULL)
		return;
	if(head->check && strcmp(hashed_string[1][head->hash_val],"<condionalStmt")==0)
	{
		//printf("found switch.................\n");
		int t = (head->child->Next)->sym_ptr->type;
		//int t = BOOLEAN;
		int rowNum = (head->child->Next)->ptr->rowNum;

		if(t==INTEGER)
		{
			s1(head, rowNum);
		}
		else if(t==BOOLEAN)
		{
			s2(head, rowNum);
		}
		else if(t==REAL)
		{
			semantic_error = 1;
			printf("ERROR (%d): Switch ID can not have REAL datatype\n",rowNum);
		}
		else
		{
			semantic_error = 1;
			printf("ERROR (%d): Switch ID invalid datatype\n",rowNum);
		}
		return;
	}
	//VISTING ITS CHILDREN
	node* temp = head->child;
	while(temp!=NULL)
	{
		switchStmtErrors(temp);
		temp = temp->Next;
	}
}

