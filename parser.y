%{
#include <stdio.h>

// int yydebug = 1;

extern FILE *yyin;
extern int yylineno;
extern int yylex(void);
int yyerror(char *msg);
%}

%token ID INTEGER_LITERAL STRING_LITERAL OPERATOR BLANK COMMENT OTHER
%token LEFTBRACE RIGHTBRACE LEFTBRACKET RIGHTBRACKET LEFTSQUARE RIGHTSQUARE LEFTRIGHTSQUARE SEMICOLON COMMA EQUAL NOT PLUS MINUS PLUSPLUS DOT
%token CLASS PUBLIC STATIC VOID MAIN STRING EXTENDS RETURN INT BOOLEAN IF ELSE WHILE SOUL SOUT LENGTH TRUETK FALSETK THIS NEW

%start Program

%%
Program: MainClass ClassDeclList
;
MainClass: CLASS ID LEFTBRACE PUBLIC STATIC VOID MAIN LEFTBRACKET STRING LEFTRIGHTSQUARE ID RIGHTBRACKET LEFTBRACE Statement RIGHTBRACE RIGHTBRACE
;
ClassDeclList: ClassDeclList ClassDecl
|
;
ClassDecl: CLASS ID LEFTBRACE VarDeclList MethodDeclList RIGHTBRACE
| CLASS ID EXTENDS ID LEFTBRACE VarDeclList MethodDeclList RIGHTBRACE
;
VarDeclList: VarDeclList VarDecl
|
;
VarDecl: Type ID SEMICOLON
;
MethodDeclList: MethodDeclList MethodDecl
|
;
MethodDecl: PUBLIC Type ID LEFTBRACKET FormalList RIGHTBRACKET LEFTBRACE VarDeclList StatementList RETURN Exp SEMICOLON RIGHTBRACE
;
FormalList: Type ID FormalRestList
|
;
FormalRestList: FormalRestList FormalRest
|
;
FormalRest: COMMA Type ID
;
PrimeType: INT
| BOOLEAN
| ID
;
Type: PrimeType
| Type LEFTRIGHTSQUARE
;
StatementList: Statement StatementList
|
;
Statement: LEFTBRACE StatementList RIGHTBRACE
| IF LEFTBRACKET Exp RIGHTBRACKET Statement ELSE Statement
| WHILE LEFTBRACKET Exp RIGHTBRACKET Statement
| SOUL LEFTBRACKET Exp RIGHTBRACKET SEMICOLON
| SOUL LEFTBRACKET STRING_LITERAL RIGHTBRACKET SEMICOLON
| SOUT LEFTBRACKET Exp RIGHTBRACKET SEMICOLON
| SOUT LEFTBRACKET STRING_LITERAL RIGHTBRACKET SEMICOLON
| ID EQUAL Exp SEMICOLON
| ID Index EQUAL Exp SEMICOLON
| RETURN Exp SEMICOLON
;
Index: LEFTSQUARE Exp RIGHTSQUARE
| Index LEFTSQUARE Exp RIGHTSQUARE
;
Exp: ExpOp DecoExp
| DecoExp
;
DecoExp: NOT SingleExp
| PLUS SingleExp
| MINUS SingleExp
| SingleExp
;
SingleExp: LEFTBRACKET Exp RIGHTBRACKET
| ID Index
| ID DOT LENGTH
| ID Index DOT LENGTH
| INTEGER_LITERAL
| TRUETK
| FALSETK
| Object
| ID
| Object DOT ID LEFTBRACKET ExpList RIGHTBRACKET
| ID DOT ID LEFTBRACKET ExpList RIGHTBRACKET
;
ExpOp: Exp OPERATOR
| Exp PLUS
| Exp MINUS
;
Object: ID
| THIS
| NEW ID LEFTBRACKET RIGHTBRACKET
| NEW PrimeType Index
;
ExpList: Exp ExpRestList
|
;
ExpRestList: ExpRestList ExpRest
|
;
ExpRest: COMMA Exp
;
%%
int main(int argc, char* argv[])
{
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("Failed to open file\n");
        return 1;
    }
    yyparse();
    fclose(yyin);
    return 0;
}

int yyerror(char *msg)
{
    printf("Syntax errors in %d\n", yylineno);
}
