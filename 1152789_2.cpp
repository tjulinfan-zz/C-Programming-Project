#include <stdio.h>
#include <stdlib.h>
#include "1152789_1.h"
#include "1152789_2.h"

extern Table *Head , *Tail;

int Insert_Column(FILE *file , Table *t)  //����ÿһ��
{
	int index = t -> NoC;
	//�е�λ�ù̶��� �������±�����ʾ

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
    //��ȡ��һ���֣��е�����
	
	char *st = (char *)malloc(MAXLEN * sizeof(char));
	char *st_bac = st;
	Read_Data(file , s);
	if (*s == 'f')
		t -> types[index] = FLOAT;
	    //�����float�͵� ��ôtypes[index]Ϊ0
	else
	{
		//����������֮����ַ���ת�����ֺ󴢴�����
		while (*s != '(') s++;
		for (s ++ ; *s != ')' ; s ++ , st ++)
			*st = *s;
		*st = '\0';
		t -> types[index] = atoi(st_bac) + 1;
		//�����char�͵� types[index]��ʾ��󳤶�
	}
	free(st_bac);
	//��ȡ�ڶ����֣���������

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
	//�������� �ɿջ��߲��ɿ�	

	t -> NoC ++;
	return 1;
}

int Create_Table(char *filepath) //�������
{
	FILE *file = fopen(filepath , "r"); //���ļ�
	if (file == NULL)				    //����ļ�û�д�
	{
		printf("---The file does not exist.---\n");
		return -1;
	}  

	char *table_name = (char *)malloc(MAXLEN * sizeof(char)); //�����������
	if (table_name == NULL)
	{
		NoSpace();
		return -1;
	}
	
	Read_Data(file , table_name);  //read "create"
	Read_Data(file , table_name);  //read "table"
	Read_Data(file , table_name);  //�������
	int len = Strlen(table_name) + 1;

    //��������
	if (Head == NULL)     
		Tail = Head = (Table *)malloc(sizeof(Table));  //���ͷ���Ϊ�� ��ô�¿��ٳ�һ���ڵ�
	else
		Tail = Tail -> next = (Table *)malloc(sizeof(Table)); //����ӵ�β�ڵ�֮�� ������β�ڵ�
	Tail -> T_name = (char *)malloc(len * sizeof(char)); //Ϊ���������ڴ�
	if (Tail -> T_name == NULL)
	{
		NoSpace();
		return -1;
	}
	Strcopy(Tail -> T_name , table_name); 
    free(table_name);						//����ʱ������copy���� ���ͷ���ʱ�������ڴ�

	int Num = 0;           //Num��������������е�����
	for (int ch = getc(file) ; ch != EOF ; ch = getc(file))
		if (ch == ';')     //��ͬ���á���������
			Num ++;

	//�¿��ٵĽڵ���ֳ�ʼ��
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
	Tail -> types = (int *)malloc(Num * sizeof(int));   //Ϊ���ֱ��������ڴ��Լ���ʼ��
	if (Tail -> types == NULL)
	{
		NoSpace();
		return -1;
	}

	rewind(file);  //���õ��ļ�ͷ
	char *s = (char *)malloc(MAXLEN * sizeof(char));
	for (int i = 0 ; i < 4 ; i ++)
		Read_Data(file , s);
	free(s);       //����"create table table_name ��"
	while (1)
	{
	    if (!Insert_Column(file , Tail))  //����в���ÿһ��
			break;
	}
	fclose(file);

	printf("---Create Table Completed!---\n");
	return 0;
}