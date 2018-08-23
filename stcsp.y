// useful guide: http://preserve.mactech.com/articles/mactech/Vol.16/16.07/UsingFlexandBison/index.html

%define parse.error verbose

%token STATEMENT RANGE LIST
%token VAR OBJ ARR
%token LE_CON GE_CON EQ_CON NE_CON IMPLY_CON UNTIL_CON
%token LT_OP GT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP NOT_OP
%token AT FIRST NEXT FBY IF THEN ELSE
%token ABS

%{
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <sys/resource.h>
#include <map>
#include <vector>
#include <unordered_set>

#include "include/types.h"
#include "include/Variable.h"

#include "include/Expression.h"
#include "include/expressions/AddExpression.h"
#include "include/expressions/specialExpressions/VariableExpression.h"
#include "include/expressions/specialExpressions/ConstantExpression.h"

#include "include/Constraint.h"
#include "include/constraints/specialConstraints/EqualConstraint.h"

domain_t constructDomain(int lower, int upper);

// PERHAPS IN THE FUTURE MAKE A GLOBAL SOLVER, AND ADD TO IT INSTEAD OF ADDING TO THESE. SHOULD PROBABLY DO THAT
struct cmp_str
{
   bool operator()(char const *a, char const *b)
   {
      return strcmp(a, b) == 0;
   }
};
// maps a string identifier to its variable instance
std::map<char *, Variable *, cmp_str> variableMap;
std::vector<Constraint *> constraintList;

extern "C"
{
    int yyparse(void);
    int yylex(void);
    int yywrap();
}

void yyerror(const char *msg);

int line_num = 1;
int my_argc = 0;
char **my_argv = NULL;
%}

%union {
    char *str;
    int num;
    void *ptr;
}

%token <str> IDENTIFIER
%token <num> CONSTANT
%type <ptr> constraint
%type <ptr> expression_statement

%type <ptr> primary_expression
%type <ptr> binary_expression

%start program_statement

%%

program_statement
	: program_statement declaration_statement
	| program_statement constraint_statement
	| declaration_statement
	| constraint_statement
	;

// places variable in global map
declaration_statement
    : VAR IDENTIFIER ':' '[' CONSTANT ',' CONSTANT ']' ';' { variableMap[$2] = new Variable(constructDomain($5, $7)); }
    ;

// I'm not having these end in ';' bc expressions should only appear within constraints, which themselves end in ';'
expression_statement
    : primary_expression
//    | unary_expression
    | binary_expression
//    | ternary_expression
    ;

primary_expression
    : IDENTIFIER { $$ = new VariableExpression(variableMap[$1]); }
    | CONSTANT { $$ = new ConstantExpression($1); }
    | '(' expression_statement ')' { $$ = $2; }
    ;

binary_expression
    : primary_expression '+' primary_expression { $$ = new AddExpression((Expression *)$1, (Expression *)$3); }
    ;

//logical_or_expression
//    : logical_and_expression { $$ = $1; }
//    | logical_or_expression OR_OP logical_and_expression { $$ = basicNodeNew(OR_OP, $1, $3); }
//    ;

//logical_and_expression
//    : equality_expression { $$ = $1; }
//    | logical_and_expression AND_OP equality_expression { $$ = basicNodeNew(AND_OP, $1, $3); }
//    ;

//equality_expression
//    : relational_expression { $$ = $1; }
//    | equality_expression EQ_OP relational_expression { $$ = basicNodeNew(EQ_OP, $1, $3); }
//    | equality_expression NE_OP relational_expression { $$ = basicNodeNew(NE_OP, $1, $3); }
//    ;

//relational_expression
//    : additive_expression { $$ = $1; }
//    | relational_expression relational_operator additive_expression { $$ = basicNodeNew($2, $1, $3); }
//    ;

//relational_operator
//    : LT_OP { $$ = LT_OP; }
//    | GT_OP { $$ = GT_OP; }
//    | LE_OP { $$ = LE_OP; }
//    | GE_OP { $$ = GE_OP; }
//    ;

//additive_expression
//    : multiplicative_expression { $$ = $1; }
//    | additive_expression '+' multiplicative_expression { $$ = basicNodeNew('+', $1, $3); }
//    | additive_expression '-' multiplicative_expression { $$ = basicNodeNew('-', $1, $3); }
//    ;

//multiplicative_expression
//    : at_expression { $$ = $1; }
//    | multiplicative_expression '*' at_expression { $$ = basicNodeNew('*', $1, $3); }
//    | multiplicative_expression '/' at_expression { $$ = basicNodeNew('/', $1, $3); }
//    | multiplicative_expression '%' at_expression { $$ = basicNodeNew('%', $1, $3); }
//    ;

//at_expression
//    : fby_expression { $$ = $1;}
//    | fby_expression AT CONSTANT { $$ = nodeNew(AT, NULL, $3, 0, $1, NULL); }
//    ;

//fby_expression
//    : unary_expression { $$ = $1; }
//    | unary_expression FBY fby_expression { $$ = basicNodeNew(FBY, $1, $3); }
//    ;

//unary_expression
//    : primary_expression { $$ = $1; }
//    | FIRST unary_expression { $$ = basicNodeNew(FIRST, NULL, $2); }
//    | NEXT unary_expression { $$ = basicNodeNew(NEXT, NULL, $2); }
//    | IF expression THEN expression ELSE unary_expression { $$ = basicNodeNew(IF, $2, basicNodeNew(THEN, $4, $6)); }
//    | ABS unary_expression { $$ = basicNodeNew(ABS, NULL, $2); }
//    ;

// just a wrapper to cause this code to execute for all constraints (add them to the global map)
constraint_statement
    : constraint { constraintList.push_back((Constraint *)$1); }

constraint
    : expression_statement EQ_CON expression_statement ';' { $$ = new EqualConstraint((Expression *)$1, (Expression *)$3); }
//    | expression '<' expression ';' { $$ = new ; }
//    | expression '>' expression ';' { $$ = (int)'>'; }
//    | expression LE_CON expression ';' { $$ = LE_CON; }
//    | expression GE_CON expression ';' { $$ = GE_CON; }
//    | expression NE_CON expression ';' { $$ = NE_CON; }
//    | expression UNTIL_CON expression ';' { $$ = UNTIL_CON; }
//    | expression IMPLY_CON expression ';' { $$ = IMPLY_CON; }
    ;

%%

extern FILE *yyin;

int main(int argc, char *argv[]) {
    //setting the memory to be unlimit
    #undef YYMAXDEPTH
    #define YYMAXDEPTH 100000
    /*
    struct rlimit x;
    if (getrlimit(RLIMIT_STACK, &x) < 0)
        perror("getrlimit");
    x.rlim_cur = RLIM_INFINITY;
    if (setrlimit(RLIMIT_STACK, &x) < 0)
        perror("setrlimit");
    */
    
    int i;
    char *filename;

    my_argc = argc;
    my_argv = argv;

    filename = NULL;
    i = 1;
    while (filename == NULL && i < argc) {
        if (argv[i][0] != '-') {
            filename = argv[i];
        }
        i++;
    }

    if (filename != NULL) {
        yyin = fopen(filename, "r");
    }

    yyparse();

    if (filename != NULL) {
        fclose(yyin);
    }
    //std::cout<<"num variables: "<<variableMap.size()<<", num constraints: "<<constraintList.size()<<"\n";
    printf("num variables: %d, num constraints: %d\n", variableMap.size(), constraintList.size());

    return 0;
}

void yyerror(const char *msg) {
    fprintf(stdout, "Line %d: %s\n", line_num, msg);
    exit(1);
}

std::unordered_set<int> constructDomain(int lower, int upper) {
    std::unordered_set<int> domain;
    for (int i=lower;i<=upper;i++) {
        domain.insert(i);
    }
    return domain;
}