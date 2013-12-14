#include <stdio.h>
#include <stdlib.h>
#include "1152789_1.h"
#include "1152789_2.h"
#include "1152789_3.h"
#include "1152789_4.h"

Table *Head = NULL , *Tail = NULL;            //��������table�������ͷ�����β�ڵ�
int sel_run = 0 , up_run = 0 , del_run = 0;   //select update delete�����б��

int main()
{
	Copyright();  //���������Ϣ

	char *s = (char *)malloc(MAXLEN * sizeof(char));         //����������������
	char *backups_s = s;                                     //����sָ�� �����ͷ��ڴ�
    char *filepath = (char *)malloc(MAXLEN * sizeof(char));  //���������ļ�·��
	char *backups_p = filepath;
	while (1)
	{
        Pre();
		s = backups_s;
		Read_Argv(s);
		if (*s != '-')
		{
			gets(s);
			Error();
			continue;
		}
		s ++;
		filepath = backups_p;
        
		if (Equal(s , "index"))
		{
		    int t;
			scanf("%d" , &t);
			if (t == 1)
			    printf("---Open the index function successfully.---\n");
			else
				if (t == 0)
					printf("---Close the index function successfully.---\n");
				else
					Error();
			gets(s);
			continue;
		}
		else
			if (*(s + 1) != '\0')
			{
				gets(s);
				Error();
				continue;
			}
		switch (*s) 
		{
		    case 'c' : Get_Path(filepath);
				       Create_Table(filepath);
					   break;

			case 'i' : s = backups_s;   
					   Get_Table(s);        //�ڶ������� ����
				       Get_Path(filepath);
					   Import_Data(s , filepath);
					   break;

			case 's' : Get_Path(filepath);
					   Select(filepath);
					   break;

			case 'u' : Get_Path(filepath);
					   Update(filepath);
				       break;

			case 'd' : Get_Path(filepath);
					   Delete(filepath);
					   break;

			case 'h' : Help();
				       break;

		    case 'q' : Exit_pro();
					   free(backups_s);
					   free(backups_p);
					   Free();
				       return 0;

			default  : gets(s);
				       Error();
				       break;
		}
		gets(s);
	}

	return 0;
}