#include <stdio.h>
#include <stdlib.h>
#include "1152789_1.h"
#include "1152789_2.h"
#include "1152789_3.h"

extern Table *Head , *Tail;

void Get_Table(char *table_name)  //读入表名
{
	scanf("%s" , table_name);
}

int Insert_Row(Table *Wh , char *rData)
{
	Row *pre = Wh -> tail;
	Row *p = NULL;
	if (Wh -> head == NULL)
		p = Wh -> head = (Row *)malloc(sizeof(Row));
	else
		p = Wh -> tail -> next = (Row *)malloc(sizeof(Row));;
	Wh -> tail = p;
	if (Wh -> tail == NULL)
	{
		NoSpace();
		return -1;
	}

	p -> next = NULL;
	p -> info = (void **)malloc(Wh -> NoC * sizeof(void *)); //指针数组 用来储存每一行的数据
	if (p -> info == NULL)
	{
		NoSpace();
		return -1;
	}

	for (int i = 0 ; i < Wh -> NoC ; i ++)
	{
		if (Wh -> types[i] == FLOAT)
			p -> info[i] = (void *)malloc(sizeof(double));
		else
			p -> info[i] = (void *)malloc(Wh ->types[i] * sizeof(char));
		if (p -> info[i] == NULL)
		{
			NoSpace();
			return -1;
		}
	}

	p -> select = 0;
	char *tmp = (char *)malloc(MAXLEN * sizeof(char));
	if (tmp == NULL)
	{
		NoSpace();
		return -1;
	}
	char *tmp_bac = tmp;
	int index = 0;
	for ( ; *rData != '\0' ; rData ++)  //每碰到一个逗号就处理这个区间的数据
	{
		if (*rData == ',' || *rData == '\n')
		{
			*tmp = '\0';
			if (*tmp_bac == '\0' && Wh -> null[index] == NOT_NULL)  //如果该列值为空 但是建表时要求不能为空 则error
			{
				if (pre == NULL)
					Wh -> head = NULL;
				else
					Wh -> tail = pre;
				printf("---The column '%s' can not be empty.---\n" , Wh -> names[index]);
				return -1;
			}
			if (Wh -> types[index] == 0)
				*((double *)(p -> info[index])) = atof(tmp_bac);
			else
				Strcopy((char *)(p -> info[index]) , tmp_bac);
			index ++;
			tmp = tmp_bac;
		}
		else
			*(tmp ++) = *rData;
	}
	return 0;
}

Table *Find(char *table_name)
{
	Table *p = Head;
	while (p != NULL)
	{
		if (Equal(p -> T_name , table_name))
			return p;
		p = p -> next;
	}
	return NULL;
}

int getline(char *s , FILE *file)
{
	int ch = getc(file) , len = 0;
	while (ch != '\n' && ch != EOF)
	{
		len ++;
		*(s ++) = ch;
		ch = getc(file);
	}
	*s = ',';
	*(++ s) = '\0';
	return len;
}

int Import_Data(char *table_name , char *filepath)
{
	FILE *file = fopen(filepath , "r");
	//read the data from this file
	if (file == NULL) //如果文件没有打开
	{
		printf("---The file does not exist.---\n");
		return -1;
	}  

	Table *Wh = Find(table_name);  //找到该表的地址
	if (Wh == NULL)
	{
		printf("---The table does not exist.---\n");
		return -1;
	}

	int records = 0;
	char *rData = (char *)malloc(MAXLEN * sizeof(char));
	while (getline(rData , file))     //每读入一行就处理一行
	{
		records ++;
	    int tmp = Insert_Row(Wh , rData); 
		if (tmp == -1) 
		{
			free(rData);
			fclose(file);
			return -1;
		}
	}
	free(rData);
	fclose(file);

	printf("---Import Data Completed!---\n");
	printf("---%d Records Imported Successfully---\n" , records);
	return 0;
}