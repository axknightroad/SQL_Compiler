/*
*SQL词法分析器
*by axknightroad
*2013/5/10
*/
%option noyywrap nodefault yylineno case-insensitive
%{
    #include "sql.parsing.tab.h"
    #include <stdarg.h>
    #include <string.h>

    void yyerror(char *s, ...);


%}
%s BTWMODE


%%

CREATE           {return CREATE;}
DATABASE         {return DATABASE;}
DATABASES        {return DATABASES;}
SHOW             {return SHOW;}
DROP             {return DROP;}
USE              {return USE;}
TABLE            {return TABLE;}
TABLES           {return TABLES;}
INSERT           {return INSERT;}
INTO             {return INTO;}
VALUES           {return VALUES;}
SELECT           {return SELECT;}
FROM             {return FROM;}
WHERE            {return WHERE;}

BETWEEN          {BEGIN BTWMODE; return BETWEEN;}
<BTWMODE>AND     {BEGIN INITIAL; return AND;}
AND              {return ANDOP;}
OR               {return OR;}
XOR              {return XOR;}
NOT              {return NOT;}
DELETE           {return DELETE;}
UPDATE           {return UPDATE;}
SET              {return SET;}
SCHEMA           {return DATABASE;}
SCHEMAS          {return DATABASES;}
CASCADE          {return CASCADE;}
RESTRICT         {return RESTRICT;}
CHAR             {return CHAR;}
VARCHAR          {return VARCHAR;}
INT              {return INT;}
SMALLINT         {return SMALLINT;}
NUMERIC          {return NUMERIC;}
REAL             {return REAL;}
DOUBLE           {return DOUBLE;}
PRECISION        {return PRECISION;}
FLOAT            {return FLOAT;}
DATE             {return DATE;}
TIME             {return TIME;}
ADD              {return ADD;}
ALTER            {return ALTER;}
COLUMN           {return COLUMN;}
UNIQUE           {return UNIQUE;}
CLUSTER          {return CLUSTER;}
INDEX            {return INDEX;}
ON               {return ON;}
ALL              {return ALL;}
DISTINCT         {return DISTINCT;}
GROUP            {return GROUP;}
BY               {return BY;}
HAVING           {return HAVING;}
ORDER            {return ORDER;}
ASC              {return ASC;}
DESC             {return DESC;}
LIKE             {return LIKE;}
NULL             {return NULLX;}
IN               {return IN;}
LEFT             {return LEFT;}
RIGHT            {return RIGHT;}
OUT              {return OUT;}
JOIN             {return JOIN;}
ANY              {return ANY;}
SOME             {return SOME;}

EXISTS              {yylval.subtok=0; return EXISTS;}
NOT[ \t\n]+EXISTS   {yylval.subtok=1; return EXISTS;}
UNION            {return UNION;}
INTERSECT        {return INTERSECT;}
EXCEPT           {return EXCEPT;}
VIEW             {return VIEW;}
AS               {return AS;}
WITH             {return WITH;}
CHECK            {return CHECK;}
OPTION           {return OPTION;}
DISTINCTROW      {return DISTINCTROW;}






-?[0-9]+                       { yylval.intval=atoi(yytext); return INTNUM;}
-?[0-9]+"."[0-9]* |
-?[0-9]+E[-+]?[0-9]+ |
-?[0-9]+"."[0-9]*E[-+]?[0-9]+  {yylval.floatval=atof(yytext); return APPROXNUM;}


TRUE      {yylval.intval=1; return BOOL;}
UNKNOWN   {yylval.intval=-1; return BOOL;}
FALSE     {yylval.intval=0; return BOOL;}


'(''|[^'\n])*'    {yylval.strval=strdup(yytext);return STRING;}
'(''|[^'\n])*$    {yyerror("Unterminated string %s",yytext);}



[-+&~|^/%*(),.;!]   {return yytext[0];}
"&&"                {return ANDOP;}
"||"                {return OR;}

"="        {yylval.subtok=4;return COMPARISON;}
"<=>"      {yylval.subtok=12;return COMPARISON;}
">="       {yylval.subtok=6;return COMPARISON;}
">"        {yylval.subtok=2;return COMPARISON;}
"<="       {yylval.subtok=5;return COMPARISON;}
"<"        {yylval.subtok=1;return COMPARISON;}
"!="|"<>"  {yylval.subtok=3;return COMPARISON;}
"!<"       {yylval.subtok=7;return COMPARISON;}
"!>"       {yylval.subtok=8;return COMPARISON;}
"<<"       {yylval.subtok=1;return SHIFT;}
">>"       {yylval.subtok=2;return SHIFT;}



COUNT {int c=input();unput(c);
       if(c=='(') return FCOUNT;
	     yylval.strval=strdup(yytext);
	     return NAME;
      }


[A-Za-z][A-Za-z0-9_]*  {yylval.strval=strdup(yytext); return NAME;}


[ \t\n]                /*whitespace*/
.                      {yyerror("mystery character '%c'",*yytext);}

%%
