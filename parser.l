%{
#include "y.tab.h"
%}

%option yylineno

letter      [a-zA-Z]
digit       [0-9]
id          {letter}({letter}|{digit}|_)*
string      \"((\\\\)*(\\\"|\\?[^\\"]))*(\\\\)*\"
sout        System\.out\.println
operator    &&|\|\||<|>|<=|>=|==|!=|\*|\/
comment     \/\*([^*]|\**[^*/])*\*+\/
linecmt     \/\/.*
blank       [ \t\r\n]+
lrsquare    \[{blank}?\]

%%

{linecmt}   ;
{comment}   ;
{string}    return STRING_LITERAL;
{blank}     ;

\{          return LEFTBRACE;
\}          return RIGHTBRACE;
\(          return LEFTBRACKET;
\)          return RIGHTBRACKET;
{lrsquare}  return LEFTRIGHTSQUARE;
\[          return LEFTSQUARE;
\]          return RIGHTSQUARE;
\;          return SEMICOLON;
\,          return COMMA;
=           return EQUAL;
!           return NOT;
\+\+        return PLUSPLUS;
\+          return PLUS;
-           return MINUS;
\.          return DOT;

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
{sout}      return SOUT;
length      return LENGTH;
true        return TRUETK;
false       return FALSETK;
this        return THIS;
new         return NEW;

{operator}  return OPERATOR;
{id}        return ID;
{digit}+    return INTEGER_LITERAL;

.           return OTHER;
%%
int yywrap()
{
    return 1;
}

// void main() {
//     int x;
//     while (x = yylex()) {
//         printf("%d\n", x);
//     }
// }
