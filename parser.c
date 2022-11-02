#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#define MOD 1009
#define START 263
#define DOLLAR 36
#define epsilon 101

ssize_t getline(char **buf, size_t *bufsiz, FILE *fp)
{
	char *ptr, *eptr;
    int delimiter='\n';

	if (*buf == NULL || *bufsiz == 0) {
		*bufsiz = BUFSIZ;
		if ((*buf = malloc(*bufsiz)) == NULL)
			return -1;
	}

	for (ptr = *buf, eptr = *buf + *bufsiz;;) {
		int c = fgetc(fp);
		if (c == -1) {
			if (feof(fp))
				return ptr == *buf ? -1 : ptr - *buf;
			else
				return -1;
		}
		*ptr++ = c;
		if (c == delimiter) {
			*ptr = '\0';
			return ptr - *buf;
		}
		if (ptr + 2 >= eptr) {
			char *nbuf;
			size_t nbufsiz = *bufsiz * 2;
			ssize_t d = ptr - *buf;
			if ((nbuf = realloc(*buf, nbufsiz)) == NULL)
				return -1;
			*buf = nbuf;
			*bufsiz = nbufsiz;
			eptr = nbuf + nbufsiz;
			ptr = nbuf + d;
		}
	}
}


node* parseTree_head = NULL;

node* getParseTreeNode(int hash_val,int check,node* par)
{
	node* temp = (node*)malloc(sizeof(node));
	temp->hash_val = hash_val;
	temp->check = check;
	temp->par = par;
	temp->child  = NULL;
	temp->Next = NULL;
	temp->ptr = NULL;
	temp->sym_ptr = NULL;
	return temp;
}

void push_stk(node* parent,int rule_idx)
{
	int first=0;
	node* prev = NULL;
	node* arr[15];
	construct* trav = G[rule_idx]->first->Next;
	int arrptr = 0;

	if(trav->hash_val==epsilon)
	{
		parent->child = getParseTreeNode(trav->hash_val,trav->check,parent);
		parent->child->ptr = (struct Token*)malloc(sizeof(struct Token));
		strcpy(parent->child->ptr->tokenName,"epsilon");
		(parent->child)->par = parent;
	}

	while(trav!=NULL && trav->hash_val!=epsilon)
	{
		arr[arrptr] = getParseTreeNode(trav->hash_val,trav->check,parent);
		if(first==0)
		{
			parent->child = arr[arrptr];
			first = 1;
		}
		if(prev!=NULL)
			prev->Next = arr[arrptr];
		prev = arr[arrptr];
		arrptr++;
		trav = trav->Next;
	}
	arrptr--;
	while(arrptr > -1)
	{
		mystack[stktop] = arr[arrptr];
		arrptr--;
		stktop++;
	}
}

int append_fir(int i,int j)// tell if added something new and add it
{
	int bit=final_term_hash[j];
	if(bit==check_epsilon) hasepsilon[i]=1;
	if(ff[0][i] & (1ll<<bit) ) return 0;
	//printf("%s %s\n",hashed_string[1][i],hashed_string[0][j]);
	ff[0][i]|=(1ll<<bit);
	return 1;
}

void find_first()
{
	int change = 0;	int i;int k;
	memset(ff[0],0,sizeof ff[0]);
	do
	{
		change = 0;
		for(i = 0;i<=rule_no;i++)
		{
			construct *cur,*prev;
            cur = prev = G[i]->first;
			construct *trav = cur->Next;
			do
			{
				if(trav->check == 0)            //if terminal...directly add
				{
					if(append_fir(cur->hash_val,trav->hash_val)==1)
                        change=1;
					break;
				}
				int tmp = trav->hash_val;
				unsigned long long int thing = ff[0][tmp];
				int k;
				//printf("5\n");
				for(k = 0; k<num_term ;k++)
				{
				    if( ( thing & (1ll<<k)) == 0 ) continue;
					if(k == check_epsilon)
						continue;
					if(append_fir(cur->hash_val,vapas_hash[k])==1)
					change =1;
				}
				if(hasepsilon[trav->hash_val])
				{
					prev = trav;
					trav = trav->Next;
				}
				else
					break;
			}while(trav!=NULL);
			if(trav == NULL && hasepsilon[prev->hash_val] == 1)
				append_fir(cur->hash_val,101);
		}
	}while(change!=0);
}

int append_fol(int i,int j)// tell if added something new and add it
{
	int bit=final_term_hash[j];
	if(ff[1][i] & (1ll<<bit) ) return 0;
	//printf("%s %s\n",hashed_string[1][i],hashed_string[0][j]);
	ff[1][i] |= (1ll<<bit);
	return 1;
}

void find_follow()
{
	int i,change,k;
	memset(ff[1],0,sizeof ff[1]);
	append_fol(START,DOLLAR);//
    //printf("cjkdhkofhro");
	do
	{
		change = 0;
		for (i = 0; i <= rule_no; ++i)
		{
			construct *cur = G[i]->first;
			construct *trav = cur->Next;
			while(trav!=NULL)
			{
				if(trav->check == 0)            //terminal...keep going
				{
					trav = trav->Next;
				}
				else
				{
					construct* tNext = trav->Next;          //go to the last nonterminal...A->BC
					while(1)
					{
						if(tNext == NULL)                    //cur is A lhs
						{
						    unsigned long long int thing = ff[1][cur->hash_val];
						    int k;
                            for(k=0; k<num_term; k++)
                            {
                                        if( ( thing & (1ll<<k)) == 0 ) continue;
                                        if(append_fol(trav->hash_val,vapas_hash[k])==1)
                                            change=1;
                            }
                            break;
						}
						else if(tNext->check==0)
						{
							if(append_fol(trav->hash_val,tNext->hash_val)==1)
                                change=1;
							break;
						}
						else
						{
							int got_epsilon = 0;
							unsigned long long int thing = ff[0][tNext->hash_val];
							int k;
							for(k = 0;k<num_term;k++)
							{
							    if( ( thing & (1ll<<k)) == 0 ) continue;
								if(k!=check_epsilon){
                                     if(append_fol(trav->hash_val,vapas_hash[k])==1)
                                        change=1;}
								else
                                    got_epsilon = 1;
							}
							if(!got_epsilon)
								break;
						}
						tNext = tNext->Next;
					 }
					 trav = trav->Next;
				}
			}
		}
	}while(change!=0);
}

int generate_hash_value(int nonterm, char* in)
{
	int init = 0;
	int l = strlen(in);
	int i;

	if (nonterm)
		{
		for(i=1;i < l ;i++)
	{
	//init=hashfn(in);
		init = (init*97+in[i])%MOD;
	}
		}
	else
		{
		for(i=0;i < l ;i++)
	{
	    	//init=hashfn(in);

		init = (init*97+in[i])%MOD;
	}
		}
		int ret = -10;
	int add = 1;
	int iter = 0;
	int h=init;
	while(1)
	{
		if(nonterm)
		{
			iter++;
			if(iter == 10000)
				return -1;
			if(hashed_string[nonterm][init][0] == '\0')
			{
				strcpy(hashed_string[nonterm][init], in);
				ret = init;
				break;
			}
			else
			if(strcmp(hashed_string[nonterm][init],in) == 0)
			{
				ret = init;
				break;
			}
			else
			{
				init=(init+add*add)%MOD;
				add++;
			}
		}
		else
		{
			iter++;
			if(iter == 10000)
				return -1;// error hai;
			if(hashed_string[nonterm][init][0] == '\0')
			{
				strcpy(hashed_string[nonterm][init], in);
				ret = init;
				break;
			}
			else
			if(strcmp(hashed_string[nonterm][init],in) == 0)
			{
				ret = init;
				break;
			}
			else
			{
				init=(init+add*add)%MOD;
				add++;
			}
		}
    }
    if(!nonterm) {
            //if(ret==101) check_epsilon=num_term;
            //printf("%d %d %s\n",h,ret,in);
            //printf("%s %d\n",hashed_string[0][ret],num_term);
            if(final_term_hash[ret]!=-1) return ret;

            if(ret==101) check_epsilon=num_term;
            //printf("%d %d %s\n",h,ret,in);
            //printf("%s %d\n",hashed_string[0][ret],num_term);
            final_term_hash[ret]=num_term;
            vapas_hash[num_term]=ret;
            num_term++;
    }
	return ret;
}

void insertConstruct(int pos,int nonterm, int hashval)
{
	construct* newconstruct = (construct* )malloc(sizeof(construct));
	newconstruct->check = nonterm;
	newconstruct->hash_val = hashval;
	newconstruct->Next = NULL;
	if(G[pos]->first == NULL)
	{
		G[pos]->first = newconstruct;
		G[pos]->last = newconstruct;


	}
	else
	{
		G[pos]->last->Next = newconstruct;
		G[pos]->last = G[pos]->last->Next;
	}
}

void ComputeFirstAndFollowSets()
{
	find_first();
	find_follow();
}

void createParseTable()
{
	memset(parse_table,-1,sizeof parse_table);
	ComputeFirstAndFollowSets();
	int i,j;
	for(i = 0; i<= rule_no; i++)    // iterate over rules in linked list
	{
		int lhs = G[i]->first->hash_val;
	    construct* trav = G[i]->first->Next;
	    int is_eps = 0;
	    while(trav!=NULL)
	    {
	    	if(trav->hash_val == epsilon)
	    	{
	    		is_eps = 1;
	    		break;
	    	}
	    	else if(trav->check == 0)
	    	{
	    		parse_table[lhs][final_term_hash[trav->hash_val]] = i;
	    		break;
	    	}
	    	else
	    	{
	    		int is_eps2 = 0;
	    		unsigned long long int thing = ff[0][trav->hash_val];
	    		int k;
	    		for(k=0; k<num_term; k++)
	    		{
	    		    if( ( thing & (1ll<<k)) == 0 ) continue;
	    			if( k == check_epsilon)
	    			{
	    				is_eps2 = 1;
	    			}
	    			else
	    			{
	    				parse_table[lhs][k] = i;
	    			}
	    		}
	    		if(is_eps2 == 0)
	    			break;
	    	}
	    	trav = trav->Next;
	    }
	    if(trav == NULL)
	    	is_eps = 1;
	    if(is_eps)
	    {
	        unsigned long long int thing = ff[1][lhs];
	        int k;
	    	for(k=0; k<num_term; k++)
	    		{
	    		    if( ( thing & (1ll<<k)) == 0 ) continue;
                    parse_table[lhs][k] = i;
	    	}
	    }
	}
    /*
	int fg[1010];
	for(i=0;i<1010;i++)
		fg[i] = 0;
	for(i = 0; i<= rule_no; i++)
	{
		int lhs = G[i]->first->hash_val;
		if(fg[lhs]==1)
			continue;
		fg[lhs] = 1;
		for(j=0;j<1010;j++)
			if(parse_table[lhs][j]!=-1)
				{
				    printf("%s %s %d ",hashed_string[1][lhs],hashed_string[0][j],i);
				    construct *temp=G[i]->first;
				    while(temp) {
                            if(temp->check) printf("%s ",hashed_string[1][temp->hash_val]);
                            else printf("%s ",hashed_string[0][temp->hash_val]);
                            temp=temp->Next;
				    }
				    printf("\n");
                }
	}
    */
}

void print_lexeme(char* str)
{
	if(strcmp(str,"MUL")==0)
		printf("*\t");
	else if(strcmp(str,"GE")==0)
		printf(">=\t");
	else if(strcmp(str,"ENDDEF")==0)
		printf(">>\t");
	else if(strcmp(str,"GT")==0)
		printf(">\t");
	else if(strcmp(str,"DRIVERENDDEF")==0)
		printf(">>>\t");
	else if(strcmp(str,"LE")==0)
		printf("<=\t");
	else if(strcmp(str,"DEF")==0)
		printf("<<\t");
	else if(strcmp(str,"LT")==0)
		printf("<\t");
	else if(strcmp(str,"DRIVERDEF")==0)
		printf("<<<\t");
	else if(strcmp(str,"DIV")==0)
		printf("/\t");
	else if(strcmp(str,"MINUS")==0)
		printf("-\t");
	else if(strcmp(str,"PLUS")==0)
		printf("+\t");
	else if(strcmp(str,"EQ")==0)
		printf("==\t");
	else if(strcmp(str,"NE")==0)
		printf("!=\t");
	else if(strcmp(str,"ASSIGNOP")==0)
		printf(":=\t");
	else if(strcmp(str,"COLON")==0)
		printf(":\t");
	else if(strcmp(str,"RANGEOP")==0)
		printf("..\t");
	else if(strcmp(str,"SEMICOL")==0)
		printf(";\t");
	else if(strcmp(str,"COMMA")==0)
		printf(",\t");
	else if(strcmp(str,"SQBO")==0)
		printf("[\t");
	else if(strcmp(str,"SQBC")==0)
		printf("]\t");
	else if(strcmp(str,"BO")==0)
		printf("(\t");
	else if(strcmp(str,"BC")==0)
		printf(")\t");
	else
		printf("%s\t",str);
}

void raise_error(struct Token* token_ptr, char expected[])
{
    printf("\nLine %d: Syntactic error between the token %s for lexeme ", token_ptr->rowNum, token_ptr->tokenName);
    if(token_ptr->value != NULL)
        printf("%s ",token_ptr->value);
    else
        print_lexeme(token_ptr->tokenName);
    printf("and expected token %s(at top of stack)\n", expected);
    token_ptr = token_ptr->Next;
}

Token *detectError(int hval, Token *token_ptr)
{
    unsigned long long int thing = ff[1][hval];
    int expFoll=0,i;
    for(i=0;i<num_term;i++){
        if(thing & (1ll<<i)) break;
    }
    expFoll=i;
    while(token_ptr != NULL)
    {

        int bit=final_term_hash[generate_hash_value(0,token_ptr->tokenName)];
        if(thing & (1ll<<bit)) {
                stktop--;
                return token_ptr;
        }
        char expectedtoken[50];
        strcpy(expectedtoken,hashed_string[0][vapas_hash[expFoll]]);
        printf("\nLine %d: Syntactic error between token %s and expected token %s \n", token_ptr->rowNum, token_ptr->tokenName, expectedtoken);
        printf("The compiler keeps reading NEW TOKENS (from input stream) till we reach a token that is PRESENT in follow of current stack top NONTERMINAL\n");
        token_ptr=getNextToken();
        while(strcmp(token_ptr->tokenName,"ERROR") == 0)
        {
            token_ptr = getNextToken();
        }
    }
    return token_ptr;
}
/*
void parseInputSourceCode()
{
	int local_mark[1010];// error resolution
	int i;
	memset(local_mark,0,sizeof local_mark);
	struct Token* temp_dollar = (struct Token*)malloc(sizeof(struct Token));
	strcpy(temp_dollar->tokenName,"$");
	temp_dollar->tokenVal = NULL;
	temp_dollar->rowNum = -1;
	temp_dollar->Next = NULL;
	tokenList_tail->Next = temp_dollar;

	stktop = 0;
    parseTree_head = getParseTreeNode(START,1,NULL);
	mystack[stktop] = parseTree_head;
	stktop++;
	node* store_head;

	struct Token* token_ptr = tokenList_head;
	while(stktop!=0 && token_ptr!=NULL)
	{
		int hval = mystack[stktop-1]->hash_val;
		if (mystack[stktop-1]->check == 0)
		{
			if(strcmp(hashed_string[0][hval],token_ptr->tokenName) == 0)
			{
				mystack[stktop-1]->ptr = token_ptr;
				stktop--;
				token_ptr = token_ptr->Next;
			}
			else
			{
				error = 1;
				printf("ERROR_5: The token %s for lexeme ",token_ptr->tokenName);
				if(token_ptr->tokenVal == NULL)
					print_lexeme(token_ptr->tokenName);
				else
					printf("%s ",(token_ptr->tokenVal)->tokenVal);
				printf("does not match at line %d. The expected token here is %s\n",token_ptr->rowNum,hashed_string[0][hval]);
				stktop--;
			}
		}
		else
		{
			int hash_of_name = generate_hash_value(0,token_ptr->tokenName);
			int rule_idx = parse_table[hval][final_term_hash[hash_of_name]];
			if (rule_idx == -1)
			{
				error = 1;
				char expected[40];
				int iter=0;
				for(;iter<64;iter++) if(ff[0][hval]&(1ll<<iter)) break;
				strcpy(expected,hashed_string[0][vapas_hash[iter]]);
				while(token_ptr!=NULL && parse_table[hval][final_term_hash[generate_hash_value(0,token_ptr->tokenName)]] == -1)
				{
					if(strcmp(token_ptr->tokenName,"$")==0)
					{
						token_ptr = token_ptr->Next;
						continue;
					}
					printf("ERROR_5: The token %s for lexeme ",token_ptr->tokenName);
					if(token_ptr->tokenVal == NULL)
						print_lexeme(token_ptr->tokenName);
					else
						printf("%s ",(token_ptr->tokenVal)->tokenVal);
					printf("does not match at line %d. The expected token here is %s\n",token_ptr->rowNum,expected);
					token_ptr = token_ptr->Next;
				}
				continue;
			}// skip over linked list tilL ele in first or follow on nonterm
			else
			{
				store_head = mystack[stktop-1];
				stktop--;
				push_stk(store_head,rule_idx);
			}
		}
	}
	while(stktop!=0)
		stktop--;

	tokenList_tail->Next = NULL;
	free(temp_dollar);
}*/

void resetParser()
{
    int i, j;
    for(i = 0; i<1025;i++)
    {
        hasepsilon[i] = 0;
        final_term_hash[i] = 0;
        for(j = 0; j<64; j++)
        {
            parse_table[i][j] = 0;
        }
    }

    rule_no = 0;
    num_term = 0;
    check_epsilon = 0;
    nodes_par = 0;
    nodes_ast = 0;


    //used for function scoping
    for(i = 0; i<MAXSZ; i++)
        mystack[i] = NULL;


    stktop = 0;
    parseTree_head = NULL;

    for(i = 0; i<MAXRULES; i++)
        G[i] = NULL;

    //0 is terminal
    //1 is non terminal
    for(i = 0; i<2; i++)
    {
        for(j = 0; j<1025; j++)
        {
            hashed_string[i][j][0] = '\0';
        }
    }

    for(i = 0; i<64; i++)
        vapas_hash[i] = 0;

    for(i = 0; i<2; i++)
    {
        for(j = 0; j<1025; j++)
        {
            ff[i][j] = 0;
        }
    }
}

void parseInputSourceCode()
{
	int local_mark[1010];
	int i;
	memset(local_mark,0,sizeof local_mark);

	stktop = 0;
    parseTree_head = getParseTreeNode(START,1,NULL);
	mystack[stktop] = parseTree_head;
	stktop++;
	node* store_head;

	//cntToken++;
	struct Token* token_ptr = getNextToken();

	while(1)
	{
        if(stktop == 0 || token_ptr == NULL)
            break;

        while(strcmp(token_ptr->tokenName,"ERROR") == 0)
        {
            token_ptr = getNextToken();
        }

		int hval = mystack[stktop-1]->hash_val;
        int c=	mystack[stktop-1]->check ;
        switch(c)
        {
            case(1):
            {
                int hash_of_name = generate_hash_value(0,token_ptr->tokenName);
                int rule_idx = parse_table[hval][final_term_hash[hash_of_name]];

                if (rule_idx != -1)
                {
                    store_head = mystack[stktop-1];
                    stktop--;
                    push_stk(store_head,rule_idx);
                }
                else
                {
                    token_ptr=detectError(hval, token_ptr);
                }
            }
            break;
            case(0):
            {
                if(strcmp(hashed_string[0][hval],token_ptr->tokenName) == 0)
                {
                    mystack[stktop-1]->ptr = token_ptr;
                    stktop--;
                    //cntToken++;
                    token_ptr = getNextToken();
                }
                else
                {
                    raise_error(token_ptr,hashed_string[0][hval]);
                    stktop--;
                }
            }
            break;
        }

	}
	while(stktop!=0)
		stktop--;
}

void printParseTree(node* head)
{
	if(head==NULL)
		return;

	printParseTree(head->child);
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

	//PRINTING SIBLINGS
	if(head->child!=NULL)
	{
		node* siblings = (head->child)->Next;
		while(siblings!=NULL)
		{
			printParseTree(siblings);
			siblings = siblings->Next;
		}
	}
}


int count_r=0;

void insert_grammar(char *gram_temp, int rule_no){

    if (count_r!=rule_no){
        count_r = rule_no;
    }
    //printf("%s",gram_temp);


    char unit[50];
    int l = strlen(gram_temp) , i=0;
    int nonterm = 1;
    for(i =0; i<l;i++)unit[i] = gram_temp[i];
    if(unit[l-1] != '>')
        nonterm = 0;
    else
        unit[l-1] = '\0';
    unit[l] = '\0';
    //int initval = get_hash_value(nonterm,unit);
    //printf("%d - %s\n", rule_no, unit);
    int hashval =  generate_hash_value(nonterm , unit);
    if(hashval == -1)
    {
        puts("BAD HASH!");
        return;
    }
    insertConstruct(rule_no,nonterm,hashval);
}

void Read_Grammar()
{
    FILE* fp = fopen("MyGrammar.txt","r");

    memset(hashed_string,0,sizeof hashed_string);
    memset(final_term_hash,-1,sizeof final_term_hash);
    memset(vapas_hash,-1,sizeof vapas_hash);

    int i=0;

    while(i<MAXRULES)
    {
            G[i]=(rule *)malloc(sizeof(rule));
            G[i]->first=NULL;
            G[i]->last=NULL;
            i++;
    }

    char *line = NULL;
    size_t len = 0;
    rule_no = -1;
    int j;
    char gram_temp[50] = {'\0'};
    while(getline(&line,&len,fp)!=-1)
    {
        int l = strlen(line);
        char nt[50];
        char initial[50];
        int first = 1;
        memset(nt,'\0',sizeof nt);
        int ptr = 0;
        int tptr = 0;
        rule_no++;
        while(ptr < l)
        {
            int j=0;
            for(j = 0; j<50;j++)nt[j] = '\0';
            tptr = 0;
            while(ptr < l && line[ptr]!='-' && line[ptr]!=' ' &&
            line[ptr]!='|' && line[ptr]!='\t' && line[ptr]!='\n' && line[ptr]!='\r')
            {
                nt[tptr] = line[ptr];
                ptr++;tptr++;
            }
            if(first == 1){
                int i;
                for(i = 0; i < tptr ;i++)initial[i] = nt[i];
                first = 0;
            }
            if(nt[0]!='\0') insert_grammar(nt, rule_no);
            if(tptr!=0 || line[ptr] == '|')
            {
                if(line[ptr] == '|')
                {
                    rule_no++;
                    insert_grammar(initial, rule_no);
                }
            }
            ptr++;
        }
        int j;
        for(j=0;j<50;j++)initial[j] = '\0';
    }
    char dol[5] = "$";
    generate_hash_value(0,dol);
    fclose(fp);
}
