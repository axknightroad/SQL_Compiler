/*
 *函数实现与C例程
 *by axknightroad
 *2013/05/27
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "sql.struct.h"

#define PAGE_SIZE 2048 /* 页大小 */

char now_db[50]; //当前数据库
char *xjgl="XJGL"; //缺省数据库
char null[5]; //空字符串

/*
  栈函数


static struct DataStack *DStack;
static struct LineStack *LStack;

int dsisempty()
{
  return DStack==NULL;
}

int lsisempty()
{
  return LStack==NULL;
}
int isfull()
{
  return 0;
}

void dpush(struct DataStack *ds,struct t_data *td)
{
  struct DataStack *temp=malloc(sizeof(struct DataStack));
  //assert(!isfull());
  //assert(temp!=NULL);
  temp->num=1+ds->num;
  temp->next=ds;
  ds=temp;
  ds->td=td;
} 

struct t_data *dpop(struct DataStack *ds)
{
  struct t_data *td;
  struct DataStack *temp;
  //assert(!dsisempty());
  temp=ds;
  td=ds->td;
  ds=temp->next;
  free(temp);
  return td;
}

void lpush(struct LineStack *ls,struct t_line *tl)
{
  struct DataStack *temp=malloc(sizeof(struct DataStack));
  //assert(!isfull());
  //assert(temp!=NULL);
  //temp->num=1+ls->num;
  temp->next=ls;
  ls=temp;
  ls->tl=tl;
} 
struct t_line *lpop(struct LineStack *ls)
{
  struct t_line *tl;
  struct DataStack *temp;
  //assert(!lsisempty());
  temp=ls;
  tl=ls->tl;
  ls=temp->next;
  free(temp);
  return tl;
}
*/
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
struct Create_struct *new_create_struct(char *table,struct Create_fields *cf)
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
struct Select_struct *new_select_struct(int select_flag,struct Select_fields *sf,struct Select_tables *st,struct Conditions *cons,struct Groupby *gb,struct Conditions *ha,struct Groupby *ob)
{
  struct Select_struct *a =malloc(sizeof(struct Select_struct));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->select_flag=select_flag;
  a->sf=sf;
  a->st=st;
  a->cons=cons;
  a->gb=gb;
  a->ha=ha;
  a->ob=ob;
  return a;
}

/*构造select语句条件字段的语法树*/
struct Conditions *new_conditions(struct Conditions *l,struct Conditions *r,int com_opt,int  type,char *value,int num,float flo,int boolnum,char *table,int not_flag,int any_flag,int null_flag,int like_flag,int exists_flag,int fun_flag,int all_flag,int in_num,struct Select_struct *ss)
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
  a->ss=ss;
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
struct Update_asgn *new_update_asgn(char *name,struct Conditions *con,struct Update_asgn *next)
{
  struct Update_asgn *a =malloc(sizeof(struct Update_asgn));
  if(!a)
    {
      yyerror("out of space");
      exit(0);
    }
  a->name=name;
  a->con=con;
  a->next=next;
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


/*
 * 函数定义
 */
void create_database(char *name)        //创建数据库函数
{
  FILE *fp1,*fp2,*fp3;
  char db_name[50],dat_name[50];
  strcpy(db_name,name);
  strcpy(dat_name,name);
  strcat(db_name,".db");
  strcat(dat_name,".dat");
  fp1=fopen(db_name,"ab+");
  fp2=fopen(dat_name,"ab+");
  fclose(fp1);
  fclose(fp2);
  int db_num=0;
  char db_found[50];
  fp3=fopen("db.info","rb+");
  if(fp3==NULL)
    fp3=fopen("db.info","wb+");
  fseek(fp3,0,SEEK_SET);
  fread(&db_num,sizeof(int),1,fp3);
  int i;
  for(i=0;i<db_num;i++)
    {
      fseek(fp3,sizeof(int)+i*50,SEEK_SET);
      fread(db_found,50,1,fp3);
      if(strcmp(db_found,name)==0)
  {
	  printf("This database have been created\n");
	  exit(0);
	}
    }
  fseek(fp3,sizeof(int)+50*db_num,SEEK_SET);
  fwrite(name,50,1,fp3);
  db_num++;
  fseek(fp3,0,SEEK_SET);
  fwrite(&db_num,sizeof(int),1,fp3);
  fclose(fp3);
  printf("Create database successed\n");
}







void drop_database(char *name)         //删除数据库函数
{
  char db_name[50],dat_name[50];
  strcpy(db_name,name);
  strcpy(dat_name,name);
  strcat(db_name,".db");
  strcat(dat_name,".dat");
  unlink(db_name);
  unlink(dat_name);
  FILE *fp3;
  char s[50];
  int db_num,this_db_num,i;
  char db_found[50];
  fp3=fopen("db.info","rb+");
  fseek(fp3,0,SEEK_SET);
  fread(&db_num,sizeof(int),1,fp3);
  for(i=0;i<db_num;i++)
    {
      fseek(fp3,sizeof(int)+i*50,SEEK_SET);
      fread(db_found,50,1,fp3);
      if(strcmp(db_found,name)==0)
	{
	  this_db_num=i;
	  break;
	}
      if(i==db_num-1)
	{
	  printf("no this database\n");
	  exit(0);
	}
    }
  if(this_db_num==db_num-1)
    {
      fseek(fp3,sizeof(int)+this_db_num*50,SEEK_SET);
      fwrite(null,5,10,fp3);
    }
  else
    {
      fseek(fp3,sizeof(int)+db_num-1*50,SEEK_SET);
      fread(db_found,50,1,fp3);
      fseek(fp3,sizeof(int)+this_db_num*50,SEEK_SET);
      fwrite(db_found,50,1,fp3);
      fseek(fp3,sizeof(int)+db_num-1*50,SEEK_SET);
      fwrite(null,5,10,fp3);
    }
  db_num--;
  fseek(fp3,0,SEEK_SET);
  fwrite(&db_num,sizeof(int),1,fp3);
  fclose(fp3);

  printf("Drop database successed\n");
}


void show_databases()
{
  FILE *fp3;
  int db_num,this_db_num,i;
  char db_found[50];
  fp3=fopen("db.info","rb+");
  fseek(fp3,0,SEEK_SET);
  fread(&db_num,sizeof(int),1,fp3);
  for(i=0;i<db_num;i++)
    {
      fseek(fp3,sizeof(int)+i*50,SEEK_SET);
      fread(db_found,50,1,fp3);
      printf("database:%s\n",db_found);
    }
}

void use(char *name)
{
  strcpy(now_db,name);
  FILE *fp1;  
  char db_name[50];
  strcpy(db_name,now_db);
  strcat(db_name,".db");
  fp1=fopen(db_name,"rb+");
  fseek(fp1,PAGE_SIZE-50,SEEK_SET);
  fread(null,5,1,fp1);
  printf("use database %s now\n",name);
  
}

void create_table(struct Create_struct *t_name)   //创建表
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
  char table_name[20],now_table[20];
  int i=0;
  for(i=0;i<20;i++)
    {
      table_name[i]='\0';
      table_name[i]='\0';
    }
  strcpy(table_name,t_name->table);
  rewind(fp1);
  if(fread(now_table,20,1,fp1)==1)
    {
      int p;
      for(p=0;p<db_page;p++)
	{
	  fseek(fp1,PAGE_SIZE*p,SEEK_SET);
	  
	  if(fread(now_table,20,1,fp1)!=1)
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
  fwrite(table_name,20,1,fp1);  //向表字典写表名
  if(db_page>1)
    {
      int last_table_db_page=db_page-2;
      fseek(fp1,PAGE_SIZE*last_table_db_page+20,SEEK_SET);     
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
  fseek(fp1,PAGE_SIZE*db_page+20,SEEK_SET);
  fwrite(&dat_page,sizeof(int),1,fp1); //向表字典写起始页
  fseek(fp1,PAGE_SIZE*(db_page+1)-sizeof(int),SEEK_SET);
  fwrite(&db_page,sizeof(int),1,fp1);
  //写列字典
  db_page++;
  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);
  struct Create_fields *a=malloc(sizeof(struct Create_fields));
  struct Create_field *b=malloc(sizeof(struct Create_field));
  a=t_name->cf;
  char row_name[20];
  for(i=0;i<20;i++)
    {
      row_name[i]='\0';
    }
  int row_offset=0;
  int row_length=0;
  int row=0;
  while(a!=NULL)
    {
      b=a->cf;
      if(fwrite(table_name,20,1,fp1)!=1)//向列字典写表名
	{
	  printf("Error in writeing data\n");
	  exit(0);
	} 
      if(fwrite(&row,sizeof(int),1,fp1)!=1) //向列字典写列号
	{
	  printf("Error in writeing data\n");
	  exit(0);
	}       
      row++;
      strcpy(row_name,b->field);
      if(fwrite(row_name,20,1,fp1)!=1)//写列名
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
	{
	  row_length=sizeof(int);
	  if(fwrite(&row_length,sizeof(int),1,fp1)!=1)//写宽度
	    {
	      printf("Error in writeing data\n");
	      exit(0);
	    } 	  
	  int type=0;
	  fwrite(&type,sizeof(int),1,fp1);//写数据类型
	}
      else	
	{
	  row_length=b->length;
	  if(fwrite(&row_length,sizeof(int),1,fp1)!=1)//写宽度
	    {
	      printf("Error in writeing data\n");
	      exit(0);
	    } 
	  int type=1;
	  fwrite(&type,sizeof(int),1,fp1); //写数据类型
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
  printf("Create table successed\n");
}








void insert_eval(struct Insert_struct *name){              //计算insert函数 
  
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
  char table_name[20],now_table[20];
  int i=0;
  for(i=0;i<20;i++)
    {
      table_name[i]='\0';
      table_name[i]='\0';
    }
  strcpy(table_name,name->table);
  rewind(fp1);
  if(fread(now_table,20,1,fp1)==1)
    {
      while(strcmp(table_name,now_table)!=0)
	{
	  db_page++;
	  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);	  
	  if(fread(now_table,20,1,fp1)!=1)
	    {
	      printf("There is no this table\n");
	      exit(0);
	    }
	}
    }
  else
    {
      printf("There is no this table\n");
      exit(0);
    }
  if(fread(&dat_page,sizeof(int),1,fp1)!=1)
    {
      printf("Erroe in reading data\n");
      exit(0);
    }
  fseek(fp2,PAGE_SIZE*dat_page,SEEK_SET);//将基本数据文件读写头移到该表存储页
  //获取表信息
  db_page++;
  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);
  int row_num=0;  //该表共有列数
  char eof_flag[20];
  i=0;
  for(i=0;i<20;i++)
    {
      eof_flag[i]='\0';
    }
  int rowsize=sizeof(int)*4+40; //列字典每行长度
  if(fread(eof_flag,20,1,fp1)!=1)
    {
      printf("There is no this table\n");
      exit(0);
    }
  while(strcmp(eof_flag,table_name)==0)
    {
      row_num++;
      fseek(fp1,rowsize*row_num+PAGE_SIZE*db_page,SEEK_SET);
      if(fread(eof_flag,20,1,fp1)!=1)
	break;
    }
  fseek(fp1,PAGE_SIZE*db_page+rowsize*row_num-3*sizeof(int),SEEK_SET);
  int line_offset,tmp0; //该表每行的偏移量
  if(fread(&line_offset,sizeof(int),1,fp1)!=1)
    	{
	  printf("Erroe in reading data\n");
	  exit(0);
	}
  if(fread(&tmp0,sizeof(int),1,fp1)!=1)
    {
      printf("Erroe in reading data\n");
      exit(0);
    }
  line_offset=tmp0+line_offset;
  int line_num=0;   //该表现有行数
  char tmp2[5];
  i=0;
  for(i=0;i<5;i++)
    {
      tmp2[i]='\0';
    }
  fseek(fp2,PAGE_SIZE*dat_page,SEEK_SET);
  fread(tmp2,5,1,fp2);
  while(strcmp(null,tmp2)!=0)
    {
      line_num++;
      fseek(fp2,line_offset*line_num+PAGE_SIZE*dat_page,SEEK_SET);
      fread(tmp2,2,1,fp2);
    }
  fseek(fp2,PAGE_SIZE*dat_page+line_num*line_offset,SEEK_SET); //读写头移到空白处准备写入数据
  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);
  int row_offset[row_num],row_length[row_num],row_type[row_num];//该表每列偏移量、长度、数据类型
  char row_name[row_num*20];  //每列名称  
  for(i=0;i<20*row_num;i++)
    {
      row_name[i]='\0';
    }
    for(i=0;i<row_num;i++)
    {
      fseek(fp1,20+sizeof(int)+PAGE_SIZE*db_page+i*rowsize,SEEK_SET);
      if(fread(&row_name[i*20],1,20,fp1)!=20)
	{
	  printf("Erroe in reading data\n");
	  exit(0);
	}
      if(fread(&row_offset[i],sizeof(int),1,fp1)!=1)
	{
	  printf("Erroe in reading data\n");
	  exit(0);
	}
      if(fread(&row_length[i],sizeof(int),1,fp1)!=1)
	{
	  printf("Erroe in reading data\n");
	  exit(0);
	}
      if(fread(&row_type[i],sizeof(int),1,fp1)!=1)
	{
	  printf("Erroe in reading data\n");
	  exit(0);
	}
    }
   
  struct Insert_struct *is=malloc(sizeof(struct Insert_struct));
  is=name;
  //写入数据
  fseek(fp2,dat_page*PAGE_SIZE+line_num*line_offset,SEEK_SET);
  int p=ftell(fp2);
  struct Column_list_struct *cl;
  struct Insert_val_list_struct *ivl;
  cl=is->cl;
  ivl=is->ivl;
  for(i=0;i<row_num;i++)
    {
      p=ftell(fp2);
       if(is->cl==NULL)           //所有属性列都有数据
	{	  
	   switch(ivl->cons->type)
	    {
	    case 2:                 //添加字符串
	      {
		char s[22],s1[20];
		strcpy(s,ivl->cons->value);
		int j=0;
		while(s[j+1]!='\'')
		  {
		    s1[j]=s[j+1];
		    j++;
		  } 
		int k;
		for(k=j;k<20;k++)
		  {
		    s1[k]='\0';
		  }
		if(fwrite(s1,row_length[i],1,fp2)!=1)
		  {
		    printf("Erroe in writeing data\n");
		    exit(0);
		  }
	      }  
	      break;
	    case 3:           //添加整数
	      {
		int rv2=ivl->cons->num;
		if(fwrite(&rv2,row_length[i],1,fp2)!=1)
		  {
		    printf("Erroe in writeing data\n");
		    exit(0);
		  }
	      }
	      break;
	    } 
	   ivl=ivl->next_ivl;
	}
       else           //添加个别列
	 {
	   if(cl==NULL)
	     {
	       fwrite("null",4,1,fp2);
	       if(row_length[i]-4>0)		 
		 fwrite('\0',1,row_length[i]-4,fp2);	
	       goto over;
	     }
	   char t1[20],t2[20];
	   int k;
	   for(k=0;k<20;k++)
	     t1[k]=row_name[i*20+k];
	   strcpy(t2,cl->name);
	   if(strcmp(t1,t2)==0)
	     {
	       switch(ivl->cons->type)
		 {
		 case 2:
		   {
		     char s[22],s1[20];
		     strcpy(s,ivl->cons->value);
		     int j=0;
		     while(s[j+1]!='\'')
		       {
			 s1[j]=s[j+1];
			 j++;
		       } 
		     int k;
		     for(k=j;k<20;k++)
		       {
			 s1[k]='\0';
		       }
		     if(fwrite(s1,row_length[i],1,fp2)!=1)
		       {
			 printf("Erroe in writeing data\n");
			 exit(0);
		       }
		   }
		   break;
		 case 3:
		   {
		     int rv2=ivl->cons->num;
		     if(fwrite(&rv2,row_length[i],1,fp2)!=1)
		       {
			 printf("Erroe in writeing data\n");
			 exit(0);
		       }
		   }
		   break;
		 }
	     }
	 next:ivl=ivl->next_ivl;	       
	   cl=cl->next_cl;

	 }

    }
 over:fclose(fp1);
  fclose(fp2);
  printf("Insert successed\n");
}








struct t_data *loaddata(FILE *fp2,int row_num,char row_name[],int row_type[],int row_length[],int row_offset[],int lnum,int rnum,int dat_page,int line_offset,int line_num)
{   //读每项数据
  struct t_data *a=malloc(sizeof(struct t_data));
  a->lnum=lnum;
  a->loffset=line_offset;
  a->rnum=rnum;
  int i;
  char name0[20];
  char value[row_length[rnum]];
  for(i=0;i<20;i++)
    name0[i]=row_name[rnum*20+i];
  //strncpy(name0,&row_name[num*20],20);
  a->name=name0;
  a->roffset=row_offset[rnum];
  a->type=row_type[rnum];
  // fseek(fp2,line_offset*line_num+PAGE_SIZE*dat_page+row_offset[num],SEEK_SET);
  char s[4];
  if(fread(s,4,1,fp2)!=1)
    {
      printf("Error in reading data\n");
      exit(0);
    }
  if(strcmp(s,"null")==0) //判断是否为空数据
    {
      a->nflag=1;
      a->value=NULL;
      a->num=-1;
      fseek(fp2,-4,SEEK_CUR);
      fseek(fp2,row_length[rnum],SEEK_CUR);
    }
  else             //读具体数据
    {  
      fseek(fp2,-4,SEEK_CUR);
      //fseek(fp2,PAGE_SIZE*dat_page+row_offset[rnum]+line_offset*lnum,SEEK_SET); 
      a->nflag=0;
      if(a->type==0)
	{
	  int anum;      
	  if(fread(&anum,sizeof(int),1,fp2)!=1)
	    {
	      printf("Error in reading data\n");
	      exit(0);
	    }
	  a->num=anum;
	  a->length=0;
	}
      else
	{
	  if(fread(value,row_length[rnum],1,fp2)!=1)
	    {
	      printf("Error in reading data\n");
	      exit(0);
	    }
	  //strcpy(va,value);
	  //	  value2=value0;
	  //  *value1=*value2;
	  a->value=value;
	  a->length=row_length[rnum];
	}
     
    }
  a->rsflag=0;
  
  if(rnum<row_num-1)    
      a->next_data=loaddata(fp2,row_num,row_name,row_type,row_length,row_offset,lnum,rnum+1,dat_page,line_offset,line_num);
 
  else if(lnum<line_num-1)
    a->next_data=loaddata(fp2,row_num,row_name,row_type,row_length,row_offset,lnum+1,0,dat_page,line_offset,line_num);
  else
    a->next_data=NULL;
  return a;
}
  

/*


  struct t_line *loadline(FILE *fp2,int line_num,int row_num,char row_name[],int row_type[],int row_length[],int line_offset,int row_offset[],int num,int dat_page)  //读行数据
{
  struct t_line *a=malloc(sizeof(struct t_line));
  a->lnum=num;
  a->loffset=line_offset;
  a->sflag=0;
  fseek(fp2,dat_page*PAGE_SIZE+num*line_offset,SEEK_SET);
  int i;
  /*struct t_data *td;
  for(i=0;i<row_num-1;i++)
    {
      td=loaddata(fp2,row_num,row_name,row_type,row_length,row_offset,i,dat_page,line_offset,num); //读此行数据
      dpush(DStack,td);
      }
  a->ld=loaddata(fp2,row_num,row_name,row_type,row_length,row_offset,0,dat_page,line_offset,num);
  //  td->next_data=NULL;
  /*  for(i=0;i<row_num-1;i++)
    {
      struct t_data *temp;
      temp=td;
      td=dpop(DStack);
      td->next_data=temp;
      } 
  //a->ld=td;
  if(num<line_num-1)
    a->next_line=loadline(fp2,line_num,row_num,row_name,row_type,row_length,line_offset,row_offset,num+1,dat_page); //递归下一行
  else
  a->next_line=NULL;
  return a;
}
 */



struct table_s *loadtable(FILE *fp1,FILE *fp2,int table_num)  //读表数据
{

  struct table_s *a=malloc(sizeof(struct table_s));
  int dat_page,db_page;
  char table_name[20];
  char *table_name0;
  db_page=table_num;
  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);
  if(fread(table_name,20,1,fp1)!=1)
    {
      printf("Error in reading data");
      exit(0);
    }
  table_name0=table_name;
  a->table=table_name0;
  if(fread(&dat_page,sizeof(int),1,fp1)!=1)
    {
      printf("Error in reading data");
      exit(0);
    }
  a->start_page=dat_page;
  db_page++;
  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);
  int row_num=0;  //该表共有列数
  char eof_flag[20];
  int rowsize=sizeof(int)*4+40; //列字典每行长度
  if(fread(eof_flag,20,1,fp1)!=1)
    {
      printf("There is no this table\n");
      exit(0);
    }
  while(strcmp(eof_flag,table_name)==0)
    {
      row_num++;
      fseek(fp1,rowsize*row_num+PAGE_SIZE*db_page,SEEK_SET);
      if(fread(eof_flag,20,1,fp1)!=1)
	break;
    }
  a->row_num=row_num; //写该表列数
  fseek(fp1,PAGE_SIZE*db_page+rowsize*row_num-3*sizeof(int),SEEK_SET);
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
  char tmp2[2];
  fseek(fp2,PAGE_SIZE*dat_page,SEEK_SET);
  fread(tmp2,2,1,fp2);
  while(strcmp(null,tmp2)!=0)
    {
      line_num++;
      fseek(fp2,line_offset*line_num+PAGE_SIZE*dat_page,SEEK_SET);
      fread(tmp2,2,1,fp2);
    }
  a->line_num=line_num;
  fseek(fp2,PAGE_SIZE*dat_page+line_num*line_offset,SEEK_SET); //读写头移到空白处准备写入数据
  fseek(fp1,PAGE_SIZE*db_page,SEEK_SET);
  int row_offset[row_num],row_length[row_num],row_type[row_num];//该表每列偏移量、长度、数据类型
  char row_name[row_num*20];  //每列名称
  int i;
  for(i=0;i<row_num;i++)
    {
      fseek(fp1,PAGE_SIZE*db_page+i*rowsize+20+sizeof(int),SEEK_SET);
      if(fread(&row_name[i*20],1,20,fp1)!=20)
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
      if(fread(&row_type[i],sizeof(int),1,fp1)!=1)
	{
	  printf("Erroe in reading data.");
	  exit(0);
	}
    }
  /* struct t_line *tl;
  for(i=0;i<line_num-1;i++)
    {      
      tl=loadline(fp2,line_num,row_num,row_name,row_type,row_length,line_offset,row_offset,0,dat_page); //读表的行数据
      lpush(LStack,tl);
    } */
  fseek(fp2,PAGE_SIZE*dat_page,SEEK_SET);
  a->data=loaddata(fp2,row_num,row_name,row_type,row_length,row_offset,0,0,dat_page,line_offset,line_num);
  //  tl->next_line=NULL;
  /*  for(i=0;i<line_num-1;i++)
    {
      struct t_line *temp;
      temp=tl;
      tl=lpop(LStack);
      tl->next_line=temp;
    } 
  //a->line=tl;
   if(num<table_num-1)
    a->next_table=loadtable(fp1,fp2,table_num,num+1);//递归下一个表
    else */
    a->next_table=NULL; 
  return a;
}

//声明
struct t_data *select_eval(struct Select_struct *name);
struct table_s *cons_eval(struct Conditions *cons,struct table_s *table);




struct table_s *ao_eval(struct Conditions *cons,struct table_s *ts)  //条件句中and、or语句
{
  struct Conditions *cons1;
  struct Conditions *cons2;
  struct table_s *ts0;
  struct t_data *td;
  int i;
  int line_num;
  line_num=ts->line_num;
  int line_sflag0[line_num];
  int line_sflag1[line_num];
  int line_sflag2[line_num];
  cons1=cons->l;
  cons2=cons->r;
  ts0=ts;
  ts0=cons_eval(cons1,ts0);
  td=ts0->data;
  for(i=0;i<line_num;i++)
    {
	line_sflag1[i]=0;
    }
  while(td!=NULL)
    {
      if(td->lsflag==1)
	line_sflag1[td->lnum]=1;
      td=td->next_data;
    } 
  td=ts0->data;
  while(td!=NULL)
    {
      td->lsflag=0;
      td=td->next_data;
    } 
  ts0=cons_eval(cons2,ts0);
  td=ts0->data;
  for(i=0;i<line_num;i++)
    {
	line_sflag2[i]=0;
    }
  while(td!=NULL)
    {
      if(td->lsflag==1)
	line_sflag2[td->lnum]=1;
      td=td->next_data;
    } 
  td=ts0->data;
  while(td!=NULL)
    {
      td->lsflag=0;
      td=td->next_data;
    }  
   for(i=0;i<line_num;i++)
    {
	line_sflag0[i]=0;
    }
   for(i=0;i<line_num;i++)
    {
      switch(cons->com_opt)
	{
	case 9:
	  if(line_sflag1[i]==1&&line_sflag2[i]==1)
	    line_sflag0[i]=1;
	  break;
	case 10:
	  if(line_sflag1[i]==1||line_sflag2[i]==1)
	    line_sflag0[i]=1;
	  break;	  
	}

    }
  for(i=0;i<line_num;i++)
    {
      if(line_sflag0[i]==1)
	{
	  td=ts0->data;
	  while(td!=NULL)
	    {
	      if(td->lnum==i)
		{
		  td->lsflag=1;
		  break;
		}
	      td=td->next_data;
	    }
	}
    }
  return ts0;
}


//struct table_s *exits_eval(struct Select_struct *ss,struct table_s *ts);

struct table_s *likecons(struct Conditions *cons,struct table_s *table)
{
  struct table_s *ts;
  ts=table;
  struct t_data *td;
     td=ts->data;	
      while(td!=NULL)
	{ 	    	  
	  if(td->nflag)
	    {
	      goto end_match;
	    }   
	  if(strcmp(td->name,cons->l->value)==0)
	    {
	      int length=strlen(cons->r->value);
	      char s0[length];
	      int i=0;
	      for(i=1;i<length;i++)
		{
		  s0[length]='\0';
		}
	      strcpy(s0,cons->r->value);
	      char s2[length-2];
	      char s1[td->length];
	      strcpy(s1,td->value);
	      int j;
	      j=0;
	      for(i=0;i<length-2;i++)
		{
		  if(s0[i+1]=='\'')
		    {
		      s2[i]='\0';
		      break;
		    }
		  s2[i]=s0[i+1];
		}
	      for(i=0;i<length-2;i++)
		{
		  if(j>=td->length-1)
		    goto end_match;
		  if(s2[i]!='%'&&s2[i]!='_'&&s2[i]!=s1[j])   //不匹配时
		    goto end_match;
		  else 
		    {
		      if(s2[i]=='%')                        //匹配到%时
			{
			  if(i==length-3)
			    goto match_succeed;
			  i++;
			  while(s2[i]!=s1[j])
			    {
			      if(j=td->length-1)
				goto end_match;
			      j++;
			    }
			}		      
		    }	
		  j++;	  
		  if(j!=td->length-1&&i==length-3)
		    goto end_match;
		}
	    match_succeed:td->lsflag=1;
	    }	      
	end_match: td=td->next_data;	    
	}  
      return ts;
}

struct table_s *incons(struct Conditions *cons,struct table_s *table)
{
  struct table_s *ts;
  ts=table;
  struct t_data *td;
      td=ts->data;	
      while(td!=NULL)
	{ 	   	  
	  if(td->nflag)
	    {
	      goto in_end;
	    }
	  if(strcmp(td->name,cons->l->value)==0)   
	    {
	      if(cons->ss==NULL)     //in值列表
		{
		  struct Conditions *val;
		  val=cons->r;
		  int i=0;
		  for(i=0;i<cons->in_num;i++)
		    {
		      if(td->type==0)
			{
			  switch(val->type)
			    {
			    case 3:
			      if(td->num==val->num)
				{
				  td->lsflag=1;
				  goto in_end;
				}
			      break;
			    case 4:
			      if(td->num==val->flo)
				{
				  td->lsflag=1;
				  goto in_end;
				}
			      break;
			    }			      				
			}
		      else
			{
			  if(strcmp(td->value,val->value)==0)
			    {
			      td->lsflag=1;
			      goto in_end;
			    }
			}
		      val=val->r;
		    }
		}
	      else               //in select语句
		{
		  struct t_data *std;
		  std=select_eval(cons->ss);
		  while(std!=NULL)
		    {
		      if(td->type==0)
			{
			  if(td->num==std->num)
			    {
			      td->lsflag=1;
			      goto in_end;
			    }			  
			}
		      else
			{
			  if(strcmp(td->value,std->value)==0)
			    {
			      td->lsflag=1;
			      goto in_end;
			    }
			}
		      std=std->next_data;
		    }
		}
	    }
	in_end: td=td->next_data;
	}
      return ts;
}




struct table_s *othercons(struct Conditions *cons,struct table_s *table)
{
  struct table_s *ts;
  ts=table;
  struct t_data *td;
     {
      struct t_data *std;
      td=ts->data;
      while(td!=NULL)
	{  	  
	  if(td->nflag)
	    {
	      goto ss_end;
	    }
	  if(strcmp(td->name,cons->l->value)==0)        
	    {	      
	      char str0[td->length];
	      char str1[td->length+2];
	      switch(cons->any_flag)              
		{
		case 1:   //与any select语句比较 		   
		  std=select_eval(cons->ss);
		  while(std!=NULL)
		    {
		      switch(cons->com_opt)
			{
			case 1:
			  if(td->num<std->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }		
			  break;
			case 2:
			  if(td->num>std->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }		
			  break;
			case 3:
			  if(td->type)
			    {
			      if(td->num!=std->num)
				{
				  td->lsflag=1;
				  goto ss_end;
				}		
			    }
			  else
			    {
			      if(strcmp(td->value,std->value)!=0)
				{
				  td->lsflag=1;
				  goto ss_end;
				} 
			    }
			  break;
			case 4:
			  if(td->type)
			    {
			      if(td->num==std->num)
				{
				  td->lsflag=1;
				  goto ss_end;
				}		
			    }
			  else
			    {
			      if(!strcmp(td->value,std->value))
				{
				  td->lsflag=1;
				  goto ss_end;
				} 
			    }		
			  break;
			case 5: case 8:
			  if(td->num>=std->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }		
			  break;
			case 6: case 7:
			  if(td->num<=std->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }		
			  break;			  
			}
		      std=std->next_data;
		    }
		  break;
		case 2:                          //与all select语句比较
		  std=select_eval(cons->ss);
		  while(std!=NULL)
		    {
		      switch(cons->com_opt)
			{
			case 1:
			  if(td->num<=std->num)
			    {
			      goto ss_end;
			    }		
			  break;
			case 2:
			  if(td->num<std->num)
			    {
			      goto ss_end;
			    }		
			  break;
			case 3:
			  if(td->type)
			    {
			      if(td->num==std->num)
				{
				  goto ss_end;
				}		
			    }
			  else
			    {
			      if(strcmp(td->value,std->value)==0)
				{
				  goto ss_end;
				} 
			    }		
			  break;
			case 4:
			  if(td->type)
			    {
			      if(td->num!=std->num)
				{
				  goto ss_end;
				}		
			    }
			  else
			    {
			      if(strcmp(td->value,std->value)!=0)
				{
				  goto ss_end;
				} 
			    }	
			  break;
			case 5: case 8:
			  if(td->num<std->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }		
			  break;
			case 6: case 7:
			  if(td->num>std->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }      	
			  break;			  
			}
		      std=std->next_data;
		    }
		  td->lsflag=1;
		  break;
		case 0:
		  switch(cons->r->type)
		    {
		    case 1:                       //与属性列表比较
		      std=ts->data;
		      while(std!=NULL)
			{
			  if(strcmp(cons->r->value,std->name)==0)
				{
				  switch(cons->com_opt)
				    {
				    case 1:
				      if(td->num<std->num)
					{
					  td->lsflag=1;
					  goto ss_end;
					}		
				      break;
				    case 2:
				      if(td->num>std->num)
					{
					  td->lsflag=1;
					  goto ss_end;
					}		
				      break;
				    case 3:
				      if(td->type)
			 		{
					  if(td->num!=std->num)
					    {
					      td->lsflag=1;
					      goto ss_end;
					    }		
					}
				      else
					{
					  if(strcmp(td->value,std->value)!=0)
					    {
					      td->lsflag=1;
					      goto ss_end;
					    } 
					}
				      break;
				    case 4:
				      if(td->type)
					{
					  if(td->num==std->num)
					    {
					      td->lsflag=1;
					      goto ss_end;
					    }		
					}
				      else
					{
					  if(strcmp(td->value,std->value)==0)
					    {
					      td->lsflag=1;
					      goto ss_end;
					    } 
					}		
				      break;
				    case 5: case 8:
				      if(td->num>=std->num)
					{
					  td->lsflag=1;
					  goto ss_end;
					}		
				      break;
				    case 6: case 7:
				      if(td->num<=std->num)
					{
					  td->lsflag=1;
					  goto ss_end;
					}		
				      break;			  
				    }	
				}
			      std=std->next_data;
			}			  
		      break;
		    case 3:		      //与整数常量比较
		      switch(cons->com_opt)
			{
			case 1:
			  if(td->num<cons->r->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }		
			  break;
			case 2:
			  if(td->num>cons->r->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }		
			  break;
			case 3:
			  if(td->num!=cons->r->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    } 			      
			  break;
			case 4:
			  if(td->num==cons->r->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }			
			  break;
			case 5: case 8:
			  if(td->num>=cons->r->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }		
			  break;
			case 6: case 7:
			  if(td->num<=cons->r->num)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }		
			  break;			  
			    }
		      break;
		    case 4:       //与小数常量比较
		      switch(cons->com_opt)
			{
			case 1:
			  if(td->num<cons->r->flo)
				{
				  td->lsflag=1;
				  goto ss_end;
				}		
			  break;
			case 2:
			      if(td->num>cons->r->flo)
				{
				  td->lsflag=1;
				  goto ss_end;
				}		
			      break;
			case 3:
			      if(td->num!=cons->r->flo)
				{
				  td->lsflag=1;
				  goto ss_end;
				}		
			      break;
			case 4:
			  if(td->num==cons->r->flo)
			    {
			      td->lsflag=1;
			      goto ss_end;
				}		
			  break;
			case 5: case 8:
			  if(td->num>=cons->r->flo)
			    {
				  td->lsflag=1;
				  goto ss_end;
			    }		
			  break;
			case 6: case 7:
			      if(td->num<=cons->r->flo)
				{
				  td->lsflag=1;
				  goto ss_end;
				}		
			      break;			  
			}
		      break;
		    case 2: //与字符串比较
		      strcpy(str1,cons->r->value);
		      int t,u;
		      for(t=0;t<td->length;t++)
			{
			  if(str1[t+1]!='\'')
			    str0[t]=str1[t+1];
			  else
			    {
			      u=t;
			      break;
			    }
			}
		      for(t=u;t<td->length;t++)
			{
			  str0[t]='\0';
			}
		      switch(cons->com_opt)
			{
			case 3:
			  if(strcmp(td->value,str0)!=0)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }			  
			  break;
			case 4:
			  if(strcmp(td->value,str0)==0)
			    {
			      td->lsflag=1;
			      goto ss_end;
			    }			  
			break;
			}
		      break;
		    }
		  break;
		}		  
	    }
	ss_end: td=td->next_data;
	}
    }  
     return ts;
}

struct table_s *makelsflag(struct table_s *table)
{
  struct t_data *td;
  struct table_s *ts;
  ts=table;
  td=ts->data;
  int line_num;
  line_num=ts->line_num;
  int line_sflag[line_num];
  while(td!=NULL)
    {
      if(td->lsflag==1)
	line_sflag[td->lnum]=1;
      td=td->next_data;
    }
  td=ts->data;
  int i=0;
  for(i=0;i<line_num;i++)
    {
      if(line_sflag[i]!=1)
	line_sflag[i]=0;
    }
  for(i=0;i<line_num;i++)
    {
      if(line_sflag[i]==1)
	{
	  td=ts->data;
	  while(td!=NULL)
	    {
	      if(td->lnum==i)
		td->lsflag=1;
	      td=td->next_data;
	    }
	}
    }
  return ts;
}




struct table_s *cons_eval(struct Conditions *cons,struct table_s *table)  //计算条件函数
{
  struct table_s *ts;
  struct t_data *td;
  ts=table;
  td=ts->data;

// if(cons->existe_flag==1)  //exists条件
 // exits_eval(ss,table);
  if(cons->null_flag==1)  //NULL条件
    {      
      td=ts->data;	
      while(td!=NULL)
	{	      
	  if(strcmp(td->name,cons->value)==0)
	    {
	      if(td->nflag==1)
		td->lsflag=1;
	    }
	  td=td->next_data;	    
	}      
    }
  if(cons->like_flag==1)  //like条件
    ts=likecons(cons,ts);
  if(cons->in_num>0)         //in条件
    ts=incons(cons,ts);         
  if(cons->com_opt>0&&cons->com_opt!=9&&cons->com_opt!=10) //普通比较条件
    ts=othercons(cons,ts);
  if(cons->not_flag==1)
    {
      td=ts->data;
      while(td!=NULL)
	{
	  td->lsflag=1-td->lsflag;
	  td=td->next_data;
	}
    }
  if(cons->com_opt==9||cons->com_opt==10)       //and、or条件
    {
      ts=ao_eval(cons,ts);  
    }
  return ts;
}

struct t_data *select_result(struct table_s *table)  //生成select结果
{
  struct table_s *ts;
  struct t_data *td,*td2;
  ts=table;
  td=ts->data;
  td2=td;
  int i=0;
  while(td2!=NULL)
    {
      if(td2->rsflag==1)
	{
	  if(!i)
	    {
	      ts->data=td2;
	      td=ts->data;
	      i++;
	    }
	  else
	    {
	      td->next_data=td2;
	      td=td->next_data;
	    }
	  
	}
      td2=td2->next_data;
    }
  td->next_data=NULL;
  td=ts->data;
 return td;
}



struct t_data *select_eval(struct Select_struct *name) //select语句计算
{
  //打开基本数据文件和元数据文件
  FILE *fp1,*fp2;
  char db_name[50],dat_name[50];
  strcpy(db_name,now_db);
  strcpy(dat_name,now_db);
  strcat(db_name,".db");
  strcat(dat_name,".dat");
  fp1=fopen(db_name,"rb");
  fp2=fopen(dat_name,"rb");

  int db_page,dat_page,used_page;  //元数据文件和基本数据文件页数
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
  int table_num;
  struct table_s *table_data;
  struct table_s *ts;
  struct Select_struct *ss;
  struct Select_tables *st;
  struct t_data *td;
  struct Select_fields *sf;
  ss=name;
  st=ss->st;
  int i;
  char tname[20];
  for(i=0;i<db_page;i++)
    {
      fseek(fp1,PAGE_SIZE*i,SEEK_SET);
      fread(tname,20,1,fp1);
      if(strcmp(tname,st->table)==0)
	{
	  table_num=i;
	  break;
	}
    }
  table_data=loadtable(fp1,fp2,table_num);
//处理条件语句
  ts=table_data;
  if(strcmp(ts->table,st->table)==0)
    {	  
      if(ss->cons!=NULL)
	ts=cons_eval(ss->cons,ts);
      else
	{
	  td=ts->data;
	  while(td!=NULL)
	    {
	      td->lsflag=1;
	      td=td->next_data;
	    }
	}
    }	  
  ts=makelsflag(ts);
  ss=name;
  sf=ss->sf;
  while(sf!=NULL)    //处理目标列表达式
    {
      td=ts->data;
      while(td!=NULL)
	{
	  if(td->lsflag==1)
	    {		  
	      if(sf->sall_flag==1)
		{
		  td->rsflag=1;
		}
	      else
		{	  
		  if(strcmp(td->name,sf->field)==0)
		    td->rsflag=1;
		}
	    }
	  td=td->next_data;
	}
      
      sf=sf->next_sf;
    }
  
  struct t_data *result;
  result=select_result(ts);
  //fclose(fp1);
  //fclose(fp2);
  // print_select(result);
  return result;
}

void print_select(struct t_data *td)
{
  char *s1,*s2;
  int d,l;
  struct t_data *ptd;
  ptd=td;
  while(ptd!=NULL)
    {
      if(ptd->rsflag)
	{
	  if(ptd->nflag)
	    { 
	      printf("行号:%d,列名:%s,数据:空项\n",ptd->lnum,ptd->name);
	    }
	  else
	    {	    
	      switch(ptd->type)
		{
		case 0:
		  printf("行号:%d,列名:%s,数据:%d\n",ptd->lnum,ptd->name,ptd->num);
		  break;
		case 1:
		  printf("行号:%d,列名:%s,数据:%s\n",ptd->lnum,ptd->name,ptd->value);
		  break;
		}
	    }
	}
      ptd=ptd->next_data;
    }
}



void delete_eval(struct Delete_struct *ds)
{
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
  struct table_s *table_data;
  int table_num;
  int i;
  char tname[20];
  for(i=0;i<db_page;i++)
    {
      fseek(fp1,PAGE_SIZE*i,SEEK_SET);
      fread(tname,20,1,fp1);
      if(strcmp(tname,ds->table)==0)
	{
	  table_num=i;
	  break;
	}
    }
  table_data=loadtable(fp1,fp2,table_num);
  struct table_s *ts;
  ts=table_data;
  if(ds->cons!=NULL)      
    {
      ts=cons_eval(ds->cons,ts);
      ts=makelsflag(ts);
    }
  dat_page=ts->start_page;
  struct t_data *td;
  td=ts->data;
  while(td!=NULL)
    {
      if(ds->cons!=NULL)
	{
	  if(td->lsflag==1) 
	    {
	      fseek(fp2,dat_page*PAGE_SIZE+td->lnum*td->loffset+td->roffset,SEEK_SET);
	      fwrite("null",1,4,fp2);
	      if(td->length>4)
		fwrite(null,1,td->length-4,fp2);
	    }
	}
      else
	{	 	    
	  fseek(fp2,dat_page*PAGE_SIZE+td->lnum*td->loffset+td->roffset,SEEK_SET);
	  fwrite("null",1,4,fp2);
	  if(td->length>4)
	    fwrite(null,1,td->length-4,fp2);	     
	}
      td=td->next_data;
    }
  fclose(fp1);
  fclose(fp2);
 printf("Delete successed\n");
}





void update_eval(struct Update_struct *us)
{
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
  int table_num;
  struct table_s *table_data;
  struct table_s *ts;
  int i;
  char tname[20];
  for(i=0;i<db_page;i++)
    {
      fseek(fp1,PAGE_SIZE*i,SEEK_SET);
      fread(tname,20,1,fp1);
      if(strcmp(tname,us->table)==0)
	{
	  table_num=i;
	  break;
	}
    }
  table_data=loadtable(fp1,fp2,table_num);
  ts=table_data;
  ts=cons_eval(us->con,ts);
  ts=makelsflag(ts);
  struct t_data *td;
  struct Update_asgn *ua;
  ua=us->ua;
  int length;
   //修改表数据结构中要修改的数据
  
  while (ua!=NULL)
    {
      td=ts->data;
      while(td!=NULL)
	{
	  if(td->lsflag)
	    {
	      if(strcmp(td->name,ua->name)==0)
		{
		  if(ua->con->type==2)
		    {
		      char *s=ua->con->value;
		      length=td->length;
		      char s0[length];
		      char s1[length];
		      int i=0;
		      for(i=0;i<length;i++)
			{
			  s0[i]='\0';
			  s1[i]='\0';
			}
		      strcpy(s0,ua->con->value);
		      for(i=0;i<length;i++)
			{
			  if(s0[i]!='\'')
			    s1[i]=s0[i+1];
			  else
			    break;
			}
		  td->value=s1;
		    }
	      else if(ua->con->type==3)
		td->num=ua->con->num;
	      else if(ua->con->type==4)
		td->num=ua->con->flo;
		  td->rsflag=1;
		}
	    }
	  td=td->next_data;
	}
      ua=ua->next;
    }


  dat_page=ts->start_page;
  int data;
  //写入数据
  td=ts->data;
  while(td!=NULL)
    {
      if(td->rsflag==1)
	{
	  fseek(fp2,dat_page*PAGE_SIZE+td->loffset*td->lnum+td->roffset,SEEK_SET);
	  switch(td->type)
	    {
	    case 0:
	      data=td->num;
	      fwrite(&data,sizeof(int),1,fp2);
	      break;
	    case 1:
	      fwrite(td->value,td->length,1,fp2);
	      break;
	    }
	}
      td=td->next_data;
    }

  fclose(fp1);
  fclose(fp2);
  printf("Update successed\n");  
}



void drop_table(char *name)
{
    //打开基本数据文件和元数据文件
  FILE *fp1;
  char db_name[50];
  strcpy(db_name,now_db);
  strcat(db_name,".db");
  fp1=fopen(db_name,"rb+");
  
  int db_page,dat_page,used_page;  //元数据文件和基本数据文件页数
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
  int i,page;
  char table_name[20];
  for(i=0;i<db_page;i++)
    {
      fseek(fp1,PAGE_SIZE*i,SEEK_SET);
      fread(table_name,20,1,fp1);
      if(strcmp(table_name,name)==0)
	{
	  page=i;
	  break;
	}
      if(i==db_page-1)
	{
	  printf("There is no this table\n");
	  exit(0);
	}
    }
  fseek(fp1,PAGE_SIZE*page,SEEK_SET);
  fwrite(null,1,PAGE_SIZE,fp1);
  fseek(fp1,PAGE_SIZE*(page+1),SEEK_SET);
  fwrite(null,1,PAGE_SIZE,fp1);
  fclose(fp1);
  printf("Drop table successed\n");
}

void show_tables()
{
  //打开基本数据文件和元数据文件
  FILE *fp1;
  char db_name[50];
  strcpy(db_name,now_db);
  strcat(db_name,".db");
  fp1=fopen(db_name,"rb");

  int db_page,used_page;  //元数据文件和基本数据文件页数
  db_page=0;
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
  int table_num=db_page/2;
  int i;
  char s[20];
  for(i=0;i<table_num;i++)
    {
      fseek(fp1,PAGE_SIZE*i*2,SEEK_SET);
      if(fread(s,20,1,fp1)!=1)
	{
	  printf("Error in reading data");
	  exit(0);
	}
      printf("Table: %s\n",s);
    }
  fclose(fp1);
}


void emit(char *s,...)
{
  extern yylineno;
  va_list ap;
  va_start(ap,s);
  
  printf("rpn: ");
  fprintf(stdout,s,ap);
  printf("\n");
}

void yyerror(char *s,...)
{
  extern yylineno;
  
  va_list ap;
  va_start(ap,s);
  
  fprintf(stderr,"%d:error: ",yylineno);
  fprintf(stderr,s,ap);
  fprintf(stderr,"\n");
}

int main(int ac,char **av)
{
  printf("SQL分析器 ver0.1\n");
  strcpy(now_db,xjgl);
  extern FILE *yyin;
  if(ac >1 && !strcmp(av[1],"-d"))
    {
      ac--;av++;
    }
  if(ac>1 &&(yyin=fopen(av[1],"r"))==NULL)
    {
      perror(av[1]);
      exit(1);
    }
  if(!yyparse())
    printf("SQL parse worked\n");
  else
    printf("SQL parse failed\n");
  return yyparse();
}
