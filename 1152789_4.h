#ifndef LF4
#define LF4

#define NOT_FIND -1

struct store_ans             //��������select����������Ϣ
{
	Row *point;              //ָ����е�ָ���ַ
	struct store_ans *next;  //���� ��һ���ڵ�
};

typedef struct store_ans Sel;

int Index(Table * , char *); //���������ҳ����ǵڼ���
int Sift(FILE * , Table *);  //ɸѡ����

int Select(char *);
int Update(char *);
int Delete(char *);

#endif