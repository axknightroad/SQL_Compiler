/*
 *语法结构定义与声明
 *by axknightroad
 *2013/05/22
*/


/*
 *抽象语法树中的节点
 */

/*
  enum exp_type{name,string,intnum,bool,apprpxnun} 
  
  struct exp{
  char *exp_field;
  enum exp_type type;
  }
*/
char now_db[50]; //当前数据库

/* creat类型 */


struct Create_field{ /* creat语句中的字段定义 */
  char *field; //字段名称
  int type; //字段类型,0是int，1是字符串
  int  length; //字段长度
};

struct Create_fields { /* creat语句中的字段组定义 */
  struct Create_field *cf; //此字段
  struct Create_fields *next_cf; //下一字段
};

struct Create_struct{ /* creat 语法树根节点 */
  char *table;  //基本表名称
  struct Create_fields *cf; //字段定义 
};

/*通用字段定义*/

struct Conditions{ /* 通用字段定义 */
  struct Conditions *l;         //左部字段
  struct Conditions *r;         //右部字段
  int com_opt;          //COMPARISON，and是9，or是10，其余同词法
  int  type;             //0是字段，1是名字，2是字符串，3是整数，4是小数，5是BOOL
  char *value;           //根据type存放字段名、字符串
  int  num;              //存放整数
  float flo;             //存放小数
  int boolnum;           //存放布尔值
  char *table;           //NULL或表名
  int  not_flag;         //判断是否为not，1表示是not
  int  any_flag;         //判断select语句带条件语句，1是any/some，2是all
  int  null_flag;        //1是null
  int  like_flag;        //1是like
  int  exists_flag;      //1是exists
  int  fun_flag;         //判断select语句中函数，1是count函数
  int  all_flag;         //当select为*时,1是*
  int  in_num;           //当是in语句时，表达式数量，0表示非in语句
};



/*select类型*/

struct Val_list_struct{ /*val_list结构*/
  struct conditions *val_con;  //字段
  int  val_num;           //计数
};


struct Select_fields{ /*select 语句中选中的字段 */
  char *table;           //字段所属表
  char *field;           //字段名称
  struct Select_fields *next_sf; //下一个字段
  int sall_flag;          //当列名是*时
};


struct  Select_tables{ /*select语句中选中的表*/	
  char *table;		//基本表名称
  struct Select_tables *next_st; //下一个表
};


struct Groupby{ /*select语句中Groupby语句*/
  struct Conditions *gr_con;   //条件
  struct Groupby    *next_gr;  //下一个语句
  int sc_flag;        //升降序标志位,0为ASC，升序
};


struct	Select_struct{/*select语法树的根节点*/		
  int select_flag;              //可选项 ALL|DISTINCT
  struct Select_fields  *sf;	//所选字段
  struct Select_tables	*st;	//所选基本表
  struct Conditions	*cons;	//条件
  struct Groupby        *gb;    //Groupby
  struct Conditions     *ha;    //Having
  struct Groupby        *ob;    //Orderby
  struct Conditions     *rc;    //select选出的结果
};

/*delete类型*/
struct Delete_struct{/*delete语法树的根节点*/
  char *table;    //所选基本表
  struct Conditions    *cons; //条件
};
  
/*insert类型*/
struct Column_list_struct{/*字段名列表*/
  char *name;   //名称
  struct Column_list_struct *next_cl; //下一个Column_list
};

struct Insert_val_list_struct{/*属性值列表*/
  struct Conditions *cons; //字段
  struct Insert_val_list_struct *next_ivl; //下一个Insert_val_list
};

struct Insert_struct{/*insert语法树的根节点*/
  char *table;     //表名
  struct Column_list_struct *cl; //属性列
  struct Insert_val_list_struct *ivl; //属性值
};

/*update类型*/
struct Update_asgn{//update的列名及表达式
  char *name;   //列名
  struct Conditions *con; //表达式
};
  
struct Update_struct{/*update语法树的根节点*/
  char *table;   //表名
  struct Update_asgn *ua; //列名及表达式
  struct Conditions *con; //条件
};


/*表字典类型*/
struct table_dictionary {
  char table[20];  //表名
  int  start_page; //起始页
}


/*
*构造抽象语法树
*/

/*构造exp类型*/
// struct exp *new_exp(char *exp_field,enum exp_type type);

/*构造create语句中字段的语法树*/
struct Create_field *new_create_field(char *field,int type,int  length);

/*构造create语句中字段组的语法树*/
struct Create_fields *new_create_fields(struct Create_field *cf,struct Create_fields *next_cf);

/*构造create语法树的根节点*/
struct Create_struct *new_create_struct(char *table,struct Create_fields *cf);

/*构造select语法树的根节点的语法树*/
struct Select_struct *new_select_struct(int Select_flag,struct Select_fields *sf,struct Select_tables *st,struct Conditions *cons,struct Groupby *gb,struct Conditions *ha,struct Groupby *ob,struct Conditions *rc);

/*构造select语句条件字段的语法树*/
struct Conditions *new_conditions(struct Conditions *l,struct Conditions *r,int com_opt,int  type,char *value,int num,float flo,int boolnum,char *table,int not_flag,int any_flag,int null_flag,int like_flag,int exists_flag,int fun_flag,int all_flag,int in_num);

/*构造select 语句中选中的字段的语法树*/
struct Select_fields *new_select_fields(char *table,char *field,struct Select_fields *next_sf,int sall_flag);

/*构造select语句中选中的表的语法树*/
struct Select_tables *new_select_tables(char *table,struct Select_tables *next_st);

/*构造select语句中Groupby语句的语法树*/
struct Groupby *new_groupby(struct Conditions *gr_con,struct Groupby *next_gr,int sc_flag);

/*构造val_list结构的语法树*/
struct Val_list_struct *new_val_list_struct(struct Conditions *val_con,int val_num);

/*构造delete语法根节点树*/
struct Delete_struct *new_delete_struct(char *table,struct Conditions *cons);

/*构造字段名列表语法树*/
struct Column_list_struct *new_column_list_struct(char *name,struct Column_list_struct *next_cl);

/*构造Insert语句中值列表语法树*/
struct Insert_val_list_struct *new_insert_val_list_struct(struct Conditions *cons,struct Insert_val_list_struct *next_ivl);

/*构造Insert语法树根节点*/
struct Insert_struct *new_insert_struct(char *table,struct Column_list_struct *cl,struct Insert_val_list_struct *ivl);

/*构造update语句中列名及表达式语法树*/
struct Update_asgn *new_update_asgn(char *name,struct Conditions *con);

/*构造update语法树的根节点*/  
struct Update_struct *new_update_struct(char *table,struct Update_asgn *ua,struct Conditions *con);

/*
 * 函数声明
 */
void select_eval(struct Select_struct *name);

struct Conditions result_con(struct Select_struct *name);

void delete_eval(struct Delete_struct *name);

void insert_eval(struct Insert_struct *name);

void update_eval(struct Update_struct *name);

void select_free(struct Select_struct *name);

void create_database(char *name);

void create_table(struct Creat_struct *t_name);

void drop_database(char *name);

void drop_table(char *name);

void show_talbe();
