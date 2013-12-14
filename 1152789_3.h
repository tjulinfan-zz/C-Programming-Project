#ifndef LF3
#define LF3

void Get_Table(char *);             //读入表名
int Insert_Row(Table * , char *);   //插入行
int Import_Data(char * , char *);   //插入数据 -i过程

Table *Find(char *);                //根据表名查找表并返回表的指针
int getline(char * , FILE *);       //读入一行

#endif