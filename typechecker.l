%{
#include "typechecker.tab.hpp"
%}

%option yylineno

letter      [a-zA-Z]
digit       [0-9]
id          {letter}({letter}|{digit}|_)*
string      \"((\\\\)*(\\\"|\\?[^\\"]))*(\\\\)*\"
char        \{|\}|\(|\)|\[|\]|\;|\,|=|!|\.|\+|-|\*|\/|>|<
comment     \/\*([^*]|\**[^*/])*\*+\/
linecmt     \/\/.*
blank       [ \t\r\n]+
lrsquare    \[{blank}?\]

%%

{linecmt}   ;
{comment}   ;
{string}    {
        yylval.stringValue = strdup(yytext) + 1;
        yylval.stringValue[strlen(yytext) - 2] = '\0';
        return STRING_LITERAL;
    }
{blank}     ;

\+\+        return PLUSPLUS;
&&          return AND;
\|\|        return OR;
\<=         return LESSEQ;
\>=         return GREATEREQ;
==          return EQUAL;
!=          return NOTEQ;
{lrsquare}  return LRSQUARE;
{char}      return *yytext;

class       return CLASS;
public      return PUBLIC;
static      return STATIC;
void        return VOID;
main        return MAIN;
String      return STRING;
extends     return EXTENDS;
return      return RETURN;
int         return INT;
boolean     return BOOLEAN;
if          return IF;
else        return ELSE;
while       return WHILE;
System      return SYSTEM;
out         return OUT;
println     return PRINTLN;
print       return PRINT;
length      return LENGTH;
this        return THIS;
new         return NEW;

true        { yylval.intValue = 1; return BOOLEAN_LITERAL; }
false       { yylval.intValue = 0; return BOOLEAN_LITERAL; }

{id}        { yylval.stringValue = strdup(yytext); return ID; }
{digit}+    { yylval.intValue = atoi(yytext); return INTEGER_LITERAL; }

.           return *yytext;
%%
int yywrap()
{
    return 1;
}
