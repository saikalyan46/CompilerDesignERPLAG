#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

#define BOOLEAN  3
#define LOGICALOP  2
#define ERROR  5
#define REAL  2
#define MOD  1009
#define ARRAY  4
#define RELATIONALOP  3
#define ARITHMETICOP  1
#define EXPR  325
#define INTEGER  1
#define epsilon  101
#define ASSIGNOP  4

struct left_assign{
	int is_dynamic;
	int is_array;
	int value;
	fields *left_temp;
	int offset;
};

int switchCnt = 0;

int relopCnt = 0;

// int is_array = 0;
// fields *left_temp = NULL;
//new
int boolean_print = 0;
int array_scan = 0;
int array_print = 0;

int is_array_declared = 0;
int is_array_dynamic = 0;
fields *array_temp_declared = NULL;
int array_declared_value = -1;
int array_declared_offset = -1;
struct left_assign assign_temp;

int switchR = 0;
int forCnt = 0;
int whileCnt = 0;
int indLoc = 0;

void assignopCode(node* head)
{
	if(head==NULL)
		return;
	node *temp = head->child;
	while(temp!=NULL)
	{
		assignopCode(temp);
		temp = temp->Next;
	}

	int op;
	//this node
	if(head->check==0)
	{
		if(strcmp(head->ptr->tokenName,"ID")==0 )
		{
			//fprintf(fp_asm,"in ID\n");

			MainStack[++tchinter] = head;
			if(assign_temp.left_temp!=NULL && assign_temp.is_array == 1 && assign_temp.value==-1 && assign_temp.offset == -1){
				assign_temp.offset = head->sym_ptr->offset;
			}
			if (assign_temp.left_temp==NULL){
				assign_temp.left_temp = head->sym_ptr;
				if(head->sym_ptr->type==ARRAY){
					assign_temp.is_array = 1;
					if(head->sym_ptr->isDynamic == 1){
						assign_temp.is_dynamic = 1;
					}
				}
			}
			if(head->sym_ptr->type==ARRAY){
				is_array_declared = 1;
				array_temp_declared = head->sym_ptr;
				if(head->sym_ptr->isDynamic == 1){
					is_array_dynamic = 1;
				}
				return;
			}
			if (is_array_declared == 1 && array_declared_value == -1 && is_array_dynamic == 0)
			{
				array_declared_offset = head->sym_ptr->offset;
				fprintf(fp_asm,"XOR ECX, ECX\n");
				fprintf(fp_asm,"MOV AX, 2\n");
				fprintf(fp_asm,"MOV CX, word[MEM+%d]\n", array_declared_offset);
				fprintf(fp_asm,"MOV DX, %d\n", array_temp_declared->lb);
				fprintf(fp_asm,"CMP CX,DX\n");
				fprintf(fp_asm,"JL error_bound\n");
				fprintf(fp_asm,"MOV DX, %d\n", array_temp_declared->ub);
				fprintf(fp_asm,"CMP CX,DX\n");
				fprintf(fp_asm,"JG error_bound\n");
				fprintf(fp_asm,"SUB CX, %d\n",array_temp_declared->lb);
				fprintf(fp_asm,"MUL CX\n");
				fprintf(fp_asm,"MOV CX,AX\n");
				fprintf(fp_asm,"MOV AX,word[MEM+%d+ECX]\n",array_temp_declared->offset);
				fprintf(fp_asm,"PUSH AX\n");
				array_temp_declared = NULL;
				array_declared_value = -1;
				is_array_declared = 0;
				array_declared_offset = -1;
				is_array_dynamic = 0;
				return;
			}
			if (is_array_declared == 1 && array_declared_value == -1 && is_array_dynamic == 1)
			{
				array_declared_offset = head->sym_ptr->offset;
				fprintf(fp_asm,"XOR ECX, ECX\n");
				fprintf(fp_asm,"MOV AX, 2\n");
				fprintf(fp_asm,"MOV CX, word[MEM+%d]\n", array_declared_offset);
				fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", array_temp_declared->lowerBound->offset);
				fprintf(fp_asm,"CMP CX,DX\n");
				fprintf(fp_asm,"JL error_bound\n");
				fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", array_temp_declared->higherBound->offset);
				fprintf(fp_asm,"CMP CX,DX\n");
				fprintf(fp_asm,"JG error_bound\n");
				fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", array_temp_declared->lowerBound->offset);
				fprintf(fp_asm,"SUB CX, DX\n");
				fprintf(fp_asm,"MUL CX\n");
				fprintf(fp_asm,"MOV CX,AX\n");
				fprintf(fp_asm,"MOV AX,word[DYNA+%d+ECX]\n",array_temp_declared->offset*10);
				fprintf(fp_asm,"PUSH AX\n");
				array_temp_declared = NULL;
				array_declared_value = -1;
				is_array_declared = 0;
				array_declared_offset = -1;
				is_array_dynamic = 0;
				return;
			}

			fprintf(fp_asm,"MOV AX,word[MEM+%d]\n",head->sym_ptr->offset);
			fprintf(fp_asm,"PUSH AX\n");

		}
		else if(strcmp(head->ptr->tokenName,"NUM")==0)
		{
			MainStack[++tchinter] = head;


			if(assign_temp.left_temp!=NULL && assign_temp.is_array == 1 && assign_temp.value==-1 && assign_temp.offset == -1){
				assign_temp.value = atoi(head->ptr->tokenVal->tokenVal);
			}

			if (is_array_declared == 1 && array_declared_offset == -1 && is_array_dynamic == 0)
			{
				array_declared_value = atoi(head->ptr->tokenVal->tokenVal);
				// fprintf(fp_asm,"MOV CX, %d\n", array_declared_value);
				// fprintf(fp_asm,"MOV DX, %d\n", array_temp_declared->lb);
				// fprintf(fp_asm,"JG error_bound\n");
				fprintf(fp_asm,"MOV AX,word[MEM+%d]\n",array_temp_declared->offset +(array_declared_value-array_temp_declared->lb)*2);
				fprintf(fp_asm,"PUSH AX\n");
				is_array_dynamic = 0;
				array_temp_declared = NULL;
				array_declared_value = -1;
				is_array_declared = 0;
				array_declared_offset = -1;
				return;
			}
			if (is_array_declared == 1 && array_declared_offset == -1 && is_array_dynamic == 1)
			{
				// array_declared_value = atoi(head->ptr->tokenVal->tokenVal);
				// fprintf(fp_asm,"MOV CX, %d\n", array_declared_value);
				// fprintf(fp_asm,"MOV DX, %d\n", array_temp_declared->lb);
				// fprintf(fp_asm,"CMP CX,DX\n");
				// fprintf(fp_asm,"JL error_bound\n");
				// fprintf(fp_asm,"MOV DX, %d\n", array_temp_declared->ub);
				// fprintf(fp_asm,"CMP CX,DX\n");
				// fprintf(fp_asm,"JG error_bound\n");
				// fprintf(fp_asm,"MOV AX,word[MEM+%d]\n",array_temp_declared->offset +(array_declared_value-array_temp_declared->lb)*2);
				// fprintf(fp_asm,"PUSH AX\n");

				array_declared_value = atoi(head->ptr->tokenVal->tokenVal);
				fprintf(fp_asm,"XOR ECX, ECX\n");
				fprintf(fp_asm,"MOV AX, 2\n");
				fprintf(fp_asm,"MOV CX, %d\n", array_declared_value);
				fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", array_temp_declared->lowerBound->offset);
				fprintf(fp_asm,"CMP CX,DX\n");
				fprintf(fp_asm,"JL error_bound\n");
				fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", array_temp_declared->higherBound->offset);
				fprintf(fp_asm,"CMP CX,DX\n");
				fprintf(fp_asm,"JG error_bound\n");
				fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", array_temp_declared->lowerBound->offset);
				fprintf(fp_asm,"SUB CX, DX\n");
				fprintf(fp_asm,"MUL CX\n");
				fprintf(fp_asm,"MOV CX,AX\n");
				fprintf(fp_asm,"MOV AX,word[DYNA+%d+ECX]\n",array_temp_declared->offset*10);
				fprintf(fp_asm,"PUSH AX\n");
				array_temp_declared = NULL;
				array_declared_value = -1;
				is_array_declared = 0;
				array_declared_offset = -1;
				is_array_dynamic = 0;
				return;
				return;
			}

			fprintf(fp_asm,"MOV AX,%s\n",head->ptr->tokenVal->tokenVal);
			fprintf(fp_asm,"PUSH AX\n");


		}
		else if(strcmp(head->ptr->tokenName,"TRUE")==0)
		{
			//fprintf(fp_asm,"in NUM\n");
			MainStack[++tchinter] = head;
			fprintf(fp_asm,"MOV AX,1\n");
			fprintf(fp_asm,"PUSH AX\n");
		}
		else if(strcmp(head->ptr->tokenName,"FALSE")==0)
		{
			//fprintf(fp_asm,"in NUM\n");
			MainStack[++tchinter] = head;
			fprintf(fp_asm,"MOV AX,0\n");
			fprintf(fp_asm,"PUSH AX\n");
		}
		else if(op=operationNode(head->ptr->tokenName))
		{
			node* temp = getParseTreeNode(0,0,NULL);

			fprintf(fp_asm,"POP BX\n");
			fprintf(fp_asm,"POP AX\n");
			tchinter--;
			int a;//new
			// checking for array
			if(MainStack[tchinter]->par!=NULL && MainStack[tchinter]->par->child!=MainStack[tchinter])
			{
				//fprintf(fp_asm,"got in\n");
				node* tt = MainStack[tchinter]->par->child;
				while(tt->Next!=MainStack[tchinter])
					tt = tt->Next;

				if(tt->check==0 && strcmp(tt->ptr->tokenName,"SQBO")==0)
				{
					//fprintf(fp_asm,"yes found\n");
					if(MainStack[tchinter]->sym_ptr==NULL)
					{
						// int
						a = atoi(MainStack[tchinter]->ptr->tokenVal->tokenVal);
						fprintf(fp_asm,";MOV word[arr],%d\n",2*a-2);
					}
					else
					{
						fprintf(fp_asm,";MOV AX,word[MEM+%d]\n",MainStack[tchinter]->sym_ptr->offset);
						if(MainStack[tchinter]->sym_ptr->subtype==INTEGER)
						{
							fprintf(fp_asm,";IMUL 2\n");
							fprintf(fp_asm,";SUB AX,2\n");
						}
						else
							fprintf(fp_asm,";SUB AX,1\n");
						fprintf(fp_asm,";MOV word[arr],AX\n");
					}
					indLoc = 1;
					fprintf(fp_asm,";POP AX\n");
					tchinter--;
				}
				else
					tchinter--;
			}

			if(op==ASSIGNOP)
			{
				if(MainStack[tchinter]->sym_ptr->type==BOOLEAN)
				{
					if(indLoc)
					{
						indLoc = 0;
						fprintf(fp_asm,";MOV CX,word[arr]\n");
						fprintf(fp_asm,";AND ECX,0x00001111\n");
						fprintf(fp_asm,";MOV byte[MEM+%d+ECX],BL\n",MainStack[tchinter]->sym_ptr->offset);
						fprintf(fp_asm,"MOV byte[MEM+%d],BL\n",MainStack[tchinter]->sym_ptr->offset+(a-MainStack[tchinter]->sym_ptr->lb)*1);
					}
					else
						fprintf(fp_asm,"MOV byte[MEM+%d],BL\n",MainStack[tchinter]->sym_ptr->offset);
				}
				else
				{
					if(indLoc)
					{
						indLoc = 0;
						fprintf(fp_asm,";MOV CX,word[arr]\n");
						fprintf(fp_asm,";AND ECX,0x00001111\n");
						fprintf(fp_asm,";MOV word[MEM+%d+ECX],BX\n",MainStack[tchinter]->sym_ptr->offset);
						fprintf(fp_asm,";MOV word[MEM+%d],BX\n",MainStack[tchinter]->sym_ptr->offset+(a-MainStack[tchinter]->sym_ptr->lb)*2);
						if (assign_temp.is_array == 1 && assign_temp.value == -1 && assign_temp.offset !=-1 && assign_temp.is_dynamic == 1)
						{
							fprintf(fp_asm,"XOR ECX, ECX\n");
							fprintf(fp_asm,"MOV AX, 2\n");
							fprintf(fp_asm,"MOV CX, word[MEM+%d]\n", assign_temp.offset);
							fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", assign_temp.left_temp->lowerBound->offset);
							fprintf(fp_asm,"CMP CX,DX\n");
							fprintf(fp_asm,"JL error_bound\n");
							fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", assign_temp.left_temp->higherBound->offset);
							fprintf(fp_asm,"CMP CX,DX\n");
							fprintf(fp_asm,"JG error_bound\n");
							fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", assign_temp.left_temp->lowerBound->offset);
							fprintf(fp_asm,"SUB CX, DX\n");
							fprintf(fp_asm,"MUL CX\n");
							fprintf(fp_asm,"MOV CX,AX\n");
							fprintf(fp_asm,"MOV word[DYNA+%d+ECX],BX\n",assign_temp.left_temp->offset*10);
							// fprintf(fp_asm,"PUSH AX\n");
							tchinter--;
							return;
						}
						if (assign_temp.is_array == 1 && assign_temp.value != -1 && assign_temp.offset ==-1 && assign_temp.is_dynamic == 1)
						{
							fprintf(fp_asm,"XOR ECX, ECX\n");
							fprintf(fp_asm,"MOV AX, 2\n");
							fprintf(fp_asm,"MOV CX, %d\n", assign_temp.value);
							fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", assign_temp.left_temp->lowerBound->offset);
							fprintf(fp_asm,"CMP CX,DX\n");
							fprintf(fp_asm,"JL error_bound\n");
							fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", assign_temp.left_temp->higherBound->offset);
							fprintf(fp_asm,"CMP CX,DX\n");
							fprintf(fp_asm,"JG error_bound\n");
							fprintf(fp_asm,"MOV DX, word[MEM+%d]\n", assign_temp.left_temp->lowerBound->offset);
							fprintf(fp_asm,"SUB CX, DX\n");
							fprintf(fp_asm,"MUL CX\n");
							fprintf(fp_asm,"MOV CX,AX\n");
							fprintf(fp_asm,"MOV word[DYNA+%d+ECX],BX\n",assign_temp.left_temp->offset*10);
							// fprintf(fp_asm,"PUSH AX\n");
							tchinter--;
							return;
						}

						if(assign_temp.is_array == 1 && assign_temp.value == -1 && assign_temp.offset !=-1 && assign_temp.is_dynamic != 1){
							fprintf(fp_asm,"XOR ECX, ECX\n");
							fprintf(fp_asm,"MOV AX, 2\n");
							fprintf(fp_asm,"MOV CX, word[MEM+%d]\n", assign_temp.offset);
							fprintf(fp_asm,"MOV DX, %d\n", assign_temp.left_temp->lb);
							fprintf(fp_asm,"CMP CX,DX\n");
							fprintf(fp_asm,"JL error_bound\n");
							fprintf(fp_asm,"MOV DX, %d\n", assign_temp.left_temp->ub);
							fprintf(fp_asm,"CMP CX,DX\n");
							fprintf(fp_asm,"JG error_bound\n");
							// fprintf(fp_asm,"SUB CX, %d\n",array_temp_declared->lb);
							// fprintf(fp_asm,"MUL CX\n");
							// fprintf(fp_asm,"MOV CX,AX\n");
							// fprintf(fp_asm,"MOV AX,word[MEM+%d+ECX]\n",array_temp_declared->offset);
							// fprintf(fp_asm,"PUSH AX\n");




							// fprintf(fp_asm,"XOR ECX, ECX\n");
							// fprintf(fp_asm,"MOV AX, 2\n");
							fprintf(fp_asm,"MOV CX, word[MEM+%d]\n", assign_temp.offset);
							fprintf(fp_asm,"SUB CX, %d\n", assign_temp.left_temp->lb);
							fprintf(fp_asm,"MUL CX\n");
							fprintf(fp_asm,"MOV CX,AX\n");
							fprintf(fp_asm,"MOV word[MEM+%d+ECX], BX\n",assign_temp.left_temp->offset);
							tchinter--;
							return;
						}


						fprintf(fp_asm,"MOV word[MEM+%d],BX\n", assign_temp.left_temp->offset+(assign_temp.value-assign_temp.left_temp->lb)*2);
					}
					else
						fprintf(fp_asm,"MOV word[MEM+%d],BX\n",MainStack[tchinter]->sym_ptr->offset);
				}
				tchinter--;
			}
			else if(strcmp(head->ptr->tokenName,"PLUS")==0)
			{
				fprintf(fp_asm,"ADD AX,BX\n");
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"MINUS")==0)
			{
				fprintf(fp_asm,"SUB AX,BX\n");
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"MUL")==0)
			{
				fprintf(fp_asm,"IMUL BX\n");
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"DIV")==0)
			{
				fprintf(fp_asm,"IDIV BX\n");
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"AND")==0)
			{
				fprintf(fp_asm,"AND AX,BX\n");
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"OR")==0)
			{
				fprintf(fp_asm,"AND AX,BX\n");
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"LT")==0)
			{
				fprintf(fp_asm,"CMP AX,BX\n");
				fprintf(fp_asm,"JL TRUE%d\n",relopCnt);
				fprintf(fp_asm,"MOV AX,0\n");
				fprintf(fp_asm,"JMP FALSE%d\n",relopCnt);
				fprintf(fp_asm,"TRUE%d: MOV AX,1\n",relopCnt);
				fprintf(fp_asm,"FALSE%d:\n",relopCnt);
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
				relopCnt++;
			}
			else if(strcmp(head->ptr->tokenName,"LE")==0)
			{
				fprintf(fp_asm,"CMP AX,BX\n");
				fprintf(fp_asm,"JLE TRUE%d\n",relopCnt);
				fprintf(fp_asm,"MOV AX,0\n");
				fprintf(fp_asm,"JMP FALSE%d\n",relopCnt);
				fprintf(fp_asm,"TRUE%d: MOV AX,1\n",relopCnt);
				fprintf(fp_asm,"FALSE%d:\n",relopCnt);
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
				relopCnt++;
			}
			else if(strcmp(head->ptr->tokenName,"GT")==0)
			{
				fprintf(fp_asm,"CMP AX,BX\n");
				fprintf(fp_asm,"JG TRUE%d\n",relopCnt);
				fprintf(fp_asm,"MOV AX,0\n");
				fprintf(fp_asm,"JMP FALSE%d\n",relopCnt);
				fprintf(fp_asm,"TRUE%d: MOV AX,1\n",relopCnt);
				fprintf(fp_asm,"FALSE%d:\n",relopCnt);
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
				relopCnt++;
			}
			else if(strcmp(head->ptr->tokenName,"GE")==0)
			{
				fprintf(fp_asm,"CMP AX,BX\n");
				fprintf(fp_asm,"JGE TRUE%d\n",relopCnt);
				fprintf(fp_asm,"MOV AX,0\n");
				fprintf(fp_asm,"JMP FALSE%d\n",relopCnt);
				fprintf(fp_asm,"TRUE%d: MOV AX,1\n",relopCnt);
				fprintf(fp_asm,"FALSE%d:\n",relopCnt);
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
				relopCnt++;
			}
			else if(strcmp(head->ptr->tokenName,"EQ")==0)
			{
				fprintf(fp_asm,"CMP AX,BX\n");
				fprintf(fp_asm,"JE TRUE%d\n",relopCnt);
				fprintf(fp_asm,"MOV AX,0\n");
				fprintf(fp_asm,"JMP FALSE%d\n",relopCnt);
				fprintf(fp_asm,"TRUE%d: MOV AX,1\n",relopCnt);
				fprintf(fp_asm,"FALSE%d:\n",relopCnt);
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
				relopCnt++;
			}
			else if(strcmp(head->ptr->tokenName,"NQ")==0)
			{
				fprintf(fp_asm,"CMP AX,BX\n");
				fprintf(fp_asm,"JNE TRUE%d\n",relopCnt);
				fprintf(fp_asm,"MOV AX,0\n");
				fprintf(fp_asm,"JMP FALSE%d\n",relopCnt);
				fprintf(fp_asm,"TRUE%d: MOV AX,1\n",relopCnt);
				fprintf(fp_asm,"FALSE%d:\n",relopCnt);
				fprintf(fp_asm,"PUSH AX\n");
				MainStack[tchinter] = temp;
				relopCnt++;
			}
		}
	}
}

void conditionalopCode(node* head)
{
	switchR = 0;
	node* curr = head->child->Next;
	node* temp;

	if(curr->sym_ptr==NULL)
		fprintf(fp_asm,"null in switch Please check\n");
	else if(curr->sym_ptr->type==BOOLEAN)
	{
		fprintf(fp_asm,"MOV CL,byte[MEM+%d]\n",curr->sym_ptr->offset);
		temp = curr->Next;
		while(!(temp->check==0 && strcmp(temp->ptr->tokenName,"epsilon")==0))
		{
			if(strcmp((temp->child->Next)->ptr->tokenName,"TRUE")==0)
				fprintf(fp_asm,"CMP CL,1\n");
			else if(strcmp((temp->child->Next)->ptr->tokenName,"FALSE")==0)
				fprintf(fp_asm,"CMP CL,0\n");
			else
				fprintf(fp_asm,"CMP CL,%s\n",(temp->child->Next->ptr->tokenVal->tokenVal));
			fprintf(fp_asm,"JNZ C%d%d\n",switchCnt,switchR);
			codeGeneration(temp->child->Next->Next);
			fprintf(fp_asm,"JMP SWITCH_EXIT%d\n",switchCnt);
			fprintf(fp_asm,"C%d%d:\n",switchCnt,switchR);
			switchR++;
			temp = temp->child->Next->Next->Next;
		}
		fprintf(fp_asm,"SWITCH_EXIT%d:\n",switchCnt);
	}
	else if(curr->sym_ptr->type==INTEGER)
	{
		fprintf(fp_asm,"MOV CX,word[MEM+%d]\n",curr->sym_ptr->offset);
		temp = curr->Next;
		while(!(temp->check==0 && strcmp(temp->ptr->tokenName,"epsilon")==0))
		{
			fprintf(fp_asm,"CMP CX,%s\n",(temp->child->Next->ptr->tokenVal->tokenVal));
			fprintf(fp_asm,"JNZ C%d%d\n",switchCnt,switchR);
			codeGeneration(temp->child->Next->Next);
			fprintf(fp_asm,"JMP SWITCH_EXIT%d\n",switchCnt);
			fprintf(fp_asm,"C%d%d:\n",switchCnt,switchR);
			switchR++;
			temp = temp->child->Next->Next->Next;
		}
		//default statement
		codeGeneration(curr->Next->Next->child->Next);
		fprintf(fp_asm,"SWITCH_EXIT%d:\n",switchCnt);
	}
	else
		fprintf(fp_asm,"ERROR_22: invalid input in conditionalopCode\n");
	switchCnt++;
}

void loopOpCode(node* head)
{
	if(strcmp(head->child->ptr->tokenName,"FOR")==0)
	{
		forCnt++;
		node* curr = head->child->Next;
		node* num1 = curr->Next->child;
		node* num2 = num1->Next;

		fprintf(fp_asm,"MOV CX,%s\n",num1->ptr->tokenVal->tokenVal);
		fprintf(fp_asm,"MOV word[MEM+%d],CX\n",curr->sym_ptr->offset);
		fprintf(fp_asm,"FOR_LOOP%d:\n",forCnt);
		codeGeneration(curr->Next->Next);
		fprintf(fp_asm,"MOV CX,word[MEM+%d]\n",curr->sym_ptr->offset);
		fprintf(fp_asm,"INC CX\n");
		fprintf(fp_asm,"MOV word[MEM+%d],CX\n",curr->sym_ptr->offset);
		fprintf(fp_asm,"MOV AX,%s\n",num2->ptr->tokenVal->tokenVal);
		fprintf(fp_asm,"CMP CX,AX\n");
		fprintf(fp_asm,"JLE FOR_LOOP%d\n",forCnt);
	}
	else if(strcmp(head->child->ptr->tokenName,"WHILE")==0)
	{
		whileCnt++;
		codeGeneration(head->child->Next);
		fprintf(fp_asm,"POP AX\n");
		fprintf(fp_asm,"CMP AL,1\n");
		fprintf(fp_asm,"JNZ WHILE_END%d\n",whileCnt);
		fprintf(fp_asm,"WHILE_LOOP%d:\n",whileCnt);
		codeGeneration(head->child->Next->Next);
		codeGeneration(head->child->Next);
		fprintf(fp_asm,"POP AX\n");
		fprintf(fp_asm,"CMP AL,1\n");
		fprintf(fp_asm,"JZ WHILE_LOOP%d\n",whileCnt);
		fprintf(fp_asm,"WHILE_END%d:\n",whileCnt);
	}
}

void printCode(node *head)
{
	fprintf(fp_asm,"\n");
	fprintf(fp_asm,"mov rax,0\n");
	fprintf(fp_asm,"MOV rbx,0xFFFFFFFFFFFFFFFF\n");
	fprintf(fp_asm,"cmovl rax,rbx\n");

	if(head->child->Next->check==0)
	{
		if(strcmp(head->child->Next->ptr->tokenName,"TRUE")==0)
			fprintf(fp_asm,"mov AX,1\n");
		else if(strcmp(head->child->Next->ptr->tokenName,"FALSE")==0)
			fprintf(fp_asm,"mov AX,0\n");
		else
			fprintf(fp_asm,"mov AX,%s\n",head->child->Next->ptr->tokenVal->tokenVal);
	}
	else if(head->child->Next->child->Next->check==0 && strcmp(head->child->Next->child->Next->ptr->tokenName,"epsilon")==0){
		if(head->child->Next->child->sym_ptr->type==BOOLEAN){//boolean print
			// fprintf(fp_asm,"mov AL,1\n");
			fprintf(fp_asm,"mov AL,[MEM+%d]\n",head->child->Next->child->sym_ptr->offset);
			fprintf(fp_asm,"sub rsp, 0x28\n");
			fprintf(fp_asm,"push    rax\n");
			fprintf(fp_asm,"  push    rcx\n");
			fprintf(fp_asm,"\tmov     rdi, fal\n");
			fprintf(fp_asm,"CMP AL,0\n");
			fprintf(fp_asm,"JE is_fal%d\n",boolean_print);
			fprintf(fp_asm,"\tmov     rdi, tru\nis_fal%d:\n",boolean_print);
			// fprintf(fp_asm,"\tmov     rdx, rax\n");
			fprintf(fp_asm,"\txor     rax, rax\n");
			fprintf(fp_asm,"\tcall    printf\n");
			fprintf(fp_asm,"  pop     rcx\n");
			fprintf(fp_asm,"pop     rax\n");
			fprintf(fp_asm,"add rsp, 0x28\n");
			boolean_print ++;
			return;
		}
		else if(head->child->Next->child->sym_ptr->type==ARRAY && head->child->Next->child->sym_ptr->isDynamic != 1){
			fprintf(fp_asm,"sub rsp, 0x28\n");
			fprintf(fp_asm,"push    rax\n");
			fprintf(fp_asm,"  push    rcx\n");
			fprintf(fp_asm,"\tmov     rdi, out\n");
			// fprintf(fp_asm,"\tmov     rdx, rax\n");
			fprintf(fp_asm,"\txor     rax, rax\n");
			fprintf(fp_asm,"\tcall    printf\n");
			fprintf(fp_asm,"  pop     rcx\n");
			fprintf(fp_asm,"pop     rax\n");
			fprintf(fp_asm,"add rsp, 0x28\n");

			int i;
			for(i = head->child->Next->child->sym_ptr->lb;i<=head->child->Next->child->sym_ptr->ub;i++){
				fprintf(fp_asm,"\n");
				fprintf(fp_asm,"movsx EAX,word[MEM+%d]\n",head->child->Next->child->sym_ptr->offset+(i-head->child->Next->child->sym_ptr->lb)*2);
				fprintf(fp_asm,"sub rsp, 0x28\n");
				fprintf(fp_asm,"push    rax\n");
				fprintf(fp_asm,"  push    rcx\n");
				fprintf(fp_asm,"\tmov     rdi, out_array\n");
				fprintf(fp_asm,"\tmov     rsi, rax\n");
				fprintf(fp_asm,"\txor     rax, rax\n");
				fprintf(fp_asm,"\tcall    printf\n");
				fprintf(fp_asm,"  pop     rcx\n");
				fprintf(fp_asm,"pop     rax\n");
				fprintf(fp_asm,"add rsp, 0x28\n");
			}
			fprintf(fp_asm,"sub rsp, 0x28\n");
			fprintf(fp_asm,"push    rax\n");
			fprintf(fp_asm,"  push    rcx\n");
			fprintf(fp_asm,"\tmov     rdi, new_line\n");
			// fprintf(fp_asm,"\tmov     rdx, rax\n");
			fprintf(fp_asm,"\txor     rax, rax\n");
			fprintf(fp_asm,"\tcall    printf\n");
			fprintf(fp_asm,"  pop     rcx\n");
			fprintf(fp_asm,"pop     rax\n");
			fprintf(fp_asm,"add rsp, 0x28\n");

		}
		else if(head->child->Next->child->sym_ptr->type==ARRAY && head->child->Next->child->sym_ptr->isDynamic == 1){

			fprintf(fp_asm,"sub rsp, 0x28\n");
			fprintf(fp_asm,"push    rax\n");
			fprintf(fp_asm,"  push    rcx\n");
			fprintf(fp_asm,"\tmov     rdi, out\n");
			// fprintf(fp_asm,"\tmov     rdx, rax\n");
			fprintf(fp_asm,"\txor     rax, rax\n");
			fprintf(fp_asm,"\tcall    printf\n");
			fprintf(fp_asm,"  pop     rcx\n");
			fprintf(fp_asm,"pop     rax\n");
			fprintf(fp_asm,"add rsp, 0x28\n");
			fprintf(fp_asm,"\n");
			fprintf(fp_asm,"mov ax, word[MEM+%d]\n", head->child->Next->child->sym_ptr->higherBound->offset);
			fprintf(fp_asm,"mov bx, word[MEM+%d]\n", head->child->Next->child->sym_ptr->lowerBound->offset);
			fprintf(fp_asm,"sub ax, bx\n");
			fprintf(fp_asm,"inc ax\n");
			fprintf(fp_asm,"mov dx,ax\n");
			fprintf(fp_asm,"mov cx,0\n");
			fprintf(fp_asm,"print_dyna_array%d:\n",array_print);
			fprintf(fp_asm,"push DX\n");
			fprintf(fp_asm,"PUSH CX\n");
			fprintf(fp_asm,"XOR ECX,ECX\n");
			fprintf(fp_asm,"POP CX\n");
			fprintf(fp_asm,"PUSH CX\n");
			fprintf(fp_asm,"movsx EAX,word[DYNA+%d+ECX]\n",head->child->Next->child->sym_ptr->offset*10);
			fprintf(fp_asm,"sub rsp, 0x28\n");
			// fprintf(fp_asm,"MOV RDX,var+0\n");
			// fprintf(fp_asm,"MOV RCX,inputFormat\n");
			// fprintf(fp_asm,"XOR RAX,RAX\n");
			// fprintf(fp_asm,"call scanf\n");
			fprintf(fp_asm,"\tmov     rdi, out_array\n");
			fprintf(fp_asm,"\tmov     rsi, rax\n");
			fprintf(fp_asm,"\txor     rax, rax\n");
			fprintf(fp_asm,"\tcall    printf\n");
			// fprintf(fp_asm,"MOV AX,word[MEM+%d]\n",curr->sym_ptr->offset);
			// fprintf(fp_asm,"PUSH AX\n");
			// fprintf(fp_asm,"MOV BX,word[var+0]\n");
			fprintf(fp_asm,"add rsp, 0x28\n");
			// fprintf(fp_asm,"PUSH AX\n");
			// fprintf(fp_asm,"POP BX\n");
			// fprintf(fp_asm,"POP AX\n");
			fprintf(fp_asm,"POP CX\n");
			fprintf(fp_asm,"POP DX\n");
			// fprintf(fp_asm,"MOV word[DYNA+%d+ECX],BX\n",head->child->Next->child->sym_ptr->offset*10);
			fprintf(fp_asm,"add cx,2\n");
			fprintf(fp_asm,"dec dx\n");
			fprintf(fp_asm,"cmp dx,0\n");
			fprintf(fp_asm,"jne print_dyna_array%d\n",array_print);
			array_scan++;
			fprintf(fp_asm,"sub rsp, 0x28\n");
			fprintf(fp_asm,"push    rax\n");
			fprintf(fp_asm,"  push    rcx\n");
			fprintf(fp_asm,"\tmov     rdi, new_line\n");
			// fprintf(fp_asm,"\tmov     rdx, rax\n");
			fprintf(fp_asm,"\txor     rax, rax\n");
			fprintf(fp_asm,"\tcall    printf\n");
			fprintf(fp_asm,"  pop     rcx\n");
			fprintf(fp_asm,"pop     rax\n");
			fprintf(fp_asm,"add rsp, 0x28\n");
		}
		else{
			fprintf(fp_asm,"movsx EAX,word[MEM+%d]\n",head->child->Next->child->sym_ptr->offset);
			fprintf(fp_asm,"sub rsp, 0x28\n");
			fprintf(fp_asm,"push    rax\n");
			fprintf(fp_asm,"  push    rcx\n");
			fprintf(fp_asm,"\tmov     rdi, men\n");
			fprintf(fp_asm,"\tmov     rsi, rax\n");
			fprintf(fp_asm,"\txor     rax, rax\n");
			fprintf(fp_asm,"\tcall    printf\n");
			fprintf(fp_asm,"  pop     rcx\n");
			fprintf(fp_asm,"pop     rax\n");
			fprintf(fp_asm,"add rsp, 0x28\n");
		}
	}
	else
	{
		node* curr = head->child->Next->child;
		node* temp = curr->Next->child->Next;
		if(temp->sym_ptr==NULL)
		{
			int a = atoi(temp->ptr->tokenVal->tokenVal);
			fprintf(fp_asm,"MOV word[arr],%d\n",a=a*2-2);
		}
		else
		{
			fprintf(fp_asm,"MOV AX,word[MEM+%d]\n",temp->sym_ptr->offset);
			if(curr->sym_ptr->subtype==INTEGER)
			{
				fprintf(fp_asm,"IMUL 2\n");
				fprintf(fp_asm,"SUB AX,2\n");
			}
			else
				fprintf(fp_asm,"SUB AX,1\n");
			fprintf(fp_asm,"MOV word[arr],AX\n");
		}
		fprintf(fp_asm,"MOV CX,word[arr]\n");
		fprintf(fp_asm,"AND ECX,0x00001111\n");
		fprintf(fp_asm,";movsx EAX,word[MEM+%d+ECX]\n",curr->sym_ptr->offset);
		fprintf(fp_asm,"movsx EAX,word[MEM+%d]\n",curr->sym_ptr->offset+(atoi(temp->ptr->tokenVal->tokenVal)-curr->sym_ptr->lb)*2);
		// fprintf(fp_asm,"MOVSX word[MEM+%d],BX\n",temp->sym_ptr->offset+(atoi(temp->ptr->tokenVal->tokenVal)-temp->sym_ptr->lb)*2);
		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"push    rax\n");
		fprintf(fp_asm,"  push    rcx\n");
		fprintf(fp_asm,"\tmov     rdi, men\n");
		fprintf(fp_asm,"\tmov     rsi, rax\n");
		fprintf(fp_asm,"\txor     rax, rax\n");
		fprintf(fp_asm,"\tcall    printf\n");
		fprintf(fp_asm,"  pop     rcx\n");
		fprintf(fp_asm,"pop     rax\n");
		fprintf(fp_asm,"add rsp, 0x28\n");
	}

	// fprintf(fp_asm,"sub rsp, 0x28\n");
	// fprintf(fp_asm,"push    rax\n");
	// fprintf(fp_asm,"  push    rcx\n");
	// fprintf(fp_asm,"\tmov     rcx, men\n");
	// fprintf(fp_asm,"\tmov     rdx, rax\n");
	// fprintf(fp_asm,"\txor     rax, rax\n");
	// fprintf(fp_asm,"\tcall    printf\n");
	// fprintf(fp_asm,"  pop     rcx\n");
	// fprintf(fp_asm,"pop     rax\n");
	// fprintf(fp_asm,"add rsp, 0x28\n");
}

void inputCode(node* head)
{
	node* curr = head->child->Next;
	// fprintf(fp_asm,"\n");
	// fprintf(fp_asm,"sub rsp, 0x28\n");
	// fprintf(fp_asm,"MOV RDX,var+0\n");
	// fprintf(fp_asm,"MOV RCX,inputFormat\n");
	// fprintf(fp_asm,"XOR RAX,RAX\n");
	// fprintf(fp_asm,"call scanf\n");
	// fprintf(fp_asm,"MOV AX,word[var+0]\n");

	if(curr->sym_ptr->type==INTEGER){
		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"push    rax\n");
  		fprintf(fp_asm,"push    rcx\n");
		fprintf(fp_asm,"mov     rdi, input_int\n");
		fprintf(fp_asm,"xor     rax, rax\n");
		fprintf(fp_asm,"call    printf\n");
  		fprintf(fp_asm,"pop     rcx\n");
		fprintf(fp_asm,"pop     rax\n");
		fprintf(fp_asm,"add rsp, 0x28\n");
		fprintf(fp_asm,"\n");
		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"MOV RSI,var+0\n");
		fprintf(fp_asm,"MOV RDI,inputFormat\n");
		fprintf(fp_asm,"XOR RAX,RAX\n");
		fprintf(fp_asm,"call scanf\n");
		fprintf(fp_asm,"MOV AX,word[var+0]\n");
		fprintf(fp_asm,"MOV word[MEM+%d],AX\n",curr->sym_ptr->offset);
		fprintf(fp_asm,"add rsp, 0x28\n");
	}
	else if(curr->sym_ptr->type==ARRAY && curr->sym_ptr->isDynamic != 1){
		fprintf(fp_asm,"xor rax, rax\n");
		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"push    rax\n");
  		fprintf(fp_asm,"push    rcx\n");
		fprintf(fp_asm,"mov eax, %d\n",(curr->sym_ptr->ub-curr->sym_ptr->lb)+1);
		fprintf(fp_asm,"mov rsi, rax\n");
		fprintf(fp_asm,"mov     rdi, input_array_1\n");
		fprintf(fp_asm,"xor     rax, rax\n");
		fprintf(fp_asm,"call    printf\n");
  		fprintf(fp_asm,"pop     rcx\n");
		fprintf(fp_asm,"pop     rax\n");
		fprintf(fp_asm,"add rsp, 0x28\n");
		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"push    rax\n");
  		fprintf(fp_asm,"push    rcx\n");
		fprintf(fp_asm,"mov eax, %d\n",(curr->sym_ptr->lb));
		fprintf(fp_asm,"mov rsi, rax\n");
		fprintf(fp_asm,"mov     rdi, input_array_2\n");
		fprintf(fp_asm,"xor     rax, rax\n");
		fprintf(fp_asm,"call    printf\n");
  		fprintf(fp_asm,"pop     rcx\n");
		fprintf(fp_asm,"pop     rax\n");
		fprintf(fp_asm,"add rsp, 0x28\n");
		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"push    rax\n");
  		fprintf(fp_asm,"push    rcx\n");
		fprintf(fp_asm,"mov eax, %d\n",(curr->sym_ptr->ub));
		fprintf(fp_asm,"mov rsi, rax\n");
		fprintf(fp_asm,"mov     rdi, input_array_3\n");
		fprintf(fp_asm,"xor     rax, rax\n");
		fprintf(fp_asm,"call    printf\n");
  		fprintf(fp_asm,"pop     rcx\n");
		fprintf(fp_asm,"pop     rax\n");
		fprintf(fp_asm,"add rsp, 0x28\n");
		fprintf(fp_asm,"\n");
            int i;
		for(i = curr->sym_ptr->lb;i<=curr->sym_ptr->ub;i++){
			fprintf(fp_asm,"\n");
			fprintf(fp_asm,"sub rsp, 0x28\n");
			fprintf(fp_asm,"MOV RSI,var+0\n");
			fprintf(fp_asm,"MOV RDI,inputFormat\n");
			fprintf(fp_asm,"XOR RAX,RAX\n");
			fprintf(fp_asm,"call scanf\n");
			fprintf(fp_asm,"MOV AX,word[MEM+%d]\n",curr->sym_ptr->offset);
			fprintf(fp_asm,"PUSH AX\n");
			// fprintf(fp_asm,"MOV AX,%d\n",curr->sym_ptr->lb);
			fprintf(fp_asm,"MOV AX,%d\n",i);
			fprintf(fp_asm,"PUSH AX\n");
			fprintf(fp_asm,"MOV AX,word[var+0]\n");
			fprintf(fp_asm,"PUSH AX\n");
			fprintf(fp_asm,"POP BX\n");
			fprintf(fp_asm,"POP AX\n");
			fprintf(fp_asm,"MOV word[arr],%d\n",18+(i-curr->sym_ptr->lb)*2);
			fprintf(fp_asm,"POP AX\n");
			fprintf(fp_asm,"MOV CX,word[arr]\n");
			fprintf(fp_asm,"AND ECX,0x00001111\n");
			fprintf(fp_asm,";MOV word[MEM+%d+ECX],BX\n",curr->sym_ptr->offset);
			fprintf(fp_asm,"MOV word[MEM+%d],BX\n",curr->sym_ptr->offset+(i-curr->sym_ptr->lb)*2);
			fprintf(fp_asm,"add rsp, 0x28\n");
		}
// MOV AX,word[MEM+0]
// PUSH AX
// MOV AX,10
// PUSH AX
// MOV AX,word[var+0]
// PUSH AX
// POP BX
// POP AX
// MOV word[arr],18
// POP AX
// MOV CX,word[arr]
// AND ECX,0x00001111
// MOV word[MEM+0+ECX],BX
	}
	else if(curr->sym_ptr->type==ARRAY && curr->sym_ptr->isDynamic == 1){

		fprintf(fp_asm,"xor rax, rax\n");
		fprintf(fp_asm,"mov ax, word[MEM+%d]\n", curr->sym_ptr->higherBound->offset);
		fprintf(fp_asm,"mov bx, word[MEM+%d]\n", curr->sym_ptr->lowerBound->offset);
		fprintf(fp_asm,"sub ax, bx\n");
		fprintf(fp_asm,"inc ax\n");
		fprintf(fp_asm,"cmp ax,1\n");
		fprintf(fp_asm,"jl error_bound\n");

		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"push    rax\n");
  		fprintf(fp_asm,"push    rcx\n");
		// fprintf(fp_asm,"movsx eax, %d\n",(curr->sym_ptr->ub-curr->sym_ptr->lb)+1);
		fprintf(fp_asm,"mov rsi, rax\n");
		fprintf(fp_asm,"mov     rdi, input_array_1\n");
		fprintf(fp_asm,"xor     rax, rax\n");
		fprintf(fp_asm,"call    printf\n");
  		fprintf(fp_asm,"pop     rcx\n");
		fprintf(fp_asm,"pop     rax\n");
		fprintf(fp_asm,"add rsp, 0x28\n");
		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"push    rax\n");
  		fprintf(fp_asm,"push    rcx\n");
		fprintf(fp_asm,"movsx eax, word[MEM+%d]\n", curr->sym_ptr->lowerBound->offset);
		fprintf(fp_asm,"mov rsi, rax\n");
		fprintf(fp_asm,"mov     rdi, input_array_2\n");
		fprintf(fp_asm,"xor     rax, rax\n");
		fprintf(fp_asm,"call    printf\n");
  		fprintf(fp_asm,"pop     rcx\n");
		fprintf(fp_asm,"pop     rax\n");
		fprintf(fp_asm,"add rsp, 0x28\n");
		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"push    rax\n");
  		fprintf(fp_asm,"push    rcx\n");
		fprintf(fp_asm,"movsx eax, word[MEM+%d]\n", curr->sym_ptr->higherBound->offset);
		fprintf(fp_asm,"mov rsi, rax\n");
		fprintf(fp_asm,"mov     rdi, input_array_3\n");
		fprintf(fp_asm,"xor     rax, rax\n");
		fprintf(fp_asm,"call    printf\n");
  		fprintf(fp_asm,"pop     rcx\n");
		fprintf(fp_asm,"pop     rax\n");
		fprintf(fp_asm,"add rsp, 0x28\n");
		fprintf(fp_asm,"\n");
		fprintf(fp_asm,"\n");
		fprintf(fp_asm,"mov ax, word[MEM+%d]\n", curr->sym_ptr->higherBound->offset);
		fprintf(fp_asm,"mov bx, word[MEM+%d]\n", curr->sym_ptr->lowerBound->offset);
		fprintf(fp_asm,"sub ax, bx\n");
		fprintf(fp_asm,"inc ax\n");
		fprintf(fp_asm,"cmp ax,1\n");
		fprintf(fp_asm,"jl error_bound\n");
		fprintf(fp_asm,"mov dx,ax\n");
		fprintf(fp_asm,"xor ecx,ecx\n");
		fprintf(fp_asm,"mov cx,0\n");
		fprintf(fp_asm,"mov word[arr+0],cx\n");
		fprintf(fp_asm,"scan_dyna_array%d:\n",array_scan);
		fprintf(fp_asm,";push DX\n");
		fprintf(fp_asm,";PUSH CX\n");
		fprintf(fp_asm,"mov bx,dx\n");
		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"MOV RSI,var+0\n");
		fprintf(fp_asm,"MOV RDI,inputFormat\n");
		fprintf(fp_asm,"XOR RAX,RAX\n");
		fprintf(fp_asm,"call scanf\n");
		// fprintf(fp_asm,"MOV AX,word[MEM+%d]\n",curr->sym_ptr->offset);
		// fprintf(fp_asm,"PUSH AX\n");
		// // fprintf(fp_asm,"MOV AX,%d\n",curr->sym_ptr->lb);
		// fprintf(fp_asm,"MOV AX,%d\n",i);
		// fprintf(fp_asm,"PUSH AX\n");
		fprintf(fp_asm,"add rsp, 0x28\n");
		fprintf(fp_asm,"mov dx,bx\n");
		fprintf(fp_asm,"MOV BX,word[var+0]\n");
		// fprintf(fp_asm,"PUSH AX\n");
		// fprintf(fp_asm,"POP BX\n");
		// fprintf(fp_asm,"POP AX\n");
		// fprintf(fp_asm,"MOV word[arr],%d\n",18+(i-curr->sym_ptr->lb)*2);
		// fprintf(fp_asm,"POP AX\n");
		// fprintf(fp_asm,"MOV CX,word[arr]\n");
		// fprintf(fp_asm,"AND ECX,0x00001111\n");
		// fprintf(fp_asm,";MOV word[MEM+%d+ECX],BX\n",curr->sym_ptr->offset);
		fprintf(fp_asm,"XOR ECX,ECX\n");
		fprintf(fp_asm,";POP CX\n");
		fprintf(fp_asm,";POP DX\n");
		fprintf(fp_asm,"mov cx,word[arr+0]\n");
		fprintf(fp_asm,"MOV word[DYNA+%d+ECX],BX\n",curr->sym_ptr->offset*10);
		fprintf(fp_asm,"add cx,2\n");
		fprintf(fp_asm,"mov word[arr+0],cx\n");
		fprintf(fp_asm,"dec dx\n");
		fprintf(fp_asm,"cmp dx,0\n");
		fprintf(fp_asm,"jne scan_dyna_array%d\n",array_scan);
		array_scan++;
	}
	else{
		fprintf(fp_asm,"\n");
		fprintf(fp_asm,"sub rsp, 0x28\n");
		fprintf(fp_asm,"MOV RDX,var+0\n");
		fprintf(fp_asm,"MOV RCX,inputFormat\n");
		fprintf(fp_asm,"XOR RAX,RAX\n");
		fprintf(fp_asm,"call scanf\n");
		fprintf(fp_asm,"MOV AX,word[var+0]\n");
		fprintf(fp_asm,"MOV byte[MEM+%d],AL\n",curr->sym_ptr->offset);
		fprintf(fp_asm,"add rsp, 0x28\n");
	}
	// fprintf(fp_asm,"add rsp, 0x28\n");
}

//DO THIS WHEN NO ERRORS
void codeGeneration(node* head)
{
	if(head==NULL)
		return;

	if(head->check && strcmp(hashed_string[1][head->hash_val],"<assignmentStmt")==0)
	{
		assign_temp.is_array=0;
		assign_temp.value=-1;
		assign_temp.is_dynamic = 0;
		assign_temp.left_temp = NULL;
		assign_temp.offset = -1;
		tchinter = -1;
		assignopCode(head);
		assign_temp.is_array=0;
		assign_temp.value=-1;
		assign_temp.is_dynamic = 0;
		assign_temp.left_temp = NULL;
		assign_temp.offset = -1;
		return;

	}
	else if(head->check && strcmp(hashed_string[1][head->hash_val],"<condionalStmt")==0)
	{
		conditionalopCode(head);
		return;
	}
	else if(head->check && strcmp(hashed_string[1][head->hash_val],"<iterativeStmt")==0)
	{
		loopOpCode(head);
		return;
	}
	else if(head->check && strcmp(hashed_string[1][head->hash_val],"<ioStmt")==0 && strcmp(head->child->ptr->tokenName,"PRINT")==0)
	{
		printCode(head);
		return;
	}
	else if(head->check && strcmp(hashed_string[1][head->hash_val],"<ioStmt")==0 && strcmp(head->child->ptr->tokenName,"GET_VALUE")==0)
	{
		inputCode(head);
		return;
	}

	node *temp = head->child;
	while(temp!=NULL)
	{
		codeGeneration(temp);
		temp = temp->Next;
	}
}

void resetCodeGen()
{
    relopCnt = 0;
    switchCnt = 0;
    switchR = 0;
    forCnt = 0;
    whileCnt = 0;
    indLoc = 0;
}
