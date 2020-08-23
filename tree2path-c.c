#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ASCII	99999
#define printline()	printf("%s",line)
#define printpath()	printf("%s",path)
#define printname()	printf("%s\n",name)
#define printfpath()	write(destfile,path,path_len)
#define printffile()	write(destfile,file,file_len)
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
/*
ANSI�ļ����룬ʵ������Ŀ¼���ļ�·����ת��
*/

FILE *srcfile = NULL;
//FILE *destfile = NULL;
int line_cnt = 0;
int error_line_cnt = 1;
int error_flag = 0;
int dir_cnt = 0;
int dir_cnt_old = 1;
char line[MAX_ASCII];
char path[MAX_ASCII] = "/";
char name[MAX_ASCII];
char name_old[MAX_ASCII];

//��·��ƴ������
//δ���
void path_add(char* dir_name ,int len)
{
	strcat(path,dir_name);
	//printf("path+:%s+%d+%d+\n",path,path_len,strlen(path));
} 

void path_del()
{
  //printf("path:%s,",path);
  int len = strlen(path) - 1;
	while(path[--len] != '/')
	{
		if(len <= 0)
		{
			error_flag = 1;//���ִ���ֱ�ӽ�������
			return;
		}
		path[len] = '\0';
	}
	//printf("%s\n",path);
	return ;
}


//"��",169,192
//"��",169,166
//"��",169,184
//"��",169,164
//��ȡ�ļ����ļ������֣�����ɡ�
void get_name(unsigned char* _line,unsigned char* _name)
{
  int dir_flag = 0;
  int space_cnt = 0; 
  //���ɵ�ǰ����ַ�
  while(*_line != '\0')
  {
    //�������ļ������� 
    if( ((*_line == 169) && *(_line+1) == 166)	//"��",169,166 
    || ((*_line == 169) && *(_line+1) == 164))	//"��",169,164
    {
      _line += 2;
    }
    //�����ļ������� 
    else if( ((*_line == 169) && *(_line+1) == 192)	//"��",169,192
    ||	     ((*_line == 169) && *(_line+1) == 184))	//"��",169,184
    {
      dir_flag = 1;
      _line += 2;
    }
    else if((*_line ==  32))
    {
      ++_line;
      ++space_cnt;
    }
    else
    {
      //ǰ��ֻ�ܳ��� "��""��"/"��""��"" "��Щ�ַ���������Ϊǰ���ַ����� 
      break;
    }
  }
  //printf("space_cnt:%d\t",space_cnt);
  //printf("cnt :%d\n",cnt);
  //printf("%d,%d,%d,%d,%d,%\n",*line,*(line+1),*(line+2),*(line+3),*(line+4),*(line+5));
  //ǰ���ַ��������������С��ļ��б�ʶ����������������Ϊ���ָ�����ɡ�
  while( *_line != '\n'
  //&& *_line != '/'
  && *_line != '\0' )
  {
    *_name++ = *_line++;
  }
} 

//��ȡ��ǰ�ļ��в���
//δ���
unsigned char get_dir_cnt(unsigned char* _line)
{
  int _dir_cnt = 0;
  while(*_line != '\0')
  {
    if(	((*_line == 169) && *(_line+1) == 192)	//"��",169,192 
    ||  ((*_line == 169) && *(_line+1) == 166)  //"��",169,166
    ||	((*_line == 169) && *(_line+1) == 184))	//"��",169,184
    {
      ++_dir_cnt;
    }
    ++_line;
  }
  return _dir_cnt-1;
}

void dir_cnt_deal()
{
    dir_cnt_old = dir_cnt;
    dir_cnt = get_dir_cnt(line);

    if(dir_cnt > dir_cnt_old)
    {
      path_add(name_old,0);
    }
    if(dir_cnt < dir_cnt_old)
    {
      int diff = dir_cnt_old- dir_cnt;
      while(diff--)
      {
        path_del();
      }
    }
}


//ÿһ�н��д���
//δ��ɡ�
int line_deal(char* _line)
{
    line_cnt ++;
    get_name(_line,name);
    dir_cnt_deal();

    //printf("%d  ",dir_cnt);
    printf("%s",path);
    printf("%s",name);
    //printf("%s",name_old);
    //printf("line:%s",line);
    printf("\n");
    //calc_line(line);
}

int file_deal(char* file)
{
  char* ret;
  unsigned char dir_flag;
  //srcfile = fopen(argv[1],"r");//ֻ����ʽ��tree�ļ�
  srcfile = fopen("./src_false.txt","r");
  //destfile = fopen("2019-.txt","w+");//ֻд��ʽ���ļ�
  while(!feof(srcfile))
  {
    memcpy(name_old,name,sizeof(name));
    memset(line,0,strlen(line));
    memset(name,0,strlen(name));
    ret = fgets(line,MAX_ASCII,srcfile);
    if(ret == NULL)//�������ļ�ĩβ������ѭ��
    {
      break;
    }
    line_deal(line);
  }
  fclose(srcfile);
  //fclose(destfile);
  printf("�ļ���%d�У�����·����%d�С�\n",line_cnt,error_line_cnt);
}





int main(int argc, char *argv[])
{
//	if(argc <= 1)
//	{
//		printf("no file to input,program will exit.\n");
//		return 0;
//	}
	file_deal(argv[1]);
  return 0;

}
