#include <stdio.h>
#include <stdlib.h>
#include "1152789_1.h"
#include "1152789_2.h"

extern Table *Head , *Tail;

int Insert_Column(FILE *file , Table *t)  //插入每一列
{
	int index = t -> NoC;
	//列的位置固定了 所以用下标来表示

	char *s = (char *)malloc(MAXLEN * sizeof(char));
	char *s_bac = s;
	Read_Data(file , s);
	if (*s == ')') 
	{
		free(s_bac);
		return 0;
	}
	t -> names[index] = (char *)malloc((Strlen(s) + 1) * sizeof(char));
	Strcopy(t -> names[index] , s);
    //提取第一部分：列的名字
	
	char *st = (char *)malloc(MAXLEN * sizeof(char));
	char *st_bac = st;
	Read_Data(file , s);
	if (*s == 'f')
		t -> types[index] = FLOAT;
	    //如果是float型的 那么types[index]为0
	else
	{
		//将“（）”之间的字符串转成数字后储存下来
		while (*s != '(') s++;
		for (s ++ ; *s != ')' ; s ++ , st ++)
			*st = *s;
		*st = '\0';
		t -> types[index] = atoi(st_bac) + 1;
		//如果是char型的 types[index]表示最大长度
	}
	free(st_bac);
	//提取第二部分：数据类型

	s = s_bac;
	Read_Data(file , s);
	if (*(s + 1) == 'O')
	{
		t -> null[index] = 1;
		Read_Data(file , s);
	}
	else
		t -> null[index] = 0;
	free(s_bac);
	//第三部分 可空或者不可空	

	t -> NoC ++;
	return 1;
}

int Create_Table(char *filepath) //建表过程
{
	FILE *file = fopen(filepath , "r"); //打开文件
	if (file == NULL)				    //如果文件没有打开
	{
		printf("---The file does not exist.---\n");
		return -1;
	}  

	char *table_name = (char *)malloc(MAXLEN * sizeof(char)); //用来储存表名
	if (table_name == NULL)
	{
		NoSpace();
		return -1;
	}
	
	Read_Data(file , table_name);  //read "create"
	Read_Data(file , table_name);  //read "table"
	Read_Data(file , table_name);  //读入表名
	int len = Strlen(table_name) + 1;

    //接入链表
	if (Head == NULL)     
		Tail = Head = (Table *)malloc(sizeof(Table));  //如果头结点为空 那么新开辟出一个节点
	else
		Tail = Tail -> next = (Table *)malloc(sizeof(Table)); //否则接到尾节点之后 并更新尾节点
	Tail -> T_name = (char *)malloc(len * sizeof(char)); //为表名开辟内存
	if (Tail -> T_name == NULL)
	{
		NoSpace();
		return -1;
	}
	Strcopy(Tail -> T_name , table_name); 
    free(table_name);						//从临时变量中copy表名 并释放临时变量的内存

	int Num = 0;           //Num变量用来储存表中的列数
	for (int ch = getc(file) ; ch != EOF ; ch = getc(file))
		if (ch == ';')     //不同列用“；”隔开
			Num ++;

	//新开辟的节点各种初始化
	Tail -> NoC = 0;
	Tail -> next = NULL;
	Tail -> head = NULL;
	Tail -> tail = NULL;
	Tail -> null = (int *)malloc(Num * sizeof(int));
	if (Tail -> null == NULL)
	{
		NoSpace();
		return -1;
	}
	Tail -> names = (char **)malloc(Num * sizeof(char *));
	if (Tail -> names == NULL)
	{
		NoSpace();
		return -1;
	}
	Tail -> types = (int *)malloc(Num * sizeof(int));   //为各种变量开辟内存以及初始化
	if (Tail -> types == NULL)
	{
		NoSpace();
		return -1;
	}

	rewind(file);  //重置到文件头
	char *s = (char *)malloc(MAXLEN * sizeof(char));
	for (int i = 0 ; i < 4 ; i ++)
		Read_Data(file , s);
	free(s);       //读入"create table table_name （"
	while (1)
	{
	    if (!Insert_Column(file , Tail))  //向表中插入每一列
			break;
	}
	fclose(file);

	printf("---Create Table Completed!---\n");
	return 0;
}