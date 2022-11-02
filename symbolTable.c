#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

manage *arrayDimensionsCheck(node *arrayNode, int hash_val_array, node *rng_ptr, int *f1, int *f2)
{
    int isDynamic = 0;
    int comp1 = strcmp(hashed_string[0][rng_ptr->child->child->hash_val],"ID");
    int comp2 = strcmp(hashed_string[0][rng_ptr->child->Next->Next->child->hash_val],"ID");
    manage *manageNode = (manage *)malloc(sizeof(manage));

    if(comp1 == 0)
    {
        int x = put_hash_sym(rng_ptr->child->child->ptr->tokenVal->tokenVal);
        //neither declared or defined as a function
        if(funcs[0][x]==0 && funcs[1][x]==0)
        {
            int scope_trav = currentScope;
            while(scope_trav!=0) // no global decs assumption...so scope = 0 means outermost scope
            {
                if(scope[scope_trav][x].sc!=-1)
                    break;
                else
                    scope_trav = parent[scope_trav]; //keep going to previous scope till we reach the scope where this ID is declared
            }

            if(scope_trav == 0 && (rng_ptr->child->child->sym_ptr == NULL || rng_ptr->child->child->sym_ptr->sc == -1))//not EVEN in symbol table....not created at all
            {
                *f1 = 1;
                sprintf(errorList[errorCount++],"ERROR (%d): THE FIRST DIMENSION - RANGE_BEGIN '%s' is not declared\n",rng_ptr->child->child->ptr->rowNum,rng_ptr->child->child->ptr->tokenVal->tokenVal);
                printf("No array can be declared without defined first dimension!\n");
                semantic_error = 1;
                rng_ptr->child->child->sym_ptr = &emptynode;

                //the arrays symbol table pos isnt defined yet
                //arrId->sym_ptr->ind1 = NULL;
            }
            else if(scope[scope_trav][x].type != INTEGER)
            {
                *f1 = 1;
                sprintf(errorList[errorCount++],"ERROR (%d): THE FIRST DIMENSION - RANGE_BEGIN '%s' is not an INTEGER type\n",rng_ptr->child->child->ptr->rowNum,rng_ptr->child->child->ptr->tokenVal->tokenVal);
               // printf("No array can be declared without defined first dimension!\n");
                semantic_error = 1;
               // rng_ptr->child->child->sym_ptr = &emptynode;
            }
            else
            {
                isDynamic = 1;
                rng_ptr->child->child->sym_ptr = &scope[scope_trav][x];
                rng_ptr->child->child->sym_ptr->isBound = 1;
                rng_ptr->child->child->sym_ptr->arrayHash = hash_val_array;

                manageNode->lowerBound = &scope[scope_trav][x];
                strcpy(manageNode->lowerBoundLexeme, rng_ptr->child->child->ptr->tokenVal->tokenVal);
            }
        }
    }
    else
    {
        manageNode->lowerBound = NULL;
        strcpy(manageNode->lowerBoundLexeme, "");
    }

    if(*f1== 0 && comp2 == 0)
    {
        //printf("second is an id\n");
        //*d2 = 1;
        int x = put_hash_sym(rng_ptr->child->Next->Next->child->ptr->tokenVal->tokenVal);

        //neither declared or defined
        if(funcs[0][x]==0 && funcs[1][x]==0)
        {
            int scope_trav = currentScope;
            while(scope_trav!=0) // no global decs assumption...so scope = 0 means outermost scope
            {
                if(scope[scope_trav][x].sc!=-1)//i.e. that ID IS declared in this particular scope called scope_trav
                    break;
                else
                    scope_trav = parent[scope_trav]; //keep going to previous scope till we reach the scope where this ID is declared
            }
            if(scope_trav == 0 && (rng_ptr->child->Next->Next->child->sym_ptr == NULL || rng_ptr->child->Next->Next->child->sym_ptr->sc == -1))//not EVEN in symbol table....not created at all
            {
                *f2 = 1;
                sprintf(errorList[errorCount++],"ERROR (%d): THE second DIMENSION - RANGE_END '%s' is not declared\n",rng_ptr->child->Next->Next->child->ptr->rowNum, rng_ptr->child->Next->Next->child->ptr->tokenVal->tokenVal);
                printf("No array can be declared without defined second dimensions!\n");
                semantic_error = 1;
                rng_ptr->child->Next->Next->child->sym_ptr = &emptynode;
            }
            else if(scope[scope_trav][x].type != INTEGER)
            {
                *f2 = 1;
                sprintf(errorList[errorCount++],"ERROR (%d): THE second DIMENSION - RANGE_END '%s' is not INTEGER type\n",rng_ptr->child->Next->Next->child->ptr->rowNum, rng_ptr->child->Next->Next->child->ptr->tokenVal->tokenVal);
                //printf("No array can be declared without defined second dimensions!\n");
                semantic_error = 1;
                //rng_ptr->child->Next->Next->child->sym_ptr = &emptynode;
            }
            else
            {
                isDynamic = 1;
                rng_ptr->child->Next->Next->child->sym_ptr = &scope[scope_trav][x];
                rng_ptr->child->Next->Next->child->sym_ptr->isBound = 1;
                rng_ptr->child->Next->Next->child->sym_ptr->arrayHash = hash_val_array;

                manageNode->higherBound = &scope[scope_trav][x];
                strcpy(manageNode->higheBoundLexeme, rng_ptr->child->Next->Next->child->ptr->tokenVal->tokenVal);
            }
        }
    }
    else
    {
        manageNode->higherBound = NULL;
        strcpy(manageNode->higheBoundLexeme, "");
    }
    manageNode->isDynamic = isDynamic;
    return manageNode;
}

int getWidthOfType(int x)
{
    switch(x)
    {
        case BOOLEAN:
            return 1;
        case INTEGER:
            return 2;
        case REAL:
            return 4;
        default:
            return -1;
    }
}

void resetSymbolTable()
{
    int i,j;

    for(i=0;i<1125;i++){
        funcs[0][i] = 0;
        funcs[1][i] = 0;
    }
    for(i=0;i<1125;i++)
        realVariables[i] = 0;
    redeclaration =0;
    serialNumber = 0;
    moduleName = 0;

    findScopeName[0] = '\0';

    scopedOrNot =0;

    for(i=0;i<1025;i++){
        inputList[i] = NULL;
        outputList[i] = NULL;
    }

    for(i=0;i<1050;i++)
        errorList[i][0] = '\0';
    errorCount = 0;


    memset(things, 0, sizeof(things));

    utilptr =0;

    globalScopeControl = 0;
    currentScope = 0;

    for(i=0;i<1050;i++)
    {
        hash[i][0] = '\0';
        used[i] = 0;
    }

    memset(scope, 0, sizeof(scope));

    for(i = 0; i<1110; i++)
    {
        parent[i] = 0;
        scopeMap[i] = 0;
        offsetCurr[i] =0;
        lines[0][i] = 0;
        lines[1][i] = 0;
    }
    for(i=0;i<50;i++)
        ret[i] ='\0';

    fooOffset = 0;
    globalTesting = 0;
}

int max(int a,int b)
{
    if(b>a)
        return b;
    return a;
}

int getType(char* str)
{
	if(strcmp(str,"integer") == 0)
		return INTEGER;//1
	else if(strcmp(str,"real") == 0)
		return REAL;//2
	else if(strcmp(str,"boolean") == 0)
        return BOOLEAN;//3
	else
		return ARRAY;//4
}

void setScopeMan(node *ch, int x, int typ, int inInputList)
{
    scope[currentScope][x].sc = currentScope;
    scope[currentScope][x].type = typ;
    scope[currentScope][x].dec = 0;

    //to be used in shadowing
    scope[currentScope][x].inInputList = inInputList;

    scope[currentScope][x].width = getWidthOfType(scope[currentScope][x].type);
    scope[currentScope][x].offset = offsetCurr[moduleName];

    offsetCurr[moduleName] += scope[currentScope][x].width;

    fooOffset = max(fooOffset,offsetCurr[moduleName]);

    strcpy(scope[currentScope][x].module_name,findScopeName);

    ch->sym_ptr = &scope[currentScope][x];
}

void setScopeManForArray(node *ch, int x, int lo, int hi, int subtype, int inInputList, int isDynamic, manage *manNode)
{
    //error if any bounds are declared negative
    if(isDynamic == 0 && (lo < 0 || hi < 0))
    {
        sprintf(errorList[errorCount++],"ERROR (%d): Arrays CAN NOT have bound that are negative as declared\n",ch->ptr->rowNum);
        semantic_error = 1;
        ch->sym_ptr = &emptynode;
    }
    else
    {
        //error if bounds are wrong
        if(lo > hi && manNode->higherBound == NULL && manNode->lowerBound == NULL)
        {
            sprintf(errorList[errorCount++],"ERROR (%d): Arrays CAN NOT have lower index greater than higher index\n",ch->ptr->rowNum);
            //printf("How is it possible to declare an array where its lower dimension is greater than its higher dimension\n");
            semantic_error = 1;
            ch->sym_ptr = &emptynode;
        }
        else
        {
            scope[currentScope][x].dec = 0;
            scope[currentScope][x].sc = currentScope;
            scope[currentScope][x].type = ARRAY;
            scope[currentScope][x].subtype = subtype;
            scope[currentScope][x].lb = lo;
            scope[currentScope][x].ub = hi;
            scope[currentScope][x].isDynamic = isDynamic;
            //printf("hash value is %d\n", x);
            scope[currentScope][x].arrayHash = x;

            scope[currentScope][x].lowerBound = manNode->lowerBound;
            scope[currentScope][x].higherBound = manNode->higherBound;

            strcpy(scope[currentScope][x].lowerBoundLexeme, manNode->lowerBoundLexeme);
            strcpy(scope[currentScope][x].higheBoundLexeme, manNode->higheBoundLexeme);
            //to know that this variable is hashed in the input list
            //will be used for shadowing
            scope[currentScope][x].inInputList = inInputList;

            scope[currentScope][x].width =  getWidthOfType(scope[currentScope][x].subtype) * (hi - lo + 1);
            scope[currentScope][x].offset = offsetCurr[moduleName];
            offsetCurr[moduleName] += scope[currentScope][x].width;
            fooOffset = max(fooOffset,offsetCurr[moduleName]);
            strcpy(scope[currentScope][x].module_name, findScopeName);
            ch->sym_ptr = &scope[currentScope][x];
        }
    }
}

//reverse of getType function
char* getNameOfType(int x)
{
	if(x==INTEGER)
		strcpy(ret,"integer");
	if(x==REAL)
		strcpy(ret,"real");
	if(x==BOOLEAN)
		strcpy(ret,"boolean");
	if(x==ARRAY)
		strcpy(ret,"array");
	return ret;
}

//putting in a 2D array called hash defined in symbolTableDef.h
//returns the UNIQUE hash value for that particular value
int put_hash_sym_unqiue(char* in)
{

    int init = 0, l = strlen(in), i, ret = -10, add = 1, iter = 0;
	for(i = 0;i < l ;i++)
	{
		init = (init*97+in[i])%MOD;
	}

	while(1)
	{
		iter++;
		if(iter == 10000)
			return -1;
		if(hash[init][0] == '\0')
		{
			ret = init;
			break;
		}
		else if(strcmp(hash[init],in) == 0)
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
	return ret;
}

int put_hash_sym(char* in)
{
    int init = 0, l = strlen(in), i, ret = -10, add = 1, iter = 0;
	for(i = 0;i < l ;i++)
	{
		init = (init*97+in[i])%MOD;
	}

	while(1)
	{
		iter++;
		if(iter == 10000)
			return -1;
		if(hash[init][0] == '\0')
		{
			strcpy(hash[init], in);
			ret = init;
			break;
		}
		else if(strcmp(hash[init],in) == 0)
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
	return ret;
}

//passing the new scope for 'FOR'
//called by postOrder
fields* getScopeFor(int x)
{
	fields* newnode = (fields*)malloc(sizeof(fields));
	newnode->sc = x;
	return newnode;
}

fields *getScopeSwitch(int x)
{
    fields* newnode = (fields*)malloc(sizeof(fields));
	newnode->sc = x;
	return newnode;
}

fields *getScopeWhile(int x)
{
    fields* newnode = (fields*)malloc(sizeof(fields));
	newnode->sc = x;
	return newnode;
}

//giving an Sequence of type TYP as arguments
//store in the scope's row => i.e. symbol table for that scope
int putHashId(node* head, int typ)
{
	int ret = -1;
	int iter = 0;
	node* ch;
	int y;

	//head is initially <Sequence>
	while(head!=NULL)
	{
		iter++;
		//ch is always pointing to an ID
		if(iter == 1)
            ch = head->child;
		else
            ch = head->child->Next;

		if(ch == NULL)
			break;

		//int x = get_hash_value_sym(ch->ptr->tokenVal->tokenVal);
        int	x = put_hash_sym(ch->ptr->tokenVal->tokenVal);


        //printf("this one is %s\n", ch->ptr->tokenVal->tokenVal);
        // redeclaration (in the same scope) inside the  so it is wrong
		if(scope[currentScope][x].sc!=-1)
		{
		    //if redeclaration inside the function or redeclaration of output var...its an error
		    if(scope[currentScope][x].inInputList == 2 || scope[currentScope][x].inInputList == 1)
            {
                ch->sym_ptr =  (fields *)malloc(sizeof (fields));
                ch->sym_ptr->dec = -1;
                sprintf(errorList[errorCount++],"ERROR (%d): '%s' was re-declared\n", ch->ptr->rowNum, hash[x]);
                ret = x;
            }
            else
            {
                //allowing shadowing to take place
                //doubt here whether it points to fields for the id in input list or to a new fields;
                setScopeMan(ch, x, typ, 2);
            }
		}
		else //not previously declared
		{
		 //   printf("must be declare only in scope\n");
		 //   printf("scope now is %d\n", currentScope);
			setScopeMan(ch, x, typ, 2);
		}
		head = ch->Next;
	}
	return ret;
}

// returns -1 if error (redeclartion) else 0;
int makeHashArray(node* head, int typ, int lo, int hi,int isDynamic, manage *manNode)
{
	int ret = -1;
	int j = 0;
	int y;
	node* ch;
	while(head!=NULL)
	{
		j++;
		if(j == 1)
            ch = head->child;
        else
            ch = head->child->Next;

		if(ch == NULL)
			break;
		int x = put_hash_sym(ch->ptr->tokenVal->tokenVal);
		if(scope[currentScope][x].sc!=-1)// redecleration in same scope with same name is damn wrong
		{
			// dont scope it
			if(scope[currentScope][x].inInputList == 2 || scope[currentScope][x].inInputList == 1)
            {
                ch->sym_ptr =  (fields*)malloc(sizeof (fields));
                ch->sym_ptr->dec = -1;
                sprintf(errorList[errorCount++],"ERROR (%d): '%s' was re-declared\n", ch->ptr->rowNum, hash[x]);
                ret = x;                    //return x;// that id's hash val for printing
			}
			else
            {
                setScopeManForArray(ch, x, lo, hi, typ, 2,isDynamic, manNode);
            }
		}
		else
		{
			setScopeManForArray(ch, x, lo, hi, typ, 2,isDynamic, manNode);
		}
		head = ch->Next;
	}
	//printf("iter%d\n",iter);
	return ret;
}

void fixList(fields *position, int hash_val, int x, int o)
{
    if(o == 1)
    {
        outputList[hash_val] = (Sequence*)malloc(sizeof(Sequence));
        outputList[hash_val]->sc = position->sc;
        outputList[hash_val]->type = position->type;
        outputList[hash_val]->subtype = position->subtype;
        outputList[hash_val]->ub = position->ub;
        outputList[hash_val]->hash_val = x;
        outputList[hash_val]->Next = NULL;
    }
    else
    {
        inputList[hash_val] = (Sequence*)malloc(sizeof(Sequence));
        inputList[hash_val]->sc = position->sc;
        inputList[hash_val]->type = position->type;
        inputList[hash_val]->subtype = position->subtype;
        inputList[hash_val]->ub = position->ub;
        //inputList[hash_val]->hash_val = x;
        inputList[hash_val]->Next = NULL;
    }
}

void fixAtEndOfList(fields *position, int hash_val, int x, int o)
{
    Sequence *id_list;

    ((o == 1)? (id_list = outputList[hash_val]) : (id_list = inputList[hash_val]));

    //going to END of the list
    while(id_list->Next!=NULL)
        id_list = id_list->Next;

    id_list->Next = (Sequence*)malloc(sizeof(Sequence));
    id_list =  id_list->Next;
    id_list->sc = position->sc;
    id_list->type = position->type;
    id_list->subtype = position->subtype;
    id_list->ub = position->ub;
    id_list->hash_val = x;
    id_list->Next = NULL;
}

//called by postOrder function
int makeHashInputList(node* head, int hash_val)
{
	int iter = 0;
	int ok = 0;
	//head is now <input_plist>
	//hash_val is unqiue hash of the module name
	//printf("thalapatgu\n");
	while(head!=NULL && head->child!=NULL)
	{
		iter++;

        //printf("%d\n",iter);

		int y = 0;
		node* var;

		if(iter == 1) // var is now ID
            var = head->child;
        else
            var = head->child->Next; //points to ID of <N1>

		if(var == NULL)
			break;

		node*  d_type = var->Next->Next;

		//got unqiue hash for that ID pointed to by 'var'
		//placed it in the 'hash' array
		//int x = get_hash_value_sym(var->ptr->tokenVal->tokenVal);
		int x = put_hash_sym(var->ptr->tokenVal->tokenVal);

		//gets the type - datatype
		int datatype = getType(d_type->child->ptr->tokenVal->tokenVal);

		if(datatype!=ARRAY)
		{
		    //printf("yup not an array\n");
            setScopeMan(var, x, datatype, 0);
            //printf("%d ", datatype);
		}
		else
        {
			node* rng_ptr = d_type->child->Next->Next;
            int f1 = 0, f2 = 0, d1 = 0, d2 = 0;


            /*int isDynamic = arrayDimensionsCheck(var, x, rng_ptr, &f1, &f2);*/
            manage *manNode = arrayDimensionsCheck(var, x, rng_ptr, &f1, &f2);
            int isDynamic = manNode->isDynamic;

            if(f1 == 0 && f2 == 0)      //i.e. both are defined...both are nums...this can happen at compile time
            {
                if(d1 == 0 && d2 == 0)
                {
                    //printf("yup its a compile time array\n");
                    int lo = -1, hi = -1;
                    if(isDynamic == 0)
                    {
                        lo = atoi(rng_ptr->child->child->ptr->tokenVal->tokenVal);
                        hi = atoi(rng_ptr->child->Next->Next->child->ptr->tokenVal->tokenVal);
                    }

                    if(manNode->lowerBound == NULL)
                        lo = atoi(rng_ptr->child->child->ptr->tokenVal->tokenVal);

                    if(manNode->higherBound == NULL)
                        hi = atoi(rng_ptr->child->Next->Next->child->ptr->tokenVal->tokenVal);

                    int subtype = getType(rng_ptr->Next->Next->Next->child->ptr->tokenVal->tokenVal);
                    setScopeManForArray(var, x, lo, hi, subtype, 0, isDynamic, manNode);
            //        printf("%d(%d) ", ARRAY, subtype);
                }
                else
                {
                    //dynamic array...looked at at RUN TIME
                }
            }
            else
            {
                printf("Error in the array dimensions...cannot pass this array as a function parameter\n");
            }
		}

		//<N1> didn't open yet
		if(iter == 1)
		{
		    //0 for inputList
		    //1 for outputList
		    fixList(&scope[currentScope][x], hash_val, x, 0);
		}
		else //<N1> has opened
		{
		    fixAtEndOfList(&scope[currentScope][x], hash_val, x, 0);
		}
		head = d_type->Next;
	}
	return ok;
}

// cant return arrays rest is same as above
//called by postOrder function for <module>
int makeHashOutputList(node* head,int hash_val)
{
	int iter = 0;
	int ok = 0;

	while(head!=NULL && head->child!=NULL)
	{
		iter++;
		int y = 0;
		node* var;

		if(iter == 1)
        {
            //var is now ID for <outp_list> -> ID COLON <type> <N2>
            var = head->child;
		}
        else
        {
            //if <N2> -> e then the head->child->Next will be NULL
            var = head->child->Next;
        }


		if(var == NULL)
			break;

        //d_type is now <type>
		node*  d_type = var->Next->Next;

        //get unique hash value for var which is ID
//		int x = get_hash_value_sym(var->ptr->tokenVal->tokenVal);
	int	x = put_hash_sym(var->ptr->tokenVal->tokenVal);

		int datatype = getType(d_type->child->ptr->tokenVal->tokenVal);

		if(datatype != ARRAY)
		{
		    setScopeMan(var, x, datatype, 1);
		}
		else
		{
			sprintf(errorList[errorCount++],"ERROR (%d): Can't have arrays as return type of modules for module '%s'", head->ptr->rowNum, hash[hash_val]);
			semantic_error = 1;
			//according to Grammar too...its not possible as <out_plist> -> ID COLON <type> <N2> and <type> -> INT | REAL | BOOLEAN...but no ARRAY
		}

		//i.e. var is ID
		//hash_val is unique hash value of that module name
		if(iter == 1)
		{
		    fixList(&scope[currentScope][x], hash_val, x, 1);
		}
		else
		{
		    //0 for inp_list
		    //1 for outputList
		    fixAtEndOfList(&scope[currentScope][x], hash_val, x, 1);
		}
		head = d_type->Next;
	}
	return ok;
}

//called by postOrder
int matchInputParameters(int x, node* head,int currentScope)
{
	int iter = 0;
	Sequence* trav = inputList[x];

	//trav is the args of the function
	//head is the args we are passing
    node* ch;
	while(trav!=NULL && head!=NULL)
	{


		//printf("%d\n",trav->type);
		iter++;
		if(iter == 1)
            ch = head->child; //<Sequence> -> id
		else
            ch = head->child->Next; //<N3> -> id

        if(ch == NULL)
            break;

        //get unique value for that value (ch->ptr->tokenVal->tokenVal) after putting into 2D array hash
		int x = put_hash_sym(ch->ptr->tokenVal->tokenVal);

        //function with hash value 'x' is neither declared nor defined
		if(funcs[0][x]==0 && funcs[1][x]==0)
		{//printf("%d  %s\n",scope[1][x].sc,head->ptr->tokenVal->tokenVal);

			int scope_trav = currentScope;
			while(scope_trav!=0)
			{
				if(scope[scope_trav][x].sc!=-1)
                    break;
				else
					scope_trav = parent[scope_trav];//keep going up to parent scopes
			}

			if(scope_trav == 0)
			{
				sprintf(errorList[errorCount++],"ERROR (%d): '%s' is not declared\n",ch->ptr->rowNum, ch->ptr->tokenVal->tokenVal);
				semantic_error = 1;
				ch->sym_ptr = &emptynode;
			}
			else
			{
				ch->sym_ptr = &scope[scope_trav][x];
			}
		}

		//check if the args we are passing are of same type as the args defined in the funtion
		if(ch->sym_ptr->type == trav->type)
		{
            if(trav->type == ARRAY)
			{
			    //printf("knckflnkfn line number %d",)
                //if the 2 arrays ar of different types (i.e. array of ints and array of reals) or have different upper bounds, return 0 (i.e. not a match)
				if(trav->subtype != ch->sym_ptr->subtype)
					return 0;
				else if(trav->ub != ch->sym_ptr->ub)
					return 0;
                //else if(trav->lb != ch->sym_ptr->lb)
                //    return 0;
			}
		}
		else
        {
			return 0;
		}
		//keep checking more
		head = ch->Next;
		trav = trav->Next;
	}
    if(ch == NULL && trav != NULL)
        return 0;
	if(head == NULL || head->child == NULL)
		return 0;
	if(trav == NULL && strcmp(hashed_string[0][head->child->hash_val],"e") == 0)
		return 1;
	else
		return 0;
}


int matchReturnedParameters(int x, node* head,int currentScope)
{
    //printf("%s\n", hash[x]);

	int iter = 0;
	Sequence* id_lst = outputList[x];
    node *ch;
	while(id_lst!=NULL && head!=NULL)
	{
		iter++;
		//node* ch;
        if(iter == 1)
        {
            ch = head->child;
           // printf("%s\n", ch->ptr->tokenVal->tokenVal);
        }
		else
        {
            ch = head->child->Next;
            //printf("%s\n", ch->ptr->tokenVal->tokenVal);
        }

        if(ch == NULL)
            break;

		int x = put_hash_sym(ch->ptr->tokenVal->tokenVal);

		if(funcs[0][x]==0 && funcs[1][x]==0)
		{//printf("%d  %s\n",scope[1][x].sc,head->ptr->tokenVal->tokenVal);
			int scope_trav = currentScope;
			while(scope_trav!=0)
			{
				if(scope[scope_trav][x].sc!=-1)
					break;
				else
					scope_trav = parent[scope_trav];
			}

			if(scope_trav == 0)
			{
				sprintf(errorList[errorCount++],"ERROR (%d): '%s' is not declared\n",ch->ptr->rowNum, ch->ptr->tokenVal->tokenVal);
				semantic_error = 1;
				ch->sym_ptr = &emptynode;
			}
			else
			{
				ch->sym_ptr = &scope[scope_trav][x];
			}
		}
		//puts("chk_matchInputParameters");
		//printf("%d %d\n",trav->type,ch->sym_ptr->type);
		if(ch->sym_ptr->type == id_lst->type)
		{
		    //printf("t1\n");
			if(id_lst->type == ARRAY)
			{
				//printf("trav %d ch %d\n",trav->subtype,ch->sym_ptr->subtype);
				if(id_lst->subtype!=ch->sym_ptr->subtype)
					return 0;
				else if(id_lst->ub!=ch->sym_ptr->ub)
					return 0;
			}
		}
		else
        {
			return 0;
		}

		head = ch->Next;
		id_lst = id_lst->Next;
	}
	//printf("%s\n",hashed_string[0][head->child->hash_val]);
	if(ch == NULL && id_lst != NULL)
        return 0;
	if(head == NULL || head->child == NULL)
		return 0;
	if(id_lst == NULL && strcmp(hashed_string[0][head->child->hash_val],"e") == 0)
		return 1;
	else
		return 0;
}


//extracting type information of that variable and storing it in the hash table
void stmtManip1(node *head)
{
    //head is <declareStmt>
    //t is <Sequence>
    node* t = head->child->Next;
    node *idNode = t->child;

    //typee is datatype
    node* node_type = t->Next->Next;

    int put_type = getType(node_type->child->ptr->tokenVal->tokenVal);

 //   printf("\nmacha what is type ma %s\n", typee->child->ptr->tokenVal->tokenVal);
    if(put_type!=ARRAY)
    {
        //t is <Sequence>
        //chk is the unique hash value of that ID that was redeclared
        //chk will be -1 if none of the IDs were redeclared => implies NO ERROR
        int chk = putHashId(t, put_type);
   //     printf("brother done bro\n");
        if(chk!=-1) // -1 here means all went well since hash val can't be -1
        {
            //sprintf(errorList[errorCount++],"ERROR_2: '%s' was re-declared at line number %d\n", hash[chk], typee->child->ptr->rowNum);
            semantic_error = 1;
        }
    }
    else // it is a damn array
    {
        //is <range>...should be <range_arrays>
        node* rangeptr = node_type->child->Next->Next;

        node* sub_type_ptr = rangeptr->Next->Next->Next;

        int sub_type = getType(sub_type_ptr->child->ptr->tokenVal->tokenVal);

        //dynamic array error to come here
        int f1 = 0, f2 = 0, d1 = 0, d2 = 0;
        //printf("yes this one only\n\n");
        //arrayDimensionsCheck(rng_ptr, &f1, &f2, &d1, &d2);
        //t->child points to ID
        //printf("heere\n");
        int x = put_hash_sym_unqiue( t->child->ptr->tokenVal->tokenVal);
        //printf("heere\n");
        /*int isDynamic = arrayDimensionsCheck(idNode, x, rangeptr, &f1, &f2);*/
        //printf("heere\n");
        manage *manNode =  arrayDimensionsCheck(idNode, x, rangeptr, &f1, &f2);
        //printf("heere\n");
        int isDynamic = manNode->isDynamic;

        int lo = -1, hi = -1, chk;

        if(f1 == 0 && f2 == 0)
        {
            if(isDynamic == 0)
            {
                lo = atoi(rangeptr->child->child->ptr->tokenVal->tokenVal);
                hi = atoi(rangeptr->child->Next->Next->child->ptr->tokenVal->tokenVal);
            }

            if(manNode->lowerBound == NULL)
                lo = atoi(rangeptr->child->child->ptr->tokenVal->tokenVal);

            if(manNode->higherBound == NULL)
                hi = atoi(rangeptr->child->Next->Next->child->ptr->tokenVal->tokenVal);

            chk = makeHashArray(t,sub_type,lo,hi,isDynamic,manNode);
            if(chk!=-1)//it was obviously redeclared
            {
                //sprintf(errorList[errorCount++],"ERROR_2: '%s' was re-declared at line number %d\n", hash[chk], typee->child->ptr->rowNum);
                semantic_error = 1;
            }
        }
    }
}

void stmtManip2(node *head)
{
    node* module_ = head->child->Next->Next;
    int hash_val = put_hash_sym(module_->ptr->tokenVal->tokenVal);
    if(funcs[0][hash_val])
    {
        sprintf(errorList[errorCount++],"ERROR (%d): '%s' is being redeclared\n",module_->ptr->rowNum, module_->ptr->tokenVal->tokenVal);
        semantic_error = 1;
    }
    funcs[0][hash_val] = 1;
}


void stmtManip3(node *head)
{
    int hash_val;

    scopedOrNot = 1;
    int scope_pt = ++globalScopeControl;
    parent[scope_pt] = currentScope;
    currentScope = scope_pt;
    moduleName = currentScope;

    node* inp_list = head->child; //pointing to <DEF>
    int ctr = 0;

    //DEF-> MODULE->ID->ENDDEF->TAKES->INPUT->SQBO-><input_plist>->SQBC SEMICOL <ret> <moduleDef>
    //break from this while loop when <input_plist> is hit
    while(ctr!=7)
    {
        inp_list = inp_list->Next;
        ctr++;
        if(ctr == 2)    //here we get name of module
        {
            hash_val = put_hash_sym(inp_list->ptr->tokenVal->tokenVal);

            if(funcs[1][hash_val] == 1)
            {
                sprintf(errorList[errorCount++],"ERROR (%d): '%s' is being re-defined\n\tTherefore, all the variables declared in this function redeclaration are NOT considered\n\n", inp_list->ptr->rowNum, inp_list->ptr->tokenVal->tokenVal);
                semantic_error = 1;

                //trying to return after redeclaration
                //redeclaration = 1;
                //return;
                //return 73;
            }
            else if(funcs[0][hash_val] == 1 && used[hash_val] == 0)     //function definition following its declaration
            {
                sprintf(errorList[errorCount++],"ERROR (%d): '%s' is being defined following its declaration. Its NOT being called in between declaration & definition\n\n", inp_list->ptr->rowNum, inp_list->ptr->tokenVal->tokenVal);
                semantic_error = 1;
            }

            funcs[1][hash_val] = 1;
            strcpy(findScopeName, inp_list->ptr->tokenVal->tokenVal);
        }
    }

    //printf("done till here bro\n");
    //if(strcmp(hash[hash_val],"function1") == 0)
        //printf("calling our required function in makeHashInputList\n");
                            //hash_val is the hash value of that new module name found in the while loop above
    //if(redeclaration == 0)
    //{
        int dot = makeHashInputList(inp_list, hash_val);// hash vars into new scope + fill def list variables for this module

        //printf("done till here bro\n");

        if(dot)
        {
            sprintf(errorList[errorCount++],"ERROR: Some anomaly in parameter list of module '%s'\n",hash[hash_val]);
            semantic_error = 1;
        }

        dot = 0;

        node* outp_list = inp_list;

        //outp_list is now <ret>
        outp_list = outp_list->Next->Next->Next;


        //i.e. if the <ret> IS NOT EMPTY
        if(strcmp(hashed_string[0][outp_list->child->hash_val],"e") != 0)
        {
            //now it is <out_plist>
            outp_list = outp_list->child->Next->Next;

            dot = makeHashOutputList(outp_list,hash_val);

            //hash_val is unique hash value of module name
            checkVarsAssigned(hash_val,head);

            Sequence* id_lst = outputList[hash_val];

            while(id_lst!=NULL)
            {
                if(id_lst->mark!=1)
                {
                    sprintf(errorList[errorCount++],"ERROR: variable '%s' in module '%s' return list not assigned a value\n",hash[id_lst->hash_val],hash[hash_val]);
                    semantic_error = 1;
                }
                id_lst = id_lst->Next;
            }
        }
    //}
}

void printArrays()
{
    int i,j;
    int sr = 1;
    for(i=0;i<110;i++)
    {
        for(j=0;j<1150;j++)
        {
            if(scope[i][j].type == ARRAY)
            {
                int s = scope[i][j].sc;
                printf("%d\t%s\t\t", sr, scope[i][j].module_name);

                //printf("t\n");
                printf("%d-",lines[0][s]);
                printf("%d",lines[1][s]);

                //int p = scope[i][j].arrayHash;

                //printf("\n%d\n", p);
                printf("\t%s\t", hash[j]);

                //printf("t\t");

                if(scope[i][j].isDynamic == 1)
                {

                    printf("yes\t");
                    //printf("hi is %d", scope[i][j].ub);

                    if(scope[i][j].lowerBound != NULL)
                        printf("[%s, ", scope[i][j].lowerBoundLexeme);
                    else
                        printf("[%d, ", scope[i][j].lb);

                    if(scope[i][j].higherBound != NULL)
                        printf("%s]\t", scope[i][j].higheBoundLexeme);
                    else
                        printf("%d]\t", scope[i][j].ub);
                }
                else{
                    printf("no\t");
                    printf("[%d,%d]  \t", scope[i][j].lb, scope[i][j].ub);
                }

                int sb = scope[i][j].subtype;
                if(sb == 1)
                {
                    printf("integer\n");
                }
                else if(sb == 2)
                {
                    printf("real\n");
                }
                else if(sb == 3)
                {
                    printf("boolean\n");
                }
                sr++;
            }
        }
    }
}

void stmtManip4(node *head)
{
    scopedOrNot = 1;
    int scope_pt = ++globalScopeControl;
    parent[scope_pt] = currentScope;
    currentScope = scope_pt;
    moduleName = currentScope;
    strcpy(findScopeName,"driver");
    int x = put_hash_sym( "driver");
    //printf("x is %d", x);
    funcs[1][x] = 1;
}

void stmtManip6(node *head)
{
    // printf("Yes start is called\n");
    if(!scopedOrNot)//not scoped
    {
        //creating new scope, attaching it to parent scope
        int scope_pt = ++globalScopeControl;
        parent[scope_pt] = currentScope;
        currentScope = scope_pt;
    }
    else//scoped
    {
        scopedOrNot = 0;
    }
    //this new scope (our current scope) starts at line number....
    lines[0][currentScope] = head->ptr->rowNum;
}

void stmtManip7(node *head)
{
    // printf("yes end is called\n");
    lines[1][currentScope] = head->ptr->rowNum;
    currentScope = parent[currentScope];  //go back to parent scope cuz this scope is OVER
}

void stmtManip8(node *head)
{
    scopedOrNot = 1;
    int scope_pt = ++globalScopeControl;
    parent[scope_pt] = currentScope;
    currentScope = scope_pt;
    head->sym_ptr = getScopeFor(currentScope);
    //printf("thala\n");
}

void stmtManip11(node *head)
{
    scopedOrNot = 1;
    int scope_pt = ++globalScopeControl;
    parent[scope_pt] = currentScope;
    currentScope = scope_pt;
    //printf("thala\n");
    head->sym_ptr = getScopeFor(currentScope);
}

void stmtManip9(node *head)
{
    // printf("\njfoeono\n");

    node *idInSwitch = head->Next->Next;
    int type_switchvar = getType(idInSwitch->ptr->tokenVal->tokenVal);

    //printf("%d\n", REAL);
    //printf("%s type of switch var is %d\n", idInSwitch->ptr->tokenVal->tokenVal, typeOfSwitchVar);

    if(type_switchvar == REAL)
    {
        semantic_error = 1;
        printf("ERROR (%d): '%s' in SWITCH construct is of type REAL\n", idInSwitch->ptr->rowNum, idInSwitch->ptr->tokenVal->tokenVal);
    }

    scopedOrNot = 1;
    int scope_pt = ++globalScopeControl;
    parent[scope_pt] = currentScope;
    currentScope = scope_pt;                    //'SWITCH' has its new scope
    //printf("thala\n");
    head->sym_ptr = getScopeSwitch(currentScope);   //returns 'fields' with its scope fixed to currentScope for 'FOR'
    //printf("thala\n");
}

void stmtManip10(node *head)
{
    int x = put_hash_sym(head->ptr->tokenVal->tokenVal);

    //neither declared or defined
    if(funcs[0][x]==0 && funcs[1][x]==0)
    {
        int scope_trav = currentScope;
        while(scope_trav!=0)
        {
            if(scope[scope_trav][x].sc!=-1)
                break;
            else
                scope_trav = parent[scope_trav]; //keep going to up scopes
        }
        if(scope_trav == 0 && head->sym_ptr == NULL)
        {
            sprintf(errorList[errorCount++],"ERROR (%d): '%s' is not declared\n",head->ptr->rowNum,head->ptr->tokenVal->tokenVal);
            semantic_error = 1;
            head->sym_ptr = &emptynode;
        }
        else // update in parse tree' pointer
        {
            head->sym_ptr = &scope[scope_trav][x];
        }
    }
}

void matcherMan(node *head, node *to_call, node *list, int hash_call)
{
    //unique hash val of that function, node *list, currentScope int
    //this function returns 0 => not match
    //              returns 1 => yes match
   //printf("t0\n");
    int inp = matchInputParameters(hash_call, list, currentScope);

    //printf("value of input_match is %d", input_match);
//    printf("t1\n");
    //therefore, no match => error
    if(inp == 0)
    {
        sprintf(errorList[errorCount++],"ERROR (%d): '%s' parameter list does NOT match the call\n", to_call->ptr->rowNum, to_call->ptr->tokenVal->tokenVal);
        semantic_error = 1;
    }

    int out = 1;

    //printf("hello\n");

    //head->child->child == <moduleReuseStmt>-><optional>->SQBO
    if(strcmp(hashed_string[0][head->child->child->hash_val],"SQBO") == 0)
    {
        //printf("t1\n");
        // there's a return list
        //passing unqiue value of func name, <Sequence>, scopenum
        //very similar function call as matchInputParameters above
        out = matchReturnedParameters(hash_call,head->child->child->Next,currentScope);
        //printf("t1\n");
    }
    if(out == 0)   //error
    {
        sprintf(errorList[errorCount++],"ERROR (%d): '%s' return list does NOT match the call\n", to_call->ptr->rowNum, to_call->ptr->tokenVal->tokenVal);
        semantic_error = 1;
    }
}

void helper1(node *head, node *_node, int hash_call)
{
    //printf("t2\n");
    //implies we are calling same function again

    if(strcmp(findScopeName, _node->ptr->tokenVal->tokenVal) == 0)
    {
        //log error into error log
        sprintf(errorList[errorCount++],"ERROR (%d): Recursion not allowed\n",_node->ptr->rowNum);
        semantic_error = 1;
    }
    //list points to <Sequence>
    node* list  =  _node->Next->Next->Next;
    //printf("t1\n");
    //NOW must match input and output lists...
    matcherMan(head, _node, list, hash_call);
    used[hash_call] = 1;            //i.e. that function is used up in definition
    //printf("t2\n");
}

void helper2(node *head, node *_node, int hash_call)
{
    //printf("%s on line number %d\n", hash[hash_call], to_call->ptr->rowNum);
    //pointing to <Sequence>
    node* list  =  _node->Next->Next->Next;

    /*
    if(list != NULL && strcmp(hash[hash_call], "function1") == 0)
        printf("yup the input list is NOT null\n");
    */
    things[utilptr].in = list;

    //sqb points to 'USE' to get rownum
    node *sqb = head->child->Next;

    things[utilptr].row_num = sqb->ptr->rowNum;
    things[utilptr].scope = currentScope;

                                //pointing to SQBO
    if(strcmp(hashed_string[0][head->child->child->hash_val],"SQBO") == 0){
        //pointing to <Sequence> of optional
        things[utilptr].out = head->child->child->Next;
    }
    else
        things[utilptr].out = NULL;

    //UNIQUE hash value of function call
    things[utilptr].x = hash_call;
    utilptr++;
    used[hash_call] = 1;
}

//when calling a function
void stmtManip5(node *head)
{
    //printf("t1\n");
    //head == <moduleReuseStmt>
    node* _node = head->child->Next->Next->Next;//<optional>-> use->mod->id i.e. the function name which is to be called
    //value of that token (function name ... MOD ID) only
    int hash_call = put_hash_sym(_node->ptr->tokenVal->tokenVal);

    //if a function with hash value x is already defined
    if(funcs[1][hash_call])
    {
        helper1(head, _node, hash_call);
    }
    else if(funcs[0][hash_call])     //that function is not defined BUT declared
    {
        helper2(head, _node, hash_call);
    }
    else                        //that function NIETHER defined or declared => ERROR
    {
        _node->sym_ptr = &emptynode;  //that function call ID points to EMPTY 'fields'
        sprintf(errorList[errorCount++],"ERROR (%d): The function '%s' has not been defined\n", _node->ptr->rowNum, hash[hash_call]);
        semantic_error = 1;
    }
 //   printf("t3\n");
}

void checkIfArrayElement(node *ch, int x)
{
    int ret = 0;
    node *arr = ch;
    ch = ch->Next;  //<whichID>
    ch = ch->child; //SQBO


    if(strcmp(ch->ptr->tokenVal->tokenVal, "SQBO") == 0) //it is an array element
    {
        ret = 1;
        node *ind = ch->Next;   //<index>
        //strcmp(hashed_string[0][rng_ptr->child->child->hash_val],"ID") == 0
        if(strcmp(hashed_string[0][ind->child->hash_val],"ID") == 0)//A[k]
        {
            node *idnode = ind->child;
            int x = put_hash_sym( idnode->ptr->tokenVal->tokenVal);

            if(funcs[0][x]==0 && funcs[1][x]==0)
            {
                int scope_trav = currentScope;
                while(scope_trav!=0)
                {
                    if(scope[scope_trav][x].sc!=-1)
                        break;
                    else
                        scope_trav = parent[scope_trav];
                }
                if(scope_trav == 0 && (idnode->sym_ptr == NULL || idnode->sym_ptr->sc == -1))//not EVEN in symbol table....not created at all
                {
                    sprintf(errorList[errorCount++],"ERROR (%d): Cannot access array '%s' with element that is not declared\n", idnode->ptr->rowNum, arr->ptr->tokenVal->tokenVal);
                    semantic_error = 1;
                    idnode->sym_ptr = &emptynode;
                }
                else // update in parse tree' pointer
                {
                    idnode->sym_ptr = &scope[scope_trav][x];
                }
            }

        }
        else    //A[3]
        {

        }
    }
    else
    {

    }
}

//called by postOrder
void checkVarsAssigned(int idx, node* head)
{
	if(head == NULL)
		return;

    //i.e. head is a non terminal <assignmentStmt>
    //comes in <moduleDef> in <module>
	if(head->check == 1 && strcmp(hashed_string[1][head->hash_val],"<assignmentStmt") == 0)
	{
	    //get the output list of that particular module
	    //head->child points to ID now for <assignmentStmt> -> ID <whichStmt>
		Sequence* id_lst = outputList[idx];
		//i.e. if that ID exists as an output variable but it is NOT defined yet in assignment statements inside the module
		while(id_lst!=NULL && id_lst->hash_val!=(put_hash_sym(head->child->ptr->tokenVal->tokenVal)))
		{
			id_lst = id_lst->Next;
		}
		if(id_lst!=NULL)
			id_lst->mark = 1;
	}
	node* temp = head->child;
	while(temp!=NULL)
	{
		checkVarsAssigned(idx,temp);
		temp = temp->Next;
	}
}

//called by makeSymbolTable function
void postOrder(node* head)
{
	if(head == NULL)
		return;

	int has = head->hash_val;

	//check 0 => terminal
	//check 1 => non terminal
	//declarestmt, module, moduledeclaration, modulereusestmt, drivermodule
	if(head->check == 1)
	{
        if(strcmp(hashed_string[1][has],"<declareStmt") == 0)
		    stmtManip1(head);
		else if(strcmp(hashed_string[1][has],"<moduleDeclaration") == 0)  // DECLARATIONS
			stmtManip2(head);
		else if(strcmp(hashed_string[1][has],"<module") == 0){
			stmtManip3(head);
		}
		else if(strcmp(hashed_string[1][has],"<driverModule") == 0){
			stmtManip4(head);
		}
		else if(strcmp(hashed_string[1][has],"<moduleReuseStmt") == 0) // calling modules handling functions defined above
        {
            stmtManip5(head);
        }
	}

	//START, END, FOR, ID
	if(head->check == 0) // NOW consider TERMINALS i.e. starts and ends
	{
	    /*if(strcmp(hashed_string[0][head->hash_val],"CASE") == 0)
            printf("yes it is calling case\n");

        if(strcmp(hashed_string[0][head->hash_val],"BREAK") == 0)
            printf("yes it is calling break\n");

        if(strcmp(hashed_string[0][head->hash_val],"DEFAULT") == 0)
            printf("yes it is calling default\n");
        */
		if(strcmp(hashed_string[0][head->hash_val],"START") == 0)
		{
		   stmtManip6(head);
		}
		else if(strcmp(hashed_string[0][head->hash_val],"END") == 0)
		{
		   stmtManip7(head);
		}
		else if(strcmp(hashed_string[0][head->hash_val],"FOR") == 0)
		{
            stmtManip8(head);
		}
		else if(strcmp(hashed_string[0][head->hash_val],"WHILE") == 0)
        {
            stmtManip11(head);
        }
		else if(strcmp(hashed_string[0][head->hash_val], "SWITCH") == 0)
        {
            stmtManip9(head);
        }
		else if(strcmp(hashed_string[0][head->hash_val],"ID") == 0)
		{
			stmtManip10(head);
		}
	}


    node* temp = head->child;
    //post order...first as deep as possible then rightwards
    //printf("%s\n", hashed_string[head->check][head->hash_val]);
    while(temp!=NULL)
    {
        postOrder(temp);
        temp = temp->Next;
    }
}

void checkErrorFor(){
        Token * node_=tokenList_head;
        while(node_!=NULL){
            if(strcmp(node_->tokenName,"FOR")==0){
                node_=node_->Next;
                node_=node_->Next;
                char temp[50];
                strcpy(temp,node_->value);
                node_=node_->Next;
                int cnt=0;
                while(strcmp(node_->tokenName,"START")!=0) node_=node_->Next;
                cnt++;
                node_=node_->Next;
                while(cnt>0){
                    int flag=1;
                    if(strcmp(node_->value,temp)!=0 && strcmp(node_->tokenName,"ID")==0) {
                        node_=node_->Next;
                        if(strcmp(node_->tokenName,"SQBO")==0){
                            node_=node_->Next;
                            if(strcmp(node_->value,temp)==0){
                                node_=node_->Next;
                            }
                        }
                    }
                    if(strcmp(node_->tokenName,"END")==0) cnt--;
                    else if(strcmp(node_->tokenName,"START")==0) cnt++;
                    if(strcmp(node_->value,temp)==0){
                        node_=node_->Next;
                        int currRow=node_->rowNum;
                        while(node_->rowNum==currRow){
                            if(strcmp(node_->tokenName,"ASSIGNOP")==0){
                                sprintf(errorList[errorCount++],"ERROR (%d): Can't assign for loop variable again\n",node_->rowNum);
                            }
                            node_=node_->Next;
                            flag=0;
                        }
                    }
                    if(flag) node_=node_->Next;
                }
            }
            node_=node_->Next;
        }
}

//passing the parse tree head
void makeSymbolTable(node* pt_head)
{
	emptynode.sc = -1;
	emptynode.type = -1;
	emptynode.subtype = -1;
	emptynode.lb = -1;
	emptynode.ub = -1;
	emptynode.offset = -1;
	emptynode.dec = -1;

	memset(parent,-1,sizeof parent);
	memset(scope,-1,sizeof scope);

	checkErrorFor();

	int i;
	struct Symbol_Table* sym_ = symbolTable_head;

	//symbol table made in the lexer
	//postOrder it and for all IDs...i.e. variables
	//get their unique hash value and put in 'hash'
	int cnt = 0;

	//printf("\n\nSTART\n");
	while(sym_!=NULL)
	{
		if(strcmp(sym_->tokenName,"ID")==0)
		{
		    cnt++;
		    //printf("%d. %s\t\t%s\n", cnt, trav->tokenName, trav->tokenVal);
			//int x = get_hash_value_sym(trav->tokenVal);
			int x = put_hash_sym(sym_->tokenVal);
		}
		sym_ = sym_->Next;
	}
	//printf("\nDONE\n\n");

	postOrder(pt_head);

	for (i = 0; i <=1000 ; ++i)
	{
		int j = 1;
		int curr = i;

        while(parent[curr]!=0)
		{
			++j;
			curr = parent[curr];
		}
		scopeMap[i] = j;
	}
}


void printErrors()
{
	int i;
	for (i = 0; i < errorCount; ++i)
	{
		printf("%s",errorList[i]);
	}
}
//this function works by traversing the whole parsetree but ONLY hitting IDs
//and prints the IDs' name, type, scope, start and end lines, and offset at which it hits
//must be used only after postOrder....cuz postOrder is the one that fills the symbol table 'fields'
void printSymbolTable(node* head)
{
	int m;

	if(head == NULL)
		return;


	if(strcmp(hashed_string[0][head->hash_val],"ID") == 0)
	{
		int x = put_hash_sym(head->ptr->tokenVal->tokenVal);

		if(funcs[0][x]==0 && funcs[1][x]==0 && head->sym_ptr->dec == 0)
		{
            serialNumber++;
			head->sym_ptr->dec = 1;
			printf("%d\t",serialNumber);
			printf("%s\t",head->ptr->tokenVal->tokenVal);
			printf("%s\t",head->sym_ptr->module_name);
			int y = head->sym_ptr->sc;
			printf("%d",lines[0][y]);;
            //it+=
            printf("-");
            //it+=
            printf("%d\t",lines[1][y]);

            printf("%d\t",head->sym_ptr->width);

            if(head->sym_ptr->type == ARRAY)
            {
                printf("yes");
            }
            else
            {
                printf("no");
            }
            printf("\t");

            if(head->sym_ptr->type == ARRAY)
            {
                if(head->sym_ptr->isDynamic == 1)
                    printf("yes");
                else
                    printf("no");
            }
            else
            {
                printf("--");
            }
            printf("\t");

            if(head->sym_ptr->type == ARRAY)
            {
                if(head->sym_ptr->isDynamic)
                {
                    if(head->sym_ptr->lowerBound!= NULL)
                        printf("[%s, ", head->sym_ptr->lowerBoundLexeme);
                    else
                        printf("[%d, ", head->sym_ptr->lb);

                    if(head->sym_ptr->higherBound != NULL)
                        printf("%s]\t", head->sym_ptr->higheBoundLexeme);
                    else
                        printf("%d]\t", head->sym_ptr->ub);
                }
                else
                {
                    printf("[%d,%d]", head->sym_ptr->lb, head->sym_ptr->ub);
                }
            }
            else
            {
                printf("--");
            }
            printf("\t");

            printf("%s\t",getNameOfType(head->sym_ptr->type));

            printf("%d\t",head->sym_ptr->offset);

            if(head->sym_ptr->inInputList == 0 || head->sym_ptr->inInputList == 1)
                printf("0\t");
            else
                printf("%d\t",scopeMap[head->sym_ptr->sc]);

            printf("\n");
			realVariables[x] = 1;
		}
	}

	node* temp = head->child;
	while(temp!=NULL)
	{
		printSymbolTable(temp);
		temp = temp->Next;
	}
}

void remainingErrors()
{
	int i;
	for (i = 0; i < utilptr; ++i)
	{
		int inp = matchInputParameters(things[i].x, things[i].in, things[i].scope);
		if(inp == 0)
		{
            sprintf(errorList[errorCount++],"ERROR (%d): '%s' parameter list doesn't match the call\n", things[i].row_num, hash[things[i].x]);
			semantic_error = 1;
		}
		int out = 1;
		if(things[i].out!=NULL) // there's a return list
			out = matchReturnedParameters(things[i].x,things[i].out,things[i].scope);
		if(out == 0)
		{
            sprintf(errorList[errorCount++],"ERROR (%d): '%s' return list does NOT match the call\n", things[i].row_num, hash[things[i].x]);
			semantic_error = 1;
		}
	}
}
