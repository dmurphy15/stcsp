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

#include "../../include/all.h"

domain_t constructDomain(int lower, int upper);

// PERHAPS IN THE FUTURE MAKE A GLOBAL SOLVER, AND ADD TO IT INSTEAD OF ADDING TO THESE. SHOULD PROBABLY DO THAT
struct cmp_str
{
   bool operator()(char const *a, char const *b)
   {
      return strcmp(a, b) < 0;
   }
};
// maps a string identifier to its variable instance
std::map<char *, Variable *, cmp_str> variableMap;
Solver s(BC_NODE, 2);

//#extern "C"
//{
    int yyparse(void);
    int yylex(void);
    int yywrap();
//}

void yyerror(const char *msg);

int line_num = 1;
int my_argc = 0;
char **my_argv = NULL;
%}


%union {
    char *str;
    int num;
    Expression *expr;
}

%token <str> IDENTIFIER
%token <num> CONSTANT

%type <expr> expression
%type <expr> logical_or_expression
%type <expr> logical_and_expression
%type <expr> equality_expression
%type <expr> relational_expression
%type <expr> additive_expression
%type <expr> multiplicative_expression
%type <expr> at_expression
%type <expr> fby_expression
%type <expr> unary_expression
%type <expr> primary_expression


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
    : VAR IDENTIFIER ':' '[' CONSTANT ',' CONSTANT ']' ';' { variableMap[$2] = new Variable(constructDomain($5, $7), $2); }
    ;

constraint_statement
    : expression '<' expression ';' { s.addConstraint(*new LTConstraint(*$1, *$3)); }
    | expression '>' expression ';' { s.addConstraint(*new GTConstraint(*$1, *$3)); }
    | expression LE_CON expression ';' { s.addConstraint(*new LEConstraint(*$1, *$3)); }
    | expression GE_CON expression ';' { s.addConstraint(*new GEConstraint(*$1, *$3)); }
    | expression EQ_CON expression ';' { s.addConstraint(*new EqualConstraint(*$1, *$3)); }
    | expression NE_CON expression ';' { s.addConstraint(*new NEQConstraint(*$1, *$3)); }
    | expression UNTIL_CON expression ';' { s.addConstraint(*new UntilConstraint(*$1, *$3)); }
    | expression IMPLY_CON expression ';' { s.addConstraint(*new ImplyConstraint(*$1, *$3)); }
    ;

expression
    : logical_or_expression { $$ = $1; }
    ;

logical_or_expression
    : logical_and_expression { $$ = $1; }
    | logical_or_expression OR_OP logical_and_expression { $$ = new LOrExpression(*$1, *$3); }
    ;

logical_and_expression
    : equality_expression { $$ = $1; }
    | logical_and_expression AND_OP equality_expression { $$ = new LAndExpression(*$1, *$3); }
    ;

equality_expression
    : relational_expression { $$ = $1; }
    | equality_expression EQ_OP relational_expression { $$ = new EqualExpression(*$1, *$3); }
    | equality_expression NE_OP relational_expression { $$ = new NEQExpression(*$1, *$3); }
    ;

relational_expression
    : additive_expression { $$ = $1; }
    | relational_expression LT_OP additive_expression { $$ = new LTExpression(*$1, *$3); }
    | relational_expression GT_OP additive_expression { $$ = new GTExpression(*$1, *$3); }
    | relational_expression LE_OP additive_expression { $$ = new LEExpression(*$1, *$3); }
    | relational_expression GE_OP additive_expression { $$ = new GEExpression(*$1, *$3); }
    ;

additive_expression
    : multiplicative_expression { $$ = $1; }
    | additive_expression '+' multiplicative_expression { $$ = new AddExpression(*$1, *$3); }
    | additive_expression '-' multiplicative_expression { $$ = new SubtractExpression(*$1, *$3); }
    ;

multiplicative_expression
    : at_expression { $$ = $1; }
    | multiplicative_expression '*' at_expression { $$ = new MultiplyExpression(*$1, *$3); }
    | multiplicative_expression '/' at_expression { $$ = new DivideExpression(*$1, *$3); }
    | multiplicative_expression '%' at_expression { $$ = new ModExpression(*$1, *$3); }
    ;

at_expression
    : fby_expression { $$ = $1;}
    | fby_expression AT CONSTANT { $$ = new AtExpression(*$1, *new ConstantExpression($3)); }
    ;

fby_expression
    : unary_expression { $$ = $1; }
    | unary_expression FBY fby_expression { $$ = new FbyExpression(*$1, *$3); }
    ;

unary_expression
    : primary_expression { $$ = $1; }
    | FIRST unary_expression { $$ = new FirstExpression(*$2); }
    | NEXT unary_expression { $$ = new NextExpression(*$2); }
    | IF expression THEN expression ELSE unary_expression { $$ = new IfThenElseExpression(*$2, *$4, *$6); }
    | ABS unary_expression { $$ = new AbsExpression(*$2); }
    | NOT_OP unary_expression { $$ = new NotExpression(*$2); }
    ;

primary_expression
    : IDENTIFIER { $$ = new VariableExpression(*variableMap.at($1)); }
    | CONSTANT { $$ = new ConstantExpression($1); }
    | '(' expression ')' { $$ = $2; }
    ;

%%

#include <iostream>
#include <chrono>

using namespace std;

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

	auto start = chrono::system_clock::now();
	s.solve();
	auto ending = chrono::system_clock::now();
	std::chrono::duration<double> elapsed = ending - start;
	s.printTree();
	s.writeGraph();
	cout<<"Time elapsed; "<< elapsed.count() <<"\n";

    return 0;
}

void yyerror(const char *msg) {
    fprintf(stdout, "Line %d: %s\n", line_num, msg);
    exit(1);
}

domain_t constructDomain(int lower, int upper) {
    domain_t ret;
    for (int i=lower;i<=upper;i++) {
        ret.insert(i);
    }
    return ret;
}