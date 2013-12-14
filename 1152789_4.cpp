#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "1152789_1.h"
#include "1152789_2.h"
#include "1152789_3.h"
#include "1152789_4.h"

extern int sel_run , up_run , del_run;

Sel *First = NULL , *Last = NULL;

void Swap(Row **a , Row **b)  //由于用Row*数组储存答案 所以该Swap过程用于快排中
{
	Row **t=(Row **)malloc(sizeof(Row *));
	*t = *a;
	*a = *b;
	*b = *t;
	free(t);
}

int compare(int i , int j , int wh , Row **a) //如果小于则返回1 否则返回0
{
	if (wh < 0)
		return (*(double *)(a[i] -> info[-wh - 1]) < *(double *)(a[j] -> info[-wh - 1]));
	else
		return Strcmp((char *)(a[i] -> info[wh]) , (char *)(a[j] -> info[wh]));
	return 0;
}

void Qsort(int h , int t , int wh , Row **a) //快速排序 将a数组按关键字从小到大排序 平均时间复杂度O（nlogn）
{
	int i = h , j = t;
	int mid = (i + j) / 2;
	while (i <= j)
	{ 
		while (compare(i , mid , wh , a)) i ++;
		while (compare(mid , j , wh , a)) j --;
		if (i <= j)
		{
			Swap(a + i , a + j);
			i ++;
			j --;
		}
	}
	if (i < t) Qsort(i , t , wh , a);
	if (j > h) Qsort(h , j , wh , a);
}

int Index(Table *Wh , char *s) //根据列名找出对应的列号
{
	for (int i = 0 ; i < Wh -> NoC ; i ++)
		if (Equal(s , Wh -> names[i])) //s与第i列的列名相同
			return i;
	return -1;
}

//判断是不是符合条件 即like或者严格等于 1表示符合条件 否则返回0
int Run_OK(Row *p , int wh , char *s , int like) 
{
	if (like)
		return Like(s , (char *)(p -> info[wh]));
	else
		return Equal(s , (char *)(p -> info[wh]));
	return 0;
}

void Add(Row *can) //将可行的行添加到储存符合条件的行的链表中
{
	if (First == NULL)
	{
		First = (Sel *)malloc(sizeof(Sel));
		Last = First;
	}
	else
		Last = Last -> next = (Sel *)malloc(sizeof(Sel));
	Last -> point = can;
	Last -> next = NULL;
}

void Change() //将符合筛选条件的行的select值更新
{
	Sel *Tmp = First;
	while (Tmp != NULL)
	{
		Tmp -> point -> select = 1;
		Tmp = Tmp -> next;
	}
}

void Free_Anslist() //释放储存符合条件的链表所占的空间
{
	while (First != NULL)
	{
	    Sel *t = First;
		First = First -> next;
		free(t);
	}
	First = NULL;
	Last = NULL;
}

int Sift(FILE *file , Table *p)
{
	First = NULL;
	Last = NULL;
	int times = 0;
	while (1)
	{
		times ++;
		char *column_name = (char *)malloc(MAXLEN * sizeof(char));
		Read_Data(file , column_name);
		char *sign = (char *)malloc(MAXLEN * sizeof(char));
		Read_Data(file , sign);
		char *column_value = (char *)malloc(MAXLEN * sizeof(char));
		Read_Data(file , column_value);

		int index = Index(p , column_name);
		int type = p -> types[index];
		int like = *sign == '=' ? 0 : 1;

		if (times == 1)
		{
		    Row *h = p -> head;
		    while (h != NULL)
			{
			    if (type == FLOAT)
				{
					if (*(double *)(h -> info[index]) == atof(column_value))
					    Add(h);
				}
				else
					if (Run_OK(h , index , column_value , like))
						Add(h);
				h = h -> next;
			}
		}
		else
		{
			Sel *Tmp = First;
			Sel *Pre = First;
			while (Tmp != NULL)
			{
				Sel *need_free = NULL;
				if ((type == FLOAT && *(double *)(Tmp -> point -> info[index]) != atof(column_value)) || (type != FLOAT && !Run_OK(Tmp -> point , index , column_value , like)))
				{
					if (Tmp == First)
						First = Tmp -> next;
					else
						Pre -> next = Tmp -> next;
					need_free = Tmp;
				}
				Pre = Tmp;
				Tmp = Tmp -> next;
				if (need_free != NULL)
					free(need_free);
			}
		}

		char *logic = (char *)malloc(MAXLEN * sizeof(char));
		*logic = '\0';
		Read_Data(file , logic);
		if (*logic == '\0' || Equal(logic , "order"))
		{
			Change();
			Free_Anslist();
			free(column_name);
			free(sign);
			free(column_value);
			int ch = *logic;
			free(logic);
			return (ch == '\0');
		}
		if (*logic == 'o')
		{
			times = 0;
			Change();
			Free_Anslist();
		}
	}
	return 0;
}

void Print_Float(double output)
{
	if ((int)output == output) //如果是整数的话
		printf("%.0lf" , output);
	else
		printf("%lf" , output);
}

void Print_Row(Row *a , Table *p , int *Bool , int tot)
{
	//Row *a为要输出的行
	for (int i = 0 ; i < p -> NoC ; i ++)
		if (Bool[i])
		{
			if (p -> types[i] == FLOAT)
				Print_Float(*(double *)(a -> info[i]));
			else
				printf("%s" , (char *)(a -> info[i]));
			if (--tot) //如果不是最后要输出的列则输出'，’否则输出换行
				printf(",");
			else
				printf("\n");
		}
}

void Print(Row **a , int order , int n , int top , Table *p , int *Bool , int tot)
{
	//order 0 升序 1 降序
	if (!order)
		for (int i = 1 ; i <= top ; i ++)
			Print_Row(*(a + i) , p , Bool , tot);
	else
		for (int i = 1 ; i <= top ; i ++ , n --)
			Print_Row(*(a + n) , p , Bool , tot);
}

int Select(char *filepath)   //-s过程
{
	clock_t st_time = clock(); //过程开始的时间
	FILE *file = fopen(filepath , "r");
    if (file == NULL) //如果文件没有打开
	{
		printf("---The file does not exist.---\n");
		return -1;
	}

	char *s = (char *)malloc(MAXLEN * sizeof(char));
	if (s == NULL)
	{
		NoSpace(); fclose(file);
		return -1;
	}
	char *s_bac = s;
	char *need = (char *)malloc(MAXLEN * sizeof(char));
	if (need == NULL)
	{
		NoSpace(); free(s_bac); fclose(file);
		return -1;
	}
	char *need_bac = need;
	Read_Data(file , s);    //read "select"
	Read_Data(file , s);
	int top = -1 , all = 0;
	if (*s == 't')
	{
		//如果输入了“top” top表示后面跟的参数
		Read_Data(file , s);    top = atoi(s);
		Read_Data(file , s);
	}
	//s表示输入的“*”或者“{column_names}”
	if (*s == '*')
		all = 1;
		//是“*”的话标记全部
	else
		Strcopy(need , s);  //column names
		//否则将这个字符串复制到need中

	Read_Data(file , s);      //read "from"
	char *table_name = (char *)malloc(MAXLEN * sizeof(char));
	if (table_name == NULL)
	{
		NoSpace(); free(s_bac); free(need_bac); fclose(file);
		return -1;
	}
	Read_Data(file , table_name);  //读入表名
	Table *tmp = Find(table_name); //找出表的位置
	if (tmp == NULL)
	{
		printf("---The table does not exist.---\n");
		free(s); free(need_bac); fclose(file);
		return -1;
	}
	free(table_name);

	int *Bool = (int *)malloc(tmp -> NoC * sizeof(int)); 
	//bool数组用来储存该列是否需要输出
	if (Bool == NULL)
	{
		NoSpace(); free(s_bac); free(need_bac); fclose(file);
		return -1;
	}
	int *Bool_bac = Bool;
	for (int i = 0 ; i < tmp -> NoC ; i ++)
		Bool[i] = all;
	//如果all为1这是一个全选的过程
	if (!all)
	{
		need ++; //忽略掉开头的'{'
		//need表示读入的"{cloumn_names}"
		//s字符串表示读入的列名
	    for (s = s_bac ; *need != '\0' ; need ++)
		{
			if (*need == ',' || *need == '}') //如果碰到‘，’或者‘}’就说明已经读入了一个列名了
			{
				*s = '\0';
				int i = Index(tmp , s_bac);       //找出该列的标号
				if (i < 0 || i > tmp -> NoC)  //如果超出范围
				{
		            free(s_bac); free(need_bac); free(Bool_bac); fclose(file);
					printf("---The column '%s' does not exist.---\n" , s);
					return -1;
				}
				Bool[i] = 1;                  //否则将该列标记为需要输出
				s = s_bac;                    //将s清空
			}
			else
				*(s ++) = *need;              
		}
	}
	free(need_bac);

	int order = -1 , sort_key = -1;   //order判断是升序降序或者乱序 sort_key表示哪一列为关键字排序
	s = s_bac;
	*s = '\0';
	Read_Data(file , s);   //read "where"
	if (*s == EOF)
	{
		//如果没有读入where 则选择所有行
		Row *p = tmp -> head;
		while (p != NULL)
		{
			p -> select = 1;
			p = p -> next;
		}
	}
	else
	{
		int ord = Sift(file , tmp);  //读取筛选数据的那一行
		*s = '\0';
		if (ord)
		    Read_Data(file , s);     //read "order"
		else
			*s = 'o';
		if (*s == 'o')
		{
			order = 0;
			Read_Data(file , s); //"by"
			Read_Data(file , s); //column_name
			int i = Index(tmp , s);
			if (i == -1)
			{
				free(s_bac); free(Bool_bac); fclose(file);
				printf("---The column '%s' does not exist.---\n" , s);
				return -1;
		    }
			sort_key = i;        //读入排序关键列
			*s = '\0';
			Read_Data(file , s); //"ASC" or "DESC"
			if (*s == 'D')
				order = 1;
		}
	}
	free(s_bac);

    Row **Ans = (Row **)malloc(10000 * sizeof(Row*)); //Row **Ans用来储存筛选出来的行的地址
	if (Ans == NULL)
	{
		NoSpace(); free(Bool_bac); fclose(file);
		return -1;
	}
	int n = 0;

	//将筛选出来的行全部加入Ans数组以便于排序
	Row *p = tmp -> head;
	while (p != NULL)
	{
		if (p -> select == 1)
		{
			Ans[++ n] = p;
			p -> select = 0;
		}
		p = p -> next;
	}
		 
	//order： 0升序输出 1降序输出
	if (order != -1)
	{
		if (tmp -> types[sort_key] == FLOAT)  
			sort_key = - sort_key - 1;
		//sort_key为了便于比较 如果是该列是浮点数的话做一次转化 比较的时候判断如果是负数就解码
	    Qsort(1 , n , sort_key , Ans);
	}
	else
		order = 0;

	//tot为总共输出的列数
	int tot = 0;
	for (int i = 0 ; i < tmp -> NoC ; i ++)
		tot += Bool[i];
	if (top > n || top == -1)
		top = n;
	fclose(file);

	printf("---Select Completed!---\n");
	char *fout = (char *)malloc(MAXLEN * sizeof(char));
	*fout = '\0';
	sprintf(fout , "1152789_select_%d.txt" , ++sel_run);
	//fout为输出文件 sel_run为运行select过程的次数
	printf("---%d Records Selected, Results in %s---\n" , top , fout);
	//top即为输出的行数

    //重定向到fout中
	freopen(fout , "w" , stdout);
	clock_t ed_time = clock(); //结束时间
	printf("Execute Time: %0.lf ms\n" , (double)(ed_time - st_time) / CLOCKS_PER_SEC * 1000);
	printf("Execute Result:\n");
	Print(Ans , order , n , top , tmp , Bool , tot);
	/*////////////////////////////////////////////////////////
	//  Print过程完成输出
	//  order    0 升序 1 降序 
	//  n Ans    数组中的行数 
	//  top      需要输出的行数
	//  tmp      这些操作所在的表
	//  Bool     用来储存哪些列需要输出
	//  tot      需要输出的总列数
	*/////////////////////////////////////////////////////////
	free(Bool_bac);
	free(Ans);
	free(fout);
	freopen("CON" , "w" , stdout);
	return 0;
}

/*
  update过程和delete过程都是基于select过程中的筛选过程
  于是我将这三个过程写到了一个文件中
*/

int Update(char *filepath)
{
	clock_t st_time = clock();
	FILE *file = fopen(filepath , "r");
	if (file == NULL) //如果文件没有打开
	{
		printf("---The file does not exist.---\n");
		return -1;
	}
	char *s = (char *)malloc(MAXLEN * sizeof(char));
	Read_Data(file , s);  //"update"

	char *table_name = (char *)malloc(MAXLEN * sizeof(char));
	Read_Data(file , table_name);
	Table *table = Find(table_name);
	//读入表名并找到它的位置
	free(table_name);
	if (table == NULL)
	{
		printf("---The table does not exist.---\n");
		return -1;
	}

	Read_Data(file , s);  //"set"

	char *column_name = (char *)malloc(MAXLEN * sizeof(char));
	Read_Data(file , column_name);  //需要更新的列 index代表其列号
	int index = Index(table , column_name);
	if (index == -1)
	{
		printf("---The column '%s' does not exist.---\n" , column_name);
		free(column_name);
		return -1;
	}
	free(column_name);

	Read_Data(file , s);  //"="
	char *column_value = (char *)malloc(MAXLEN * sizeof(char));
	Read_Data(file , column_value);

	int all = 0;
	*s = '\0';
	Read_Data(file , s);
	if (*s == 'w') //如果读入了“where”
	    Sift(file , table);
	else
		all = 1;   //否则全选

	double num = -1;
	int tp = table -> types[index];
	if (tp == FLOAT)
	    num = atof(column_value);
	//如果需要更新的列储存的数据是浮点类型的话 num代表更新之后的值

	int records = 0;    //records代表更新的行数
	//更新过程
    Row *p = table -> head;
	while (p != NULL)
	{
		if (p -> select || all) //如果全选或者该行被选中
		{
			if (tp == FLOAT)
			    *(double *)(p -> info[index]) = num;
			else
				Strcopy((char *)(p -> info[index]) , column_value);
			records ++;
		}
		p = p -> next;
	}
	free(s);
	free(column_value);
	fclose(file);
	printf("---Update completed!---\n");
	char *fout = (char *)malloc(MAXLEN * sizeof(char));
	*fout = '\0';
	sprintf(fout , "1152789_update_%d.txt" , ++up_run);
	//fout为输出的文件名
	printf("---%d Records Updated, Results in %s---\n" , records , fout);

	//重定向到fout中
	freopen(fout , "w" , stdout);
	clock_t ed_time = clock();
	printf("Execute Time: %0.lf ms\n" , (double)(ed_time - st_time) / CLOCKS_PER_SEC * 1000);
	printf("Execute Result:\n");

	//上面那个步骤是为了统计时间 接下来再做一遍以便输出
	int *Bool = (int *)malloc(table -> NoC * sizeof(int));
	for (int i = 0 ; i < table -> NoC ; i ++)
		Bool[i] = 1;
	p = table -> head;
	while (p != NULL)
	{
		if (p -> select || all)
		{
			Print_Row(p , table , Bool , table -> NoC);
			p -> select = 0;
		}
		p = p -> next;
	}
	freopen("CON" , "w" , stdout);
	free(fout);
	free(Bool);
	return 0;
}


//delete过程基本和update过程相同 不明白的地方参见update过程
int Delete(char *filepath)
{
	clock_t st_time = clock();
	FILE *file = fopen(filepath , "r");
	if (file == NULL) //如果文件没有打开
	{
		printf("---The file does not exist.---\n");
		return -1;
	}
	char *s = (char *)malloc(MAXLEN * sizeof(char));
	Read_Data(file , s); //"delete"
	Read_Data(file , s); //"from"
	Read_Data(file , s);
	Table *table = Find(s);  //删除操作所在的表
	if (table == NULL)
	{
		printf("---The table does not exist.---\n");
		return -1;
	}
	*s = '\0';
	Read_Data(file , s);
	int all = 0;
	if (*s == 'w')   //如果读入了“where”
		Sift(file , table);
	else
		all = 1;

	int records = 0;   //记录有多少行被删除
	Row *p = table -> head;
	Row *pre = p;
	while (p != NULL)
	{
		Row *need_free = NULL;
		if (p -> select || all)
		{
			need_free = p;
			if (p == table -> head)
				table -> head = p -> next;
			else
				pre -> next = p -> next;
		}
		p = p -> next;
		if (need_free != NULL)
		{
		    Free_Row(need_free , table -> NoC);
			records ++;
		}
	}
	fclose(file);
	printf("---Delete completed!---\n");
	char *fout = (char *)malloc(MAXLEN * sizeof(char));
	*fout = '\0';
	sprintf(fout , "1152789_delete_%d.txt" , ++del_run);
	printf("---%d Records Deleted, Results in %s---\n" , records , fout);

	freopen(fout , "w" , stdout);
	clock_t ed_time = clock();
	printf("Execute Time: %0.lf ms\n" , (double)(ed_time - st_time) / CLOCKS_PER_SEC * 1000);
	printf("Execute Result:\n");
	int *Bool = (int *)malloc(table -> NoC * sizeof(int));
	for (int i = 0 ; i < table -> NoC ; i ++)
		Bool[i] = 1;
	p = table -> head;
	while (p != NULL)
	{
		Print_Row(p , table , Bool , table -> NoC);
		p = p -> next;
	}
	free(Bool);
	freopen("CON" , "w" , stdout);
	return 0;
}