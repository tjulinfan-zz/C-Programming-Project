#ifndef LF1
#define LF1

#define MAXLEN 512          //�ַ�������󳤶�

struct Row_list             //��������һ�е���Ϣ
{
	void ** info;           //һ�е���Ϣ ָ������
	int select;				//�жϸ����Ƿ�ѡ��
	struct Row_list *next;  //���� ��һ���ڵ�
};

typedef struct Row_list Row;

struct Table_list
{
	char *T_name;           //�������
	int NoC;				//����
	int *null;				//�������� �����жϸ����Ƿ���Ϊ��
	int *types;				//�������� ��������ÿһ�е����� ���Ϊ0����float ����Ϊchar�͵���󳤶�
	char **names;			//�����������е�����
	Row *head , *tail;		//����������������� ����ֻ��¼ͷ����β�ڵ�
	struct Table_list *next;//���� ��һ���ڵ�
};

typedef struct Table_list Table;

void Strcopy(char * , char *); //�ѵڶ����ַ����еĶ������Ƶ���һ���ַ�����

void Copyright();			   //�����Ȩ��Ϣ
void Pre();					   //��ӡ "DB=>"
void Get_Path(char *);
void Read_Argv(char *);		   //�������
void Read_Data(FILE * , char *); 
							   //���ļ��ж�������
void Help();				   //������Ϣ
void Exit_pro();			   //�˳�����ʱ�����
void Error();				   //���Ϸ�������
void NoSpace();				   //���û�пռ��򱨴�
void Free();				   //������������ͷ������ڴ�
void Free_Row(Row * , int);	   //�ͷ�������ռ�õ��ڴ�

int Equal(char * , char *);	   //�ж������ַ����Ƿ���ͬ
int Strlen(char *);            //�ַ�������
int Strcmp(char * , char *);   //�ж��ַ����ֵ��� �����һ���ַ������ֵ���Ƚϴ���ô����1���򷵻�0
int Like(char * , char *);     //�ж��Ƿ�Ϊģ��ƥ��
#endif