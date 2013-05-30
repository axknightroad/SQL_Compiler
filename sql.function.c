/*
 *函数实现与C例程
 *by axknightroad
 *2013/05/27
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sql.struct.h"

#define PAGE_SIZE 2048 /* 页大小 */

char now_db[50]; //当前数据库

/*
 *构造节点的函数定义
 */

/*构造create语句中字段的语法树*/
struct Create_field *new_create_field(char *field,int type,int length)
{
  struct Create_field *a =malloc(sizeof(struct Create_field));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->field=field;
  a->type=type;
  a->length=length;
  return a;
}

/*构造create语句中字段组的语法树*/
struct Create_fields *new_create_fields(struct Create_field *cf,struct Create_fields *next_cf)
{
  struct Create_fields *a =malloc(sizeof(struct Create_fields));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->cf=cf;
  a->next_cf=next_cf;
  return a;
}

/*构造create语法树的根节点*/
struct Create_struct *new_create_struct(char *table,struct Create_field *cf)
{
  struct Create_struct *a =malloc(sizeof(struct Create_struct));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->table=table;
  a->cf=cf;
  return a;
}


/*构造select语法树的根节点的语法树*/
struct Select_struct *new_select_struct(int Select_flag,struct Select_fields *sf,struct Select_tables *st,struct Conditions *cons,struct Groupby *gb,struct Conditions *ha,struct Groupby *ob,struct Conditions *rc)
{
  struct Select_struct *a =malloc(sizeof(struct Select_struct));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->Select_flag=Select_flag;
  a->sf=sf;
  a->st=st;
  a->cons=cons;
  a->gb=gb;
  a->ha=ha;
  a->ob=ob;
  a->rc=rc;
  return a;
}

/*构造select语句条件字段的语法树*/
struct Conditions *new_conditions(struct Conditions *l,struct Conditions *r,int com_opt,int  type,char *value,int num,float flo,int boolnum,char *table,int not_flag,int any_flag,int null_flag,int like_flag,int exists_flag,int fun_flag,int all_flag,int in_num)
{
  struct Conditions *a =malloc(sizeof(struct Conditions));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->l=l;
  a->r=r;
  a->com_opt=com_opt;
  a->type=type;
  a->value=value;
  a->num=num;
  a->flo=flo;
  a->boolnum=boolnum;
  a->table=table;
  a->not_flag=not_flag;
  a->any_flag=any_flag;
  a->null_flag=null_flag;
  a->like_flag=like_flag;
  a->exists_flag=exists_flag;
  a->fun_flag=fun_flag;
  a->all_flag=all_flag;
  a->in_num=in_num;
  return a;
}

/*构造select 语句中选中的字段的语法树*/
struct Select_fields *new_select_fields(char *table,char *field,struct Select_fields *next_sf,int sall_flag)
{
  struct Select_fields *a =malloc(sizeof(struct Select_fields));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->table=table;
  a->field=field;
  a->next_sf=next_sf;
  a->sall_flag=sall_flag;
  return a;
}
  

/*构造select语句中选中的表的语法树*/
struct Select_tables *new_select_tables(char *table,struct Select_tables *next_st)
{
  struct Select_tables *a =malloc(sizeof(struct Select_tables));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->table=table;
  a->next_st=next_st;
  return a;
}

/*构造select语句中Groupby语句的语法树*/
struct Groupby *new_groupby(struct Conditions *gr_con,struct Groupby *next_gr,int sc_flag)
{
  struct Groupby *a =malloc(sizeof(struct Groupby));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->gr_con=gr_con;
  a->next_gr=next_gr;
  a->sc_flag=sc_flag;
  return a;
}

/*构造val_list结构的语法树*/
struct Val_list_struct *new_val_list_struct(struct Conditions *val_con,int val_num)
{
  struct Val_list_struct *a =malloc(sizeof(struct Val_list_struct));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->val_con=val_con;
  a->val_num=val_num;
  return a;
}
/*构造delete语法根节点树*/
struct Delete_struct *new_delete_struct(char *table,struct Conditions *cons)
{
  struct Delete_struct *a =malloc(sizeof(struct Delete_struct));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->table=table;
  a->cons=cons;
  return a;
}

/*构造字段名列表语法树*/
struct Column_list_struct *new_column_list_struct(char *name,struct Column_list_struct *next_cl)
{
  struct Column_list_struct *a =malloc(sizeof(struct Column_list_struct));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->name=name;
  a->next_cl=next_cl;
  return a;
}
/*构造Insert语句中值列表语法树*/
struct Insert_val_list_struct *new_insert_val_list_struct(struct Conditions *cons,struct Insert_val_list_struct *next_ivl)
{
  struct Insert_val_list_struct *a =malloc(sizeof(struct Insert_val_list_struct));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->cons=cons;
  a->next_ivl=next_ivl;
  return a;
}

/*构造Insert语法树根节点*/
struct Insert_struct *new_insert_struct(char *table,struct Column_list_struct *cl,struct Insert_val_list_struct *ivl)
{
  struct Insert_struct *a =malloc(sizeof(struct Insert_struct));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->table=table;
  a->cl=cl;
  a->ivl=ivl;
  return a;
}

/*构造update语句中列名及表达式语法树*/
struct Update_asgn *new_update_asgn(char *name,struct Conditions *con)
{
  struct Update_asgn *a =malloc(sizeof(struct Update_asgn));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->name=name;
  a->con=con;
  return a;
}

/*构造update语法树的根节点*/  
struct Update_struct *new_update_struct(char *table,struct Update_asgn *ua,struct Conditions *con)
{
  struct Update_struct *a =malloc(sizeof(struct Update_struct));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->table=table;
  a->ua=ua;
  a->con=con;
  return a;
}

struct table_dictionary *new_talbe_dictionary(char table[20],int start_page)
{
  struct table_dictionary *a =malloc(sizeof(struct table_dictionary));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->table=table;
  a->start_page=start_page;
  return a;
}

/*
 * 函数定义
 */

void create_database(char *name)
{
  FILE *fp1,*fp2;
  char db_name[50],dat_name[50];
  strcpy(db_name,name);
  strcpy(dat_name,name);
  strcat(db_name,".db");
  strcat(dat_name,".dat");
  fp1=fopen(db_name,"ab+");
  fp2=fopen(dat_name,"ab+");
  fclose(fp1);
  fclose(fp2);
  strcpy(now_db,name);
}

void drop_database(char *name)
{
  char db_name[50],dat_name[50];
  strcpy(db_name,name);
  strcpy(dat_name,name);
  strcat(db_name,".db");
  strcat(dat_name,".dat");
  unlink(db_name);
  unlink(dat_name);
}


void create_table(struct Creat_struct *t_name)
{
  //打开元数据文件和基本数据文件
  FILE *fp1,*fp2;
  char db_name[50],dat_name[50];
  strcpy(db_name,now_db);
  strcpy(dat_name,now_db);
  strcat(db_name,".db");
  strcat(dat_name,".dat");
  fp1=fopen(db_name,"rb+");
  fp2=fopen(dat_name,"rb+");
  int db_page,dat_page,used_page;
  db_page=0;
  dat_page=0;
  used_page=-1;
  //确定空白页
  fseek(fp1,PAGE_SIZE*(db_page+1)-sizeof(int),SEEK_SET);
  //int t=ftell(fp1);
  int t2=fread(&used_page,sizeof(int),1,fp1);
  while(db_page==used_page)
    {
      db_page++;
      fseek(fp1,PAGE_SIZE*(db_page+1)-sizeof(int),SEEK_SET);
      if(fread(&used_page,sizeof(int),1,fp1)!=1)
  break;
    }
  //确认是否重复
  char table_name[10],now_table[10];
  strcpy(table_name,t_name->table);
  rewind(fp1);
  if(fread(now_table,10,1,fp1)==1)
    {
      int p;
      for(p=0;p<db_page;p++)
	{
	  fseek(fp1,PAGE_SIZE*p,SEEK_SET);
	  
	  if(fread(now_table,10,1,fp1)!=1)
	    {
	      printf("Error in reading data\n");
	      exit(0);
	    }
	  if(strcmp(table_name,now_table)==0)
	    {
	      printf("This table has been created\n");
	      exit(0);
	    }
	}
    }
  //写表字典
  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);
  fwrite(table_name,10,1,fp1);  //向表字典写表名
  if(db_page>1)
    {
      int last_table_db_page=db_page-2;
      fseek(fp1,PAGE_SIZE*last_table_db_page+10,SEEK_SET);     
      if(fread(&dat_page,sizeof(int),1,fp1)!=1)
	{
	  printf("Error in reading data\n");
	  exit(0);
	}
      dat_page++;
    }
  fseek(fp2,PAGE_SIZE*(dat_page+1)-sizeof(int),SEEK_SET);
  if(fread(&used_page,sizeof(int),1,fp2)==1)
    {
      while(dat_page==used_page)
	{
	  dat_page++;
	  fseek(fp2,PAGE_SIZE*(dat_page+1)-sizeof(int),SEEK_SET);
	  if(fread(&used_page,sizeof(int),1,fp2)!=1)
	    break;
	}
    } 
  fseek(fp2,PAGE_SIZE*(dat_page+1)-sizeof(int),SEEK_SET);
  fwrite(&dat_page,sizeof(int),1,fp2);
  fseek(fp1,PAGE_SIZE*db_page+10,SEEK_SET);
  fwrite(&dat_page,sizeof(int),1,fp1); //向表字典写起始页
  fseek(fp1,PAGE_SIZE*(db_page+1)-sizeof(int),SEEK_SET);
  fwrite(&db_page,sizeof(int),1,fp1);
  //写列字典
  db_page++;
  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);
  struct Create_fields *a=malloc(sizeof(struct Create_fields));
  struct Create_field *b=malloc(sizeof(struct Create_field));
  a=t_name->cf;
  b=a->cf;
  char row_name[10];
  int row_offset=0;
  int row_length=0;
  while(a!=NULL)
    {
      if(fwrite(table_name,10,1,fp1)!=1)//向列字典写表名
	{
	  printf("Error in writeing data\n");
	  exit(0);
	} 
      int row=0;
      if(fwrite(&row,sizeof(int),1,fp1)!=1) //向列字典写列号
	{
	  printf("Error in writeing data\n");
	  exit(0);
	}       
      b=a->cf;
      strcpy(row_name,b->field);
      if(fwrite(row_name,10,1,fp1)!=1)//写列名
	{
	  printf("Error in writeing data\n");
	  exit(0);
	} 
      row_offset=row_offset+row_length;
      if(fwrite(&row_offset,sizeof(int),1,fp1)!=1)//写偏移量
       	{
	  printf("Error in writeing data\n");
	  exit(0);
	} 
      if(b->type==0)
	row_length=sizeof(int);
      else
	row_length=b->length;
      if(fwrite(&row_length,sizeof(int),1,fp1)!=1)//写宽度
	{
	  printf("Error in writeing data\n");
	  exit(0);
	} 
      a=a->next_cf;
    }
  fseek(fp1,PAGE_SIZE*(db_page+1)-sizeof(int),SEEK_SET);
  if(fwrite(&db_page,sizeof(int),1,fp1)!=1) 
    {
      printf("Error in writeing data\n");
      exit(0);
    }
  fclose(fp1);
  fclose(fp2);
}

void insert_eval(struct Insert_struct *name){
  
  //打开基本数据文件和元数据文件
  FILE *fp1,*fp2;
  char db_name[50],dat_name[50];
  strcpy(db_name,now_db);
  strcpy(dat_name,now_db);
  strcat(db_name,".db");
  strcat(dat_name,".dat");
  fp1=fopen(db_name,"rb");
  fp2=fopen(dat_name,"rb+");
  
  int db_page,dat_page,used_page;  //元数据文件和基本数据文件页数
  db_page=0;
  dat_page=0;
  used_page=-1;
  
  //确定该表在元数据文件所在页
  char table_name[10],now_table[10];
  strcpy(table_name,name->table);
  rewind(fp1);
  if(fread(now_table,10,1,fp1)==1)
    {
      while(strcmp(table_name,now_table)!=0)
	{
	  db_page++;
	  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);	  
	  if(fread(now_table,10,1,fp1)!=1)
	    {
	      printf("There is no this table\n");
	      exit(0);
	    }
	}
    }
  else
    {
      printf("There is no this table");
      exit(0);
    }
  if(fread(&dat_page,sizeof(int),1,fp1)!=1)
    {
      printf("Erroe in reading data.");
      exit(0);
    }
  fseek(fp2,PAGE_SIZE*dat_page,SEEK_SET);//将基本数据文件读写头移到该表存储页
  //获取表信息
  db_page++;
  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);
  int row_num=0;  //行数
  char eof_flag[10];
  int rowsize=sizeof(int)*3+20; //列字典每行长度
  if(fread(eof_flag,10,1,fp1)!=1)
    {
      printf("There is no this table\n");
      exit(0);
    }
  while(strcmp(eof_flag,table_name)==0)
    {
      row_num++;
      fseek(fp1,rowsize*row_num+PAGE_SIZE*db_page,SEEK_SET);
      if(fread(eof_flag,10,1,fp1)!=1)
	break;
    }
  fseek(fp1,PAGE_SIZE*db_page+rowsize*row_num-2*sizeof(int),SEEK_SET);
  int line_offset,tmp0; //该表每行的偏移量
  if(fread(&line_offset,sizeof(int),1,fp1)!=1)
    	{
	  printf("Erroe in reading data.");
	  exit(0);
	}
  if(fread(&tmp0,sizeof(int),1,fp1)!=1)
    {
      printf("Erroe in reading data.");
      exit(0);
    }
  line_offset=tmp0+line_offset;
  int line_num=0;   //该表现有行数
  while(fgets(eof_flag,10,fp1)!=NULL)
    {
      line_num++;
      fseek(fp2,line_offset,SEEK_CUR);      
    }
  fseek(fp2,PAGE_SIZE*dat_page+line_num*line_offset,SEEK_SET); //读写头移到空白处准备写入数据
  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);
  int row_offset[row_num],row_length[row_num]; //该表每列偏移量与长度
  char row_name[row_num*10];  //每列名称
    int i;
    for(i=0;i<row_num;i++)
    {
      fseek(fp1,10+sizeof(int),SEEK_CUR);
      if(fread(&row_name[i*10],1,10,fp1)!=10)
	{
	  printf("Erroe in reading data.");
	  exit(0);
	}
      if(fread(&row_offset[i],sizeof(int),1,fp1)!=1)
	{
	  printf("Erroe in reading data.");
	  exit(0);
	}
      if(fread(&row_length[i],sizeof(int),1,fp1)!=1)
	{
	  printf("Erroe in reading data.");
	  exit(0);
	}
    }
   
  struct Insert_struct *is=malloc(sizeof(struct Insert_struct));
  is=name;
  //写入数据
  fseek(fp2,dat_page*PAGE_SIZE,SEEK_SET);
  for(i=0;i<row_num;i++)
    {
       if(is->cl==NULL)           //所有属性列都有数据
	{	  
	   switch(is->ivl->cons->type)
	    {
	    case 2:                 //添加字符串
	      {
		char s[20],s1[20],s2[1];
		strcpy(s,is->ivl->cons->value);
		strcpy(s2,"'");
		int j=0;
		while(s[j+1]!=s2[0])
		  {
		    s1[j]=s[j+1];
		    j++;
		  } 
		if(fwrite(s1,row_length[i],1,fp2)!=1)
		  {
		    printf("Erroe in writeing data.");
		    exit(0);
		  }
	      }  
	      break;
	    case 3:           //添加整数
	      {
		int rv2=is->ivl->cons->num;
		if(fwrite(&rv2,row_length[i],1,fp2)!=1)
		  {
		    printf("Erroe in writeing data.");
		    exit(0);
		  }
	      }
	      break;
	    } 
	   is->ivl=is->ivl->next_ivl;
	}
       else           //添加个别列
	 {
	   char t1[10],t2[10];
	   int k;
	   for(k=0;k<10;k++)
	     t1[k]=row_name[i*10+k];
	   strcpy(t2,is->cl->name);
	   if(strcmp(t1,t2)==0)
	     {
	       switch(is->ivl->cons->type)
		 {
		 case 2:
		   {
		     char s[20],rv1[20],s1[1];
		     strcpy(s,is->ivl->cons->value);
		     strcpy(s1,"'");
		     int j=0;
		     while(s[j+1]!=s1[0])
		       {
			rv1[j]=s[j+1];
			j++;
		       }
		     if(fwrite(rv1,row_length[i],1,fp2)!=1)
		       {
			  printf("Erroe in writeing data.");
			  exit(0);
			}
		   }
		   break;
		 case 3:
		   {
		     int rv2=is->ivl->cons->num;
		     if(fwrite(&rv2,row_length[i],1,fp2)!=1)
		       {
			 printf("Erroe in writeing data.");
			 exit(0);
		       }
		   }
		   break;
		 }
	       is->ivl=is->ivl->next_ivl;
	       is->cl=is->cl->next_cl;
	     }
	   else
	     fseek(fp2,row_offset[i]+PAGE_SIZE*dat_page,SEEK_SET);
	 }
    }

}


char select_eval(struct Select_struct *);

struct Conditions result_con(struct Select_struct *);

char delete_eval(struct Delete_struct *);

char update_eval(struct Update_struct *);


void select_free(struct Select_struct *);

void drop_table(char *name);

void show_talbe();
