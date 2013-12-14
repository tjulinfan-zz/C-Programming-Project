#include <stdio.h>
#include <stdlib.h>
#include "1152789_1.h"

extern Table *Head;

void Pre()
{
	printf("\nDB=>");
}

void Get_Path(char *s)     //�����ļ�·��
{
	scanf("%s" , s);
}

void Read_Argv(char *s)    //�������
{
    scanf("%s" , s);
}

void Read_Data(FILE * file , char *s)  //���ļ��ж�������
{
	fscanf(file , "%s" , s);
}

void Copyright()
{
	printf("\t****************************************\n");
	printf("\t*    Tiny In-Mem DataBase version 0.1  *\n");
	printf("\t*   Copyright �ַ� All Rights Reserved *\n");
	printf("\t****************************************\n");
	printf("��ʹ�ø�ʽ�Ϸ������ݣ�лл��\n");
}

void Help() //-h
{
	printf("a) ��ʽ�� -c �ļ�·����\n");
	printf("   ���ã� ����ָ��λ�õ��ļ�����佨��һ�����ݿ��\n");

	printf("b) ��ʽ�� -i ���ݿ���� �����ļ�·����\n");
	printf("   ���ã� ��ָ���ı��ļ��е������ݵ����ݿ�ı���\n"); 

	printf("c) ��ʽ�� -s �ļ�·����\n");
	printf("   ����:  ��ָ���ı��ļ��ж�ȡ���ִ�в�ѯ����\n");

	printf("d) ��ʽ�� -u �ļ�·����\n");
	printf("   ���ã� ��ָ���ı��ļ��ж�ȡ���ִ�и��²���\n");

	printf("e) ��ʽ�� -d �ļ�·����\n");
	printf("   ���ã� ��ָ���ı��ļ��ж�ȡ���ִ��ɾ������\n");

	printf("f) ��ʽ�� -h\n");
	printf("   ���ã� ������Ϣ����ʾ���������ĸ�ʽ�ͺ���\n");

	printf("g) ��ʽ�� -q\n");
	printf("   ���ã� �˳�����\n");

	printf("h) ��ʽ�� -index 0/1\n");
	printf("   ���ã� 0��ʾ�ر��������ܣ�1��ʾ����������\n");
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
void Strcopy(char *s , char *t)  //���ַ���t�е�����copy��s��
{
	for ( ; *t != '\0' ; t ++ , s++)
		*s = *t;
	*s = '\0';
}

void NoSpace() //û�пռ�
{
	printf("There is no space\n");
}

void Free_Row(Row *r , int index) //�ͷ�һ����ռ���ڴ�
{
	for (int i = 0 ; i < index ; i ++)
		free(r -> info[i]);
	free(r -> info);
}

void Free_Table(Table *t) //�ͷ�һ������ռ���ڴ�
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

void Free() //�ͷ�table������ռ���ڴ�
{ 
	Table *p = Head;
	while (p != NULL)
	{
		Table *tmp = p -> next;
		Free_Table(p);
		p = tmp;
	}
}

int Equal(char *s , char *t) //�ж��ַ���s��t�Ƿ���ͬ
{
	for ( ; *s != '\0' && *t != '\0' ; s ++ , t ++)
		if (*s != *t)
			return 0;
	if (*s != '\0' || *t != '\0') 
		return 0;
	return 1;
}

int Strlen(char *s)          //�����ַ�������
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