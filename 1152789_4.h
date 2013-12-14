#ifndef LF4
#define LF4

#define NOT_FIND -1

struct store_ans             //用来储存select出来的行信息
{
	Row *point;              //指向的行的指针地址
	struct store_ans *next;  //链表 下一个节点
};

typedef struct store_ans Sel;

int Index(Table * , char *); //根据列名找出它是第几列
int Sift(FILE * , Table *);  //筛选过程

int Select(char *);
int Update(char *);
int Delete(char *);

#endif