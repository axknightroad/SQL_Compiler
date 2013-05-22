/*
*SQL词法分析器
*by axknightroad
*2013/5/10
*/
%option noyywrap nodefault yylineno case-insensitive
%{
    #include "psql.tab.h"
    #include <stdarg.h>
    #include <string.h>

    void yyerror(char *s, ...);
    
%}
%s BTWMODE


%%

/* 关键字 */
CREAT            {return CREAT;}
DATABASE         {return DATABASE;}
DATABASES        {return DATABASES;}
SHOW             {return SHOW;}
DROP             {return DROP;}
USE              {return USE;}
TABLE            {return TABLE;}
INSERT           {return INSERT;}
INTO             {return INTO;}
VALUES           {return VALUES;}
SELECT           {return SELECT;}
FROM             {return FROM;}
WHERE            {return WHERE;}
/* 处理BETWEEN...AND... */
BETWEEN          {BEGIN BTWMODE; return BETWEEN;}
<BTWMODE>AND     {BEGIN INITIAL; return AND;}
AND              {return ANDOP;}
OR               {return OR;}
NOT              {return NOT;}
DELETE           {return DELETE;}
UPDATE           {return UPDATE;}
SET              {return SET;}
SCHEMA           {return SCHEMA;}
CASCADE          {return CASADE;}
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
NULL             {return NULL;}
IN               {return IN;}
LEFT             {return LEFT;}
RIGHT            {return RIGHT;}
OUT              {return OUT;}
JOIN             {return JOIN;}
ANY              {return ANY;}
/* 避免NOT和NOT EXISTS的移进/归约冲突 */
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





/* 数字 */
-?[0-9]+                       { yylval.intval=atoi(yytext); return INTNUM;}
-?[0-9]+"."[0-9]*|
-?[0-9]+E[-+]?[0-9]+|
-?[0-9]+"."[0-9]*E[-+]?[0-9]+  {yylval.floatval=atof(yytext); return APPROXNUM;}

/* 布尔值 */
TRUE      {yylval.intval=1; return BOOL;}
UNKNOWN   {yylval.intval=-1; return BOOL;}
FALSE     {yylval.intval=0; return BOOL;}

/* 字符串 */
'(''|[^'\n])*'    {yylval.strval=strdup(yytext);return STRING;}
'(''|[^'\n])*$    {yyerror("Unterminated string %s",yytext);}

/*操作符*/

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
":="       {return ASSIGN;}

/* 函数 */
COUNT {int c=input();unput(c);
       if(c=='(') return FCOUNT;
       yylval.strval=strdup(yytext);
	     return NAME;
      }

/* 名字 */
[A-Za-z][A-Za-z0-9_]*  {yylval.strval=strdup(yytext); return NAME;}

/* 其他一切 */
[ \t\n]                /*whitespace*/
.                      {yyerror("mystery character '%c'",*yytext);}

%%
