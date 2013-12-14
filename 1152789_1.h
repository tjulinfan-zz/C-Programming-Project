#ifndef LF1
#define LF1

#define MAXLEN 512          //字符串的最大长度

struct Row_list             //用来储存一行的信息
{
	void ** info;           //一行的信息 指针数组
	int select;				//判断该行是否被选定
	struct Row_list *next;  //链表 下一个节点
};

typedef struct Row_list Row;

struct Table_list
{
	char *T_name;           //储存表名
	int NoC;				//列数
	int *null;				//整形数组 用来判断该列是否能为空
	int *types;				//整形数组 用来储存每一列的类型 如果为0代表float 否则为char型的最大长度
	char **names;			//用来储存所有的列名
	Row *head , *tail;		//行是用链表来储存的 这里只记录头结点和尾节点
	struct Table_list *next;//链表 下一个节点
};

typedef struct Table_list Table;

void Strcopy(char * , char *); //把第二个字符串中的东西复制到第一个字符串中

void Copyright();			   //输出版权信息
void Pre();					   //打印 "DB=>"
void Get_Path(char *);
void Read_Argv(char *);		   //读入参数
void Read_Data(FILE * , char *); 
							   //从文件中读入数据
void Help();				   //帮助信息
void Exit_pro();			   //退出程序时的输出
void Error();				   //不合法的输入
void NoSpace();				   //如果没有空间则报错
void Free();				   //程序运行完后释放所有内存
void Free_Row(Row * , int);	   //释放行链表占用的内存

int Equal(char * , char *);	   //判断两个字符串是否相同
int Strlen(char *);            //字符串长度
int Strcmp(char * , char *);   //判断字符串字典序 如果第一个字符串的字典序比较大那么返回1否则返回0
int Like(char * , char *);     //判断是否为模糊匹配
#endif