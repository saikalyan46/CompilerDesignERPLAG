#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "symbolTable.h"
#include "AST.h"

//int main()
int main(int argc,char *argv[])
{
clock_t begin;
    int in;
	FILE *fp;
	char *line = NULL;
	//char *argv[5];

	//argv[1]="t10.txt";
	//argv[2]="test.asm";

    //printf("hello\n");
	//size_t len = 0;
    int flag = 0;
	while(1)
    {
        if(flag)
        {
            printf("\n\t\tGoodbye\n\n");
            break;
        }

        printf("\n\n\n\t\tPress option for the defined task\n\n");
        printf("0 : For exiting the program loop\n");
        printf("1 : For printing the token list generated by the lexer\n");
        printf("2 : For parsing to verify the syntactic correctness of the input source code and to produce parse tree\n");
        printf("3 : For printing the Abstract Syntax Tree in appropriate format. Also specify the traversal order at the beginning\n");
        printf("4 : For displaying the amount of allocated memory and number of nodes to each of parse tree and abstract syntax tree\n");
        printf("5 : For printing the Symbol Table in appropriate format showing all relevant information\n");
        printf("6 : For printing the total memory requirement (sum total of widths of all variables in the function scope) for each function\n");
        printf("7 : For printing the type expressions and width of array variables in a line for a test case\n");
        printf("8 : For compiling to verify the syntactic and semantic correctness of the input source code\n");
        printf("9 : For producing assembly code\n");
        scanf("%d",&in);

        switch(in)
        {
            case 0: //wanting to exit from the loop
                    flag = 1;
                    break;
            case 1:
                    printf("\t\t\nPrinting the token stream...\n");
                    resetLexer();
                    resetParser();
                    resetSymbolTable();
                    resetTypeCheck();
                    resetCodeGen();

                    read_symbolTable();
                    setUpStream(argv[1]);
                    printTokenStream();
                    break;
            case 2:
                    resetLexer();
                    resetParser();
                    resetSymbolTable();
                    resetTypeCheck();
                    resetCodeGen();

                    read_symbolTable();
                    setUpStream(argv[1]);
                    Read_Grammar();
                    createParseTable();
                    parseInputSourceCode();
                    if(error == 0)
                        printf("\t\t\nNo syntactic errors. Printing the parse tree...\n");
                    else
                        printf("\t\t\nSyntactic errors present. But printing the partial parse tree...\n");
                    printParseTree(parseTree_head);
                    break;

            case 3:
                    resetLexer();
                    resetParser();
                    resetSymbolTable();
                    resetTypeCheck();
                    resetCodeGen();

                    read_symbolTable();
                    setUpStream(argv[1]);
                    Read_Grammar();
                    createParseTable();
                    parseInputSourceCode();

                    if(error == 0)
                    {
                        printf("\t\t\nNo syntactic errors. Printing the AST in inorder traversal...\n");
                        generateAST(parseTree_head);
                        printAST(parseTree_head);
                    }
                    else
                    {
                        printf("There are syntactic errors in code, hence can't generate AST\n");
                    }
                    break;
            case 4:
                    resetLexer();
                    resetParser();
                    resetSymbolTable();
                    resetTypeCheck();
                    resetCodeGen();

                    read_symbolTable();
                    setUpStream(argv[1]);
                    Read_Grammar();
                    createParseTable();
                    parseInputSourceCode();

                    if(error == 0)
                    {
                        ParseTreenodes(parseTree_head);
                        generateAST(parseTree_head);
                        ASTnodes(parseTree_head);
                        printf("Parse tree     Number of nodes = %d     Allocated Memory = %lu Bytes\n",nodes_par,nodes_par*sizeof(node));
                        printf("AST            Number of nodes = %d     Allocated Memory = %lu Bytes\n",nodes_ast,nodes_ast*sizeof(node));
                        printf("Compression percentage = %lf\n",(nodes_par-nodes_ast)*100.0/nodes_par);
                    }
                    else
                    {
                        printf("Code is syntactically not correct & hence parse tree and AST can't be constructed\n");
                    }
                    break;
            case 5:
                    resetLexer();
                    resetParser();
                    resetSymbolTable();
                    resetTypeCheck();
                    resetCodeGen();

                    read_symbolTable();
                    setUpStream(argv[1]);
                    Read_Grammar();
                    createParseTable();
                    parseInputSourceCode();

                    if(error == 0)
                    {
                        makeSymbolTable(parseTree_head);
                        printf("s\tvar_n\tscope\t\tlines\twidth\tisArr\tisDyn\t  range\ttype\toffset\tnest\n");
                        printSymbolTable(parseTree_head);
                    }
                    else
                    {
                        puts("Symbol Table can't be printed since there are syntactical errors");
                    }
                    break;
            case 6: //printing the activation record size
                    resetLexer();
                    resetParser();
                    resetSymbolTable();
                    resetTypeCheck();
                    resetCodeGen();

                    read_symbolTable();
                    setUpStream(argv[1]);
                    Read_Grammar();
                    createParseTable();
                    parseInputSourceCode();
                    makeSymbolTable(parseTree_head);

                    int i,j,k;
                    int f1 = 0, f2 = 0;
                    printf("functionName\t\tactivationRecordSize\n");
                    for(i = 0; i<1125; i++)
                    {
                        int cnt = 0;
                        if(funcs[1][i] == 1)//i.e. function is defined;
                        {
                            //printf("hello\n");
                            for(j =0;j< 110;j++)
                            {
                                for(k=0;k<1150;k++)
                                {
                                    if(strcmp(scope[j][k].module_name, hash[i]) == 0 && scope[j][k].inInputList == 2)
                                        cnt+=scope[j][k].width;
                                }
                            }
                            printf("%s\t\t\t\t%d\n", hash[i], cnt);
                        }
                    }

                    break;
            case 7: //printing the different static and dynamic arrays declared in the code
                    resetLexer();
                    resetParser();
                    resetSymbolTable();
                    resetTypeCheck();
                    resetCodeGen();

                    read_symbolTable();
                    setUpStream(argv[1]);
                    Read_Grammar();
                    createParseTable();
                    parseInputSourceCode();
                    makeSymbolTable(parseTree_head);

                    if(error == 0)
                    {
                        printf("s\tscope\tlines\tvar_n\tisDyn\trange\ttype\n");
                        printArrays();  //calls all arrays in the program
                    }
                    else
                    {
                        puts("Symbol Table can't be printed since there are syntactical errors");
                    }
                    break;
           case 8:


			begin = clock();
                    resetLexer();
                    resetParser();
                    resetSymbolTable();
                    resetTypeCheck();
                    resetCodeGen();

                    read_symbolTable();
                    setUpStream(argv[1]);
                    Read_Grammar();
                    createParseTable();
                    parseInputSourceCode();

                    if(error == 0)
                    {
                        printf("No Synatactic Errors\n");
                        printf("Semantic errors are mentioned below:\n\n");
                        makeSymbolTable(parseTree_head);
                        remainingErrors();
                        printErrors();
                        generateAST(parseTree_head);

                        checkTypes(parseTree_head);

                        switchStmtErrors(parseTree_head);

                        if(semantic_error == 0)
                            puts("Code compiles successfully...\n");
                    }
		    begin = clock() - begin;
		    double time_spent = 0.0;
		    time_spent += (double)(begin) / CLOCKS_PER_SEC;
		    printf("Time elpased is %f seconds", time_spent);

                    break;
            case 9:
                    resetLexer();
                    resetParser();
                    resetSymbolTable();
                    resetTypeCheck();
                    resetCodeGen();

                    read_symbolTable();
                    setUpStream(argv[1]);
                    Read_Grammar();
                    createParseTable();
                    parseInputSourceCode();

                    if(error==0)
                    {
                        printf("\n");
                        makeSymbolTable(parseTree_head);
                        remainingErrors();
                        generateAST(parseTree_head);
                        checkTypes(parseTree_head);
                        switchStmtErrors(parseTree_head);

                        if(semantic_error==0)
                        {
                            // freopen(argv[2],"w",stdout);
                            // //printf("%c",'%');
                            // freopen("CON", "w", stdout);
			    //int temp_stdout = dup(1);
			    //FILE *fp;
                            //fp = freopen(argv[2],"w",stdout);
			    fp_asm = fopen(argv[2], "w");
                            fprintf(fp_asm,"global main\nextern printf\nextern scanf\nextern puts\nsection .data\nMEM: times 1000 db 0\narr:  times 2 db 0\nmen:  db \"Output: %%d \",10,0\ntru:  db \"Output: True\",10,0\nfal:  db \"Output: False\",10,0\ninputFormat: db \"%%d\",0\nvar: times 8 db 0\ninput_int: db \"Input: Enter an integer value\",10,0\n");
                            fprintf(fp_asm,"input_array_1: db \"Input: Enter %%d array elements of integer type \",0\n");
                            fprintf(fp_asm,"input_array_2: db \"for range %%d \",0\n");
                            fprintf(fp_asm,"input_array_3: db \"to %%d\",10,0\n");
                            fprintf(fp_asm,"out: db \"Output:\",0\nout_array:db \" %%d\",0\n");
                            fprintf(fp_asm,"new_line: db 10,0\n");
                            // printf("dynacount: times 8 db 0\n");
                            fprintf(fp_asm,"DYNA: times 1000 db 0\n");
                            fprintf(fp_asm,"error_print: db \"RUN TIME ERROR:  Array index out of bound\",10,0\n");
                            fprintf(fp_asm,"\n\nsection .text\nmain:\n\n");
                            codeGeneration(parseTree_head);
                            fprintf(fp_asm,"jmp exit\n");
			    fprintf(fp_asm,"error_bound:\n");
                            fprintf(fp_asm,"sub rsp, 0x28\n");
                            fprintf(fp_asm,"push    rax\n");
                            fprintf(fp_asm,"  push    rcx\n");
                            fprintf(fp_asm,"\tmov     rdi, error_print\n");
                            // printf("\tmov     rdx, rax\n");
                            fprintf(fp_asm,"\txor     rax, rax\n");
                            fprintf(fp_asm,"\tcall    printf\n");
                            fprintf(fp_asm,"  pop     rcx\n");
                            fprintf(fp_asm,"pop     rax\n");
                            fprintf(fp_asm,"add rsp, 0x28\n");
			    fprintf(fp_asm,"pop ax\n");
                            // printf("mov rax, 60\n");
                            // printf("syscall\n");
                            fprintf(fp_asm,"exit:\n");
                            //freopen("CON", "w", stdout);
			    fclose(fp_asm);
		   	    //exit(0);
                        }
                        else
                            printf("\nSemantic Errors are present. No code generation possible...\n");
                    }
                    break;
            default:
                    printf("This option number is not defined. Please re-enter a suitable option\n");
        }
    }
	return 0;
}
