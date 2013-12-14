#include <stdio.h>
#include <stdlib.h>
#include "1152789_1.h"

extern Table *Head;

void Pre()
{
	printf("\nDB=>");
}

void Get_Path(char *s)     //读入文件路径
{
	scanf("%s" , s);
}

void Read_Argv(char *s)    //读入参数
{
    scanf("%s" , s);
}

void Read_Data(FILE * file , char *s)  //从文件中读入数据
{
	fscanf(file , "%s" , s);
}

void Copyright()
{
	printf("\t****************************************\n");
	printf("\t*    Tiny In-Mem DataBase version 0.1  *\n");
	printf("\t*   Copyright 林凡 All Rights Reserved *\n");
	printf("\t****************************************\n");
	printf("请使用格式合法的数据，谢谢！\n");
}

void Help() //-h
{
	printf("a) 格式： -c 文件路径名\n");
	printf("   作用： 根据指定位置的文件内语句建立一个数据库表\n");

	printf("b) 格式： -i 数据库表名 数据文件路径名\n");
	printf("   作用： 从指定文本文件中导入数据到数据库的表中\n"); 

	printf("c) 格式： -s 文件路径名\n");
	printf("   作用:  从指定文本文件中读取语句执行查询操作\n");

	printf("d) 格式： -u 文件路径名\n");
	printf("   作用： 从指定文本文件中读取语句执行更新操作\n");

	printf("e) 格式： -d 文件路径名\n");
	printf("   作用： 从指定文本文件中读取语句执行删除操作\n");

	printf("f) 格式： -h\n");
	printf("   作用： 帮助信息，提示各个参数的格式和含义\n");

	printf("g) 格式： -q\n");
	printf("   作用： 退出程序\n");

	printf("h) 格式： -index 0/1\n");
	printf("   作用： 0表示关闭索引功能，1表示打开索引功能\n");
}

void Exit_pro()
{
	printf("---Bye!---\n");
}

void Error()
{
	printf("---Error---\n");
	printf("---Please enter '-h' for help.---\n");
}

//some functions of string
void Strcopy(char *s , char *t)  //把字符串t中的内容copy到s中
{
	for ( ; *t != '\0' ; t ++ , s++)
		*s = *t;
	*s = '\0';
}

void NoSpace() //没有空间
{
	printf("There is no space\n");
}

void Free_Row(Row *r , int index) //释放一行所占的内存
{
	for (int i = 0 ; i < index ; i ++)
		free(r -> info[i]);
	free(r -> info);
}

void Free_Table(Table *t) //释放一个表所占的内存
{
	Row *p = t -> head;
	while (p != NULL)
	{
		Row *tmp = p -> next;
		Free_Row(p , t -> NoC);
		p = tmp;
	}
	free(t -> null);
	free(t -> names);
	free(t -> types);
}

void Free() //释放table链表所占的内存
{ 
	Table *p = Head;
	while (p != NULL)
	{
		Table *tmp = p -> next;
		Free_Table(p);
		p = tmp;
	}
}

int Equal(char *s , char *t) //判断字符串s和t是否相同
{
	for ( ; *s != '\0' && *t != '\0' ; s ++ , t ++)
		if (*s != *t)
			return 0;
	if (*s != '\0' || *t != '\0') 
		return 0;
	return 1;
}

int Strlen(char *s)          //返回字符串长度
{
	int len = 0;
	while (*(s ++) != '\0')
		len ++;
	return len;
}

int Strcmp(char *f , char *s) //if f < s then Strcmp = 1 else Strcmp = 0
{
	for ( ; *f != '\0' && *s != '\0' ; f ++ , s ++)
		if (*f < *s)
			return 1;
		else
		    if (*s < *f)
				return 0;
	if (*s == '\0') return 0;
	return 1;
}

int Like(char *s1 , char *s2) //if s1 likes s2 then return 1 else ruturn 0
{
	char *bac = s1;
	for ( ; *s2 != '\0' ; s2 ++)
	{
		char *st = s2;
		for (s1 = bac ; *s1 != '\0' && *st != '\0' ; s1 ++ , st ++)
			if (*st != *s1) 
				break;
		if (*s1 == '\0') return 1;
	}
	return 0;
}