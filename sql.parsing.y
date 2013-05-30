/*
*SQL语法分析器
*by axknightroad
*2013/05/22
*/

%{
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include "sql.struct.h"

  void yyerror(char *s,...);
  void emit(char *s,...);
%}

%union {
  /*基本数据类型*/
  int intval;
  double floatval;
  char *strval;
  int subtok;
  
  /*select语法树类型*/
  struct Conditions *con_val;
  struct Select_fields *sf_val;
  struct Select_tables *st_val;
  struct Groupby *gb_val;
  struct Select_struct *ss_val;
  struct Val_list_struct *vl_val;
  
  /*delete类型*/
  struct Delete_struct *ds_val;
  
  /*insert类型*/
  struct Column_list_struct *cl_val;
  struct Insert_val_list_struct *ivl_val;
  struct Insert_struct *is_val;
  
  /*update类型*/
  struct Update_asgn *ua_val;
  struct Update_struct *us_val;
 
  /*create类型*/
  enum Type ty_val;
  struct Create_field  *cf_val;
  struct Create_fields *cfs_val;
  struct Create_struct *cs_val;
}

/* 名字和字面值 */
%token <strval> NAME
%token <strval> STRING
%token <intval> INTNUM
%token <intval> BOOL
%token <floatval> APPROXNUM


/* 操作符和优先级 */
%left OR
%left XOR
%left ANDOP
%noassoc IN IS LIKE
%left NOT '!'
%left BETWEEN
%left <subtok> COMPARISON /* = <> < > <= >= !> !< <=> */
%left '|'
%left '&'
%left <subtok> SHIFT /* << >> */
%left '+' '-'
%left '*' '/' '%' MOD
%left '^'

/* 终结符定义 */
%token CREAT
%token DATABASE
%token DATABASES
%token SHOW
%token DROP
%token USE
%token TABLE
%token TABLES
%token INSERT
%token INTO
%token VALUES
%token SELECT
%token FROM
%token WHERE
%token BETWEEN
%token AND
%token ANDOP
%token OR
%token XOR
%token NOT
%token DELETE
%token UPDATE
%token SET
%token SCHEMA
%token CASAD
%token RESTRICT
%token CHAR
%token VARCHAR
%token INT
%token SMALLINT
%token NUMERIC
%token REAL
%token DOUBLE
%token PRECISION
%token FLOAT
%token DATE
%token TIME
%token ADD
%token ALTER
%token COLUMN
%token UNIQUE
%token CLUSTER
%token INDEX
%token ON
%token ALL
%token DISTINCT
%token GROUP
%token BY
%token HAVING
%token ORDER
%token ASC
%token DESC
%token LIKE
%token NULLX
%token IN
%token LEFT
%token RIGHT
%token OUT
%token JOIN
%token ANY
%token SOME
%token <subtok> EXISTS  /*NOT EXISTS 或者 EXISTS*/
%token UNION
%token INTERSECT
%token EXCEPT
%token VIEW
%token AS
%token WITH
%token CHECK
%token OPTION
%token <intval> INTNUM
%token <floatval> APPROXNUM
%token <intval> BOOL
%token <strval> STRING
%token <subtok> COMPARISOON
%token <subtok> SHIFT
%token FCOUNT
%token <strval> NAME

/* 非终结符语义值类型定义及绑定  */
%type <intval> select_opts 
%type <intval> opt_asc_desc
/* 
%type <intval> opt_inner_cross opt_outer
%type <intval> left_or_right opt_left_or_right_outer column_list
%type <intval> index_list opt_for_join
*/
%type <strval> table_reference table_factor

%type <con_val> expr opt_where opt_having
%type <sf_val> select_expr_list
%type <st_val> table_references
%type <gb_val> opt_groupby groupby_list opt_orderby
%type <ss_val> select_stmt
%type <vl_val> val_list opt_list

%type <ds_val> delete_stmt

%type <cl_val> opt_col_names column_list
%type <ivl_val> insert_val_list insert_vals
%type <is_val> insert_stmt

%type <ua_val> update_asgn_list
%type <us_val> update_stmt

%type <strval> create_database_stmt drop_table_stmt drop_database_stmt

%type <ty_val> data_type 
%type <cf_val> create_definition
%type <cfs_val> create_col_list
%type <cs_val> create_table_stmt
%type <intval> opt_length

%start stmt_list
%% 

 /* 顶层规则 */
stmt_list: stmt ';'
| stmt_list stmt ';'
;

/* 表达式 */
expr: NAME 
{
  emit("NAME %s",$1); 
  $$=new_conditions(NULL,NULL,0,1,$1,0,0,0,NULL,0,0,0,0,0,0,0,0);
}
| NAME '.' NAME {
  emit("FIELDNAME %s.%s",$1,$3); 
  $$=new_conditions(NULL,NULL,0,1,$3,0,0,0,$1,0,0,0,0,0,0,0,0);
  }
| STRING {
  emit("STRING %s",$1); 
  $$=new_conditions(NULL,NULL,0,2,$1,0,0,0,NULL,0,0,0,0,0,0,0,0);
  }
| INTNUM {
  emit("NUMBER %d",$1); 
  $$=new_conditions(NULL,NULL,0,3,NULL,$1,0,0,NULL,0,0,0,0,0,0,0,0);
  }
| APPROXNUM {
  emit("FLOAT %g",$1); 
  $$=new_conditions(NULL,NULL,0,4,NULL,0,$1,0,NULL,0,0,0,0,0,0,0,0);
  }
| BOOL {
  emit("BOOL %d",$1); 
  $$=new_conditions(NULL,NULL,0,5,NULL,0,0,$1,NULL,0,0,0,0,0,0,0,0);
  }
;

expr: expr '+' expr {
  emit("ADD");
  $$=$1;
  if($1->type=3)
    {
      if($3->type=3)
  $$->num=$1->num+$3->num;
      else
	{
	  $$->type=4;
	  $$->flo=$1->num+$3->flo;
	}
    }
  else
    {
      if($3->type=3)
	$$->flo=$1->flo+$3->num;
      else
	$$->flo=$1->flo+$3->flo;
    }
  
 }
| expr '-' expr {
  emit("SUB");
  $$=$1;
  if($1->type=3)
    {
      if($3->type=3)
	$$->num=$1->num-$3->num;
      else
	{
	  $$->type=4;
	  $$->flo=$1->num-$3->flo;
	}
    }
  else
    {
      if($3->type=3)
	$$->flo=$1->flo-$3->num;
      else
	$$->flo=$1->flo-$3->flo;
    }
  
  }
| expr '*' expr {
  emit("MUL");
  $$=$1;
  if($1->type=3)
    {
      if($3->type=3)
	$$->num=$1->num*$3->num;
      else
	{
	  $$->type=4;
	  $$->flo=$1->num*$3->flo;
	}
    }
  else
    {
      if($3->type=3)
	$$->flo=$1->flo*$3->num;
      else
	$$->flo=$1->flo*$3->flo;
    }
  
  }
| expr '/' expr {
  emit("DIV");
  $$=$1;
  if($1->type=3)
    {
      if($3->type=3)
	$$->num=$1->num/$3->num;
      else    
	$$->flo=$1->num/$3->flo;
    }
  else
    {
      if($3->type=3)
	$$->flo=$1->flo/$3->num;
      else
	$$->flo=$1->flo/$3->flo;
    }
  $$->type=4;
  }
| expr '%' expr {
  emit("MOD");
  $$=$1;
    if($1->type=3)
    {
      if($3->type=3)
	$$->num=$1->num%$3->num;
      else    
	$$->flo=$1->num%$3->flo;
    }
  else
    {
      if($3->type=3)
	$$->flo=$1->flo%$3->num;
      else
	$$->flo=$1->flo%$3->flo;
    }
    $$->type=4;
  }
| expr MOD expr {
  emit("MOD");
  $$=$1;
  if($1->type=3)
    {
      if($3->type=3)
	$$->num=$1->num%$3->num;
      else    
	$$->flo=$1->num%$3->flo;
    }
  else
    {
      if($3->type=3)
	$$->flo=$1->flo%$3->num;
      else
	$$->flo=$1->flo%$3->flo;
    }
  $$->type=4;
 }
| '-' expr %prec UMINUS {
  emit("NEG");
  $$=$1;
  if($1->type=3)
    $$->num=-$$->num;
  else
    $$->flo=-$$->flo;
  }
;

expr: expr ANDOP expr {
  emit("AND");
  $$=new_conditions($1,$3,9,0,NULL,0,0,0,NULL,0,0,0,0,0,0,0,0);
 }
| expr OR expr {
  emit("OR");
  $$=new_conditions($1,$3,10,0,NULL,0,0,0,NULL,0,0,0,0,0,0,0,0);
 }
| NOT expr {
  emit("NOT");
  $$=$2;
  $$->not_flag=1;
 }
| '!' expr {
  emit("NOT");
  $$=$2;
  $$->not_flag=1;
  }
| expr COMPARISON expr {
  emit("CMP %d",$2);
  $$=new_conditions($1,$3,$2,0,NULL,0,0,0,NULL,0,0,0,0,0,0,0,0);
 }
;

/*

expr: expr '|' expr {emit("BITOR");}
| expr '&' expr {emit("BITAND");}
| expr '^' expr {emit("BITXOR");}
| expr SHIFT expr {emit("SHIFT %S",$2==1?"left":"right");}
*/

/* 递归select和比较表达式 */
expr: expr COMPARISON '(' select_stmt ')' {
  emit("CMPSELECT %d",$2);
  $$=new_conditions($1,$4->rc,$2,0,NULL,0,0,0,NULL,0,0,0,0,0,0,0,0);
 }
| expr COMPARISON ANY '(' select_stmt ')' {
  emit("CMPANYSELECT %d",$2);
  $$=new_conditions($1,$4->rc,$2,0,NULL,0,0,0,NULL,0,1,0,0,0,0,0,0);
 }
| expr COMPARISON SOME '(' select_stmt ')' {
  emit("CMPANYSELECT %d",$2);
  $$=new_conditions($1,$4->rc,$2,0,NULL,0,0,0,NULL,0,1,0,0,0,0,0,0);
 }
| expr COMPARISON ALL '(' select_stmt ')' {
  emit("CMPALLSELECT %d",$2);
  $$=new_conditions($1,$4->rc,$2,0,NULL,0,0,0,NULL,0,2,0,0,0,0,0,0);
 }

| expr IS NULLX {
  emit("ISNULL");
  $$=$1;
  $$->NULL_flag=1;
 }
| expr IS NOT NULLX {
  emit("ISNULL");
  $$=$1;
  $$->NULL_flag=1;
  $$->not_flag=1;
 }

/*
| expr IS BOOL {emit("ISBOOL %d",$3);}
| expr IS NOT BOOL {emit("ISBOOL %d",$4);emit("NOT");}
*/

| expr BETWEEN expr AND expr %prec BETWEEN {
  emit("BETWEEN");
  struct Conditions *tmp_exp1,*tmp_exp2;
  tmp_exp1=new_conditions($1,$3,6,0,NULL,0,0,0,NULL,0,0,0,0,0,0,0,0);
  tmp_exp2=new_conditions($1,$3,5,0,NULL,0,0,0,NULL,0,0,0,0,0,0,0,0);
  $$=new_conditions(tmp_exp1,tmp_exp2,0,0,NULL,0,0,0,NULL,0,0,0,0,0,0,0,0);
 }
;

/* 值列表,变长表达式计数 */
val_list: expr {
  $$=new_val_list_struct($1,1);
  
 }
| expr ',' val_list {
  struct Conditions *tmp_val_exp;
  tmp_val_exp=new_conditions($1,$3->val_con,0,0,NULL,0,0,0,NULL,0,0,0,0,0,0,0,0);
  $$=new_val_list_struct(tmp_val_exp,$3->val_num+1);
  }
;

opt_val_list: /* nil */{$$=NULL;}
| val_list {$$=$1;}
;

expr: expr IN '(' val_list ')' {
  emit("ISIN %d",$4);
  $$=new_conditions($1,$4->val_con,0,0,NULL,0,0,0,NULL,0,0,0,0,0,0,0,$4->val_num);
 }
| expr NOT IN '(' val_list ')' {
  emit("ISIN %d",$5); 
  emit("NOT");
  $$=new_conditions($1,$4->val_con,0,0,NULL,0,0,0,NULL,1,0,0,0,0,0,0,$4->val_num);
 }
| expr IN '(' select_stmt ')' {
  emit("CMPANYSELECT 4");
  $$=new_conditions($1,$4->rc,0,0,NULL,0,0,0,NULL,0,0,0,0,0,0,0,0);
 }
| expr NOT IN '(' select_stmt ')' {
  emit("CMPALLSELECT 3");
  $$=new_conditions($1,$5->rc,0,0,NULL,0,0,0,NULL,1,0,0,0,0,0,0,0);
 }
| EXISTS '(' select_stmt ')' {
  emit("EXISTSSELECT");
  $$=new_conditions($1,$3->rc,0,0,NULL,0,0,0,NULL,0,0,0,0,1,0,0,0);
  if($1)
    {
     emit("NOT");
     $$->not_flag=1;
    }
  }
;

/*函数*/
expr: FCOUNT '(' * ')' {
  emit("COUNTALL");
  $$=new_conditions(NULL,NULL,0,0,NULL,0,0,0,NULL,0,0,0,0,0,1,1,0);
 }
| FCOUNT '(' expr ')' {
  emit("CALL 1 COUNT");
  $$=$3;
  $$->fun_flag=1;
  }
;

/*其他表达式*/
expr: expr LIKE expr {
  emit("LIKE");
  $$=new_conditions($1,$3,0,0,NULL,0,0,0,NULL,0,0,0,1,0,0,0,0);
 }
| expr NOT LIKE expr {
  emit("LIKE");
  emit("NOT");
  $$=new_conditions($1,$4,0,0,NULL,0,0,0,NULL,1,0,0,1,0,0,0,0);
 }
;

/*
*select语句
*/
stmt: select_stmt {
  emit("STMT");
  select_eval($1);
 }
;

select_stmt: SELECT select_opts select_expr_list FROM table_references opt_where opt_groupby opt_having opt_orderby {
  emit("SELECT");
  $$=new_select_struct($2,$3,$5,$6,$7,$8,NULL);
  $$->rc=result_con($$);
}
;

opt_where: /*空*/ {$$=NULL;}
| WHERE expr {
  emit("WHERE");
  $$=$1;
 }
;

opt_groupby: /*空*/ {$$=NULL;}
| GROUP BY groupby_list {
  emit("GROUPBYLIST");
  $$=$3;
 }
;

groupby_list: expr opt_asc_desc { 
  emit("GROUPBY");
  $$=new_groupby($1,NULL,$2);
 }
| groupby_list ',' expr opt_asc_desc {
  emit("GROUPBY");
  $$=new_groupby($3,$1,$4);
  }
;

opt_asc_desc: /*空*/ {$$=0;}
| ASC  {$$=0;}
| DESC {$$=1;}
;

opt_having: /*空*/ {$$=NULL;}
| HAVING expr {
  emit("HAVING");
  $$=$2;
 }
;

opt_orderby: /*空*/ {$$=NULL;}
| ORDER BY groupby_list {
  emit("ORDERBY");
  $$=$3;
 }
;

/* 处理SELECT语句中主表达式 */

select_opts: /*空*/ {$$=0;}
|ALL {$$=0;}
|DISTINCTROW {$$=1;}
;

select_expr_list: expr {
  $$=new_select_fields($1->table,$1->value,NULL,0);
 }
| select_expr_list ',' expr {
  $$=new_select_fields($3->table,$3->value,$1,0);
  } 
| '*' {
  emit("SELECTALL");
  $$=new_select_fields(NULL,NULL,NULL,1);
  }
;

/* select表引用 */

table_references: table_table_reference {
  $$=new_select_tables($1,NULL);
 }
|table_references ',' table_reference {
  $$=new_select_tables($3,$1);
 }
;

table_reference: table_factor {$$=$1;}
		 /*| join_table*/
;

table_factor: NAME {
  emit("TABLE %s",$1);
  $$=$1;
 }
;

/* delete语句 */
stmt: delete_stmt {
  emit("STMT");
  delete_eval($1);
 }
;

delete_stmt:DELETE FROM NAME opt_where {
  emit("TABLE %s DELETE",$3);
  $$=new_delete_struct($3,$4);
 }
;

/* 插入语句 */
stmt: insert_stmt {
  emit("STMT");
  insert_eval($1);
 }
;

insert_stmt: INSERT INTO NAME opt_col_names insert_val_list{
  emit("INSERT %S",$3);
  &&=new_insert_struct($3,$4,$5);
 }
;

opt_col_names: /*空*/ {$$=NULL}
| '(' column_list ')' {
  emit("INSERTCOLS");
  $$=$2;
  }
;

column_list: NAME {
  emit("COLUMN %s",$1);
  $$=new_column_list_struct($1,NULL);
 }
| column_list ',' NAME {
  emit("COLUMN %s",$3);
  $$=new_column_list_struct($3,$1);
  }
;

insert_val_list: VALUES '(' insert_vals ')'{
  emit("INSERTVALUES");
  $$=$2;
 }
| select_stmt{
  emit("INSERTSELECT");
  $$=new_insert_val_list_struct($1->rc,NULL);
  }
;

insert_vals: expr{
  $$=new_insert_val_list_struct($1,NULL);
 }
| insert_vals ',' expr{
  $$=new_insert_val_list_struct($3,$1);
  }
;

/*更新语句*/
stmt: update_stmt {
  emit("STMT");
  update_eval($1);
 }
;

update_stmt: UPDATE table_reference SET update_asgn_list opt_where{
  emit("UPDATE");
  $$=new_update_struct($2,$4,$5);
 }
;

update_asgn_list: NAME COMPARISON expr{
  if($2!=4)
    {
      yyerror("bad update assignment to %s",$1);
      YYERROE;
    }
  else
    $$=new_update_asgn($1,$3);
 }
;

/*创建数据库*/
stmt: create_database_stmt {
  emit("STMT");
  creat_datebase($1);
 }
;

create_database_stmt: CREATE DATABASE NAME{
  emit("CREATE DATABASE %s",$3);
  $$=$3;
 }
;

/*创建表*/

stmt: create_table_stmt {
  emit("STMT");
  create_table($1);
 }
;

create_table_stmt: CREATE TABLE NAME '(' create_col_list ')' {
  emit("CREATE TABLE %s",$3);
  $$=new_create_struct($3,$5);
 }
;

create_col_list: create_definition {$$=new_create_fields($1,NULL);}
| create_col_list ',' create_definition {$$=new_create_fields($3,$1);}
;

create_definition: NAME data_type opt_length{
  emit("COLUMNDEF");
  $$=new_create_field($1,$2,$3);
 }
;

opt_length:/*NULL*/ {$$=0;}
| '(' INTNUM ')' {$$=$2;}
;

data_type: CHAR {$$=1;}
| INTNUM {$$=0;}
;

/*删除表和数据库,显示表名*/
stmt: drop_table_stmt{
  emit("STMT"); 
  drop_table($1);
 }
| drop_database_stmt{
  emit("STMT");
  drop_database($1);
  }
| show_tables_stmt{
  emit("STMT");
  show_tables();
  }
;

drop_table_stmt: DROP TABLE NAME{
  emit("DROP TABLE %s",$3);
  $$=$3;
 } 
;

drop_database_stmt: DROP DATABASE NAME{
  emit("DROP DATABASE %s",$3);
  $$=$3;
 } 
;

show_tables_stmt: SHOW TABLES {
  emit("SHOW TABLES");
 }
; 

%%
