#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

struct Symbol_Table *symbolTable_head = NULL;
struct Symbol_Table *symbolTable_tail = NULL;
struct Token *tokenList_head = NULL;
struct Token *tokenList_tail = NULL;
int error = 0;



//void printST()
//{
//	struct Symbol_Table *head = symbolTable_head;
//    while(head!=NULL)
//    {
//        printf("%s %s\n",head->tokenName,head->tokenVal);
//        head = head->Next;
//    }
//}

//reads from filename1, writes uncommented code into f2
void removeComments(char * filename1)
{
	FILE * fp1;
	char c1,c2;
	fp1=fopen(filename1,"r");

	FILE *f2= fopen("withoutcomments.txt","w");

	int flag=0;
	if(fp1==NULL)
		return;

	do {
		c1 = getc(fp1);
		if(c1 == EOF)
			break;
		if(c1 != '*') {
			fprintf(f2,"%c", c1);
		}
		else {
			c2 = getc(fp1);
			if(c2 == EOF)
				break;
			if(c2 == '*') {
                fprintf(f2," ");
				while(1) {
					char c3=getc(fp1);
					if(c3==EOF) {
						flag=1;
						break;
					}
					else if(c3=='*') {
						char c4=getc(fp1);
						if(c4==EOF) {
							flag=1;
							break;
						}
						else if(c4=='*'){
                            fprintf(f2," ");
							break;
						}
						else if(c4 == '\n')
                        {
                            fprintf(f2,"\n");
                        }
					}
                    else if(c3=='\n') fprintf(f2,"\n");
				}
				if(flag==1)
					break;
			}
			else {
                fprintf(f2,"%c", c1);
				fprintf(f2,"%c", c2);
			}
		}
	} while(1);

	fclose(fp1);
	fclose(f2);
}

nodeHM *newNode(char *value, char *token, int hash)
{
	nodeHM *nd;
	nd = (nodeHM *) malloc(sizeof(nodeHM));
	nd->hash = hash;
    strcpy(nd->tokenName, token);
	strcpy(nd->value, value);
	nd->Next=NULL;
	return nd;
}

int hashFunction(char *value)
{
    int key=0, i;
	for(i=0;i<strlen(value);i++) {
		key=key*31 + value[i];
		key=key%NUM_TOKENS;
	}
	return key;
}



struct Symbol_Table* getSymbolTableNode(char* tokenName,char* tokenVal)
{
    struct Symbol_Table* temp = (struct Symbol_Table*)malloc(sizeof(struct Symbol_Table));
    strcpy(temp->tokenName,tokenName);
    if(tokenVal!=NULL)
        strcpy(temp->tokenVal,tokenVal);
    else
        temp->tokenVal[0] = '\0';
    temp->Next = NULL;
    return temp;
}

void insertHM(char *value, char *token)
{
	int hash = hashFunction(value);
	nodeHM *ele, nxt;
	ele = newNode(value, token, hash);

	if(hmot[hash] == NULL)
    {
		hmot[hash] = ele;
	}
	else
	{
	    nodeHM *ptr = hmot[hash];
        while(ptr->Next != NULL)
        {
            ptr = ptr->Next;
        }
        ptr->Next = ele;
	}
}

int setUpStream(char *filename) {

    removeComments(filename);
	initLexor();
	fp = fopen("withoutcomments.txt", "r");

	memset(buf1, 0, BUFF_SIZE);
	memset(buf2, 0, BUFF_SIZE);

	fread(buf1, 1, BUFF_SIZE, fp);

	bufferPointer = 0;
	bufferUsed=0;
	lnm = 1;
	flag = 0;
	return 0;
}


void insertSymbolTable(char* tokenName,char* tokenVal)
{
    struct Symbol_Table* temp = getSymbolTableNode(tokenName,tokenVal);
    if(symbolTable_head==NULL)
    {
        symbolTable_head = temp;
        symbolTable_tail = temp;
    }
    else
    {
        symbolTable_tail->Next = temp;
        symbolTable_tail = temp;
    }
}

Token* searchSymbolTable(char *str,char *str1)
{
	struct Symbol_Table *ST = symbolTable_head;
	while(ST!=NULL)
	{
		if(strcmp(ST->tokenVal,str)==0)
		{
			return makeTokenMan(str,str1,ST);
		}
		ST = ST->Next;
	}

	if(strcmp(str1,"ID")==0 && strlen(str)>20)
	{
		error = 1;
		printf("ERROR_1: Var_Identifier at line %d is longer than the prescribed length\n",lnm);
		return;
	}


	insertSymbolTable(str1,str);

	return makeTokenMan(str,str1,symbolTable_tail);
}

void read_symbolTable()
{
	char str1[100];
	char str2[100];

	FILE *fp = fopen("keywords.txt","r");

    //printf("opening file now \n");
	if(fp!=NULL)
    {
		while(fscanf(fp,"%s %s",str1,str2)==2)
			insertSymbolTable(str2,str1);
	}
	fclose(fp);
	//printf("done with read_symbolTable \n");
}

void resetLexer()
{
    int i,j;
lnm=0; state = 0; lexemeptr=0;
bufferPointer = 0;flag =0;bufferUsed =0;

fp = NULL;

for(i = 0; i<NUM_TOKENS; i++)
    hmot[i] = NULL;

for(i = 0; i<BUFF_SIZE; i++)
{
    buf1[i] = '\0';
    buf2[i] = '\0';
}

for(i = 0; i<200; i++){
    lexeme[i] = '\0';
}


symbolTable_head =NULL;
symbolTable_tail = NULL;
//cntToken = 0;
tokenList_head = NULL;
tokenList_tail = NULL;
error = 0;
}


void initLexor()
{
    char temp[200];

    insertHM("integer","INTEGER");
    insertHM("real",  "REAL");
    insertHM("boolean", "BOOLEAN");
    insertHM("of", "OF");
    insertHM("array", "ARRAY");
    insertHM("start", "START");
    insertHM("end", "END");
    insertHM("declare", "DECLARE");
    insertHM("module", "MODULE");
    insertHM("driver", "DRIVER");
    insertHM("program", "PROGRAM");
    insertHM("Program", "PROGRAM");
    insertHM("record", "RECORD");
    insertHM("tagged", "TAGGED");
    insertHM("union", "UNION");
    insertHM("get_value", "GET_VALUE");
    insertHM("print", "PRINT");
    insertHM("use", "USE");
    insertHM("with", "WITH");
    insertHM("parameters", "PARAMETERS");
    insertHM("true", "TRUE");
    insertHM("false", "FALSE");
    insertHM("TRUE", "TRUE");
    insertHM("FALSE", "FALSE");
    insertHM("case", "CASE");
    insertHM("takes", "TAKES");
    insertHM("input", "INPUT");
    insertHM("returns", "RETURNS");
    insertHM("AND", "AND");
    insertHM("OR", "OR");
    insertHM("for", "FOR");
    insertHM("in", "IN");
    insertHM("switch", "SWITCH");
    insertHM("if", "IF");
    insertHM("break", "BREAK");
    insertHM("default", "DEFAULT");
    insertHM("while", "WHILE");
}

Token *makeTokenMan(char *value, char *token,struct Symbol_Table* tokenVal)
{
    Token *tk;
	tk = (Token *) malloc(sizeof(Token));

	tk->rowNum = lnm;
    strcpy(tk->value, value);
	strcpy(tk->tokenName, token);
	tk->tokenVal=tokenVal;
	tk->Next=NULL;

    memset(lexeme, 0, sizeof(lexeme));
    lexemeptr = 0;
    state=0;

    if(tokenList_head==NULL)
    {
        tokenList_head = tk;
        tokenList_tail = tk;
    }
    else
    {
        tokenList_tail->Next = tk;
        tokenList_tail = tk;
    }

    return tk;
}

char readBuffer() {
	int readC;

	if(flag==0) {
		char ch=buf1[bufferPointer];
		bufferPointer++;
		if(bufferPointer==BUFF_SIZE) {
			flag=1;
			bufferPointer=0;

			if(bufferUsed==0) {
				memset(buf2, 0, BUFF_SIZE);
				fread(buf2, 1, BUFF_SIZE, fp);
				bufferUsed=1;
			}
		}
		return ch;
	}
	else {
		char ch = buf2[bufferPointer];
		bufferPointer++;
		if(bufferPointer==BUFF_SIZE) {
			flag=0;
			bufferPointer=0;

			if(bufferUsed==1) {
				memset(buf1, 0, BUFF_SIZE);
				fread(buf1, 1, BUFF_SIZE, fp);
				bufferUsed=0;
			}
		}
		return ch;
	}
}

void modifybufferPointer() {

    if(flag==0 && bufferPointer==0) {
		bufferPointer=BUFF_SIZE-1;
		flag=1;
    }
	else if(flag==1 && bufferPointer==0) {
		bufferPointer=BUFF_SIZE-1;
		flag=0;
	}
	else bufferPointer--;
}


char *search(char *value,char *temp)
{

	int key = hashFunction(value);
	nodeHM *nd = hmot[key];

	while(nd!=NULL) {
		if(strcmp(nd->value, value) == 0){
            strcpy(temp,nd->tokenName);
			return temp;
		}
		nd=nd->Next;
	}
	return NULL;
}

void modifyLexeme(){
    lexeme[--lexemeptr]='\0';

	while(lexeme[lexemeptr]==' ' || lexeme[lexemeptr]=='\t' || lexeme[lexemeptr]=='\n'|| lexeme[lexemeptr]=='\r') lexeme[lexemeptr--]='\0';
    int j=0;
    while(lexeme[j]==' ' || lexeme[j]=='\t' || lexeme[j]=='\n'|| lexeme[j]=='\r') j++;
    int i=0;
    while(lexeme[j]) lexeme[i++]=lexeme[j++];
    lexeme[i]='\0';
}

void modifyBufferForError()
{
    if(flag == 0 && bufferPointer == BUFF_SIZE-1)
    {
        flag = 1;
        bufferPointer = 0;
    }
    else if(flag == 1 && bufferPointer == BUFF_SIZE-1)
    {
        flag = 0;
        bufferPointer = 0;
    }
    else
    {
        bufferPointer++;
    }
}

Token *retraceToPrevState(int st) {

	modifybufferPointer();
    modifyLexeme();

	if(st == 1) {

        char *temp = (char *)malloc(sizeof(char)*200);
        temp=search(lexeme,temp);

		if(temp)
        {
            return searchSymbolTable(lexeme,temp);
        }
		else
            return searchSymbolTable(lexeme,"ID");

	}
	else if(st == 38) {
		return makeTokenMan(":","COLON",NULL);
	}
	else if(st == 8) {
		return searchSymbolTable(lexeme,"RNUM");
	}
	else if(st == 13){
        return makeTokenMan("*", "MUL",NULL);
	}
	else if(st == 3) {
		return searchSymbolTable(lexeme,"NUM");
	}
	else if(st == 15) {
		return makeTokenMan(lexeme,"LT",NULL);
	}
	else if(st == 18) {
		return makeTokenMan(lexeme,"DEF",NULL);
	}
	else if(st == 21) {
		return makeTokenMan(lexeme,"GT",NULL);
	}
	else if(st == 24) {
		return makeTokenMan(lexeme,"ENDDEF",NULL);
	}
	else if(st==5 || st==4){
        return searchSymbolTable(lexeme,"RNUM");
	}
	else
	{

	    modifybufferPointer();
	    char c;
        if(flag == 0)
            c = buf1[bufferPointer];
        else
            c = buf1[bufferPointer];

        printf("\nLine %d: Lexical Error because of symbol - '%c'\n", lnm,  c);
        modifyBufferForError();

        state=0;

        Token *errortoken = (Token *)malloc(sizeof(Token));
        errortoken->rowNum = lnm;
        errortoken->tokenVal=NULL;
        errortoken->Next = NULL;
        strcpy(errortoken->tokenName, "ERROR");
        strcpy(errortoken->value, "");
        return errortoken;
	}

}

void printhmot()
{
    int i;
    for(i = 0; i<NUM_TOKENS; i++)
    {
        if(hmot[i]!=NULL)
        {
            nodeHM *ptr = hmot[i];
            while(ptr!=NULL)
            {
                printf("%s ", ptr->tokenName);
                ptr = ptr->Next;
            }
        }
        printf("\n");
    }
}

Token *getNextToken()
{
    lexemeptr = 0;
    memset(lexeme, 0, sizeof(lexeme));
    int insidecom = 0;
    int cntId = 0;
    while(1)
    {
        char c = readBuffer();
        if(insidecom == 0)
        {
            lexeme[lexemeptr++] = c;
            if(c == '\0')
            {
                if(state == 0)
                {
                    //printf("TOKENIZING OVER \n");
                    return NULL;
                }
                else
                {
                    return retraceToPrevState(state);
                }
            }
            else
            {
                if(c == 10 || c == 9 || c == 32 || c == '\r')
                {
                    if(state == 0)
                    {
                        if(c == 10)
                        {
                            lnm++;
                        }
                        continue;
                    }
                    else
                    {
                        return retraceToPrevState(state);
                    }
                    lexemeptr--;
                }
                else if(c== '+')
                {
                    if(state == 0)
                    {
                        state = 11;
                        return makeTokenMan("+", "PLUS",NULL);
                    }
                    else if(state == 6)
                    {
                        state = 7;
                    }
                    else
                    {
                        return retraceToPrevState(state);
                    }
                }
                else if(c== '-')
                {
                    if(state == 0)
                    {
                        state = 12;
                        return makeTokenMan("-", "MINUS",NULL);
                    }
                    else if(state == 6)
                    {
                        state = 7;
                    }
                    else
                    {
                       return retraceToPrevState(state);
                    }
                }
                else if(c== '*')
                {
                    if(state == 0)
                    {
                        state = 13;
                    }
                    else if(state == 13)
                    {
                        state = 43;
                        insidecom = 1;
                    }
                    else return retraceToPrevState(state);
                }
                else if(c== '/')
                {
                    if(state == 0)
                    {
                        state = 13;
                        return makeTokenMan("/", "DIV",NULL);
                    }
                    else
                    {
                        return retraceToPrevState(state);
                    }
                }
                else if(c== '<')
                {
                    if(state == 0)
                    {
                        state = 15;
                    }
                    else if(state == 15)
                    {
                        state = 18;
                    }
                    else if(state == 18){
                        state = 19;
                        return makeTokenMan("<<<", "DRIVERDEF",NULL);
                    }
                    else
                        return retraceToPrevState(state);
                }
                else if(c== '>')
                {
                    if(state == 0)
                    {
                        state = 21;
                    }
                    else if(state == 21)
                    {
                        state = 24;
                    }
                    else if(state == 24){
                        state = 25;
                        return makeTokenMan(">>>", "DRIVERENDDEF",NULL);
                    }
                    else
                        return retraceToPrevState(state);
                }
                else if(c== '=')
                {
                    if(state == 0)
                    {
                        state = 27;
                    }
                    else if(state == 27)
                    {
                        state = 29;
                        return makeTokenMan("==", "EQ",NULL);
                    }
                    else if(state == 15)
                    {
                        state = 17;
                        return makeTokenMan("<=", "LE",NULL);
                    }
                    else if(state == 21)
                    {
                        state = 23;
                        return makeTokenMan(">=", "GE",NULL);
                    }
                    else if(state == 38)
                    {
                        state = 40;
                        return makeTokenMan(":=", "ASSIGNOP",NULL);
                    }
                    else if(state == 41)
                    {
                        state = 42;
                        return makeTokenMan("!=", "NE",NULL);
                    }
                    else
                    {
                        return retraceToPrevState(state);
                    }
                }

                else if(c== '(')
                {
                    if(state == 0)
                    {
                        state = 31;
                        return makeTokenMan("(", "BO",NULL);
                    }
                    else
                    {
                        return retraceToPrevState(state);
                    }
                }
                else if(c== ')')
                {
                    if(state == 0)
                    {
                        state = 30;
                        return makeTokenMan(")", "BC",NULL);
                    }
                    else
                    {
                    return retraceToPrevState(state);
                    }
                }
                else if(c== '[')
                {
                    if(state == 0) {
                    state = 33;
                    return makeTokenMan("[", "SQBO",NULL);
                    }
                    else
                    {
                        return retraceToPrevState(state);
                    }
                }
                else if(c== ']')
                {
                    if(state == 0) {
                    state = 32;
                    return makeTokenMan("]", "SQBC",NULL);
                    }
                    else {
                        return retraceToPrevState(state);
                    }
                }
                else if(c== '.')
                {
                    if(state == 0) {
                        state = 34;
                    }
                    else if(state == 34) {
                        state = 35;
                        return makeTokenMan("..", "RANGEOP",NULL);
                    }
                    else if(state == 3) {
                    state = 4;
                    }
                    else if(state == 4) {
                        state = 3;
                        modifybufferPointer();
                        lexeme[--lexemeptr]=='\0';
                        return retraceToPrevState(state);
                    }
                    else {
                        return retraceToPrevState(state);
                    }
                }
                else if(c== ',')
                {
                    if(state == 0) {
                        state = 36;
                        return makeTokenMan(",", "COMMA",NULL);
                    }
                    else {
                        return retraceToPrevState(state);
                    }
                }
                else if(c== ';')
                {
                    if(state == 0) {
                        state = 37;
                        return makeTokenMan(";", "SEMICOL",NULL);
                    }
                    else {
                        return retraceToPrevState(state);
                    }
                }
                else if(c==':')
                {
                    if(state == 0) {
                        state = 38;
                    }
                    else {
                        return retraceToPrevState(state);
                    }
                }
                else if(c== '!')
                {
                    if(state == 0) {
                        state = 41;
                    }
                    else {
                        return retraceToPrevState(state);
                    }
                }
                else if((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
                {

                    if(state == 0) {
                        state = 1;
                        cntId++;
                    }
                    else if(state == 1) {
                        state = 1;
                        cntId++;
                        if(cntId > 20)
                        {
                            cntId = 0;
                            printf("Length of current token exceeded 20 characters...breaking to maximum 20 and converting the rest of the input stream to another id\n");
                            return retraceToPrevState(1);
                        }
                    }
                    else if(state == 5 && (c == 'e' || c == 'E')) {
                        state = 6;
                    }
                    else {
                        return retraceToPrevState(state);
                    }
                }
                else if(c >= 48 && c <= 57)
                {

                    if(state == 0) {
                        state = 3;
                    }
                    else if(state == 1 || state == 3 || state == 5 || state == 8) {

                            if(state == 1)
                            {
                                cntId++;
                                if(cntId > 20)
                                {
                                    cntId = 0;
                                    printf("\nLine %d: Length of current token exceeded 20 characters...breaking to maximum 20 and converting the rest of the input stream to another id\n", lnm);
                                    return retraceToPrevState(1);
                                }
                            }

                    }
                    else if(state == 4) {
                        state = 5;
                    }
                    else if(state == 6 || state == 7) {
                        state = 8;
                    }
                    else {
                        return retraceToPrevState(state);
                    }
                }
                else if(c== '_')
                {

                    if(state == 1)
                    {
                        state = 1;
                    }
                    else {
                        return retraceToPrevState(state);
                    }
                }
                else
                {
                    modifybufferPointer();
                    char c;
                    if(flag == 0)
                        c = buf1[bufferPointer];
                    else
                        c = buf1[bufferPointer];

                    printf("\nLine %d: Lexical Error because of symbol - '%c'\n", lnm,  c);
                    modifyBufferForError();
                    state=0;

                    Token *errortoken = (Token *)malloc(sizeof(Token));
                    errortoken->rowNum = lnm;
                    errortoken->Next = NULL;
                    errortoken->tokenVal=NULL;
                    strcpy(errortoken->tokenName, "ERROR");
                    strcpy(errortoken->value, "");
                    return errortoken;
                }
                }
            }
            else
            {
                char c = readBuffer();
                if(c == '*')
                {
                    if(state == 43)
                    {
                        char p = readBuffer();
                        if(p==EOF) break;
                        if(p == '*')
                        {
                            state = 0;
                            insidecom = 0;
                        }
                        else
                        {

                        }
                    }
                }
            }
        }
}

void printTokenStream()
{
    Token *curr = getNextToken();
    printf("\n");

    while(curr)
    {
        if(strcmp(curr->tokenName, "ERROR") != 0)
            printf("%s\t \t%s\n", curr->value, curr->tokenName);
        curr = getNextToken();
    }

    printf("\n");
    return;
}

void getTokens()
{
    Token *curr=getNextToken();
    while(curr) curr=getNextToken();
	/*struct Token *head = tokenList_head;
    while(head!=NULL)
    {
        printf("%s\t",head->tokenName);
        head->tokenVal==NULL ? print_lexeme(head->tokenName) : printf("%s\t",(head->tokenVal)->tokenVal);printf("%d\n",head->rowNum);
        head = head->Next;
    }*/
}

