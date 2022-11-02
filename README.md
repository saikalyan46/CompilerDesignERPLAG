# Compiler-Design-ERPLAG
- Run the make file.</br>
- Run the generated executable with two arguments, one as the input and other as the output. Ex: ./a.exe input.txt output.txt</br>
- The generated output file is a assembly code, run it in a nasm compiler to generate its equivalent machine executable file</br>

## ERPLAG Specifications
The language ERPLAG is a strongly typed language with primitive data types as integer and floating
point. It also supports two other data types: boolean and arrays. The language supports arithmetic and boolean
expressions. The language supports assignment statements, input/output statements, declarative, conditional,
iterative and function call statements. The scope of the variables is static and the variable is visible only in the block where it is declared.

## 1. Lexical structure
### 1.1 Keywords and identifiers
The reserved keywords are declare, driver, program, for, start, end, module, get_value, print, use,
with, parameters, true, false, takes, input, returns, AND, OR, switch, case, break, default and while. A
complete list of tokens for the keywords is given in table 1. An identifier is a sequence of letters, digits and an
underscore starting with a lower or upper case letter in a-z. Regular expression for an identifier is [a-z|A-Z][a-
z|A-Z|0-9|_]*. Identifiers can be at most 20 characters long. The language is case sensitive. This means that the
lexeme switch is considered as a keyword which is different from SWITCH (use of uppercase) or Switch(use of
uppercase in S only) which should be tokenized as identifiers. The lexemes Value and value are different from
each other and represent different variables. An identifier is tokenised as ID. Valid identifiers are computeSum,
C, A_1, a2cD2___, abc_123, abc123, etc. The names 1abc, _Abc are not valid identifiers. Identifiers are
separated from keywords through a white space.
### 1.2 White Spaces, separator and comments
The white spaces are blanks, tabs and new line characters. These are used to separate the tokens. Any
number of white spaces is ignored and need not be tokenized. Keywords and identifiers must be separated by a
white space or any other token which is different from keyword and identifiers. For example, valueabc is a
single identifier, while value:integer is a stream of lexemes value, :, and integer and are tokenized as ID, a
COLON and INTEGER respectively. A white space can be a blank character, a tab or a new line character.
Therefore, value
:
integer is also the same sequence of tokens as described above. A white space
between two sequences of characters as in val ue makes them different than value. These will be tokenized as
ID ID and not as one token ID. A semicolon ; separates the statements from each other. A comment starts with
** and ends with **.
### 1.3 Numbers
An integer number is a sequence of digits. The numbers 234, 1,45, 90123 etc. represent integers and are
tokenised as NUM. The type of the integer numbers is integer. A floating point number can be either a
sequence of digits followed by a decimal point, followed by the fraction part of it again as a continuous
sequence of digits, for example, numbers 23.89, 908.567 and 25.0 are valid floating point numbers, but 26. is
not because there is no digit after the decimal point. Also, the number 0.25 is valid but .25 is not. These
numbers can also be represented in mantissa and exponent form, for example, 123.2E+6, 124.2E-1, 124.2e-1
etc. E can be both in upper case and lower case. Signs are optional and if no sign is used with E, the exponent
value should be assumed as positive. The floating point number will not start with a decimal point, for example,
.124E+2 is not valid while 1.24E+1 is a valid lexeme. A floating point number is of data type real and is
tokenised as RNUM.

1.4 Operators
The arithmetic operations are recognized by the lexemes +, -, *, / in their usual meaning as plus, minus,
multiplication and division respectively and can be applied on operands of integer and real number types.
However, the array variables themselves cannot be added or subtracted while the elements of array can be used
as operands of these operators (to be explained in detail later). The relational operators are simply <, <=, >, >=,
== and !=, known in their usual meaning as less than, less than or equal to, greater than, greater than or
equal to, equal to and not equal to respectively. The logical and and or operations are permissible and the
lexemes AND and OR (only in uppercase letters) are valid to be recognized as these two operations.
A special operator .. (dot dot) represents the range in defining the array range. An array is declared as
declare C:array[1..10] of integer; The pair of these dots is different from the dot appearing in the lexeme of
floating point number. The assignment operator is := and is tokenised as ASSIGNOP. The tokens
corresponding to these are given in table 2.
## 2. Language Features
### 2.1. Data Types
ERPLAG supports four data types-integer, real, boolean and array. The supported primitive data types
are two, integer and floating point numbers represented by the types integer and real. The language also
supports a boolean data type. The variables of boolean data type can attain one of the two values TRUE and
FALSE. A conditional expression which evaluates to true or false is also of boolean type.
A constructed data type is a single dimensional array, defined over a range of indices. An array can be
of elements of any of the above three types supported by this language. For instance, the declaration declare C:
array[5..10] of real; represents the identifier name C as an array of real numbers and is of size 6 where the
array elements are accessed using constructed names C[5], C[6], C[7], C[8], C[9] and C[10] . A range can start
with an integer number (e.g. 5) and end with a positive integer (e.g. 10). The array can also be declared as a
dynamic array using variable identifiers as range values. For example: declare C: array [a..b] of real; or
declare B:array [5..b] of real; where a and b are of integer type. The address computation and type checking
for elements of dynamic arrays are done at run time. This means that your compiler should generate the
assembly language code for such dynamic checks which eventually are performed when the translated user
code is executed. The array elements can be accessed using an integer constant or an integer variable e.g. C[5],
C[u], C[k], C[10] etc. The bound checking for C[k] is done at run time. While accessing an array element, its
index should always be taken as a positive integer or an identifier. If there are three array variables P, Q and R,
declared as declare P, Q, R: array[3..9] of real; , then operations such as P+Q, P-Q, P*Q and P/Q are not
allowed. But, P[7]+Q[3], P[4]/3, Q[5]*P[4] are valid for array elements. However, an array variable (not its
elements) can be assigned to another structurally equivalent array variable as in P := Q;. This makes P to refer
to Q's elements and not its own. If a prior assignment of P to another variable R using R := P; was made before
its reassignment to elements of Q, then all the elements of P could be accessed using R, else this will be treated
as memory leak for an amount equal to the memory allocated to all elements of P. The language supports
structural type equivalence for arrays and other permissible identifiers. More details on type checking rules will
be posted later.
### 2.2.Expressions
The expressions are of two types: arithmetic and boolean. An arithmetic expression is a usual infix
expression. The precedence of operators * and / is high over + and -, while * and / are of the same precedence
and + and – are of the same precedence. For example, an arithmetic expression 15+29-12*2 evaluates to 20. A
parenthesis pair has the largest precedence. For instance 15 + (29-12)*2 is computed as 49. The operators are
left associative.
A boolean expression is an expression obtained by the conjunction of the arithmetic expression through
the relational operators (<,<=, >, >=, ==, or !=). Two or more boolean expressions when ANDed or ORed,
through the operators AND and OR, also compute to the values true or false. Example: ((x>=10 AND y<0) OR
x<10) evaluates to true for values of x and y as 6 and -10 respectively. The static values of TRUE and FALSE
cannot be taken as 1 and 0 as is usually assumed with C programming language.
2.3.Statements
The language supports five types of statements, declarative, simple, input/output statements,
conditional and iterative statements. Declarative statements declare variables (identifiers) of defined type .
As the language is strongly typed, each variable must have a type associated with it. The expression comprising
of a number of variables also has a type based on the context.
A declaration statement, declare a, b, c:integer; declares the names (identifiers) a, b and c to be of
type integer. A declaration statement can appear anywhere in the program and is valid for any use thereafter
with in its scope. An identifier cannot be declared multiple times in the same scope. Also, an identifier must be
declared before its use.
A simple statement has the following structure
<left value> := <right expression>
A left value can be a simple identifier or a constructed expression for accessing an array element say
A[i] = x+y; assigns value of the right hand side expression to the i th element of the array A. The right hand side
expression can be a function call and correspondingly the left hand side changes to a list of actual parameters as
given below
[r,m] := use mod1 with parameters v, w;
The input statement get_value(v); intends to read value from the keyboard and associate with the
variable v. The statement print(v); intends to write the value of variable v on the monitor.
The only conditional statement supported by this language is the C-like switch-case statement. There
is no statement of C-like if. The switch applies to both integers and boolean numbers, but is not applicable to
real numbers. A switch statement with an identifier of type real is not valid and an error should be reported. A
switch statement with an integer type identifier associated with it, can have case statement with case keyword
followed by an integer only and the case statements must be followed by a default statement.
Any boolean expression consisting of integers or real numbers equates to boolean TRUE or FALSE.
A C-like if statement is not supported by ERPLAG but can be constructed using the switch case
  statement. Consider a C-like if statement</br>
&nbsp;&nbsp;&nbsp;&nbsp;if(boolean condition)</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;statements S1;</br>
&nbsp;&nbsp;&nbsp;&nbsp;else</br>
&nbsp;&nbsp;&nbsp;&nbsp;statements S2;</br>
This if-statement can be equivalently coded in ERPLAG as follows</br>
&nbsp;&nbsp;&nbsp;&nbsp;declare flag : boolean;</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;flag = <boolean condition>;</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;switch(flag)</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;start</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;case TRUE</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:<statements S1>;</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;break;</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;case FALSE</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:<statements S2>;</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;break;</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;end</br>
The switch statement with a boolean identifier must have cases with values TRUE and FALSE. There is
no default value associated with a switch statement with a boolean identifier. A switch statement with an integer
value can have any number of case statements and a default statement must follow these case statements. The
case statements are separated by a break.
The iterative statements supported are for and while loops. The for loop iterates over the range of
positive integer values. The execution termination is controlled by the range itself. There is no other guard
condition controlling the execution in the for loop construct. An example is as follows.</br>
&nbsp;&nbsp;&nbsp;&nbsp;for( k in 2..8)</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;start</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;x=x+2*k;</br>
&nbsp;&nbsp;&nbsp;&nbsp;end</br>
The value of k is not required to be incremented by the user. The range itself takes care of this through
your compiler implementation. The above piece of ERPLAG code produces x (say for its initial value as 5) as
9,15, 23, 33, 45, 59, 75 across the iterations. A for statement must not redefine the variable that participates in
the iterating over the range.
The while loop iterates over a code segment and is controlled by a guard condition. Any one of the
variables used in the boolean expression used as conditional check for the while loop must be assigned a value
to ensure the change during the iteration. For example, the following while code is correct as the variable i (one
of the two variables i and k) has been assigned a value through the assignment statement i:=i+1;</br>
&nbsp;&nbsp;&nbsp;&nbsp;while(i<=k)</br>
&nbsp;&nbsp;&nbsp;&nbsp;start</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;get_value(tempvar);</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;arr1[i]:=tempvar;</br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;i:=i+1;</br>
&nbsp;&nbsp;&nbsp;&nbsp;end</br>

## 3. TOKEN LIST
The lexemes with following patterns are tokenized with corresponding token names. Lexemes AND and
OR are in upper case letter while all other lexemes are in lower case letters. Token names are represented in
upper case letters.
### Table 1: Keywords
| Pattern | Token |
| --- | --- |
| integer | INTEGER |
| real | REAL |
| boolean | BOOLEAN |
| of | OF |
| array | ARRAY |
| start | START |
| end | END |
| declare | DECLARE |
| module | MODULE |
| driver | DRIVER |
| program | PROGRAM |
| record | RECORD |
| tagged | TAGGED |
| union | UNION |
| get_value | GET_VALUE |
| print | PRINT |
| use | USE |
| with | WITH |
| parameters | PARAMETERS |
| true | TRUE |
| false | FALSE |
| takes | TAKES |
| input | INPUT |
| returns | RETURNS |
| AND | AND  |
| OR | OR |
| for | FOR |
| in | IN |
| switch | SWITCH |
| case | CASE |
| break | BREAK |
| default | DEFAULT |
| while | WHILE |

### Table 2. Symbols
| Pattern | Token |
| --- | --- |
| + | PLUS |
| - | MINUS |
| * | MUL |
| / | DIV |
| < | LT |
| <= | LE |
| >= | GE |
| > | GT |
| == | EQ |
| != | NE |
| << | DEF |
| >> | ENDDEF |
| : | COLON |
| .. | RANGEOP |
| ; | SEMICOL |
| , | COMMA |
| := | ASSIGNOP |
| [ | SQBO |
| ] | SQBC |
| ( | BO |
| ) | BC |
| ** | COMMENTMARK |

## 4. Grammar :
Start Symbol : \<program\> </br>
\<program\> --> \<moduleDeclarations\> \<otherModules\>\<driverModule\>\<otherModules\></br>
\<moduleDeclarations\> --> \<moduleDeclaration\>\<moduleDeclarations\> | ε</br>
\<moduleDeclaration> --> DECLARE MODULE ID SEMICOL</br>
\<otherModules\> --> \<module\>\<otherModules\> | ε</br>
\<driverModule\> --> DEF DRIVER PROGRAM ENDDEF \<moduleDef\></br>
\<module\> --> DEF MODULE ID ENDDEF TAKES INPUT SQBO \<input_plist\> SQBC SEMICOL \<ret\> \<moduleDef\></br>
\<ret\> --> RETURNS SQBO \<output_plist\> SQBC SEMICOL | ε</br>
\<input_plist\> --> \<input_plist\> COMMA ID COLON \<dataType\> | ID COLON \<dataType\></br>
\<output_plist\> --> \<output_plist\> COMMA ID COLON \<type\> | ID COLON \<type\></br>
\<dataType\> --> INTEGER | REAL | BOOLEAN | ARRAY SQBO \<range\> SQBC OF \<type\></br>
\<type\> --> INTEGER | REAL | BOOLEAN</br>
\<moduleDef\> --> START \<statements\> END</br>
\<statements\> --> \<statement\> \<statements\> | ε</br>
\<statement\> --> \<ioStmt\> | \<simpleStmt\> | \<declareStmt\> | \<condionalStmt\> | \<iterativeStmt\></br>
\<ioStmt\> --> GET_VALUE BO ID BC SEMICOL | PRINT BO \<var\> BC SEMICOL</br>
\<var\> --> ID \<whichId\> | NUM | RNUM</br>
\<whichId\> --> SQBO ID SQBC | ε</br>
\<simpleStmt\> --> \<assignmentStmt\> | <moduleReuseStmt></br>
\<assignmentStmt\> --> ID \<whichStmt\></br>
\<whichStmt\> --> \<lvalueIDStmt\> | \<lvalueARRStmt\></br>
\<lvalueIDStmt\> --> ASSIGNOP \<expression\> SEMICOL</br>
\<lvalueARRStmt\> --> SQBO \<index\> SQBC ASSIGNOP \<expression\> SEMICOL</br>
\<index\> --> NUM | ID</br>
\<moduleReuseStmt\> --> \<optional\> USE MODULE ID WITH PARAMETERS \<idList\> SEMICOL</br>
\<optional\> --> SQBO \<idList\> SQBC ASSIGNOP | ε</br>
\<idList\> --> \<idList\> COMMA ID | ID</br>
\<expression\> --> \<arithmeticExpr\> | \<booleanExpr\></br>
\<arithmeticExpr\> --> \<arithmeticExpr\> \<op\> \<term\></br>
\<arithmeticExpr\> --> \<term\></br>
\<term\> --> \<term\> \<op\> \<factor\></br>
\<term\> --> \<factor\></br>
\<factor\> --> BO \<arithmeticExpr\> BC</br>
\<factor\> --> \<var\></br>
\<op\> --> PLUS | MINUS | MUL | DIV</br>
\<booleanExpr\> --> \<booleanExpr\> \<logicalOp\> \<booleanExpr\></br>
\<logicalOp\> --> AND | OR</br>
\<booleanExpr\> --> \<arithmeticExpr\> \<relationalOp\> \<arithmeticExpr\></br>
\<booleanExpr\> --> BO \<booleanExpr\> BC</br>
\<relationalOp\> --> LT | LE | GT | GE | EQ | NE</br>
\<declareStmt\> --> DECLARE \<idList\> COLON \<dataType\> SEMICOL</br>
\<condionalStmt\> --> SWITCH BO ID BC START \<caseStmt\> \<default\> END</br>
\<caseStmt\> --> CASE \<value\> COLON \<statements\> BREAK SEMICOL \<caseStmt\></br>
\<value\> --> NUM | TRUE | FALSE</br>
\<default\> --> DEFAULT COLON \<statements\> BREAK SEMICOL | ε</br>
\<iterativeStmt\> --> FOR BO ID IN \<range\> BC START \<statements\> END | WHILE BO \<booleanExpr\> BC START \<statements\> END</br>
\<range\> --> NUM RANGEOP NUM</br>
