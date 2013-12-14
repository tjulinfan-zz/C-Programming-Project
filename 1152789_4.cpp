#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "1152789_1.h"
#include "1152789_2.h"
#include "1152789_3.h"
#include "1152789_4.h"

extern int sel_run , up_run , del_run;

Sel *First = NULL , *Last = NULL;

void Swap(Row **a , Row **b)  //������Row*���鴢��� ���Ը�Swap�������ڿ�����
{
	Row **t=(Row **)malloc(sizeof(Row *));
	*t = *a;
	*a = *b;
	*b = *t;
	free(t);
}

int compare(int i , int j , int wh , Row **a) //���С���򷵻�1 ���򷵻�0
{
	if (wh < 0)
		return (*(double *)(a[i] -> info[-wh - 1]) < *(double *)(a[j] -> info[-wh - 1]));
	else
		return Strcmp((char *)(a[i] -> info[wh]) , (char *)(a[j] -> info[wh]));
	return 0;
}

void Qsort(int h , int t , int wh , Row **a) //�������� ��a���鰴�ؼ��ִ�С�������� ƽ��ʱ�临�Ӷ�O��nlogn��
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

int Index(Table *Wh , char *s) //���������ҳ���Ӧ���к�
{
	for (int i = 0 ; i < Wh -> NoC ; i ++)
		if (Equal(s , Wh -> names[i])) //s���i�е�������ͬ
			return i;
	return -1;
}

//�ж��ǲ��Ƿ������� ��like�����ϸ���� 1��ʾ�������� ���򷵻�0
int Run_OK(Row *p , int wh , char *s , int like) 
{
	if (like)
		return Like(s , (char *)(p -> info[wh]));
	else
		return Equal(s , (char *)(p -> info[wh]));
	return 0;
}

void Add(Row *can) //�����е�����ӵ���������������е�������
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

void Change() //������ɸѡ�������е�selectֵ����
{
	Sel *Tmp = First;
	while (Tmp != NULL)
	{
		Tmp -> point -> select = 1;
		Tmp = Tmp -> next;
	}
}

void Free_Anslist() //�ͷŴ������������������ռ�Ŀռ�
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
	if ((int)output == output) //����������Ļ�
		printf("%.0lf" , output);
	else
		printf("%lf" , output);
}

void Print_Row(Row *a , Table *p , int *Bool , int tot)
{
	//Row *aΪҪ�������
	for (int i = 0 ; i < p -> NoC ; i ++)
		if (Bool[i])
		{
			if (p -> types[i] == FLOAT)
				Print_Float(*(double *)(a -> info[i]));
			else
				printf("%s" , (char *)(a -> info[i]));
			if (--tot) //����������Ҫ������������'���������������
				printf(",");
			else
				printf("\n");
		}
}

void Print(Row **a , int order , int n , int top , Table *p , int *Bool , int tot)
{
	//order 0 ���� 1 ����
	if (!order)
		for (int i = 1 ; i <= top ; i ++)
			Print_Row(*(a + i) , p , Bool , tot);
	else
		for (int i = 1 ; i <= top ; i ++ , n --)
			Print_Row(*(a + n) , p , Bool , tot);
}

int Select(char *filepath)   //-s����
{
	clock_t st_time = clock(); //���̿�ʼ��ʱ��
	FILE *file = fopen(filepath , "r");
    if (file == NULL) //����ļ�û�д�
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
		//��������ˡ�top�� top��ʾ������Ĳ���
		Read_Data(file , s);    top = atoi(s);
		Read_Data(file , s);
	}
	//s��ʾ����ġ�*�����ߡ�{column_names}��
	if (*s == '*')
		all = 1;
		//�ǡ�*���Ļ����ȫ��
	else
		Strcopy(need , s);  //column names
		//��������ַ������Ƶ�need��

	Read_Data(file , s);      //read "from"
	char *table_name = (char *)malloc(MAXLEN * sizeof(char));
	if (table_name == NULL)
	{
		NoSpace(); free(s_bac); free(need_bac); fclose(file);
		return -1;
	}
	Read_Data(file , table_name);  //�������
	Table *tmp = Find(table_name); //�ҳ����λ��
	if (tmp == NULL)
	{
		printf("---The table does not exist.---\n");
		free(s); free(need_bac); fclose(file);
		return -1;
	}
	free(table_name);

	int *Bool = (int *)malloc(tmp -> NoC * sizeof(int)); 
	//bool����������������Ƿ���Ҫ���
	if (Bool == NULL)
	{
		NoSpace(); free(s_bac); free(need_bac); fclose(file);
		return -1;
	}
	int *Bool_bac = Bool;
	for (int i = 0 ; i < tmp -> NoC ; i ++)
		Bool[i] = all;
	//���allΪ1����һ��ȫѡ�Ĺ���
	if (!all)
	{
		need ++; //���Ե���ͷ��'{'
		//need��ʾ�����"{cloumn_names}"
		//s�ַ�����ʾ���������
	    for (s = s_bac ; *need != '\0' ; need ++)
		{
			if (*need == ',' || *need == '}') //����������������ߡ�}����˵���Ѿ�������һ��������
			{
				*s = '\0';
				int i = Index(tmp , s_bac);       //�ҳ����еı��
				if (i < 0 || i > tmp -> NoC)  //���������Χ
				{
		            free(s_bac); free(need_bac); free(Bool_bac); fclose(file);
					printf("---The column '%s' does not exist.---\n" , s);
					return -1;
				}
				Bool[i] = 1;                  //���򽫸��б��Ϊ��Ҫ���
				s = s_bac;                    //��s���
			}
			else
				*(s ++) = *need;              
		}
	}
	free(need_bac);

	int order = -1 , sort_key = -1;   //order�ж���������������� sort_key��ʾ��һ��Ϊ�ؼ�������
	s = s_bac;
	*s = '\0';
	Read_Data(file , s);   //read "where"
	if (*s == EOF)
	{
		//���û�ж���where ��ѡ��������
		Row *p = tmp -> head;
		while (p != NULL)
		{
			p -> select = 1;
			p = p -> next;
		}
	}
	else
	{
		int ord = Sift(file , tmp);  //��ȡɸѡ���ݵ���һ��
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
			sort_key = i;        //��������ؼ���
			*s = '\0';
			Read_Data(file , s); //"ASC" or "DESC"
			if (*s == 'D')
				order = 1;
		}
	}
	free(s_bac);

    Row **Ans = (Row **)malloc(10000 * sizeof(Row*)); //Row **Ans��������ɸѡ�������еĵ�ַ
	if (Ans == NULL)
	{
		NoSpace(); free(Bool_bac); fclose(file);
		return -1;
	}
	int n = 0;

	//��ɸѡ��������ȫ������Ans�����Ա�������
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
		 
	//order�� 0������� 1�������
	if (order != -1)
	{
		if (tmp -> types[sort_key] == FLOAT)  
			sort_key = - sort_key - 1;
		//sort_keyΪ�˱��ڱȽ� ����Ǹ����Ǹ������Ļ���һ��ת�� �Ƚϵ�ʱ���ж�����Ǹ����ͽ���
	    Qsort(1 , n , sort_key , Ans);
	}
	else
		order = 0;

	//totΪ�ܹ����������
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
	//foutΪ����ļ� sel_runΪ����select���̵Ĵ���
	printf("---%d Records Selected, Results in %s---\n" , top , fout);
	//top��Ϊ���������

    //�ض���fout��
	freopen(fout , "w" , stdout);
	clock_t ed_time = clock(); //����ʱ��
	printf("Execute Time: %0.lf ms\n" , (double)(ed_time - st_time) / CLOCKS_PER_SEC * 1000);
	printf("Execute Result:\n");
	Print(Ans , order , n , top , tmp , Bool , tot);
	/*////////////////////////////////////////////////////////
	//  Print����������
	//  order    0 ���� 1 ���� 
	//  n Ans    �����е����� 
	//  top      ��Ҫ���������
	//  tmp      ��Щ�������ڵı�
	//  Bool     ����������Щ����Ҫ���
	//  tot      ��Ҫ�����������
	*/////////////////////////////////////////////////////////
	free(Bool_bac);
	free(Ans);
	free(fout);
	freopen("CON" , "w" , stdout);
	return 0;
}

/*
  update���̺�delete���̶��ǻ���select�����е�ɸѡ����
  �����ҽ�����������д����һ���ļ���
*/

int Update(char *filepath)
{
	clock_t st_time = clock();
	FILE *file = fopen(filepath , "r");
	if (file == NULL) //����ļ�û�д�
	{
		printf("---The file does not exist.---\n");
		return -1;
	}
	char *s = (char *)malloc(MAXLEN * sizeof(char));
	Read_Data(file , s);  //"update"

	char *table_name = (char *)malloc(MAXLEN * sizeof(char));
	Read_Data(file , table_name);
	Table *table = Find(table_name);
	//����������ҵ�����λ��
	free(table_name);
	if (table == NULL)
	{
		printf("---The table does not exist.---\n");
		return -1;
	}

	Read_Data(file , s);  //"set"

	char *column_name = (char *)malloc(MAXLEN * sizeof(char));
	Read_Data(file , column_name);  //��Ҫ���µ��� index�������к�
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
	if (*s == 'w') //��������ˡ�where��
	    Sift(file , table);
	else
		all = 1;   //����ȫѡ

	double num = -1;
	int tp = table -> types[index];
	if (tp == FLOAT)
	    num = atof(column_value);
	//�����Ҫ���µ��д���������Ǹ������͵Ļ� num�������֮���ֵ

	int records = 0;    //records������µ�����
	//���¹���
    Row *p = table -> head;
	while (p != NULL)
	{
		if (p -> select || all) //���ȫѡ���߸��б�ѡ��
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
	//foutΪ������ļ���
	printf("---%d Records Updated, Results in %s---\n" , records , fout);

	//�ض���fout��
	freopen(fout , "w" , stdout);
	clock_t ed_time = clock();
	printf("Execute Time: %0.lf ms\n" , (double)(ed_time - st_time) / CLOCKS_PER_SEC * 1000);
	printf("Execute Result:\n");

	//�����Ǹ�������Ϊ��ͳ��ʱ�� ����������һ���Ա����
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


//delete���̻�����update������ͬ �����׵ĵط��μ�update����
int Delete(char *filepath)
{
	clock_t st_time = clock();
	FILE *file = fopen(filepath , "r");
	if (file == NULL) //����ļ�û�д�
	{
		printf("---The file does not exist.---\n");
		return -1;
	}
	char *s = (char *)malloc(MAXLEN * sizeof(char));
	Read_Data(file , s); //"delete"
	Read_Data(file , s); //"from"
	Read_Data(file , s);
	Table *table = Find(s);  //ɾ���������ڵı�
	if (table == NULL)
	{
		printf("---The table does not exist.---\n");
		return -1;
	}
	*s = '\0';
	Read_Data(file , s);
	int all = 0;
	if (*s == 'w')   //��������ˡ�where��
		Sift(file , table);
	else
		all = 1;

	int records = 0;   //��¼�ж����б�ɾ��
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